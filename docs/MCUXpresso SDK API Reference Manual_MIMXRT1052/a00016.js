var a00016 =
[
    [ "csi_config_t", "a00016.html#a00351", [
      [ "width", "a00016.html#ac5d64ed31de712862131d8ba0c0acada", null ],
      [ "height", "a00016.html#af353234bdd91adf25b2bfa24cec0288e", null ],
      [ "polarityFlags", "a00016.html#a5715e589bee1f54aac85ab996a59f204", null ],
      [ "bytesPerPixel", "a00016.html#ae55dcf6dcbf3635432320d5a81ff9c4b", null ],
      [ "linePitch_Bytes", "a00016.html#ae1642a6be28003bcb2642e6931cc75dd", null ],
      [ "workMode", "a00016.html#a13aec4d48580eed8cd9ea6ee9952b408", null ],
      [ "dataBus", "a00016.html#a0c7630ee5ca46f6cf1a23a83378bdb1f", null ],
      [ "useExtVsync", "a00016.html#a436dcb1c1ed0cd25910b9df57267bd02", null ]
    ] ],
    [ "csi_handle_t", "a00016.html#a00302", [
      [ "frameBufferQueue", "a00016.html#aa1a9c395647713b07bc92e4b89a9a547", null ],
      [ "queueWriteIdx", "a00016.html#a98aac4e50e3a3375e6dd6199e8135527", null ],
      [ "queueReadIdx", "a00016.html#afe1aaca7fbcc6240601eec4106026bf5", null ],
      [ "emptyBuffer", "a00016.html#aefe7a48a1af919dafb2d60e8a2bd46e7", null ],
      [ "emptyBufferCnt", "a00016.html#aee71b528f87fee4bfe8b58d8940e91f2", null ],
      [ "activeBufferNum", "a00016.html#ab7f697fa8fe1046abab800cb46b66137", null ],
      [ "transferStarted", "a00016.html#a2a6ce7912da50c9dcfb863e2f86cfff3", null ],
      [ "callback", "a00016.html#a902b1fc7c58b91d0c4c41fc271c06a3c", null ],
      [ "userData", "a00016.html#a6b8b45ae015b412d2a9bb94925ff844f", null ]
    ] ],
    [ "CSI_DRIVER_QUEUE_SIZE", "a00016.html#ga6f3f169f7ba5517e946634a28add89e1", null ],
    [ "CSI_DRIVER_FRAG_MODE", "a00016.html#ga24e366c9c656595ba3258f0e6bc59c74", null ],
    [ "csi_transfer_callback_t", "a00016.html#ga68a7e9c86a91b7ffb6547cf762ef6f3f", [
      [ "kStatus_CSI_NoEmptyBuffer", "a00016.html#ggadf764cbdea00d65edcd07bb9953ad2b7a14ddc6ba82ee752fdfe1f4b0e64d5533", null ],
      [ "kStatus_CSI_NoFullBuffer", "a00016.html#ggadf764cbdea00d65edcd07bb9953ad2b7a4fb022ca8ae559e96d18524e7ea09a5f", null ],
      [ "kStatus_CSI_QueueFull", "a00016.html#ggadf764cbdea00d65edcd07bb9953ad2b7a6517cec370f757b84ba21eb690e51635", null ],
      [ "kStatus_CSI_FrameDone", "a00016.html#ggadf764cbdea00d65edcd07bb9953ad2b7ac9a47e7b267d5ece60e04306d2e245d2", null ]
    ] ],
    [ "csi_work_mode_t", "a00016.html#ga5539d9ae21ae84983a8ed9418c83cc5f", [
      [ "kCSI_GatedClockMode", "a00016.html#gga5539d9ae21ae84983a8ed9418c83cc5fab707b88b2c5b3ca70afe6b78f2988571", null ],
      [ "kCSI_NonGatedClockMode", "a00016.html#gga5539d9ae21ae84983a8ed9418c83cc5faefff73d6d0ed48e20bb12df629036633", null ],
      [ "kCSI_CCIR656ProgressiveMode", "a00016.html#gga5539d9ae21ae84983a8ed9418c83cc5fabab5a5dfbcc1c77bc66513760fdb0ddf", null ]
    ] ],
    [ "csi_data_bus_t", "a00016.html#gab69baf9a3cfb4a7be68e3ba250716df5", [
      [ "kCSI_DataBus8Bit", "a00016.html#ggab69baf9a3cfb4a7be68e3ba250716df5ae6e9f2234f6b659241d9d504457e04dd", null ],
      [ "kCSI_DataBus16Bit", "a00016.html#ggab69baf9a3cfb4a7be68e3ba250716df5ab41330fa7ebc27664112a4b592b53626", null ],
      [ "kCSI_DataBus24Bit", "a00016.html#ggab69baf9a3cfb4a7be68e3ba250716df5ad26851c8a6a7be5e501625034e0f6a99", null ]
    ] ],
    [ "_csi_polarity_flags", "a00016.html#gafda41057e6d13039ca0d139b89abde64", [
      [ "kCSI_HsyncActiveLow", "a00016.html#ggafda41057e6d13039ca0d139b89abde64aa1f34086d7ab2823fc0036e30e0b5889", null ],
      [ "kCSI_HsyncActiveHigh", "a00016.html#ggafda41057e6d13039ca0d139b89abde64aab952a4e21500ee629a8bd16ece03b3f", null ],
      [ "kCSI_DataLatchOnRisingEdge", "a00016.html#ggafda41057e6d13039ca0d139b89abde64aa98b2058628ba0d2bff537f81df0bfc9", null ],
      [ "kCSI_DataLatchOnFallingEdge", "a00016.html#ggafda41057e6d13039ca0d139b89abde64a86d385db0cc0163dc5a0a44aad733a80", null ],
      [ "kCSI_VsyncActiveHigh", "a00016.html#ggafda41057e6d13039ca0d139b89abde64a4926dcbdf349c2cf3d8dd88d12f4bb9f", null ],
      [ "kCSI_VsyncActiveLow", "a00016.html#ggafda41057e6d13039ca0d139b89abde64a9c1a612c6d6e49ba13091df9d6046aab", null ]
    ] ],
    [ "csi_fifo_t", "a00016.html#gaf354282828a94ddb23079e6e98399520", [
      [ "kCSI_RxFifo", "a00016.html#ggaf354282828a94ddb23079e6e98399520a2665957976c094c559d20661126fc914", null ],
      [ "kCSI_StatFifo", "a00016.html#ggaf354282828a94ddb23079e6e98399520a36df4b7466c177205c81d8fd4fe8dfa4", null ],
      [ "kCSI_AllFifo", "a00016.html#ggaf354282828a94ddb23079e6e98399520a960822cab30b37b78e71b03754b1adfc", null ]
    ] ],
    [ "_csi_interrupt_enable", "a00016.html#ga445b9c8db2ffe230018ced411b88cc3e", [
      [ "kCSI_EndOfFrameInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ea7346f16c89a990476f2551ceb7b40065", null ],
      [ "kCSI_ChangeOfFieldInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ead360f344cb258fc94c31091c60a9921c", null ],
      [ "kCSI_StatFifoOverrunInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ead3eae43cd538d4963a92537e10857375", null ],
      [ "kCSI_RxFifoOverrunInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3eabe9ee51fbfa0916ac89d43cf1dceec5a", null ],
      [ "kCSI_StatFifoDmaDoneInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ea88e9447b7f3d6730a8817418a9149bdf", null ],
      [ "kCSI_StatFifoFullInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ea25e1d2b38d337c7ac800a14ed153496a", null ],
      [ "kCSI_RxBuffer1DmaDoneInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3eab51980279060e64d99dea0b615b1a6eb", null ],
      [ "kCSI_RxBuffer0DmaDoneInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3eaa93e6aebf81cc468b6827ecae5e996e3", null ],
      [ "kCSI_RxFifoFullInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ea32b521a89db1b0a953e54ff82ddaafe1", null ],
      [ "kCSI_StartOfFrameInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ea1f737081cd588e20b0e37adda571380a", null ],
      [ "kCSI_EccErrorInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ea426a30446ecdcd5a8ef11d7114587afc", null ],
      [ "kCSI_AhbResErrorInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3eae5a566edbf0dbd027cbd20bc6649fb00", null ],
      [ "kCSI_BaseAddrChangeErrorInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ea485b9acd1e879204a29e1351eef0f34b", null ],
      [ "kCSI_Field0DoneInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ea53b2171811b48986f94d99640bc293e6", null ],
      [ "kCSI_Field1DoneInterruptEnable", "a00016.html#gga445b9c8db2ffe230018ced411b88cc3ea51b6eae094b905ec5c42f09330ab0cc2", null ]
    ] ],
    [ "_csi_flags", "a00016.html#gac962030ce8597d68455e55874a51721f", [
      [ "kCSI_RxFifoDataReadyFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa32acc9a0622596c7f830e6d70568931c", null ],
      [ "kCSI_EccErrorFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa3e02610408a6ebfe221869a1923009ba", null ],
      [ "kCSI_AhbResErrorFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa984df7af7f9825fb4659d11f42463f44", null ],
      [ "kCSI_ChangeOfFieldFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fabe92f518f5c2f0ccbe55ebdcf4c29e89", null ],
      [ "kCSI_Field0PresentFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fab337940c172c17058f8cde8bb676378f", null ],
      [ "kCSI_Field1PresentFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa157a47bde1ba661fdf22ca2ae61b3c0b", null ],
      [ "kCSI_StartOfFrameFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa064c85b25c414abdeb86d501eb57360c", null ],
      [ "kCSI_EndOfFrameFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fac6409ca23ef24a10e22fb9582213f2bc", null ],
      [ "kCSI_RxFifoFullFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fabdd835d96491b90bf5ae39dc5e4b0497", null ],
      [ "kCSI_RxBuffer1DmaDoneFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa27f18871d1b522ca628722a3fb80b89d", null ],
      [ "kCSI_RxBuffer0DmaDoneFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa3168d94660e46e761c0639a1a20b74ee", null ],
      [ "kCSI_StatFifoFullFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa920018020044eb07e3ffed1f423208a5", null ],
      [ "kCSI_StatFifoDmaDoneFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa961784728b693f7741adb7b17ebe425b", null ],
      [ "kCSI_StatFifoOverrunFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa0350ec81b55a8180eaffcd8b1f537cdf", null ],
      [ "kCSI_RxFifoOverrunFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fab01731bc864c9b47644a6de2bc2f88d5", null ],
      [ "kCSI_Field0DoneFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa2676d308e2677cc91f47ae0661355d10", null ],
      [ "kCSI_Field1DoneFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa6e7f6d415b0e9848c2f33f25ea9c0036", null ],
      [ "kCSI_BaseAddrChangeErrorFlag", "a00016.html#ggac962030ce8597d68455e55874a51721fa254be4a91697e3bf3146a663454c2e60", null ]
    ] ],
    [ "CSI_Init", "a00016.html#ga6ab1581b894e0addd36e259454d894fb", null ],
    [ "CSI_Deinit", "a00016.html#ga29130aa6333127427e9945203a941c31", null ],
    [ "CSI_Reset", "a00016.html#gaaf0c3b036356a13b113c190762035317", null ],
    [ "CSI_GetDefaultConfig", "a00016.html#ga44bd4343d5866367abc152651f3d8a03", null ],
    [ "CSI_ClearFifo", "a00016.html#gaa9a972883f2e04aefb94bac426fa9c72", null ],
    [ "CSI_ReflashFifoDma", "a00016.html#gabd61e583724350ad66feacf6cb55e86c", null ],
    [ "CSI_EnableFifoDmaRequest", "a00016.html#ga266530ad9e8aa0e0e4977317b5cd9899", null ],
    [ "CSI_Start", "a00016.html#gace757ddd954a29a1a11c0bdc119b17ad", null ],
    [ "CSI_Stop", "a00016.html#ga49731f4255e64f48eb0f26628296c13e", null ],
    [ "CSI_SetRxBufferAddr", "a00016.html#ga17f23deb7d61ad8d88b10165e512d50d", null ],
    [ "CSI_EnableInterrupts", "a00016.html#ga433cf3d4d36b2d602b2e41b415abd85f", null ],
    [ "CSI_DisableInterrupts", "a00016.html#gaaef95a89e63407cdb28b9dbf81ce4fe5", null ],
    [ "CSI_GetStatusFlags", "a00016.html#gac756a072ac5afdb9567632d02d13e73f", null ],
    [ "CSI_ClearStatusFlags", "a00016.html#ga9368862568891bbdb29b0a6287801506", null ],
    [ "CSI_TransferCreateHandle", "a00016.html#gaff15e4c1cd458f686e4f296ce3d417d5", null ],
    [ "CSI_TransferStart", "a00016.html#ga65a17c86c64297a09857e435588a65d6", null ],
    [ "CSI_TransferStop", "a00016.html#gac20498234465760225985287ff862751", null ],
    [ "CSI_TransferSubmitEmptyBuffer", "a00016.html#ga807a6677a22856cf8ad1e37253b14d44", null ],
    [ "CSI_TransferGetFullBuffer", "a00016.html#ga36f7bf646f55c098fb12d6fa387d6424", null ],
    [ "CSI_TransferHandleIRQ", "a00016.html#gaa8118210b401dacd2257dbb75c6a6310", null ]
];