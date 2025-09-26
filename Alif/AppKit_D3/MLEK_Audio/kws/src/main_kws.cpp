/*
 * SPDX-FileCopyrightText: Copyright 2025 Arm Limited and/or its
 * affiliates <open-source-office@arm.com>
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * This keyword spotting example is intended to work with the
 * CMSIS pack produced by ml-embedded-eval-kit. The pack consists
 * of platform agnostic end-to-end ML use case API's that can be
 * used to construct ML examples for any target that can support
 * the memory requirements for TensorFlow Lite Micro framework and
 * some heap for the API runtime.
 */
#include <cstdint>
#include <string>
#include <vector>

#include "AudioUtils.hpp"
#include "AudioSource.hpp"      /* Interface to audio data array */

#include "BufAttributes.hpp"    /* Buffer attributes to be applied */
#include "Classifier.hpp"       /* Classifier for the result */
#include "KwsProcessing.hpp"    /* Pre and Post Process */
#include "KwsResult.hpp"        /* KWS results class */
#include "Labels.hpp"           /* Label Data for the model */
#include "MicroNetKwsModel.hpp" /* Model API */

#include "cmsis_os2.h"          /* CMSIS-RTOS2 API */

/* Platform dependent files */
#include "main.h"
#include "log_macros.h"      /* Logging macros (optional) */

namespace arm {
namespace app {
    /* Tensor arena buffer */
    static uint8_t tensorArena[ACTIVATION_BUF_SZ] ACTIVATION_BUF_ATTRIBUTE;

    /* Optional getter function for the model pointer and its size. */
    namespace kws {
        extern uint8_t* GetModelPointer();
        extern size_t GetModelLen();
    } /* namespace kws */
} /* namespace app */
} /* namespace arm */

void app_main_thread(void *arg)
{
    /* Model object creation and initialisation. */
    arm::app::MicroNetKwsModel model;
    if (!model.Init(arm::app::tensorArena,
                    sizeof(arm::app::tensorArena),
                    arm::app::kws::GetModelPointer(),
                    arm::app::kws::GetModelLen())) {
        printf_err("Failed to initialise model\n");
        return;
    }

    constexpr int minTensorDims = static_cast<int>(
        (arm::app::MicroNetKwsModel::ms_inputRowsIdx > arm::app::MicroNetKwsModel::ms_inputColsIdx)
            ? arm::app::MicroNetKwsModel::ms_inputRowsIdx
            : arm::app::MicroNetKwsModel::ms_inputColsIdx);

    const auto mfccFrameLength = 640;
    const auto mfccFrameStride = 320;
    const auto scoreThreshold  = 0.7;

    /* Get Input and Output tensors for pre/post processing. */
    TfLiteTensor* inputTensor  = model.GetInputTensor(0);
    TfLiteTensor* outputTensor = model.GetOutputTensor(0);
    if (!inputTensor->dims) {
        printf_err("Invalid input tensor dims\n");
        return;
    } else if (inputTensor->dims->size < minTensorDims) {
        printf_err("Input tensor dimension should be >= %d\n", minTensorDims);
        return;
    }

    /* Get input shape for feature extraction. */
    TfLiteIntArray* inputShape     = model.GetInputShape(0);
    const uint32_t numMfccFeatures = inputShape->data[arm::app::MicroNetKwsModel::ms_inputColsIdx];
    const uint32_t numMfccFrames   = inputShape->data[arm::app::MicroNetKwsModel::ms_inputRowsIdx];

    /* We expect to be sampling 1 second worth of data at a time.
     * NOTE: This is only used for time stamp calculation. */
    const float secondsPerSample = 1.0 / arm::app::audio::MicroNetKwsMFCC::ms_defaultSamplingFreq;

    /* Classifier object for results */
    arm::app::KwsClassifier classifier;

    /* Object to hold label strings. */
    std::vector<std::string> labels;

    /* Declare a container to hold results from across the whole audio clip. */
    std::vector<arm::app::kws::KwsResult> finalResults;

    /* Object to hold classification results */
    std::vector<arm::app::ClassificationResult> singleInfResult;

    /* Populate the labels here. */
    GetLabelsVector(labels);

    /* Set up pre and post-processing. */
    arm::app::KwsPreProcess preProcess = arm::app::KwsPreProcess(
        inputTensor, numMfccFeatures, numMfccFrames, mfccFrameLength, mfccFrameStride);

    arm::app::KwsPostProcess postProcess =
        arm::app::KwsPostProcess(outputTensor, classifier, labels, singleInfResult);

    uint32_t file_idx{0};
    uint32_t inferenceCount{0};
    std::string lastValidKeywordDetected{};

    while (open_audio_source(file_idx)) {

        debug("Using audio data from %s\n", get_audio_name(file_idx));

        /* Creating a sliding window through the whole audio clip. */
        auto audioDataSlider = arm::app::audio::SlidingWindow<const int16_t>(get_audio_array(file_idx),
                                                                             get_audio_array_size(file_idx),
                                                                             preProcess.m_audioDataWindowSize,
                                                                             preProcess.m_audioDataStride);
        close_audio_source(file_idx++);

        /* Reset sliding window position */
        audioDataSlider.Reset();

        while (audioDataSlider.HasNext()) {
            const int16_t* inferenceWindow = audioDataSlider.Next();

            /* Run the pre-processing, inference and post-processing. */
            if (!preProcess.DoPreProcess(inferenceWindow, audioDataSlider.Index())) {
                printf_err("Pre-processing failed.");
                return;
            }

            info("Inference #: %" PRIu32 "\n", ++inferenceCount);

            if (!model.RunInference()) {
                printf_err("Inference failed.");
                return;
            }

            if (!postProcess.DoPostProcess()) {
                printf_err("Post-processing failed.");
                return;
            }

            /* Add results from this window to our final results vector. */
            finalResults.emplace_back(arm::app::kws::KwsResult(
                singleInfResult,
                audioDataSlider.Index() * secondsPerSample * preProcess.m_audioDataStride,
                audioDataSlider.Index(),
                scoreThreshold));
        }

        for (const auto& result : finalResults) {

            std::string topKeyword{"<none>"};
            float score = 0.f;
            if (!result.m_resultVec.empty()) {
                topKeyword = result.m_resultVec[0].m_label;
                score      = result.m_resultVec[0].m_normalisedVal;

                if (topKeyword != "<none>" && topKeyword != "_unknown_") {

                    if (lastValidKeywordDetected != topKeyword) {
                        /* Update last keyword. */
                        lastValidKeywordDetected = topKeyword;
                        info("Detected: %s; Prob: %0.2f\n", topKeyword.c_str(), score);
                        std::string dispStr = " Last Keyword: " + topKeyword;
                    }
                }
            }
        }

        finalResults.clear();
    }
}

/* Application initialization */
int app_main (void) {
    const osThreadAttr_t attr = {
        .stack_size = 4096,
    };
    /* Create application thread and start the kernel */
    osThreadNew(app_main_thread, NULL, &attr);
    osKernelStart();
    return 0;
}
