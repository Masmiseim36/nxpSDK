# shell

## Overview
The Shell Demo application demonstrates to control Leds by commands.
Note: Please input one character at a time. If you input too many characters each time, the receiver may overflow
because the low level UART uses simple polling way for receiving. If you want to try inputting many characters each time,
just define DEBUG_CONSOLE_TRANSFER_NON_BLOCKING in your project to use the advanced debug console utility.
Besides, the demo does not support semihosting mode. The shell component is based on debug console and 
serial manager. When semihosting is used, debug console and serial manager are bypassed. So the shell demo cannot
work with semihosting.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/demo_apps/shell/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/demo_apps/shell/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/demo_apps/shell/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/demo_apps/shell/example_board_readme.md)
- [EVK-MIMXRT1010](../../_boards/evkmimxrt1010/demo_apps/shell/example_board_readme.md)
- [EVK-MIMXRT1015](../../_boards/evkmimxrt1015/demo_apps/shell/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/demo_apps/shell/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/demo_apps/shell/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/demo_apps/shell/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/demo_apps/shell/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/demo_apps/shell/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/demo_apps/shell/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/demo_apps/shell/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/demo_apps/shell/example_board_readme.md)
- [FRDM-K22F](../../_boards/frdmk22f/demo_apps/shell/example_board_readme.md)
- [FRDM-K32L2A4S](../../_boards/frdmk32l2a4s/demo_apps/shell/example_board_readme.md)
- [FRDM-K32L2B](../../_boards/frdmk32l2b/demo_apps/shell/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/demo_apps/shell/example_board_readme.md)
- [FRDM-KE15Z](../../_boards/frdmke15z/demo_apps/shell/example_board_readme.md)
- [FRDM-KE16Z](../../_boards/frdmke16z/demo_apps/shell/example_board_readme.md)
- [FRDM-KE17Z](../../_boards/frdmke17z/demo_apps/shell/example_board_readme.md)
- [FRDM-KE17Z512](../../_boards/frdmke17z512/demo_apps/shell/example_board_readme.md)
- [FRDM-MCXC041](../../_boards/frdmmcxc041/demo_apps/shell/example_board_readme.md)
- [FRDM-MCXC242](../../_boards/frdmmcxc242/demo_apps/shell/example_board_readme.md)
- [FRDM-MCXC444](../../_boards/frdmmcxc444/demo_apps/shell/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/demo_apps/shell/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/demo_apps/shell/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/demo_apps/shell/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/demo_apps/shell/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/demo_apps/shell/example_board_readme.md)
- [KW45B41Z-LOC](../../_boards/kw45b41zloc/demo_apps/shell/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/demo_apps/shell/example_board_readme.md)
- [LPCXpresso51U68](../../_boards/lpcxpresso51u68/demo_apps/shell/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/demo_apps/shell/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/demo_apps/shell/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/demo_apps/shell/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/demo_apps/shell/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/demo_apps/shell/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/demo_apps/shell/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/demo_apps/shell/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/demo_apps/shell/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/demo_apps/shell/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/demo_apps/shell/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/demo_apps/shell/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/demo_apps/shell/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/demo_apps/shell/example_board_readme.md)
- [TWR-KM34Z50MV3](../../_boards/twrkm34z50mv3/demo_apps/shell/example_board_readme.md)
- [TWR-KM34Z75M](../../_boards/twrkm34z75m/demo_apps/shell/example_board_readme.md)
- [TWR-KM35Z75M](../../_boards/twrkm35z75m/demo_apps/shell/example_board_readme.md)
