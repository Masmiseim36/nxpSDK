# flexspi_flr_ocram_polling_transfer

## Overview
The flexspi_flr_ocram_polling_transfer example shows how to use the spi leader to access the OCRAM via the spi follower:

In this example, FlexSPI and FlexSPI Follower are both enabled. The FlexSPI will read and write the internal OCRAM
controlled by FlexSPI Follower. During this process, FlexSPI will send a mailbox to the FlexSPI follower to trigger
an interrupt.

## Supported Boards
- FRDM-IMXRT1186