var group__lpsci__driver =
[
    [ "lpsci_config_t", "group__lpsci__driver.html#structlpsci__config__t", [
      [ "baudRate_Bps", "group__lpsci__driver.html#a9784bc13e2e22e05b90ed54381d044ea", null ],
      [ "parityMode", "group__lpsci__driver.html#afa5e6a26841a9125eecc9a3f1b609455", null ],
      [ "stopBitCount", "group__lpsci__driver.html#a49b303b2277239088323bcaa3285aa3d", null ],
      [ "enableTx", "group__lpsci__driver.html#a0911a9698d1bb9828b7f43d7fcdaa14a", null ],
      [ "enableRx", "group__lpsci__driver.html#a7704cb6216a86690577eec27d7ea585a", null ]
    ] ],
    [ "lpsci_transfer_t", "group__lpsci__driver.html#structlpsci__transfer__t", [
      [ "data", "group__lpsci__driver.html#a9103461ccfcb084e6225bd2854cab146", null ],
      [ "dataSize", "group__lpsci__driver.html#a2934133599e8c636d1ed2faa0825d3ad", null ]
    ] ],
    [ "FSL_LPSCI_DRIVER_VERSION", "group__lpsci__driver.html#ga8206caef92ca2331e5494925d8cb5aed", null ],
    [ "lpsci_transfer_callback_t", "group__lpsci__driver.html#gae398db03e2da3c8973859b9fe0047f10", null ],
    [ "_lpsci_status", "group__lpsci__driver.html#gac1d1feb1e13e35beba93e7afce633e16", [
      [ "kStatus_LPSCI_TxBusy", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a7073247ab412c287a4ba590e9a3f1763", null ],
      [ "kStatus_LPSCI_RxBusy", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a5e225b6f93a42436b01525305eb345b8", null ],
      [ "kStatus_LPSCI_TxIdle", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16acb9eaa06221caa3b15102135bcc58cb0", null ],
      [ "kStatus_LPSCI_RxIdle", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a4d3b44544996f003e9246e4b725e1db3", null ],
      [ "kStatus_LPSCI_FlagCannotClearManually", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16acb37b9124115df5b59a01b4311ad1243", null ],
      [ "kStatus_LPSCI_BaudrateNotSupport", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a1ac38136f23a4f1ca18a86972ee51246", null ],
      [ "kStatus_LPSCI_Error", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a741410f4c0886c858378ce1ac51da0a5", null ],
      [ "kStatus_LPSCI_RxRingBufferOverrun", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a2288eb249cfe864ee29a1e53e5254f47", null ],
      [ "kStatus_LPSCI_RxHardwareOverrun", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a3d001d307f032b50fa810b1374abf4e0", null ],
      [ "kStatus_LPSCI_NoiseError", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a24c82dc147e0ddc96f7b882a8c4d8d54", null ],
      [ "kStatus_LPSCI_FramingError", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a526b9a4050c3dcd92b53c59565dc033f", null ],
      [ "kStatus_LPSCI_ParityError", "group__lpsci__driver.html#ggac1d1feb1e13e35beba93e7afce633e16a432958a10e6bf938ba2301f890e84a55", null ]
    ] ],
    [ "lpsci_parity_mode_t", "group__lpsci__driver.html#ga914e25b9b58c086d6ee790f7bbe96de1", [
      [ "kLPSCI_ParityDisabled", "group__lpsci__driver.html#gga914e25b9b58c086d6ee790f7bbe96de1a702ea1a55bf3eb879fd5d817cd95a696", null ],
      [ "kLPSCI_ParityEven", "group__lpsci__driver.html#gga914e25b9b58c086d6ee790f7bbe96de1aa95669109142cc4f43e257e702ddd6fe", null ],
      [ "kLPSCI_ParityOdd", "group__lpsci__driver.html#gga914e25b9b58c086d6ee790f7bbe96de1afa5d5edd522b4ade258f3792901927c1", null ]
    ] ],
    [ "lpsci_stop_bit_count_t", "group__lpsci__driver.html#ga1e71c77aa3c6663fa13a36614ecd5218", [
      [ "kLPSCI_OneStopBit", "group__lpsci__driver.html#gga1e71c77aa3c6663fa13a36614ecd5218a807eb926b7cd6cc5d25a34689e16075f", null ],
      [ "kLPSCI_TwoStopBit", "group__lpsci__driver.html#gga1e71c77aa3c6663fa13a36614ecd5218a12446eb122de9a901036aabab33d4bdc", null ]
    ] ],
    [ "_lpsci_interrupt_enable_t", "group__lpsci__driver.html#gaa58d87d0b59b83414988fbb1b7c79cdb", [
      [ "kLPSCI_LinBreakInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdba09ef11585639ac53f945a7a524674c47", null ],
      [ "kLPSCI_RxActiveEdgeInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdbaac1b4da30c60fd98e4f5dcdada110a38", null ],
      [ "kLPSCI_TxDataRegEmptyInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdbaea5f6ef08a50d1576b99c3aad6618fe4", null ],
      [ "kLPSCI_TransmissionCompleteInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdbaacef19879d6eee40c11bd0ced8756602", null ],
      [ "kLPSCI_RxDataRegFullInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdbaf51e3ebc1b14f13e9b4cfe4859c0a9b3", null ],
      [ "kLPSCI_IdleLineInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdbac75c2bc89795da716eda76084fe79b17", null ],
      [ "kLPSCI_RxOverrunInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdba1b0296dd785a43396080e4167452ff54", null ],
      [ "kLPSCI_NoiseErrorInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdba2fcadea24f1f01f6e2953ad0a96e5621", null ],
      [ "kLPSCI_FramingErrorInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdba5354940b55c917fd282585166e8b97f6", null ],
      [ "kLPSCI_ParityErrorInterruptEnable", "group__lpsci__driver.html#ggaa58d87d0b59b83414988fbb1b7c79cdba38b0f225428a019dff8bada27b15dc0d", null ]
    ] ],
    [ "_lpsci_status_flag_t", "group__lpsci__driver.html#ga581702dbe0430a177c664a5d495224be", [
      [ "kLPSCI_TxDataRegEmptyFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224bea36464c4b39e7c08242b717978cca4217", null ],
      [ "kLPSCI_TransmissionCompleteFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224bea6500e8ab15028ce548dbe9a43484fb4c", null ],
      [ "kLPSCI_RxDataRegFullFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224bea35ec5c8d6104b078fa6b01c0bee0a15e", null ],
      [ "kLPSCI_IdleLineFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224bea6d471331d3bea279810a621312ae3840", null ],
      [ "kLPSCI_RxOverrunFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224bead2330d54a37be33a8732aa4d6fe3e288", null ],
      [ "kLPSCI_NoiseErrorFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224bea621a62a970f875d220befad233ad6a58", null ],
      [ "kLPSCI_FramingErrorFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224bea6923b3152838210054bfc4791d8ac43c", null ],
      [ "kLPSCI_ParityErrorFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224beacb0ea6d4b7af5a744a5901f0b1b7520f", null ],
      [ "kLPSCI_LinBreakFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224bea7142ca52e95d1881d6c0d1c764247226", null ],
      [ "kLPSCI_RxActiveEdgeFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224bea921c671d6a1a82e189977db8f44973d1", null ],
      [ "kLPSCI_RxActiveFlag", "group__lpsci__driver.html#gga581702dbe0430a177c664a5d495224beaa7a30af77f7c1c6725fa055aa4c466b2", null ]
    ] ],
    [ "LPSCI_Init", "group__lpsci__driver.html#gaba2b35edef52515ce868e0e2f7fade81", null ],
    [ "LPSCI_Deinit", "group__lpsci__driver.html#ga9c1cc2d653394f88a5f9a4c3a6c5f498", null ],
    [ "LPSCI_GetDefaultConfig", "group__lpsci__driver.html#ga4246a6f2a19ea028917059e372592b7d", null ],
    [ "LPSCI_SetBaudRate", "group__lpsci__driver.html#ga58f4b156863ea675c4143882208a8736", null ],
    [ "LPSCI_GetStatusFlags", "group__lpsci__driver.html#gadece7c20e6564c0a726f39862b5e8fdd", null ],
    [ "LPSCI_EnableInterrupts", "group__lpsci__driver.html#gae7cd8c6678f9fe56adb5df2d8e3bc121", null ],
    [ "LPSCI_DisableInterrupts", "group__lpsci__driver.html#ga1864d45e565615e20382215869b51ea7", null ],
    [ "LPSCI_GetEnabledInterrupts", "group__lpsci__driver.html#gaf1fb22a4d3ce4827ff8c9feb0b1e7988", null ],
    [ "LPSCI_GetDataRegisterAddress", "group__lpsci__driver.html#gaa39e1eb0307e57d24a3305f86ba5a7d1", null ],
    [ "LPSCI_EnableTxDMA", "group__lpsci__driver.html#ga5ccc358115e35a4ebf86e6550352e0c7", null ],
    [ "LPSCI_EnableRxDMA", "group__lpsci__driver.html#ga0f55bc5c5ac9e88598da3b57b8f80d29", null ],
    [ "LPSCI_EnableTx", "group__lpsci__driver.html#ga63bf1a445a943202ea7c86a90141f56c", null ],
    [ "LPSCI_EnableRx", "group__lpsci__driver.html#ga9ba0aa7051a89771bececd06dbf74bb1", null ],
    [ "LPSCI_WriteByte", "group__lpsci__driver.html#gaa13d00aa1d674d1132909792912ec92c", null ],
    [ "LPSCI_ReadByte", "group__lpsci__driver.html#ga90ac0899c71c47fdff8db3a5752a7130", null ],
    [ "LPSCI_WriteBlocking", "group__lpsci__driver.html#ga73f784ea0b3d7c225f3bbbb2d728ee02", null ],
    [ "LPSCI_ReadBlocking", "group__lpsci__driver.html#gadeab9cb40b639b5e9f4943f08152b76c", null ],
    [ "LPSCI_TransferCreateHandle", "group__lpsci__driver.html#ga2e327eede7ae083807a022a3e83a0443", null ],
    [ "LPSCI_TransferStartRingBuffer", "group__lpsci__driver.html#gad13abbc5047bf0489bc17bce8480e1af", null ],
    [ "LPSCI_TransferStopRingBuffer", "group__lpsci__driver.html#ga44c795eb194dad914d8394d99c1d4552", null ],
    [ "LPSCI_TransferSendNonBlocking", "group__lpsci__driver.html#ga998f33848590a21b5351d2536c64c116", null ],
    [ "LPSCI_TransferAbortSend", "group__lpsci__driver.html#gafde36882901dc0a2fed8ffe42a648ecd", null ],
    [ "LPSCI_TransferGetSendCount", "group__lpsci__driver.html#gad2eb23791ceb822e4d93d11af4ce9ee8", null ],
    [ "LPSCI_TransferReceiveNonBlocking", "group__lpsci__driver.html#gae1fc41709768728caf4b29fd2ab889e7", null ],
    [ "LPSCI_TransferAbortReceive", "group__lpsci__driver.html#gabc500f220e7d1dcf00baddf6337d31ba", null ],
    [ "LPSCI_TransferGetReceiveCount", "group__lpsci__driver.html#ga56e94947fb4c3766a582808193a52903", null ],
    [ "LPSCI_TransferHandleIRQ", "group__lpsci__driver.html#gaf279ff1dadb04bb817242c8be3c31acf", null ],
    [ "LPSCI_TransferHandleErrorIRQ", "group__lpsci__driver.html#ga4d6e29854dad94ef7c88ff5747a515af", null ]
];