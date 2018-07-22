var group__cmp =
[
    [ "cmp_config_t", "group__cmp.html#structcmp__config__t", [
      [ "enableCmp", "group__cmp.html#a87248f8e354fbe0310fa9968d2437db3", null ],
      [ "hysteresisMode", "group__cmp.html#aa4e3d61d0cbbf97ef31d633ea731eb58", null ],
      [ "enableHighSpeed", "group__cmp.html#a0449a799c02a2b534609c576509af9ee", null ],
      [ "enableInvertOutput", "group__cmp.html#a5e1cfbd954c1ce419a47a0f495b011ed", null ],
      [ "useUnfilteredOutput", "group__cmp.html#afe5cb349fa86a64a7fa195655d8be65b", null ],
      [ "enablePinOut", "group__cmp.html#a2dee30cf50e5df9902360f800d0bbe69", null ],
      [ "enableTriggerMode", "group__cmp.html#a3843ec359f9dc7907fc8677fed52c4bd", null ]
    ] ],
    [ "cmp_filter_config_t", "group__cmp.html#structcmp__filter__config__t", [
      [ "enableSample", "group__cmp.html#ad84aa6fee10de5e95b8c933296923df9", null ],
      [ "filterCount", "group__cmp.html#a1ee3e17eae70b6efa023d51367c9d798", null ],
      [ "filterPeriod", "group__cmp.html#ac56e3c13889d8eee7c595a58718c8964", null ]
    ] ],
    [ "cmp_dac_config_t", "group__cmp.html#structcmp__dac__config__t", [
      [ "referenceVoltageSource", "group__cmp.html#a01753d40f590d16345dd45abefb8bbe6", null ],
      [ "DACValue", "group__cmp.html#aa44061486be24d6276a2aae0283c8153", null ]
    ] ],
    [ "FSL_CMP_DRIVER_VERSION", "group__cmp.html#ga8b6413b07d768817e75758d8a6ce612b", null ],
    [ "_cmp_interrupt_enable", "group__cmp.html#ga985bb411310dc841bb432072599dceb5", [
      [ "kCMP_OutputRisingInterruptEnable", "group__cmp.html#gga985bb411310dc841bb432072599dceb5a0ff51a957f1d4f252f6857731c0b5ca1", null ],
      [ "kCMP_OutputFallingInterruptEnable", "group__cmp.html#gga985bb411310dc841bb432072599dceb5a8976b76c2ef49c93106d9f561ca666d2", null ]
    ] ],
    [ "_cmp_status_flags", "group__cmp.html#ga948ec869580a67bebaff6f2e0f3e42c8", [
      [ "kCMP_OutputRisingEventFlag", "group__cmp.html#gga948ec869580a67bebaff6f2e0f3e42c8aa646eb5f296db0906e89322ca6ec0da1", null ],
      [ "kCMP_OutputFallingEventFlag", "group__cmp.html#gga948ec869580a67bebaff6f2e0f3e42c8a1c9bd8b6e0825ad465a47c60a3dad473", null ],
      [ "kCMP_OutputAssertEventFlag", "group__cmp.html#gga948ec869580a67bebaff6f2e0f3e42c8a9b4382c01c2497d4a8510fe8cd009def", null ]
    ] ],
    [ "cmp_hysteresis_mode_t", "group__cmp.html#gab375044e3121d8cf7d56a528f075b4c9", [
      [ "kCMP_HysteresisLevel0", "group__cmp.html#ggab375044e3121d8cf7d56a528f075b4c9a204ac01271f075b8f56785c3777b61ba", null ],
      [ "kCMP_HysteresisLevel1", "group__cmp.html#ggab375044e3121d8cf7d56a528f075b4c9a1f34db50168b964cf653f7ee8b8a3afb", null ],
      [ "kCMP_HysteresisLevel2", "group__cmp.html#ggab375044e3121d8cf7d56a528f075b4c9a5e86245ea1f88cdcc54482707c7dc2d2", null ],
      [ "kCMP_HysteresisLevel3", "group__cmp.html#ggab375044e3121d8cf7d56a528f075b4c9a3923e973b3f6a0f2b6d0ffd271b3735d", null ]
    ] ],
    [ "cmp_reference_voltage_source_t", "group__cmp.html#ga2f0d85949fd79dbb3b10fa0fde34f329", [
      [ "kCMP_VrefSourceVin1", "group__cmp.html#gga2f0d85949fd79dbb3b10fa0fde34f329ac567b56a364a1bc2816bfe68a4696b95", null ],
      [ "kCMP_VrefSourceVin2", "group__cmp.html#gga2f0d85949fd79dbb3b10fa0fde34f329a4debdbdd1f9e6ade8b7ef646332251ca", null ]
    ] ],
    [ "CMP_Init", "group__cmp.html#ga6f62245ac63801ce51bfaf9188c9bc99", null ],
    [ "CMP_Deinit", "group__cmp.html#ga3b6fb1ead2c6dd4b13246b630a91f4d8", null ],
    [ "CMP_Enable", "group__cmp.html#ga265b23b5d173fc389289aeac99f94402", null ],
    [ "CMP_GetDefaultConfig", "group__cmp.html#gae3f987e58dece632e66665514c01e109", null ],
    [ "CMP_SetInputChannels", "group__cmp.html#ga7021cf6a3df2d233781d9763d4562260", null ],
    [ "CMP_EnableDMA", "group__cmp.html#gaf2a4aaf25e16818814fd9d68d81e705c", null ],
    [ "CMP_EnableWindowMode", "group__cmp.html#ga51ddc6286d9078a971af1c21fcd67b35", null ],
    [ "CMP_EnablePassThroughMode", "group__cmp.html#ga2aaf2c74619f7f36f4d97cde78428e44", null ],
    [ "CMP_SetFilterConfig", "group__cmp.html#ga57bade782c390ef4b805bde6198fa611", null ],
    [ "CMP_SetDACConfig", "group__cmp.html#ga61434c1828d7ef35a349eef6ec37d0c2", null ],
    [ "CMP_EnableInterrupts", "group__cmp.html#ga9e7dc0f35a19a4b10894d0eca547befb", null ],
    [ "CMP_DisableInterrupts", "group__cmp.html#ga10b5e9acf21bc30c754b76dc9afd0db6", null ],
    [ "CMP_GetStatusFlags", "group__cmp.html#ga383d33c2a6b879aabfe2e5edf283ee3a", null ],
    [ "CMP_ClearStatusFlags", "group__cmp.html#gae9ff11446780bc7a7e76a66660a5072a", null ]
];