# MCUXpresso SDK : mcuxsdk-middleware-lwip

## Overview
This repository is a fork of lwIP TCP/IP stack (http://git.savannah.gnu.org/cgit/lwip.git) for MCUXpresso SDK delivery and it contains the components officially provided in NXP MCUXpresso SDK. This repository is part of the MCUXpresso SDK overall delivery which is composed of several sub-repositories/projects. Navigate to the top/parent repository (mcuxsdk-manifests) for the complete delivery of MCUXpresso SDK.

## Documentation
Overall details can be reviewed here: [MCUXpresso SDK Online Documentation](https://mcuxpresso.nxp.com/mcuxsdk/latest/html/introduction/README.html)  

Visit [lwIP - Documentation](https://mcuxpresso.nxp.com/mcuxsdk/latest/html/middleware/lwip/index.html) to review details on the contents in this sub-repo.  

## Setup
Instructions on how to install the MCUXpresso SDK provided from GitHub via west manifest [Getting Started with SDK - Detailed Installation Instructions](https://mcuxpresso.nxp.com/mcuxsdk/latest/html/gsd/installation.html#installation)

## Contribution
Contributions are not currently accepted.
If the intended contribution is not related to NXP specific code, consider contributing directly to the upstream lwIP project. Once this lwIP fork is synchronized with the upstream project, such contributions will end up here as well.
If the intended contribution is a bugfix or improvement for NXP porting layer or for code added or modified by NXP, please open an issue or contact NXP support.

---------------------------------
## NXP Fork
This fork of lwIP contains specific modifications and enhancements for NXP MCUXpresso SDK integration, including:
- Adaptation layers for some of the network interfaces found on NXP MCU platforms.
- Adaptation layer supporting compilers used with MCUXpresso SDK.
- Operating system abstraction layers for FreeRTOS and bare-metal.
- NXP implementation of HTTP server.
- Bugfixes and modifications of upstream code, which could not be upstreamed directly.
- CMake and Kconfig files for integration with MCUXpresso SDK build system.

See [changelog](CHANGELOG.md) for details.
