var group__flash__driver =
[
    [ "flash_execute_in_ram_function_config_t", "group__flash__driver.html#structflash__execute__in__ram__function__config__t", [
      [ "activeFunctionCount", "group__flash__driver.html#a219f882d6490567a156d8e03f38807a6", null ],
      [ "flashRunCommand", "group__flash__driver.html#a91613a8ef82280dbc1c7933f98e0e8de", null ],
      [ "flashCommonBitOperation", "group__flash__driver.html#aa8517087fd0458d59355a7bbfa71ce75", null ]
    ] ],
    [ "flash_swap_state_config_t", "group__flash__driver.html#structflash__swap__state__config__t", [
      [ "flashSwapState", "group__flash__driver.html#a2096dae01732139381f9c675424a14a8", null ],
      [ "currentSwapBlockStatus", "group__flash__driver.html#acaf7027b909be19405f506077e3a1d1c", null ],
      [ "nextSwapBlockStatus", "group__flash__driver.html#a8a46cdb3ff6bce898f3adcc01755aae9", null ]
    ] ],
    [ "flash_swap_ifr_field_config_t", "group__flash__driver.html#structflash__swap__ifr__field__config__t", [
      [ "swapIndicatorAddress", "group__flash__driver.html#ad2d35cf06849eb4a9d7623a44f31d18e", null ],
      [ "swapEnableWord", "group__flash__driver.html#af16943eff42ca625a095685327a8208c", null ],
      [ "reserved0", "group__flash__driver.html#ac694215c8a67627d80baa7098f529a40", null ]
    ] ],
    [ "flash_swap_ifr_field_data_t", "group__flash__driver.html#unionflash__swap__ifr__field__data__t", [
      [ "flashSwapIfrData", "group__flash__driver.html#aea1fd5495b66c828aa92cbd40a564efb", null ],
      [ "flashSwapIfrField", "group__flash__driver.html#ae4b9effbdc4430217a72e6d52210a570", null ]
    ] ],
    [ "pflash_protection_status_low_t", "group__flash__driver.html#unionpflash__protection__status__low__t", [
      [ "protl32b", "group__flash__driver.html#ae7eb0954d72a942292cfb7bb632433c4", null ],
      [ "protsl", "group__flash__driver.html#afd499b87e0a9db3bff4e478aa8ba6f83", null ],
      [ "protsh", "group__flash__driver.html#a68792a49fb3812b0eaa22dca8d491f65", null ]
    ] ],
    [ "pflash_protection_status_t", "group__flash__driver.html#structpflash__protection__status__t", [
      [ "valueLow32b", "group__flash__driver.html#a6899bb8d4d68bf8a8943405a31ed0bf0", null ]
    ] ],
    [ "flash_prefetch_speculation_status_t", "group__flash__driver.html#structflash__prefetch__speculation__status__t", [
      [ "instructionOption", "group__flash__driver.html#af1c9503027d735d885794fe715ee846f", null ],
      [ "dataOption", "group__flash__driver.html#a81112dfde038a529c11802035dcf9a04", null ]
    ] ],
    [ "flash_protection_config_t", "group__flash__driver.html#structflash__protection__config__t", [
      [ "regionBase", "group__flash__driver.html#ab73e588e71938c0e1c4977461c1d9558", null ],
      [ "regionSize", "group__flash__driver.html#ae41715e28f8ece0e70d06c7e47dd71a4", null ],
      [ "regionCount", "group__flash__driver.html#a00c0b6d4b2af6cd93b8b32a48a18659c", null ]
    ] ],
    [ "flash_access_config_t", "group__flash__driver.html#structflash__access__config__t", [
      [ "SegmentBase", "group__flash__driver.html#a1c2fd2e22ffcd2c414a7e38c98daa593", null ],
      [ "SegmentSize", "group__flash__driver.html#ac619df0af88661f6dfaa2fd66e1688d8", null ],
      [ "SegmentCount", "group__flash__driver.html#a5cfa7e584d8dac3afa7a41959168d3b2", null ]
    ] ],
    [ "flash_operation_config_t", "group__flash__driver.html#structflash__operation__config__t", [
      [ "convertedAddress", "group__flash__driver.html#a8c896af670931c72a880df56624ee9eb", null ],
      [ "activeSectorSize", "group__flash__driver.html#acd7eaf8c4af1e02f2b343cbfb67899b9", null ],
      [ "activeBlockSize", "group__flash__driver.html#a6b4b0904f376a3518805d0dd51356fe7", null ],
      [ "blockWriteUnitSize", "group__flash__driver.html#ab1a2d9d6cf1d620134f3d668319d56fa", null ],
      [ "sectorCmdAddressAligment", "group__flash__driver.html#a3b16324d8c454e26e3184f73b23d278d", null ],
      [ "sectionCmdAddressAligment", "group__flash__driver.html#ace5e7199a69c381ede28bd534efd2707", null ],
      [ "resourceCmdAddressAligment", "group__flash__driver.html#ac437b13f1b86ee5b52234c81cd150ed5", null ],
      [ "checkCmdAddressAligment", "group__flash__driver.html#a049b2085180c04ad2f89ef7129667202", null ]
    ] ],
    [ "flash_config_t", "group__flash__driver.html#structflash__config__t", [
      [ "PFlashBlockBase", "group__flash__driver.html#a38d149791c84262f5526f278c250db6c", null ],
      [ "PFlashTotalSize", "group__flash__driver.html#a3d19cd0bb2c4f30c3a0e1a46400f9184", null ],
      [ "PFlashBlockCount", "group__flash__driver.html#a21cfa5c3a9375d76b670ab852f2c5250", null ],
      [ "FlashMemoryIndex", "group__flash__driver.html#acbb0153ae846a7519cda969cede57d3d", null ],
      [ "FlashCacheControllerIndex", "group__flash__driver.html#a2ef1e35b826c8a007b541cb0f0d2eb68", null ],
      [ "Reserved0", "group__flash__driver.html#aeddf4272cd7b01a7a7ddace96acb95e1", null ],
      [ "PFlashSectorSize", "group__flash__driver.html#ae9bec460d5fd4b27c7e4e7096036eebd", null ],
      [ "PFlashCallback", "group__flash__driver.html#a763d03e1c1230bf10a4c97a364b182b6", null ],
      [ "PFlashAccessSegmentSize", "group__flash__driver.html#a237cd61f95c8dd86d418f0853eece25d", null ],
      [ "PFlashAccessSegmentCount", "group__flash__driver.html#acef43a75197233cb7dc2cd7bff8dc938", null ],
      [ "flashExecuteInRamFunctionInfo", "group__flash__driver.html#a01fb9b4fc8ddd85f261eb369febaa9d3", null ],
      [ "FlexRAMBlockBase", "group__flash__driver.html#ac8cd40bbadba45089c0f805221974ef4", null ],
      [ "FlexRAMTotalSize", "group__flash__driver.html#a7d8a95c74597e8741b6fd3b316d89a4d", null ],
      [ "DFlashBlockBase", "group__flash__driver.html#a3b44c68a452f45b1bef4bfbec374baef", null ],
      [ "DFlashTotalSize", "group__flash__driver.html#aecc5a6763e38286f9dc40b126b419c47", null ],
      [ "EEpromTotalSize", "group__flash__driver.html#a464aa51380e2684a11081d56be79499d", null ]
    ] ],
    [ "MAKE_VERSION", "group__flash__driver.html#ga812138aa3315b0c6953c1a26130bcc37", null ],
    [ "FSL_FLASH_DRIVER_VERSION", "group__flash__driver.html#gae110eddb64c67bfc620e80afd94dd281", null ],
    [ "FLASH_SSD_CONFIG_ENABLE_FLEXNVM_SUPPORT", "group__flash__driver.html#ga11c88faa6e9203bb2a2e32914344e4de", null ],
    [ "FLASH_SSD_IS_FLEXNVM_ENABLED", "group__flash__driver.html#ga19c2c365c7d5884b9193e2ff4c63c906", null ],
    [ "FLASH_SSD_CONFIG_ENABLE_SECONDARY_FLASH_SUPPORT", "group__flash__driver.html#ga5baf54de5b08aa4b43c511237411704f", null ],
    [ "FLASH_SSD_IS_SECONDARY_FLASH_ENABLED", "group__flash__driver.html#ga875323f786780305df8bd2538d0e365d", null ],
    [ "FLASH_DRIVER_IS_FLASH_RESIDENT", "group__flash__driver.html#ga1984b55076c622b4ec6bbf187b9a6612", null ],
    [ "FLASH_DRIVER_IS_EXPORTED", "group__flash__driver.html#ga7c3e3dfacf9227253ad6dfc0b95e5c20", null ],
    [ "kStatusGroupGeneric", "group__flash__driver.html#gae148d1f9acb1fee339187aabba151288", null ],
    [ "MAKE_STATUS", "group__flash__driver.html#gad76dfbb0423d0cd3a7b77cbcd9043146", null ],
    [ "FOUR_CHAR_CODE", "group__flash__driver.html#ga3caa666e2a56338f45fa9a008473694d", null ],
    [ "flash_callback_t", "group__flash__driver.html#ga0b6fe9da451666f9c9f48e4c281c9ba5", null ],
    [ "_flash_driver_version_constants", "group__flash__driver.html#ga2cdaf5b1f6b03013e2f0cf2c6d26adae", [
      [ "kFLASH_DriverVersionName", "group__flash__driver.html#gga2cdaf5b1f6b03013e2f0cf2c6d26adaea599e22575bcd9a7aa32a1c7b43591f0b", null ],
      [ "kFLASH_DriverVersionMajor", "group__flash__driver.html#gga2cdaf5b1f6b03013e2f0cf2c6d26adaea4735add39aa49ca212b24fbef9fcb952", null ],
      [ "kFLASH_DriverVersionMinor", "group__flash__driver.html#gga2cdaf5b1f6b03013e2f0cf2c6d26adaeab54b9127ada5f04373dd17edc4e005f4", null ],
      [ "kFLASH_DriverVersionBugfix", "group__flash__driver.html#gga2cdaf5b1f6b03013e2f0cf2c6d26adaea1a3661a27450809aa90f49d6b01781b3", null ]
    ] ],
    [ "_flash_status", "group__flash__driver.html#gaa4639fd4d4503c0dec9925f749d308b2", [
      [ "kStatus_FLASH_Success", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2aacee5186bbe9db2e091dc5c36531cd7c", null ],
      [ "kStatus_FLASH_InvalidArgument", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a85c375f1213ec36dd2cf60e8d9851867", null ],
      [ "kStatus_FLASH_SizeError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a6735801bd1fbd61f7d96a07c2c2f9a76", null ],
      [ "kStatus_FLASH_AlignmentError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a098148af25679d78133103cace7580ea", null ],
      [ "kStatus_FLASH_AddressError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2acc367a368270606eae83264f59b55211", null ],
      [ "kStatus_FLASH_AccessError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2ab9e32512512c1ea1508bc2832f2bd528", null ],
      [ "kStatus_FLASH_ProtectionViolation", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2ab406157d740ab0a07df1fc5d2b159fc3", null ],
      [ "kStatus_FLASH_CommandFailure", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a42567b719be83f23619c498da80643ed", null ],
      [ "kStatus_FLASH_UnknownProperty", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a0ac435d9c4aa24ccb8ab983de1979780", null ],
      [ "kStatus_FLASH_EraseKeyError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a9741e88e6013710b11b81e2af7071a92", null ],
      [ "kStatus_FLASH_RegionExecuteOnly", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2aa3ace4bf14ea333efd6189b8ddf47aca", null ],
      [ "kStatus_FLASH_ExecuteInRamFunctionNotReady", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a87d744021836148574383ddcae44db32", null ],
      [ "kStatus_FLASH_PartitionStatusUpdateFailure", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a91eb156935e27973454eb8df37a36c53", null ],
      [ "kStatus_FLASH_SetFlexramAsEepromError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2ab8b9f6c116209a00338a430e741a1a40", null ],
      [ "kStatus_FLASH_RecoverFlexramAsRamError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a128d2d13d9674d46619a5c698bc7a7b9", null ],
      [ "kStatus_FLASH_SetFlexramAsRamError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2abf40cb19a8c5af9880414ef5f9be1c6d", null ],
      [ "kStatus_FLASH_RecoverFlexramAsEepromError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2aa7652faf3cc559ccda57f8aea502045b", null ],
      [ "kStatus_FLASH_CommandNotSupported", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2ab032e11743a3397f9c81afac3faa9c25", null ],
      [ "kStatus_FLASH_SwapSystemNotInUninitialized", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a5e0c3747a74458caefc231b995f09679", null ],
      [ "kStatus_FLASH_SwapIndicatorAddressError", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2aefaeaf1008f0361fb3ff2d051720cd82", null ],
      [ "kStatus_FLASH_ReadOnlyProperty", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2aa8ed02f4e56f8d6d9ce74b231b5ae2ab", null ],
      [ "kStatus_FLASH_InvalidPropertyValue", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2a78a2e4b86462e7e7ee6090cb81eb9f56", null ],
      [ "kStatus_FLASH_InvalidSpeculationOption", "group__flash__driver.html#ggaa4639fd4d4503c0dec9925f749d308b2aecec5f61f7266f73c1dbce151a56e4f7", null ]
    ] ],
    [ "_flash_driver_api_keys", "group__flash__driver.html#ga95539796ed8672eda06bfc363883a508", [
      [ "kFLASH_ApiEraseKey", "group__flash__driver.html#gga95539796ed8672eda06bfc363883a508a5b52819ce5c4ed6cb099b6f89b7eac56", null ]
    ] ],
    [ "flash_margin_value_t", "group__flash__driver.html#ga2105f5de393a06479e62141dab8dd3fc", [
      [ "kFLASH_MarginValueNormal", "group__flash__driver.html#gga2105f5de393a06479e62141dab8dd3fca9071189977c1f756d42d710288cfb6be", null ],
      [ "kFLASH_MarginValueUser", "group__flash__driver.html#gga2105f5de393a06479e62141dab8dd3fca1c1271a7e38d4e062e202730f6a5ebed", null ],
      [ "kFLASH_MarginValueFactory", "group__flash__driver.html#gga2105f5de393a06479e62141dab8dd3fcadc331ba6a0e6de26eb5c1e755136608d", null ],
      [ "kFLASH_MarginValueInvalid", "group__flash__driver.html#gga2105f5de393a06479e62141dab8dd3fcaa297e7607bb0ccefbe181d1f1d828deb", null ]
    ] ],
    [ "flash_security_state_t", "group__flash__driver.html#ga00b3eb2666116e617f6e45a10218f6a9", [
      [ "kFLASH_SecurityStateNotSecure", "group__flash__driver.html#gga00b3eb2666116e617f6e45a10218f6a9ad48939abe53a98896541d2ee1e7bf752", null ],
      [ "kFLASH_SecurityStateBackdoorEnabled", "group__flash__driver.html#gga00b3eb2666116e617f6e45a10218f6a9a4c4da1c4c06977cc999930f75f38ecc7", null ],
      [ "kFLASH_SecurityStateBackdoorDisabled", "group__flash__driver.html#gga00b3eb2666116e617f6e45a10218f6a9a5c7043f3abad4e8d65f12179c4636dc2", null ]
    ] ],
    [ "flash_protection_state_t", "group__flash__driver.html#ga9378245c649eebea9451367d6b8f9ce2", [
      [ "kFLASH_ProtectionStateUnprotected", "group__flash__driver.html#gga9378245c649eebea9451367d6b8f9ce2ace4690203221c04a4e00de6278a59f1e", null ],
      [ "kFLASH_ProtectionStateProtected", "group__flash__driver.html#gga9378245c649eebea9451367d6b8f9ce2a2c73fefaf7e6e3a065744be1aab71149", null ],
      [ "kFLASH_ProtectionStateMixed", "group__flash__driver.html#gga9378245c649eebea9451367d6b8f9ce2aca2ca63f9c507495e7b0f510b69497bb", null ]
    ] ],
    [ "flash_execute_only_access_state_t", "group__flash__driver.html#gad38961e5692f64548dbf84d652b9ccde", [
      [ "kFLASH_AccessStateUnLimited", "group__flash__driver.html#ggad38961e5692f64548dbf84d652b9ccdea38101c1bc20b060f8fa2f5ac43a04f66", null ],
      [ "kFLASH_AccessStateExecuteOnly", "group__flash__driver.html#ggad38961e5692f64548dbf84d652b9ccdeae275896979de1eca572ae3c7449148f1", null ],
      [ "kFLASH_AccessStateMixed", "group__flash__driver.html#ggad38961e5692f64548dbf84d652b9ccdea0b22f0d8b1f43e064f04d12659293c38", null ]
    ] ],
    [ "flash_property_tag_t", "group__flash__driver.html#ga7b6766fe08a63cf92a5e2a90dd6d010f", [
      [ "kFLASH_PropertyPflashSectorSize", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010faa6c433411ecb4bfea9d74504e266e219", null ],
      [ "kFLASH_PropertyPflashTotalSize", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fa68caa1558315b7704b08d85c42869eb5", null ],
      [ "kFLASH_PropertyPflashBlockSize", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fad8c3b85ca1dc8fc5ef03e1954aef873e", null ],
      [ "kFLASH_PropertyPflashBlockCount", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fabb40541593520b9c1a87648a26e81957", null ],
      [ "kFLASH_PropertyPflashBlockBaseAddr", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fa0159abfba4e6ca47c70a89ea2fc24ab9", null ],
      [ "kFLASH_PropertyPflashFacSupport", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fa82ed61ee8b06813070ded3e06675e07c", null ],
      [ "kFLASH_PropertyPflashAccessSegmentSize", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fab1ff313c13a7c4c9794c97d894c784d1", null ],
      [ "kFLASH_PropertyPflashAccessSegmentCount", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fad73e31438b9bf8ea5d95e4807371e414", null ],
      [ "kFLASH_PropertyFlexRamBlockBaseAddr", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fa9ca389a1c885600feb6900b0d34f7c14", null ],
      [ "kFLASH_PropertyFlexRamTotalSize", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fa062cdf2d78e722b71b6037eb6f60cbf4", null ],
      [ "kFLASH_PropertyDflashSectorSize", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fa51190ea2cbee3594588931079b0c7fc6", null ],
      [ "kFLASH_PropertyDflashTotalSize", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fa4a988286f54bb8572af9487870bbb32c", null ],
      [ "kFLASH_PropertyDflashBlockSize", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fa4229b9f10be15a5169dc4b6e011536e3", null ],
      [ "kFLASH_PropertyDflashBlockCount", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fad6ce4a3ff01396a5d61fdba8bbd2bb9d", null ],
      [ "kFLASH_PropertyDflashBlockBaseAddr", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fa8ec2b47abac2083b760dd47d9bebcde0", null ],
      [ "kFLASH_PropertyEepromTotalSize", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fab3520a58d0f8494d3d7e987d30cee6ed", null ],
      [ "kFLASH_PropertyFlashMemoryIndex", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010faa6fec8db4334119a40f503e8383aef5f", null ],
      [ "kFLASH_PropertyFlashCacheControllerIndex", "group__flash__driver.html#gga7b6766fe08a63cf92a5e2a90dd6d010fae0544369d5a032620fe9ba67efd3e2eb", null ]
    ] ],
    [ "_flash_execute_in_ram_function_constants", "group__flash__driver.html#gae76bb8924742f0483f66c4d3917bb31e", [
      [ "kFLASH_ExecuteInRamFunctionMaxSizeInWords", "group__flash__driver.html#ggae76bb8924742f0483f66c4d3917bb31eade9157bf0da5bc77b08d22ad1c33316b", null ],
      [ "kFLASH_ExecuteInRamFunctionTotalNum", "group__flash__driver.html#ggae76bb8924742f0483f66c4d3917bb31ea2e18b9dc426a3146df8f959c1c874ce0", null ]
    ] ],
    [ "flash_read_resource_option_t", "group__flash__driver.html#ga70d45ebc64d50e9868f7016c8a417c51", [
      [ "kFLASH_ResourceOptionFlashIfr", "group__flash__driver.html#gga70d45ebc64d50e9868f7016c8a417c51a80bd0a5fe1432c2a97a3bd8e4a5320ab", null ],
      [ "kFLASH_ResourceOptionVersionId", "group__flash__driver.html#gga70d45ebc64d50e9868f7016c8a417c51a900b2f3a7dbd7260d7883fe9fa816c6d", null ]
    ] ],
    [ "_flash_read_resource_range", "group__flash__driver.html#ga4fcca89ee32fbd2497a2ee1f8425a0ea", [
      [ "kFLASH_ResourceRangePflashIfrSizeInBytes", "group__flash__driver.html#gga4fcca89ee32fbd2497a2ee1f8425a0eaa3ceb799170297b7e2d6c763ea865e5ce", null ],
      [ "kFLASH_ResourceRangeVersionIdSizeInBytes", "group__flash__driver.html#gga4fcca89ee32fbd2497a2ee1f8425a0eaa441baa7064c883858a7e2c21a35ce286", null ],
      [ "kFLASH_ResourceRangeVersionIdStart", "group__flash__driver.html#gga4fcca89ee32fbd2497a2ee1f8425a0eaaa06ff3ae089ba3b7540a1e33c678ac42", null ],
      [ "kFLASH_ResourceRangeVersionIdEnd", "group__flash__driver.html#gga4fcca89ee32fbd2497a2ee1f8425a0eaad1fafb95c14b7c13d8f613328a820808", null ],
      [ "kFLASH_ResourceRangePflashSwapIfrEnd", "group__flash__driver.html#gga4fcca89ee32fbd2497a2ee1f8425a0eaa59a0f5e9ae49fbe879f95168143a7036", null ],
      [ "kFLASH_ResourceRangeDflashIfrStart", "group__flash__driver.html#gga4fcca89ee32fbd2497a2ee1f8425a0eaaab7328ad8167382b6f954439416d819a", null ],
      [ "kFLASH_ResourceRangeDflashIfrEnd", "group__flash__driver.html#gga4fcca89ee32fbd2497a2ee1f8425a0eaa1186524c65263c078d37ea5156af2d9b", null ]
    ] ],
    [ "_k3_flash_read_once_index", "group__flash__driver.html#gaacb40e28ac279f97ab0540b21d5f92a7", [
      [ "kFLASH_RecordIndexSwapAddr", "group__flash__driver.html#ggaacb40e28ac279f97ab0540b21d5f92a7a36698c67d897f6202be3e91cf4b8a040", null ],
      [ "kFLASH_RecordIndexSwapEnable", "group__flash__driver.html#ggaacb40e28ac279f97ab0540b21d5f92a7acf9207f98cfd61addf57656fc9c17113", null ],
      [ "kFLASH_RecordIndexSwapDisable", "group__flash__driver.html#ggaacb40e28ac279f97ab0540b21d5f92a7ab61fc632bb7166ef6fd4c3d1b873f896", null ]
    ] ],
    [ "flash_flexram_function_option_t", "group__flash__driver.html#ga3812bab3ef6aacfbe889b1a81e25c76d", [
      [ "kFLASH_FlexramFunctionOptionAvailableAsRam", "group__flash__driver.html#gga3812bab3ef6aacfbe889b1a81e25c76dae2deac28c06aeebad0a1449006055042", null ],
      [ "kFLASH_FlexramFunctionOptionAvailableForEeprom", "group__flash__driver.html#gga3812bab3ef6aacfbe889b1a81e25c76daf87c9b48af2e57f1b390934473f3a05d", null ]
    ] ],
    [ "_flash_acceleration_ram_property", "group__flash__driver.html#gabcfe38ad71d8e03bac192b4f3d217fa7", null ],
    [ "flash_swap_function_option_t", "group__flash__driver.html#ga799e1be1686e21a35457d674fc746728", [
      [ "kFLASH_SwapFunctionOptionEnable", "group__flash__driver.html#gga799e1be1686e21a35457d674fc746728ac633995521391aa0d8c50ec6f0d9b51d", null ],
      [ "kFLASH_SwapFunctionOptionDisable", "group__flash__driver.html#gga799e1be1686e21a35457d674fc746728a3e7f48ea919486120b3e12f045f70856", null ]
    ] ],
    [ "flash_swap_control_option_t", "group__flash__driver.html#gac4c39d832a41bddec3f2953df588adeb", [
      [ "kFLASH_SwapControlOptionIntializeSystem", "group__flash__driver.html#ggac4c39d832a41bddec3f2953df588adeba7ff8249d311051d01d7d82cf8d5c697c", null ],
      [ "kFLASH_SwapControlOptionSetInUpdateState", "group__flash__driver.html#ggac4c39d832a41bddec3f2953df588adeba1f12897efebbc3eb661baa12d9d68f3a", null ],
      [ "kFLASH_SwapControlOptionSetInCompleteState", "group__flash__driver.html#ggac4c39d832a41bddec3f2953df588adebae6b96fa981926920018e86905146c15d", null ],
      [ "kFLASH_SwapControlOptionReportStatus", "group__flash__driver.html#ggac4c39d832a41bddec3f2953df588adebacba9f04f370da73a53f123a5ff7b78a7", null ],
      [ "kFLASH_SwapControlOptionDisableSystem", "group__flash__driver.html#ggac4c39d832a41bddec3f2953df588adeba871265364e63fa96388c0434e7528697", null ]
    ] ],
    [ "flash_swap_state_t", "group__flash__driver.html#gabbbbdf78a0245100d29ad83bd6f0d141", [
      [ "kFLASH_SwapStateUninitialized", "group__flash__driver.html#ggabbbbdf78a0245100d29ad83bd6f0d141acb872b5aad441b829eacd3a2f62f1e5b", null ],
      [ "kFLASH_SwapStateReady", "group__flash__driver.html#ggabbbbdf78a0245100d29ad83bd6f0d141a969d0802a8bcb2eef29797f9b709c219", null ],
      [ "kFLASH_SwapStateUpdate", "group__flash__driver.html#ggabbbbdf78a0245100d29ad83bd6f0d141aef30cba32280c7666c4e4432c12b477b", null ],
      [ "kFLASH_SwapStateUpdateErased", "group__flash__driver.html#ggabbbbdf78a0245100d29ad83bd6f0d141af2fecdb89c4ca80ba7a4550d529af174", null ],
      [ "kFLASH_SwapStateComplete", "group__flash__driver.html#ggabbbbdf78a0245100d29ad83bd6f0d141a34e8e14bf7d3915ab4b10331b9e964f6", null ],
      [ "kFLASH_SwapStateDisabled", "group__flash__driver.html#ggabbbbdf78a0245100d29ad83bd6f0d141aac58ee8947ef084ac3b1be621ab08328", null ]
    ] ],
    [ "flash_swap_block_status_t", "group__flash__driver.html#gafa125571c739fe8d5c41cfe9402cb063", [
      [ "kFLASH_SwapBlockStatusLowerHalfProgramBlocksAtZero", "group__flash__driver.html#ggafa125571c739fe8d5c41cfe9402cb063a8ddfa234a76fbc404ff79bf6f39cc14a", null ],
      [ "kFLASH_SwapBlockStatusUpperHalfProgramBlocksAtZero", "group__flash__driver.html#ggafa125571c739fe8d5c41cfe9402cb063a3068333e63702a882c3638d4eeee56c6", null ]
    ] ],
    [ "flash_partition_flexram_load_option_t", "group__flash__driver.html#ga46a69f0afbc5ceaae3748429aa2d7901", [
      [ "kFLASH_PartitionFlexramLoadOptionLoadedWithValidEepromData", "group__flash__driver.html#gga46a69f0afbc5ceaae3748429aa2d7901aac927695cefaca6a14f393c9e51cb162", null ],
      [ "kFLASH_PartitionFlexramLoadOptionNotLoaded", "group__flash__driver.html#gga46a69f0afbc5ceaae3748429aa2d7901a90872ebb415ec51f16de8e798f95c0d3", null ]
    ] ],
    [ "flash_memory_index_t", "group__flash__driver.html#ga646da5564edc127f844aa8ce8089b85a", [
      [ "kFLASH_MemoryIndexPrimaryFlash", "group__flash__driver.html#gga646da5564edc127f844aa8ce8089b85aa991e0f42ebab7f5e509ceaa77555a016", null ],
      [ "kFLASH_MemoryIndexSecondaryFlash", "group__flash__driver.html#gga646da5564edc127f844aa8ce8089b85aad309ef5166c98397df8348e26aa69b4c", null ]
    ] ],
    [ "flash_cache_controller_index_t", "group__flash__driver.html#ga047144ff4c44e3c2feb2cd2642004f92", [
      [ "kFLASH_CacheControllerIndexForCore0", "group__flash__driver.html#gga047144ff4c44e3c2feb2cd2642004f92a48ce668a117a04bf1b1d8bd331dcdfc5", null ],
      [ "kFLASH_CacheControllerIndexForCore1", "group__flash__driver.html#gga047144ff4c44e3c2feb2cd2642004f92a51a1988f398646c68dd5bdd6e35922f8", null ]
    ] ],
    [ "flash_prefetch_speculation_option_t", "group__flash__driver.html#ga806f9cbac3077a9025ef03993a189695", null ],
    [ "flash_cache_clear_process_t", "group__flash__driver.html#ga94171369e7b336eaad8f715f9a959f6e", [
      [ "kFLASH_CacheClearProcessPre", "group__flash__driver.html#gga94171369e7b336eaad8f715f9a959f6ea89b6d15e6f0b42ad6a3a225da573eb5b", null ],
      [ "kFLASH_CacheClearProcessPost", "group__flash__driver.html#gga94171369e7b336eaad8f715f9a959f6ea9fbc2d0126cb8884ff1ad6a35103e698", null ]
    ] ],
    [ "FLASH_Init", "group__flash__driver.html#ga19c10d3524010a2d33369855d74a5c04", null ],
    [ "FLASH_SetCallback", "group__flash__driver.html#gaa89a4c7704b471af58c2d0f629a87ff4", null ],
    [ "FLASH_PrepareExecuteInRamFunctions", "group__flash__driver.html#gab69bf91c617921217e0c5f302cc33daf", null ],
    [ "FLASH_EraseAll", "group__flash__driver.html#ga620aad3b28de9e89e3297bd1bbcd5618", null ],
    [ "FLASH_Erase", "group__flash__driver.html#gae9cb58f4cf76fec37631c4f344e2fb10", null ],
    [ "FLASH_EraseAllExecuteOnlySegments", "group__flash__driver.html#gad39d36451aef62e097b545518ef984f6", null ],
    [ "FLASH_Program", "group__flash__driver.html#gaac44aa0a054e62adb306bf4910a99bd4", null ],
    [ "FLASH_ProgramOnce", "group__flash__driver.html#ga7e45501b1e1c6ddd5b10a76e422c4d29", null ],
    [ "FLASH_ReadResource", "group__flash__driver.html#gad9ffbb7b82028934b864dd6fb359df92", null ],
    [ "FLASH_ReadOnce", "group__flash__driver.html#ga3921a84a4dc5f7f824d6816d17cbc55b", null ],
    [ "FLASH_GetSecurityState", "group__flash__driver.html#gac228a2b25c575e489b47ac3c0652a50a", null ],
    [ "FLASH_SecurityBypass", "group__flash__driver.html#gae54de3df1d9c73f19b1bd2ad713c2607", null ],
    [ "FLASH_VerifyEraseAll", "group__flash__driver.html#ga85a55ca1a9dcdd134989e6c722007db6", null ],
    [ "FLASH_VerifyErase", "group__flash__driver.html#gaf88e46a975e29f113fd29798aeb5b66c", null ],
    [ "FLASH_VerifyProgram", "group__flash__driver.html#ga17820a03a93127cf52f6e7b5ed830c8b", null ],
    [ "FLASH_VerifyEraseAllExecuteOnlySegments", "group__flash__driver.html#gab15b9e74df82e1ebc15c802c34eaeaab", null ],
    [ "FLASH_IsProtected", "group__flash__driver.html#ga3c28d781a630bb833e469cd11342f03a", null ],
    [ "FLASH_IsExecuteOnly", "group__flash__driver.html#ga288c88008135094a458b0464aca7724a", null ],
    [ "FLASH_GetProperty", "group__flash__driver.html#gafd0d83bc8fec50fe9a9121d2ec906697", null ],
    [ "FLASH_SetProperty", "group__flash__driver.html#gad48974c59920c8a0958355f1411569ef", null ],
    [ "FLASH_PflashSetProtection", "group__flash__driver.html#ga8581c684a81956bb74aec112041dfe4f", null ],
    [ "FLASH_PflashGetProtection", "group__flash__driver.html#ga5c648d2d1d6e6c6a1942c6615c58e38d", null ]
];