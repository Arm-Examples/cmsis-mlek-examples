/*
 * SPDX-FileCopyrightText: Copyright 2021-2024 Arm Limited and/or its
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
 * This object detection example is intended to work with the
 * CMSIS pack produced by ml-embedded-eval-kit. The pack consists
 * of platform agnostic end-to-end ML use case API's that can be
 * used to construct ML examples for any target that can support
 * the memory requirements for TensorFlow Lite Micro framework and
 * some heap for the API runtime.
 */
#include "BufAttributes.hpp" /* Buffer attributes to be applied */
#include "Classifier.hpp"    /* Classifier for the result */
#include "DetectionResult.hpp"
#include "DetectorPostProcessing.hpp" /* Post Process */
#include "DetectorPreProcessing.hpp"  /* Pre Process */
#include "VideoSource.hpp"
#include "YoloFastestModel.hpp"       /* Model API */

#include "cmsis_os2.h"                /* ::CMSIS:RTOS2 */

/* Platform dependent files */
#include "main.h"
#include "log_macros.h"      /* Logging macros (optional) */

namespace arm {
namespace app {
    /* Tensor arena buffer */
    static uint8_t tensorArena[ACTIVATION_BUF_SZ] ACTIVATION_BUF_ATTRIBUTE;

    /* Optional getter function for the model pointer and its size. */
    namespace object_detection {
        extern uint8_t* GetModelPointer();
        extern size_t GetModelLen();
    } /* namespace object_detection */
} /* namespace app */
} /* namespace arm */

void app_main_thread(void *arg)
{
    /* Model object creation and initialisation. */
    arm::app::YoloFastestModel model;
    if (!model.Init(arm::app::tensorArena,
                    sizeof(arm::app::tensorArena),
                    arm::app::object_detection::GetModelPointer(),
                    arm::app::object_detection::GetModelLen())) {
        printf_err("Failed to initialise model\n");
        return;
    }

    auto initialImgIdx = 0;

    TfLiteTensor* inputTensor   = model.GetInputTensor(0);
    TfLiteTensor* outputTensor0 = model.GetOutputTensor(0);
    TfLiteTensor* outputTensor1 = model.GetOutputTensor(1);

    if (!inputTensor->dims) {
        printf_err("Invalid input tensor dims\n");
        return;
    } else if (inputTensor->dims->size < 3) {
        printf_err("Input tensor dimension should be >= 3\n");
        return;
    }

    TfLiteIntArray* inputShape = model.GetInputShape(0);

    const int inputImgCols = inputShape->data[arm::app::YoloFastestModel::ms_inputColsIdx];
    const int inputImgRows = inputShape->data[arm::app::YoloFastestModel::ms_inputRowsIdx];

    /* Set up pre and post-processing. */
    arm::app::DetectorPreProcess preProcess =
        arm::app::DetectorPreProcess(inputTensor, true, model.IsDataSigned());

    std::vector<arm::app::object_detection::DetectionResult> results;
    const arm::app::object_detection::PostProcessParams postProcessParams{
        inputImgRows,
        inputImgCols,
        arm::app::object_detection::originalImageSize,
        arm::app::object_detection::anchor1,
        arm::app::object_detection::anchor2};
    arm::app::DetectorPostProcess postProcess =
        arm::app::DetectorPostProcess(outputTensor0, outputTensor1, results, postProcessParams);

    auto dstPtr = static_cast<uint8_t*>(inputTensor->data.uint8);

    uint32_t img_idx = 0;
    size_t img_sz;

    void *rgbFrame;
    const uint8_t *img_buf;

    while (open_img_source(img_idx)) {
        results.clear();

        img_buf = get_img_array(img_idx);
        img_sz  = get_img_array_size(img_idx);

        /* Run the pre-processing, inference and post-processing. */
        if (!preProcess.DoPreProcess(img_buf, img_sz)) {
            printf_err("Pre-processing failed.\n");
            return;
        }

        printf("Image %" PRIu32 ": ", img_idx);

        /* Run inference over this image. */
        if (!model.RunInference()) {
            printf_err("Inference failed.\n");
            return;
        }

        if (!postProcess.DoPostProcess()) {
            printf_err("Post-processing failed.\n");
            return;
        }

        if (results.empty()) {
            printf("No object detected\n");
        }
        else {
            printf("Detected objects ");
            for (const auto& result : results) {
                /* Set object detection box to the image */
                set_img_object_box(img_idx, result.m_x0, result.m_y0, result.m_w, result.m_h);

                /* Sent detection coordinates to the console */
                printf(":: [x=%" PRIu32 ", y=%" PRIu32 ", w=%" PRIu32 ", h=%" PRIu32 "] ", result.m_x0,
                                                                                           result.m_y0,
                                                                                           result.m_w,
                                                                                           result.m_h);
            }
            printf("\n");
        }

        close_img_source(img_idx++);
    }
}

/* Application initialization */
int app_main (void) {
    const osThreadAttr_t attr = {
        .stack_size = 4096U
    };

    /* Create application thread and start the kernel */
    osThreadNew(app_main_thread, NULL, &attr);
    osKernelStart();
    return 0;
}
