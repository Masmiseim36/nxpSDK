var group__lpi2c__master__driver =
[
    [ "lpi2c_master_config_t", "group__lpi2c__master__driver.html#structlpi2c__master__config__t", [
      [ "enableMaster", "group__lpi2c__master__driver.html#a36f58debd82d2fe56bffc3d8d8748e22", null ],
      [ "enableDoze", "group__lpi2c__master__driver.html#a4f75d18b353af3668e0c3a8dc2c65133", null ],
      [ "debugEnable", "group__lpi2c__master__driver.html#a73c9f8dca26ed429fb5df6eebbb757b1", null ],
      [ "ignoreAck", "group__lpi2c__master__driver.html#af66e69bf2cf504a3f420774a2ee3456b", null ],
      [ "pinConfig", "group__lpi2c__master__driver.html#abf68fadb2fd229f0fd034e1961935ee4", null ],
      [ "baudRate_Hz", "group__lpi2c__master__driver.html#a2f8d353d1e637b082ad8ebe978a4d6b9", null ],
      [ "busIdleTimeout_ns", "group__lpi2c__master__driver.html#a936409ced864d404b3fdf0f66144ccd9", null ],
      [ "pinLowTimeout_ns", "group__lpi2c__master__driver.html#aa7788a850bf2ff954320368b237221ee", null ],
      [ "sdaGlitchFilterWidth_ns", "group__lpi2c__master__driver.html#a6b20d7345badb866950662d13177f137", null ],
      [ "sclGlitchFilterWidth_ns", "group__lpi2c__master__driver.html#abec5caf29a17c3cd3480e5b1b07f742a", null ],
      [ "enable", "group__lpi2c__master__driver.html#adf43f53aece30b66a9dc29a6fe2bb02d", null ],
      [ "source", "group__lpi2c__master__driver.html#a2c56d9865a4d122894a4918b21540151", null ],
      [ "polarity", "group__lpi2c__master__driver.html#acb38f33f1c5f33595cae1133882c4d24", null ],
      [ "hostRequest", "group__lpi2c__master__driver.html#a57c55a1e7b4f9d9b6ebc75ad4205d021", null ]
    ] ],
    [ "lpi2c_data_match_config_t", "group__lpi2c__master__driver.html#structlpi2c__data__match__config__t", [
      [ "matchMode", "group__lpi2c__master__driver.html#a1ee7d6261dcde31818b381f189569d80", null ],
      [ "rxDataMatchOnly", "group__lpi2c__master__driver.html#a2d2eed3e5f6fb4a94b653f416818ae30", null ],
      [ "match0", "group__lpi2c__master__driver.html#ad28aaeb70cf478b4564cc05da64ce062", null ],
      [ "match1", "group__lpi2c__master__driver.html#a8062fa234f06a2888f04dc95dab28240", null ]
    ] ],
    [ "lpi2c_master_transfer_t", "group__lpi2c__master__driver.html#struct__lpi2c__master__transfer", [
      [ "flags", "group__lpi2c__master__driver.html#a1c11b4cb590384ca6a8f9b8b43d23558", null ],
      [ "slaveAddress", "group__lpi2c__master__driver.html#a7b9a1f78b5cf27502969224775e2134b", null ],
      [ "direction", "group__lpi2c__master__driver.html#af9c1114cb5c6834f07c2069e39faba17", null ],
      [ "subaddress", "group__lpi2c__master__driver.html#a377ed24db3b848a1253bc9a5344e732f", null ],
      [ "subaddressSize", "group__lpi2c__master__driver.html#abb7feabae4704bcf7b090d50b6d9c951", null ],
      [ "data", "group__lpi2c__master__driver.html#a6f242bd0a1ce4821c7d1d26074b29a9d", null ],
      [ "dataSize", "group__lpi2c__master__driver.html#a79682c750a97b52d05515165f3f530e1", null ]
    ] ],
    [ "lpi2c_master_handle_t", "group__lpi2c__master__driver.html#struct__lpi2c__master__handle", [
      [ "state", "group__lpi2c__master__driver.html#aa754ef003d1639ef78a69dbe450c9e72", null ],
      [ "remainingBytes", "group__lpi2c__master__driver.html#a5a35aa5dfb9d0b99bf6b63d8ca2d0571", null ],
      [ "buf", "group__lpi2c__master__driver.html#a54a8e0398a4a439336f677034a221e29", null ],
      [ "commandBuffer", "group__lpi2c__master__driver.html#ad4580490ec11fe2ce2f12143a5ad55a0", null ],
      [ "transfer", "group__lpi2c__master__driver.html#a9c451e008467a29718e70cbc3c978a0c", null ],
      [ "completionCallback", "group__lpi2c__master__driver.html#a45504c346312e5b6d8f0ee3e2a9be3c6", null ],
      [ "userData", "group__lpi2c__master__driver.html#a6891adb76d1887f61142ecc89815dcd4", null ]
    ] ],
    [ "lpi2c_master_transfer_callback_t", "group__lpi2c__master__driver.html#ga62ccf3faece8d48363807833b8c58bf2", null ],
    [ "_lpi2c_master_flags", "group__lpi2c__master__driver.html#ga830bba7ea584a7f98d3fb6afd946d739", [
      [ "kLPI2C_MasterTxReadyFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739af3208f156244a4b1c520db35b147ee33", null ],
      [ "kLPI2C_MasterRxReadyFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739ae09650509ffef3de4ede348c4ac66be5", null ],
      [ "kLPI2C_MasterEndOfPacketFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739adf431b1a9475437d7089df5abcea7122", null ],
      [ "kLPI2C_MasterStopDetectFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739aad8d90df6216a4d27680fd530c42710d", null ],
      [ "kLPI2C_MasterNackDetectFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739a3ea7b4560b395c82e5293678228fff81", null ],
      [ "kLPI2C_MasterArbitrationLostFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739a9e012e945342b8f0333e59060affb0b1", null ],
      [ "kLPI2C_MasterFifoErrFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739aac4e2ec9c92ea22b5246bb62000f93a9", null ],
      [ "kLPI2C_MasterPinLowTimeoutFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739a37ceb9a9bdaa1ec7261d7265a51a5f17", null ],
      [ "kLPI2C_MasterDataMatchFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739aaaad4c02a63dcf7d021baa5c4a9dcf2e", null ],
      [ "kLPI2C_MasterBusyFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739ac1de9e29d8ffe76b10b8425f81db1368", null ],
      [ "kLPI2C_MasterBusBusyFlag", "group__lpi2c__master__driver.html#gga830bba7ea584a7f98d3fb6afd946d739a408ed7773539a17625803f6c9d3a6629", null ]
    ] ],
    [ "lpi2c_direction_t", "group__lpi2c__master__driver.html#gaf7540e799ad5eb17c678b39ea1ace80c", [
      [ "kLPI2C_Write", "group__lpi2c__master__driver.html#ggaf7540e799ad5eb17c678b39ea1ace80ca4a913c200cf54c773380ea8271e7b42b", null ],
      [ "kLPI2C_Read", "group__lpi2c__master__driver.html#ggaf7540e799ad5eb17c678b39ea1ace80ca50ba3a53d79baed347bb8fa07d7884ab", null ]
    ] ],
    [ "lpi2c_master_pin_config_t", "group__lpi2c__master__driver.html#ga07db39ec1d066e1427d7e58a52c7ea12", [
      [ "kLPI2C_2PinOpenDrain", "group__lpi2c__master__driver.html#gga07db39ec1d066e1427d7e58a52c7ea12ada174b660785782567aee14c05918d89", null ],
      [ "kLPI2C_2PinOutputOnly", "group__lpi2c__master__driver.html#gga07db39ec1d066e1427d7e58a52c7ea12a9a8053876d619b3beeb745b212b9a1fb", null ],
      [ "kLPI2C_2PinPushPull", "group__lpi2c__master__driver.html#gga07db39ec1d066e1427d7e58a52c7ea12a53f8d0708cfc2ea887abb2e2d440d29e", null ],
      [ "kLPI2C_4PinPushPull", "group__lpi2c__master__driver.html#gga07db39ec1d066e1427d7e58a52c7ea12a9642890d09eeb5ed0b8d9e59aac6e3b7", null ],
      [ "kLPI2C_2PinOpenDrainWithSeparateSlave", "group__lpi2c__master__driver.html#gga07db39ec1d066e1427d7e58a52c7ea12ad809117a74ffea333e273e97dc93aff7", null ],
      [ "kLPI2C_2PinOutputOnlyWithSeparateSlave", "group__lpi2c__master__driver.html#gga07db39ec1d066e1427d7e58a52c7ea12a297f630d584519da05064b2962596ee5", null ],
      [ "kLPI2C_2PinPushPullWithSeparateSlave", "group__lpi2c__master__driver.html#gga07db39ec1d066e1427d7e58a52c7ea12a5e8d93f3f8f55f5c9a47c7cddd4df495", null ],
      [ "kLPI2C_4PinPushPullWithInvertedOutput", "group__lpi2c__master__driver.html#gga07db39ec1d066e1427d7e58a52c7ea12acb4290916ade8b65741bc18d632acbd6", null ]
    ] ],
    [ "lpi2c_host_request_source_t", "group__lpi2c__master__driver.html#gac9a07c4c7370019ccabc914bd26ea1c8", [
      [ "kLPI2C_HostRequestExternalPin", "group__lpi2c__master__driver.html#ggac9a07c4c7370019ccabc914bd26ea1c8a4634268e7c377ffd2e7db9a93b2776eb", null ],
      [ "kLPI2C_HostRequestInputTrigger", "group__lpi2c__master__driver.html#ggac9a07c4c7370019ccabc914bd26ea1c8a1a8773fbec157bc1a069826af08a2aae", null ]
    ] ],
    [ "lpi2c_host_request_polarity_t", "group__lpi2c__master__driver.html#ga78edbfb2d1e9213e7ebfabe32423f5d2", [
      [ "kLPI2C_HostRequestPinActiveLow", "group__lpi2c__master__driver.html#gga78edbfb2d1e9213e7ebfabe32423f5d2aab540d2690ea7446d68f0106e46cfb36", null ],
      [ "kLPI2C_HostRequestPinActiveHigh", "group__lpi2c__master__driver.html#gga78edbfb2d1e9213e7ebfabe32423f5d2a1f72f10b4bfe685cc57e8994df237a16", null ]
    ] ],
    [ "lpi2c_data_match_config_mode_t", "group__lpi2c__master__driver.html#ga28ac6ee1cc7991c2907abb18adbb1b31", [
      [ "kLPI2C_MatchDisabled", "group__lpi2c__master__driver.html#gga28ac6ee1cc7991c2907abb18adbb1b31a00ef085f1047daf41240a7c9daa7aee5", null ],
      [ "kLPI2C_1stWordEqualsM0OrM1", "group__lpi2c__master__driver.html#gga28ac6ee1cc7991c2907abb18adbb1b31ae41fc9d1480c995ecab8be9afc489108", null ],
      [ "kLPI2C_AnyWordEqualsM0OrM1", "group__lpi2c__master__driver.html#gga28ac6ee1cc7991c2907abb18adbb1b31af994783c76b2008af82fa9209d1fb746", null ],
      [ "kLPI2C_1stWordEqualsM0And2ndWordEqualsM1", "group__lpi2c__master__driver.html#gga28ac6ee1cc7991c2907abb18adbb1b31a309463f537d290dc18770d4245e0046e", null ],
      [ "kLPI2C_AnyWordEqualsM0AndNextWordEqualsM1", "group__lpi2c__master__driver.html#gga28ac6ee1cc7991c2907abb18adbb1b31a747cdb86ebade4b5d09fc25a0f199aee", null ],
      [ "kLPI2C_1stWordAndM1EqualsM0AndM1", "group__lpi2c__master__driver.html#gga28ac6ee1cc7991c2907abb18adbb1b31a1b963126207a01049b68b0fd7645ff4a", null ],
      [ "kLPI2C_AnyWordAndM1EqualsM0AndM1", "group__lpi2c__master__driver.html#gga28ac6ee1cc7991c2907abb18adbb1b31a37e28fca69438b64be9ca862bade372b", null ]
    ] ],
    [ "_lpi2c_master_transfer_flags", "group__lpi2c__master__driver.html#ga6d8e7ad8c6f187673a91247a23a52dc1", [
      [ "kLPI2C_TransferDefaultFlag", "group__lpi2c__master__driver.html#gga6d8e7ad8c6f187673a91247a23a52dc1a413ec0da1a0be18365a3ba1285fe78f4", null ],
      [ "kLPI2C_TransferNoStartFlag", "group__lpi2c__master__driver.html#gga6d8e7ad8c6f187673a91247a23a52dc1adc9924923c68af7960d4b8cf43e5609e", null ],
      [ "kLPI2C_TransferRepeatedStartFlag", "group__lpi2c__master__driver.html#gga6d8e7ad8c6f187673a91247a23a52dc1a2d6fdbacf571dc6eea70ef0349c73ccb", null ],
      [ "kLPI2C_TransferNoStopFlag", "group__lpi2c__master__driver.html#gga6d8e7ad8c6f187673a91247a23a52dc1a56c1abdffee0978f6d5aeedba30971c3", null ]
    ] ],
    [ "LPI2C_MasterGetDefaultConfig", "group__lpi2c__master__driver.html#gaf1753f02b6a39bb4391250c646236eae", null ],
    [ "LPI2C_MasterInit", "group__lpi2c__master__driver.html#ga31c7e21bae74602cb831109e7de33ade", null ],
    [ "LPI2C_MasterDeinit", "group__lpi2c__master__driver.html#ga1e5ac26414d36018ac33f31332026b82", null ],
    [ "LPI2C_MasterConfigureDataMatch", "group__lpi2c__master__driver.html#gae258a7281c9afd04f3b0133430449404", null ],
    [ "LPI2C_MasterReset", "group__lpi2c__master__driver.html#ga75ac46a8612bfb21b52699a3feb2cd03", null ],
    [ "LPI2C_MasterEnable", "group__lpi2c__master__driver.html#ga46bd9c4d6579fa0273c3a1cc1c94b65a", null ],
    [ "LPI2C_MasterGetStatusFlags", "group__lpi2c__master__driver.html#ga02cab0c6281b9532bd6f8e2b04e5a2ba", null ],
    [ "LPI2C_MasterClearStatusFlags", "group__lpi2c__master__driver.html#ga27324dea42cdddefdfdf53a034118522", null ],
    [ "LPI2C_MasterEnableInterrupts", "group__lpi2c__master__driver.html#ga41f9e18b131a5a3ed0f0573c468c3123", null ],
    [ "LPI2C_MasterDisableInterrupts", "group__lpi2c__master__driver.html#ga50ab891c01b147071d465138b144fce7", null ],
    [ "LPI2C_MasterGetEnabledInterrupts", "group__lpi2c__master__driver.html#ga8894194d00537fa3a5bae2b88910790c", null ],
    [ "LPI2C_MasterEnableDMA", "group__lpi2c__master__driver.html#ga682c0d77de6725eb842b8bf26d70ef6e", null ],
    [ "LPI2C_MasterGetTxFifoAddress", "group__lpi2c__master__driver.html#ga5350c8b782a982c3a2a7103a32a55569", null ],
    [ "LPI2C_MasterGetRxFifoAddress", "group__lpi2c__master__driver.html#ga84da0ad9e383e1b06ddfce53a52f2777", null ],
    [ "LPI2C_MasterSetWatermarks", "group__lpi2c__master__driver.html#gaf0eb7016afbb3ab6bf8d624dbc95347d", null ],
    [ "LPI2C_MasterGetFifoCounts", "group__lpi2c__master__driver.html#ga485117a5769985079d759dbf083450fd", null ],
    [ "LPI2C_MasterSetBaudRate", "group__lpi2c__master__driver.html#ga7f9f7d0d1c86fb7ee26d84dba706bc9a", null ],
    [ "LPI2C_MasterGetBusIdleState", "group__lpi2c__master__driver.html#ga5b26965efb8b28aeedaf4d8e4491fb50", null ],
    [ "LPI2C_MasterStart", "group__lpi2c__master__driver.html#gad5ec3758566b673bdec4f7030395b863", null ],
    [ "LPI2C_MasterRepeatedStart", "group__lpi2c__master__driver.html#gacd57806adaf72ea3c3506414c3984f84", null ],
    [ "LPI2C_MasterSend", "group__lpi2c__master__driver.html#ga5a6e1ac09452fc5182b0fd3a0da1d9b3", null ],
    [ "LPI2C_MasterReceive", "group__lpi2c__master__driver.html#ga6a80cb5c1ccdbac0c753f86e8dd4d00d", null ],
    [ "LPI2C_MasterStop", "group__lpi2c__master__driver.html#ga63d65ab0751f91adecb03944b117f3df", null ],
    [ "LPI2C_MasterTransferCreateHandle", "group__lpi2c__master__driver.html#gafa1c62e098fb418bca7cfbe1ffa6aeaf", null ],
    [ "LPI2C_MasterTransferNonBlocking", "group__lpi2c__master__driver.html#ga2b0c04b87bd15fcaeac176993969bc29", null ],
    [ "LPI2C_MasterTransferGetCount", "group__lpi2c__master__driver.html#ga5d69127f2ea64b803c67861adea21413", null ],
    [ "LPI2C_MasterTransferAbort", "group__lpi2c__master__driver.html#gae0d4f78a7d8c34d85bea368822cdbbce", null ],
    [ "LPI2C_MasterTransferHandleIRQ", "group__lpi2c__master__driver.html#gadd5f2dac21a966298a1faa03a66b60eb", null ]
];