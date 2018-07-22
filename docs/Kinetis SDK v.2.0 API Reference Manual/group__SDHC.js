var group__sdhc =
[
    [ "sdhc_adma2_descriptor_t", "group__sdhc.html#structsdhc__adma2__descriptor__t", [
      [ "attribute", "group__sdhc.html#a036f3555dbbd0db4df2dcc62786bb9c3", null ],
      [ "address", "group__sdhc.html#afce107d67a63c9b306082da582cdf2d9", null ]
    ] ],
    [ "sdhc_capability_t", "group__sdhc.html#structsdhc__capability__t", [
      [ "specVersion", "group__sdhc.html#aa3dbbe493bcad7009ab1f8ce98ef1810", null ],
      [ "vendorVersion", "group__sdhc.html#adf1780607151276c66a01c0e6f34159b", null ],
      [ "maxBlockLength", "group__sdhc.html#a888aee84340011e87920d49cbd790fe7", null ],
      [ "maxBlockCount", "group__sdhc.html#adb1dbc1b93eb1326d0a89223ad82b68c", null ],
      [ "flags", "group__sdhc.html#a05a5ceb542c93aa959045731ee3e76a5", null ]
    ] ],
    [ "sdhc_transfer_config_t", "group__sdhc.html#structsdhc__transfer__config__t", [
      [ "dataBlockSize", "group__sdhc.html#afbcde58027d1f343bd475b5b8bb35d8f", null ],
      [ "dataBlockCount", "group__sdhc.html#a8721f9d8daadb5e73ec901d29fb0dcdf", null ],
      [ "commandArgument", "group__sdhc.html#ab259acec4e8e6690606ae8fac58988cf", null ],
      [ "commandIndex", "group__sdhc.html#aff239b3f81adb8d643066ac82a307ee8", null ],
      [ "flags", "group__sdhc.html#a883bfde89525fafed05c9ce467652f57", null ]
    ] ],
    [ "sdhc_boot_config_t", "group__sdhc.html#structsdhc__boot__config__t", [
      [ "ackTimeoutCount", "group__sdhc.html#aeecfe80bfb2c686c4733ca432954379a", null ],
      [ "bootMode", "group__sdhc.html#ab37976cec68200e32f8bb75436cd1e07", null ],
      [ "blockCount", "group__sdhc.html#ad651d25b9152290899b592c4d4390ccb", null ],
      [ "enableBootAck", "group__sdhc.html#a2c9e48eef0bd6de34a833d8850cb6c4d", null ],
      [ "enableBoot", "group__sdhc.html#a4002f25dea21d34b4bb44ab37de0a9f9", null ],
      [ "enableAutoStopAtBlockGap", "group__sdhc.html#af0880a5e5f436bccddd729d122f495fd", null ]
    ] ],
    [ "sdhc_config_t", "group__sdhc.html#structsdhc__config__t", [
      [ "cardDetectDat3", "group__sdhc.html#a69f01c6064f5943930b466e0705f007e", null ],
      [ "endianMode", "group__sdhc.html#a3fe664ce0f37701c3642039090b94e44", null ],
      [ "dmaMode", "group__sdhc.html#a797abbb6dfdbec0792fef51e02b3a783", null ],
      [ "readWatermarkLevel", "group__sdhc.html#a9b463d38b963a076c0b331042201b7f1", null ],
      [ "writeWatermarkLevel", "group__sdhc.html#a093b92cb665be8b3d5c7c2d2d02fe828", null ]
    ] ],
    [ "sdhc_data_t", "group__sdhc.html#structsdhc__data__t", [
      [ "enableAutoCommand12", "group__sdhc.html#a16d8a412bffabb67833e1bc23ee990c9", null ],
      [ "enableIgnoreError", "group__sdhc.html#a74cd94a77ec2d8e131b7d8f83da98c4f", null ],
      [ "blockSize", "group__sdhc.html#a7d02cfe45ebe063197fee25e57de6022", null ],
      [ "blockCount", "group__sdhc.html#a275c9defd44ef986999a673a5b61691b", null ],
      [ "rxData", "group__sdhc.html#a8c6eaa7be8ced5bf80a2b392ca760093", null ],
      [ "txData", "group__sdhc.html#aabcd72932e420a0062fbd1c66c49db35", null ]
    ] ],
    [ "sdhc_command_t", "group__sdhc.html#structsdhc__command__t", [
      [ "index", "group__sdhc.html#a68a552e22462e4a65826d44d1681f7fa", null ],
      [ "argument", "group__sdhc.html#a66ed3ccd69e07c0ca68f483c0612108f", null ],
      [ "type", "group__sdhc.html#aa1c359220b3fbb5e3a2edc2f22bc24a9", null ],
      [ "responseType", "group__sdhc.html#a6146398883331e125574399f795515e9", null ],
      [ "response", "group__sdhc.html#adca4b664acb81017168cfbb7c5a2c9f3", null ]
    ] ],
    [ "sdhc_transfer_t", "group__sdhc.html#structsdhc__transfer__t", [
      [ "data", "group__sdhc.html#a291ef4f8184b137bf9d4d0a90377f70f", null ],
      [ "command", "group__sdhc.html#ab78ebd9d3823a9dbfac98d5bb9b5db67", null ]
    ] ],
    [ "sdhc_transfer_callback_t", "group__sdhc.html#structsdhc__transfer__callback__t", [
      [ "CardInserted", "group__sdhc.html#af13fb36a8dcaa7e844aeefc9a15ba976", null ],
      [ "CardRemoved", "group__sdhc.html#a8342c786787456975e5eed609175e3de", null ],
      [ "SdioInterrupt", "group__sdhc.html#a68c8c175a46573d1f45c17dc7b61fcf9", null ],
      [ "SdioBlockGap", "group__sdhc.html#aca580ad44188899226852eb6b45787da", null ],
      [ "TransferComplete", "group__sdhc.html#a7ab9451d894ee52bdd7e288df5e975fd", null ]
    ] ],
    [ "sdhc_handle_t", "group__sdhc.html#struct__sdhc__handle", [
      [ "data", "group__sdhc.html#a13dd6ba498b65081dbf57cb5d4c07c4b", null ],
      [ "command", "group__sdhc.html#a76e9bccbd5e62df9790f18b342012763", null ],
      [ "interruptFlags", "group__sdhc.html#a877bf48c729bb024dd28db336656baae", null ],
      [ "transferredWords", "group__sdhc.html#a3f3143b088b2c54f36ffff4c1eb53def", null ],
      [ "callback", "group__sdhc.html#a3e64fc85e2a67c9fa8914d27d1cf2679", null ],
      [ "userData", "group__sdhc.html#a5c35129aeb2ea81bd8fc119795913a7e", null ]
    ] ],
    [ "sdhc_host_t", "group__sdhc.html#structsdhc__host__t", [
      [ "base", "group__sdhc.html#aafb31f76cb20b0ad0026fea81f677c90", null ],
      [ "sourceClock_Hz", "group__sdhc.html#a521a85873b97642fa621937e40508ca4", null ],
      [ "config", "group__sdhc.html#ab534a374ef199022be8c5aaae42d23bb", null ],
      [ "capability", "group__sdhc.html#a98d486a0c9708a7237c8d607934f6454", null ],
      [ "transfer", "group__sdhc.html#acc596a5bbca9ecccc9f92a5c3e3c1f93", null ]
    ] ],
    [ "FSL_SDHC_DRIVER_VERSION", "group__sdhc.html#ga6c39bbdf9b37f182509d649092e91c37", null ],
    [ "SDHC_MAX_BLOCK_COUNT", "group__sdhc.html#ga4db0da4a98cf1e21d32bc99f5e898f40", null ],
    [ "SDHC_ADMA1_ADDRESS_ALIGN", "group__sdhc.html#ga5b7b8c872ea10c5f6fa59f6df93d6abd", null ],
    [ "SDHC_ADMA1_LENGTH_ALIGN", "group__sdhc.html#ga837d1099cc82805a1b11c840d61d0525", null ],
    [ "SDHC_ADMA2_ADDRESS_ALIGN", "group__sdhc.html#gab79cc93199b0e278e2e0aa8fa3057577", null ],
    [ "SDHC_ADMA2_LENGTH_ALIGN", "group__sdhc.html#ga38b2b0692611bea4be75daa1659478d8", null ],
    [ "SDHC_ADMA1_DESCRIPTOR_ADDRESS_SHIFT", "group__sdhc.html#ga2821ccdf48e69bff201da38bebbf1803", null ],
    [ "SDHC_ADMA1_DESCRIPTOR_ADDRESS_MASK", "group__sdhc.html#ga72623d09325ed63843a76a65c0214562", null ],
    [ "SDHC_ADMA1_DESCRIPTOR_LENGTH_SHIFT", "group__sdhc.html#ga32effc63d5138f96787bcf2bfc9b866f", null ],
    [ "SDHC_ADMA1_DESCRIPTOR_LENGTH_MASK", "group__sdhc.html#gaf81a4bc67010a39cdae0e589b01754ec", null ],
    [ "SDHC_ADMA1_DESCRIPTOR_MAX_LENGTH_PER_ENTRY", "group__sdhc.html#ga4d6ca2b13f280b3f47cfd07269dc662f", null ],
    [ "SDHC_ADMA2_DESCRIPTOR_LENGTH_SHIFT", "group__sdhc.html#gadc8bd7917b5f7d9f35a5bb7e9898fac5", null ],
    [ "SDHC_ADMA2_DESCRIPTOR_LENGTH_MASK", "group__sdhc.html#ga430b0e01b6037de59c3a82ee8a4189ca", null ],
    [ "SDHC_ADMA2_DESCRIPTOR_MAX_LENGTH_PER_ENTRY", "group__sdhc.html#gae76613428e6d575d3b0ce40061a6f2b5", null ],
    [ "sdhc_adma1_descriptor_t", "group__sdhc.html#ga2b6bc2598a8c8dfd3fe378b119cfb7e0", null ],
    [ "sdhc_transfer_function_t", "group__sdhc.html#ga5b23a0437ff12d34000b2e22796161b6", null ],
    [ "_sdhc_status", "group__sdhc.html#gac5fd878c73a717bd2b60e8e8b17ad260", [
      [ "kStatus_SDHC_BusyTransferring", "group__sdhc.html#ggac5fd878c73a717bd2b60e8e8b17ad260ab13836ffb5bfbeed70ab00ac8073bceb", null ],
      [ "kStatus_SDHC_PrepareAdmaDescriptorFailed", "group__sdhc.html#ggac5fd878c73a717bd2b60e8e8b17ad260a678d230c1fcb66d08e066c90f67ccd62", null ],
      [ "kStatus_SDHC_SendCommandFailed", "group__sdhc.html#ggac5fd878c73a717bd2b60e8e8b17ad260ac4d833192da4419737aa3843d0aea6b5", null ],
      [ "kStatus_SDHC_TransferDataFailed", "group__sdhc.html#ggac5fd878c73a717bd2b60e8e8b17ad260a91d2822f903f2f93705a2ff4e206b3be", null ]
    ] ],
    [ "_sdhc_capability_flag", "group__sdhc.html#ga3347b4274dd96478908e1bc21e3e3443", [
      [ "kSDHC_SupportAdmaFlag", "group__sdhc.html#gga3347b4274dd96478908e1bc21e3e3443a698ee473d69046334c18b6fc26cbfd98", null ],
      [ "kSDHC_SupportHighSpeedFlag", "group__sdhc.html#gga3347b4274dd96478908e1bc21e3e3443ac820746b8685d29c417fe2380707dba2", null ],
      [ "kSDHC_SupportDmaFlag", "group__sdhc.html#gga3347b4274dd96478908e1bc21e3e3443a3c8f0eeda5240cc60a9a2c93c09c7f96", null ],
      [ "kSDHC_SupportSuspendResumeFlag", "group__sdhc.html#gga3347b4274dd96478908e1bc21e3e3443a6e2d77c2ce66720eda58c5a258a4834c", null ],
      [ "kSDHC_SupportV330Flag", "group__sdhc.html#gga3347b4274dd96478908e1bc21e3e3443a209c0bb2c7b9bdaaf732b6902af8eb8e", null ],
      [ "kSDHC_Support4BitFlag", "group__sdhc.html#gga3347b4274dd96478908e1bc21e3e3443a5fbbf064568e6a555a04b19cfe1caf4c", null ],
      [ "kSDHC_Support8BitFlag", "group__sdhc.html#gga3347b4274dd96478908e1bc21e3e3443a96179960f4d7e674f6dfc5e6cc6f4ac0", null ]
    ] ],
    [ "_sdhc_wakeup_event", "group__sdhc.html#ga2f18c3d83f0a619a80885d669956b342", [
      [ "kSDHC_WakeupEventOnCardInt", "group__sdhc.html#gga2f18c3d83f0a619a80885d669956b342af2adcaaa50acdc5f4951b91958ac5c10", null ],
      [ "kSDHC_WakeupEventOnCardInsert", "group__sdhc.html#gga2f18c3d83f0a619a80885d669956b342a228c4a743fea6056dfb15fe0270d3450", null ],
      [ "kSDHC_WakeupEventOnCardRemove", "group__sdhc.html#gga2f18c3d83f0a619a80885d669956b342a41a88d954bc5f42bf0c574942d84f97f", null ],
      [ "kSDHC_WakeupEventsAll", "group__sdhc.html#gga2f18c3d83f0a619a80885d669956b342a0ce3b5848888785ea36adcd1ccff13fa", null ]
    ] ],
    [ "_sdhc_reset", "group__sdhc.html#ga3c30abc5419aed904ec6ea30bb993e97", [
      [ "kSDHC_ResetAll", "group__sdhc.html#gga3c30abc5419aed904ec6ea30bb993e97a957f09907aa11c598e653140357cff70", null ],
      [ "kSDHC_ResetCommand", "group__sdhc.html#gga3c30abc5419aed904ec6ea30bb993e97abfea23b92778e48263e4b4732d317d8a", null ],
      [ "kSDHC_ResetData", "group__sdhc.html#gga3c30abc5419aed904ec6ea30bb993e97adc5074a8e3ae21078d240f7563b23590", null ],
      [ "kSDHC_ResetsAll", "group__sdhc.html#gga3c30abc5419aed904ec6ea30bb993e97ad947faed78dc42e8341ca7ee1844ac2c", null ]
    ] ],
    [ "_sdhc_transfer_flag", "group__sdhc.html#ga19d918d40bbd32c24361b392c1b3807d", [
      [ "kSDHC_EnableDmaFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807dac1f8b317ea69625bd225dc3a7db785eb", null ],
      [ "kSDHC_CommandTypeSuspendFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807da121767a759ad1ec70506bdff7260afd6", null ],
      [ "kSDHC_CommandTypeResumeFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807da4f7d38b721d25fd279e08b6a42cf2232", null ],
      [ "kSDHC_CommandTypeAbortFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807da7175ed3d7989a2a51748e815dad5d4c8", null ],
      [ "kSDHC_EnableBlockCountFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807dab76aae16fbc2842fb7cdf2dae576e08d", null ],
      [ "kSDHC_EnableAutoCommand12Flag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807da0e59ac9118e52b3fc7f7ca837789cf5c", null ],
      [ "kSDHC_DataReadFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807da18be28e91706858b235e6084658a9114", null ],
      [ "kSDHC_MultipleBlockFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807da6b03cedf431979922a6481e06d90c5b2", null ],
      [ "kSDHC_ResponseLength136Flag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807dab01eab8a8e9bf3fa2fb92c41cbb23e06", null ],
      [ "kSDHC_ResponseLength48Flag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807da80b62319b425f3ad53b4e90ea2a56cb0", null ],
      [ "kSDHC_ResponseLength48BusyFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807da1f00bb7720497364190639e40ae2156b", null ],
      [ "kSDHC_EnableCrcCheckFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807dadeae905298ad904b7cfcc31eee9ac590", null ],
      [ "kSDHC_EnableIndexCheckFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807daf73867aae75d6305de999a9aa2018655", null ],
      [ "kSDHC_DataPresentFlag", "group__sdhc.html#gga19d918d40bbd32c24361b392c1b3807daebe4a18f467b454d367b8b1992fe8390", null ]
    ] ],
    [ "_sdhc_present_status_flag", "group__sdhc.html#gaf4373b0d2652db7719ff9bc45885573d", [
      [ "kSDHC_CommandInhibitFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573daeb394c4c64e3378de4f31f5ab3f8ec15", null ],
      [ "kSDHC_DataInhibitFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da815d0f4e2bc86fbab5c547050a7cb420", null ],
      [ "kSDHC_DataLineActiveFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da56a0c8bd03d69021075422b4855874a7", null ],
      [ "kSDHC_SdClockStableFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da06b30b0f8df3ded8285fa460123c1156", null ],
      [ "kSDHC_WriteTransferActiveFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573daceeaa773a162cd40850aa34b93ab0676", null ],
      [ "kSDHC_ReadTransferActiveFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da67282c1c9a07d87ac6e63b5998d0d6d9", null ],
      [ "kSDHC_BufferWriteEnableFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da11c6408ba0de5815e7be581d5fea3611", null ],
      [ "kSDHC_BufferReadEnableFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da08eb721256af653fa23bd4c7f11a8591", null ],
      [ "kSDHC_CardInsertedFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573daea59f2f09ba1e55873827532ec2e25c8", null ],
      [ "kSDHC_CommandLineLevelFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da46c325d708b5aa61ef5370b8251b9365", null ],
      [ "kSDHC_Data0LineLevelFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da15323ea985c43dc44fdd19b9166341b9", null ],
      [ "kSDHC_Data1LineLevelFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da0945be9842acd0cbe3dc17573d784d48", null ],
      [ "kSDHC_Data2LineLevelFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573dae390c98df94c2528f9684fd6c7396820", null ],
      [ "kSDHC_Data3LineLevelFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da47d141862ec5d72067f1c0bdf2e10f46", null ],
      [ "kSDHC_Data4LineLevelFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da9d88a695edf93ae1e499aadeaf042423", null ],
      [ "kSDHC_Data5LineLevelFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573da4ad7088466c057deade089129803fa92", null ],
      [ "kSDHC_Data6LineLevelFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573dad319efc3648f2a98e1bc672f6b2324a6", null ],
      [ "kSDHC_Data7LineLevelFlag", "group__sdhc.html#ggaf4373b0d2652db7719ff9bc45885573dadd0aa13c45bf2d8bbf9ad36088f8058e", null ]
    ] ],
    [ "_sdhc_interrupt_status_flag", "group__sdhc.html#gac048f5958eda32adea2e89a1284b27f2", [
      [ "kSDHC_CommandCompleteFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a4cf8be4bf25929992613d9c0b1bc0655", null ],
      [ "kSDHC_DataCompleteFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2aa98ce11a98374c77ab145189fa66489f", null ],
      [ "kSDHC_BlockGapEventFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a1441e0bb77e35d3cb130622abcbccb45", null ],
      [ "kSDHC_DmaCompleteFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a299c300028f7501bd71e565d231f2baa", null ],
      [ "kSDHC_BufferWriteReadyFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2afdd2addce0d0cb6d72894dac504443d7", null ],
      [ "kSDHC_BufferReadReadyFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a175711fd18dcb8a824d0db9c291c82a4", null ],
      [ "kSDHC_CardInsertionFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a64257e483a2795eeeb3d731c82ae24b3", null ],
      [ "kSDHC_CardRemovalFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a29fcab4ba7b3acbc1799dd5830e51fa1", null ],
      [ "kSDHC_CardInterruptFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a4cafddc7a2f41baf55d3a518d2f954de", null ],
      [ "kSDHC_CommandTimeoutFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a693edaa986062b236fb39c9eee313aa9", null ],
      [ "kSDHC_CommandCrcErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2aba090899e6315fea914bcaab013e2a02", null ],
      [ "kSDHC_CommandEndBitErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a85196ffcb4f92cd577766d4ebc70b28d", null ],
      [ "kSDHC_CommandIndexErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a4c92c47dd0c337f35826d64526fb40f3", null ],
      [ "kSDHC_DataTimeoutFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a3e5530f7002251bf4be8f36215e512d1", null ],
      [ "kSDHC_DataCrcErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a56674bd7081b9c27d14c51f4be7d1cdb", null ],
      [ "kSDHC_DataEndBitErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a2519436ead9f90e11aab318b6b67b65d", null ],
      [ "kSDHC_AutoCommand12ErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a9302a0d52276357b87a13cde98ab1dad", null ],
      [ "kSDHC_DmaErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2ae1ba4e16f02ba2e6c1bb73364214f5f3", null ],
      [ "kSDHC_CommandErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2aec155a494e41d6084ecf664645aca9be", null ],
      [ "kSDHC_DataErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2ae1a28ede35f2a454f70bac1b7949e5e0", null ],
      [ "kSDHC_ErrorFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a0401cdc0eba5131ba9688d3ce6182149", null ],
      [ "kSDHC_DataFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2af2613a73f2b9ede2e1d38cfa5301897b", null ],
      [ "kSDHC_CommandFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a5a88b255081a936f5c237635c6afcc0d", null ],
      [ "kSDHC_CardDetectFlag", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a61c4c4f1b06d9cc624ba4f223ef0b104", null ],
      [ "kSDHC_AllInterruptFlags", "group__sdhc.html#ggac048f5958eda32adea2e89a1284b27f2a8b377638820fae8c331274282aec65a8", null ]
    ] ],
    [ "_sdhc_auto_command12_error_status_flag", "group__sdhc.html#ga3992e8891c0251305343dcadc73bc3a2", [
      [ "kSDHC_AutoCommand12NotExecutedFlag", "group__sdhc.html#gga3992e8891c0251305343dcadc73bc3a2a0f1e7ecb88a1ebdc878418122d238661", null ],
      [ "kSDHC_AutoCommand12TimeoutFlag", "group__sdhc.html#gga3992e8891c0251305343dcadc73bc3a2a4de53543c6c2d29c45518d8eb8a17dac", null ],
      [ "kSDHC_AutoCommand12EndBitErrorFlag", "group__sdhc.html#gga3992e8891c0251305343dcadc73bc3a2a3f3296f892559f158ce35ad24320344f", null ],
      [ "kSDHC_AutoCommand12CrcErrorFlag", "group__sdhc.html#gga3992e8891c0251305343dcadc73bc3a2a2631c7a13b274b0db44f4d71f2562473", null ],
      [ "kSDHC_AutoCommand12IndexErrorFlag", "group__sdhc.html#gga3992e8891c0251305343dcadc73bc3a2a745ef9b089c06a73dfac5fb370927cd0", null ],
      [ "kSDHC_AutoCommand12NotIssuedFlag", "group__sdhc.html#gga3992e8891c0251305343dcadc73bc3a2a2cb93c10edcc43e7c9c69015fbde554c", null ]
    ] ],
    [ "_sdhc_adma_error_status_flag", "group__sdhc.html#ga7ad05c95fc789f8bf6e9bec3615d9fb9", [
      [ "kSDHC_AdmaLenghMismatchFlag", "group__sdhc.html#gga7ad05c95fc789f8bf6e9bec3615d9fb9a0d8f5269fe0e72aa1223383bd845d4a0", null ],
      [ "kSDHC_AdmaDescriptorErrorFlag", "group__sdhc.html#gga7ad05c95fc789f8bf6e9bec3615d9fb9a2acec6d21b3eb507f34a47f2d1ade9e7", null ]
    ] ],
    [ "sdhc_adma_error_state_t", "group__sdhc.html#gabd6eafe43d4c2012c4fd05d1661da581", [
      [ "kSDHC_AdmaErrorStateStopDma", "group__sdhc.html#ggabd6eafe43d4c2012c4fd05d1661da581ac56aff828fc54f59b2061e16a448d581", null ],
      [ "kSDHC_AdmaErrorStateFetchDescriptor", "group__sdhc.html#ggabd6eafe43d4c2012c4fd05d1661da581a3a8da786d14b3d7209a53a9e69d3f6b6", null ],
      [ "kSDHC_AdmaErrorStateChangeAddress", "group__sdhc.html#ggabd6eafe43d4c2012c4fd05d1661da581a727f99dce62794b47d038d4e14451c01", null ],
      [ "kSDHC_AdmaErrorStateTransferData", "group__sdhc.html#ggabd6eafe43d4c2012c4fd05d1661da581a6d8d87cae93a2be7809784e555d14cf2", null ]
    ] ],
    [ "_sdhc_force_event", "group__sdhc.html#ga8265448f3ce3066606f3f0592d9f3ddf", [
      [ "kSDHC_ForceEventAutoCommand12NotExecuted", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfa62ac012787af78935c81a70c95a4ba13", null ],
      [ "kSDHC_ForceEventAutoCommand12Timeout", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfa60703242a97eca33df537dadefac81bb", null ],
      [ "kSDHC_ForceEventAutoCommand12CrcError", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfaf4eb23ef8e2c32f2a580c3c08983126b", null ],
      [ "kSDHC_ForceEventEndBitError", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfaa7268c2d24ee243f77a83b338b6b3395", null ],
      [ "kSDHC_ForceEventAutoCommand12IndexError", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfa146dec41162b3a6f0b63341a9817bd9d", null ],
      [ "kSDHC_ForceEventAutoCommand12NotIssued", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfa3de1a593f1e9bcc3f25b4e9b0e7f7411", null ],
      [ "kSDHC_ForceEventCommandTimeout", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfacab052a80d177c4d67155b220a7c1cfe", null ],
      [ "kSDHC_ForceEventCommandCrcError", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfab527d9d2aed483b784e8359739bbf550", null ],
      [ "kSDHC_ForceEventCommandEndBitError", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfa8ee4105d7bf37a0e77c5c1079adcc44c", null ],
      [ "kSDHC_ForceEventCommandIndexError", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfa69e043408bfb1c8e41bfae21d3785b87", null ],
      [ "kSDHC_ForceEventDataTimeout", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfafa8190d00d9801e5f22da6afef20113f", null ],
      [ "kSDHC_ForceEventDataCrcError", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfad613829a2d696a470c96246cde110e19", null ],
      [ "kSDHC_ForceEventDataEndBitError", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfacc78b9fc5b0143e4462152ad5f9525f9", null ],
      [ "kSDHC_ForceEventAutoCommand12Error", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfafcb36fbb26f6ea934f88908b319625f4", null ],
      [ "kSDHC_ForceEventCardInt", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfa08c9344cf8af7ff7a7291c5d6522ab7f", null ],
      [ "kSDHC_ForceEventDmaError", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfa84d3a105cb22eac803e5567f3c1d673c", null ],
      [ "kSDHC_ForceEventsAll", "group__sdhc.html#gga8265448f3ce3066606f3f0592d9f3ddfab9fa422e63891a58b43e74d313f1d8c8", null ]
    ] ],
    [ "sdhc_data_bus_width_t", "group__sdhc.html#ga3e20e00d0b04a86cd06bfca44af4c2fe", [
      [ "kSDHC_DataBusWidth1Bit", "group__sdhc.html#gga3e20e00d0b04a86cd06bfca44af4c2fea4ea5e6f2d002a82b365933df1f6d7f6f", null ],
      [ "kSDHC_DataBusWidth4Bit", "group__sdhc.html#gga3e20e00d0b04a86cd06bfca44af4c2fea6f4a9a7790ef33855714be3f91e67228", null ],
      [ "kSDHC_DataBusWidth8Bit", "group__sdhc.html#gga3e20e00d0b04a86cd06bfca44af4c2fea814da04ce051dd9f7d37877e8e15d17b", null ]
    ] ],
    [ "sdhc_endian_mode_t", "group__sdhc.html#ga78985a7b3bc5eca1121ea6fce1f59b0f", [
      [ "kSDHC_EndianModeBig", "group__sdhc.html#gga78985a7b3bc5eca1121ea6fce1f59b0fa407172ca7560c61a1b8ef3274d435ac9", null ],
      [ "kSDHC_EndianModeHalfWordBig", "group__sdhc.html#gga78985a7b3bc5eca1121ea6fce1f59b0fa80941eddae38a3e5caa0d88957f5c871", null ],
      [ "kSDHC_EndianModeLittle", "group__sdhc.html#gga78985a7b3bc5eca1121ea6fce1f59b0fa5817878c6e534fa00be888e06aa693de", null ]
    ] ],
    [ "sdhc_dma_mode_t", "group__sdhc.html#ga711fe184b707121dcd0b81eb68463020", [
      [ "kSDHC_DmaModeNo", "group__sdhc.html#gga711fe184b707121dcd0b81eb68463020a5350a9c2598b44d767b4eb6005a38da3", null ],
      [ "kSDHC_DmaModeAdma1", "group__sdhc.html#gga711fe184b707121dcd0b81eb68463020a687cc69533e6f6c314ea1520f1806009", null ],
      [ "kSDHC_DmaModeAdma2", "group__sdhc.html#gga711fe184b707121dcd0b81eb68463020a8bb80959984dc6c7196dfa813dcac59c", null ]
    ] ],
    [ "_sdhc_sdio_control_flag", "group__sdhc.html#ga5ba89087dfd9c3e955ff6c40aafa1899", [
      [ "kSDHC_StopAtBlockGapFlag", "group__sdhc.html#gga5ba89087dfd9c3e955ff6c40aafa1899aa3cbdf09e1c88e9e35f572ec8ebb69bd", null ],
      [ "kSDHC_ReadWaitControlFlag", "group__sdhc.html#gga5ba89087dfd9c3e955ff6c40aafa1899afb4a6e80f8bd5a40dc6d126bad8289b0", null ],
      [ "kSDHC_InterruptAtBlockGapFlag", "group__sdhc.html#gga5ba89087dfd9c3e955ff6c40aafa1899a86c8753125b8662d237f98d1c23fa6a1", null ],
      [ "kSDHC_ExactBlockNumberReadFlag", "group__sdhc.html#gga5ba89087dfd9c3e955ff6c40aafa1899a25b576d5a0d05a3d28580208b88a1d95", null ]
    ] ],
    [ "sdhc_boot_mode_t", "group__sdhc.html#ga4bc551b837b19fbdad45f03ac83d1234", [
      [ "kSDHC_BootModeNormal", "group__sdhc.html#gga4bc551b837b19fbdad45f03ac83d1234a74f2e0615059a2a1cfb7f20356de8211", null ],
      [ "kSDHC_BootModeAlternative", "group__sdhc.html#gga4bc551b837b19fbdad45f03ac83d1234acabcca2f2455ce9f055d001f3f2b3e5f", null ]
    ] ],
    [ "sdhc_command_type_t", "group__sdhc.html#ga85d6c39338779861884196e102bdef28", [
      [ "kSDHC_CommandTypeNormal", "group__sdhc.html#gga85d6c39338779861884196e102bdef28a5ba7787facbdb98d251eb9b56e2ed475", null ],
      [ "kSDHC_CommandTypeSuspend", "group__sdhc.html#gga85d6c39338779861884196e102bdef28a2cef586313989d40e6cded7e01bf7e9a", null ],
      [ "kSDHC_CommandTypeResume", "group__sdhc.html#gga85d6c39338779861884196e102bdef28ab61246994d8650c3be4d02df75365849", null ],
      [ "kSDHC_CommandTypeAbort", "group__sdhc.html#gga85d6c39338779861884196e102bdef28aae3fd8206408358bc11107ed9743743c", null ]
    ] ],
    [ "sdhc_response_type_t", "group__sdhc.html#gaf9799517c5368332dda9faca79d807b2", [
      [ "kSDHC_ResponseTypeNone", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2ab7142760f2836546a08ca99f7467cc0e", null ],
      [ "kSDHC_ResponseTypeR1", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2a01576c00e4ac52c0b81004a41e51262f", null ],
      [ "kSDHC_ResponseTypeR1b", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2a55107b6a99908292ce8af1c20265fd3b", null ],
      [ "kSDHC_ResponseTypeR2", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2ac4fed27bec7be82c6e61a498201bb1e3", null ],
      [ "kSDHC_ResponseTypeR3", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2a9c7c7e82ab8efb29ba252b285f38d034", null ],
      [ "kSDHC_ResponseTypeR4", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2a2fffd0ed65101b411628aa215e80b65e", null ],
      [ "kSDHC_ResponseTypeR5", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2a6286b979a0ccaae996aecc50ac67dbda", null ],
      [ "kSDHC_ResponseTypeR5b", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2a70e37cbb7590ba7f86a38704d17f4f31", null ],
      [ "kSDHC_ResponseTypeR6", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2abad4406022f2e8bd5fc7b191f4c502bb", null ],
      [ "kSDHC_ResponseTypeR7", "group__sdhc.html#ggaf9799517c5368332dda9faca79d807b2ae660aca5c28623e7af91ac24dfec0e19", null ]
    ] ],
    [ "_sdhc_adma1_descriptor_flag", "group__sdhc.html#ga85bfce57d7d4e01e8c794fd8fada47ce", [
      [ "kSDHC_Adma1DescriptorValidFlag", "group__sdhc.html#gga85bfce57d7d4e01e8c794fd8fada47cea61362c0b10d13432b0cb9259ff2ccaf3", null ],
      [ "kSDHC_Adma1DescriptorEndFlag", "group__sdhc.html#gga85bfce57d7d4e01e8c794fd8fada47cea7941dd86f1969b0c877319f3d0d455cb", null ],
      [ "kSDHC_Adma1DescriptorInterrupFlag", "group__sdhc.html#gga85bfce57d7d4e01e8c794fd8fada47cea7cf6da4105962b88e037c97858e49b1f", null ],
      [ "kSDHC_Adma1DescriptorActivity1Flag", "group__sdhc.html#gga85bfce57d7d4e01e8c794fd8fada47cea2a324cc25930d8d6677e4b09bc1c9f5d", null ],
      [ "kSDHC_Adma1DescriptorActivity2Flag", "group__sdhc.html#gga85bfce57d7d4e01e8c794fd8fada47cea6c85f3492685aa41ff8be404d9e6e829", null ],
      [ "kSDHC_Adma1DescriptorTypeNop", "group__sdhc.html#gga85bfce57d7d4e01e8c794fd8fada47cea3127fc440f074276fe103eb7f491daba", null ],
      [ "kSDHC_Adma1DescriptorTypeTransfer", "group__sdhc.html#gga85bfce57d7d4e01e8c794fd8fada47cea2c9981eb2aa0619561a5a51b67539a3f", null ],
      [ "kSDHC_Adma1DescriptorTypeLink", "group__sdhc.html#gga85bfce57d7d4e01e8c794fd8fada47cea6e69b71e26c244d708181ee1333e2b82", null ],
      [ "kSDHC_Adma1DescriptorTypeSetLength", "group__sdhc.html#gga85bfce57d7d4e01e8c794fd8fada47ceaf218fe0ba0376f6a4dd447a0f2b535b4", null ]
    ] ],
    [ "_sdhc_adma2_descriptor_flag", "group__sdhc.html#ga86e6a459c96181f2370fa1d56fc49057", [
      [ "kSDHC_Adma2DescriptorValidFlag", "group__sdhc.html#gga86e6a459c96181f2370fa1d56fc49057adac034e4e2e9901c255866f09c4f3246", null ],
      [ "kSDHC_Adma2DescriptorEndFlag", "group__sdhc.html#gga86e6a459c96181f2370fa1d56fc49057a27cb5e7e289ecf3b20f4e2d6dd159f27", null ],
      [ "kSDHC_Adma2DescriptorInterruptFlag", "group__sdhc.html#gga86e6a459c96181f2370fa1d56fc49057ada3e36c681097def69281fae84555c88", null ],
      [ "kSDHC_Adma2DescriptorActivity1Flag", "group__sdhc.html#gga86e6a459c96181f2370fa1d56fc49057a2e2b3dd028899480d9b87543cbe8eae8", null ],
      [ "kSDHC_Adma2DescriptorActivity2Flag", "group__sdhc.html#gga86e6a459c96181f2370fa1d56fc49057a4d40a0471104c634353703f1e303659b", null ],
      [ "kSDHC_Adma2DescriptorTypeNop", "group__sdhc.html#gga86e6a459c96181f2370fa1d56fc49057a8355ac5e2d334e344e21ed38a9c02411", null ],
      [ "kSDHC_Adma2DescriptorTypeReserved", "group__sdhc.html#gga86e6a459c96181f2370fa1d56fc49057ae840ff693d62947e60f4797b67905987", null ],
      [ "kSDHC_Adma2DescriptorTypeTransfer", "group__sdhc.html#gga86e6a459c96181f2370fa1d56fc49057a82281598858f885a1760cef45b6c0c5e", null ],
      [ "kSDHC_Adma2DescriptorTypeLink", "group__sdhc.html#gga86e6a459c96181f2370fa1d56fc49057af41421a1ce8dca0816f5fe86717fc0dc", null ]
    ] ],
    [ "SDHC_Init", "group__sdhc.html#gab0396bcea6d22d90d245f3137016648b", null ],
    [ "SDHC_Deinit", "group__sdhc.html#ga236c0e8d36fd07a93333aadf290f7a75", null ],
    [ "SDHC_Reset", "group__sdhc.html#ga90614daed1a221b8b51dec1353ca5f7b", null ],
    [ "SDHC_SetAdmaTableConfig", "group__sdhc.html#gabf00123d0533512016709a55f65cb396", null ],
    [ "SDHC_EnableInterruptStatus", "group__sdhc.html#gae6b6b289b10ac3449883205c9e09df44", null ],
    [ "SDHC_DisableInterruptStatus", "group__sdhc.html#ga1bd6a1b0a4c07dfe036055aefafc148a", null ],
    [ "SDHC_EnableInterruptSignal", "group__sdhc.html#ga1b3d0e711544e2893d959e0490f64209", null ],
    [ "SDHC_DisableInterruptSignal", "group__sdhc.html#gad5fd8d703a036489297c65d50ba0e647", null ],
    [ "SDHC_GetInterruptStatusFlags", "group__sdhc.html#gaa76cb42c206c5236d5eeaae26c640861", null ],
    [ "SDHC_ClearInterruptStatusFlags", "group__sdhc.html#ga1b3e9936ed076d7de9e45108ef39d801", null ],
    [ "SDHC_GetAutoCommand12ErrorStatusFlags", "group__sdhc.html#gab8c8d1cfa7dc602c76b750dc3623b377", null ],
    [ "SDHC_GetAdmaErrorStatusFlags", "group__sdhc.html#ga12142b63dd21b4f411293a53456bdef7", null ],
    [ "SDHC_GetPresentStatusFlags", "group__sdhc.html#ga28717baedca99bc6ad1c2f170af44c0a", null ],
    [ "SDHC_GetCapability", "group__sdhc.html#ga746ecfb0aac2dde9e8b553d9303612e0", null ],
    [ "SDHC_EnableSdClock", "group__sdhc.html#ga796c9398ef2a506955e001baf797ba68", null ],
    [ "SDHC_SetSdClock", "group__sdhc.html#gac40a02df7172e1b815a474ebd48638fc", null ],
    [ "SDHC_SetCardActive", "group__sdhc.html#ga58280f8b442e0efa32a3b8641878de82", null ],
    [ "SDHC_SetDataBusWidth", "group__sdhc.html#gaddc8a6897bb66367cae60a3e8111c728", null ],
    [ "SDHC_SetTransferConfig", "group__sdhc.html#gaccce2a905bb391ccb52ba86784b8fb88", null ],
    [ "SDHC_GetCommandResponse", "group__sdhc.html#ga9d558885d3a5efac30d1d2acb1a5cee1", null ],
    [ "SDHC_WriteData", "group__sdhc.html#gaddb56ec84fb1c9dd76e8a977367149fe", null ],
    [ "SDHC_ReadData", "group__sdhc.html#gab21b337f5231623a71634e08e615b58b", null ],
    [ "SDHC_EnableWakeupEvent", "group__sdhc.html#gac2a854019063f4d87e46e1080c69ca73", null ],
    [ "SDHC_EnableCardDetectTest", "group__sdhc.html#ga138fa1b67e04507ec32e6f088b306799", null ],
    [ "SDHC_SetCardDetectTestLevel", "group__sdhc.html#gace8391834f409f1782321d52e7108374", null ],
    [ "SDHC_EnableSdioControl", "group__sdhc.html#gaba1eb2d21f74317dfca356531bcf31b2", null ],
    [ "SDHC_SetContinueRequest", "group__sdhc.html#gaf0f126c2d04d60642c960168eb2fb1a6", null ],
    [ "SDHC_SetMmcBootConfig", "group__sdhc.html#ga1b89b31459369ebd5bdd299e02dbe96c", null ],
    [ "SDHC_SetForceEvent", "group__sdhc.html#gae73cea2a9f2f9c70af5c58443d105c77", null ],
    [ "SDHC_TransferBlocking", "group__sdhc.html#ga2b12d7bc4c2ca1ea159a08b79c94c89f", null ],
    [ "SDHC_TransferCreateHandle", "group__sdhc.html#ga90499fd6eb98f9b2a3174b8a2780e89f", null ],
    [ "SDHC_TransferNonBlocking", "group__sdhc.html#ga890957169d20f7ae039ecd3b53ae0fae", null ],
    [ "SDHC_TransferHandleIRQ", "group__sdhc.html#ga7e141ce05ff822d2d3023db8a53e1057", null ]
];