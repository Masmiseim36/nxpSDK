# lvgl_demo_widgets

## Overview
A demo application to show lvgl widgets.

## Known issue
The CPU load remains at 100% even during low-intensity tests, confirming that FreeRTOS is not reporting the load correctly.

For all tests that use FreeRTOS, the following macros must be defined, typically at the end of FreeRTOSConfig_Gen.h

workaround code:
--------------------------------------------------------------------------
#if defined(__ICCARM__)||defined(__CC_ARM)||defined(__GNUC__)
extern void traceTaskSwitchedOut(void);
extern void traceTaskSwitchedIn(void);
#endif

#define traceTASK_SWITCHED_IN()  traceTaskSwitchedIn()
#define traceTASK_SWITCHED_OUT() traceTaskSwitchedOut()
--------------------------------------------------------------------------

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/lvgl_examples/lvgl_examples_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/lvgl_examples/lvgl_examples_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/lvgl_examples/lvgl_examples_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/lvgl_examples/lvgl_examples_readme.md)
- [EVK-MIMXRT1010](../../_boards/evkmimxrt1010/lvgl_examples/lvgl_examples_readme.md)
- [EVK-MIMXRT1015](../../_boards/evkmimxrt1015/lvgl_examples/lvgl_examples_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/lvgl_examples/lvgl_examples_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/lvgl_examples/lvgl_examples_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/lvgl_examples/lvgl_examples_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/lvgl_examples/lvgl_examples_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/lvgl_examples/lvgl_examples_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/lvgl_examples/lvgl_examples_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/lvgl_examples/lvgl_examples_readme.md)
- [FRDM-MCXA276](../../_boards/frdmmcxa276/lvgl_examples/lvgl_examples_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/lvgl_examples/lvgl_examples_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/lvgl_examples/lvgl_examples_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/lvgl_examples/lvgl_examples_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/lvgl_examples/lvgl_examples_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/lvgl_examples/lvgl_examples_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/lvgl_examples/lvgl_examples_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/lvgl_examples/lvgl_examples_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/lvgl_examples/lvgl_examples_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/lvgl_examples/lvgl_examples_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/lvgl_examples/lvgl_examples_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/lvgl_examples/lvgl_examples_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/lvgl_examples/lvgl_examples_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/lvgl_examples/lvgl_examples_readme.md)
