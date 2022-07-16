var a00048 =
[
    [ "lpadc_config_t", "a00048.html#a00392", [
      [ "enableInDozeMode", "a00048.html#ac47a5a50ba40ca3b021b4be239a048f3", null ],
      [ "enableAnalogPreliminary", "a00048.html#ab62dd78747ca5df971c30ad59d0127ba", null ],
      [ "powerUpDelay", "a00048.html#a05885e7359e788bfee8bee8b1b7e17ac", null ],
      [ "referenceVoltageSource", "a00048.html#af2a2e2e253efb0a783435fe171091b3f", null ],
      [ "powerLevelMode", "a00048.html#aa95f676c2ce209a53e411045a118b7d7", null ],
      [ "triggerPriorityPolicy", "a00048.html#a6790dea9cdf41dbe47a312f46d046d39", null ],
      [ "enableConvPause", "a00048.html#a54cc3c1d27305b3916a0ce2fc1f86af0", null ],
      [ "convPauseDelay", "a00048.html#a95cd7bea05bf46e0f5444762aba38fbe", null ],
      [ "FIFO0Watermark", "a00048.html#a5ca017520f60b0ece2907f4797ef1b2b", null ],
      [ "FIFO1Watermark", "a00048.html#af849ddcf7997df4d3481225972626472", null ]
    ] ],
    [ "lpadc_conv_command_config_t", "a00048.html#a00393", [
      [ "sampleScaleMode", "a00048.html#aa8a989a84f3f494e8a222a14e4fa2740", null ],
      [ "sampleChannelMode", "a00048.html#ad83d6cc054bd648551aa7a875e5e8a58", null ],
      [ "channelNumber", "a00048.html#a34b3392e374a496930b09d8c5a410d7b", null ],
      [ "chainedNextCommandNumber", "a00048.html#adcf04257bab0b975b7667a25bf4cbcdf", null ],
      [ "enableAutoChannelIncrement", "a00048.html#a66e3347e2c2f96cadd5805bca6409646", null ],
      [ "loopCount", "a00048.html#a7ce69c5cf297a804b5510d779036c867", null ],
      [ "hardwareAverageMode", "a00048.html#a1c44cc7c4a60d180e6a9aae0501e8752", null ],
      [ "sampleTimeMode", "a00048.html#a24deb3ab5d573ce3b17ed48987350b65", null ],
      [ "hardwareCompareMode", "a00048.html#a5bb19c6af0abf963655462a350c53ce4", null ],
      [ "hardwareCompareValueHigh", "a00048.html#a4bc748a87503d1d30fce8652296a21a1", null ],
      [ "hardwareCompareValueLow", "a00048.html#aded6fd827d36d7f20693cf5f361ec57e", null ],
      [ "enableWaitTrigger", "a00048.html#aaebaf295fa7a725ae26273e4a79735a8", null ]
    ] ],
    [ "lpadc_conv_trigger_config_t", "a00048.html#a00395", [
      [ "targetCommandId", "a00048.html#ae1c77e5afcb5c9d2b80ac92cc5de597e", null ],
      [ "delayPower", "a00048.html#a33fd3b422ca9c26c1eb915ef8fdc3c3e", null ],
      [ "priority", "a00048.html#a2e4d3a907597cd2c8c712ae967c5ac2f", null ],
      [ "enableHardwareTrigger", "a00048.html#a32a44ab0d65a0666b659027b93738c95", null ]
    ] ],
    [ "lpadc_conv_result_t", "a00048.html#a00394", [
      [ "commandIdSource", "a00048.html#a7ed6dcbf44ad017014525335013c817b", null ],
      [ "loopCountIndex", "a00048.html#a87e1ee666c960928797ca574f1bcae1b", null ],
      [ "triggerIdSource", "a00048.html#ad3b63c03e1c136e4293dd82a443f1de2", null ],
      [ "convValue", "a00048.html#a4add4f13e761e8048d4434eb195f44d5", null ]
    ] ],
    [ "FSL_LPADC_DRIVER_VERSION", "a00048.html#gab8a795637177dc9973e0fd2f5b29b6c3", null ],
    [ "LPADC_GET_ACTIVE_COMMAND_STATUS", "a00048.html#gae305eca1c95975e49ea88d0d39aeb3e0", null ],
    [ "LPADC_GET_ACTIVE_TRIGGER_STATUE", "a00048.html#ga48a0c8b6bac1e45cc7bdc93ccc288786", null ],
    [ "_lpadc_status_flags", "a00048.html#gaf40ddd93026be88551f23b9428e98e22", [
      [ "kLPADC_ResultFIFO0OverflowFlag", "a00048.html#ggaf40ddd93026be88551f23b9428e98e22a0bc43d455f0c92b0455d392b02a4581e", null ],
      [ "kLPADC_ResultFIFO0ReadyFlag", "a00048.html#ggaf40ddd93026be88551f23b9428e98e22a16e8001de19be822847a36c2d6e6146c", null ],
      [ "kLPADC_ResultFIFO1OverflowFlag", "a00048.html#ggaf40ddd93026be88551f23b9428e98e22a98008f6e871aeed2f717710483f09548", null ],
      [ "kLPADC_ResultFIFO1ReadyFlag", "a00048.html#ggaf40ddd93026be88551f23b9428e98e22a17cf355f38f096341993469ef272c9ec", null ]
    ] ],
    [ "_lpadc_interrupt_enable", "a00048.html#gaeba33be014c52b093d5e7815dcf8e6b3", [
      [ "kLPADC_ResultFIFO0OverflowInterruptEnable", "a00048.html#ggaeba33be014c52b093d5e7815dcf8e6b3a8591fb75b5c63ceab76e5766ef49363e", null ],
      [ "kLPADC_FIFO0WatermarkInterruptEnable", "a00048.html#ggaeba33be014c52b093d5e7815dcf8e6b3a2bab7f05a3af09e6f8b90a6ca39fc2b6", null ],
      [ "kLPADC_ResultFIFO1OverflowInterruptEnable", "a00048.html#ggaeba33be014c52b093d5e7815dcf8e6b3a15655b1aa12ffe21d7c02b8ee00add82", null ],
      [ "kLPADC_FIFO1WatermarkInterruptEnable", "a00048.html#ggaeba33be014c52b093d5e7815dcf8e6b3ad9cb8f22e447f38f81ad6021bc8b4a3c", null ]
    ] ],
    [ "lpadc_sample_scale_mode_t", "a00048.html#gaa0d71fb3a1bd431af8cf5aacc2e8fc6c", [
      [ "kLPADC_SamplePartScale", "a00048.html#ggaa0d71fb3a1bd431af8cf5aacc2e8fc6ca2d5ef37dc371402d7b50992f6724f2fd", null ],
      [ "kLPADC_SampleFullScale", "a00048.html#ggaa0d71fb3a1bd431af8cf5aacc2e8fc6cacf9a7acbde288c4de0a60d0018457721", null ]
    ] ],
    [ "lpadc_sample_channel_mode_t", "a00048.html#ga03c78bd98bf88df854e5a67e86521e7a", [
      [ "kLPADC_SampleChannelSingleEndSideA", "a00048.html#gga03c78bd98bf88df854e5a67e86521e7aae511411bd331e3049a5c9583163e4fe2", null ],
      [ "kLPADC_SampleChannelSingleEndSideB", "a00048.html#gga03c78bd98bf88df854e5a67e86521e7aab477a25f4b1cef6b47115b7bb4b9583c", null ],
      [ "kLPADC_SampleChannelDiffBothSideAB", "a00048.html#gga03c78bd98bf88df854e5a67e86521e7aa44153790e20e1d99c9ab49165b27db98", null ],
      [ "kLPADC_SampleChannelDiffBothSideBA", "a00048.html#gga03c78bd98bf88df854e5a67e86521e7aa29943cdedc87ec9e31f6e4f5b08452c8", null ]
    ] ],
    [ "lpadc_hardware_average_mode_t", "a00048.html#gabb529e856c73d606ce37f7da01a0f6f3", [
      [ "kLPADC_HardwareAverageCount1", "a00048.html#ggabb529e856c73d606ce37f7da01a0f6f3adeae8ae097545482c1942926119d968c", null ],
      [ "kLPADC_HardwareAverageCount2", "a00048.html#ggabb529e856c73d606ce37f7da01a0f6f3a03e731d3d2e024623fd22145c484b797", null ],
      [ "kLPADC_HardwareAverageCount4", "a00048.html#ggabb529e856c73d606ce37f7da01a0f6f3a0eda65451db63f4a0c1d154f851f9353", null ],
      [ "kLPADC_HardwareAverageCount8", "a00048.html#ggabb529e856c73d606ce37f7da01a0f6f3a3cc487e2f5720a5c45318f69028c32cc", null ],
      [ "kLPADC_HardwareAverageCount16", "a00048.html#ggabb529e856c73d606ce37f7da01a0f6f3ad5fece9b31b47f741c413fe7445df855", null ],
      [ "kLPADC_HardwareAverageCount32", "a00048.html#ggabb529e856c73d606ce37f7da01a0f6f3a4cfb338bc49982bc7e58a7f0949bf425", null ],
      [ "kLPADC_HardwareAverageCount64", "a00048.html#ggabb529e856c73d606ce37f7da01a0f6f3a081530884310024c8d2750735b6295ea", null ],
      [ "kLPADC_HardwareAverageCount128", "a00048.html#ggabb529e856c73d606ce37f7da01a0f6f3a4f86910a33fa9874108753b6de1947ce", null ]
    ] ],
    [ "lpadc_sample_time_mode_t", "a00048.html#gac70e2c24ba9ef059ee2487d821136e85", [
      [ "kLPADC_SampleTimeADCK3", "a00048.html#ggac70e2c24ba9ef059ee2487d821136e85abfd94e19de3d57b94fae9baded5804db", null ],
      [ "kLPADC_SampleTimeADCK5", "a00048.html#ggac70e2c24ba9ef059ee2487d821136e85a441c59d3df7a1ad5b64416e6a08dd246", null ],
      [ "kLPADC_SampleTimeADCK7", "a00048.html#ggac70e2c24ba9ef059ee2487d821136e85aa2a17502f50e36de117b337994c21dc2", null ],
      [ "kLPADC_SampleTimeADCK11", "a00048.html#ggac70e2c24ba9ef059ee2487d821136e85a4a980813baf6845d528db2a265475905", null ],
      [ "kLPADC_SampleTimeADCK19", "a00048.html#ggac70e2c24ba9ef059ee2487d821136e85a7f5bb84adb8783fd6fa8d3ce5ed31f3a", null ],
      [ "kLPADC_SampleTimeADCK35", "a00048.html#ggac70e2c24ba9ef059ee2487d821136e85a9d59f3e6e9335223746533051f83a20b", null ],
      [ "kLPADC_SampleTimeADCK67", "a00048.html#ggac70e2c24ba9ef059ee2487d821136e85a653e653b0ee479f6b42ff4b8ffea49ac", null ],
      [ "kLPADC_SampleTimeADCK131", "a00048.html#ggac70e2c24ba9ef059ee2487d821136e85ae327cbad1ded4882cbcd53ad5e6c2262", null ]
    ] ],
    [ "lpadc_hardware_compare_mode_t", "a00048.html#gad7a09557254487374b1b125f2146ba22", [
      [ "kLPADC_HardwareCompareDisabled", "a00048.html#ggad7a09557254487374b1b125f2146ba22ab205c4a4aa1cd79f38be81e6469f2fd2", null ],
      [ "kLPADC_HardwareCompareStoreOnTrue", "a00048.html#ggad7a09557254487374b1b125f2146ba22a0e30824c70217e386a1dfbaaecbd7b67", null ],
      [ "kLPADC_HardwareCompareRepeatUntilTrue", "a00048.html#ggad7a09557254487374b1b125f2146ba22a11bc71ef00c5e95a18431947fe0362e1", null ]
    ] ],
    [ "lpadc_reference_voltage_source_t", "a00048.html#gac8bce529fe3a77c5a22136eac3c7d626", [
      [ "kLPADC_ReferenceVoltageAlt1", "a00048.html#ggac8bce529fe3a77c5a22136eac3c7d626a8d40316bc847f29e588ff1a50775ab11", null ],
      [ "kLPADC_ReferenceVoltageAlt2", "a00048.html#ggac8bce529fe3a77c5a22136eac3c7d626a7f45d8cf9f4a252f22d52b8a82db304a", null ],
      [ "kLPADC_ReferenceVoltageAlt3", "a00048.html#ggac8bce529fe3a77c5a22136eac3c7d626a8c41f94955b5b1141956d61fc2feb44c", null ]
    ] ],
    [ "lpadc_power_level_mode_t", "a00048.html#gab930081517abc866eaa456e58fd365aa", [
      [ "kLPADC_PowerLevelAlt1", "a00048.html#ggab930081517abc866eaa456e58fd365aaac88970640320826c36101d8e7fa5a9c9", null ],
      [ "kLPADC_PowerLevelAlt2", "a00048.html#ggab930081517abc866eaa456e58fd365aaaca71931306fe195df044d6a889639919", null ],
      [ "kLPADC_PowerLevelAlt3", "a00048.html#ggab930081517abc866eaa456e58fd365aaaed7771e49eeec9fee65726c600956d6d", null ],
      [ "kLPADC_PowerLevelAlt4", "a00048.html#ggab930081517abc866eaa456e58fd365aaa2f34a351174cf34ee124ccfc7f2bf9d2", null ]
    ] ],
    [ "lpadc_trigger_priority_policy_t", "a00048.html#ga4c65872fd8d9cca041a6d668473537c3", [
      [ "kLPADC_TriggerPriorityPreemptImmediately", "a00048.html#gga4c65872fd8d9cca041a6d668473537c3a0fec3ebd14c77fc5a1711cb3ed7a2ab5", null ],
      [ "kLPADC_TriggerPriorityPreemptSoftly", "a00048.html#gga4c65872fd8d9cca041a6d668473537c3a406eb5620e19a0cad27c10211c548a46", null ],
      [ "kLPADC_TriggerPriorityPreemptSubsequently", "a00048.html#gga4c65872fd8d9cca041a6d668473537c3a216e6ba47136231c657b2c0353558fb0", null ]
    ] ],
    [ "LPADC_Init", "a00048.html#gaf13dbab92e7a26afb07b3bb2cbaab793", null ],
    [ "LPADC_GetDefaultConfig", "a00048.html#ga94c79af85a28c45bc642226292b14d1c", null ],
    [ "LPADC_Deinit", "a00048.html#gabc10ae5663788ea5c7f77e2f3517a677", null ],
    [ "LPADC_Enable", "a00048.html#gaf76592d9f392cd30b738f599b2e71344", null ],
    [ "LPADC_DoResetFIFO0", "a00048.html#ga6612a1652b3be8d03305eb0d9b8c34c1", null ],
    [ "LPADC_DoResetFIFO1", "a00048.html#ga711046d11ca4de396fc2216846517a57", null ],
    [ "LPADC_DoResetConfig", "a00048.html#ga3d6911f2f9ad7fd2fdf93c3b5d1e80f7", null ],
    [ "LPADC_GetStatusFlags", "a00048.html#gaa2793911f11091179bfcd8e9b1860300", null ],
    [ "LPADC_ClearStatusFlags", "a00048.html#ga77ba72fad1e0f84b42809da6918e6532", null ],
    [ "LPADC_EnableInterrupts", "a00048.html#ga2c147558843514556795fd52f86aca56", null ],
    [ "LPADC_DisableInterrupts", "a00048.html#ga5c633af2aeab39c215d3b5aa6172ccb7", null ],
    [ "LPADC_EnableFIFO0WatermarkDMA", "a00048.html#ga3ebd7521d7dbdc3ed1d3d733a9f92826", null ],
    [ "LPADC_EnableFIFO1WatermarkDMA", "a00048.html#gac2fefd17278fab79ee80e6f50ddc8335", null ],
    [ "LPADC_GetConvResultCount", "a00048.html#ga1d86d3df8b071435e2c6fd4c140274a8", null ],
    [ "LPADC_GetConvResult", "a00048.html#ga4a74fc31f174364f81ece1c0dc775dae", null ],
    [ "LPADC_SetConvTriggerConfig", "a00048.html#ga67927722ab525eda9e31d1f7554f02a4", null ],
    [ "LPADC_GetDefaultConvTriggerConfig", "a00048.html#ga44e286bc27b1e8fee3f4796ca9196a27", null ],
    [ "LPADC_DoSoftwareTrigger", "a00048.html#gacaef94ac48a3e0a34f9235f88767b7a3", null ],
    [ "LPADC_SetConvCommandConfig", "a00048.html#ga500d8b7b0323136353e10fdfad2e40c7", null ],
    [ "LPADC_GetDefaultConvCommandConfig", "a00048.html#gaa3f7fd90bdbd65fbc4e5b88e904173f9", null ]
];