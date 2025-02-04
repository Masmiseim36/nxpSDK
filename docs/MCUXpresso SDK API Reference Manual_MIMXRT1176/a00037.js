var a00037 =
[
    [ "FlexIO eDMA I2S Driver", "a00038.html", "a00038" ],
    [ "_flexio_i2s_type", "a00037.html#a00509", [
      [ "flexioBase", "a00037.html#ab449013f40d8a8a2d597e5d1e88a8b97", null ],
      [ "txPinIndex", "a00037.html#aa53e30ebdd5c5f646dca2543fcd9b76e", null ],
      [ "rxPinIndex", "a00037.html#a712be31b7f9d3d6b759eb12c3b667ddd", null ],
      [ "bclkPinIndex", "a00037.html#aa67581730594eb6b372b369bea8f032c", null ],
      [ "fsPinIndex", "a00037.html#a3e2c745c2005607b4371c41cc4a5f769", null ],
      [ "txShifterIndex", "a00037.html#a9fab8c45d53cdffcfa6cf7771a410a78", null ],
      [ "rxShifterIndex", "a00037.html#a4ec10356b8eb4c1da32505a6e16e3616", null ],
      [ "bclkTimerIndex", "a00037.html#afcd9b14fce1388d2b6e18eb1108a5858", null ],
      [ "fsTimerIndex", "a00037.html#abd009d69ef6c06fdcdde60e2b3fb1875", null ]
    ] ],
    [ "_flexio_i2s_config", "a00037.html#a00504", [
      [ "enableI2S", "a00037.html#adb49e8c4018f36d415c3f27b7b4a45ce", null ],
      [ "masterSlave", "a00037.html#adb6202db43c51c67124a7a2507356d20", null ],
      [ "txPinPolarity", "a00037.html#a5afd952a94b5ec444bcc17ae1fa5b3b0", null ],
      [ "rxPinPolarity", "a00037.html#a46396136324c5e05f0b11c0a2a4584dd", null ],
      [ "bclkPinPolarity", "a00037.html#ab77309e749827ec8f41ba9f2d2d0c4cd", null ],
      [ "fsPinPolarity", "a00037.html#af34a05b8906de1dbfbbd8887b4cd64f1", null ],
      [ "txTimerPolarity", "a00037.html#a39495ea95a20060dec9f4c1779749370", null ],
      [ "rxTimerPolarity", "a00037.html#a42a66f46fb56b064f1ba7ffb36a8db90", null ]
    ] ],
    [ "_flexio_i2s_format", "a00037.html#a00506", [
      [ "bitWidth", "a00037.html#a9f3f738467fa2ad3d971b67f74bf7183", null ],
      [ "sampleRate_Hz", "a00037.html#a0bfe7e50f67660594c07d8af2ddb9fda", null ]
    ] ],
    [ "_flexio_i2s_transfer", "a00037.html#a00508", [
      [ "data", "a00037.html#a138c2a667eb11137ede4bce12bcae674", null ],
      [ "dataSize", "a00037.html#a273a94a7eff323bc25c653f485be30b4", null ]
    ] ],
    [ "_flexio_i2s_handle", "a00037.html#a00507", [
      [ "state", "a00037.html#ab174c57d2ddb32975053a804610196df", null ],
      [ "callback", "a00037.html#a2db2fd7655807590d6c7222aae46a136", null ],
      [ "userData", "a00037.html#a57746c8830cc4bd7b878be18a71cf669", null ],
      [ "bitWidth", "a00037.html#aa55d04f1de3bf81fd4a3320e1c10005b", null ],
      [ "queue", "a00037.html#a71efb264e6c1362fd91af882999ce0a9", null ],
      [ "transferSize", "a00037.html#a79cf1f1369ee1d49081817a005830cdd", null ],
      [ "queueUser", "a00037.html#a553d7b7ee77695b269b9d2b2c10ff35d", null ],
      [ "queueDriver", "a00037.html#a74b6814e572ae5cf43928c8a9768886b", null ]
    ] ],
    [ "FSL_FLEXIO_I2S_DRIVER_VERSION", "a00037.html#ga8ac64ab2c050d6a3b5e6dcc784bde2dd", null ],
    [ "I2S_RETRY_TIMES", "a00037.html#ga7ea2ecb47ec7c253f394013b3d5b680f", null ],
    [ "FLEXIO_I2S_XFER_QUEUE_SIZE", "a00037.html#ga16823ec3547c940091b4eccc87fb5847", null ],
    [ "FLEXIO_I2S_Type", "a00037.html#ga2a438e02744f1a3b4ad6f18314536966", null ],
    [ "flexio_i2s_master_slave_t", "a00037.html#gad647608b94bd0214471c0bb6179fb1b5", null ],
    [ "flexio_i2s_config_t", "a00037.html#gaa83a7a055a625b3807fd406dee85a2c6", null ],
    [ "flexio_i2s_format_t", "a00037.html#ga59a1d4e10fda508bc6872a65c72a4234", null ],
    [ "flexio_i2s_sample_rate_t", "a00037.html#ga5d8ced458856b0750946948456bcc593", null ],
    [ "flexio_i2s_word_width_t", "a00037.html#ga59710e800d8eea16e58454588e5f8889", null ],
    [ "flexio_i2s_transfer_t", "a00037.html#gac75d826cfb18663febe134757abe38b4", null ],
    [ "flexio_i2s_callback_t", "a00037.html#ga5ab500341d34539faaea54f985950046", [
      [ "kStatus_FLEXIO_I2S_Idle", "a00037.html#ggaa156d1cebb38c8a65846c4d9c006012aa78e5217077552655a4fbd4990ec5c311", null ],
      [ "kStatus_FLEXIO_I2S_TxBusy", "a00037.html#ggaa156d1cebb38c8a65846c4d9c006012aa48d8c97ea4e46f6631429992f1814a7a", null ],
      [ "kStatus_FLEXIO_I2S_RxBusy", "a00037.html#ggaa156d1cebb38c8a65846c4d9c006012aafc24c81bf7a6c092dc2ac0131739fb33", null ],
      [ "kStatus_FLEXIO_I2S_Error", "a00037.html#ggaa156d1cebb38c8a65846c4d9c006012aa73c78f01390e990b4cc75cca203780a5", null ],
      [ "kStatus_FLEXIO_I2S_QueueFull", "a00037.html#ggaa156d1cebb38c8a65846c4d9c006012aae38ee4495779398804472d81dac37c3e", null ],
      [ "kStatus_FLEXIO_I2S_Timeout", "a00037.html#ggaa156d1cebb38c8a65846c4d9c006012aa1857c052202d7abc210a4f1defb2332c", null ]
    ] ],
    [ "_flexio_i2s_master_slave", "a00037.html#ga2e37fec79a7313afcc1a4c13d76137ce", [
      [ "kFLEXIO_I2S_Master", "a00037.html#gga2e37fec79a7313afcc1a4c13d76137cea07afd29192b090e4721733409c076924", null ],
      [ "kFLEXIO_I2S_Slave", "a00037.html#gga2e37fec79a7313afcc1a4c13d76137cea97fd81b843abe473a98104592b07573c", null ],
      [ "kFLEXIO_I2S_TxDataRegEmptyInterruptEnable", "a00037.html#ggaf8654c714018c6241d9ebd01f3fb5a26a1ef2aba36fa79d0d6c3137cbc396703f", null ],
      [ "kFLEXIO_I2S_RxDataRegFullInterruptEnable", "a00037.html#ggaf8654c714018c6241d9ebd01f3fb5a26ac5723570f50576ab3f106a8391e0836e", null ],
      [ "kFLEXIO_I2S_TxDataRegEmptyFlag", "a00037.html#gga2f80701c36e79c0640d91c788feee0b3a19e09f8fa7b55c47396e2b1766675f35", null ],
      [ "kFLEXIO_I2S_RxDataRegFullFlag", "a00037.html#gga2f80701c36e79c0640d91c788feee0b3a33dfc99c1e90d0e6b0e09964625405e2", null ]
    ] ],
    [ "_flexio_i2s_sample_rate", "a00037.html#ga59290805116faa55aee36e2eb01e17d2", [
      [ "kFLEXIO_I2S_SampleRate8KHz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2abdbb06784965d0befa972cf261bfe4e7", null ],
      [ "kFLEXIO_I2S_SampleRate11025Hz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2a971f9b7f81784ff181beeac3f185db27", null ],
      [ "kFLEXIO_I2S_SampleRate12KHz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2a10e640c145f0971cdb4af9831ed23d37", null ],
      [ "kFLEXIO_I2S_SampleRate16KHz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2a546954b43a4e8dd9d0ca58f327a2d814", null ],
      [ "kFLEXIO_I2S_SampleRate22050Hz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2a02a2091579d4685d9e7b8578a3f0bf63", null ],
      [ "kFLEXIO_I2S_SampleRate24KHz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2a1991d896745d99bcc1e03a0753ced4be", null ],
      [ "kFLEXIO_I2S_SampleRate32KHz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2a01854dae59bfbd8c27f0f5797fb2da28", null ],
      [ "kFLEXIO_I2S_SampleRate44100Hz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2a9c5fe1c399dedb54a3d142cda2fa9322", null ],
      [ "kFLEXIO_I2S_SampleRate48KHz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2a973959baadd2e2f5b42caeeb09831e9b", null ],
      [ "kFLEXIO_I2S_SampleRate96KHz", "a00037.html#gga59290805116faa55aee36e2eb01e17d2a5d26aebf6a38300f5b365cbe84bac915", null ]
    ] ],
    [ "_flexio_i2s_word_width", "a00037.html#gacdaf4100e8cae30648e1d1e59921971c", [
      [ "kFLEXIO_I2S_WordWidth8bits", "a00037.html#ggacdaf4100e8cae30648e1d1e59921971ca7c48374533cf9da13e97c4140142df54", null ],
      [ "kFLEXIO_I2S_WordWidth16bits", "a00037.html#ggacdaf4100e8cae30648e1d1e59921971ca7b9f51780b4da157105774eb145b900e", null ],
      [ "kFLEXIO_I2S_WordWidth24bits", "a00037.html#ggacdaf4100e8cae30648e1d1e59921971cad5f6e6d1944dc2eedb868d397eb47418", null ],
      [ "kFLEXIO_I2S_WordWidth32bits", "a00037.html#ggacdaf4100e8cae30648e1d1e59921971caa605b9bea21a94a75d862dcc416d694b", null ]
    ] ],
    [ "FLEXIO_I2S_Init", "a00037.html#ga2e15edfb41b26c8b6c1172e59b4e5ca2", null ],
    [ "FLEXIO_I2S_GetDefaultConfig", "a00037.html#ga63430c0096dd4f0efa0576b9f8d8e715", null ],
    [ "FLEXIO_I2S_Deinit", "a00037.html#ga39a22e12f1f18ac14a018a3a26f2d6d5", null ],
    [ "FLEXIO_I2S_Enable", "a00037.html#gac5e2d12ff598e8063622799bed7a7c07", null ],
    [ "FLEXIO_I2S_GetStatusFlags", "a00037.html#ga6392f280ddddea41518ab6f17bc10cff", null ],
    [ "FLEXIO_I2S_EnableInterrupts", "a00037.html#ga0cab52ff3cd5ee103cbc785d0982ee7a", null ],
    [ "FLEXIO_I2S_DisableInterrupts", "a00037.html#gac82fba515a043339d8c131ebda348510", null ],
    [ "FLEXIO_I2S_TxEnableDMA", "a00037.html#ga7163a1f52584f134fc328574ccc208d0", null ],
    [ "FLEXIO_I2S_RxEnableDMA", "a00037.html#ga3542137bbd509b5657097f3e0aa2e4bf", null ],
    [ "FLEXIO_I2S_TxGetDataRegisterAddress", "a00037.html#gaa407468114d14b18d58745464df5151f", null ],
    [ "FLEXIO_I2S_RxGetDataRegisterAddress", "a00037.html#gabc9503a27418cb0e1110803853171fb1", null ],
    [ "FLEXIO_I2S_MasterSetFormat", "a00037.html#ga12f775c48ef0376616d271de5e224e03", null ],
    [ "FLEXIO_I2S_SlaveSetFormat", "a00037.html#ga31e254b013b68f5c84a45794ee67e9b6", null ],
    [ "FLEXIO_I2S_WriteBlocking", "a00037.html#gadf86d43e7db486d68ddf3c41d12ef37c", null ],
    [ "FLEXIO_I2S_WriteData", "a00037.html#ga19b30ed9f8b1fc37675bc92cac35b871", null ],
    [ "FLEXIO_I2S_ReadBlocking", "a00037.html#gac03239be31a9c4768e2d409de9e455f9", null ],
    [ "FLEXIO_I2S_ReadData", "a00037.html#ga87630cef372b980eb1c39cb339c432d0", null ],
    [ "FLEXIO_I2S_TransferTxCreateHandle", "a00037.html#ga31c71deb66991f9b63e80c88652461c5", null ],
    [ "FLEXIO_I2S_TransferSetFormat", "a00037.html#ga725a2486896d3e60f2723d651bd82b6f", null ],
    [ "FLEXIO_I2S_TransferRxCreateHandle", "a00037.html#ga38f2a431871f5dfd9d07905b0f5ed81a", null ],
    [ "FLEXIO_I2S_TransferSendNonBlocking", "a00037.html#ga225948d3be017124dc7db4518e6a41d4", null ],
    [ "FLEXIO_I2S_TransferReceiveNonBlocking", "a00037.html#ga474f218ab7765525d1fd30ed10c04437", null ],
    [ "FLEXIO_I2S_TransferAbortSend", "a00037.html#ga3fb83f37b693d00a84f9b3108690cdff", null ],
    [ "FLEXIO_I2S_TransferAbortReceive", "a00037.html#ga95208c72642fe24909c7708c1efbd9a4", null ],
    [ "FLEXIO_I2S_TransferGetSendCount", "a00037.html#ga590a1845d2f37e2d8ca67d95a85b842a", null ],
    [ "FLEXIO_I2S_TransferGetReceiveCount", "a00037.html#gab1c4b7d6e2c63bdbd33a86ad4acf8c25", null ],
    [ "FLEXIO_I2S_TransferTxHandleIRQ", "a00037.html#gab7706d68288d8c1fa2ad05d43ed459fe", null ],
    [ "FLEXIO_I2S_TransferRxHandleIRQ", "a00037.html#gacb80c2a8216a31744378ebb39ad65c2b", null ]
];