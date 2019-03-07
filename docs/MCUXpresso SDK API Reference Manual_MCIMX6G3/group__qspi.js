var group__qspi =
[
    [ "QSPI eDMA Driver", "group__qspi__edma.html", "group__qspi__edma" ],
    [ "qspi_dqs_config_t", "group__qspi.html#structqspi__dqs__config__t", [
      [ "portADelayTapNum", "group__qspi.html#ab166711d9c9ea0accacffbd9463ba91a", null ],
      [ "portBDelayTapNum", "group__qspi.html#a29bc6e7eb4849cc0a1f813891e08a9cc", null ],
      [ "shift", "group__qspi.html#a41a08deef01859490a87688da1a2f166", null ],
      [ "enableDQSClkInverse", "group__qspi.html#a501b65137e9c19834a56df7812708328", null ],
      [ "enableDQSPadLoopback", "group__qspi.html#a3d373d727aeb713b1475bc32ede02f7e", null ],
      [ "enableDQSLoopback", "group__qspi.html#a22a7cacc9e4f07ea4408694387b541a5", null ]
    ] ],
    [ "qspi_flash_timing_t", "group__qspi.html#structqspi__flash__timing__t", [
      [ "dataHoldTime", "group__qspi.html#a1c4812527eaebb35bab982a6635b4106", null ],
      [ "CSHoldTime", "group__qspi.html#a46b400bc95b18888574e5d20ae4c2c1e", null ],
      [ "CSSetupTime", "group__qspi.html#a94b7c0ea1ba1f3dbf06b11062d0e8e85", null ]
    ] ],
    [ "qspi_config_t", "group__qspi.html#structqspi__config__t", [
      [ "clockSource", "group__qspi.html#a4636154d339f8a457267a51d17c6028f", null ],
      [ "baudRate", "group__qspi.html#ab90e8b90f6ac0da5ce3102ec2104e6f7", null ],
      [ "txWatermark", "group__qspi.html#aacef56c9b3a210643d8d3de452dbf1b5", null ],
      [ "rxWatermark", "group__qspi.html#aab860bcdf2dd6e4cfc708484330334c1", null ],
      [ "AHBbufferSize", "group__qspi.html#ac4a1f15dc587c01444fc506b32d82b1f", null ],
      [ "AHBbufferMaster", "group__qspi.html#a0b223821f4f9a57e61244b66da926113", null ],
      [ "enableAHBbuffer3AllMaster", "group__qspi.html#a8d0e52b14b97ed123ff72a7878e4bb67", null ],
      [ "area", "group__qspi.html#a29b0ae614176264f9d58d2152e80cd70", null ],
      [ "enableQspi", "group__qspi.html#ad280dc96ddc5397ce1cde0d8b1104098", null ]
    ] ],
    [ "qspi_flash_config_t", "group__qspi.html#structqspi__flash__config__t", [
      [ "flashA1Size", "group__qspi.html#adb2a4c29ddc58b85de0d82f75b0f045b", null ],
      [ "flashA2Size", "group__qspi.html#acace5b5a54558a7c1cbe9e3ab4948a82", null ],
      [ "flashB1Size", "group__qspi.html#a415ea1611adead970da3abd8de2d3f40", null ],
      [ "flashB2Size", "group__qspi.html#a951fddb2a65c4d9ea47559706d3467cb", null ],
      [ "lookuptable", "group__qspi.html#a6a0feb5d24a33026c8f539004ba60ce3", null ],
      [ "CSHoldTime", "group__qspi.html#a362347a0f5e9899540f58e9d2826c868", null ],
      [ "CSSetupTime", "group__qspi.html#a411afc45b9fabb624cad4bb0cd2e16f5", null ],
      [ "cloumnspace", "group__qspi.html#a6b77c14127052ae12253deec016c9dbc", null ],
      [ "dataLearnValue", "group__qspi.html#a0722287c779f056eda14e0e1be68de04", null ],
      [ "endian", "group__qspi.html#a83ac59682812f8f55a5be71c2ff9e579", null ],
      [ "enableWordAddress", "group__qspi.html#add68d248634adee8b831a50bada4c37d", null ]
    ] ],
    [ "qspi_transfer_t", "group__qspi.html#structqspi__transfer__t", [
      [ "data", "group__qspi.html#aa543a15bba26a35f1ea6bc8a1d6bac3f", null ],
      [ "dataSize", "group__qspi.html#a0570176d8c8dac4e5a64f52505d07138", null ]
    ] ],
    [ "FSL_QSPI_DRIVER_VERSION", "group__qspi.html#gab20dd936fe00a3df5289d4f66e5d8e69", null ],
    [ "QSPI_LUT_SEQ", "group__qspi.html#ga6bfca51c0942f0910fc73019f35a9b90", null ],
    [ "QSPI_CMD", "group__qspi.html#ga25a69200802c0573901a61e4f942c2fb", null ],
    [ "QSPI_PAD_1", "group__qspi.html#gae30d16840c705584ef8ee3a63fc2ecbc", null ],
    [ "_status_t", "group__qspi.html#ga9006d37909032f8ad238cb8f953115a4", [
      [ "kStatus_QSPI_Idle", "group__qspi.html#gga9006d37909032f8ad238cb8f953115a4a650abd7e718c3d31a1b1c31628223255", null ],
      [ "kStatus_QSPI_Busy", "group__qspi.html#gga9006d37909032f8ad238cb8f953115a4adc835d439498f87c2f72a4b0681d64a9", null ],
      [ "kStatus_QSPI_Error", "group__qspi.html#gga9006d37909032f8ad238cb8f953115a4af658051f373360010b0c962e513c18f7", null ]
    ] ],
    [ "qspi_read_area_t", "group__qspi.html#ga0381ee0bc3439d26a1189e292e44e6b6", [
      [ "kQSPI_ReadAHB", "group__qspi.html#gga0381ee0bc3439d26a1189e292e44e6b6ae47d046fafe2f36a71f54ee103df7a2c", null ],
      [ "kQSPI_ReadIP", "group__qspi.html#gga0381ee0bc3439d26a1189e292e44e6b6a84c74f3313d795034c7af6be1a933d57", null ]
    ] ],
    [ "qspi_command_seq_t", "group__qspi.html#gaa2acaac73027bbcb126d98e0ea4373de", [
      [ "kQSPI_IPSeq", "group__qspi.html#ggaa2acaac73027bbcb126d98e0ea4373dea08eb5e396b6f69e58343b4d342075c84", null ],
      [ "kQSPI_BufferSeq", "group__qspi.html#ggaa2acaac73027bbcb126d98e0ea4373deaaada5445077b4e683eb30513686bbd79", null ]
    ] ],
    [ "qspi_fifo_t", "group__qspi.html#ga8832e709c9e9dfda7d14e55813019bd6", [
      [ "kQSPI_TxFifo", "group__qspi.html#gga8832e709c9e9dfda7d14e55813019bd6a871de46c2cffb988e01e2ad489022fd1", null ],
      [ "kQSPI_RxFifo", "group__qspi.html#gga8832e709c9e9dfda7d14e55813019bd6a1b9d0be39e4238aae341742d0569615e", null ],
      [ "kQSPI_AllFifo", "group__qspi.html#gga8832e709c9e9dfda7d14e55813019bd6af764de596574813acb3cc6e17c3427fa", null ]
    ] ],
    [ "qspi_endianness_t", "group__qspi.html#gad5667033853bf78f23328a479a17eaa2", [
      [ "kQSPI_64BigEndian", "group__qspi.html#ggad5667033853bf78f23328a479a17eaa2a3fe7fc21c848ff6f539bda069633d86d", null ],
      [ "kQSPI_32LittleEndian", "group__qspi.html#ggad5667033853bf78f23328a479a17eaa2ae0be29f1fded1d7138d7be191b7015bd", null ],
      [ "kQSPI_32BigEndian", "group__qspi.html#ggad5667033853bf78f23328a479a17eaa2a6c1315ef947f64f0e49b8386c2509dab", null ],
      [ "kQSPI_64LittleEndian", "group__qspi.html#ggad5667033853bf78f23328a479a17eaa2a6c29c130d5df807189c316219c19e9b9", null ]
    ] ],
    [ "_qspi_error_flags", "group__qspi.html#gaa2e47ba48984567f2e839abd7532ab75", [
      [ "kQSPI_DataLearningFail", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75a82ebf1886216b4ecec09af6a6cc914af", null ],
      [ "kQSPI_TxBufferFill", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75ae78da606993213030e1cd2855f6c685d", null ],
      [ "kQSPI_TxBufferUnderrun", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75a66f4f039dbb8ac448d7f46209f36db1f", null ],
      [ "kQSPI_IllegalInstruction", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75ae210fba7b33e0a0fc903bad01959f9bb", null ],
      [ "kQSPI_RxBufferOverflow", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75a9455e4db5cbed29a39c27176d5cb3b80", null ],
      [ "kQSPI_RxBufferDrain", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75ae66e640f89aa7cf7b7d46549b3aba452", null ],
      [ "kQSPI_AHBSequenceError", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75a584a94d7b5b3f45f169352ea9a269288", null ],
      [ "kQSPI_AHBBufferOverflow", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75a0bd70649a9ff6ef4e16f87d612150697", null ],
      [ "kQSPI_IPCommandUsageError", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75a45b4bc65ee7ca727055555ba80a7fcb7", null ],
      [ "kQSPI_IPCommandTriggerDuringAHBAccess", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75ab0a8e3e663a9b17682351f85bfa45660", null ],
      [ "kQSPI_IPCommandTriggerDuringIPAccess", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75ae6cdc3da0c34c3f6b1cd53310ae0bf00", null ],
      [ "kQSPI_IPCommandTriggerDuringAHBGrant", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75a58223f241bc3eaaf8b95c242c2ca86e2", null ],
      [ "kQSPI_IPCommandTransactionFinished", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75a9ea0badcfc031cb266bd46305b8c72c5", null ],
      [ "kQSPI_FlagAll", "group__qspi.html#ggaa2e47ba48984567f2e839abd7532ab75a5187e8e76e136e46bc36dcf64d6aa505", null ]
    ] ],
    [ "_qspi_flags", "group__qspi.html#gac80201db90818bf11e2077721c03191d", [
      [ "kQSPI_DataLearningSamplePoint", "group__qspi.html#ggac80201db90818bf11e2077721c03191dab31d2931287b150f85fd6e946b680f85", null ],
      [ "kQSPI_TxBufferFull", "group__qspi.html#ggac80201db90818bf11e2077721c03191dad338c737040d982e80da1067f84ec7b5", null ],
      [ "kQSPI_TxBufferEnoughData", "group__qspi.html#ggac80201db90818bf11e2077721c03191dadafdf55a7ab0b550c9ea4769f091bb89", null ],
      [ "kQSPI_RxDMA", "group__qspi.html#ggac80201db90818bf11e2077721c03191da66b82b71371dc29d8d14d79c6b89e05c", null ],
      [ "kQSPI_RxBufferFull", "group__qspi.html#ggac80201db90818bf11e2077721c03191da299468e0e4cdc5bb9efce3b9258306ae", null ],
      [ "kQSPI_RxWatermark", "group__qspi.html#ggac80201db90818bf11e2077721c03191dab17290cbdc5fc52fff138beb59d5e5bf", null ],
      [ "kQSPI_AHB3BufferFull", "group__qspi.html#ggac80201db90818bf11e2077721c03191da5e9830dd3846a935b7c4bfa83cda5cde", null ],
      [ "kQSPI_AHB2BufferFull", "group__qspi.html#ggac80201db90818bf11e2077721c03191da2bc889249e7bd0f9526fedfc10be0a78", null ],
      [ "kQSPI_AHB1BufferFull", "group__qspi.html#ggac80201db90818bf11e2077721c03191da9688c15b08c02d4067b3c6ce2bbc4955", null ],
      [ "kQSPI_AHB0BufferFull", "group__qspi.html#ggac80201db90818bf11e2077721c03191da42bcfcb7dfe1c5cec3efa5d26b3372ba", null ],
      [ "kQSPI_AHB3BufferNotEmpty", "group__qspi.html#ggac80201db90818bf11e2077721c03191da556d615a880a16a4b09c51cb9c0fbce2", null ],
      [ "kQSPI_AHB2BufferNotEmpty", "group__qspi.html#ggac80201db90818bf11e2077721c03191da11643501f7d3dced6945810c8bb7901f", null ],
      [ "kQSPI_AHB1BufferNotEmpty", "group__qspi.html#ggac80201db90818bf11e2077721c03191dad3c527aaf9d4040a2e89ef56e8df9445", null ],
      [ "kQSPI_AHB0BufferNotEmpty", "group__qspi.html#ggac80201db90818bf11e2077721c03191daafea8c3058c75dc4b9c866d38200eaae", null ],
      [ "kQSPI_AHBTransactionPending", "group__qspi.html#ggac80201db90818bf11e2077721c03191da4236214258a3e6be0ef645d3a4d8d39d", null ],
      [ "kQSPI_AHBCommandPriorityGranted", "group__qspi.html#ggac80201db90818bf11e2077721c03191da4007b205447c1bf7921f5c86a14fa16c", null ],
      [ "kQSPI_AHBAccess", "group__qspi.html#ggac80201db90818bf11e2077721c03191da847d2a719002e35165cb0aa8130fd75f", null ],
      [ "kQSPI_IPAccess", "group__qspi.html#ggac80201db90818bf11e2077721c03191daa0f11d2b4e556c57193c5aae011ae80a", null ],
      [ "kQSPI_Busy", "group__qspi.html#ggac80201db90818bf11e2077721c03191da87c1c4af9ab8541dc78c6a4198939819", null ],
      [ "kQSPI_StateAll", "group__qspi.html#ggac80201db90818bf11e2077721c03191dadd5aaac1cedbb2f8e7d23b354a7897a8", null ]
    ] ],
    [ "_qspi_interrupt_enable", "group__qspi.html#ga80a4c90f4dbe2c7bd06c9ec824e71620", [
      [ "kQSPI_DataLearningFailInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620a4c7859018c3ba6a32464dad38ceeb98a", null ],
      [ "kQSPI_TxBufferFillInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620ab64ec7c3212490cc40063097020338cf", null ],
      [ "kQSPI_TxBufferUnderrunInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620ab8c32ad372c38910a2a81f3c4ac1a2d0", null ],
      [ "kQSPI_IllegalInstructionInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620a3a782247c667cd8ffbe58b9242d3c9b8", null ],
      [ "kQSPI_RxBufferOverflowInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620a5f4318bf4c3620010b64e1825dea1d07", null ],
      [ "kQSPI_RxBufferDrainInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620a7a6f2d3b6da206d8b3bd0134381886ed", null ],
      [ "kQSPI_AHBSequenceErrorInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620a2f34733c442c1910b52120adfecf2aa0", null ],
      [ "kQSPI_AHBBufferOverflowInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620aecd6ab9aae98a046cd4b4e4fbfb105f0", null ],
      [ "kQSPI_IPCommandUsageErrorInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620acab70e76c9cbbe53862a0d1f6af712ad", null ],
      [ "kQSPI_IPCommandTriggerDuringAHBAccessInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620ab6b2676806bbd1d0e5fff969dc1a6232", null ],
      [ "kQSPI_IPCommandTriggerDuringIPAccessInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620ade1d0eba9ec9616b7f4c4f062d6f8e05", null ],
      [ "kQSPI_IPCommandTriggerDuringAHBGrantInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620abc06806afeb1080ce4cbdf034db18c7c", null ],
      [ "kQSPI_IPCommandTransactionFinishedInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620a869691c42260fdcc22722697a852b2a9", null ],
      [ "kQSPI_AllInterruptEnable", "group__qspi.html#gga80a4c90f4dbe2c7bd06c9ec824e71620abcdb2dfb68fd5384703ec696dea81468", null ]
    ] ],
    [ "_qspi_dma_enable", "group__qspi.html#ga91267f891f702b8e11cd604f0d153933", [
      [ "kQSPI_RxBufferDrainDMAEnable", "group__qspi.html#gga91267f891f702b8e11cd604f0d153933acd40d4e277c00c44a2baeaef30c314ff", null ]
    ] ],
    [ "qspi_dqs_phrase_shift_t", "group__qspi.html#ga4c5e6599ed955646d4d62a2adee8d002", [
      [ "kQSPI_DQSNoPhraseShift", "group__qspi.html#gga4c5e6599ed955646d4d62a2adee8d002a94dd1a3609d8de61cdd20d05dc80f251", null ],
      [ "kQSPI_DQSPhraseShift45Degree", "group__qspi.html#gga4c5e6599ed955646d4d62a2adee8d002a3972d55777dc0e768c835db39c0066a8", null ],
      [ "kQSPI_DQSPhraseShift90Degree", "group__qspi.html#gga4c5e6599ed955646d4d62a2adee8d002a66c02aabf568579a8c896c10c489d325", null ],
      [ "kQSPI_DQSPhraseShift135Degree", "group__qspi.html#gga4c5e6599ed955646d4d62a2adee8d002addf9b4efca420cfeca813ffeb2354d55", null ]
    ] ],
    [ "QSPI_Init", "group__qspi.html#ga0e7472a25e28d1c09bcd4051b9b6818c", null ],
    [ "QSPI_GetDefaultQspiConfig", "group__qspi.html#ga38adbb3ea74dce5e9e9baf5adaa5776c", null ],
    [ "QSPI_Deinit", "group__qspi.html#ga1378896ddde1800fb94f6afd5aad5ca2", null ],
    [ "QSPI_SetFlashConfig", "group__qspi.html#ga1a00d1764ecc36ab4ce7c817af37b628", null ],
    [ "QSPI_SoftwareReset", "group__qspi.html#gad00bf85bbe7ef011949b8b0828512ec5", null ],
    [ "QSPI_Enable", "group__qspi.html#gafc5a4faf1e4b331e3597f9935fda7701", null ],
    [ "QSPI_GetStatusFlags", "group__qspi.html#gadb3593e738eefab160595896905a0346", null ],
    [ "QSPI_GetErrorStatusFlags", "group__qspi.html#ga4250bc764815cf145db54fb58beeb4b9", null ],
    [ "QSPI_ClearErrorFlag", "group__qspi.html#ga1fe1d4bfac62079c21216eb8ae273caf", null ],
    [ "QSPI_EnableInterrupts", "group__qspi.html#ga77cfe54f589439821856ee17d7fcb0db", null ],
    [ "QSPI_DisableInterrupts", "group__qspi.html#ga580d078ddd276d430cf3dc003e6a11e6", null ],
    [ "QSPI_EnableDMA", "group__qspi.html#gae3653d1e7795291a68a59136f80279c6", null ],
    [ "QSPI_GetTxDataRegisterAddress", "group__qspi.html#gaf7f749b3b7ce62871debfe7dc59aaf13", null ],
    [ "QSPI_GetRxDataRegisterAddress", "group__qspi.html#gac76b614d300554715b39728d4281e238", null ],
    [ "QSPI_SetIPCommandAddress", "group__qspi.html#gac607c65a92caa8900bb2d3b8f9f8ebd0", null ],
    [ "QSPI_SetIPCommandSize", "group__qspi.html#gae9e10faa05475788e90778b3d3c0e7dc", null ],
    [ "QSPI_ExecuteIPCommand", "group__qspi.html#ga07d1bfa5b9527409211f04f1e294f221", null ],
    [ "QSPI_ExecuteAHBCommand", "group__qspi.html#gae695194b3abbce3754cc77dae5dc2339", null ],
    [ "QSPI_EnableIPParallelMode", "group__qspi.html#ga2f10bbac3b5f21a244cacfaae176619e", null ],
    [ "QSPI_EnableAHBParallelMode", "group__qspi.html#gad09aca0f6e00fdaefcbbc02857dfae72", null ],
    [ "QSPI_UpdateLUT", "group__qspi.html#ga15246abf61c0ea63921a206eaccc9974", null ],
    [ "QSPI_ClearFifo", "group__qspi.html#ga8d2d3f5c2e46e3b7732b774583f2535b", null ],
    [ "QSPI_ClearCommandSequence", "group__qspi.html#gac694b6467a2afd4ef558745989235194", null ],
    [ "QSPI_EnableDDRMode", "group__qspi.html#ga09d5d2ca0c22fababcb480e6ad574b43", null ],
    [ "QSPI_SetReadDataArea", "group__qspi.html#ga24880c10846bbe843ec287091f318e88", null ],
    [ "QSPI_WriteBlocking", "group__qspi.html#gab2c26100daa3f9f95091e524d8bf5c7d", null ],
    [ "QSPI_WriteData", "group__qspi.html#gaf6225414bf13955c55471779308d531c", null ],
    [ "QSPI_ReadBlocking", "group__qspi.html#ga4aab7113f914858544075e60dc3b6c4f", null ],
    [ "QSPI_ReadData", "group__qspi.html#ga673d32dc8663771f44cdda19b091a5d3", null ],
    [ "QSPI_TransferSendBlocking", "group__qspi.html#gaba3ec60feea5813ae34fc7fb75f6d2db", null ],
    [ "QSPI_TransferReceiveBlocking", "group__qspi.html#ga2734478dd21ba237b925e1e86e147580", null ]
];