var a00016 =
[
    [ "dspi_command_data_config_t", "a00016.html#a00254", [
      [ "isPcsContinuous", "a00016.html#a1b7521543b11fe504f32d7beb728c14d", null ],
      [ "whichCtar", "a00016.html#a9e84875f1d5c926deb4592fd09c73d0f", null ],
      [ "whichPcs", "a00016.html#a511d93fdc0c9736dec3f8b39bb1eecb6", null ],
      [ "isEndOfQueue", "a00016.html#a378b8b8a618355869de07b435ad52f82", null ],
      [ "clearTransferCount", "a00016.html#aaf4b200e2ca05a92686ff5e158e3a61f", null ]
    ] ],
    [ "dspi_master_ctar_config_t", "a00016.html#a00257", [
      [ "baudRate", "a00016.html#abc9dde7dd3b924290ecd6180f7a7e0cb", null ],
      [ "bitsPerFrame", "a00016.html#a935cc12276a643aac250e07b6fd26841", null ],
      [ "cpol", "a00016.html#a87112137c6b8b714d1700756557c86c9", null ],
      [ "cpha", "a00016.html#a3649f0e62d8f5421366afb76792e957b", null ],
      [ "direction", "a00016.html#aafbec5b2cc4cdec420381f29b32a04e8", null ],
      [ "pcsToSckDelayInNanoSec", "a00016.html#a32c83fc580640a66049071dd9b672a1a", null ],
      [ "lastSckToPcsDelayInNanoSec", "a00016.html#af6e22013a735cf762e671973afbed487", null ],
      [ "betweenTransferDelayInNanoSec", "a00016.html#a8d900155157617487c5089a91c41c638", null ]
    ] ],
    [ "dspi_master_config_t", "a00016.html#a00256", [
      [ "whichCtar", "a00016.html#a822cafb9ccbaac368bdd456f004dd7f1", null ],
      [ "ctarConfig", "a00016.html#a31ffb9db13f0b3b9ac1cd6272674047b", null ],
      [ "whichPcs", "a00016.html#a01111111bd757122f7874576e3b859e7", null ],
      [ "pcsActiveHighOrLow", "a00016.html#a70b19520cae538f1860d355bacc4abca", null ],
      [ "enableContinuousSCK", "a00016.html#a9fd922242d41c151599c4906c4d8d7f2", null ],
      [ "enableRxFifoOverWrite", "a00016.html#aa9a8bb9487711b83566ae0e04b386905", null ],
      [ "enableModifiedTimingFormat", "a00016.html#a86656569461eb376c1a7db424efb8916", null ],
      [ "samplePoint", "a00016.html#a1827b5d3bf3ff28b7ce940bbae1a6d35", null ]
    ] ],
    [ "dspi_slave_ctar_config_t", "a00016.html#a00259", [
      [ "bitsPerFrame", "a00016.html#a3ddbe384d7b4c256ced99ea6671ed73d", null ],
      [ "cpol", "a00016.html#abc3449874392a507f8ca15263119001e", null ],
      [ "cpha", "a00016.html#a80127ee24970efe8ea1c8760bb647ce5", null ]
    ] ],
    [ "dspi_slave_config_t", "a00016.html#a00258", [
      [ "whichCtar", "a00016.html#a7b488cbe0d44e605c74341b241aba6e8", null ],
      [ "ctarConfig", "a00016.html#aa69a2a4dab7a6e026ee1e568418b18f7", null ],
      [ "enableContinuousSCK", "a00016.html#abd2c9b108ae18e0abd71fbb457d17d70", null ],
      [ "enableRxFifoOverWrite", "a00016.html#ad1db8c46797dbc50d5a3b2a915b8ce58", null ],
      [ "enableModifiedTimingFormat", "a00016.html#a9a8cd1afddd8edca36c217d1f4e440ad", null ],
      [ "samplePoint", "a00016.html#a7771f990c0f85360232e144cb0f467f2", null ]
    ] ],
    [ "dspi_transfer_t", "a00016.html#a00260", [
      [ "txData", "a00016.html#a90810cbdeac26cc69fef68d1b964ef12", null ],
      [ "rxData", "a00016.html#ac922665b229d98d1bf2123eaa33d1457", null ],
      [ "dataSize", "a00016.html#a9e9f9810b6206130faa670b41a5f0f47", null ],
      [ "configFlags", "a00016.html#a15da053c55c0fe080aa6895a6deda4e2", null ]
    ] ],
    [ "dspi_half_duplex_transfer_t", "a00016.html#a00255", [
      [ "txData", "a00016.html#a6bd203987ede3e0c62545d1ae7cfbeb6", null ],
      [ "rxData", "a00016.html#a46466a7e1c28ca979ee2fb7bf451f77a", null ],
      [ "txDataSize", "a00016.html#a38996bf9e51c59ed7e577c177c761a60", null ],
      [ "rxDataSize", "a00016.html#aeb3ebbd644b878013a0aaac1e966998f", null ],
      [ "configFlags", "a00016.html#a85f599a97f87ce9dad2fdf056d07c6c6", null ],
      [ "isPcsAssertInTransfer", "a00016.html#ac7436f50c4a7a4908a93ec5fdbab0184", null ],
      [ "isTransmitFirst", "a00016.html#a00165c53cad4caf9ccdc78499f72cdd9", null ]
    ] ],
    [ "dspi_master_handle_t", "a00016.html#a00224", [
      [ "bitsPerFrame", "a00016.html#ae40ea7aebf40ae6a0ef39a2bc0c11979", null ],
      [ "command", "a00016.html#aad1c69d203233d76b5952e475d053f84", null ],
      [ "lastCommand", "a00016.html#a606dff7a7dd4d18f4be97656f0102d59", null ],
      [ "fifoSize", "a00016.html#a9ba5d93da2d5aeb52da12560ee919c23", null ],
      [ "isPcsActiveAfterTransfer", "a00016.html#aa9da46d9cb79356658db575799d5bc83", null ],
      [ "isThereExtraByte", "a00016.html#aa486cbc9a515a7eab48bffc835cbcd00", null ],
      [ "txData", "a00016.html#aaaa4534d6baf59254e805b08f1bf7dad", null ],
      [ "rxData", "a00016.html#a40f937c1bd502db19cc9e8ed2eda0949", null ],
      [ "remainingSendByteCount", "a00016.html#a8d2239362d3da9fca02747c04209e004", null ],
      [ "remainingReceiveByteCount", "a00016.html#a0b874ce3772b6d421a2c987032d47852", null ],
      [ "totalByteCount", "a00016.html#a4e7587734f97a75b1eed4d790abfa596", null ],
      [ "state", "a00016.html#a782ff1c918a8aa3491bf29aa6d9a1027", null ],
      [ "callback", "a00016.html#ab08248997b9c6250932742abd64d842f", null ],
      [ "userData", "a00016.html#ab603f42c0b22ed2cad93bd176fdd863c", null ]
    ] ],
    [ "dspi_slave_handle_t", "a00016.html#a00226", [
      [ "bitsPerFrame", "a00016.html#af17a0af7a2f6dfa4483302425ee26284", null ],
      [ "isThereExtraByte", "a00016.html#a066e7025eeeb49fc66390eb62efbd935", null ],
      [ "txData", "a00016.html#a043d3f013f1a34161036b90549c55546", null ],
      [ "rxData", "a00016.html#ab51f36fcc1194017f0890f002d45e85f", null ],
      [ "remainingSendByteCount", "a00016.html#a4cf577cf53d51d5341b5abeb036116fa", null ],
      [ "remainingReceiveByteCount", "a00016.html#a2cb76e44a835d9df89e523646e33282f", null ],
      [ "totalByteCount", "a00016.html#a432ae812101804ba12e00f7e557f895f", null ],
      [ "state", "a00016.html#a003e7d2ad9226e5459b7534e6fea7355", null ],
      [ "errorCount", "a00016.html#af76902a142c73721cd36a477771855b5", null ],
      [ "callback", "a00016.html#af72dca24f829b41eef262acf362f9f93", null ],
      [ "userData", "a00016.html#a39ee6975cb1628b49b045227a6bf1710", null ]
    ] ],
    [ "FSL_DSPI_DRIVER_VERSION", "a00016.html#ga9ed59b934c560c5d88000b17b8171a01", null ],
    [ "DSPI_DUMMY_DATA", "a00016.html#gae3af6268fdc846d553d23c82fd8c8668", null ],
    [ "DSPI_MASTER_CTAR_SHIFT", "a00016.html#gac4a272cf2546e7d51aa45d76b10dc174", null ],
    [ "DSPI_MASTER_CTAR_MASK", "a00016.html#ga15e4a446e06e9964a77d20c00b7d3397", null ],
    [ "DSPI_MASTER_PCS_SHIFT", "a00016.html#ga082f0377ee21656002efecaeaf77034c", null ],
    [ "DSPI_MASTER_PCS_MASK", "a00016.html#ga213b335dc840f0c1637ca37b0cf3f4f4", null ],
    [ "DSPI_SLAVE_CTAR_SHIFT", "a00016.html#ga5d0819629aecb560192865e1850bff07", null ],
    [ "DSPI_SLAVE_CTAR_MASK", "a00016.html#gaf41a736795e34277fdb594a75754ab69", null ],
    [ "dspi_master_transfer_callback_t", "a00016.html#gad191922bda6ac07f95f241a67eb52f48", null ],
    [ "dspi_slave_transfer_callback_t", "a00016.html#ga0c9ef1b6a6d8034b5e47b8310f2d52dc", [
      [ "kStatus_DSPI_Busy", "a00016.html#gga61dadd085c1777f559549e05962b2c9ead5ecc8346da4119b8609c6bcb4c57e40", null ],
      [ "kStatus_DSPI_Error", "a00016.html#gga61dadd085c1777f559549e05962b2c9eaaae2ba17a6a622142816b0ffec7b9f7a", null ],
      [ "kStatus_DSPI_Idle", "a00016.html#gga61dadd085c1777f559549e05962b2c9ea638dc0d050e7660225a46cc7cd6e38c7", null ],
      [ "kStatus_DSPI_OutOfRange", "a00016.html#gga61dadd085c1777f559549e05962b2c9eac1713712f0410e28da008d714734a6bd", null ]
    ] ],
    [ "_dspi_flags", "a00016.html#ga2bfefaf6ba65ba464e764d1c918c904f", [
      [ "kDSPI_TxCompleteFlag", "a00016.html#gga2bfefaf6ba65ba464e764d1c918c904faffc8e8711d9083470cddb0db647b75b0", null ],
      [ "kDSPI_EndOfQueueFlag", "a00016.html#gga2bfefaf6ba65ba464e764d1c918c904fae91c7a5cc2a90fa051c89f13bbb6d8ed", null ],
      [ "kDSPI_TxFifoUnderflowFlag", "a00016.html#gga2bfefaf6ba65ba464e764d1c918c904fae36215137d8ce7cf215349199db877b7", null ],
      [ "kDSPI_TxFifoFillRequestFlag", "a00016.html#gga2bfefaf6ba65ba464e764d1c918c904fae9704d53b57758969f8ea5ea6c86f7f0", null ],
      [ "kDSPI_RxFifoOverflowFlag", "a00016.html#gga2bfefaf6ba65ba464e764d1c918c904fa30f039adca01f89dbbd02f70dff725ee", null ],
      [ "kDSPI_RxFifoDrainRequestFlag", "a00016.html#gga2bfefaf6ba65ba464e764d1c918c904fa092b7f39357ce8cb82ec825e93536605", null ],
      [ "kDSPI_TxAndRxStatusFlag", "a00016.html#gga2bfefaf6ba65ba464e764d1c918c904fa58771b3977aef221dab6a67a6739f8d6", null ],
      [ "kDSPI_AllStatusFlag", "a00016.html#gga2bfefaf6ba65ba464e764d1c918c904fa4a742818251256d8fc35ab63a6af9c9e", null ]
    ] ],
    [ "_dspi_interrupt_enable", "a00016.html#gaeb57298690a2f1a09d94d696c893c4b2", [
      [ "kDSPI_TxCompleteInterruptEnable", "a00016.html#ggaeb57298690a2f1a09d94d696c893c4b2ab2b1ba228fd75de23a2de7e56c1ee438", null ],
      [ "kDSPI_EndOfQueueInterruptEnable", "a00016.html#ggaeb57298690a2f1a09d94d696c893c4b2a069483b28469fcbfa5890b04cd6439b3", null ],
      [ "kDSPI_TxFifoUnderflowInterruptEnable", "a00016.html#ggaeb57298690a2f1a09d94d696c893c4b2aa430e623e0bb240752381eaddda1a973", null ],
      [ "kDSPI_TxFifoFillRequestInterruptEnable", "a00016.html#ggaeb57298690a2f1a09d94d696c893c4b2ada57830661d523d12e49892060fde201", null ],
      [ "kDSPI_RxFifoOverflowInterruptEnable", "a00016.html#ggaeb57298690a2f1a09d94d696c893c4b2a190746a0aeaa61db32c6c1a7b850d0ee", null ],
      [ "kDSPI_RxFifoDrainRequestInterruptEnable", "a00016.html#ggaeb57298690a2f1a09d94d696c893c4b2aa7d99e6ac31bd6c7d835d89f36cec1a6", null ],
      [ "kDSPI_AllInterruptEnable", "a00016.html#ggaeb57298690a2f1a09d94d696c893c4b2a530d972d6cd16ab6e929d7ddaaf09b30", null ]
    ] ],
    [ "_dspi_dma_enable", "a00016.html#gae3359796dc0680797b1f74b83fc0c0d9", [
      [ "kDSPI_TxDmaEnable", "a00016.html#ggae3359796dc0680797b1f74b83fc0c0d9ae772dc49e5a28df00b817f9c6dab0749", null ],
      [ "kDSPI_RxDmaEnable", "a00016.html#ggae3359796dc0680797b1f74b83fc0c0d9a15ec9c9897199d53a1b354ccce6d0445", null ]
    ] ],
    [ "dspi_master_slave_mode_t", "a00016.html#gad7f974015f32db057dafada8b95641aa", [
      [ "kDSPI_Master", "a00016.html#ggad7f974015f32db057dafada8b95641aaa8330c6ad827da3c783df5805244fa7d9", null ],
      [ "kDSPI_Slave", "a00016.html#ggad7f974015f32db057dafada8b95641aaa2e075745386fd71bee2535606f29dd87", null ]
    ] ],
    [ "dspi_master_sample_point_t", "a00016.html#gae783895e2917abe07adbe27a253510a2", [
      [ "kDSPI_SckToSin0Clock", "a00016.html#ggae783895e2917abe07adbe27a253510a2abbcf84bafbd94a63a9600647162b8d86", null ],
      [ "kDSPI_SckToSin1Clock", "a00016.html#ggae783895e2917abe07adbe27a253510a2a61e5f5d7122c849c737513ae7c5c4c50", null ],
      [ "kDSPI_SckToSin2Clock", "a00016.html#ggae783895e2917abe07adbe27a253510a2a305d68c9446ca0866da7a2ace743ae4d", null ]
    ] ],
    [ "dspi_which_pcs_t", "a00016.html#ga36d77cff6cfa202e0ff3a05ab4c2a632", [
      [ "kDSPI_Pcs0", "a00016.html#gga36d77cff6cfa202e0ff3a05ab4c2a632a5c6297be9586ee874fa1a84a16d810b7", null ],
      [ "kDSPI_Pcs1", "a00016.html#gga36d77cff6cfa202e0ff3a05ab4c2a632a62d3c43292cebeed478a36bff2cd033a", null ],
      [ "kDSPI_Pcs2", "a00016.html#gga36d77cff6cfa202e0ff3a05ab4c2a632a625c90d5151e8458be6f89ace68f2fe2", null ],
      [ "kDSPI_Pcs3", "a00016.html#gga36d77cff6cfa202e0ff3a05ab4c2a632a7fae848c0f775a86562b90ecfd171cc8", null ],
      [ "kDSPI_Pcs4", "a00016.html#gga36d77cff6cfa202e0ff3a05ab4c2a632a0fd968cdbfd2e088987e309f49cb20f2", null ],
      [ "kDSPI_Pcs5", "a00016.html#gga36d77cff6cfa202e0ff3a05ab4c2a632a67653d39cbd675c9141bb014d4576a0b", null ]
    ] ],
    [ "dspi_pcs_polarity_config_t", "a00016.html#gab466e73cb54b2c023459d43918c4197d", [
      [ "kDSPI_PcsActiveHigh", "a00016.html#ggab466e73cb54b2c023459d43918c4197da79a6807edd30a1230477ab26068060fd", null ],
      [ "kDSPI_PcsActiveLow", "a00016.html#ggab466e73cb54b2c023459d43918c4197daa678a5937bbb9975e3c014592c3d542c", null ]
    ] ],
    [ "_dspi_pcs_polarity", "a00016.html#gad23a66cefb04826de83504ad485f19a9", [
      [ "kDSPI_Pcs0ActiveLow", "a00016.html#ggad23a66cefb04826de83504ad485f19a9ac731b21eefcc16342d2c606a12a00547", null ],
      [ "kDSPI_Pcs1ActiveLow", "a00016.html#ggad23a66cefb04826de83504ad485f19a9aa6ee5dca40cbe9bf03623cf986adbadd", null ],
      [ "kDSPI_Pcs2ActiveLow", "a00016.html#ggad23a66cefb04826de83504ad485f19a9a6fd76d22cb6c8f943ae397bb91ba68f4", null ],
      [ "kDSPI_Pcs3ActiveLow", "a00016.html#ggad23a66cefb04826de83504ad485f19a9a57e33d7e4195864f89db11d2f5e6cc4b", null ],
      [ "kDSPI_Pcs4ActiveLow", "a00016.html#ggad23a66cefb04826de83504ad485f19a9a15c201d8e7bd0bab1dd7117b73a111ec", null ],
      [ "kDSPI_Pcs5ActiveLow", "a00016.html#ggad23a66cefb04826de83504ad485f19a9a88e1e00a5a7755561358f004a5a1b1d4", null ],
      [ "kDSPI_PcsAllActiveLow", "a00016.html#ggad23a66cefb04826de83504ad485f19a9adb2bef5058b4bf00533cc89f1928e2d1", null ]
    ] ],
    [ "dspi_clock_polarity_t", "a00016.html#ga1e0a9074742794ef89f597d220296651", [
      [ "kDSPI_ClockPolarityActiveHigh", "a00016.html#gga1e0a9074742794ef89f597d220296651ab5279f36f0c6b1617aa937824806d71d", null ],
      [ "kDSPI_ClockPolarityActiveLow", "a00016.html#gga1e0a9074742794ef89f597d220296651abcde58b8834e5cd1181b8b98aa4a10ef", null ]
    ] ],
    [ "dspi_clock_phase_t", "a00016.html#ga4269ec144334dd60666a92e6fd2c1476", [
      [ "kDSPI_ClockPhaseFirstEdge", "a00016.html#gga4269ec144334dd60666a92e6fd2c1476a996e921abbf325ee9978a42681aee0d5", null ],
      [ "kDSPI_ClockPhaseSecondEdge", "a00016.html#gga4269ec144334dd60666a92e6fd2c1476a43ee643e847b3118e38da0a9811d97f9", null ]
    ] ],
    [ "dspi_shift_direction_t", "a00016.html#ga06fad8ae17b680f6dddfd798c9d3b30d", [
      [ "kDSPI_MsbFirst", "a00016.html#gga06fad8ae17b680f6dddfd798c9d3b30da8885a916a15d0b97ffd0f28d81242f6f", null ],
      [ "kDSPI_LsbFirst", "a00016.html#gga06fad8ae17b680f6dddfd798c9d3b30da76701314fa7dbd70e4011feb326b9050", null ]
    ] ],
    [ "dspi_delay_type_t", "a00016.html#ga1ca2fbee37b3cb046c075a7e765d64ed", [
      [ "kDSPI_PcsToSck", "a00016.html#gga1ca2fbee37b3cb046c075a7e765d64eda71185ae0d4d9dd61acbc69bce93f33f5", null ],
      [ "kDSPI_LastSckToPcs", "a00016.html#gga1ca2fbee37b3cb046c075a7e765d64edaa2ce775b9575a3870ce82b8444b9d56c", null ],
      [ "kDSPI_BetweenTransfer", "a00016.html#gga1ca2fbee37b3cb046c075a7e765d64eda83ed3f05b8a61f94c0da066c1ded7a1e", null ]
    ] ],
    [ "dspi_ctar_selection_t", "a00016.html#ga992d5562af4cf4c45371feb8c5c1a1bf", [
      [ "kDSPI_Ctar0", "a00016.html#gga992d5562af4cf4c45371feb8c5c1a1bfadb2a4c8c9b722c6a1b8cbb03b17a6519", null ],
      [ "kDSPI_Ctar1", "a00016.html#gga992d5562af4cf4c45371feb8c5c1a1bfad6db3f5779fd74fdfa9bda2375573227", null ],
      [ "kDSPI_Ctar2", "a00016.html#gga992d5562af4cf4c45371feb8c5c1a1bfa406d09f42f5e009617a40f4c30cc10d9", null ],
      [ "kDSPI_Ctar3", "a00016.html#gga992d5562af4cf4c45371feb8c5c1a1bfaf1df973bc8d89efbfb8d7bff51af0265", null ],
      [ "kDSPI_Ctar4", "a00016.html#gga992d5562af4cf4c45371feb8c5c1a1bfa13960000166ae1cc18b19f5c4c9405ff", null ],
      [ "kDSPI_Ctar5", "a00016.html#gga992d5562af4cf4c45371feb8c5c1a1bfad0b231829a94051ce913cd367135c1f2", null ],
      [ "kDSPI_Ctar6", "a00016.html#gga992d5562af4cf4c45371feb8c5c1a1bfaa7bb6aaabeb65811e58af0460c38e373", null ],
      [ "kDSPI_Ctar7", "a00016.html#gga992d5562af4cf4c45371feb8c5c1a1bfa6ae1a9c5243a507f36c3db1ef14c216e", null ]
    ] ],
    [ "_dspi_transfer_config_flag_for_master", "a00016.html#gac74dfe19c844271a393314a4fd13792f", [
      [ "kDSPI_MasterCtar0", "a00016.html#ggac74dfe19c844271a393314a4fd13792faf7ddf42278af30a1b81f10c4058ecddd", null ],
      [ "kDSPI_MasterCtar1", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa57508605f5d5fb0a2fb7eddfcdb89f12", null ],
      [ "kDSPI_MasterCtar2", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa6cf50df8fd75f5be1347efcaec8a68f4", null ],
      [ "kDSPI_MasterCtar3", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa70471fdf900dba881f4e742d303d307c", null ],
      [ "kDSPI_MasterCtar4", "a00016.html#ggac74dfe19c844271a393314a4fd13792faad989e96bfed1f2fbb0fcc3adb99d04b", null ],
      [ "kDSPI_MasterCtar5", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa5c3dbe0ddb8e9f3f67496592ef3ec902", null ],
      [ "kDSPI_MasterCtar6", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa5e898da1cd4e093f048f947bc751b7fa", null ],
      [ "kDSPI_MasterCtar7", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa90cf553b9933d1e3d692469e0fa5ddc3", null ],
      [ "kDSPI_MasterPcs0", "a00016.html#ggac74dfe19c844271a393314a4fd13792fad51bd34d51062d900b07801e0fd193cc", null ],
      [ "kDSPI_MasterPcs1", "a00016.html#ggac74dfe19c844271a393314a4fd13792fad07c95fafd30869cb6110d4ea3ed7ca1", null ],
      [ "kDSPI_MasterPcs2", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa116fef8c0a72727a80e72e1d1d0d0ffc", null ],
      [ "kDSPI_MasterPcs3", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa2266cc2ddbf05da3164fa6ad680facd9", null ],
      [ "kDSPI_MasterPcs4", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa3b32f4a57a5aaaaf0064d7ec1373a154", null ],
      [ "kDSPI_MasterPcs5", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa8047faff72926a57c0659f4147787353", null ],
      [ "kDSPI_MasterPcsContinuous", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa8309b1b52bbaa930bbcc3e2407f1a6f5", null ],
      [ "kDSPI_MasterActiveAfterTransfer", "a00016.html#ggac74dfe19c844271a393314a4fd13792fa458df11cc493759474f31873cfa8d4c1", null ]
    ] ],
    [ "_dspi_transfer_config_flag_for_slave", "a00016.html#ga5070a73633ee72428adda72058f7fb5f", [
      [ "kDSPI_SlaveCtar0", "a00016.html#gga5070a73633ee72428adda72058f7fb5fa6e63c217f9b392f78fb96ee039c991c8", null ]
    ] ],
    [ "_dspi_transfer_state", "a00016.html#ga97c65523863f89cddbf06691c678a7f9", [
      [ "kDSPI_Idle", "a00016.html#gga97c65523863f89cddbf06691c678a7f9ae739fb0dabff3a7cb72c39eef943a373", null ],
      [ "kDSPI_Busy", "a00016.html#gga97c65523863f89cddbf06691c678a7f9a4b636d65ab83d136e81ed31e30de4429", null ],
      [ "kDSPI_Error", "a00016.html#gga97c65523863f89cddbf06691c678a7f9a6d94f11a50f542371683efe9ea22efb9", null ]
    ] ],
    [ "DSPI_MasterInit", "a00016.html#gaadf23f732f4c1b61d6634bd17b1a36d7", null ],
    [ "DSPI_MasterGetDefaultConfig", "a00016.html#ga0061c90bc787dc1faffde79cb256e8a4", null ],
    [ "DSPI_SlaveInit", "a00016.html#gacf6cecb6b73f02eaa448634a8d705851", null ],
    [ "DSPI_SlaveGetDefaultConfig", "a00016.html#gad85a8d4e7bd2747103691a63ef9a67e1", null ],
    [ "DSPI_Deinit", "a00016.html#gaa669bb8f6438b1d4f7ec38ba180653fa", null ],
    [ "DSPI_Enable", "a00016.html#ga38a2ee1ed351246ebbdc4b242b835164", null ],
    [ "DSPI_GetStatusFlags", "a00016.html#ga11005216bf792c91894d9e670b0323f8", null ],
    [ "DSPI_ClearStatusFlags", "a00016.html#ga11454768ad4c96b65b298cccf1f0401c", null ],
    [ "DSPI_EnableInterrupts", "a00016.html#ga9b9e4c8ae54ea108952c80940e11b3a8", null ],
    [ "DSPI_DisableInterrupts", "a00016.html#gabf5c4ec1216387b8c476853e45a9bfeb", null ],
    [ "DSPI_EnableDMA", "a00016.html#ga313d41fd54ca75781bb7596b319d4849", null ],
    [ "DSPI_DisableDMA", "a00016.html#ga543b12952cb5ac404ebbdaa572628c8e", null ],
    [ "DSPI_MasterGetTxRegisterAddress", "a00016.html#gad3e8a8107cfda29dbae45fc5166d63f3", null ],
    [ "DSPI_SlaveGetTxRegisterAddress", "a00016.html#ga8912754715dfadde5473a419f7b8ff93", null ],
    [ "DSPI_GetRxRegisterAddress", "a00016.html#ga0d2bcb0a744852ab2701466a7fd974f6", null ],
    [ "DSPI_GetInstance", "a00016.html#ga55a8d287feaf774cb3a7b644171e0f2f", null ],
    [ "DSPI_SetMasterSlaveMode", "a00016.html#gac3e11f3876e81d7636a77fb268c2365a", null ],
    [ "DSPI_IsMaster", "a00016.html#gae606c91960692b493d17d067c38d67b3", null ],
    [ "DSPI_StartTransfer", "a00016.html#gac3fb40ea05b407f5b335c0a47330e3a8", null ],
    [ "DSPI_StopTransfer", "a00016.html#ga09021ebd27d4ccf5d85398b5bbf12045", null ],
    [ "DSPI_SetFifoEnable", "a00016.html#gad9112153c575eeeb6af747d9e6396514", null ],
    [ "DSPI_FlushFifo", "a00016.html#ga3cbb532b5bd6981f5cc0115f49a9ee9a", null ],
    [ "DSPI_SetAllPcsPolarity", "a00016.html#ga1c42d5efc75982041f4a66f4f1fc71a4", null ],
    [ "DSPI_MasterSetBaudRate", "a00016.html#gac76cf793dd837dd0b502770913058592", null ],
    [ "DSPI_MasterSetDelayScaler", "a00016.html#ga56d5b87114e56507c0ec2d631ffefaa2", null ],
    [ "DSPI_MasterSetDelayTimes", "a00016.html#gac60f64fd410404ebab553ee878b464c2", null ],
    [ "DSPI_MasterWriteData", "a00016.html#gabe0d615b273c4cb0eaf26d9679b73ad6", null ],
    [ "DSPI_GetDefaultDataCommandConfig", "a00016.html#gad9f3df616e7284696af57cce8f49899e", null ],
    [ "DSPI_MasterWriteDataBlocking", "a00016.html#ga70a0f7d7fe2fbce7993bbcc8c427b2b0", null ],
    [ "DSPI_MasterGetFormattedCommand", "a00016.html#ga4068b27da40c419a700badf2070fc5e4", null ],
    [ "DSPI_MasterWriteCommandDataBlocking", "a00016.html#ga0718581088422b572cb4494f26aad1f9", null ],
    [ "DSPI_SlaveWriteData", "a00016.html#ga952c2bfcb7e3ac7d3608ec16add273dc", null ],
    [ "DSPI_SlaveWriteDataBlocking", "a00016.html#gad7a98ccdb5dcd3ea9c282893b79cee79", null ],
    [ "DSPI_ReadData", "a00016.html#gaee93673062a6fb105dcf1e0541dd8b52", null ],
    [ "DSPI_SetDummyData", "a00016.html#ga6601ce5a69dfa993874baa4f2539252d", null ],
    [ "DSPI_MasterTransferCreateHandle", "a00016.html#ga63e04b92d99d795cf84df62379765a91", null ],
    [ "DSPI_MasterTransferBlocking", "a00016.html#gab2d0aa3acb2acc3cc5413314d758628b", null ],
    [ "DSPI_MasterTransferNonBlocking", "a00016.html#gad3dc7b85b448ce6e16e227d7bf3769d6", null ],
    [ "DSPI_MasterHalfDuplexTransferBlocking", "a00016.html#ga4e6b7b248a7cc158a17876b651f4c869", null ],
    [ "DSPI_MasterHalfDuplexTransferNonBlocking", "a00016.html#gaf6f6e6e90ff9819e53660a154e0a2aa1", null ],
    [ "DSPI_MasterTransferGetCount", "a00016.html#gadaf98a7213c03f10d5820d363e827a73", null ],
    [ "DSPI_MasterTransferAbort", "a00016.html#ga80633e998c10cb83685d6c64ecd33a55", null ],
    [ "DSPI_MasterTransferHandleIRQ", "a00016.html#ga195eed1bfdc0d21e7adb76a5d6d247dc", null ],
    [ "DSPI_SlaveTransferCreateHandle", "a00016.html#gadc23691aa2c06ae9076a5f0b16f33a8c", null ],
    [ "DSPI_SlaveTransferNonBlocking", "a00016.html#ga81f85324750f75b8e7248846c88d99e7", null ],
    [ "DSPI_SlaveTransferGetCount", "a00016.html#ga4134bb536420951e8ecbe8edb987d199", null ],
    [ "DSPI_SlaveTransferAbort", "a00016.html#ga7e1be1f74fd8d372ce1af52c960d1361", null ],
    [ "DSPI_SlaveTransferHandleIRQ", "a00016.html#gade8288c503cc6c7af542cdc86947ecd3", null ],
    [ "DSPI_GetDummyDataInstance", "a00016.html#ga055ff1f9fa7139ddc77f9457af96a9c3", null ],
    [ "g_dspiDummyData", "a00016.html#ga07177e709a23889fc72566d27715a597", null ]
];