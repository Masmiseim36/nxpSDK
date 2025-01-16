# freertos_sem

## Overview
This document explains the freertos_sem example, what to expect when running it and a brief
introduction to the API. The freertos_sem example code shows how semaphores works. Two different
tasks are synchronized in bilateral rendezvous model.

The example uses four tasks. One producer_task and three consumer_tasks. The producer_task starts by
creating of two semaphores (xSemaphore_producer and xSemaphore_consumer). These semaphores control
access to virtual item. The synchronization is based on bilateral rendezvous pattern. Both of
consumer and producer must be prepared to enable transaction.



## Running the demo
After the board is flashed the Tera Term will show debug console output.

Example output:
Producer_task created.
Consumer_task 0 created.
Consumer_task 1 created.
Consumer_task 2 created.
Consumer number: 0
Consumer 0 accepted item.
Consumer number: 1
Consumer number: 2
Producer released item.
Consumer 0 accepted item.
Producer released item.
Consumer 1 accepted item.
Producer released item.
Consumer 2 accepted item.

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/freertos_examples/freertos_sem/example_board_readme.md)
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/freertos_examples/freertos_sem/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/freertos_examples/freertos_sem/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/freertos_examples/freertos_sem/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MCIMX7ULP](../../_boards/evkmcimx7ulp/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMX8MM](../../_boards/evkmimx8mm/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMX8MN](../../_boards/evkmimx8mn/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMX8MNDDR3L](../../_boards/evkmimx8mnddr3l/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMX8MP](../../_boards/evkmimx8mp/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMX8MQ](../../_boards/evkmimx8mq/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMXRT1010](../../_boards/evkmimxrt1010/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMXRT1015](../../_boards/evkmimxrt1015/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/freertos_examples/freertos_sem/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/freertos_examples/freertos_sem/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/freertos_examples/freertos_sem/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/freertos_examples/freertos_sem/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/freertos_examples/freertos_sem/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-K22F](../../_boards/frdmk22f/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-K32L2A4S](../../_boards/frdmk32l2a4s/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-K32L2B](../../_boards/frdmk32l2b/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-K32L3A6](../../_boards/frdmk32l3a6/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-KE15Z](../../_boards/frdmke15z/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-KE16Z](../../_boards/frdmke16z/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-KE17Z](../../_boards/frdmke17z/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-KE17Z512](../../_boards/frdmke17z512/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-MCXA153](../../_boards/frdmmcxa153/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-MCXA156](../../_boards/frdmmcxa156/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-MCXA166](../../_boards/frdmmcxa166/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-MCXA276](../../_boards/frdmmcxa276/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-MCXC242](../../_boards/frdmmcxc242/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-MCXC444](../../_boards/frdmmcxc444/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/freertos_examples/freertos_sem/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/freertos_examples/freertos_sem/example_board_readme.md)
- [IMX95LP4XEVK-15](../../_boards/imx95lp4xevk15/freertos_examples/freertos_sem/example_board_readme.md)
- [IMX95LPD5EVK-19](../../_boards/imx95lpd5evk19/freertos_examples/freertos_sem/example_board_readme.md)
- [imx95verdinevk](../../_boards/imx95verdinevk/freertos_examples/freertos_sem/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/freertos_examples/freertos_sem/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/freertos_examples/freertos_sem/example_board_readme.md)
- [KW45B41Z-LOC](../../_boards/kw45b41zloc/freertos_examples/freertos_sem/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso51U68](../../_boards/lpcxpresso51u68/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso54628](../../_boards/lpcxpresso54628/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/freertos_examples/freertos_sem/example_board_readme.md)
- [LPCXpresso860MAX](../../_boards/lpcxpresso860max/freertos_examples/freertos_sem/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../_boards/mcimx93autoevk/freertos_examples/freertos_sem/example_board_readme.md)
- [MCIMX93-EVK](../../_boards/mcimx93evk/freertos_examples/freertos_sem/example_board_readme.md)
- [MCIMX93-QSB](../../_boards/mcimx93qsb/freertos_examples/freertos_sem/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/freertos_examples/freertos_sem/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/freertos_examples/freertos_sem/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/freertos_examples/freertos_sem/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/freertos_examples/freertos_sem/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/freertos_examples/freertos_sem/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/freertos_examples/freertos_sem/example_board_readme.md)
- [TWR-KM34Z50MV3](../../_boards/twrkm34z50mv3/freertos_examples/freertos_sem/example_board_readme.md)
- [TWR-KM34Z75M](../../_boards/twrkm34z75m/freertos_examples/freertos_sem/example_board_readme.md)
- [TWR-KM35Z75M](../../_boards/twrkm35z75m/freertos_examples/freertos_sem/example_board_readme.md)
