# snvs_hp_hac

## Overview
The SNVS HP HAC project shows how to use the High Assurance Counter (HAC) based on
SDK SNVS HP driver. In this example,
   1. The HAC is enabled and set a initial value.
   2. Software fatal security violation is triggered, and the SSM switches to soft fail mode.
   3. In soft fail mode, software stops the HAC counter, and clear the HAC to make SSM
      switch to hard fail mode. 
   4. The system resets when enters hard fail mode.

## Supported Boards
- [EVKB-IMXRT1050](../../../_boards/evkbimxrt1050/driver_examples/snvs/snvs_hp_hac/example_board_readme.md)
- [MIMXRT1060-EVKB](../../../_boards/evkbmimxrt1060/driver_examples/snvs/snvs_hp_hac/example_board_readme.md)
- [MIMXRT1170-EVKB](../../../_boards/evkbmimxrt1170/driver_examples/snvs/snvs_hp_hac/example_board_readme.md)
- [MIMXRT1060-EVKC](../../../_boards/evkcmimxrt1060/driver_examples/snvs/snvs_hp_hac/example_board_readme.md)
- [MIMXRT1040-EVK](../../../_boards/evkmimxrt1040/driver_examples/snvs/snvs_hp_hac/example_board_readme.md)
- [EVK-MIMXRT1064](../../../_boards/evkmimxrt1064/driver_examples/snvs/snvs_hp_hac/example_board_readme.md)
- [MIMXRT1160-EVK](../../../_boards/evkmimxrt1160/driver_examples/snvs/snvs_hp_hac/example_board_readme.md)
