var a00031 =
[
    [ "FlexIO eDMA I2S Driver", "a00032.html", "a00032" ],
    [ "FLEXIO_I2S_Type", "a00031.html#a00408", [
      [ "flexioBase", "a00031.html#a17b7bf09a1a3f54afaf63e0e190927d2", null ],
      [ "txPinIndex", "a00031.html#a84af0363647425d17fffb8d5555fbdd7", null ],
      [ "rxPinIndex", "a00031.html#adbb4dced07edd10ea2a343e8309c93a7", null ],
      [ "bclkPinIndex", "a00031.html#a44493afb565e4e8bf95e02c9cf6525be", null ],
      [ "fsPinIndex", "a00031.html#a66345d47d6efdbe717ef86f072e900d2", null ],
      [ "txShifterIndex", "a00031.html#ac239084e3f4e9accc07396ffe8d16834", null ],
      [ "rxShifterIndex", "a00031.html#a3abf8ad23689e536ca16ef63ec461d21", null ],
      [ "bclkTimerIndex", "a00031.html#af86182c86d08dea7625f3e2ecdec44c0", null ],
      [ "fsTimerIndex", "a00031.html#af4ceb6fc780a851051a131dfc7174d25", null ]
    ] ],
    [ "flexio_i2s_config_t", "a00031.html#a00405", [
      [ "enableI2S", "a00031.html#a65f52d2761d26fc3b38ae8dfd2d85f1b", null ],
      [ "masterSlave", "a00031.html#af23ab88febf5c71ed99eebe478152330", null ],
      [ "txPinPolarity", "a00031.html#a7ae45f854ddc7afdd384abba1b7a1055", null ],
      [ "rxPinPolarity", "a00031.html#a30aa98ab4ffdcf552b772745c1e209f1", null ],
      [ "bclkPinPolarity", "a00031.html#a74019ce102aec8ab9b19e4a2d1f6e8c4", null ],
      [ "fsPinPolarity", "a00031.html#aa836cef7519823c4ff11309b18420d4b", null ],
      [ "txTimerPolarity", "a00031.html#a5846c5f856bba11228e15d322aed6d06", null ],
      [ "rxTimerPolarity", "a00031.html#a7b293f477331ed1edd7129d074a38b18", null ]
    ] ],
    [ "flexio_i2s_format_t", "a00031.html#a00406", [
      [ "bitWidth", "a00031.html#adb954eb873cf4622252e9c496531b006", null ],
      [ "sampleRate_Hz", "a00031.html#ab8d21f482920d2db31e60ce6073febc7", null ]
    ] ],
    [ "flexio_i2s_transfer_t", "a00031.html#a00407", [
      [ "data", "a00031.html#a3a1bad13211ca0ffe270d1efaebed0ac", null ],
      [ "dataSize", "a00031.html#a02aca7ffe23c696b0d903d43736eea7c", null ]
    ] ],
    [ "flexio_i2s_handle_t", "a00031.html#a00308", [
      [ "state", "a00031.html#ae6b4c9950aab2628a2afad4ce1843f53", null ],
      [ "callback", "a00031.html#a8595e0e9871200a31487a7366568cee9", null ],
      [ "userData", "a00031.html#ae1dbe0ea87de3e1008d53ad96f358eff", null ],
      [ "bitWidth", "a00031.html#afc3604b1f3bb1e62785de5889aa70ba2", null ],
      [ "queue", "a00031.html#acb17130610a067da4de5a433dc37c961", null ],
      [ "transferSize", "a00031.html#a4969c7a5421d426251aeef031897933c", null ],
      [ "queueUser", "a00031.html#a6cc9e11e1b29e7baa52497f147e95790", null ],
      [ "queueDriver", "a00031.html#acdd20f95d36162918324befaac2a032a", null ]
    ] ],
    [ "FSL_FLEXIO_I2S_DRIVER_VERSION", "a00031.html#ga8ac64ab2c050d6a3b5e6dcc784bde2dd", null ],
    [ "I2S_RETRY_TIMES", "a00031.html#ga7ea2ecb47ec7c253f394013b3d5b680f", null ],
    [ "FLEXIO_I2S_XFER_QUEUE_SIZE", "a00031.html#ga16823ec3547c940091b4eccc87fb5847", null ],
    [ "flexio_i2s_callback_t", "a00031.html#ga5ab500341d34539faaea54f985950046", [
      [ "kStatus_FLEXIO_I2S_Idle", "a00031.html#gga7ff5f2dff38e7639981794c43dc9167ba78e5217077552655a4fbd4990ec5c311", null ],
      [ "kStatus_FLEXIO_I2S_TxBusy", "a00031.html#gga7ff5f2dff38e7639981794c43dc9167ba48d8c97ea4e46f6631429992f1814a7a", null ],
      [ "kStatus_FLEXIO_I2S_RxBusy", "a00031.html#gga7ff5f2dff38e7639981794c43dc9167bafc24c81bf7a6c092dc2ac0131739fb33", null ],
      [ "kStatus_FLEXIO_I2S_Error", "a00031.html#gga7ff5f2dff38e7639981794c43dc9167ba73c78f01390e990b4cc75cca203780a5", null ],
      [ "kStatus_FLEXIO_I2S_QueueFull", "a00031.html#gga7ff5f2dff38e7639981794c43dc9167bae38ee4495779398804472d81dac37c3e", null ],
      [ "kStatus_FLEXIO_I2S_Timeout", "a00031.html#gga7ff5f2dff38e7639981794c43dc9167ba1857c052202d7abc210a4f1defb2332c", null ]
    ] ],
    [ "flexio_i2s_master_slave_t", "a00031.html#ga4251f1ee8699baaef1c7fd11b7f76782", [
      [ "kFLEXIO_I2S_Master", "a00031.html#gga4251f1ee8699baaef1c7fd11b7f76782a07afd29192b090e4721733409c076924", null ],
      [ "kFLEXIO_I2S_Slave", "a00031.html#gga4251f1ee8699baaef1c7fd11b7f76782a97fd81b843abe473a98104592b07573c", null ],
      [ "kFLEXIO_I2S_TxDataRegEmptyInterruptEnable", "a00031.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8a1ef2aba36fa79d0d6c3137cbc396703f", null ],
      [ "kFLEXIO_I2S_RxDataRegFullInterruptEnable", "a00031.html#ggaabfcbcb5ac86a1edac4035264bc7d2b8ac5723570f50576ab3f106a8391e0836e", null ],
      [ "kFLEXIO_I2S_TxDataRegEmptyFlag", "a00031.html#gga5d76b81b0ad4c19007a781d4edb8181fa19e09f8fa7b55c47396e2b1766675f35", null ],
      [ "kFLEXIO_I2S_RxDataRegFullFlag", "a00031.html#gga5d76b81b0ad4c19007a781d4edb8181fa33dfc99c1e90d0e6b0e09964625405e2", null ]
    ] ],
    [ "flexio_i2s_sample_rate_t", "a00031.html#ga068da57c1c28f1630de3c6f98217af43", [
      [ "kFLEXIO_I2S_SampleRate8KHz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43abdbb06784965d0befa972cf261bfe4e7", null ],
      [ "kFLEXIO_I2S_SampleRate11025Hz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43a971f9b7f81784ff181beeac3f185db27", null ],
      [ "kFLEXIO_I2S_SampleRate12KHz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43a10e640c145f0971cdb4af9831ed23d37", null ],
      [ "kFLEXIO_I2S_SampleRate16KHz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43a546954b43a4e8dd9d0ca58f327a2d814", null ],
      [ "kFLEXIO_I2S_SampleRate22050Hz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43a02a2091579d4685d9e7b8578a3f0bf63", null ],
      [ "kFLEXIO_I2S_SampleRate24KHz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43a1991d896745d99bcc1e03a0753ced4be", null ],
      [ "kFLEXIO_I2S_SampleRate32KHz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43a01854dae59bfbd8c27f0f5797fb2da28", null ],
      [ "kFLEXIO_I2S_SampleRate44100Hz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43a9c5fe1c399dedb54a3d142cda2fa9322", null ],
      [ "kFLEXIO_I2S_SampleRate48KHz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43a973959baadd2e2f5b42caeeb09831e9b", null ],
      [ "kFLEXIO_I2S_SampleRate96KHz", "a00031.html#gga068da57c1c28f1630de3c6f98217af43a5d26aebf6a38300f5b365cbe84bac915", null ]
    ] ],
    [ "flexio_i2s_word_width_t", "a00031.html#gaa2f5f84d50951eb01a3eaa9ca30a61d3", [
      [ "kFLEXIO_I2S_WordWidth8bits", "a00031.html#ggaa2f5f84d50951eb01a3eaa9ca30a61d3a7c48374533cf9da13e97c4140142df54", null ],
      [ "kFLEXIO_I2S_WordWidth16bits", "a00031.html#ggaa2f5f84d50951eb01a3eaa9ca30a61d3a7b9f51780b4da157105774eb145b900e", null ],
      [ "kFLEXIO_I2S_WordWidth24bits", "a00031.html#ggaa2f5f84d50951eb01a3eaa9ca30a61d3ad5f6e6d1944dc2eedb868d397eb47418", null ],
      [ "kFLEXIO_I2S_WordWidth32bits", "a00031.html#ggaa2f5f84d50951eb01a3eaa9ca30a61d3aa605b9bea21a94a75d862dcc416d694b", null ]
    ] ],
    [ "FLEXIO_I2S_Init", "a00031.html#ga2e15edfb41b26c8b6c1172e59b4e5ca2", null ],
    [ "FLEXIO_I2S_GetDefaultConfig", "a00031.html#ga63430c0096dd4f0efa0576b9f8d8e715", null ],
    [ "FLEXIO_I2S_Deinit", "a00031.html#ga39a22e12f1f18ac14a018a3a26f2d6d5", null ],
    [ "FLEXIO_I2S_Enable", "a00031.html#gac5e2d12ff598e8063622799bed7a7c07", null ],
    [ "FLEXIO_I2S_GetStatusFlags", "a00031.html#ga6392f280ddddea41518ab6f17bc10cff", null ],
    [ "FLEXIO_I2S_EnableInterrupts", "a00031.html#ga0cab52ff3cd5ee103cbc785d0982ee7a", null ],
    [ "FLEXIO_I2S_DisableInterrupts", "a00031.html#gac82fba515a043339d8c131ebda348510", null ],
    [ "FLEXIO_I2S_TxEnableDMA", "a00031.html#ga7163a1f52584f134fc328574ccc208d0", null ],
    [ "FLEXIO_I2S_RxEnableDMA", "a00031.html#ga3542137bbd509b5657097f3e0aa2e4bf", null ],
    [ "FLEXIO_I2S_TxGetDataRegisterAddress", "a00031.html#gaa407468114d14b18d58745464df5151f", null ],
    [ "FLEXIO_I2S_RxGetDataRegisterAddress", "a00031.html#gabc9503a27418cb0e1110803853171fb1", null ],
    [ "FLEXIO_I2S_MasterSetFormat", "a00031.html#ga12f775c48ef0376616d271de5e224e03", null ],
    [ "FLEXIO_I2S_SlaveSetFormat", "a00031.html#ga31e254b013b68f5c84a45794ee67e9b6", null ],
    [ "FLEXIO_I2S_WriteBlocking", "a00031.html#gadf86d43e7db486d68ddf3c41d12ef37c", null ],
    [ "FLEXIO_I2S_WriteData", "a00031.html#ga19b30ed9f8b1fc37675bc92cac35b871", null ],
    [ "FLEXIO_I2S_ReadBlocking", "a00031.html#gac03239be31a9c4768e2d409de9e455f9", null ],
    [ "FLEXIO_I2S_ReadData", "a00031.html#ga87630cef372b980eb1c39cb339c432d0", null ],
    [ "FLEXIO_I2S_TransferTxCreateHandle", "a00031.html#ga31c71deb66991f9b63e80c88652461c5", null ],
    [ "FLEXIO_I2S_TransferSetFormat", "a00031.html#ga725a2486896d3e60f2723d651bd82b6f", null ],
    [ "FLEXIO_I2S_TransferRxCreateHandle", "a00031.html#ga38f2a431871f5dfd9d07905b0f5ed81a", null ],
    [ "FLEXIO_I2S_TransferSendNonBlocking", "a00031.html#ga225948d3be017124dc7db4518e6a41d4", null ],
    [ "FLEXIO_I2S_TransferReceiveNonBlocking", "a00031.html#ga474f218ab7765525d1fd30ed10c04437", null ],
    [ "FLEXIO_I2S_TransferAbortSend", "a00031.html#ga3fb83f37b693d00a84f9b3108690cdff", null ],
    [ "FLEXIO_I2S_TransferAbortReceive", "a00031.html#ga95208c72642fe24909c7708c1efbd9a4", null ],
    [ "FLEXIO_I2S_TransferGetSendCount", "a00031.html#ga590a1845d2f37e2d8ca67d95a85b842a", null ],
    [ "FLEXIO_I2S_TransferGetReceiveCount", "a00031.html#gab1c4b7d6e2c63bdbd33a86ad4acf8c25", null ],
    [ "FLEXIO_I2S_TransferTxHandleIRQ", "a00031.html#gab7706d68288d8c1fa2ad05d43ed459fe", null ],
    [ "FLEXIO_I2S_TransferRxHandleIRQ", "a00031.html#gacb80c2a8216a31744378ebb39ad65c2b", null ]
];