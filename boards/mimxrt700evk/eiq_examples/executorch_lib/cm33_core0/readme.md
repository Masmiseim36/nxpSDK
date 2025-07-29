Overview
========
ExecuTorch runtime library build project

This project builds ExecuTorch runtime library for RT700. Users can debug the
runtime libary as needed.

The new ExecuTorch runtime library With armgcc/mcux toolchain can place the one
in middleware/eiq/executorch/lib/cm33/armgcc/.

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
- MIMXRT700-EVK
