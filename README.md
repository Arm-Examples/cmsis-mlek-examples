[![License](https://img.shields.io/github/license/Arm-Examples/cmsis-mlek-examples?label)](https://github.com/Arm-Examples/cmsis-mlek-examples/blob/main/LICENSE)
[![AC6 Alif build tests](https://img.shields.io/github/actions/workflow/status/Arm-Examples/cmsis-mlek-examples/AC6_test_build.yaml?logo=arm&logoColor=0091bd&label=AC6%20Alif%20Build%20Test)](https://github.com/Arm-Examples/cmsis-mlek-examples/tree/main/.github/workflows/AC6_test_build.yaml)
[![GCC Alif build tests](https://img.shields.io/github/actions/workflow/status/Arm-Examples/cmsis-mlek-examples/GCC_test_build.yaml?logo=arm&logoColor=0091bd&label=GCC%20Alif%20Build%20Test)](https://github.com/Arm-Examples/cmsis-mlek-examples/tree/main/.github/workflows/GCC_test_build.yaml)
[![CLANG Alif build tests](https://img.shields.io/github/actions/workflow/status/Arm-Examples/cmsis-mlek-examples/CLANG_test_build.yaml?logo=arm&logoColor=0091bd&label=CLANG%20Alif%20Build%20Test)](https://github.com/Arm-Examples/cmsis-mlek-examples/tree/main/.github/workflows/CLANG_test_build.yaml)
[![Audio build and execution tests](https://img.shields.io/github/actions/workflow/status/Arm-Examples/cmsis-mlek-examples/test_audio.yaml?logo=arm&logoColor=0091bd&label=Audio%20Build%20and%20Run%20Test)](https://github.com/Arm-Examples/cmsis-mlek-examples/tree/main/.github/workflows/test_audio.yaml)
[![Video build and execution tests](https://img.shields.io/github/actions/workflow/status/Arm-Examples/cmsis-mlek-examples/test_video.yaml?logo=arm&logoColor=0091bd&label=Video%20Build%20and%20Run%20Test)](https://github.com/Arm-Examples/cmsis-mlek-examples/tree/main/.github/workflows/test_video.yaml)


# CMSIS-MLEK Examples

This repository shows the usage of the [CMSIS-MLEK](https://www.keil.arm.com/packs/cmsis-mlek-arm/overview/) and [tensorflow-lite-micro](https://www.keil.arm.com/packs/tensorflow-lite-micro-tensorflow/overview/) software packs.


## Quick Start

1. Install [Keil Studio for VS Code](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack) from the VS Code marketplace.
2. In VS Code, either clone this Git repository or (if downloaded as ZIP file) open the top-level folder.
3. Open the [CMSIS View](https://mdk-packs.github.io/vscode-cmsis-solution-docs/userinterface.html#2-main-area-of-the-cmsis-view) in VS Code and use the ... menu to choose an example via Select Active Solution from workspace.
4. The related tools and software packs are downloaded and installed. Review progress with View - Output - CMSIS Solution.
5. In the CMSIS view, use the [Action buttons](https://github.com/ARM-software/vscode-cmsis-csolution?tab=readme-ov-file#action-buttons) to build, load and debug the example.


## Examples description

Each example is independently configured for directly running on the [Alif AppKit-E7-AIML Gen 2](https://www.keil.arm.com/boards/alif-semiconductor-appkit-e7-aiml-gen-2-140e28d/guide/) board utilizing Ethos-U55 and dual core configurations.

| Example name                                               | Description   |
|---                                                         |---            |
| [Alif/AppKit_D3/MLEK_AV_DualCore](./Alif/AppKit_D3/MLEK_AV_DualCore/mlek_dual.csolution.yml)    | ML and Dual-Core debugging on [Alif AppKit-E7-AIML Gen 2](https://www.keil.arm.com/boards/alif-semiconductor-appkit-e7-aiml-gen-2-140e28d/guide/) board. The example combines two Machine Learning Evaluation Kit (MLEK) applications. The Audio Keyword Spotting application executes on the Cortex-M55/Ethos-U55 High Efficiency core. The inference result is output over the serial port. The Video Object Detection application executes on the Cortex-M55/Ethos-U55 High Performance core. The inference result is shown on the display. [Webinar](https://www.arm.com/resources/webinar/keil-studio-session-3) |
| [Alif/AppKit_D3/MLEK_Audio](./Alif/AppKit_D3/MLEK_Audio/mlek_audio.csolution.yml)               | Keyword spotting example which can detect up to twelve keywords in the input audio stream and runs on the [Alif AppKit-E7-AIML Gen 2](https://www.keil.arm.com/boards/alif-semiconductor-appkit-e7-aiml-gen-2-140e28d/guide/) board. |
| [Alif/AppKit_D3/MLEK_Video](./Alif/AppKit_D3/MLEK_Video/mlek_video.csolution.yml)               | Object detection example which uses a neural network model that specialises in detecting human faces in images and runs on the [Alif AppKit-E7-AIML Gen 2](https://www.keil.arm.com/boards/alif-semiconductor-appkit-e7-aiml-gen-2-140e28d/guide/) board.|
| [Alif/AppKit_D3/Tensorflow_LiteRT_HelloWorld](./Alif/AppKit_D3/Tensorflow_LiteRT_HelloWorld/TFLiteRT_HelloWorld.csolution.yml) | This TensorFlow Lite RT Hello World application demonstrates how to use TensorFlow Lite for Microcontrollers with CMSIS support. The example trains and runs a small neural network that mimics a sine wave function by using the [Alif AppKit-E7-AIML Gen 2](https://www.keil.arm.com/boards/alif-semiconductor-appkit-e7-aiml-gen-2-140e28d/guide/) board. |

> IMPORTANT
>
> - Each example has a local VS Code configuration. Use in VS Code **Open Folder** to open the folder of each project individually.


## Directory Structure

| File/Directory                                                          | Content  |
|---                                                                      |---       |
| [.github/workflows](./.github/workflows)                                | [GitHub Actions](#github-actions) scripts for build and execution tests. |
| [Alif/AppKit_D3/MLEK_AV_DualCore](./Alif/AppKit_D3/MLEK_AV_DualCore)    | Contains the ML and Dual-Core application. |
| [Alif/AppKit_D3/MLEK_Audio](./Alif/AppKit_D3/MLEK_Audio)                | Contains the Audio Keyword Spotting application. |
| [Alif/AppKit_D3/MLEK_Video](./Alif/AppKit_D3/MLEK_Video)                | Contains the Video Object Detection application. |
| [Alif/AppKit_D3/Tensorflow_LiteRT_HelloWorld](./Alif/AppKit_D3/Tensorflow_LiteRT_HelloWorld) | Contains the TensorFlow Lite application. |


## Continuous Integration (CI)

The repository uses [GitHub Actions](.github/workflows) to test project builds by using the Arm Compiler for Embedded (AC6), GCC, and Arm Compiler armclang (CLANG). It is also builds audio and video examples and test them by using [FVP simulation models](https://arm-software.github.io/AVH/main/simulation/html/index.html) .


| CI Workflow                                                   | Description |
|---                                                            |--- |
| [AC6_test_build](./.github/workflows/AC6_test_build.yaml)     | Builds example binaries for different contexts by using the Arm Compiler for Embedded (AC6) on a GitHub hosted runner and save them as artifacts. |
| [GCC_test_build](./.github/workflows/GCC_test_build.yaml)     | Builds example binaries for different contexts by using the GCC compiler on a GitHub hosted runner and save them as artifacts. |
| [CLANG_test_build](./.github/workflows/CLANG_test_build.yaml) | Builds example binaries for different contexts using the Arm Compiler armclang on a GitHub hosted runner and save them as artifacts. |
| [test_audio](./.github/workflows/test_audio.yaml)             | Builds the Keyword spotting application (audio) for different contexts by using the Arm Compiler for Embedded (AC6), upload the binaries as artifacts, execute each of them by using [FVP simulation models](https://arm-software.github.io/AVH/main/simulation/html/index.html), capture the FVP UART output and upload them as artifact. |
| [test_video](./.github/workflows/test_video.yaml)             | Builds the Object detection application (video) for different contexts by using the Arm Compiler for Embedded (AC6) and upload the binaries as artifacts. |


## Issues

Please feel free to raise an [issue on GitHub](issues) to report misbehavior (i.e. bugs) or start discussions about enhancements. This is your best way to interact directly with the maintenance team and the community. We encourage you to append implementation suggestions as this helps to decrease the workload of the limited maintenance team.
