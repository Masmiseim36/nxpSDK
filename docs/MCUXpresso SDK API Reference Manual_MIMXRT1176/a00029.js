var a00029 =
[
    [ "enet_qos_rx_bd_struct_t", "a00029.html#a00496", [
      [ "buff1Addr", "a00029.html#accf28abfd89366ced2164f495d160b44", null ],
      [ "reserved", "a00029.html#ac798614368613e234577c52446181db8", null ],
      [ "buff2Addr", "a00029.html#a6c94edccbe7a6df983e51a3acbf18e5b", null ],
      [ "control", "a00029.html#a8ed545bc8dc618fc1c5d8e7d20cadc2e", null ]
    ] ],
    [ "enet_qos_tx_bd_struct_t", "a00029.html#a00502", [
      [ "buff1Addr", "a00029.html#a999ee28bc3185a16c7b990b6dc4fbbdf", null ],
      [ "buff2Addr", "a00029.html#abfb71b297ae43d1b1defae37600fa9d9", null ],
      [ "buffLen", "a00029.html#aeefb6df538b4e91a00a561fe4099def5", null ],
      [ "controlStat", "a00029.html#a7e207c7efa77f2ee7d09820a3849275e", null ]
    ] ],
    [ "enet_qos_ptp_time_t", "a00029.html#a00492", [
      [ "second", "a00029.html#add6d39a367b91c7a20c091094352753b", null ],
      [ "nanosecond", "a00029.html#ae3da4f2e43aca656e9a8a0fd3c5cd386", null ]
    ] ],
    [ "enet_qos_frame_info_t", "a00029.html#a00489", [
      [ "context", "a00029.html#a8d6d69337e26cc059c8fb31296b31559", null ],
      [ "isTsAvail", "a00029.html#a9fa3a3610c46e447e82a1c48d02b2d25", null ],
      [ "timeStamp", "a00029.html#a193a8f5369399aca6e0003a4916091e5", null ]
    ] ],
    [ "enet_qos_tx_dirty_ring_t", "a00029.html#a00503", [
      [ "txDirtyBase", "a00029.html#a0b276ff01345fa60435253de7f8fb0e9", null ],
      [ "txGenIdx", "a00029.html#a19afa9bcd22178891abc916caca267d6", null ],
      [ "txConsumIdx", "a00029.html#a228ef976dbee8f699479a215180f6763", null ],
      [ "txRingLen", "a00029.html#aa0e9169a46f14aa8394ff9ea8c209685", null ],
      [ "isFull", "a00029.html#a9d9487798deba95b94fa5cbc14b0a0e2", null ]
    ] ],
    [ "enet_qos_ptp_config_t", "a00029.html#a00491", [
      [ "fineUpdateEnable", "a00029.html#aa19b5f0f23baabcdae7605265e8cb518", null ],
      [ "defaultAddend", "a00029.html#a45fe53e0992e9ab9e1bf4e9a91b8dee3", null ],
      [ "ptp1588V2Enable", "a00029.html#a94552ec255ce96f79d8a0adcbae4817d", null ],
      [ "tsRollover", "a00029.html#a85e5dbc5918519a85aa51c5a69effaef", null ]
    ] ],
    [ "enet_qos_est_gate_op_t", "a00029.html#a00487", null ],
    [ "enet_qos_est_gcl_t", "a00029.html#a00488", [
      [ "enable", "a00029.html#aa7c24dd502d2ea4f4087756cc7672335", null ],
      [ "cycleTime", "a00029.html#aa4bbd74bca70fc30465dfcf75da129ee", null ],
      [ "extTime", "a00029.html#a71f3bf7a214948774f5acf61cefd1f11", null ],
      [ "numEntries", "a00029.html#a4ea0ffe5ab809275c43283f98c828211", null ],
      [ "opList", "a00029.html#a0629686817c2783cce90e48e8e13e339", null ]
    ] ],
    [ "enet_qos_rxp_config_t", "a00029.html#a00499", [
      [ "matchEnable", "a00029.html#a78f40ba815ba108790b3c8bdb7d3a1cc", null ],
      [ "acceptFrame", "a00029.html#a7c6e3915eeae25361660345e17d59777", null ],
      [ "rejectFrame", "a00029.html#a9486256dd3053e3a98aca20091d80a9d", null ],
      [ "inverseMatch", "a00029.html#a3617e8313c26325ea501773a6f2f1a2e", null ],
      [ "nextControl", "a00029.html#a022e9fcbf6525539196969f2db03d1be", null ],
      [ "reserved", "a00029.html#a619cead25a2e02bbf50fa939d53763c8", null ],
      [ "frameOffset", "a00029.html#ad31af0928d557e54ddcdf348259832ca", null ],
      [ "okIndex", "a00029.html#a31f70ae27246ee35dc4450da27410d3e", null ],
      [ "dmaChannel", "a00029.html#af88516aee66d2ce83f935a959174b0b6", null ],
      [ "reserved2", "a00029.html#abe805b1a71760279052860e14e96c415", null ]
    ] ],
    [ "enet_qos_buffer_config_t", "a00029.html#a00483", [
      [ "rxRingLen", "a00029.html#a39a0b6f276a3ffe8fd041e71adad78b6", null ],
      [ "txRingLen", "a00029.html#aa1762a26b232fd3a64c0b2b787866d36", null ],
      [ "txDescStartAddrAlign", "a00029.html#a23d231060eaa2e6ae798088dcc23fa68", null ],
      [ "txDescTailAddrAlign", "a00029.html#a551420d738a48815f693841eee848e28", null ],
      [ "txDirtyStartAddr", "a00029.html#ada9d55f6f5d25fb24122065e572cc9c1", null ],
      [ "rxDescStartAddrAlign", "a00029.html#a6918fca8576f83a1e64b5ab9865342f5", null ],
      [ "rxDescTailAddrAlign", "a00029.html#ad218cb5d38ea9b57d92e52071362109d", null ],
      [ "rxBufferStartAddr", "a00029.html#a211af4eefd77c8d4532602758337aa50", null ],
      [ "rxBuffSizeAlign", "a00029.html#ae773859a0734fdc686ec7d4b27aa4f71", null ],
      [ "rxBuffNeedMaintain", "a00029.html#a5735d2cdd1e38beffca84bc496ababdd", null ]
    ] ],
    [ "enet_qos_cbs_config_t", "a00029.html#a00485", [
      [ "sendSlope", "a00029.html#ababc91c304f511974556b94de1efd30d", null ],
      [ "idleSlope", "a00029.html#a9c7a6fe18d0454dc8b97c79bbc65b997", null ],
      [ "highCredit", "a00029.html#a40c55cf7c2b64eb6715d63030f6694d7", null ],
      [ "lowCredit", "a00029.html#aeeaab5575ab1fb8d984301ccf7f1383d", null ]
    ] ],
    [ "enet_qos_queue_tx_config_t", "a00029.html#a00494", [
      [ "mode", "a00029.html#af09a9c54b4b910dbb2a1649f81c1645a", null ],
      [ "weight", "a00029.html#a38792ec5ef874e8d8d8eb6c013e2fdf6", null ],
      [ "priority", "a00029.html#ae1d600db536386b7add1ce1d474afee4", null ],
      [ "cbsConfig", "a00029.html#aaf710e1fac3796031ec5673350b8e16b", null ]
    ] ],
    [ "enet_qos_queue_rx_config_t", "a00029.html#a00493", [
      [ "mode", "a00029.html#a3098b50f7bded29a47889956942485f0", null ],
      [ "mapChannel", "a00029.html#aa2234b6a75331155cbe42410983b2a91", null ],
      [ "priority", "a00029.html#a798a4d4a4ba1d044ac10fba675fbba62", null ],
      [ "packetRoute", "a00029.html#a940fd07af317e481d38f9017c176f046", null ]
    ] ],
    [ "enet_qos_multiqueue_config_t", "a00029.html#a00490", [
      [ "burstLen", "a00029.html#a4c49dadbc9c2f42e278b98c2100165bc", null ],
      [ "txQueueUse", "a00029.html#a0d6ed13793b99b3d4ab221e2662c5f71", null ],
      [ "mtltxSche", "a00029.html#afe28df31a318735c7e8bb165e5942d8c", null ],
      [ "txQueueConfig", "a00029.html#adf20e2ca36fca280aeab2c7d1c4970e2", null ],
      [ "rxQueueUse", "a00029.html#a8b131cdf05788c77e70bab95c9b88f18", null ],
      [ "mtlrxSche", "a00029.html#a0acc29cb63026de0db6a503539d823d4", null ],
      [ "rxQueueConfig", "a00029.html#ad39c277d50ef34ca227c66d8317dc558", null ]
    ] ],
    [ "enet_qos_config_t", "a00029.html#a00486", [
      [ "specialControl", "a00029.html#a21081fa459f13b7b5476006cf4161e79", null ],
      [ "multiqueueCfg", "a00029.html#a980778efaa8f99c31e2745f242e3f3d1", null ],
      [ "miiMode", "a00029.html#ac75802ecf58ce0e20fd937de81f5cf1e", null ],
      [ "miiSpeed", "a00029.html#a67589b4a6c0493d0d4e0b3c1c10bb8e1", null ],
      [ "miiDuplex", "a00029.html#a8399d0479271c42ea35eec39543eb1b6", null ],
      [ "pauseDuration", "a00029.html#ab1782b2c0789c6af434047e415c7af02", null ],
      [ "ptpConfig", "a00029.html#ad6ea6d01ccbd30747dc4266bef99d364", null ],
      [ "csrClock_Hz", "a00029.html#a5dd62437055489e1f715613efb0c58e9", null ],
      [ "rxBuffAlloc", "a00029.html#ad3956611f7f29c1e4caf39b41e429a88", null ],
      [ "rxBuffFree", "a00029.html#ada29029b3397a28b70d7b8ae21370800", null ]
    ] ],
    [ "enet_qos_tx_bd_ring_t", "a00029.html#a00501", [
      [ "txBdBase", "a00029.html#ac4a08449bb5ee5f1c9ac58f268b2ee76", null ],
      [ "txGenIdx", "a00029.html#a1d8fedf37127dcc58b5f013aecebd4f3", null ],
      [ "txConsumIdx", "a00029.html#ad14da1be0c9043173055f83b446dc18d", null ],
      [ "txDescUsed", "a00029.html#ab23c3a2d0ea7f47d0bff406f1a35313c", null ],
      [ "txRingLen", "a00029.html#a7f47edbb426bff26b14e4151b453d604", null ]
    ] ],
    [ "enet_qos_rx_bd_ring_t", "a00029.html#a00495", [
      [ "rxBdBase", "a00029.html#af86dbbe22995427eb4d29b322dff28c4", null ],
      [ "rxGenIdx", "a00029.html#ac391597e29fef3f49ad78a1df1fdd2d0", null ],
      [ "rxRingLen", "a00029.html#a857dedb14fc48abb4ac37d634f5ff1f2", null ],
      [ "rxBuffSizeAlign", "a00029.html#a391eab92544ba220323c74fe7bcbcdf5", null ]
    ] ],
    [ "enet_qos_handle_t", "a00029.html#a00382", [
      [ "txQueueUse", "a00029.html#a1723b41edef7cabed501ac6d01e96709", null ],
      [ "rxQueueUse", "a00029.html#a37a30003463224125f0c440cda6b8662", null ],
      [ "doubleBuffEnable", "a00029.html#a3e9aaceff3e7aa077748701e25b85fb5", null ],
      [ "rxintEnable", "a00029.html#ae349c8ba4a86ad0189d94451d72bc1da", null ],
      [ "rxMaintainEnable", "a00029.html#afb665c2ef820bf9884ab733b2e2d6567", null ],
      [ "rxBdRing", "a00029.html#abfaeb534d5c067520e7de5f743401538", null ],
      [ "txBdRing", "a00029.html#ab45483896c1df9e246d4f51ca1d595ef", null ],
      [ "txDirtyRing", "a00029.html#a3c9376a4ab7dc146e648e194f838c516", null ],
      [ "rxBufferStartAddr", "a00029.html#ad1ad1f52dc2f44f0d39fc72352d20885", null ],
      [ "callback", "a00029.html#a9033512273d0207a36a9b1264115d22b", null ],
      [ "userData", "a00029.html#a4df60878ba7feab2c8689484a13e909d", null ],
      [ "multicastCount", "a00029.html#a251b4c30f49c2238ba843114c016af55", null ],
      [ "rxBuffAlloc", "a00029.html#a0318970c2b89c7b5705d6bbd527811d5", null ],
      [ "rxBuffFree", "a00029.html#ac5b202fb2439abf831335532fee51e60", null ]
    ] ],
    [ "enet_qos_buffer_struct_t", "a00029.html#a00484", [
      [ "buffer", "a00029.html#adf0a94c6f9bc67dde10f24c4b6d262c2", null ],
      [ "length", "a00029.html#a333ed32c4a077b95fc6105443ac0d8c7", null ]
    ] ],
    [ "enet_qos_rx_frame_error_t", "a00029.html#a00497", [
      [ "rxDstAddrFilterErr", "a00029.html#a4e3a89eea076ff75c6053549681259fc", null ],
      [ "rxSrcAddrFilterErr", "a00029.html#adb973ccb550c5726466c3180a57ed1de", null ],
      [ "rxDribbleErr", "a00029.html#a4f900a3c8dd05f3d77bcdb7dc1942df1", null ],
      [ "rxReceiveErr", "a00029.html#ae0393ca962850e1eb27a4dc7d6c8711d", null ],
      [ "rxOverFlowErr", "a00029.html#acfa0f116b86eaabacee427200ccf8c47", null ],
      [ "rxWatchDogErr", "a00029.html#a562aca3128c715752f0e5b6f7ddfaa81", null ],
      [ "rxGaintPacketErr", "a00029.html#aabcc5182c9c443d5442feb1a85e929ed", null ],
      [ "rxCrcErr", "a00029.html#a4d4db30f5ef79130fdb96a3d30361640", null ]
    ] ],
    [ "enet_qos_rx_frame_struct_t", "a00029.html#a00498", [
      [ "rxBuffArray", "a00029.html#af3e1a95d6d6299b9035953121dff1fe6", null ],
      [ "totLen", "a00029.html#a2301cd5191beda78af106ee21e2c87ab", null ],
      [ "rxAttribute", "a00029.html#a346312200bc064ec2f9c8c49639c3cb3", null ],
      [ "rxFrameError", "a00029.html#abba7d437a0a1f937a6757f158275bed1", null ]
    ] ],
    [ "enet_qos_transfer_stats_t", "a00029.html#a00500", [
      [ "statsRxFrameCount", "a00029.html#ac21e87bef8e23f47705161e50faf3143", null ],
      [ "statsRxCrcErr", "a00029.html#a4a13f88edbfa13826817d4c2fb1eba87", null ],
      [ "statsRxAlignErr", "a00029.html#ab4e496ae5b85ba9181acbcfef7e73579", null ],
      [ "statsRxLengthErr", "a00029.html#ac38b3602047be2b0b176a2d1998cae9a", null ],
      [ "statsRxFifoOverflowErr", "a00029.html#a6fa62f0111fa640d62ade48b665c10e3", null ],
      [ "statsTxFrameCount", "a00029.html#a7a21585ede1bdc1ec185860ff00e3b10", null ],
      [ "statsTxFifoUnderRunErr", "a00029.html#a228d57cccc53d531d5c56081b5f6a007", null ]
    ] ],
    [ "FSL_ENET_QOS_DRIVER_VERSION", "a00029.html#ga345a6b97f20d159818684d9d54681e73", null ],
    [ "ENET_QOS_RXDESCRIP_RD_BUFF1VALID_MASK", "a00029.html#ga6cfea6ec84cf55e2f640f145167e986c", null ],
    [ "ENET_QOS_RXDESCRIP_RD_BUFF2VALID_MASK", "a00029.html#ga66eac04c788fe1c34465990304b6ed7d", null ],
    [ "ENET_QOS_RXDESCRIP_RD_IOC_MASK", "a00029.html#ga1e935e54b44da95f0beaea52f12f3428", null ],
    [ "ENET_QOS_RXDESCRIP_RD_OWN_MASK", "a00029.html#gac920c95ddf879155ec9dcbbb31f1ac58", null ],
    [ "ENET_QOS_RXDESCRIP_WR_ERR_MASK", "a00029.html#ga51eaa36c4a01006fbacbc4f334384e6b", null ],
    [ "ENET_QOS_TXDESCRIP_RD_BL1_MASK", "a00029.html#ga95a92b2a2f8c29dab62d48f5446cc382", null ],
    [ "ENET_QOS_TXDESCRIP_WB_TTSS_MASK", "a00029.html#gacd87b6776e005fda3c97c5efd39f8391", null ],
    [ "ENET_QOS_RING_NUM_MAX", "a00029.html#ga43fe49e897156716b028fd4ab95f7979", null ],
    [ "ENET_QOS_FRAME_MAX_FRAMELEN", "a00029.html#ga25ea7cc55d8dd61af5237607d549742b", null ],
    [ "ENET_QOS_FCS_LEN", "a00029.html#ga98e4ec6e8993a221a194f9a5af2bc11b", null ],
    [ "ENET_QOS_ADDR_ALIGNMENT", "a00029.html#ga9c221c1a75dcd53e106b037b7636d7d0", null ],
    [ "ENET_QOS_BUFF_ALIGNMENT", "a00029.html#gaaa6c32f81e0822a1841a45caa928d64d", null ],
    [ "ENET_QOS_MTL_RXFIFOSIZE", "a00029.html#ga5c31f4de5f1883781a1ea73b2fd25fac", null ],
    [ "ENET_QOS_MTL_TXFIFOSIZE", "a00029.html#ga55f217113f96fa7691750d5af77f9134", null ],
    [ "ENET_QOS_MACINT_ENUM_OFFSET", "a00029.html#ga0f1e01a354c34811320beb9300d2eae0", null ],
    [ "ENET_QOS_RXP_ENTRY_COUNT", "a00029.html#ga1252b4b43062373f94363bb28dd57ebc", null ],
    [ "ENET_QOS_RXP_BUFFER_SIZE", "a00029.html#ga9ce58d6082ba0834fa870536c716fe8e", null ],
    [ "ENET_QOS_EST_WID", "a00029.html#ga06151a2423e9fe12d0fc81adec13129b", null ],
    [ "ENET_QOS_EST_DEP", "a00029.html#ga3d62fe37021da538f3bcd78c945470ef", null ],
    [ "enet_qos_rx_alloc_callback_t", "a00029.html#ga0ab5557d97101c1e160b22fcb79e770b", null ],
    [ "enet_qos_rx_free_callback_t", "a00029.html#gafc68c0aad6de0f0c18828cfb8570a6a8", null ],
    [ "enet_qos_callback_t", "a00029.html#gac4c5dc0c8948323178c50cab581519e5", [
      [ "kStatus_ENET_QOS_InitMemoryFail", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a72d260069866ccddd451afe58b1750af", null ],
      [ "kStatus_ENET_QOS_RxFrameError", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a3506d4c1b43edb9c6fce092610b016e8", null ],
      [ "kStatus_ENET_QOS_RxFrameFail", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8af145d00b3ec1a1ffce58b8e777ab7bdd", null ],
      [ "kStatus_ENET_QOS_RxFrameEmpty", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a3a2be32d8b0e83b8f32ba180792e753e", null ],
      [ "kStatus_ENET_QOS_RxFrameDrop", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a4ecaa799969f15d10c674be0a1a59902", null ],
      [ "kStatus_ENET_QOS_TxFrameBusy", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a21e86f9eccd1d265cf643bb5a7a81056", null ],
      [ "kStatus_ENET_QOS_TxFrameFail", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a89dce180ed313ee32476d7644f3c779b", null ],
      [ "kStatus_ENET_QOS_TxFrameOverLen", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a5deb8bb9071714227f747e20756169de", null ],
      [ "kStatus_ENET_QOS_Est_SwListBusy", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a7e90eaecd17eb5be1de90212654ec732", null ],
      [ "kStatus_ENET_QOS_Est_SwListWriteAbort", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8ab9244f85c0d187cd35d0631dd0da96d5", null ],
      [ "kStatus_ENET_QOS_Est_InvalidParameter", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8ab9e7c871ee27d8a31fae8e179d114748", null ],
      [ "kStatus_ENET_QOS_Est_BtrError", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8af13980dd9501a05f60a9779a945da530", null ],
      [ "kStatus_ENET_QOS_TrgtBusy", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a53308a013bef01b45ad89d6228a6535f", null ],
      [ "kStatus_ENET_QOS_Timeout", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a2a39cdbdf6e6a6b77ac7f41d205d0156", null ],
      [ "kStatus_ENET_QOS_PpsBusy", "a00029.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a4fbe02e8f93feca4d2e27ea6fc03b37e", null ]
    ] ],
    [ "enet_qos_mii_mode_t", "a00029.html#ga8e23094f0e2a07c77dff660f2a2730e3", [
      [ "kENET_QOS_MiiMode", "a00029.html#gga8e23094f0e2a07c77dff660f2a2730e3a5c37875d18f9704a58f0f253b4081ef2", null ],
      [ "kENET_QOS_RgmiiMode", "a00029.html#gga8e23094f0e2a07c77dff660f2a2730e3a3153f28104dd30435e59e54a0a7159d2", null ],
      [ "kENET_QOS_RmiiMode", "a00029.html#gga8e23094f0e2a07c77dff660f2a2730e3a9c0196d59c1a7b066d00a0bf8f25cd58", null ]
    ] ],
    [ "enet_qos_mii_speed_t", "a00029.html#ga20f699a7382c5b7d923dc51d555a652d", [
      [ "kENET_QOS_MiiSpeed10M", "a00029.html#gga20f699a7382c5b7d923dc51d555a652da3fceca3428909bf1a9c7419a45c454b0", null ],
      [ "kENET_QOS_MiiSpeed100M", "a00029.html#gga20f699a7382c5b7d923dc51d555a652dac03ff077250e403fb90558121c925f7e", null ],
      [ "kENET_QOS_MiiSpeed1000M", "a00029.html#gga20f699a7382c5b7d923dc51d555a652dabe9d70e462436fc6ae2c7750237db2e5", null ],
      [ "kENET_QOS_MiiSpeed2500M", "a00029.html#gga20f699a7382c5b7d923dc51d555a652dadd6a88b3ede104b8b30d058900c8678d", null ]
    ] ],
    [ "enet_qos_mii_duplex_t", "a00029.html#ga32493ba8fa4bc94b0f062ca91c4c1915", [
      [ "kENET_QOS_MiiHalfDuplex", "a00029.html#gga32493ba8fa4bc94b0f062ca91c4c1915ab6f1b9c2263a678c18e4853999734794", null ],
      [ "kENET_QOS_MiiFullDuplex", "a00029.html#gga32493ba8fa4bc94b0f062ca91c4c1915a4ba67a5494492008b2c91aae1e029e91", null ]
    ] ],
    [ "enet_qos_mii_normal_opcode", "a00029.html#ga6119b3237ae3e7b7d2d800b3bf453b2f", [
      [ "kENET_QOS_MiiWriteFrame", "a00029.html#gga6119b3237ae3e7b7d2d800b3bf453b2fae0b9262a6e2fdeff9d17a3ebad8f3de5", null ],
      [ "kENET_QOS_MiiReadFrame", "a00029.html#gga6119b3237ae3e7b7d2d800b3bf453b2faefd737f2de007b23f4df6506c1285ee0", null ]
    ] ],
    [ "enet_qos_dma_burstlen", "a00029.html#gad3c9e13b63229ec7557b9f8ebf57a8a2", [
      [ "kENET_QOS_BurstLen1", "a00029.html#ggad3c9e13b63229ec7557b9f8ebf57a8a2a26354682e082cfc8563b3398eb8a7bfc", null ],
      [ "kENET_QOS_BurstLen2", "a00029.html#ggad3c9e13b63229ec7557b9f8ebf57a8a2a0504485a60ea8e499827dc826a816b64", null ],
      [ "kENET_QOS_BurstLen4", "a00029.html#ggad3c9e13b63229ec7557b9f8ebf57a8a2a45e63473734207bdb94adb8d1932ea72", null ],
      [ "kENET_QOS_BurstLen8", "a00029.html#ggad3c9e13b63229ec7557b9f8ebf57a8a2a36b02413fc768aafa844515e0ed3067a", null ],
      [ "kENET_QOS_BurstLen16", "a00029.html#ggad3c9e13b63229ec7557b9f8ebf57a8a2a2f8203407a072f5144f5bd16a66e4942", null ],
      [ "kENET_QOS_BurstLen32", "a00029.html#ggad3c9e13b63229ec7557b9f8ebf57a8a2a0138a64d59fa731624f82e02fd6b7eee", null ],
      [ "kENET_QOS_BurstLen64", "a00029.html#ggad3c9e13b63229ec7557b9f8ebf57a8a2a4693b2d9965c0a036dd9a6ae6ad11d25", null ],
      [ "kENET_QOS_BurstLen128", "a00029.html#ggad3c9e13b63229ec7557b9f8ebf57a8a2a4e66a77557dcf8abe151061d0f8b4f38", null ],
      [ "kENET_QOS_BurstLen256", "a00029.html#ggad3c9e13b63229ec7557b9f8ebf57a8a2ae169c1da2f859c31fcfc4ea4de40327b", null ]
    ] ],
    [ "enet_qos_desc_flag", "a00029.html#ga240e5d334ee76b0952326020349eba31", [
      [ "kENET_QOS_MiddleFlag", "a00029.html#gga240e5d334ee76b0952326020349eba31ab7ff92e1e4e73a4bcdb11219f959d9dc", null ],
      [ "kENET_QOS_LastFlagOnly", "a00029.html#gga240e5d334ee76b0952326020349eba31a51bba80171ab10ffdaf4ab2c4c26677e", null ],
      [ "kENET_QOS_FirstFlagOnly", "a00029.html#gga240e5d334ee76b0952326020349eba31ab34ebe045be96b2eed5f900bab1634f3", null ],
      [ "kENET_QOS_FirstLastFlag", "a00029.html#gga240e5d334ee76b0952326020349eba31a896b5a8f5dce37acdd04f82d3aa7f2a4", null ]
    ] ],
    [ "enet_qos_systime_op", "a00029.html#gab1a8d5f5309bda2d54e366868f71dcc4", [
      [ "kENET_QOS_SystimeAdd", "a00029.html#ggab1a8d5f5309bda2d54e366868f71dcc4ad761bc8a86846bd32de16e0a6cdf9195", null ],
      [ "kENET_QOS_SystimeSubtract", "a00029.html#ggab1a8d5f5309bda2d54e366868f71dcc4a12eb9232e308020e25b09278bbf1f230", null ]
    ] ],
    [ "enet_qos_ts_rollover_type", "a00029.html#ga40e92222a17448e782308ead4b6e0b06", [
      [ "kENET_QOS_BinaryRollover", "a00029.html#gga40e92222a17448e782308ead4b6e0b06a2561c6baba089d8250e73953c0e3c483", null ],
      [ "kENET_QOS_DigitalRollover", "a00029.html#gga40e92222a17448e782308ead4b6e0b06a2b456ec9f4bf1b887b6eeba1e2f87783", null ]
    ] ],
    [ "enet_qos_special_config_t", "a00029.html#ga566bc955ce6b991051f65e7e21d57c65", [
      [ "kENET_QOS_DescDoubleBuffer", "a00029.html#gga566bc955ce6b991051f65e7e21d57c65aa23735225dd936225255d1e358f7f998", null ],
      [ "kENET_QOS_StoreAndForward", "a00029.html#gga566bc955ce6b991051f65e7e21d57c65a59d22382d6e3f1bec549f02437d39684", null ],
      [ "kENET_QOS_PromiscuousEnable", "a00029.html#gga566bc955ce6b991051f65e7e21d57c65a851b29d5dd75fd436d90331e7c243328", null ],
      [ "kENET_QOS_FlowControlEnable", "a00029.html#gga566bc955ce6b991051f65e7e21d57c65a9c6025000d779b1419e94a94fbc8088e", null ],
      [ "kENET_QOS_BroadCastRxDisable", "a00029.html#gga566bc955ce6b991051f65e7e21d57c65aad1d44fb46fb2d7e3dd201ac186bb7f8", null ],
      [ "kENET_QOS_MulticastAllEnable", "a00029.html#gga566bc955ce6b991051f65e7e21d57c65af2687413704ffec931b0699910bcf4ac", null ],
      [ "kENET_QOS_8023AS2KPacket", "a00029.html#gga566bc955ce6b991051f65e7e21d57c65a8dc4c0acefa490a47426c418e540250d", null ],
      [ "kENET_QOS_HashMulticastEnable", "a00029.html#gga566bc955ce6b991051f65e7e21d57c65ab1776698a43409b8b0dc4e5eacb359d9", null ]
    ] ],
    [ "enet_qos_dma_interrupt_enable_t", "a00029.html#ga730ca73e0975ce001a188cc8dfe1b82c", [
      [ "kENET_QOS_DmaTx", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82ca361af6885b37bedde511c07a338aeceb", null ],
      [ "kENET_QOS_DmaTxStop", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82ca7ea8bf03e6a4884f3c2f0a089aafefa2", null ],
      [ "kENET_QOS_DmaTxBuffUnavail", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82ca1ea936716bbd55914e2a1582ff6b1163", null ],
      [ "kENET_QOS_DmaRx", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82cab789eb764e2cfa8fb63dc018c68e9943", null ],
      [ "kENET_QOS_DmaRxBuffUnavail", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82ca2597cba9d1f22735da16b302207fa455", null ],
      [ "kENET_QOS_DmaRxStop", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82ca13984d20e33d5f136333e341a5c06669", null ],
      [ "kENET_QOS_DmaRxWatchdogTimeout", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82ca03fa2c4f7359b529dd5bf44c98eb08b1", null ],
      [ "kENET_QOS_DmaEarlyTx", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82caddc25e801da74b9c75cb753c8ff90442", null ],
      [ "kENET_QOS_DmaEarlyRx", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82cacbab28ae5dd16b387d3f82e03da29333", null ],
      [ "kENET_QOS_DmaBusErr", "a00029.html#gga730ca73e0975ce001a188cc8dfe1b82ca849102bd44e65612a87befb5c2f6dcbf", null ]
    ] ],
    [ "enet_qos_mac_interrupt_enable_t", "a00029.html#ga3cf0f90324ea61cd94ab3f9845c66fe0", null ],
    [ "enet_qos_event_t", "a00029.html#gaae17b8976ff4556c39570f060d82967b", [
      [ "kENET_QOS_RxIntEvent", "a00029.html#ggaae17b8976ff4556c39570f060d82967baad9e2e0f6ec7a0cdc792bbb0aa98d609", null ],
      [ "kENET_QOS_TxIntEvent", "a00029.html#ggaae17b8976ff4556c39570f060d82967ba076a9ec3ea7748adc582bb9941dd19f7", null ],
      [ "kENET_QOS_WakeUpIntEvent", "a00029.html#ggaae17b8976ff4556c39570f060d82967ba95a40aeb8e1d0e3f64a263bd708c130f", null ],
      [ "kENET_QOS_TimeStampIntEvent", "a00029.html#ggaae17b8976ff4556c39570f060d82967ba9cb341c6fa4a92e938780a77e22b3e82", null ]
    ] ],
    [ "enet_qos_queue_mode_t", "a00029.html#ga9bfbf2f7613c28a311156c1238d92d2e", [
      [ "kENET_QOS_AVB_Mode", "a00029.html#gga9bfbf2f7613c28a311156c1238d92d2eab536d0f7ceb334736b87e7a31435ba50", null ],
      [ "kENET_QOS_DCB_Mode", "a00029.html#gga9bfbf2f7613c28a311156c1238d92d2eaf16e0249af9d2f8c480c195cc1b12c42", null ]
    ] ],
    [ "enet_qos_mtl_multiqueue_txsche", "a00029.html#ga839bd10310c3c65d4d637c0f687cda56", [
      [ "kENET_QOS_txWeightRR", "a00029.html#gga839bd10310c3c65d4d637c0f687cda56ab1791b8c7b710f7f450d210a69448654", null ],
      [ "kENET_QOS_txWeightFQ", "a00029.html#gga839bd10310c3c65d4d637c0f687cda56a10d7900ef57dbac80dbb996b58f3c217", null ],
      [ "kENET_QOS_txDefictWeightRR", "a00029.html#gga839bd10310c3c65d4d637c0f687cda56a16a0ab621a354406a6bc200e6eb866c5", null ],
      [ "kENET_QOS_txStrPrio", "a00029.html#gga839bd10310c3c65d4d637c0f687cda56a3d131b76c7585c69abc59c67c0ee9c9f", null ]
    ] ],
    [ "enet_qos_mtl_multiqueue_rxsche", "a00029.html#ga8b43085037b455c0d3343f3dbe8df82d", [
      [ "kENET_QOS_rxStrPrio", "a00029.html#gga8b43085037b455c0d3343f3dbe8df82dac7feac973acb14ce97c34b7e9e35aec8", null ],
      [ "kENET_QOS_rxWeightStrPrio", "a00029.html#gga8b43085037b455c0d3343f3dbe8df82da335abb7475f5338f60f504c85eaa6ff9", null ]
    ] ],
    [ "enet_qos_mtl_rxqueuemap_t", "a00029.html#ga098e2a647c30816cb132975cfec56d5d", [
      [ "kENET_QOS_StaticDirctMap", "a00029.html#gga098e2a647c30816cb132975cfec56d5daf871c0858822f089471fef352c23479c", null ],
      [ "kENET_QOS_DynamicMap", "a00029.html#gga098e2a647c30816cb132975cfec56d5da85650782256adebf19b308c70cd6fc97", null ]
    ] ],
    [ "enet_qos_rx_queue_route_t", "a00029.html#ga9fcf800c85065e6b1e6293a04d315b6d", null ],
    [ "enet_qos_ptp_event_type_t", "a00029.html#ga15f5058f92551f8528130864975d3297", [
      [ "kENET_QOS_PtpEventMsgType", "a00029.html#gga15f5058f92551f8528130864975d3297a32abb8c774dbe7c1f8849d84feae2eac", null ],
      [ "kENET_QOS_PtpSrcPortIdLen", "a00029.html#gga15f5058f92551f8528130864975d3297a7435967701fab5d6668f780a32e59d52", null ],
      [ "kENET_QOS_PtpEventPort", "a00029.html#gga15f5058f92551f8528130864975d3297a501b47310136c6c07d4d05829cd22d7e", null ],
      [ "kENET_QOS_PtpGnrlPort", "a00029.html#gga15f5058f92551f8528130864975d3297ac3a8740198c45c9f64e71d0add37a3ed", null ]
    ] ],
    [ "enet_qos_ptp_pps_instance_t", "a00029.html#ga6d4aad8ec63e49967622b6ab563c2bca", [
      [ "kENET_QOS_PtpPpsIstance0", "a00029.html#gga6d4aad8ec63e49967622b6ab563c2bcaa3f74c1da5f3ab898d40186ccb19159fb", null ],
      [ "kENET_QOS_PtpPpsIstance1", "a00029.html#gga6d4aad8ec63e49967622b6ab563c2bcaa55ff225f8c63477d1e14bfe00d30e09c", null ],
      [ "kENET_QOS_PtpPpsIstance2", "a00029.html#gga6d4aad8ec63e49967622b6ab563c2bcaacc0fe4b9ef76ece612cc7db24c39c38b", null ],
      [ "kENET_QOS_PtpPpsIstance3", "a00029.html#gga6d4aad8ec63e49967622b6ab563c2bcaaa414f88909fb03a026a3d2210e309475", null ]
    ] ],
    [ "enet_qos_ptp_pps_trgt_mode_t", "a00029.html#gab9176efd1192e5ee75fd5e2cb7e4086b", [
      [ "kENET_QOS_PtpPpsTrgtModeOnlyInt", "a00029.html#ggab9176efd1192e5ee75fd5e2cb7e4086ba80feaaf0a48fd99a8cad436ae23998be", null ],
      [ "kENET_QOS_PtpPpsTrgtModeIntSt", "a00029.html#ggab9176efd1192e5ee75fd5e2cb7e4086ba4919111043e88cac1ab5ec1ecbd388e5", null ],
      [ "kENET_QOS_PtpPpsTrgtModeOnlySt", "a00029.html#ggab9176efd1192e5ee75fd5e2cb7e4086ba6003374266514a145a01b2b572aec114", null ]
    ] ],
    [ "enet_qos_ptp_pps_cmd_t", "a00029.html#gae55826e0bf15e87a6557348c0d680138", [
      [ "kENET_QOS_PtpPpsCmdNC", "a00029.html#ggae55826e0bf15e87a6557348c0d680138a8bbb83914c8b453abd5d3414d1b4065c", null ],
      [ "kENET_QOS_PtpPpsCmdSSP", "a00029.html#ggae55826e0bf15e87a6557348c0d680138a558ea3e749dbc87f73a04d8fdb8c5404", null ],
      [ "kENET_QOS_PtpPpsCmdSPT", "a00029.html#ggae55826e0bf15e87a6557348c0d680138a84637f9ce0554e0250ae2f4d9d23f209", null ],
      [ "kENET_QOS_PtpPpsCmdCS", "a00029.html#ggae55826e0bf15e87a6557348c0d680138a3ed8d8280fb0df3fa82c95670bcd77d8", null ],
      [ "kENET_QOS_PtpPpsCmdSPTAT", "a00029.html#ggae55826e0bf15e87a6557348c0d680138a76a9ed879c38c4a2cb982f3b7985626f", null ],
      [ "kENET_QOS_PtpPpsCmdSPTI", "a00029.html#ggae55826e0bf15e87a6557348c0d680138a96a65a731305813973a0b03631bc2feb", null ],
      [ "kENET_QOS_PtpPpsCmdCSPT", "a00029.html#ggae55826e0bf15e87a6557348c0d680138aaa0bcb6d49706463d4d4b1857d46ac5e", null ]
    ] ],
    [ "enet_qos_ets_list_length_t", "a00029.html#ga9f35322370b6b60f1a13ea4167cc24ea", [
      [ "kENET_QOS_Ets_List_64", "a00029.html#gga9f35322370b6b60f1a13ea4167cc24eaae9e736a7fff32b886d2d47ab0da005d6", null ],
      [ "kENET_QOS_Ets_List_128", "a00029.html#gga9f35322370b6b60f1a13ea4167cc24eaa28dd7b948bf8c3ecdbe58d4b0d43878f", null ],
      [ "kENET_QOS_Ets_List_256", "a00029.html#gga9f35322370b6b60f1a13ea4167cc24eaa691566d56362c91e107fe1354215ec14", null ],
      [ "kENET_QOS_Ets_List_512", "a00029.html#gga9f35322370b6b60f1a13ea4167cc24eaa42db61240f2cc44949f9f971ebb5223e", null ],
      [ "kENET_QOS_Ets_List_1024", "a00029.html#gga9f35322370b6b60f1a13ea4167cc24eaa4ff9487814ee15986fad9721e2a4f712", null ]
    ] ],
    [ "enet_qos_ets_gccr_addr_t", "a00029.html#gaf6fbb179186a43483426ae7b226b6427", [
      [ "kENET_QOS_Ets_btr_low", "a00029.html#ggaf6fbb179186a43483426ae7b226b6427a7fba258015112f8879eeb71d6d7d770b", null ],
      [ "kENET_QOS_Ets_btr_high", "a00029.html#ggaf6fbb179186a43483426ae7b226b6427a905f2f146bf96088c198715ba5259186", null ],
      [ "kENET_QOS_Ets_ctr_low", "a00029.html#ggaf6fbb179186a43483426ae7b226b6427a57e7e569cc7f33a3c4b362a05024f8d2", null ],
      [ "kENET_QOS_Ets_ctr_high", "a00029.html#ggaf6fbb179186a43483426ae7b226b6427a36ed4b956329a36a0abedcf2f34645ff", null ],
      [ "kENET_QOS_Ets_ter", "a00029.html#ggaf6fbb179186a43483426ae7b226b6427a045e5439df3c3b369bbb45d22fbd16b5", null ],
      [ "kENET_QOS_Ets_llr", "a00029.html#ggaf6fbb179186a43483426ae7b226b6427ab3af7472d8c78fc7419f96084c2176c7", null ]
    ] ],
    [ "enet_qos_rxp_dma_chn_t", "a00029.html#ga1215487d078b5451065c0ff00fb38162", [
      [ "kENET_QOS_Rxp_DMAChn0", "a00029.html#gga1215487d078b5451065c0ff00fb38162acb5a53c767316c78059d5355fe2d4c47", null ],
      [ "kENET_QOS_Rxp_DMAChn1", "a00029.html#gga1215487d078b5451065c0ff00fb38162a380c523055299976f96ea84cc42ac2ae", null ],
      [ "kENET_QOS_Rxp_DMAChn2", "a00029.html#gga1215487d078b5451065c0ff00fb38162acf4cb61eaca50b379747037f9d310bfa", null ],
      [ "kENET_QOS_Rxp_DMAChn3", "a00029.html#gga1215487d078b5451065c0ff00fb38162a6cfe38ac1c33202ec5ad6c0e1bf13252", null ],
      [ "kENET_QOS_Rxp_DMAChn4", "a00029.html#gga1215487d078b5451065c0ff00fb38162aa66e7f1d4c5fc4f45757a4caef6aac41", null ]
    ] ],
    [ "ENET_QOS_SetSYSControl", "a00029.html#gab399305c3e2dceb26de8a4c9f9f73cac", null ],
    [ "ENET_QOS_GetDefaultConfig", "a00029.html#ga6ea9055b33a4be6261321dd7f5d7569d", null ],
    [ "ENET_QOS_Up", "a00029.html#gac647e499d665eea89f915a50b93f1b51", null ],
    [ "ENET_QOS_Init", "a00029.html#ga52c0e0010a95ae9829c3577f09045c37", null ],
    [ "ENET_QOS_Down", "a00029.html#ga93341399ee0ea4097766f6a98fb45aa1", null ],
    [ "ENET_QOS_Deinit", "a00029.html#ga46ca98c608a9370de15f5617fbc4709c", null ],
    [ "ENET_QOS_GetInstance", "a00029.html#ga0870ec0fac7bb169360630fa6d8f36ff", null ],
    [ "ENET_QOS_DescriptorInit", "a00029.html#gad77a47ac26f190cdc3f60c42033def66", null ],
    [ "ENET_QOS_RxBufferAllocAll", "a00029.html#ga44f2b775f08f09a49d4fd0f764537d59", null ],
    [ "ENET_QOS_RxBufferFreeAll", "a00029.html#ga467612901c6e653932f4ac872c5ef39c", null ],
    [ "ENET_QOS_StartRxTx", "a00029.html#ga4b9ca12ac1da8a22e4c38780dca36783", null ],
    [ "ENET_QOS_SetMII", "a00029.html#gaa9c342789c0acf79ef8696161376addb", null ],
    [ "ENET_QOS_SetSMI", "a00029.html#ga10694fd270e85101f882e43b52a6781e", null ],
    [ "ENET_QOS_IsSMIBusy", "a00029.html#ga51c58c36a3849a8956a085d63efe5c71", null ],
    [ "ENET_QOS_ReadSMIData", "a00029.html#ga2c12b9266437867716ae3d7d9e654c79", null ],
    [ "ENET_QOS_StartSMIRead", "a00029.html#gaf0c2593857afedbc2cf9c67533a3857e", null ],
    [ "ENET_QOS_StartSMIWrite", "a00029.html#gaa2b49f2ec55d410909023055fdb570e4", null ],
    [ "ENET_QOS_StartExtC45SMIWrite", "a00029.html#gaa98a0ca8b7488d70672a78a1c02e7788", null ],
    [ "ENET_QOS_StartExtC45SMIRead", "a00029.html#ga4037e0a99a390553b61efb21fbef5118", null ],
    [ "ENET_QOS_SetMacAddr", "a00029.html#gad8b191e841d3156a403de2e0f0f16a0f", null ],
    [ "ENET_QOS_GetMacAddr", "a00029.html#gad2b22a143bcebe34386c2dc1ec4c9f69", null ],
    [ "ENET_QOS_AddMulticastGroup", "a00029.html#ga81b68ccb679585b882449a8642e2e8c5", null ],
    [ "ENET_QOS_LeaveMulticastGroup", "a00029.html#ga087e9e48729f71765dd02102776522a6", null ],
    [ "ENET_QOS_AcceptAllMulticast", "a00029.html#ga4de6ae5f76f1b10e2079677a03cd288e", null ],
    [ "ENET_QOS_RejectAllMulticast", "a00029.html#gac60ea27259fe1891aea056cef1804a37", null ],
    [ "ENET_QOS_EnterPowerDown", "a00029.html#gaa4b09bff201b897d61e4b398d97f5c75", null ],
    [ "ENET_QOS_ExitPowerDown", "a00029.html#gab334639e52d3ea5d0c9e689d420b2aff", null ],
    [ "ENET_QOS_EnableRxParser", "a00029.html#gaa765c5fefd4ba74acba3504ae4c6b884", null ],
    [ "ENET_QOS_EnableInterrupts", "a00029.html#gafdacfd766bcc3dd04323a644c16cf447", null ],
    [ "ENET_QOS_DisableInterrupts", "a00029.html#ga89619e342c567cd5baab3ac1046570dc", null ],
    [ "ENET_QOS_GetDmaInterruptStatus", "a00029.html#ga29d497ebb26af7cff3070eb93724e6af", null ],
    [ "ENET_QOS_ClearDmaInterruptStatus", "a00029.html#gaba83a7379513b0f5fb2b8925a294ab53", null ],
    [ "ENET_QOS_GetMacInterruptStatus", "a00029.html#ga3645a166f852e3ffb02e6d706bdae4b7", null ],
    [ "ENET_QOS_ClearMacInterruptStatus", "a00029.html#gae6bc3f347c1e9b9da4fef9b70c9eb280", null ],
    [ "ENET_QOS_IsTxDescriptorDmaOwn", "a00029.html#gac8ec621a37feefa4472738b84b36e501", null ],
    [ "ENET_QOS_SetupTxDescriptor", "a00029.html#ga19c8bd01196f30f847943bc3db75bd84", null ],
    [ "ENET_QOS_UpdateTxDescriptorTail", "a00029.html#gae4e06381184a91d2c9b5dd85499ddf7a", null ],
    [ "ENET_QOS_UpdateRxDescriptorTail", "a00029.html#gab4f521cc00457d584e09c23596388ad1", null ],
    [ "ENET_QOS_GetRxDescriptor", "a00029.html#gac81527ecb6bc5c1c9ceea49fec09bac6", null ],
    [ "ENET_QOS_UpdateRxDescriptor", "a00029.html#gac25fbd0c003edd2e9b970de390c68107", null ],
    [ "ENET_QOS_ConfigureRxParser", "a00029.html#ga042e27956ef36cf87b10e5f80276e811", null ],
    [ "ENET_QOS_ReadRxParser", "a00029.html#gaa5a986111b990add685668a636531a7f", null ],
    [ "ENET_QOS_EstProgramGcl", "a00029.html#gadcbf26975805ada808be7d51ff3a83d9", null ],
    [ "ENET_QOS_EstReadGcl", "a00029.html#ga3dd672b353ec1e30240200a8a78883c0", null ],
    [ "ENET_QOS_FpeEnable", "a00029.html#ga545a79fd3a997a86cadf5adb490e1108", null ],
    [ "ENET_QOS_FpeDisable", "a00029.html#ga692249c7bea8c5ac939a7329c059b06b", null ],
    [ "ENET_QOS_FpeConfigPreemptable", "a00029.html#gaecd43795226430afa3c55d2968a85b13", null ],
    [ "ENET_QOS_AVBConfigure", "a00029.html#gacd94b52adafd2e9d114a9e69b858c64c", null ],
    [ "ENET_QOS_GetStatistics", "a00029.html#ga96f65c059d7cf1c5cf2ee3d0eb17347c", null ],
    [ "ENET_QOS_CreateHandler", "a00029.html#gaf3984bbc863c4116b3fe81ae53cd993f", null ],
    [ "ENET_QOS_GetRxFrameSize", "a00029.html#gaa58e73492953ac3bb2bbf95db9c80ad7", null ],
    [ "ENET_QOS_ReadFrame", "a00029.html#gac64b2ddaf2fa723ee509903aa83e90a2", null ],
    [ "ENET_QOS_SendFrame", "a00029.html#gab60a3473b01c1a47af926a8de0a07ef8", null ],
    [ "ENET_QOS_ReclaimTxDescriptor", "a00029.html#ga364d17f1e66e667b703cfc3ad548dfa9", null ],
    [ "ENET_QOS_CommonIRQHandler", "a00029.html#gab230db67b94b80559be8574c96fbd504", null ],
    [ "ENET_QOS_SetISRHandler", "a00029.html#gae44a965430ddba82c4e6b05c76d4872a", null ],
    [ "ENET_QOS_Ptp1588CorrectTimerInCoarse", "a00029.html#ga89a7000b3d1bfc69d5627f887d831b63", null ],
    [ "ENET_QOS_Ptp1588CorrectTimerInFine", "a00029.html#ga1682923839b9f427b28325976cbcafb0", null ],
    [ "ENET_QOS_Ptp1588GetAddend", "a00029.html#gabf7a57c6420c10babde9e2f1e1180ba9", null ],
    [ "ENET_QOS_Ptp1588GetTimerNoIRQDisable", "a00029.html#ga632a6c80efe7f5489fb14d62983fa837", null ],
    [ "ENET_Ptp1588PpsControl", "a00029.html#ga01707501ffeaf0a8f345098c165f417d", null ],
    [ "ENET_QOS_Ptp1588PpsSetTrgtTime", "a00029.html#gaa3a42475be8fed8c4b8303ce59b06e67", null ],
    [ "ENET_QOS_Ptp1588PpsSetWidth", "a00029.html#gab46683da2c0fec32bcfa49f48f20db0a", null ],
    [ "ENET_QOS_Ptp1588PpsSetInterval", "a00029.html#ga7d3ad74d944c1579f41992abfcadae91", null ],
    [ "ENET_QOS_Ptp1588GetTimer", "a00029.html#gad974ea8ed642c7715136c85624742338", null ],
    [ "ENET_QOS_GetTxFrame", "a00029.html#ga45da8621ce067df28af507110531d5e4", null ],
    [ "ENET_QOS_GetRxFrame", "a00029.html#ga9cba30fed7024660632dc1ca19ae5b6e", null ],
    [ "s_enetqosClock", "a00029.html#ga71319a945e835b8a50532b2f41f50276", null ]
];