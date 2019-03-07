var group__sdma =
[
    [ "sdma_config_t", "group__sdma.html#structsdma__config__t", [
      [ "enableRealTimeDebugPin", "group__sdma.html#a82b3d4ea81f30cf4c687dc0fc2f1adbe", null ],
      [ "isSoftwareResetClearLock", "group__sdma.html#a69fe3d3e41b8c4f6b48fecc3d8e11a97", null ],
      [ "ratio", "group__sdma.html#a20bd6f6adea6bbdc51fcf0b1d4331ba5", null ]
    ] ],
    [ "sdma_transfer_config_t", "group__sdma.html#structsdma__transfer__config__t", [
      [ "srcAddr", "group__sdma.html#a7d8132828202b8416a29d552264832ba", null ],
      [ "destAddr", "group__sdma.html#a6713619d134c3a690657999b187fae06", null ],
      [ "srcTransferSize", "group__sdma.html#a4a1e7cb8f3826dc03a93a91bbbe8773c", null ],
      [ "destTransferSize", "group__sdma.html#a9543ad0711187a51431a1983eb6bf895", null ],
      [ "bytesPerRequest", "group__sdma.html#ad78baf0e67245cf315a5ec866b85a2ff", null ],
      [ "transferSzie", "group__sdma.html#af09cf5efdd555593c07667cef773cf84", null ],
      [ "scriptAddr", "group__sdma.html#a67c22ed8d9db61ea5e3985218a10eb6f", null ],
      [ "eventSource", "group__sdma.html#a5fbc699b4772d9768bb5ec93775ecbc3", null ],
      [ "isEventIgnore", "group__sdma.html#aa6866cf552528f984d1e462dec1bb757", null ],
      [ "isSoftTriggerIgnore", "group__sdma.html#a599ff90023ae46e9e8410d2b747d2dff", null ],
      [ "type", "group__sdma.html#a7f60c79fa59af382ecc2daf5719e321b", null ]
    ] ],
    [ "sdma_buffer_descriptor_t", "group__sdma.html#structsdma__buffer__descriptor__t", [
      [ "count", "group__sdma.html#a7e3dd458ec183d4ca8179b9ad649b63a", null ],
      [ "status", "group__sdma.html#a9ec79aafebe5c93f1d538ce2551140c7", null ],
      [ "command", "group__sdma.html#a439f34551c2e8d13150a8f60768beb49", null ],
      [ "bufferAddr", "group__sdma.html#a2ddf152ce09e41d6890dead8c15142ea", null ],
      [ "extendBufferAddr", "group__sdma.html#aa960c896fade6283619fde1236de022b", null ]
    ] ],
    [ "sdma_channel_control_t", "group__sdma.html#structsdma__channel__control__t", [
      [ "currentBDAddr", "group__sdma.html#a7784ee8d1e42b9f35fecbddfb2c26df1", null ],
      [ "baseBDAddr", "group__sdma.html#acb0cc52393d44bc2bd25dbfcf4c76e05", null ],
      [ "channelDesc", "group__sdma.html#a09442829bd9d69cc30df580dc711c8d2", null ],
      [ "status", "group__sdma.html#a7af38afcb4ea94a5863e23e840c94d83", null ]
    ] ],
    [ "sdma_context_data_t", "group__sdma.html#structsdma__context__data__t", [
      [ "GeneralReg", "group__sdma.html#afbbb4a2f495ee70dedcc11179f0661db", null ]
    ] ],
    [ "sdma_handle_t", "group__sdma.html#structsdma__handle__t", [
      [ "callback", "group__sdma.html#a34cac4d5f1fda259609f0b047fae5ca7", null ],
      [ "userData", "group__sdma.html#a12a0aa1a191f006c1525bd4e2824af84", null ],
      [ "base", "group__sdma.html#a67a8c4e55fa8d248bd6436e079cbd690", null ],
      [ "BDPool", "group__sdma.html#ac3c6b8dc20ccf5ab03d8b18b24546915", null ],
      [ "bdCount", "group__sdma.html#a4ad6118d5a7cf45bce519b5dd3840abb", null ],
      [ "bdIndex", "group__sdma.html#a6cee67f6f26c19ce663c3f97c50573fe", null ],
      [ "eventSource", "group__sdma.html#a13646f921022231e00b0d9b0a7ae9bc4", null ],
      [ "context", "group__sdma.html#a7977a140300a8c22f64d850abaca44a8", null ],
      [ "channel", "group__sdma.html#ac2fe0b319fae78b89146bb6b61070748", null ],
      [ "priority", "group__sdma.html#a21a108eb72b4fc30074ea4027b7ad1ed", null ],
      [ "flags", "group__sdma.html#ae62a9a9f2c7b9257e6e2f14858e7e66b", null ]
    ] ],
    [ "FSL_SDMA_DRIVER_VERSION", "group__sdma.html#ga534773808b3e715f238ce2af747854ac", null ],
    [ "sdma_callback", "group__sdma.html#gaf58c0232a81493c44346fac69770dca3", null ],
    [ "sdma_transfer_size_t", "group__sdma.html#ga758eb6c79509c31397dd944429b10c55", [
      [ "kSDMA_TransferSize1Bytes", "group__sdma.html#gga758eb6c79509c31397dd944429b10c55adca09658624dd0253049ab6fadcf5262", null ],
      [ "kSDMA_TransferSize2Bytes", "group__sdma.html#gga758eb6c79509c31397dd944429b10c55a9d64435da219daa115b25164980a55f4", null ],
      [ "kSDMA_TransferSize4Bytes", "group__sdma.html#gga758eb6c79509c31397dd944429b10c55a97fe0cdd51ff2bb23ebfa0fc1b880c8c", null ]
    ] ],
    [ "sdma_bd_status_t", "group__sdma.html#gac434de57d97e9088f3b10a80e67cdc2f", [
      [ "kSDMA_BDStatusDone", "group__sdma.html#ggac434de57d97e9088f3b10a80e67cdc2fa4fa99ceb99319e981c7dff3573bcf417", null ],
      [ "kSDMA_BDStatusWrap", "group__sdma.html#ggac434de57d97e9088f3b10a80e67cdc2fac74f221723174afa9d7e626cde478bbc", null ],
      [ "kSDMA_BDStatusContinuous", "group__sdma.html#ggac434de57d97e9088f3b10a80e67cdc2faf8337d05086d2274567e5b5ddd4a1f11", null ],
      [ "kSDMA_BDStatusInterrupt", "group__sdma.html#ggac434de57d97e9088f3b10a80e67cdc2fa37a4cd8d3de0e463b2ac74f27a062dec", null ],
      [ "kSDMA_BDStatusError", "group__sdma.html#ggac434de57d97e9088f3b10a80e67cdc2faeba409e65de0ec3bdb85ef9f0ef98f5c", null ],
      [ "kSDMA_BDStatusLast", "group__sdma.html#ggac434de57d97e9088f3b10a80e67cdc2fae57223126ec501af1a61bd08066b5c0c", null ],
      [ "kSDMA_BDStatusExtend", "group__sdma.html#ggac434de57d97e9088f3b10a80e67cdc2faddc99976cd12cb6b0e4f13dcc592d2fe", null ]
    ] ],
    [ "sdma_bd_command_t", "group__sdma.html#ga6a9a8f085496af46c316b05609ef3fe5", [
      [ "kSDMA_BDCommandSETDM", "group__sdma.html#gga6a9a8f085496af46c316b05609ef3fe5a9e967566f2c13fd2fc7cadcffaea5b5e", null ],
      [ "kSDMA_BDCommandGETDM", "group__sdma.html#gga6a9a8f085496af46c316b05609ef3fe5aed4e5459cc289ba817b315c4a3f92f2c", null ],
      [ "kSDMA_BDCommandSETPM", "group__sdma.html#gga6a9a8f085496af46c316b05609ef3fe5afc9f1ebf2a9ca952daca64ef464c5a91", null ],
      [ "kSDMA_BDCommandGETPM", "group__sdma.html#gga6a9a8f085496af46c316b05609ef3fe5a3abf6e67444d69fc41db7a9ce174f292", null ],
      [ "kSDMA_BDCommandSETCTX", "group__sdma.html#gga6a9a8f085496af46c316b05609ef3fe5a7ed1e6f0d071ee1d24e414f96a9a400b", null ],
      [ "kSDMA_BDCommandGETCTX", "group__sdma.html#gga6a9a8f085496af46c316b05609ef3fe5aeaf0dc44678150d1eef8ec146159b972", null ]
    ] ],
    [ "sdma_context_switch_mode_t", "group__sdma.html#ga53d935f40711fff7150f4bfdd6fe95a3", [
      [ "kSDMA_ContextSwitchModeStatic", "group__sdma.html#gga53d935f40711fff7150f4bfdd6fe95a3a071e68bfc896b3bfa4e56470e3cb49a6", null ],
      [ "kSDMA_ContextSwitchModeDynamicLowPower", "group__sdma.html#gga53d935f40711fff7150f4bfdd6fe95a3a5deab6254b6dbea0adf963515229fe02", null ],
      [ "kSDMA_ContextSwitchModeDynamicWithNoLoop", "group__sdma.html#gga53d935f40711fff7150f4bfdd6fe95a3a202019005d4b4b2ab178d9e592991283", null ],
      [ "kSDMA_ContextSwitchModeDynamic", "group__sdma.html#gga53d935f40711fff7150f4bfdd6fe95a3a8d64cfeb7162d3fe1246b3e0786e511f", null ]
    ] ],
    [ "sdma_clock_ratio_t", "group__sdma.html#ga0d831443598fc27a848b06640241c080", [
      [ "kSDMA_HalfARMClockFreq", "group__sdma.html#gga0d831443598fc27a848b06640241c080a1d092d31f5724c51bdc25a293b3a2124", null ],
      [ "kSDMA_ARMClockFreq", "group__sdma.html#gga0d831443598fc27a848b06640241c080a0c9654fb7c2ae88c35c97e8b3ab0f462", null ]
    ] ],
    [ "sdma_transfer_type_t", "group__sdma.html#gac1b10d21cd1b227dd289eb0fad744ac2", [
      [ "kSDMA_MemoryToMemory", "group__sdma.html#ggac1b10d21cd1b227dd289eb0fad744ac2a093e03007b762e340d747d5d3b58334f", null ],
      [ "kSDMA_PeripheralToMemory", "group__sdma.html#ggac1b10d21cd1b227dd289eb0fad744ac2a0e281ffebe82e273c2658e502a81ee7c", null ],
      [ "kSDMA_MemoryToPeripheral", "group__sdma.html#ggac1b10d21cd1b227dd289eb0fad744ac2aef1f65710eb5c0a5a5e572d809f31739", null ]
    ] ],
    [ "sdma_peripheral_t", "group__sdma.html#gadc1ce96deaaa5299e7370d6c162eb612", [
      [ "kSDMA_PeripheralTypeMemory", "group__sdma.html#ggadc1ce96deaaa5299e7370d6c162eb612ac6dc3f40b0bc146d1c896bec285e65d5", null ],
      [ "kSDMA_PeripheralTypeUART", "group__sdma.html#ggadc1ce96deaaa5299e7370d6c162eb612a0a5ce17f35ef0a09d338df54cf5e3826", null ],
      [ "kSDMA_PeripheralTypeUART_SP", "group__sdma.html#ggadc1ce96deaaa5299e7370d6c162eb612a4b35766548a6ee990df838a09d447d2c", null ],
      [ "kSDMA_PeripheralTypeSPDIF", "group__sdma.html#ggadc1ce96deaaa5299e7370d6c162eb612a28e7ad3aa4b0a5ec73f432371000b806", null ],
      [ "kSDMA_PeripheralNormal", "group__sdma.html#ggadc1ce96deaaa5299e7370d6c162eb612ad33f9568aa791c5d8abdaa0edd1e836f", null ],
      [ "kSDMA_PeripheralNormal_SP", "group__sdma.html#ggadc1ce96deaaa5299e7370d6c162eb612a746f066b6d3cc1220325428afcf4fd86", null ]
    ] ],
    [ "_sdma_transfer_status", "group__sdma.html#ga265c783e56e29d88556d053d6ed6712f", [
      [ "kStatus_SDMA_ERROR", "group__sdma.html#gga265c783e56e29d88556d053d6ed6712fa03cf037d71abc39d5d16d98e07e2e217", null ],
      [ "kStatus_SDMA_Busy", "group__sdma.html#gga265c783e56e29d88556d053d6ed6712fae7f535ed8ea581f421d5bce34c0444af", null ]
    ] ],
    [ "SDMA_Init", "group__sdma.html#ga63e5054d2b3e2ac6db0bc77a6e555f3c", null ],
    [ "SDMA_Deinit", "group__sdma.html#ga2ca9d8092fff698309ddd8413e1ea15d", null ],
    [ "SDMA_GetDefaultConfig", "group__sdma.html#ga928c1c71a8cea279eb941c8b239ece93", null ],
    [ "SDMA_ResetModule", "group__sdma.html#gaa7b59dc82bebfda229556c999b53f841", null ],
    [ "SDMA_EnableChannelErrorInterrupts", "group__sdma.html#ga7e5912cbe348e787140df4df6518d216", null ],
    [ "SDMA_DisableChannelErrorInterrupts", "group__sdma.html#gaf1f0be3c0a76e1a64b5f836e918a9704", null ],
    [ "SDMA_ConfigBufferDescriptor", "group__sdma.html#gaca6942f24e6de8ad946d97598a7e9fbe", null ],
    [ "SDMA_SetChannelPriority", "group__sdma.html#ga799b8c29ad2e816e2448ffabdd4a988e", null ],
    [ "SDMA_SetSourceChannel", "group__sdma.html#ga7d17f6656c63920e2d2a1970abe95ab7", null ],
    [ "SDMA_StartChannelSoftware", "group__sdma.html#ga79d0f9a600307e3382b37fedbaf94082", null ],
    [ "SDMA_StartChannelEvents", "group__sdma.html#gafae3a665aced95a8d5c61135f419c655", null ],
    [ "SDMA_StopChannel", "group__sdma.html#gaa6a9cbbc9125797b8ca345464c06f1b9", null ],
    [ "SDMA_SetContextSwitchMode", "group__sdma.html#gac965bd4c31319485859b8e102993de29", null ],
    [ "SDMA_GetChannelInterruptStatus", "group__sdma.html#ga0d28ac87cb22d9cad006d4e97320b3b8", null ],
    [ "SDMA_ClearChannelInterruptStatus", "group__sdma.html#ga55f2efdb37c188a7603d921fc66cbfcb", null ],
    [ "SDMA_GetChannelStopStatus", "group__sdma.html#gadfc19c282f5b55dabf7c7d4c30284f79", null ],
    [ "SDMA_ClearChannelStopStatus", "group__sdma.html#ga3dbda3946690f3c99d21b5975334e3f7", null ],
    [ "SDMA_GetChannelPendStatus", "group__sdma.html#gad0c01101714d71e78c79ee90151ab9e7", null ],
    [ "SDMA_ClearChannelPendStatus", "group__sdma.html#gace9cddb0417bc011360351388369f67f", null ],
    [ "SDMA_GetErrorStatus", "group__sdma.html#ga6238761b23b3c6b7a0d95afa00d08435", null ],
    [ "SDMA_GetRequestSourceStatus", "group__sdma.html#ga7e37f4e503be946ae126f5233e33d8c9", null ],
    [ "SDMA_CreateHandle", "group__sdma.html#gaf777aa964e5cf03a07facf8deffd11ee", null ],
    [ "SDMA_InstallBDMemory", "group__sdma.html#ga02f095ca5ab6a8a5b52b96152465e6fd", null ],
    [ "SDMA_SetCallback", "group__sdma.html#gadb5bdca472ad5039473cc53bf7f6ac6e", null ],
    [ "SDMA_PrepareTransfer", "group__sdma.html#ga15697d2f5295a1e844cfd129e9037bbe", null ],
    [ "SDMA_SubmitTransfer", "group__sdma.html#gac0e4a6a850beac832430a82be1257dd3", null ],
    [ "SDMA_StartTransfer", "group__sdma.html#gad46a251361077e1a804de5ed1149c3ad", null ],
    [ "SDMA_StopTransfer", "group__sdma.html#ga03762a76c03b998b8b1fcb58ba1d790d", null ],
    [ "SDMA_AbortTransfer", "group__sdma.html#gab0775208a9fd1a9b888b6941792f0f4b", null ],
    [ "SDMA_HandleIRQ", "group__sdma.html#ga8a06c9b2014e8ed7b155168ef45ac0e4", null ]
];