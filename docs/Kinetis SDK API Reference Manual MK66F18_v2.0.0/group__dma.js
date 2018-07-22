var group__dma =
[
    [ "dma_transfer_config_t", "group__dma.html#structdma__transfer__config__t", [
      [ "srcAddr", "group__dma.html#a9a8518955d5b39c2716a2c8ea9b1f4bc", null ],
      [ "destAddr", "group__dma.html#ae5cbd4f566ccaf8d1ac4127372c7e476", null ],
      [ "enableSrcIncrement", "group__dma.html#a33cc4e86366a8e9a0051e404f651e793", null ],
      [ "srcSize", "group__dma.html#a55b086bf0f5cb86d5f26a3ddd39860e2", null ],
      [ "enableDestIncrement", "group__dma.html#adea005db854d2deb273e17700015cbf1", null ],
      [ "destSize", "group__dma.html#a1cc4200fd89b96b4ed5f6a9605c5f43e", null ],
      [ "transferSize", "group__dma.html#a1a7c8942a628323e89f489a1656d5859", null ]
    ] ],
    [ "dma_channel_link_config_t", "group__dma.html#structdma__channel__link__config__t", [
      [ "linkType", "group__dma.html#a377415cfa6e618ecfb2ba154e828cf8b", null ],
      [ "channel1", "group__dma.html#afc71a80292db667048cb89cf952aa04a", null ],
      [ "channel2", "group__dma.html#ab708f793951a1729ff8687adb8dcf696", null ]
    ] ],
    [ "dma_handle_t", "group__dma.html#structdma__handle__t", [
      [ "base", "group__dma.html#a84d667acc1301d6d56ae52573e4b6b87", null ],
      [ "channel", "group__dma.html#a7c068d330cc60423ee3fd86821221b85", null ],
      [ "callback", "group__dma.html#a47a5c6af4c934cc9db355d394bb94f46", null ],
      [ "userData", "group__dma.html#a2a10a0701fc2085ce58a0e35032e8a8e", null ]
    ] ],
    [ "FSL_DMA_DRIVER_VERSION", "group__dma.html#gac68c8082b53756a7e58ec6d5f25117d2", null ],
    [ "dma_callback", "group__dma.html#ga977bec6df5a2af46f45ab22e30ea0a25", null ],
    [ "_dma_channel_status_flags", "group__dma.html#ga36e753dba44ae75ed949766e19cc113d", [
      [ "kDMA_TransactionsBCRFlag", "group__dma.html#gga36e753dba44ae75ed949766e19cc113da2d781b440cc4a6f79a1aceb6cf06cd07", null ],
      [ "kDMA_TransactionsDoneFlag", "group__dma.html#gga36e753dba44ae75ed949766e19cc113dae890ef1d8993fb00d8d95b7f3feb7d39", null ],
      [ "kDMA_TransactionsBusyFlag", "group__dma.html#gga36e753dba44ae75ed949766e19cc113da28dabe9487a26e3cacbce41b50752a70", null ],
      [ "kDMA_TransactionsRequestFlag", "group__dma.html#gga36e753dba44ae75ed949766e19cc113da20f859fbcfa07c2a4b4a5b273910fbc6", null ],
      [ "kDMA_BusErrorOnDestinationFlag", "group__dma.html#gga36e753dba44ae75ed949766e19cc113da38ba4fb9195801c5825242cdef9b582d", null ],
      [ "kDMA_BusErrorOnSourceFlag", "group__dma.html#gga36e753dba44ae75ed949766e19cc113da3a89390c5832154e2da5e909f7655922", null ],
      [ "kDMA_ConfigurationErrorFlag", "group__dma.html#gga36e753dba44ae75ed949766e19cc113da85f087aebd042b357f46dadda6bbce83", null ]
    ] ],
    [ "dma_transfer_size_t", "group__dma.html#ga4f754951efc1486472c14f009a02ac47", [
      [ "kDMA_Transfersize32bits", "group__dma.html#gga4f754951efc1486472c14f009a02ac47a969ae3a6429e54e8a301b26ecf3ab017", null ],
      [ "kDMA_Transfersize8bits", "group__dma.html#gga4f754951efc1486472c14f009a02ac47a14a03019f98a6f50dc1c4588d995f9d3", null ],
      [ "kDMA_Transfersize16bits", "group__dma.html#gga4f754951efc1486472c14f009a02ac47a02ab56512322e1a677b7ef2911735849", null ]
    ] ],
    [ "dma_modulo_t", "group__dma.html#gac76d0efebec5b4912a0156f9d7d1c3b2", [
      [ "kDMA_ModuloDisable", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a8bd75d139becfb0a58d08a60e3682e87", null ],
      [ "kDMA_Modulo16Bytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2abef7e8c58588f33dffdbea7876ad4a35", null ],
      [ "kDMA_Modulo32Bytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a9989156fcb25590c9235239b657ec79f", null ],
      [ "kDMA_Modulo64Bytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a6b3277aa7b8571ddf1cc7d2b5ee620d1", null ],
      [ "kDMA_Modulo128Bytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a7bc2227e57ce0ff1dc41b2c71e877207", null ],
      [ "kDMA_Modulo256Bytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a2c92174d5b350cfddb0a836661bb2625", null ],
      [ "kDMA_Modulo512Bytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a83473cd4366829f5d23e52d8eeb8b4d5", null ],
      [ "kDMA_Modulo1KBytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2af419011a303b2b12dfc9e1151ce7d64e", null ],
      [ "kDMA_Modulo2KBytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2ac1dfc6c6ffe60f4000cdd465d931e18d", null ],
      [ "kDMA_Modulo4KBytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a01080cdca6f101fd35b062465823bbdc", null ],
      [ "kDMA_Modulo8KBytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a11cdc1d729d302ba4f5a198ebd597e6f", null ],
      [ "kDMA_Modulo16KBytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a02785f9374e866f65bc069e7b3860b7c", null ],
      [ "kDMA_Modulo32KBytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a0b6b6c652ee4630f01ce068607d5a23e", null ],
      [ "kDMA_Modulo64KBytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2afe618f71bddf6f80dcd7e3f3169a34e0", null ],
      [ "kDMA_Modulo128KBytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2abdb5276d85626400218399229879e050", null ],
      [ "kDMA_Modulo256KBytes", "group__dma.html#ggac76d0efebec5b4912a0156f9d7d1c3b2a868be515efaf346dc08b08a37a239654", null ]
    ] ],
    [ "dma_channel_link_type_t", "group__dma.html#ga4ff91ace4e5146d14cd505235a2c618e", [
      [ "kDMA_ChannelLinkDisable", "group__dma.html#gga4ff91ace4e5146d14cd505235a2c618eaf31c1f53736aa000546413022b8c3280", null ],
      [ "kDMA_ChannelLinkChannel1AndChannel2", "group__dma.html#gga4ff91ace4e5146d14cd505235a2c618ea0d5d1186513e85268a131166d042497c", null ],
      [ "kDMA_ChannelLinkChannel1", "group__dma.html#gga4ff91ace4e5146d14cd505235a2c618ea580b4a8b49c2d75de4171bcf0d2615b4", null ],
      [ "kDMA_ChannelLinkChannel1AfterBCR0", "group__dma.html#gga4ff91ace4e5146d14cd505235a2c618ea10fb1aa6120440f198e6e6ccf6100bc1", null ]
    ] ],
    [ "dma_transfer_type_t", "group__dma.html#ga9cb7087af6efc80106c1033f80d60219", [
      [ "kDMA_MemoryToMemory", "group__dma.html#gga9cb7087af6efc80106c1033f80d60219a24392a93deeb55e04559201839343f3c", null ],
      [ "kDMA_PeripheralToMemory", "group__dma.html#gga9cb7087af6efc80106c1033f80d60219a7bf1938ae68b1f494aade7a0f4189303", null ],
      [ "kDMA_MemoryToPeripheral", "group__dma.html#gga9cb7087af6efc80106c1033f80d60219aae1c6bab576d7a25d2fa1249f9cd4ee9", null ]
    ] ],
    [ "dma_transfer_options_t", "group__dma.html#ga1c849fdcafc858964b6a7f2e0d75390c", [
      [ "kDMA_NoOptions", "group__dma.html#gga1c849fdcafc858964b6a7f2e0d75390caddc6103be52651841e795cccc872988b", null ],
      [ "kDMA_EnableInterrupt", "group__dma.html#gga1c849fdcafc858964b6a7f2e0d75390cad289b8b98ee6cf2faa4788404fe809d8", null ]
    ] ],
    [ "_dma_transfer_status", "group__dma.html#ga63c0c8f218fd21a01a4996032b1f6ee0", null ],
    [ "DMA_Init", "group__dma.html#gade1b5efa61054ce538b37b181dd075bb", null ],
    [ "DMA_Deinit", "group__dma.html#ga634ced9b86d7dc9543e0b4387123fcac", null ],
    [ "DMA_ResetChannel", "group__dma.html#ga17a6d7e5f9d3f12777941447ed7659f5", null ],
    [ "DMA_SetTransferConfig", "group__dma.html#ga3cc33ab27c5c0dc71cff81795522629a", null ],
    [ "DMA_SetChannelLinkConfig", "group__dma.html#ga9a0aba2d09ef438d7faf8d4c4d458c11", null ],
    [ "DMA_SetSourceAddress", "group__dma.html#ga980caf1dc812aaab7b6113fb52b91bc2", null ],
    [ "DMA_SetDestinationAddress", "group__dma.html#gaac2cf26389fdac8720edc98cd8e21aec", null ],
    [ "DMA_SetTransferSize", "group__dma.html#ga76430445da424d0c768399dbbfb1930d", null ],
    [ "DMA_SetModulo", "group__dma.html#ga813a9ddafd11393d8848a946622dd80d", null ],
    [ "DMA_EnableCycleSteal", "group__dma.html#ga7d50e7ada536a2acfc626cc477075ef4", null ],
    [ "DMA_EnableAutoAlign", "group__dma.html#ga1fd6efd06eefc766c72493ca249c06d0", null ],
    [ "DMA_EnableAsyncRequest", "group__dma.html#gaafbc7d79765ae6f456175e64aa6fd399", null ],
    [ "DMA_EnableInterrupts", "group__dma.html#gaebf70b28223b77c631c3b6cfd3f2bb91", null ],
    [ "DMA_DisableInterrupts", "group__dma.html#ga2d0b06f800cecf31b418692cb8d328ea", null ],
    [ "DMA_EnableChannelRequest", "group__dma.html#gaf554aea248ac4941adc8a97b0549cde9", null ],
    [ "DMA_DisableChannelRequest", "group__dma.html#ga796491fd14d466d3a8073644ca65c524", null ],
    [ "DMA_TriggerChannelStart", "group__dma.html#ga7c779380909187ecbec0b15cbc86a05f", null ],
    [ "DMA_GetRemainingBytes", "group__dma.html#ga42aa531ad5e3664b5d71852ad2710d14", null ],
    [ "DMA_GetChannelStatusFlags", "group__dma.html#gab5faf6f718f3aa9b32d0a4ccd45bb0ac", null ],
    [ "DMA_ClearChannelStatusFlags", "group__dma.html#gacb769949a98ea36a24a14dc437157eb2", null ],
    [ "DMA_CreateHandle", "group__dma.html#ga5354ff2c0c1ec53e2cbd712169fc5558", null ],
    [ "DMA_SetCallback", "group__dma.html#ga2eb9a831b9a84c5108097f770dbe90db", null ],
    [ "DMA_PrepareTransfer", "group__dma.html#gaf6e0b93e92a31e0dd3398bce34decb40", null ],
    [ "DMA_SubmitTransfer", "group__dma.html#gadf9824800405f5b161bae1db1bf7d1f7", null ],
    [ "DMA_StartTransfer", "group__dma.html#gaf33bec6e25c422d61a41949c9a8283d4", null ],
    [ "DMA_StopTransfer", "group__dma.html#ga351567089b3c6a45f85d078a42a6a0be", null ],
    [ "DMA_AbortTransfer", "group__dma.html#ga3c61d6121d88d2cdf287fc1cd72912fd", null ],
    [ "DMA_HandleIRQ", "group__dma.html#ga3484c491bd7943623d1d8fdc013c7e97", null ]
];