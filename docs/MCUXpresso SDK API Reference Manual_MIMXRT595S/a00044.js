var a00044 =
[
    [ "_i3c_slave_config", "a00044.html#a00381", [
      [ "enableSlave", "a00044.html#a98e59d65cd067657b19586498d34b691", null ],
      [ "isHotJoin", "a00044.html#aba869c7d335f091252724496cebf3362", null ],
      [ "staticAddr", "a00044.html#ad6485b7dbb04a6c846d15efa6414fea4", null ],
      [ "vendorID", "a00044.html#ae6f1f557ff3b6329f934a7beae3cb935", null ],
      [ "enableRandomPart", "a00044.html#a9097abc948d52048f675c885dfa70367", null ],
      [ "partNumber", "a00044.html#a61ecf90dc2ed0f876f21152896ae761d", null ],
      [ "dcr", "a00044.html#a36b69d63e3e195467fc5f0a85b92e137", null ],
      [ "bcr", "a00044.html#a033b2209cf467c1093f2d4b5712af517", null ],
      [ "hdrMode", "a00044.html#a58998e01c13dc791e93fb1ba4daaa4b4", null ],
      [ "nakAllRequest", "a00044.html#aad7780d1c07aa095322741261884a262", null ],
      [ "ignoreS0S1Error", "a00044.html#aac1db73316013d7241a0740cd25cf543", null ],
      [ "offline", "a00044.html#a1bdba99be8c955f59d4ea751fee40e5e", null ],
      [ "matchSlaveStartStop", "a00044.html#a00082ef99380792997189f5681b85f9b", null ],
      [ "maxWriteLength", "a00044.html#ac22a277e89ffffb160ec9c5c2e8c1da3", null ],
      [ "maxReadLength", "a00044.html#a93b111531dcd14e2cbe2ddad4e5ec579", null ]
    ] ],
    [ "_i3c_slave_transfer", "a00044.html#a00383", [
      [ "event", "a00044.html#aae626ab0c53ea993502930acf3d8ea76", null ],
      [ "txData", "a00044.html#a6d4e5deb1087d8ac00f9107ac23c71cb", null ],
      [ "txDataSize", "a00044.html#a0a1a87baea75b5c0dc7095c2a981f612", null ],
      [ "rxData", "a00044.html#ac2ac349b2b99e7d558937fd78ba2b4cd", null ],
      [ "rxDataSize", "a00044.html#a6baf089846fb6865bbdd008992de31f0", null ],
      [ "completionStatus", "a00044.html#a65ee3f437337750d3ed2ff6b1ed2ea9a", null ],
      [ "transferredCount", "a00044.html#a19c410943c0e2c6151bd6db66fd6b129", null ]
    ] ],
    [ "_i3c_slave_handle", "a00044.html#a00382", [
      [ "transfer", "a00044.html#a50df171f9d88c49be6d6436a77f10219", null ],
      [ "isBusy", "a00044.html#acb1b7bf3f9fd364b823dbe3db77521a7", null ],
      [ "wasTransmit", "a00044.html#a72e3e1c5a9b258169ba9786a15a5e2a1", null ],
      [ "eventMask", "a00044.html#a7d00b5d609d410c27fe7ec44dd9966b0", null ],
      [ "transferredCount", "a00044.html#a099a6c61eda5a8111ac6e721fad9b177", null ],
      [ "callback", "a00044.html#a0b9ca233519ab32f42785be0a67b1330", null ],
      [ "userData", "a00044.html#a5d399edcf39c50dbfb9554b75975bacb", null ],
      [ "txFifoSize", "a00044.html#a131bc68dc0b868b949ac47af0f3c4535", null ]
    ] ],
    [ "i3c_slave_event_t", "a00044.html#gaba4f0075492b5accfbcb247cf2513be3", null ],
    [ "i3c_slave_activity_state_t", "a00044.html#ga7a6964b6cdcbbadf3bad646f03cb5157", null ],
    [ "i3c_slave_config_t", "a00044.html#gaaf5381779ee40e0601099f82336968b1", null ],
    [ "i3c_slave_transfer_event_t", "a00044.html#ga99929d7ef76befce5c7401a5a42d2453", null ],
    [ "i3c_slave_transfer_t", "a00044.html#ga33dc0e62f6fa1c9c8556dc5adacf8614", null ],
    [ "i3c_slave_transfer_callback_t", "a00044.html#gada633855ef6083061ce6205185f29187", null ],
    [ "i3c_slave_isr_t", "a00044.html#gaa57ff17d0b3919986c395a8825e00462", null ],
    [ "_i3c_slave_flags", "a00044.html#gae5c787dbdeb2420f38e50c912e36bad1", [
      [ "kI3C_SlaveNotStopFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1ae856c2de9789a964c5df9de6b59fa5b5", null ],
      [ "kI3C_SlaveMessageFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1abb232d0753ef5e1cb528c0f70daa2028", null ],
      [ "kI3C_SlaveRequiredReadFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1afc342a0ed101c4dd987047414dba5727", null ],
      [ "kI3C_SlaveRequiredWriteFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a1b44c76f76dc7592c5a3a851691c827f", null ],
      [ "kI3C_SlaveBusDAAFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a90d1bd0889b419b41710c905a023e699", null ],
      [ "kI3C_SlaveBusHDRModeFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a8134e94cfc1a1a1e41b6d7dafecc6721", null ],
      [ "kI3C_SlaveBusStartFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a007137b4800c84cf25a31eadc687bc2e", null ],
      [ "kI3C_SlaveMatchedFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1aa7410fa50b7d84e1f3da17649389cf98", null ],
      [ "kI3C_SlaveBusStopFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a8904ae203162bf4de511bd3d44166bd0", null ],
      [ "kI3C_SlaveRxReadyFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1ab04dfa145be7ae004f3e453cc5f22975", null ],
      [ "kI3C_SlaveTxReadyFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1ac0fb318358a569c3b90a754649ad480b", null ],
      [ "kI3C_SlaveDynamicAddrChangedFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a36533bd278344d58566e0316d8c32695", null ],
      [ "kI3C_SlaveReceivedCCCFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a0237cb952ce19512f77eaea85ec7dd87", null ],
      [ "kI3C_SlaveErrorFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a603496b621ece631d6b7c66398f37254", null ],
      [ "kI3C_SlaveHDRCommandMatchFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1aa40886fd07849aa04abbf78a0e90213b", null ],
      [ "kI3C_SlaveCCCHandledFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a47c0731626e7d65f7bd99befb714082b", null ],
      [ "kI3C_SlaveEventSentFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a5e512f1349b7b6d5d245be7496f9d9a2", null ],
      [ "kI3C_SlaveIbiDisableFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a111fbbfc5bd0296baaa460209acbb17b", null ],
      [ "kI3C_SlaveMasterRequestDisabledFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1ae2da05c79b6d1dcbf7087b638c95351f", null ],
      [ "kI3C_SlaveHotJoinDisabledFlag", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1af43952b3f9ba151711d1d6b105d956d7", null ],
      [ "kI3C_SlaveClearFlags", "a00044.html#ggae5c787dbdeb2420f38e50c912e36bad1a085cabea22a033d45af3bdd11ef20558", null ]
    ] ],
    [ "_i3c_slave_error_flags", "a00044.html#ga806dc86e40128eb6bf3e0853537b2b9b", [
      [ "kI3C_SlaveErrorOverrunFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9bab6fbf473367003f04876296c2fbca2af", null ],
      [ "kI3C_SlaveErrorUnderrunFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9ba15a719f050edf51372b2cb7eacb54f4f", null ],
      [ "kI3C_SlaveErrorUnderrunNakFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9ba6890862c30f340f357ab162c6f193ec0", null ],
      [ "kI3C_SlaveErrorTermFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9ba712354ffcf66274ceca67defb71a3eb0", null ],
      [ "kI3C_SlaveErrorInvalidStartFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9ba55e1ca378f58f8528cd7f7da6b34997f", null ],
      [ "kI3C_SlaveErrorSdrParityFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9baf97cf443cb9b10aa179e4c01edebfef5", null ],
      [ "kI3C_SlaveErrorHdrParityFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9ba2c9b8ea22108348a0ddb649b6caa6c10", null ],
      [ "kI3C_SlaveErrorHdrCRCFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9ba7aa919a09ffc845467c720d4cec2de62", null ],
      [ "kI3C_SlaveErrorS0S1Flag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9bae001a006772822e5feeba1a48447c8d7", null ],
      [ "kI3C_SlaveErrorOverreadFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9baf2c8a7e4796255f62eaa3f879bf42dfe", null ],
      [ "kI3C_SlaveErrorOverwriteFlag", "a00044.html#gga806dc86e40128eb6bf3e0853537b2b9ba397e6eebaeae742add9b14dc2a72a07f", null ]
    ] ],
    [ "_i3c_slave_event", "a00044.html#gae8b230de231b4fa946c919b0b68f5340", [
      [ "kI3C_SlaveEventNormal", "a00044.html#ggae8b230de231b4fa946c919b0b68f5340aa0edb1d7b4740e23426ab265c7f76da4", null ],
      [ "kI3C_SlaveEventIBI", "a00044.html#ggae8b230de231b4fa946c919b0b68f5340a42c92ea9bd328c3b65d0e7fc6674c338", null ],
      [ "kI3C_SlaveEventMasterReq", "a00044.html#ggae8b230de231b4fa946c919b0b68f5340af81749873ca6b6a227c090fd5bceeee8", null ],
      [ "kI3C_SlaveEventHotJoinReq", "a00044.html#ggae8b230de231b4fa946c919b0b68f5340a1989c0750ed0d920482b8b65d98a73b4", null ]
    ] ],
    [ "_i3c_slave_activity_state", "a00044.html#ga681c551b7705bf4b4a4c5ded645dbf88", [
      [ "kI3C_SlaveNoLatency", "a00044.html#gga681c551b7705bf4b4a4c5ded645dbf88ae04aca510c43014f567bb5ecc92b055a", null ],
      [ "kI3C_SlaveLatency1Ms", "a00044.html#gga681c551b7705bf4b4a4c5ded645dbf88acba58e3c0ddebac15feacc8810aa36e9", null ],
      [ "kI3C_SlaveLatency100Ms", "a00044.html#gga681c551b7705bf4b4a4c5ded645dbf88a7cc603734f6cea7f36f11f6e82bbec2a", null ],
      [ "kI3C_SlaveLatency10S", "a00044.html#gga681c551b7705bf4b4a4c5ded645dbf88a886e7c36aa0320db1a27769d23bbe245", null ]
    ] ],
    [ "_i3c_slave_transfer_event", "a00044.html#gad2e5167cb9cabc1582fedcec5786104c", [
      [ "kI3C_SlaveAddressMatchEvent", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104ca4ba7cde181fc4720255985ac9669563b", null ],
      [ "kI3C_SlaveTransmitEvent", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104cac1ddc4bae9e31c516d468c44c644438b", null ],
      [ "kI3C_SlaveReceiveEvent", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104cafc30108371d78ede74f4a1c9724d22b8", null ],
      [ "kI3C_SlaveRequiredTransmitEvent", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104ca16e7360bba643f1a7f699ac0d9ff5033", null ],
      [ "kI3C_SlaveStartEvent", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104ca3a226dce66e90a81b3766c9f4ab61128", null ],
      [ "kI3C_SlaveHDRCommandMatchEvent", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104cace186b327e1875eb407912f73feaa2d0", null ],
      [ "kI3C_SlaveCompletionEvent", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104cafd02679de07ec01bbda9eddff9d51142", null ],
      [ "kI3C_SlaveRequestSentEvent", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104cada90e0bf341a85503f67d12f6845ffe0", null ],
      [ "kI3C_SlaveReceivedCCCEvent", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104cad241179a27080e858b3b7e7c8f6069e8", null ],
      [ "kI3C_SlaveAllEvents", "a00044.html#ggad2e5167cb9cabc1582fedcec5786104cac872741fd80ab9d76f9ec145a674f53c", null ]
    ] ],
    [ "I3C_SlaveGetDefaultConfig", "a00041.html#gada6de6140ea7a8fd8f9b188f6ca8d561", null ],
    [ "I3C_SlaveInit", "a00041.html#ga6cbdffff9439d0b1ee27682a5c592c34", null ],
    [ "I3C_SlaveDeinit", "a00041.html#gaaf8b8a5e1974d969dfb4769ece3f2d0c", null ],
    [ "I3C_SlaveEnable", "a00041.html#ga4ee0343d4b79a3a929821af22b227949", null ],
    [ "I3C_SlaveGetStatusFlags", "a00041.html#gaf483efbd8d9c2569369d4d6d9ee043c7", null ],
    [ "I3C_SlaveClearStatusFlags", "a00041.html#ga41de2b6422398d3aed02cfdbda87ded5", null ],
    [ "I3C_SlaveGetErrorStatusFlags", "a00041.html#gaf6c4e955676f72510a17e7cf9797d389", null ],
    [ "I3C_SlaveClearErrorStatusFlags", "a00041.html#gaff291ac645d00d688a3c4539362a9e3a", null ],
    [ "I3C_SlaveGetActivityState", "a00041.html#gab93bd6b3583b8382d1d5f40795004f73", null ],
    [ "I3C_SlaveEnableInterrupts", "a00041.html#ga2af3e5de992c1276297273a63348d457", null ],
    [ "I3C_SlaveDisableInterrupts", "a00041.html#ga9e57f2e719be892e00d0a2e418a7d36a", null ],
    [ "I3C_SlaveGetEnabledInterrupts", "a00041.html#ga7e7f2933e71c84110713583b86756728", null ],
    [ "I3C_SlaveGetPendingInterrupts", "a00041.html#ga1ca3ffdca28af282a70156dd851dc6e6", null ],
    [ "I3C_SlaveEnableDMA", "a00041.html#ga295e383ac5ea2f4e1d1ce23507f6235d", null ],
    [ "I3C_SlaveGetTxFifoAddress", "a00041.html#ga7105aa1d37198b11eb064634c4ab55a3", null ],
    [ "I3C_SlaveGetRxFifoAddress", "a00041.html#ga4d7c6a568b4031a0036521a834362d3c", null ],
    [ "I3C_SlaveSetWatermarks", "a00041.html#ga636c51a8e9043322a0ac215928c2e46d", null ],
    [ "I3C_SlaveGetFifoCounts", "a00041.html#ga5598ac74ca66fa9c73998d43a3b48f5c", null ],
    [ "I3C_SlaveRequestEvent", "a00041.html#gab024187605c4c4bf9256a36e4e217b2e", null ],
    [ "I3C_SlaveSend", "a00041.html#gada1fa684cab9859f17724bc800b35b19", null ],
    [ "I3C_SlaveReceive", "a00041.html#gab35cd6ef6042ccbd3534a4b57a3dcf37", null ],
    [ "I3C_SlaveTransferCreateHandle", "a00044.html#ga3c0a5fc262795b92f8a68b6e20c572e5", null ],
    [ "I3C_SlaveTransferNonBlocking", "a00044.html#ga6105a83b8c488151a17eb8d93d51f00d", null ],
    [ "I3C_SlaveTransferGetCount", "a00044.html#ga9f19df9a2dd5d99440e7707aa662d49c", null ],
    [ "I3C_SlaveTransferAbort", "a00044.html#ga2d2b65294f290b2dd43ac2707f3ca87e", null ],
    [ "I3C_SlaveTransferHandleIRQ", "a00044.html#gaf604d3e51fccadaa37e28bd6ba6d37c3", null ],
    [ "I3C_SlaveRequestIBIWithData", "a00044.html#ga41456fa4696a936a2d7419667f39c6b8", null ],
    [ "I3C_SlaveRequestIBIWithSingleData", "a00044.html#gad2b8e86e437281cd6d464930e55cc8be", null ]
];