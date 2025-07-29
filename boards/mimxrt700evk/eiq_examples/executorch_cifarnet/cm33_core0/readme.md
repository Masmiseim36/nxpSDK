Overview
========
ExecuTorch CifarNet example project

ExecuTorch is the PyTorch ecosystem on-device inference engine,
ported for selected the NXP MCU platforms, such as RT700.

This project runs CifarNet model using ExecuTorch inference engine on
the board.

The model in the example is compiled by the script in ExecuTorch
examples/nxp. Refer to the document in it for detail.

ExecuTorch is an end-to-end solution for enabling on-device inference
capabilities across mobile and edge devices including wearables, embedded
devices and microcontrollers. It is part of the PyTorch Edge ecosystem and
enables efficient deployment of PyTorch models to edge devices. For more
information, see [1].

Key value propositions of ExecuTorch are:
- Portability: Compatibility with a wide variety of computing platforms,
  from high-end mobile phones to highly constrained embedded systems and
  microcontrollers.
- Productivity: Enabling developers to use the same toolchains and Developer
  Tools from PyTorch model authoring and conversion, to debugging and
  deployment to a wide variety of platforms.
- Performance: Providing end users with a seamless and high-performance
  experience due to a lightweight runtime and utilizing full hardware
  capabilities such as CPUs, NPUs, and DSPs.

[1] https://pytorch.org/executorch-overview

## Supported Boards
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/eiq_examples/executorch_cifarnet/example_board_readme.md)
