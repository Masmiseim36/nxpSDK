var a00041 =
[
    [ "i3c_register_ibi_addr_t", "a00041.html#a00383", [
      [ "address", "a00041.html#a0aba1cfc2aea5858f2a660d978448c63", null ],
      [ "ibiHasPayload", "a00041.html#a3f3521acf6aa258181bb482c47d2d0fd", null ]
    ] ],
    [ "i3c_baudrate_hz_t", "a00041.html#a00378", [
      [ "i2cBaud", "a00041.html#af36092c5749c19f9cecdc3c2ed1b2aac", null ],
      [ "i3cPushPullBaud", "a00041.html#ab2a4e7b867c2e2f7f5db172b0fac15ad", null ],
      [ "i3cOpenDrainBaud", "a00041.html#a366afd993b96f79934271321b567048e", null ]
    ] ],
    [ "i3c_master_config_t", "a00041.html#a00381", [
      [ "enableMaster", "a00041.html#a6a6d2101ea54756fdc8dfb62c1b5c6d7", null ],
      [ "disableTimeout", "a00041.html#a2f44f7ffd96f13965390705b0ccf7857", null ],
      [ "hKeep", "a00041.html#a8063af1c56f6e56568e9ed521264d552", null ],
      [ "enableOpenDrainStop", "a00041.html#a02607c05c70d63d6d6c0a13d3bffb2eb", null ],
      [ "enableOpenDrainHigh", "a00041.html#aacb29404b3812093a6585d23378493b9", null ],
      [ "baudRate_Hz", "a00041.html#ade8d4a66d41250e92500504891f30914", null ]
    ] ],
    [ "i3c_master_transfer_callback_t", "a00041.html#a00382", [
      [ "slave2Master", "a00041.html#ad9fee2ce6f680841ff6e9c0d0c887482", null ],
      [ "ibiCallback", "a00041.html#a6d42aad3611beb397bcc8ec4184def61", null ],
      [ "transferComplete", "a00041.html#aa40d4a35313debc5c67134bc953d50e0", null ]
    ] ],
    [ "i3c_master_transfer_t", "a00041.html#a00301", [
      [ "flags", "a00041.html#aec9a81ea02e19c36c6643dc85ed868d2", null ],
      [ "slaveAddress", "a00041.html#a33ff10d5cd0a6221d5fda6c003dcaf0e", null ],
      [ "direction", "a00041.html#a8382b4f8a4e3f4cac7c0c756d2bb2f98", null ],
      [ "subaddress", "a00041.html#a0f2fc9b2669082a96709c7c12b119f8b", null ],
      [ "subaddressSize", "a00041.html#a0c35fb2dca82119d1e4cda03606ceaca", null ],
      [ "data", "a00041.html#a899637beb77bd57c17a20e8f95880e98", null ],
      [ "dataSize", "a00041.html#a998734c1222cdd21ac53a0003fe6aacf", null ],
      [ "busType", "a00041.html#a80a8cf853d5884d1b358cf549996d306", null ],
      [ "ibiResponse", "a00041.html#a20a11a4b7bbd43315ed03c91762f5a53", null ]
    ] ],
    [ "i3c_master_handle_t", "a00041.html#a00300", [
      [ "state", "a00041.html#a9a67e46c098a6617d683a2928728d7bb", null ],
      [ "remainingBytes", "a00041.html#a4f67af21e75862ddda5f0e98f35c15da", null ],
      [ "isReadTerm", "a00041.html#ac89ead0f9ab8ad4abd308fcd3c618d5f", null ],
      [ "transfer", "a00041.html#acd7811659db203b29917081f05064b19", null ],
      [ "ibiAddress", "a00041.html#a8eb4f2abe0e545437bddecd62f2735a2", null ],
      [ "ibiBuff", "a00041.html#a1553f869fb33fa65c6f20a1ddebf5b6d", null ],
      [ "ibiPayloadSize", "a00041.html#a67612839b8c8fc5a284a7a31c7e75926", null ],
      [ "ibiType", "a00041.html#ab7796be272d61442760ac3be1a1f2dd8", null ],
      [ "callback", "a00041.html#ae2b8cec2ec91853b7c2afb57ae8f8147", null ],
      [ "userData", "a00041.html#aa6b49e4c9a9a7384bf94217687a76b0b", null ]
    ] ],
    [ "i3c_master_isr_t", "a00041.html#ga8ae14ad9c81ffec45e65b6683d47755b", null ],
    [ "_i3c_master_flags", "a00041.html#ga2a0f8dd8e33aa64908b21e0b68395519", [
      [ "kI3C_MasterBetweenFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519a8ff3a76607ce79f9d7c124cdd28c7985", null ],
      [ "kI3C_MasterNackDetectFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519aa0aff6bed7c97bb835e07fc625de5809", null ],
      [ "kI3C_MasterSlaveStartFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519ab2f6d4d678edfa717c54fb6cf4bfd1bc", null ],
      [ "kI3C_MasterControlDoneFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519a8244ed495028caf10f1ce14cfe3fab67", null ],
      [ "kI3C_MasterCompleteFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519a4d44f8f175fef7343b9fb596e20c00f7", null ],
      [ "kI3C_MasterRxReadyFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519a14de6f1976c7d735185515ec5b5814f5", null ],
      [ "kI3C_MasterTxReadyFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519a35f46f7a7c4ed5d30ebaedf5e6e814b6", null ],
      [ "kI3C_MasterArbitrationWonFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519a2c98c21244371283c882e8a9e4fff2c8", null ],
      [ "kI3C_MasterErrorFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519ace023ef9d3bb53885dc47c3cc8c16f07", null ],
      [ "kI3C_MasterSlave2MasterFlag", "a00041.html#gga2a0f8dd8e33aa64908b21e0b68395519a46c1c37b78affaf33992e72e58cdf9b2", null ]
    ] ],
    [ "_i3c_master_error_flags", "a00041.html#gab69021e82ffdab91ef787f274bfdd86b", [
      [ "kI3C_MasterErrorNackFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86baf0bfe0aae6e125b4bf35a77bb678f17f", null ],
      [ "kI3C_MasterErrorWriteAbortFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86ba39ec7bb50df7ed8e84766abad50f6d35", null ],
      [ "kI3C_MasterErrorTermFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86baf98642e744263a7525d02fe748cd285d", null ],
      [ "kI3C_MasterErrorParityFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86ba40bd413dce2f5bbc695ceffdd2b42af5", null ],
      [ "kI3C_MasterErrorCrcFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86ba6a2005160da9b774497f5f72f487324d", null ],
      [ "kI3C_MasterErrorReadFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86ba231f0e224dc4268be8777f32d1c0ff26", null ],
      [ "kI3C_MasterErrorWriteFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86bafaa9977cc9e484666d7549eb2613b6a9", null ],
      [ "kI3C_MasterErrorMsgFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86baf2b318b946c16c3b25104ba94da61ec2", null ],
      [ "kI3C_MasterErrorInvalidReqFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86ba486826076dadf1a412dcfeb48319864e", null ],
      [ "kI3C_MasterErrorTimeoutFlag", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86ba20e448faba6a6973622f70828f741bed", null ],
      [ "kI3C_MasterAllErrorFlags", "a00041.html#ggab69021e82ffdab91ef787f274bfdd86ba641187e24514a7d6f6172f520c03df58", null ]
    ] ],
    [ "i3c_master_state_t", "a00041.html#gae3484383b548d3750ff6f8f77ed899fd", [
      [ "kI3C_MasterStateIdle", "a00041.html#ggae3484383b548d3750ff6f8f77ed899fdae2a212999442c1358a3f33475a0813bb", null ],
      [ "kI3C_MasterStateSlvReq", "a00041.html#ggae3484383b548d3750ff6f8f77ed899fda7de28db44e184a12fc9e84d4f41dca3a", null ],
      [ "kI3C_MasterStateMsgSdr", "a00041.html#ggae3484383b548d3750ff6f8f77ed899fda5e8c73cd884dd233d97a15525103cb67", null ],
      [ "kI3C_MasterStateNormAct", "a00041.html#ggae3484383b548d3750ff6f8f77ed899fda3cfdc942df67efb2aa8fb53554f13e9c", null ],
      [ "kI3C_MasterStateDdr", "a00041.html#ggae3484383b548d3750ff6f8f77ed899fda63936c90c187c844aad943556884548e", null ],
      [ "kI3C_MasterStateDaa", "a00041.html#ggae3484383b548d3750ff6f8f77ed899fdab660f6fe2aa6819387189d63892c8c05", null ],
      [ "kI3C_MasterStateIbiAck", "a00041.html#ggae3484383b548d3750ff6f8f77ed899fda2608c0d6a897330db88a1abd5f84b130", null ],
      [ "kI3C_MasterStateIbiRcv", "a00041.html#ggae3484383b548d3750ff6f8f77ed899fda0f04e85ebd34666de5e2f23dde487bd4", null ]
    ] ],
    [ "i3c_master_enable_t", "a00041.html#ga62d1c8f4eab5009b4a6ab4605efde92c", [
      [ "kI3C_MasterOff", "a00041.html#gga62d1c8f4eab5009b4a6ab4605efde92ca90a4b15c92e54a38cc163c4c261be860", null ],
      [ "kI3C_MasterOn", "a00041.html#gga62d1c8f4eab5009b4a6ab4605efde92cacc6b2b44dd164dd8b21c5b6cd4c17568", null ],
      [ "kI3C_MasterCapable", "a00041.html#gga62d1c8f4eab5009b4a6ab4605efde92ca5c985cf628a99366ba2295e2beabfae9", null ]
    ] ],
    [ "i3c_master_hkeep_t", "a00041.html#ga2474a6d1a76544ae84be70e8c5eac2a1", [
      [ "kI3C_MasterHighKeeperNone", "a00041.html#gga2474a6d1a76544ae84be70e8c5eac2a1ac4aed7c1fde2d52e879f32f4993f0a3f", null ],
      [ "kI3C_MasterHighKeeperWiredIn", "a00041.html#gga2474a6d1a76544ae84be70e8c5eac2a1a817f00e81af0c6fdab88aa33f082b812", null ],
      [ "kI3C_MasterPassiveSDA", "a00041.html#gga2474a6d1a76544ae84be70e8c5eac2a1a1de048b5f803f15ea17024af3848b15e", null ],
      [ "kI3C_MasterPassiveSDASCL", "a00041.html#gga2474a6d1a76544ae84be70e8c5eac2a1a5e27edc1f84d57c853099281a101e8d6", null ]
    ] ],
    [ "i3c_bus_request_t", "a00041.html#gada557697fe6a91af58427b97ede0105b", [
      [ "kI3C_RequestNone", "a00041.html#ggada557697fe6a91af58427b97ede0105bad058e774905a6141d71c52eba8248392", null ],
      [ "kI3C_RequestEmitStartAddr", "a00041.html#ggada557697fe6a91af58427b97ede0105ba0bf318a218b3d00bf2472a213b62139d", null ],
      [ "kI3C_RequestEmitStop", "a00041.html#ggada557697fe6a91af58427b97ede0105ba5d719f4016a7422cb17e35c135ed3704", null ],
      [ "kI3C_RequestIbiAckNack", "a00041.html#ggada557697fe6a91af58427b97ede0105ba90a891d58eccc4d2d2d02f683e9415cd", null ],
      [ "kI3C_RequestProcessDAA", "a00041.html#ggada557697fe6a91af58427b97ede0105ba7173a5adca1aeacbd5bc6ac632541679", null ],
      [ "kI3C_RequestForceExit", "a00041.html#ggada557697fe6a91af58427b97ede0105bae5c6c18e3ad3df9b43b81e4a048673d3", null ],
      [ "kI3C_RequestAutoIbi", "a00041.html#ggada557697fe6a91af58427b97ede0105baf0b555309fd5b259a3e3423bc25fecba", null ]
    ] ],
    [ "i3c_bus_type_t", "a00041.html#ga577aa3f3b418d0ee796bac0387aedb39", [
      [ "kI3C_TypeI3CSdr", "a00041.html#gga577aa3f3b418d0ee796bac0387aedb39a477e572e74c815e5a1d3a6500924391f", null ],
      [ "kI3C_TypeI2C", "a00041.html#gga577aa3f3b418d0ee796bac0387aedb39ae5ece1a40cffb61e433e3fd20da012c7", null ],
      [ "kI3C_TypeI3CDdr", "a00041.html#gga577aa3f3b418d0ee796bac0387aedb39afe7c812aa683f7caf477fea7498be301", null ]
    ] ],
    [ "i3c_ibi_response_t", "a00041.html#ga90b015a217582a74e0514d134a8bcce6", [
      [ "kI3C_IbiRespAck", "a00041.html#gga90b015a217582a74e0514d134a8bcce6a027d9b083f9df866ad7ce202a6acd2da", null ],
      [ "kI3C_IbiRespNack", "a00041.html#gga90b015a217582a74e0514d134a8bcce6a2519a90b052159e72a450f6075707621", null ],
      [ "kI3C_IbiRespAckMandatory", "a00041.html#gga90b015a217582a74e0514d134a8bcce6a5d72ada5b230d6dbe23c9f99d22fce65", null ],
      [ "kI3C_IbiRespManual", "a00041.html#gga90b015a217582a74e0514d134a8bcce6a2bacf9f2587305a22c8bcf60046934e5", null ]
    ] ],
    [ "i3c_ibi_type_t", "a00041.html#ga81ac58196d67e74a0a3702c3e760ede7", [
      [ "kI3C_IbiNormal", "a00041.html#gga81ac58196d67e74a0a3702c3e760ede7aacf315a85acf6a6edb44ac3220388f3c", null ],
      [ "kI3C_IbiHotJoin", "a00041.html#gga81ac58196d67e74a0a3702c3e760ede7abb64fab19a64f6f43c9abdba6fe1c41c", null ],
      [ "kI3C_IbiMasterRequest", "a00041.html#gga81ac58196d67e74a0a3702c3e760ede7adb8186fbcd536cf85585f9ad56f4e7d2", null ]
    ] ],
    [ "i3c_ibi_state_t", "a00041.html#gaa9259b798e25f5d6d4f5b95589647c50", [
      [ "kI3C_IbiReady", "a00041.html#ggaa9259b798e25f5d6d4f5b95589647c50a97328db20756985833907feb8562e3f0", null ],
      [ "kI3C_IbiDataBuffNeed", "a00041.html#ggaa9259b798e25f5d6d4f5b95589647c50a047ee66e86f72143abce63dc7e66584a", null ],
      [ "kI3C_IbiAckNackPending", "a00041.html#ggaa9259b798e25f5d6d4f5b95589647c50a0fc3b6e473019f09ba573da6057ac5eb", null ]
    ] ],
    [ "i3c_direction_t", "a00041.html#gaddaff079d09512f9b359796b33c2df9b", [
      [ "kI3C_Write", "a00041.html#ggaddaff079d09512f9b359796b33c2df9ba04b8a24a8e847c1d15d8c2d7ffe5e2ec", null ],
      [ "kI3C_Read", "a00041.html#ggaddaff079d09512f9b359796b33c2df9babc5f006e698ffdd4f3b59ac988eeb717", null ]
    ] ],
    [ "i3c_tx_trigger_level_t", "a00041.html#gae86dffadf745f95115c1cc2c61994997", [
      [ "kI3C_TxTriggerOnEmpty", "a00041.html#ggae86dffadf745f95115c1cc2c61994997a08c0d3ebac516754a047df98e9bb0075", null ],
      [ "kI3C_TxTriggerUntilOneQuarterOrLess", "a00041.html#ggae86dffadf745f95115c1cc2c61994997ad53afbc7533eebab3e5580355a7e712f", null ],
      [ "kI3C_TxTriggerUntilOneHalfOrLess", "a00041.html#ggae86dffadf745f95115c1cc2c61994997a653aabb048af1f5c6dab8cd57d1f9a66", null ],
      [ "kI3C_TxTriggerUntilOneLessThanFull", "a00041.html#ggae86dffadf745f95115c1cc2c61994997a0f44ab411feaea7047b7017f079003f5", null ]
    ] ],
    [ "i3c_rx_trigger_level_t", "a00041.html#ga797fef11e1873212a1a02dac11a3b946", [
      [ "kI3C_RxTriggerOnNotEmpty", "a00041.html#gga797fef11e1873212a1a02dac11a3b946acb9e166bdadad37ddf79996b4673a58a", null ],
      [ "kI3C_RxTriggerUntilOneQuarterOrMore", "a00041.html#gga797fef11e1873212a1a02dac11a3b946a3b16ac7cd09d6b2ad8ceef590d625299", null ],
      [ "kI3C_RxTriggerUntilOneHalfOrMore", "a00041.html#gga797fef11e1873212a1a02dac11a3b946a0d00a073e7eb653b7bdc5430ebd1df8e", null ],
      [ "kI3C_RxTriggerUntilThreeQuarterOrMore", "a00041.html#gga797fef11e1873212a1a02dac11a3b946a205ed64836752d035fa7f6be35b1b6b7", null ]
    ] ],
    [ "_i3c_master_transfer_flags", "a00041.html#gae6afbe5acf604875c68cf03b5d733a94", [
      [ "kI3C_TransferDefaultFlag", "a00041.html#ggae6afbe5acf604875c68cf03b5d733a94a83180edbcbe8754fe9ff03c4acaa8a46", null ],
      [ "kI3C_TransferNoStartFlag", "a00041.html#ggae6afbe5acf604875c68cf03b5d733a94ae79f0063a09bc4acc5b76da7ece1bd6c", null ],
      [ "kI3C_TransferRepeatedStartFlag", "a00041.html#ggae6afbe5acf604875c68cf03b5d733a94ad59f43a07a7ceee3c6cd00ae162b4961", null ],
      [ "kI3C_TransferNoStopFlag", "a00041.html#ggae6afbe5acf604875c68cf03b5d733a94adbdc132591ba66ce34e9c2eb441265c9", null ],
      [ "kI3C_TransferWordsFlag", "a00041.html#ggae6afbe5acf604875c68cf03b5d733a94ada0a0ce241c2430131f6038ab249f67f", null ]
    ] ],
    [ "I3C_MasterGetDefaultConfig", "a00041.html#ga5bb9fd313269a1296f45be247e3904be", null ],
    [ "I3C_MasterInit", "a00041.html#ga47771e8dec518671a3e7945c9bf6a2a4", null ],
    [ "I3C_MasterDeinit", "a00041.html#ga6b5cdc9977f3d568132d84cb011cb69b", null ],
    [ "I3C_MasterEnable", "a00041.html#ga7124a74e4a514c069b31728a4ea64af8", null ],
    [ "I3C_MasterGetStatusFlags", "a00041.html#gaaf17a339d4238b58f60babf48afbe7d7", null ],
    [ "I3C_MasterClearStatusFlags", "a00041.html#gaf7351718ad22500c7c74f6b02eb96c26", null ],
    [ "I3C_MasterGetErrorStatusFlags", "a00041.html#ga9a80f1ef11fb65a007e19c8fbef8eba4", null ],
    [ "I3C_MasterClearErrorStatusFlags", "a00041.html#gac99b8dbf62c36b56b07cb87ea47d308e", null ],
    [ "I3C_MasterGetState", "a00041.html#gad0d230f2f24b57ba33207abfe0c3ed71", null ],
    [ "I3C_MasterEnableInterrupts", "a00041.html#gab070f24eca7c14de0f202e86ccf79bc8", null ],
    [ "I3C_MasterDisableInterrupts", "a00041.html#ga5189e790cd0312fcc6622916775f16df", null ],
    [ "I3C_MasterGetEnabledInterrupts", "a00041.html#ga01a44ea52ec3d15ca30d061337ff1827", null ],
    [ "I3C_MasterGetPendingInterrupts", "a00041.html#ga87173f5fefd47e7c2053122c01063ecc", null ],
    [ "I3C_MasterEnableDMA", "a00041.html#gaaac8fb39851725b47a5a6b6bf9c9d2b1", null ],
    [ "I3C_MasterGetTxFifoAddress", "a00041.html#gab95ad054974ebcd9ed068ee0fc7ce559", null ],
    [ "I3C_MasterGetRxFifoAddress", "a00041.html#ga474c9c708a422e02b8424ee3b29efe78", null ],
    [ "I3C_MasterSetWatermarks", "a00041.html#ga3b13dc565fd9b4be9932a6afb94fdfd3", null ],
    [ "I3C_MasterGetFifoCounts", "a00041.html#gaea55267e03330389fda1a501eb13e7f7", null ],
    [ "I3C_MasterSetBaudRate", "a00041.html#ga0c7f4d43f1948cbb8957bf67aafd6d2f", null ],
    [ "I3C_MasterGetBusIdleState", "a00041.html#gaf67ff2106e6626441b3118b496a627a2", null ],
    [ "I3C_MasterStart", "a00041.html#ga71f75340c40fce0d0ec093f848366df2", null ],
    [ "I3C_MasterRepeatedStart", "a00041.html#ga7ba470989a3d1a53fda1bdf802ca0b5a", null ],
    [ "I3C_MasterRepeatedStartWithRxSize", "a00041.html#ga5f35768eca781b95557aa95213099417", null ],
    [ "I3C_MasterSend", "a00041.html#ga6d5de44d3975df49c4e4dff51ebece90", null ],
    [ "I3C_MasterReceive", "a00041.html#ga4f28b5ee102b91f2cffeb6c2f146c123", null ],
    [ "I3C_MasterStop", "a00041.html#ga2ebea68fea7844aeb022ac0923fdfebb", null ],
    [ "I3C_MasterEmitRequest", "a00041.html#ga692d278a6223eafd86098c0554b09dcc", null ],
    [ "I3C_MasterEmitIBIResponse", "a00041.html#ga4c977aa668686d0e4067fad3f6bac533", null ],
    [ "I3C_MasterRegisterIBI", "a00041.html#gacae9d09c717d6b77b1e333a8dfadbfce", null ],
    [ "I3C_MasterGetIBIRules", "a00041.html#gad773adc7b026c0cbeab0e44de77ca11c", null ],
    [ "I3C_MasterProcessDAA", "a00041.html#gafcac95b489b0693850322138db6c8c61", null ],
    [ "I3C_MasterTransferBlocking", "a00041.html#ga2cc7155dd51d87ad0bf3c05cbaed378f", null ],
    [ "I3C_MasterTransferCreateHandle", "a00041.html#gaed9f9136344f725a16c614699f6e0030", null ],
    [ "I3C_MasterTransferNonBlocking", "a00041.html#ga57beac3187ce8a47e46d1dbb5abd496f", null ],
    [ "I3C_MasterTransferGetCount", "a00041.html#ga3db6f43bcce34cd952f01e37b3f19d2a", null ],
    [ "I3C_MasterTransferAbort", "a00041.html#gade7b19f9c3161dc3daac43b5e6156cd9", null ],
    [ "I3C_MasterTransferHandleIRQ", "a00041.html#ga029e91b1f4b9ad2c90644749c3e6b2a9", null ],
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
    [ "I3C_SlaveReceive", "a00041.html#gab35cd6ef6042ccbd3534a4b57a3dcf37", null ]
];