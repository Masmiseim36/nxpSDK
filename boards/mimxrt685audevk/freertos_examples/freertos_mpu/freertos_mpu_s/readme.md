# freertos_mpu

## Overview
This document explains the freertos_mpu example. This demo application utilizes TrustZone,
so it demonstrates following techniques for TrustZone applications development:
1. Application separation between secure and non-secure part
2. TrustZone environment configuration
3. Exporting secure function to non-secure world
4. Calling non-secure function from secure world
5. Configuring IAR, MDK, GCC and MCUX environments for TrustZone based projects

## Running the demo
The MPU demo creates 3 unprivileged tasks - One of which has Read Only access
to a shared memory region, the second one has Read Write access to the same
region and the third one has Read Write access to a non-secure counter. The task
with Read Only access reads from the shared memory but can not write into it
which would result in memory fault (User can try to uncomment respective line
of code to test this behavior). The Read Write task writes to the shared
memory. The third task which is created calls a secure side function and passes
a pointer to a callback function. The secure side function does two things:
1. It calls the provided callback function. The callback function increments
a non-secure counter.
2. It increments a secure counter and returns the incremented value.
After the secure function call finishes, it verifies that both the counters
are incremented.

## Supported Boards
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/freertos_examples/freertos_mpu/freertos_mpu_s/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/freertos_examples/freertos_mpu/freertos_mpu_s/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/freertos_examples/freertos_mpu/freertos_mpu_s/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/freertos_examples/freertos_mpu/freertos_mpu_s/example_board_readme.md)
