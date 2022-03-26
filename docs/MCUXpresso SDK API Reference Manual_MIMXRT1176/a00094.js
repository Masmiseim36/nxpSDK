var a00094 =
[
    [ "ssarc_descriptor_config_t", "a00094.html#a00692", [
      [ "address", "a00094.html#a662007b4e4c7502fea9b6ed639cbf4ba", null ],
      [ "data", "a00094.html#a61930fd142cc8c63904a7503a8c921cb", null ],
      [ "size", "a00094.html#a040623a9fc1146c31dcedf6fe332419e", null ],
      [ "operation", "a00094.html#ad74a1cecaf93b105e3e3d05023a679f9", null ],
      [ "type", "a00094.html#a2b6080fb90fe0297ca4830e4f655e081", null ]
    ] ],
    [ "ssarc_group_config_t", "a00094.html#a00693", [
      [ "cpuDomain", "a00094.html#afb0108b7a03709cbac87b624091dfe76", null ],
      [ "startIndex", "a00094.html#a3c546561d7190e9631a349795a99781a", null ],
      [ "endIndex", "a00094.html#af5fa9b62c388e59071f324e074df41da", null ],
      [ "restoreOrder", "a00094.html#a7495de0bad49c5abcb7e80845e0e46d7", null ],
      [ "saveOrder", "a00094.html#a7817a35d99ae8fef4bd19ff159b70a39", null ],
      [ "restorePriority", "a00094.html#a9e61e6642491ac4e2c53f87be5ab10af", null ],
      [ "savePriority", "a00094.html#a9ee3d5d4e1d76395b4e9bab00ac65d8c", null ],
      [ "powerDomain", "a00094.html#a80c6a9ec7cc2168f6fa3ccbe58826e4d", null ],
      [ "highestAddress", "a00094.html#a4d3f33528b0eee6daf6582abea6c47fa", null ],
      [ "lowestAddress", "a00094.html#ab40ba221f5ba4da0f8de56f5d276ca49", null ]
    ] ],
    [ "FSL_SSARC_DRIVER_VERSION", "a00094.html#ga27b325de74f64c08f1464f671943f66b", null ],
    [ "_ssarc_interrupt_status_flags", "a00094.html#ga8eb8bc46efecabbf832a79a1753e0eff", [
      [ "kSSARC_AddressErrorFlag", "a00094.html#gga8eb8bc46efecabbf832a79a1753e0effa9027f068d44c72ecc010c766cbd5e7f5", null ],
      [ "kSSARC_AHBErrorFlag", "a00094.html#gga8eb8bc46efecabbf832a79a1753e0effa7f49e68c09ebe6f57ac15a10760c18b1", null ],
      [ "kSSARC_SoftwareRequestDoneFlag", "a00094.html#gga8eb8bc46efecabbf832a79a1753e0effa389604ed9d2cdbfbddbb814d3d17e43d", null ],
      [ "kSSARC_TimeoutFlag", "a00094.html#gga8eb8bc46efecabbf832a79a1753e0effa2e7b5a28b065b99ba08461e9ab9cd58e", null ],
      [ "kSSARC_GroupConflictFlag", "a00094.html#gga8eb8bc46efecabbf832a79a1753e0effac0cc87c30ca069189273d71d4381ec9b", null ]
    ] ],
    [ "ssarc_descriptor_register_size_t", "a00094.html#ga278e580e271caef7217987bf4725c81f", [
      [ "kSSARC_DescriptorRegister8bitWidth", "a00094.html#gga278e580e271caef7217987bf4725c81fabd66a01347217b0ffd82acb34d7d4414", null ],
      [ "kSSARC_DescriptorRegister16bitWidth", "a00094.html#gga278e580e271caef7217987bf4725c81faf2d646983655c7d6007a56e765f9bdcc", null ],
      [ "kSSARC_DescriptorRegister32bitWidth", "a00094.html#gga278e580e271caef7217987bf4725c81fa5bd8c38bbc3df063e24694dbe2c19f44", null ]
    ] ],
    [ "ssarc_descriptor_operation_t", "a00094.html#gae7e5c6694868b03f3ed8ac80b5ae3f3a", [
      [ "kSSARC_SaveDisableRestoreDisable", "a00094.html#ggae7e5c6694868b03f3ed8ac80b5ae3f3aa3352cdd08a7581f8dd5559677c4b9255", null ],
      [ "kSSARC_SaveEnableRestoreDisable", "a00094.html#ggae7e5c6694868b03f3ed8ac80b5ae3f3aa6e075c2c8d00c06ce3f1857121ab2fe0", null ],
      [ "kSSARC_SaveDisableRestoreEnable", "a00094.html#ggae7e5c6694868b03f3ed8ac80b5ae3f3aa14a9fcf1f3713feeb193c43fffc640e5", null ],
      [ "kSSARC_SaveEnableRestoreEnable", "a00094.html#ggae7e5c6694868b03f3ed8ac80b5ae3f3aa105e9315c5dd74468605ffa19f05e386", null ]
    ] ],
    [ "ssarc_descriptor_type_t", "a00094.html#gad2240f2aec12fe46685dd8bb2834a2ec", [
      [ "kSSARC_ReadValueWriteBack", "a00094.html#ggad2240f2aec12fe46685dd8bb2834a2eca172f621d6a45f548eadc8ed3919d038f", null ],
      [ "kSSARC_WriteFixedValue", "a00094.html#ggad2240f2aec12fe46685dd8bb2834a2ecae9f1d807a3c8f46517664f9066012732", null ],
      [ "kSSARC_RMWOr", "a00094.html#ggad2240f2aec12fe46685dd8bb2834a2ecab59efd8c637641d0ed9fefb840a99e48", null ],
      [ "kSSARC_RMWAnd", "a00094.html#ggad2240f2aec12fe46685dd8bb2834a2eca9df8e2904c0e0e556056217b737ee162", null ],
      [ "kSSARC_DelayCycles", "a00094.html#ggad2240f2aec12fe46685dd8bb2834a2ecab744b784f91e6fdad2e427b28751627d", null ],
      [ "kSSARC_Polling0", "a00094.html#ggad2240f2aec12fe46685dd8bb2834a2eca207bb48e40d195b0985f81afcdda9fe4", null ],
      [ "kSSARC_Polling1", "a00094.html#ggad2240f2aec12fe46685dd8bb2834a2ecaad1254a5a7dcdc1284ed933c50bd281a", null ]
    ] ],
    [ "ssarc_save_restore_order_t", "a00094.html#gaa99f1b95210210af473b8029a13610b3", [
      [ "kSSARC_ProcessFromStartToEnd", "a00094.html#ggaa99f1b95210210af473b8029a13610b3a75739b9e473697a2c49f21b4bc070805", null ],
      [ "kSSARC_ProcessFromEndToStart", "a00094.html#ggaa99f1b95210210af473b8029a13610b3af6e5b6edccdee4d7a24e49ac65f5d368", null ]
    ] ],
    [ "ssarc_software_trigger_mode_t", "a00094.html#ga097f687e370fb0e62b9530538a05f113", [
      [ "kSSARC_TriggerSaveRequest", "a00094.html#gga097f687e370fb0e62b9530538a05f113acaf269914b1a9628ab4ded77242d295d", null ],
      [ "kSSARC_TriggerRestoreRequest", "a00094.html#gga097f687e370fb0e62b9530538a05f113ac74e3e4eacf7f6d89f77ac6a07c6d3a8", null ]
    ] ],
    [ "SSARC_GetDescriptorRegisterAddress", "a00094.html#gafdd8c9cffa0d352e32d4a0e64b8f8ef0", null ],
    [ "SSARC_GetDescriptorRegisterData", "a00094.html#ga8960d148b6596a9c8731d128c512141e", null ],
    [ "SSARC_SetDescriptorConfig", "a00094.html#gae0f04b064c0941b7e52e38742dfa4981", null ],
    [ "SSARC_GroupInit", "a00094.html#gaaee941d2eaf25b22d972e604ea67f168", null ],
    [ "SSARC_GroupDeinit", "a00094.html#ga069022bb5a3ef349b00a888ac614c564", null ],
    [ "SSARC_LockGroupDomain", "a00094.html#gae385376fa45812d5bf16188497e64240", null ],
    [ "SSARC_LockGroupWrite", "a00094.html#gaf5c94e8842d38a72d07c5f04168250e8", null ],
    [ "SSARC_LockGroupRead", "a00094.html#gaceef09e6cfcf4a9c700a0b0ab827584c", null ],
    [ "SSARC_TriggerSoftwareRequest", "a00094.html#ga53d1aece7b50a1df756aae80efce6fbb", null ],
    [ "SSARC_ResetWholeBlock", "a00094.html#gabc06c63ffa6d684334147c200d160c77", null ],
    [ "SSARC_EnableHardwareRequest", "a00094.html#ga3884e36b37fdbf11016e71eecffca7da", null ],
    [ "SSARC_GetStatusFlags", "a00094.html#ga6abbb844d06669dfc58e74dd0ae6424f", null ],
    [ "SSARC_ClearStatusFlags", "a00094.html#ga177bcf1aad06174b6c86a7e38f15a98a", null ],
    [ "SSARC_GetErrorIndex", "a00094.html#gac40c7a32bacb151a7d500aae519a8cc7", null ],
    [ "SSARC_SetTimeoutValue", "a00094.html#gacc98dcaebd5c2d845b79835032aac81b", null ],
    [ "SSARC_GetTimeoutValue", "a00094.html#ga718fc8af3b4091562d21d6bfe3433da2", null ],
    [ "SSARC_GetHardwareRequestRestorePendingGroup", "a00094.html#ga495e84f01a90e85bc68e88d25e24c784", null ],
    [ "SSARC_GetHardwareRequestSavePendingGroup", "a00094.html#ga0c5482cb0f2c0e5c7efe8c5de1f115de", null ],
    [ "SSARC_GetSoftwareRequestRestorePendingGroup", "a00094.html#gaf35f81c1a4a0c54d285b39757e128e1c", null ],
    [ "SSARC_GetSoftwareRequestSavePendingGroup", "a00094.html#gaf48bd135a46c75af2d923c3a8d2a88bf", null ]
];