var group__enet =
[
    [ "enet_rx_bd_struct_t", "group__enet.html#structenet__rx__bd__struct__t", [
      [ "length", "group__enet.html#a8f1b6c5523159bc56b37bfd98b378ab6", null ],
      [ "control", "group__enet.html#a01a2213b5bf50b154c4ca4866c6379ea", null ],
      [ "buffer", "group__enet.html#a5d13fe112ba7cfb37d8467413b0e7c75", null ]
    ] ],
    [ "enet_tx_bd_struct_t", "group__enet.html#structenet__tx__bd__struct__t", [
      [ "length", "group__enet.html#a7f6f448911920d9e7d9ac98f83472e1e", null ],
      [ "control", "group__enet.html#a26f3d526bcef8f3c3cd2a0e40760e449", null ],
      [ "buffer", "group__enet.html#a54a6524c02ec1e5f6f018fd5f6efca66", null ]
    ] ],
    [ "enet_data_error_stats_t", "group__enet.html#structenet__data__error__stats__t", [
      [ "statsRxLenGreaterErr", "group__enet.html#a49a9b5fd12c01f4e3fdf8fb3f4e1cbcf", null ],
      [ "statsRxAlignErr", "group__enet.html#a7aa8325ffacae6ae6ceea48178683da4", null ],
      [ "statsRxFcsErr", "group__enet.html#a961dfc75989fe1d056ee40670b01ee8c", null ],
      [ "statsRxOverRunErr", "group__enet.html#a4a75e297619c93fe9d0450ccb6f29c82", null ],
      [ "statsRxTruncateErr", "group__enet.html#a09504e8c4095afe2540e064d9fec42b0", null ]
    ] ],
    [ "enet_buffer_config_t", "group__enet.html#structenet__buffer__config__t", [
      [ "rxBdNumber", "group__enet.html#a65e1faa923f03d01a64c133a3830bc7f", null ],
      [ "txBdNumber", "group__enet.html#a4f2cfe59fad3a3ea6274ef33edc66676", null ],
      [ "rxBuffSizeAlign", "group__enet.html#a01edfb2c2441e18da8cf716c9104ef88", null ],
      [ "txBuffSizeAlign", "group__enet.html#aa98c505c9b7b19f77fa07e25d746ddb5", null ],
      [ "rxBdStartAddrAlign", "group__enet.html#ae90713ccb39a0bb92275a458f03c5dce", null ],
      [ "txBdStartAddrAlign", "group__enet.html#a9fc178ed9e15d39b55c62d3ad26e05db", null ],
      [ "rxBufferAlign", "group__enet.html#a0917bce7024fa1d557f742e731849768", null ],
      [ "txBufferAlign", "group__enet.html#a70bca73b0157bb5a6985f2c9ecdc4ac8", null ]
    ] ],
    [ "enet_intcoalesce_config_t", "group__enet.html#structenet__intcoalesce__config__t", [
      [ "txCoalesceFrameCount", "group__enet.html#a920e25d5e0e56e1b94ea3185da2b97f6", null ],
      [ "txCoalesceTimeCount", "group__enet.html#ab79bf31622cad0e087bef459f8c08ca9", null ],
      [ "rxCoalesceFrameCount", "group__enet.html#ad887a24639564d3f902f42fb3652499b", null ],
      [ "rxCoalesceTimeCount", "group__enet.html#a7de52c76cb9c659ab17431d0aa9d4a58", null ]
    ] ],
    [ "enet_config_t", "group__enet.html#structenet__config__t", [
      [ "macSpecialConfig", "group__enet.html#ae18a59c71607550da3eb35ac6a64ac11", null ],
      [ "interrupt", "group__enet.html#ad6126ff38c3d09e8f4b305cc0a45befd", null ],
      [ "rxMaxFrameLen", "group__enet.html#a8096267beadb3701fb82163d1e9aa6dd", null ],
      [ "miiMode", "group__enet.html#af5d9f50ec02037d29f6ed4202ffdbc1f", null ],
      [ "miiSpeed", "group__enet.html#a073bdafc878d858c56533323322be6a9", null ],
      [ "miiDuplex", "group__enet.html#a0704352d744c4da14bf1e836e95ff072", null ],
      [ "rxAccelerConfig", "group__enet.html#a7458a889dfeb4ccd0b62767486e61bbb", null ],
      [ "txAccelerConfig", "group__enet.html#a033caacd25725e29eb07aa98936e6814", null ],
      [ "pauseDuration", "group__enet.html#a4ad647d398242d29ca34f03903e119a7", null ],
      [ "rxFifoEmptyThreshold", "group__enet.html#a1a0b1d0e07cc8817dc37f0af8cce10f6", null ],
      [ "rxFifoFullThreshold", "group__enet.html#af15c6f6b67b8bbdebda299b2137e3c48", null ],
      [ "txFifoWatermark", "group__enet.html#a30ec04b6b5c7018f5fa75723778c7bca", null ]
    ] ],
    [ "enet_handle_t", "group__enet.html#struct__enet__handle", [
      [ "rxBdBase", "group__enet.html#ae7d353e9fc36b64623e4cc032481eba7", null ],
      [ "rxBdCurrent", "group__enet.html#ac73a332d5435fda9e50296f025804418", null ],
      [ "txBdBase", "group__enet.html#acda8bcf00ea56a004f44cc915a06d4f4", null ],
      [ "txBdCurrent", "group__enet.html#a2e5ab6cb102b45ae311b1f107faf2fa9", null ],
      [ "rxBuffSizeAlign", "group__enet.html#a71597b4e788e19065d717b54e0a970af", null ],
      [ "txBuffSizeAlign", "group__enet.html#a2a741f9e413490fa3002d9e9c6a18cfc", null ],
      [ "callback", "group__enet.html#ab6f1673b4b3b633bd4e6b0fa8732ea58", null ],
      [ "userData", "group__enet.html#a0fca53e60f7e5646f409633651d8e249", null ]
    ] ],
    [ "FSL_ENET_DRIVER_VERSION", "group__enet.html#ga869f211125798c5e1f17464dee22704e", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_EMPTY_MASK", "group__enet.html#ga4ecdb738a6f27b81eb9ad44d24d2f1db", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_SOFTOWNER1_MASK", "group__enet.html#gac3815af5aedc53cf345ae5186f711d6d", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_WRAP_MASK", "group__enet.html#ga37f7e7094697aa06285df27da4e3791f", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_SOFTOWNER2_Mask", "group__enet.html#ga6097b5e5b8015de197db0e54cbb5425f", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_LAST_MASK", "group__enet.html#gae302c57c88bade74af59c4bcfcac7d0f", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_MISS_MASK", "group__enet.html#gad622789c0bb27a6ac2ab54e3526d2712", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_BROADCAST_MASK", "group__enet.html#gae4cfa2a92c961ee34445976e2949c00d", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_MULTICAST_MASK", "group__enet.html#gaf47a7a6cf15a885878a39749f9d645ed", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_LENVLIOLATE_MASK", "group__enet.html#ga7b3cbc3ed367bd8fcbcbb6bcc14ef06f", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_NOOCTET_MASK", "group__enet.html#ga67881a213531011e78cc56a3ac017549", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_CRC_MASK", "group__enet.html#ga4f57d5345b3a3f349e834e0d46645ce3", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_OVERRUN_MASK", "group__enet.html#ga31d458f38d3123c1eb398bf57d1ebacc", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_TRUNC_MASK", "group__enet.html#ga9e3e6bac667500c8ff096be418e7a0e1", null ],
    [ "ENET_BUFFDESCRIPTOR_TX_READY_MASK", "group__enet.html#ga842251a5e8c62968a87578698f0dd083", null ],
    [ "ENET_BUFFDESCRIPTOR_TX_SOFTOWENER1_MASK", "group__enet.html#gae76d465c3a9f07b28f48433022a005cc", null ],
    [ "ENET_BUFFDESCRIPTOR_TX_WRAP_MASK", "group__enet.html#ga159bc70ce067d81c7b44917f16ab596e", null ],
    [ "ENET_BUFFDESCRIPTOR_TX_SOFTOWENER2_MASK", "group__enet.html#ga09c9ab0c2b634f6e97df4e5b5c4bc8a1", null ],
    [ "ENET_BUFFDESCRIPTOR_TX_LAST_MASK", "group__enet.html#ga59497a4cfa4e8df8ceb6e6b39549ea1d", null ],
    [ "ENET_BUFFDESCRIPTOR_TX_TRANMITCRC_MASK", "group__enet.html#ga4119af7a03ab546888a3853f228b09bd", null ],
    [ "ENET_BUFFDESCRIPTOR_RX_ERR_MASK", "group__enet.html#gaa47652ada5e7cc078bc37979d5487b46", null ],
    [ "ENET_FRAME_MAX_FRAMELEN", "group__enet.html#ga4fa46b5ee409f6d147980adc57493f6d", null ],
    [ "ENET_FIFO_MIN_RX_FULL", "group__enet.html#gae9e50e322b991831fd71514002a3ca7b", null ],
    [ "ENET_RX_MIN_BUFFERSIZE", "group__enet.html#gaedbdd29c8a3e25c856b42f13edebcd8d", null ],
    [ "ENET_PHY_MAXADDRESS", "group__enet.html#ga1f4edc29c5f534c4bd8f1afbd0185a4f", null ],
    [ "enet_callback_t", "group__enet.html#ga9d3c290746379a84b1e9cb744c9e452f", null ],
    [ "_enet_status", "group__enet.html#ga2763713429a9379805f0e70ddd22df0c", [
      [ "kStatus_ENET_RxFrameError", "group__enet.html#gga2763713429a9379805f0e70ddd22df0caef1f945a6dc8fcf6267d57aac2a19f99", null ],
      [ "kStatus_ENET_RxFrameFail", "group__enet.html#gga2763713429a9379805f0e70ddd22df0ca97bf0aacb16c8874c7a0a016483cb2ec", null ],
      [ "kStatus_ENET_RxFrameEmpty", "group__enet.html#gga2763713429a9379805f0e70ddd22df0ca50e097caecebfb4afacf2c8a667a5409", null ],
      [ "kStatus_ENET_TxFrameBusy", "group__enet.html#gga2763713429a9379805f0e70ddd22df0cad15b9b2af39f28f90889bb19f568e844", null ],
      [ "kStatus_ENET_TxFrameFail", "group__enet.html#gga2763713429a9379805f0e70ddd22df0ca34b4c8ff24db933516a0cf13fc8174ad", null ]
    ] ],
    [ "enet_mii_mode_t", "group__enet.html#ga4ae0699c0a7f9706deffbaf201025669", [
      [ "kENET_MiiMode", "group__enet.html#gga4ae0699c0a7f9706deffbaf201025669a8625ad828cb3aae19fa0c2e6934e9368", null ],
      [ "kENET_RmiiMode", "group__enet.html#gga4ae0699c0a7f9706deffbaf201025669ae44236ebbdfcc0ae1e3520edd3d2d602", null ]
    ] ],
    [ "enet_mii_speed_t", "group__enet.html#ga8bbf729066c3400712a19ed1d9179e4c", [
      [ "kENET_MiiSpeed10M", "group__enet.html#gga8bbf729066c3400712a19ed1d9179e4ca6e8f095a5a1944d54425565b5beb835c", null ],
      [ "kENET_MiiSpeed100M", "group__enet.html#gga8bbf729066c3400712a19ed1d9179e4ca267390f53c987b908852257bf5407e8e", null ]
    ] ],
    [ "enet_mii_duplex_t", "group__enet.html#ga0784f279168ea4e3fc2fc07018dcb9bf", [
      [ "kENET_MiiHalfDuplex", "group__enet.html#gga0784f279168ea4e3fc2fc07018dcb9bfad3b58af3cb1b983f8e3cd45b585c91b9", null ],
      [ "kENET_MiiFullDuplex", "group__enet.html#gga0784f279168ea4e3fc2fc07018dcb9bfa0d2ad2c3708231614c716bc921503d6c", null ]
    ] ],
    [ "enet_mii_write_t", "group__enet.html#ga0efd18bb4f4b754039fc30c440e38b02", [
      [ "kENET_MiiWriteNoCompliant", "group__enet.html#gga0efd18bb4f4b754039fc30c440e38b02afa74c106f9ed2436e47a5c3f54382b71", null ],
      [ "kENET_MiiWriteValidFrame", "group__enet.html#gga0efd18bb4f4b754039fc30c440e38b02a1611c88e328da10f5c43bd138eca6794", null ]
    ] ],
    [ "enet_mii_read_t", "group__enet.html#ga9c0264e7da8e7d0a3ead667d3a7b1a89", [
      [ "kENET_MiiReadValidFrame", "group__enet.html#gga9c0264e7da8e7d0a3ead667d3a7b1a89a25d7fb88a8252dc7250617be5ea78f91", null ],
      [ "kENET_MiiReadNoCompliant", "group__enet.html#gga9c0264e7da8e7d0a3ead667d3a7b1a89a3a3402b126399caf1ad17a2981f69a14", null ]
    ] ],
    [ "enet_mii_extend_opcode", "group__enet.html#ga67777b636234c2c556b6d0b1f52a4662", [
      [ "kENET_MiiAddrWrite_C45", "group__enet.html#gga67777b636234c2c556b6d0b1f52a4662aa031e39c75c2ab6c27c06f447338446b", null ],
      [ "kENET_MiiWriteFrame_C45", "group__enet.html#gga67777b636234c2c556b6d0b1f52a4662abc5c4c556438c39c9d2578254431512b", null ],
      [ "kENET_MiiReadFrame_C45", "group__enet.html#gga67777b636234c2c556b6d0b1f52a4662a06eec19796c92e001ad6cc728fc3139d", null ]
    ] ],
    [ "enet_special_control_flag_t", "group__enet.html#ga3dd434e4f34bd4c640cefd471763e834", [
      [ "kENET_ControlFlowControlEnable", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834a2274920ae245b67edc1538ecc662e2e7", null ],
      [ "kENET_ControlRxPayloadCheckEnable", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834a9493b832848510c47488327ef49e4d09", null ],
      [ "kENET_ControlRxPadRemoveEnable", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834aa29ba1cc644bf602ffc8ef9e1ae02892", null ],
      [ "kENET_ControlRxBroadCastRejectEnable", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834ade873dafe5c7263f8cc62b0ca92aada3", null ],
      [ "kENET_ControlMacAddrInsert", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834ac806eb5baebf350b694cca943bc812c7", null ],
      [ "kENET_ControlStoreAndFwdDisable", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834abb1b113283cf4f67722599cc58803643", null ],
      [ "kENET_ControlSMIPreambleDisable", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834a8eb7a5389fe9cf0a867066be878efcf1", null ],
      [ "kENET_ControlPromiscuousEnable", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834a51af87f0cfef162bc4c48fd0884c0cab", null ],
      [ "kENET_ControlMIILoopEnable", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834a8cfbb36dc3a0bebf378ed53d527e93af", null ],
      [ "kENET_ControlVLANTagEnable", "group__enet.html#gga3dd434e4f34bd4c640cefd471763e834a3553f9aa0fdc655ffff8e0f46f6cb4b7", null ]
    ] ],
    [ "enet_interrupt_enable_t", "group__enet.html#gafb9d440ac05cadfab906714b2db65027", [
      [ "kENET_BabrInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a2723f07cc888d945f74650566d79e82e", null ],
      [ "kENET_BabtInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a103227b91492fa90cf41212e63b1e7ae", null ],
      [ "kENET_GraceStopInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a1a3e80425de2ce87abc060cef41d1f9e", null ],
      [ "kENET_TxFrameInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a7b2529920e9b97d937aa6b420bf1d51f", null ],
      [ "kENET_TxBufferInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027ab48679949090d31d9245d28f4b8ac31e", null ],
      [ "kENET_RxFrameInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a35d716ce614f33b7afbcf168922f1e70", null ],
      [ "kENET_RxBufferInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027af4e8f4292fde8f09ade517e91ac8baa7", null ],
      [ "kENET_MiiInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027af14e66a1e3dd261481ae560fb2794340", null ],
      [ "kENET_EBusERInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027ace73af088e05b63d6f8d6aeda6a9908e", null ],
      [ "kENET_LateCollisionInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a1495716c4d2b819469d0243c3768eac1", null ],
      [ "kENET_RetryLimitInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a39343a8e490ffbc2146a517b8c4a82b2", null ],
      [ "kENET_UnderrunInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a1fa3a1323a0da32594d84172db1e95ed", null ],
      [ "kENET_PayloadRxInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027aefe7250848088c6a514d609ff1bd6b2c", null ],
      [ "kENET_WakeupInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a25974ac3cc05e3d928eab800a0f09c27", null ],
      [ "kENET_TsAvailInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a9bd0bb3fb86f1c342bbcc3f2407e3e06", null ],
      [ "kENET_TsTimerInterrupt", "group__enet.html#ggafb9d440ac05cadfab906714b2db65027a7bdae7cedb0c8ca1087feb1511b1bb1c", null ]
    ] ],
    [ "enet_event_t", "group__enet.html#gab6a6012153064f5996a44ecb79559fbe", [
      [ "kENET_RxEvent", "group__enet.html#ggab6a6012153064f5996a44ecb79559fbea0ef30a1e4757622d27d6ca879f300cb6", null ],
      [ "kENET_TxEvent", "group__enet.html#ggab6a6012153064f5996a44ecb79559fbeaae83f9ee29da3fe085f4d1e24983bef7", null ],
      [ "kENET_ErrEvent", "group__enet.html#ggab6a6012153064f5996a44ecb79559fbea1b4966d24ea40b037e7f0e1022441ea7", null ],
      [ "kENET_WakeUpEvent", "group__enet.html#ggab6a6012153064f5996a44ecb79559fbea6e0c81e642fcb382d2f38ad32a32ec07", null ],
      [ "kENET_TimeStampEvent", "group__enet.html#ggab6a6012153064f5996a44ecb79559fbea2d4e884e83838e4bd8985b0c846e2844", null ],
      [ "kENET_TimeStampAvailEvent", "group__enet.html#ggab6a6012153064f5996a44ecb79559fbea5f3aa0c8f959a838732c6a38965a1e2a", null ]
    ] ],
    [ "enet_tx_accelerator_t", "group__enet.html#ga5ab4bff47ce41714000c94e1f5a8a6a8", [
      [ "kENET_TxAccelIsShift16Enabled", "group__enet.html#gga5ab4bff47ce41714000c94e1f5a8a6a8a7db407a277c41ca9d215412e2ef05476", null ],
      [ "kENET_TxAccelIpCheckEnabled", "group__enet.html#gga5ab4bff47ce41714000c94e1f5a8a6a8ad2f2ea0738c6b7fcaee5e02f8f1a8645", null ],
      [ "kENET_TxAccelProtoCheckEnabled", "group__enet.html#gga5ab4bff47ce41714000c94e1f5a8a6a8aa85f2b263781a1c43c304c3f380556ae", null ]
    ] ],
    [ "enet_rx_accelerator_t", "group__enet.html#gaaa83596ca3d01b86f8da9e7e2c90cced", [
      [ "kENET_RxAccelPadRemoveEnabled", "group__enet.html#ggaaa83596ca3d01b86f8da9e7e2c90cceda55ba5425899621f83b28c0c8e61b9060", null ],
      [ "kENET_RxAccelIpCheckEnabled", "group__enet.html#ggaaa83596ca3d01b86f8da9e7e2c90cceda517175d18302c670ad3b0999a9a1c9b1", null ],
      [ "kENET_RxAccelProtoCheckEnabled", "group__enet.html#ggaaa83596ca3d01b86f8da9e7e2c90cceda122f314d9f24c28b18ce350398fcce98", null ],
      [ "kENET_RxAccelMacCheckEnabled", "group__enet.html#ggaaa83596ca3d01b86f8da9e7e2c90cceda3082dc2e884e8b71a82f857c9f24890d", null ],
      [ "kENET_RxAccelisShift16Enabled", "group__enet.html#ggaaa83596ca3d01b86f8da9e7e2c90cceda659e8b06edb4f6944c9969dd56649b89", null ]
    ] ],
    [ "ENET_GetDefaultConfig", "group__enet.html#gaf0992a0c9c3a27b2a40226ac38a26d1e", null ],
    [ "ENET_Init", "group__enet.html#gac3ebae64ff0908b9315aa4c3c27689a3", null ],
    [ "ENET_Deinit", "group__enet.html#ga72f7e3493a2f4134324e9e72492bdbbc", null ],
    [ "ENET_Reset", "group__enet.html#ga809c3f91a850b53afa5511603b1c91df", null ],
    [ "ENET_SetMII", "group__enet.html#ga9008a47cdf43fe2eddde467c9841fc7b", null ],
    [ "ENET_SetSMI", "group__enet.html#ga4e2546cfd538bd06950f2145592858f1", null ],
    [ "ENET_GetSMI", "group__enet.html#ga95deb3a7ea7873830ed7b65cabe93b88", null ],
    [ "ENET_ReadSMIData", "group__enet.html#ga1720662e45648e0f081b9152c36b3156", null ],
    [ "ENET_StartSMIRead", "group__enet.html#ga4b0247aa4825019f9939b2e468cf5758", null ],
    [ "ENET_StartSMIWrite", "group__enet.html#ga0ef39cbb1042eb31189cf59fb3509b9a", null ],
    [ "ENET_StartExtC45SMIRead", "group__enet.html#ga1a4f1800a2d7b5dc67c952d86ecf1a30", null ],
    [ "ENET_StartExtC45SMIWrite", "group__enet.html#gabab9ad8bf84c6f1347dab6c75674858e", null ],
    [ "ENET_SetMacAddr", "group__enet.html#ga924622a35ef81eaf69d4a4fd9302a7be", null ],
    [ "ENET_GetMacAddr", "group__enet.html#ga89e5a62af8d1ebf7359faa1fc13b56bc", null ],
    [ "ENET_AddMulticastGroup", "group__enet.html#ga5b9495b14a42f5b1ee24dcc5a6a9f227", null ],
    [ "ENET_LeaveMulticastGroup", "group__enet.html#ga2aa6b359b811460a6fb9a2588688bf30", null ],
    [ "ENET_ActiveRead", "group__enet.html#ga6e067d2d9b41ad32ce7b32ecad8cefc3", null ],
    [ "ENET_EnableSleepMode", "group__enet.html#ga9a7bb3eb9a6be017e21ce53f2343e8bf", null ],
    [ "ENET_GetAccelFunction", "group__enet.html#ga8d32189b130fbca31cdee7ceb57bf543", null ],
    [ "ENET_EnableInterrupts", "group__enet.html#gad9e530c5904c36e8f23fe22f8c63651b", null ],
    [ "ENET_DisableInterrupts", "group__enet.html#ga6a6f1de5e8f1dd09d217244899c3c8db", null ],
    [ "ENET_GetInterruptStatus", "group__enet.html#ga9b9f5a57ff20ccf83fc8887c1bba2cec", null ],
    [ "ENET_ClearInterruptStatus", "group__enet.html#ga98676acb368ed3d238278b4ede220469", null ],
    [ "ENET_SetCallback", "group__enet.html#gae529b5e69f900ed93741a14265eee8fa", null ],
    [ "ENET_GetRxErrBeforeReadFrame", "group__enet.html#gacc74925085246e648528cabf7c93a277", null ],
    [ "ENET_GetRxFrameSize", "group__enet.html#ga2bafd955a25a2e42279a6fcf4f0b81f9", null ],
    [ "ENET_ReadFrame", "group__enet.html#ga5fc5b46149c07df5be3a292db5c6ac15", null ],
    [ "ENET_SendFrame", "group__enet.html#ga1b09cd070bf5bb414dcac01b6c2c763e", null ],
    [ "ENET_TransmitIRQHandler", "group__enet.html#ga14ebe20169a322ceb9dcafef235b0afd", null ],
    [ "ENET_ReceiveIRQHandler", "group__enet.html#ga9a29168893ca340db8d452edac20040b", null ],
    [ "ENET_ErrorIRQHandler", "group__enet.html#ga684127bd7163faf3458f07a021ceb448", null ],
    [ "ENET_CommonFrame0IRQHandler", "group__enet.html#ga9ec95f044ec27e9e0c5f060b856744d3", null ]
];