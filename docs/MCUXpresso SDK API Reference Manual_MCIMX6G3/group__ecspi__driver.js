var group__ecspi__driver =
[
    [ "ecspi_channel_config_t", "group__ecspi__driver.html#structecspi__channel__config__t", [
      [ "channelMode", "group__ecspi__driver.html#ae4b6b855f5d5202d69995657f6b495f9", null ],
      [ "clockInactiveState", "group__ecspi__driver.html#aa08cd8243e60e65e43e5425c8a560a07", null ],
      [ "dataLineInactiveState", "group__ecspi__driver.html#a9ad22b79de30e56cb705c239c7bb62fd", null ],
      [ "chipSlectActiveState", "group__ecspi__driver.html#a373701fa6b25844c20404dbda4b08612", null ],
      [ "waveForm", "group__ecspi__driver.html#a7d1826b0744e6136569e193298968ba1", null ],
      [ "polarity", "group__ecspi__driver.html#aa6327b0a05d54c85c01ca246e86b50c4", null ],
      [ "phase", "group__ecspi__driver.html#aad4e0d3e01790b01e7c0ca7c6c5920af", null ]
    ] ],
    [ "ecspi_master_config_t", "group__ecspi__driver.html#structecspi__master__config__t", [
      [ "channel", "group__ecspi__driver.html#ab7f80a7f7b11d5c4d3d08a39199dacfd", null ],
      [ "channelConfig", "group__ecspi__driver.html#a5a267f3458edd5cb57aea83c3a17ae1f", null ],
      [ "samplePeriodClock", "group__ecspi__driver.html#ae5056430705436de599aa16a0796f45b", null ],
      [ "burstLength", "group__ecspi__driver.html#aadad8a26aa23fe97db1b1ebf26a6d7a1", null ],
      [ "chipSelectDelay", "group__ecspi__driver.html#a6b4868a81aa300dd78374535af8a5993", null ],
      [ "samplePeriod", "group__ecspi__driver.html#ae1827f3ca16c86b5da47b2009be91c00", null ],
      [ "txFifoThreshold", "group__ecspi__driver.html#a04ef6f7d1d2914b3c28d8a6cf1cf2194", null ],
      [ "rxFifoThreshold", "group__ecspi__driver.html#ae83eee3dd8599490cde034beb9243170", null ],
      [ "baudRate_Bps", "group__ecspi__driver.html#a8b5fce2d680a7e33f0ca1c03108f5a50", null ]
    ] ],
    [ "ecspi_slave_config_t", "group__ecspi__driver.html#structecspi__slave__config__t", [
      [ "burstLength", "group__ecspi__driver.html#a47dc1fd0b2e1db49b4b0adb242cce23f", null ],
      [ "txFifoThreshold", "group__ecspi__driver.html#a878b083da83577aec35e0767d0e6842a", null ],
      [ "rxFifoThreshold", "group__ecspi__driver.html#a5b28b64178ee94d946c401519eeda64c", null ],
      [ "channelConfig", "group__ecspi__driver.html#af49e825bd581a557850df75f07e880ea", null ]
    ] ],
    [ "ecspi_transfer_t", "group__ecspi__driver.html#structecspi__transfer__t", [
      [ "txData", "group__ecspi__driver.html#a024d7352f22d1a5b4e826cbc024581f0", null ],
      [ "rxData", "group__ecspi__driver.html#afc78bda57ee3f9b7d7d732ef02973c2c", null ],
      [ "dataSize", "group__ecspi__driver.html#ab2775e32b2da5edd97ca780106b17ca1", null ],
      [ "channel", "group__ecspi__driver.html#a6f085d63c254287974a6a9175befc55e", null ]
    ] ],
    [ "ecspi_master_handle_t", "group__ecspi__driver.html#struct__ecspi__master__handle", [
      [ "channel", "group__ecspi__driver.html#aa6c13a233cdacc489cf97bc3cd0fb6f5", null ],
      [ "txData", "group__ecspi__driver.html#a32a4f2d44ea110f92b775e1ec7824063", null ],
      [ "rxData", "group__ecspi__driver.html#aadd279e36f9879f8d7ce4881861cbe6f", null ],
      [ "txRemainingBytes", "group__ecspi__driver.html#a3a4689219aa53ca5bdb63ce171d2285c", null ],
      [ "rxRemainingBytes", "group__ecspi__driver.html#ae6b19b31a8d8428a10088831ab03f27b", null ],
      [ "state", "group__ecspi__driver.html#aa07e9a421f101147c70b860b63b24d72", null ],
      [ "transferSize", "group__ecspi__driver.html#a072b80c6510d0d99ce8336ed8c33d6e1", null ],
      [ "callback", "group__ecspi__driver.html#acf5bbc4426afa7bea77b107dbc37652c", null ],
      [ "userData", "group__ecspi__driver.html#a3ed57ad9fe76002e40f129febe160654", null ]
    ] ],
    [ "FSL_ECSPI_DRIVER_VERSION", "group__ecspi__driver.html#ga49e359ce2a195388c177d885b5fd3e74", null ],
    [ "ECSPI_DUMMYDATA", "group__ecspi__driver.html#ga885f0195612ef6aa0d8e48aa644cea57", null ],
    [ "ecspi_slave_handle_t", "group__ecspi__driver.html#gaba2d0ab06e8ec3b144ce09b38717c73a", null ],
    [ "ecspi_master_callback_t", "group__ecspi__driver.html#ga6d23219efb6e309fb8b4cbd1edc86e98", null ],
    [ "ecspi_slave_callback_t", "group__ecspi__driver.html#ga14759db6a77598c3d0b901154129b58d", null ],
    [ "_ecspi_status", "group__ecspi__driver.html#gab7feb7648f53585d65d172a675a42148", [
      [ "kStatus_ECSPI_Busy", "group__ecspi__driver.html#ggab7feb7648f53585d65d172a675a42148a8a46fcccbbfc30bb81a0f324c5f3d860", null ],
      [ "kStatus_ECSPI_Idle", "group__ecspi__driver.html#ggab7feb7648f53585d65d172a675a42148a7a5f40ca776d628f47310d95e68fa245", null ],
      [ "kStatus_ECSPI_Error", "group__ecspi__driver.html#ggab7feb7648f53585d65d172a675a42148acff86cd4db27adbb14cfdbc60cc4e04f", null ],
      [ "kStatus_ECSPI_HardwareOverFlow", "group__ecspi__driver.html#ggab7feb7648f53585d65d172a675a42148ad4e8224a416538ed087176bc033267e7", null ]
    ] ],
    [ "ecspi_clock_polarity_t", "group__ecspi__driver.html#gae0418192a24558da46fad3181ad2aac6", [
      [ "kECSPI_PolarityActiveHigh", "group__ecspi__driver.html#ggae0418192a24558da46fad3181ad2aac6a87b0cb4747fed7d6f25b0107ceea9a86", null ],
      [ "kECSPI_PolarityActiveLow", "group__ecspi__driver.html#ggae0418192a24558da46fad3181ad2aac6a2b62838dac3b0f2fe12f4d423636ff68", null ]
    ] ],
    [ "ecspi_clock_phase_t", "group__ecspi__driver.html#ga776487ec9cfe2ee52d26b62f4adc6878", [
      [ "kECSPI_ClockPhaseFirstEdge", "group__ecspi__driver.html#gga776487ec9cfe2ee52d26b62f4adc6878a3bd1e7add02733a4fdc701b55a28d83c", null ],
      [ "kECSPI_ClockPhaseSecondEdge", "group__ecspi__driver.html#gga776487ec9cfe2ee52d26b62f4adc6878acad0974595c9bbf39c64e1b11951a78b", null ]
    ] ],
    [ "_ecspi_interrupt_enable", "group__ecspi__driver.html#gab6c8407ece4ff07b8039cfdd99897fe3", [
      [ "kECSPI_TxfifoEmptyInterruptEnable", "group__ecspi__driver.html#ggab6c8407ece4ff07b8039cfdd99897fe3af3d3bd3c344fecda9888c66b63f9b2bc", null ],
      [ "kECSPI_TxFifoDataRequstInterruptEnable", "group__ecspi__driver.html#ggab6c8407ece4ff07b8039cfdd99897fe3a2550332987e4b2832993eec7571c102d", null ],
      [ "kECSPI_TxFifoFullInterruptEnable", "group__ecspi__driver.html#ggab6c8407ece4ff07b8039cfdd99897fe3ab51c3b033fa2b8519a53a074be3ba6f7", null ],
      [ "kECSPI_RxFifoReadyInterruptEnable", "group__ecspi__driver.html#ggab6c8407ece4ff07b8039cfdd99897fe3a6113d2098b055e3f08420ce9e3ac2d4c", null ],
      [ "kECSPI_RxFifoDataRequstInterruptEnable", "group__ecspi__driver.html#ggab6c8407ece4ff07b8039cfdd99897fe3a58dfb462d67e67e437b6fdc9449b1b6f", null ],
      [ "kECSPI_RxFifoFullInterruptEnable", "group__ecspi__driver.html#ggab6c8407ece4ff07b8039cfdd99897fe3a98ffad56047a874910ff53ba36bcaf30", null ],
      [ "kECSPI_RxFifoOverFlowInterruptEnable", "group__ecspi__driver.html#ggab6c8407ece4ff07b8039cfdd99897fe3aef5f1ecfd8cb6b4f6ea898169b1c96dd", null ],
      [ "kECSPI_TransferCompleteInterruptEnable", "group__ecspi__driver.html#ggab6c8407ece4ff07b8039cfdd99897fe3a759acc23595685c28efd304219dacd7c", null ],
      [ "kECSPI_AllInterruptEnable", "group__ecspi__driver.html#ggab6c8407ece4ff07b8039cfdd99897fe3a970cc4e4c20aa8a433953c4b3530b99f", null ]
    ] ],
    [ "_ecspi_flags", "group__ecspi__driver.html#gabc0c60a0cdfcca917d9ff1d49d32f11d", [
      [ "kECSPI_TxfifoEmptyFlag", "group__ecspi__driver.html#ggabc0c60a0cdfcca917d9ff1d49d32f11dab655bd71113cf592dae74e2776a721dc", null ],
      [ "kECSPI_TxFifoDataRequstFlag", "group__ecspi__driver.html#ggabc0c60a0cdfcca917d9ff1d49d32f11dacbb31605593949923e0d7f09c3859142", null ],
      [ "kECSPI_TxFifoFullFlag", "group__ecspi__driver.html#ggabc0c60a0cdfcca917d9ff1d49d32f11da72d999ee17dcca997f3cc9ac56647cf7", null ],
      [ "kECSPI_RxFifoReadyFlag", "group__ecspi__driver.html#ggabc0c60a0cdfcca917d9ff1d49d32f11da262aac414af41bb3db795e0aeb70b520", null ],
      [ "kECSPI_RxFifoDataRequstFlag", "group__ecspi__driver.html#ggabc0c60a0cdfcca917d9ff1d49d32f11da707210bff1488bdb9bfaa71ca043b810", null ],
      [ "kECSPI_RxFifoFullFlag", "group__ecspi__driver.html#ggabc0c60a0cdfcca917d9ff1d49d32f11da366f72bc39e182080c5e07d2922e490b", null ],
      [ "kECSPI_RxFifoOverFlowFlag", "group__ecspi__driver.html#ggabc0c60a0cdfcca917d9ff1d49d32f11daa5457be11d61af76cf8192ffe872f1c9", null ],
      [ "kECSPI_TransferCompleteFlag", "group__ecspi__driver.html#ggabc0c60a0cdfcca917d9ff1d49d32f11da3ca312f3c579ca0524ae433a4bd2758f", null ]
    ] ],
    [ "_ecspi_dma_enable_t", "group__ecspi__driver.html#ga3791f3bab2750aac043bd184f220da51", [
      [ "kECSPI_TxDmaEnable", "group__ecspi__driver.html#gga3791f3bab2750aac043bd184f220da51af73555b2b4eb12d7fcab8774e9dfb34b", null ],
      [ "kECSPI_RxDmaEnable", "group__ecspi__driver.html#gga3791f3bab2750aac043bd184f220da51a53af55fec388ff98aafcfa4c947fc215", null ],
      [ "kECSPI_DmaAllEnable", "group__ecspi__driver.html#gga3791f3bab2750aac043bd184f220da51aef9846743f20a85f6429f16d7b98687d", null ]
    ] ],
    [ "ecspi_Data_ready_t", "group__ecspi__driver.html#ga3f9aab667da727ae384a6b76e09fd5c0", [
      [ "kECSPI_DataReadyIgnore", "group__ecspi__driver.html#gga3f9aab667da727ae384a6b76e09fd5c0ae840f8158fce546e682c6ab1710d90b0", null ],
      [ "kECSPI_DataReadyFallingEdge", "group__ecspi__driver.html#gga3f9aab667da727ae384a6b76e09fd5c0a77fc4d56d82923c8cb0fcc9e5521b647", null ],
      [ "kECSPI_DataReadyLowLevel", "group__ecspi__driver.html#gga3f9aab667da727ae384a6b76e09fd5c0af48e5a90d193e110bfd88dc1b4a5c0be", null ]
    ] ],
    [ "ecspi_channel_source_t", "group__ecspi__driver.html#ga0946f249a6cbdb8987baa5f4bd39a0ab", [
      [ "kECSPI_Channel0", "group__ecspi__driver.html#gga0946f249a6cbdb8987baa5f4bd39a0aba1ac961fb6f497e8ad9082fb07504e9c5", null ],
      [ "kECSPI_Channel1", "group__ecspi__driver.html#gga0946f249a6cbdb8987baa5f4bd39a0aba3186dfb67a1311c21c6fc81a845324db", null ],
      [ "kECSPI_Channel2", "group__ecspi__driver.html#gga0946f249a6cbdb8987baa5f4bd39a0aba16c4a01e134f7db5001f496b34bb080a", null ],
      [ "kECSPI_Channel3", "group__ecspi__driver.html#gga0946f249a6cbdb8987baa5f4bd39a0aba0993f73a469ede063091c7de076c006e", null ]
    ] ],
    [ "ecspi_master_slave_mode_t", "group__ecspi__driver.html#gaa8ffe589b98df551c09da7fbd5fd8149", [
      [ "kECSPI_Slave", "group__ecspi__driver.html#ggaa8ffe589b98df551c09da7fbd5fd8149a492c128adc36b27e968376463d339b6c", null ],
      [ "kECSPI_Master", "group__ecspi__driver.html#ggaa8ffe589b98df551c09da7fbd5fd8149afa1c745867b3076523985e99aad4b58a", null ]
    ] ],
    [ "ecspi_data_line_inactive_state_t", "group__ecspi__driver.html#ga7756b59f227697a8d4fe335a2f40fa74", [
      [ "kECSPI_DataLineInactiveStateHigh", "group__ecspi__driver.html#gga7756b59f227697a8d4fe335a2f40fa74ab9b16d2f8e51662551756b6923d8384b", null ],
      [ "kECSPI_DataLineInactiveStateLow", "group__ecspi__driver.html#gga7756b59f227697a8d4fe335a2f40fa74a6d736df09298222db7f060e5bf8dae35", null ]
    ] ],
    [ "ecspi_clock_inactive_state_t", "group__ecspi__driver.html#ga524b46a0bed966cf616715354a0468a2", [
      [ "kECSPI_ClockInactiveStateLow", "group__ecspi__driver.html#gga524b46a0bed966cf616715354a0468a2ac7c53cfc3b3a445bdb4eb3a78d3d236a", null ],
      [ "kECSPI_ClockInactiveStateHigh", "group__ecspi__driver.html#gga524b46a0bed966cf616715354a0468a2a8be6d1327a55926da4cc1504e3763475", null ]
    ] ],
    [ "ecspi_chip_select_active_state_t", "group__ecspi__driver.html#ga75ba01276811af8ffaef76e74173ac7b", [
      [ "kECSPI_ChipSelectActiveStateLow", "group__ecspi__driver.html#gga75ba01276811af8ffaef76e74173ac7ba9940e869c485c1f937b0b2f9b4f7a777", null ],
      [ "kECSPI_ChipSelectActiveStateHigh", "group__ecspi__driver.html#gga75ba01276811af8ffaef76e74173ac7bade90bcbe7e7f3cf34547cf8b67b965f9", null ]
    ] ],
    [ "ecspi_wave_form_t", "group__ecspi__driver.html#gaf6594affdbf34712547ee88af6c14909", [
      [ "kECSPI_WaveFormSingle", "group__ecspi__driver.html#ggaf6594affdbf34712547ee88af6c14909a8ae0eac731958e012204a228852ae40b", null ],
      [ "kECSPI_WaveFormMultiple", "group__ecspi__driver.html#ggaf6594affdbf34712547ee88af6c14909aafd4ab11fa403da5bcf42749565343c7", null ]
    ] ],
    [ "ecspi_sample_period_clock_source_t", "group__ecspi__driver.html#ga8e15db09a9c66cb10350282507f3d5c6", [
      [ "kECSPI_spiClock", "group__ecspi__driver.html#gga8e15db09a9c66cb10350282507f3d5c6a86a48f9e83419196fadcc8189ed6e841", null ],
      [ "kECSPI_lowFreqClock", "group__ecspi__driver.html#gga8e15db09a9c66cb10350282507f3d5c6a02660ea6610c018123dd691c19939f5f", null ]
    ] ],
    [ "ECSPI_MasterGetDefaultConfig", "group__ecspi__driver.html#ga3984ed3d0a8755835f1244334ff59f67", null ],
    [ "ECSPI_MasterInit", "group__ecspi__driver.html#ga26e03d859a266145121e60a94d84e04b", null ],
    [ "ECSPI_SlaveGetDefaultConfig", "group__ecspi__driver.html#ga76aa1e3db8310570d7621766958e4249", null ],
    [ "ECSPI_SlaveInit", "group__ecspi__driver.html#ga9d64d89082d59ff4634df9cfafd92401", null ],
    [ "ECSPI_Deinit", "group__ecspi__driver.html#ga684087107b71beeb41401d78f4983685", null ],
    [ "ECSPI_Enable", "group__ecspi__driver.html#ga2e4d1cabe92e6f4b35c540ba3c8b0308", null ],
    [ "ECSPI_GetStatusFlags", "group__ecspi__driver.html#gacebe36e1f278e18350d4bf971af2fc7e", null ],
    [ "ECSPI_ClearStatusFlags", "group__ecspi__driver.html#gaa437a7c7a740dfabb3f2296b0e989adf", null ],
    [ "ECSPI_EnableInterrupts", "group__ecspi__driver.html#gad3f9f4d11b99c104503aa4b7dda53833", null ],
    [ "ECSPI_DisableInterrupts", "group__ecspi__driver.html#gac9bac11d5dcd977a97a0607d77791ba1", null ],
    [ "ECSPI_SoftwareReset", "group__ecspi__driver.html#gadc13d57141df32cefbb4b0040e6404f6", null ],
    [ "ECSPI_IsMaster", "group__ecspi__driver.html#ga92268573eb4bcbb82d4cc2417ac4933a", null ],
    [ "ECSPI_EnableDMA", "group__ecspi__driver.html#gab2f7c2269309fd273f8e41f11833e313", null ],
    [ "ECSPI_GetTxFifoCount", "group__ecspi__driver.html#ga88e66c4f5f15754fb7ba7fc016f6dc41", null ],
    [ "ECSPI_GetRxFifoCount", "group__ecspi__driver.html#gaded76e7db6b46c3a53922a528d755c57", null ],
    [ "ECSPI_SetChannelSelect", "group__ecspi__driver.html#ga3175e7552062e74bffa3538195b8df6a", null ],
    [ "ECSPI_SetChannelConfig", "group__ecspi__driver.html#gaa4a223ca2612f4e73fae0982afcff226", null ],
    [ "ECSPI_SetBaudRate", "group__ecspi__driver.html#ga4b5b00f89c45cb1164af2a4685e984d5", null ],
    [ "ECSPI_WriteBlocking", "group__ecspi__driver.html#ga510cc92b588628aef34fef1078e87716", null ],
    [ "ECSPI_WriteData", "group__ecspi__driver.html#ga99b6a9e30ad735c3634d60b81c500c2e", null ],
    [ "ECSPI_ReadData", "group__ecspi__driver.html#ga3c50b084acf2fe74f19575cc10069bbe", null ],
    [ "ECSPI_MasterTransferCreateHandle", "group__ecspi__driver.html#ga1ab5918361a605a7f0467bcb3d1760ac", null ],
    [ "ECSPI_MasterTransferBlocking", "group__ecspi__driver.html#ga20f68343a19b3772e814a8ee61d7be7f", null ],
    [ "ECSPI_MasterTransferNonBlocking", "group__ecspi__driver.html#gad77408a45d6982958068a49149ac60b6", null ],
    [ "ECSPI_MasterTransferGetCount", "group__ecspi__driver.html#ga85aef57f678a64cbe3e718ec59043841", null ],
    [ "ECSPI_MasterTransferAbort", "group__ecspi__driver.html#gacda4c03d5c4b1c2ba78578ff5213a2f5", null ],
    [ "ECSPI_MasterTransferHandleIRQ", "group__ecspi__driver.html#gada190053bf71b9ce53a619b7d6c7c245", null ],
    [ "ECSPI_SlaveTransferCreateHandle", "group__ecspi__driver.html#ga63b3bb057b97b1115ad082e9e056632b", null ],
    [ "ECSPI_SlaveTransferNonBlocking", "group__ecspi__driver.html#gad4a51681f1b62b0f26be844a209e6e10", null ],
    [ "ECSPI_SlaveTransferGetCount", "group__ecspi__driver.html#gacea2421bc4aaee6e9cc19f7d4f48e3e5", null ],
    [ "ECSPI_SlaveTransferAbort", "group__ecspi__driver.html#ga2512b5ae1d6591e8ba1d70f15221c689", null ],
    [ "ECSPI_SlaveTransferHandleIRQ", "group__ecspi__driver.html#ga8bc32a537b99b7e988228650957f25d8", null ]
];