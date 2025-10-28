[![License](https://img.shields.io/github/license/Arm-Examples/cmsis-mlek-examples?label)](https://github.com/Arm-Examples/cmsis-mlek-examples/blob/main/LICENSE)
[![Build and Execution Test](https://img.shields.io/github/actions/workflow/status/Arm-Examples/cmsis-mlek-examples/test_audio.yaml?logo=arm&logoColor=0091bd&label=Audio%20Build%20and%20Run%20Test)](https://github.com/Arm-Examples/cmsis-mlek-examples/tree/main/.github/workflows/test_audio.yaml)
[![AC6 Alif Build Test](https://img.shields.io/github/actions/workflow/status/Arm-Examples/cmsis-mlek-examples/AC6_test_build.yaml?logo=arm&logoColor=0091bd&label=AC6%20Alif%20Build%20Test)](https://github.com/Arm-Examples/cmsis-mlek-examples/tree/main/.github/workflows/AC6_test_build.yaml)
[![GCC Alif Build Test](https://img.shields.io/github/actions/workflow/status/Arm-Examples/cmsis-mlek-examples/GCC_test_build.yaml?logo=arm&logoColor=0091bd&label=AC6%20Alif%20Build%20Test)](https://github.com/Arm-Examples/cmsis-mlek-examples/tree/main/.github/workflows/GCC_test_build.yaml)

# CMSIS-MLEK Examples

This repository shows the usage of the [CMSIS-MLEK](https://www.keil.arm.com/packs/cmsis-mlek-arm/overview/) and [tensorflow-lite-micro](https://www.keil.arm.com/packs/tensorflow-lite-micro-tensorflow/overview/) software packs.

It contains configured [CMSIS Reference Applications](https://open-cmsis-pack.github.io/cmsis-toolbox/ReferenceApplications/) that are stand-alone example projects. Each example is independently configured for directly running on the related hardware board. 

Configured Boards:

- [Alif Semiconductor Ensemble E7 AI/ML AppKit](./Alif) utilizing Ethos-U55 and dual core configurations.

## Quick Start

1. Install [Keil Studio for VS Code](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack) from the VS Code marketplace.
2. In VS Code, either clone this Git repository or (if downloaded as ZIP file) open the top-level folder.
3. Open the [CMSIS View](https://mdk-packs.github.io/vscode-cmsis-solution-docs/userinterface.html#2-main-area-of-the-cmsis-view) in VS Code and use the ... menu to choose an example via Select Active Solution from workspace.
4. The related tools and software packs are downloaded and installed. Review progress with View - Output - CMSIS Solution.
5. In the CMSIS view, use the [Action buttons](https://github.com/ARM-software/vscode-cmsis-csolution?tab=readme-ov-file#action-buttons) to build, load and debug the example.

## CI Verification

A [GitHub Workflow](https://github.com/Arm-Examples/cmsis-mlek-examples/blob/main/.github/workflows/test_audio.yaml) exemplifies a verification process in a CI system. It compiles the application for a hardware target or [Arm Virtual Hardware (AVH) FVP](https://github.com/ARM-software/AVH). The output is stored as artifact. For AVH FVP it is also executed.

In a similar way it could be also compiled for the hardware target.

> TIP: You may fork this repository to explore this workflow.

## Issues

Please feel free to raise an [issue on GitHub](issues) to report misbehavior (i.e. bugs) or start discussions about enhancements. This is your best way to interact directly with the maintenance team and the community. We encourage you to append implementation suggestions as this helps to decrease the workload of the limited maintenance team.
