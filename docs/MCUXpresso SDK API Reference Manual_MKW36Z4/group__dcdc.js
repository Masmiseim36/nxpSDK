var group__dcdc =
[
    [ "dcdc_low_power_config_t", "group__dcdc.html#structdcdc__low__power__config__t", [
      [ "workModeInVLPRW", "group__dcdc.html#aaea7dce93c81745f11a38642df470fe6", null ],
      [ "workModeInVLPS", "group__dcdc.html#a7104f65423c6ccec781c2f9ecae0f1ad", null ],
      [ "enableHysteresisVoltageSense", "group__dcdc.html#a690aafc622c67c4dcd909ea403e5857b", null ],
      [ "enableAdjustHystereticValueSense", "group__dcdc.html#a01e0a27407950ccb4e3b5faf2ddbfd11", null ],
      [ "enableHystersisComparator", "group__dcdc.html#a06d6c7b0e3f5167e1207a7790eaf7c92", null ],
      [ "enableAdjustHystereticValueComparator", "group__dcdc.html#a0a8926606bf0d131fc45612d86acbff4", null ],
      [ "hystereticUpperThresholdValue", "group__dcdc.html#aae3c1384585229cfb2e21691de1ea40e", null ],
      [ "hystereticLowerThresholdValue", "group__dcdc.html#adfc5d35e29a2d53526d830d60af6a336", null ],
      [ "enableDiffComparators", "group__dcdc.html#ad8dbf4c51048e669b6033fb76c380f51", null ]
    ] ],
    [ "dcdc_loop_control_config_t", "group__dcdc.html#structdcdc__loop__control__config__t", [
      [ "enableDiffHysteresis", "group__dcdc.html#a7f4aed589aa7e3a00c187a515da6d257", null ],
      [ "enableCommonHysteresis", "group__dcdc.html#aa585f6efdba3cb2bc66b4c658c0b0e80", null ],
      [ "enableDiffHysteresisThresh", "group__dcdc.html#ab348aefa38ba7b0d037f8bcf8dfef913", null ],
      [ "enableCommonHysteresisThresh", "group__dcdc.html#af8f08555038791a2efc82c6a91532f1d", null ],
      [ "enableInvertHysteresisSign", "group__dcdc.html#afc343fa7b91648dd8823d261c9cf4ab7", null ]
    ] ],
    [ "dcdc_min_power_config_t", "group__dcdc.html#structdcdc__min__power__config__t", [
      [ "enableUseHalfFetForContinuous", "group__dcdc.html#a39ebff4d52873db9b397394cff644b80", null ],
      [ "enableUseDoubleFetForContinuous", "group__dcdc.html#a5bf717a17978c6451fba890b79ba98c5", null ],
      [ "enableUseHalfFreqForContinuous", "group__dcdc.html#adbe01c99b380a3d872f7fd6f787ab7cc", null ],
      [ "enableUseHalfFetForPulsed", "group__dcdc.html#aae1519c4541d4255c5b207f252b107b9", null ],
      [ "enableUseDoubleFetForPulsed", "group__dcdc.html#a18362837de807697f46a5988efbf2492", null ],
      [ "enableUseHalfFreqForPulsed", "group__dcdc.html#aa3b64d51b3ff33ed73c1cc7160f27351", null ]
    ] ],
    [ "dcdc_pulsed_integrator_config_t", "group__dcdc.html#structdcdc__pulsed__integrator__config__t", [
      [ "enableUseUserIntegratorValue", "group__dcdc.html#a121ce0c48e0477c30e1bd1cf12a45326", null ],
      [ "userIntegratorValue", "group__dcdc.html#a05d50ef71290a18297aaac176a91e25f", null ],
      [ "enablePulseRunSpeedup", "group__dcdc.html#ac24fb76ed2c5e9b034c2a348fb64f991", null ]
    ] ],
    [ "FSL_DCDC_DRIVER_VERSION", "group__dcdc.html#gaead28d0a1704d502fa7e4d83448530d9", null ],
    [ "_dcdc_status_flags_t", "group__dcdc.html#gae715f6e4017ad893d5b131cb1b65bd97", [
      [ "kDCDC_LockedOKStatus", "group__dcdc.html#ggae715f6e4017ad893d5b131cb1b65bd97a88bc89752444c6db226ac58686c6dda4", null ],
      [ "kDCDC_PSwitchStatus", "group__dcdc.html#ggae715f6e4017ad893d5b131cb1b65bd97a7daf36225f4f25b22d85a7fab6650660", null ],
      [ "kDCDC_PSwitchInterruptStatus", "group__dcdc.html#ggae715f6e4017ad893d5b131cb1b65bd97a5bf0c16c475f70ecc38b341df964b750", null ]
    ] ],
    [ "_dcdc_interrupt_enable_t", "group__dcdc.html#ga0282f83bfd81de43a5298e62291d2955", [
      [ "kDCDC_PSwitchEdgeDetectInterruptEnable", "group__dcdc.html#gga0282f83bfd81de43a5298e62291d2955a7d9946db119594c23f7dd84add31eb03", null ]
    ] ],
    [ "_dcdc_pswitch_detect_event_t", "group__dcdc.html#ga4a1271adc19aa86cfed737b2630e8e37", [
      [ "kDCDC_PSwitchFallingEdgeDetectEnable", "group__dcdc.html#gga4a1271adc19aa86cfed737b2630e8e37a9f710af4e0eca67273bdfa9703e58ee3", null ],
      [ "kDCDC_PSwitchRisingEdgeDetectEnable", "group__dcdc.html#gga4a1271adc19aa86cfed737b2630e8e37af002528eb55eccd5ce7c0dc60ef0bd40", null ]
    ] ],
    [ "dcdc_work_mode_t", "group__dcdc.html#ga74a1012a6404beb48bdb7df92fdaee04", [
      [ "kDCDC_WorkInContinuousMode", "group__dcdc.html#gga74a1012a6404beb48bdb7df92fdaee04a843bfd4bb5e898809d76e7539b7cfcb2", null ],
      [ "kDCDC_WorkInPulsedMode", "group__dcdc.html#gga74a1012a6404beb48bdb7df92fdaee04a797386079871fdce5a10bd04892a74e8", null ]
    ] ],
    [ "dcdc_hysteretic_threshold_offset_value_t", "group__dcdc.html#ga12240f3abf50a8f8ca1771c0e86fb042", [
      [ "kDCDC_HystereticThresholdOffset0mV", "group__dcdc.html#gga12240f3abf50a8f8ca1771c0e86fb042a35d64a6da642526473160bdb8fed2dcd", null ],
      [ "kDCDC_HystereticThresholdOffset25mV", "group__dcdc.html#gga12240f3abf50a8f8ca1771c0e86fb042ad4cb98c6d1d574a3791c7980789f91b0", null ],
      [ "kDCDC_HystereticThresholdOffset50mV", "group__dcdc.html#gga12240f3abf50a8f8ca1771c0e86fb042a15c87c8f2f5d150779412671ab123022", null ],
      [ "kDCDC_HystereticThresholdOffset75mV", "group__dcdc.html#gga12240f3abf50a8f8ca1771c0e86fb042a0e89e70eeeb0d3715d639befbdf3b93a", null ]
    ] ],
    [ "dcdc_vbat_divider_t", "group__dcdc.html#gae33a5249be955fa16f19eaa19e4806a2", [
      [ "kDCDC_VBatVoltageDividerOff", "group__dcdc.html#ggae33a5249be955fa16f19eaa19e4806a2a7a52e47650ddae46fa5535a0329a69ef", null ],
      [ "kDCDC_VBatVoltageDivider1", "group__dcdc.html#ggae33a5249be955fa16f19eaa19e4806a2a169b35b60b39a342b5e58ebb3dad0b53", null ],
      [ "kDCDC_VBatVoltageDivider2", "group__dcdc.html#ggae33a5249be955fa16f19eaa19e4806a2aeb1f2688f838eb8b8d7838f129b53f9b", null ],
      [ "kDCDC_VBatVoltageDivider4", "group__dcdc.html#ggae33a5249be955fa16f19eaa19e4806a2abbe362c6c62dc99c740e1b5d82aaab4f", null ]
    ] ],
    [ "dcdc_clock_source_t", "group__dcdc.html#ga8035a76b241261c80ef27c1c2baf614b", [
      [ "kDCDC_ClockAutoSwitch", "group__dcdc.html#gga8035a76b241261c80ef27c1c2baf614ba598ff7757e17e93192bb02060b9e17f7", null ]
    ] ],
    [ "DCDC_Init", "group__dcdc.html#gade63427da706b28f4bc5fb3df32549fc", null ],
    [ "DCDC_Deinit", "group__dcdc.html#ga6edcfa7b584ed32abeba4aa1ead354b5", null ],
    [ "DCDC_GetStatusFlags", "group__dcdc.html#ga415e1b220f4336cc3aa0796c8be0abe9", null ],
    [ "DCDC_ClearStatusFlags", "group__dcdc.html#ga5c41627cb0eaa5eda7eeeddd95a86d8c", null ],
    [ "DCDC_EnableInterrupts", "group__dcdc.html#ga9625e26b3910f2b332114d507a154832", null ],
    [ "DCDC_DisableInterrupts", "group__dcdc.html#ga50a9c19942b1652e7cd0606aa95d8dc3", null ],
    [ "DCDC_SetPSwitchInterruptConfig", "group__dcdc.html#ga5a8cfd23e8fe46cbc7b24ec84ef797b4", null ],
    [ "DCDC_GetDefaultLowPowerConfig", "group__dcdc.html#ga07748a20a7c73fd8be68c4e9ede83535", null ],
    [ "DCDC_SetLowPowerConfig", "group__dcdc.html#ga0b9c2837458d92e1924b899f883e0c6d", null ],
    [ "DCDC_GetDefaultLoopControlConfig", "group__dcdc.html#ga18d9f9a178e395a6f8412c60f1af99eb", null ],
    [ "DCDC_SetLoopControlConfig", "group__dcdc.html#gaf0b6cb34a3c948cdc225f8e1cb055e34", null ],
    [ "DCDC_EnableXtalOKDetectionCircuit", "group__dcdc.html#gac02d97c964c5c553f69672f775d1dfe4", null ],
    [ "DCDC_EnableOutputRangeComparator", "group__dcdc.html#gad851aa409cd7a30afd61f1033650ddc4", null ],
    [ "DCDC_EnableReduceCurrent", "group__dcdc.html#ga4bb669edb1a87bd2592271dd6cdd5a2e", null ],
    [ "DCDC_SetClockSource", "group__dcdc.html#ga2fae2a0e5c2bc54cc47fa4e6cbe53f37", null ],
    [ "DCDC_SetBatteryVoltageDivider", "group__dcdc.html#ga12da1dea432b8066c582aaec19393cb8", null ],
    [ "DCDC_SetBatteryMonitorValue", "group__dcdc.html#gaf169e7eb0617e332b03ad6bbdcc19ec8", null ],
    [ "DCDC_DoSoftShutdown", "group__dcdc.html#ga23198b5b8927c7e8352f9b77b3b3f2c6", null ],
    [ "DCDC_SetUpperLimitDutyCycleBoost", "group__dcdc.html#ga86fa36dbc8d77d45056066dfba14b703", null ],
    [ "DCDC_SetUpperLimitDutyCycleBuck", "group__dcdc.html#ga72badd5375ec98dc14144a6030f6fcf0", null ],
    [ "DCDC_AdjustDutyCycleSwitchingTargetOutput", "group__dcdc.html#gaddf3e9fae21dc39f0d007c6c7aa6d537", null ],
    [ "DCDC_LockTargetVoltage", "group__dcdc.html#ga017537be6f0c499b39abf56a6df58dba", null ],
    [ "DCDC_AdjustTargetVoltage", "group__dcdc.html#ga8e157f4fa7d6389ed2bf94fce6ea8e32", null ],
    [ "DCDC_GetDefaultMinPowerDefault", "group__dcdc.html#ga80b189db8c61b0ae49d4e5cbb05a466d", null ],
    [ "DCDC_SetMinPowerConfig", "group__dcdc.html#ga02d7fde9dea0d6e88559e28f6547c0eb", null ],
    [ "DCDC_GetDefaultPulsedIntegratorConfig", "group__dcdc.html#gabd528a4d2d4939468af76a4acc89166f", null ],
    [ "DCDC_SetPulsedIntegratorConfig", "group__dcdc.html#ga5468f659d14cac617b2f345bf9a04291", null ]
];