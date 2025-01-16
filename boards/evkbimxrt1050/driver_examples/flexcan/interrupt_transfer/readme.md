# flexcan_interrupt_transfer

## Overview
The flexcan_interrupt example shows how to use FlexCAN driver in none-blocking interrupt way:

In this example, 2 boards are connected through CAN bus. Endpoint A(board A) send a CAN Message to
Endpoint B(board B) when user press space key in terminal. Endpoint B receive the message, print
the message content to terminal and echo back the message. Endpoint A will increase the received
message and waiting for the next transmission of the user initiated.

For self wake up from STOP mode, since steps which MCU enters STOP mode differs on different MCUs,
take flexcan_interrupt_transfer of twrke18f for example, user should do like this:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
##include "fsl_smc.h"
...
...
flexcanConfig.enableSelfWakeup = true;
FLEXCAN_Init();
...
...
SMC_SetPowerModeStop(SMC, kSMC_PartialStop1);
if (wakenUp)
{
    PRINTF("B has been waken up!\r\n\r\n");
}
...
...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [EVK9-MIMX8ULP](../../../_boards/evk9mimx8ulp/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [EVK-MIMX8MP](../../../_boards/evkmimx8mp/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [EVK-MIMX8ULP](../../../_boards/evkmimx8ulp/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [EVK-MIMXRT1020](../../../_boards/evkmimxrt1020/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MIMXRT1024-EVK](../../../_boards/evkmimxrt1024/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MIMXRT1180-EVK](../../../_boards/evkmimxrt1180/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [FRDM-MCXA156](../../../_boards/frdmmcxa156/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [FRDM-MCXA166](../../../_boards/frdmmcxa166/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [FRDM-MCXA276](../../../_boards/frdmmcxa276/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [FRDM-MCXN236](../../../_boards/frdmmcxn236/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [FRDM-MCXN947](../../../_boards/frdmmcxn947/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [IMX95LP4XEVK-15](../../../_boards/imx95lp4xevk15/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [IMX95LPD5EVK-19](../../../_boards/imx95lpd5evk19/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [imx95verdinevk](../../../_boards/imx95verdinevk/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../../_boards/mcimx93autoevk/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MCIMX93-EVK](../../../_boards/mcimx93evk/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MCIMX93-QSB](../../../_boards/mcimx93qsb/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MCX-N9XX-EVK](../../../_boards/mcxn9xxevk/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/flexcan/interrupt_transfer/example_board_readme.md)
