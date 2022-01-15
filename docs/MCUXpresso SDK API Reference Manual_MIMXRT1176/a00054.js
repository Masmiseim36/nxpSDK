var a00054 =
[
    [ "lpadc_config_t", "a00054.html#a00566", [
      [ "enableInDozeMode", "a00054.html#ac47a5a50ba40ca3b021b4be239a048f3", null ],
      [ "enableAnalogPreliminary", "a00054.html#ab62dd78747ca5df971c30ad59d0127ba", null ],
      [ "powerUpDelay", "a00054.html#a05885e7359e788bfee8bee8b1b7e17ac", null ],
      [ "referenceVoltageSource", "a00054.html#af2a2e2e253efb0a783435fe171091b3f", null ],
      [ "powerLevelMode", "a00054.html#aa95f676c2ce209a53e411045a118b7d7", null ],
      [ "triggerPriorityPolicy", "a00054.html#a6790dea9cdf41dbe47a312f46d046d39", null ],
      [ "enableConvPause", "a00054.html#a54cc3c1d27305b3916a0ce2fc1f86af0", null ],
      [ "convPauseDelay", "a00054.html#a95cd7bea05bf46e0f5444762aba38fbe", null ],
      [ "FIFOWatermark", "a00054.html#aeb5cae2223bf1d49e13332308ceae79b", null ]
    ] ],
    [ "lpadc_conv_command_config_t", "a00054.html#a00567", [
      [ "sampleScaleMode", "a00054.html#aa8a989a84f3f494e8a222a14e4fa2740", null ],
      [ "sampleChannelMode", "a00054.html#ad83d6cc054bd648551aa7a875e5e8a58", null ],
      [ "channelNumber", "a00054.html#a34b3392e374a496930b09d8c5a410d7b", null ],
      [ "chainedNextCommandNumber", "a00054.html#adcf04257bab0b975b7667a25bf4cbcdf", null ],
      [ "enableAutoChannelIncrement", "a00054.html#a66e3347e2c2f96cadd5805bca6409646", null ],
      [ "loopCount", "a00054.html#a7ce69c5cf297a804b5510d779036c867", null ],
      [ "hardwareAverageMode", "a00054.html#a1c44cc7c4a60d180e6a9aae0501e8752", null ],
      [ "sampleTimeMode", "a00054.html#a24deb3ab5d573ce3b17ed48987350b65", null ],
      [ "hardwareCompareMode", "a00054.html#a5bb19c6af0abf963655462a350c53ce4", null ],
      [ "hardwareCompareValueHigh", "a00054.html#a4bc748a87503d1d30fce8652296a21a1", null ],
      [ "hardwareCompareValueLow", "a00054.html#aded6fd827d36d7f20693cf5f361ec57e", null ]
    ] ],
    [ "lpadc_conv_trigger_config_t", "a00054.html#a00569", [
      [ "targetCommandId", "a00054.html#ae1c77e5afcb5c9d2b80ac92cc5de597e", null ],
      [ "delayPower", "a00054.html#a33fd3b422ca9c26c1eb915ef8fdc3c3e", null ],
      [ "priority", "a00054.html#a2e4d3a907597cd2c8c712ae967c5ac2f", null ],
      [ "enableHardwareTrigger", "a00054.html#a32a44ab0d65a0666b659027b93738c95", null ]
    ] ],
    [ "lpadc_conv_result_t", "a00054.html#a00568", [
      [ "commandIdSource", "a00054.html#a7ed6dcbf44ad017014525335013c817b", null ],
      [ "loopCountIndex", "a00054.html#a87e1ee666c960928797ca574f1bcae1b", null ],
      [ "triggerIdSource", "a00054.html#ad3b63c03e1c136e4293dd82a443f1de2", null ],
      [ "convValue", "a00054.html#a4add4f13e761e8048d4434eb195f44d5", null ]
    ] ],
    [ "FSL_LPADC_DRIVER_VERSION", "a00054.html#gab8a795637177dc9973e0fd2f5b29b6c3", null ],
    [ "LPADC_GET_ACTIVE_COMMAND_STATUS", "a00054.html#gae305eca1c95975e49ea88d0d39aeb3e0", null ],
    [ "LPADC_GET_ACTIVE_TRIGGER_STATUE", "a00054.html#ga48a0c8b6bac1e45cc7bdc93ccc288786", null ],
    [ "_lpadc_status_flags", "a00054.html#gaf40ddd93026be88551f23b9428e98e22", [
      [ "kLPADC_ResultFIFOOverflowFlag", "a00054.html#ggaf40ddd93026be88551f23b9428e98e22aa52211bffea2def3062daff07532cb28", null ],
      [ "kLPADC_ResultFIFOReadyFlag", "a00054.html#ggaf40ddd93026be88551f23b9428e98e22a7bd75ce7f5f38970c10f03c77075fc5d", null ]
    ] ],
    [ "_lpadc_interrupt_enable", "a00054.html#gaeba33be014c52b093d5e7815dcf8e6b3", [
      [ "kLPADC_ResultFIFOOverflowInterruptEnable", "a00054.html#ggaeba33be014c52b093d5e7815dcf8e6b3a487f293b5443e2b9c39412f4476f7638", null ],
      [ "kLPADC_FIFOWatermarkInterruptEnable", "a00054.html#ggaeba33be014c52b093d5e7815dcf8e6b3a90f6e2b2819d89b685281a440b10b8b5", null ]
    ] ],
    [ "lpadc_sample_scale_mode_t", "a00054.html#gaa0d71fb3a1bd431af8cf5aacc2e8fc6c", [
      [ "kLPADC_SamplePartScale", "a00054.html#ggaa0d71fb3a1bd431af8cf5aacc2e8fc6ca2d5ef37dc371402d7b50992f6724f2fd", null ],
      [ "kLPADC_SampleFullScale", "a00054.html#ggaa0d71fb3a1bd431af8cf5aacc2e8fc6cacf9a7acbde288c4de0a60d0018457721", null ]
    ] ],
    [ "lpadc_sample_channel_mode_t", "a00054.html#ga03c78bd98bf88df854e5a67e86521e7a", [
      [ "kLPADC_SampleChannelSingleEndSideA", "a00054.html#gga03c78bd98bf88df854e5a67e86521e7aae511411bd331e3049a5c9583163e4fe2", null ],
      [ "kLPADC_SampleChannelSingleEndSideB", "a00054.html#gga03c78bd98bf88df854e5a67e86521e7aab477a25f4b1cef6b47115b7bb4b9583c", null ],
      [ "kLPADC_SampleChannelDiffBothSideAB", "a00054.html#gga03c78bd98bf88df854e5a67e86521e7aa44153790e20e1d99c9ab49165b27db98", null ],
      [ "kLPADC_SampleChannelDiffBothSideBA", "a00054.html#gga03c78bd98bf88df854e5a67e86521e7aa29943cdedc87ec9e31f6e4f5b08452c8", null ]
    ] ],
    [ "lpadc_hardware_average_mode_t", "a00054.html#gabb529e856c73d606ce37f7da01a0f6f3", [
      [ "kLPADC_HardwareAverageCount1", "a00054.html#ggabb529e856c73d606ce37f7da01a0f6f3adeae8ae097545482c1942926119d968c", null ],
      [ "kLPADC_HardwareAverageCount2", "a00054.html#ggabb529e856c73d606ce37f7da01a0f6f3a03e731d3d2e024623fd22145c484b797", null ],
      [ "kLPADC_HardwareAverageCount4", "a00054.html#ggabb529e856c73d606ce37f7da01a0f6f3a0eda65451db63f4a0c1d154f851f9353", null ],
      [ "kLPADC_HardwareAverageCount8", "a00054.html#ggabb529e856c73d606ce37f7da01a0f6f3a3cc487e2f5720a5c45318f69028c32cc", null ],
      [ "kLPADC_HardwareAverageCount16", "a00054.html#ggabb529e856c73d606ce37f7da01a0f6f3ad5fece9b31b47f741c413fe7445df855", null ],
      [ "kLPADC_HardwareAverageCount32", "a00054.html#ggabb529e856c73d606ce37f7da01a0f6f3a4cfb338bc49982bc7e58a7f0949bf425", null ],
      [ "kLPADC_HardwareAverageCount64", "a00054.html#ggabb529e856c73d606ce37f7da01a0f6f3a081530884310024c8d2750735b6295ea", null ],
      [ "kLPADC_HardwareAverageCount128", "a00054.html#ggabb529e856c73d606ce37f7da01a0f6f3a4f86910a33fa9874108753b6de1947ce", null ]
    ] ],
    [ "lpadc_sample_time_mode_t", "a00054.html#gac70e2c24ba9ef059ee2487d821136e85", [
      [ "kLPADC_SampleTimeADCK3", "a00054.html#ggac70e2c24ba9ef059ee2487d821136e85abfd94e19de3d57b94fae9baded5804db", null ],
      [ "kLPADC_SampleTimeADCK5", "a00054.html#ggac70e2c24ba9ef059ee2487d821136e85a441c59d3df7a1ad5b64416e6a08dd246", null ],
      [ "kLPADC_SampleTimeADCK7", "a00054.html#ggac70e2c24ba9ef059ee2487d821136e85aa2a17502f50e36de117b337994c21dc2", null ],
      [ "kLPADC_SampleTimeADCK11", "a00054.html#ggac70e2c24ba9ef059ee2487d821136e85a4a980813baf6845d528db2a265475905", null ],
      [ "kLPADC_SampleTimeADCK19", "a00054.html#ggac70e2c24ba9ef059ee2487d821136e85a7f5bb84adb8783fd6fa8d3ce5ed31f3a", null ],
      [ "kLPADC_SampleTimeADCK35", "a00054.html#ggac70e2c24ba9ef059ee2487d821136e85a9d59f3e6e9335223746533051f83a20b", null ],
      [ "kLPADC_SampleTimeADCK67", "a00054.html#ggac70e2c24ba9ef059ee2487d821136e85a653e653b0ee479f6b42ff4b8ffea49ac", null ],
      [ "kLPADC_SampleTimeADCK131", "a00054.html#ggac70e2c24ba9ef059ee2487d821136e85ae327cbad1ded4882cbcd53ad5e6c2262", null ]
    ] ],
    [ "lpadc_hardware_compare_mode_t", "a00054.html#gad7a09557254487374b1b125f2146ba22", [
      [ "kLPADC_HardwareCompareDisabled", "a00054.html#ggad7a09557254487374b1b125f2146ba22ab205c4a4aa1cd79f38be81e6469f2fd2", null ],
      [ "kLPADC_HardwareCompareStoreOnTrue", "a00054.html#ggad7a09557254487374b1b125f2146ba22a0e30824c70217e386a1dfbaaecbd7b67", null ],
      [ "kLPADC_HardwareCompareRepeatUntilTrue", "a00054.html#ggad7a09557254487374b1b125f2146ba22a11bc71ef00c5e95a18431947fe0362e1", null ]
    ] ],
    [ "lpadc_reference_voltage_source_t", "a00054.html#gac8bce529fe3a77c5a22136eac3c7d626", [
      [ "kLPADC_ReferenceVoltageAlt1", "a00054.html#ggac8bce529fe3a77c5a22136eac3c7d626a8d40316bc847f29e588ff1a50775ab11", null ],
      [ "kLPADC_ReferenceVoltageAlt2", "a00054.html#ggac8bce529fe3a77c5a22136eac3c7d626a7f45d8cf9f4a252f22d52b8a82db304a", null ],
      [ "kLPADC_ReferenceVoltageAlt3", "a00054.html#ggac8bce529fe3a77c5a22136eac3c7d626a8c41f94955b5b1141956d61fc2feb44c", null ]
    ] ],
    [ "lpadc_power_level_mode_t", "a00054.html#gab930081517abc866eaa456e58fd365aa", [
      [ "kLPADC_PowerLevelAlt1", "a00054.html#ggab930081517abc866eaa456e58fd365aaac88970640320826c36101d8e7fa5a9c9", null ],
      [ "kLPADC_PowerLevelAlt2", "a00054.html#ggab930081517abc866eaa456e58fd365aaaca71931306fe195df044d6a889639919", null ],
      [ "kLPADC_PowerLevelAlt3", "a00054.html#ggab930081517abc866eaa456e58fd365aaaed7771e49eeec9fee65726c600956d6d", null ],
      [ "kLPADC_PowerLevelAlt4", "a00054.html#ggab930081517abc866eaa456e58fd365aaa2f34a351174cf34ee124ccfc7f2bf9d2", null ]
    ] ],
    [ "lpadc_trigger_priority_policy_t", "a00054.html#ga4c65872fd8d9cca041a6d668473537c3", [
      [ "kLPADC_TriggerPriorityPreemptImmediately", "a00054.html#gga4c65872fd8d9cca041a6d668473537c3a0fec3ebd14c77fc5a1711cb3ed7a2ab5", null ],
      [ "kLPADC_TriggerPriorityPreemptSoftly", "a00054.html#gga4c65872fd8d9cca041a6d668473537c3a406eb5620e19a0cad27c10211c548a46", null ],
      [ "kLPADC_TriggerPriorityPreemptSubsequently", "a00054.html#gga4c65872fd8d9cca041a6d668473537c3a216e6ba47136231c657b2c0353558fb0", null ]
    ] ],
    [ "LPADC_Init", "a00054.html#gaf13dbab92e7a26afb07b3bb2cbaab793", null ],
    [ "LPADC_GetDefaultConfig", "a00054.html#ga94c79af85a28c45bc642226292b14d1c", null ],
    [ "LPADC_Deinit", "a00054.html#gabc10ae5663788ea5c7f77e2f3517a677", null ],
    [ "LPADC_Enable", "a00054.html#gaf76592d9f392cd30b738f599b2e71344", null ],
    [ "LPADC_DoResetFIFO", "a00054.html#gab6e4218842365ee6c620cfc59db213c4", null ],
    [ "LPADC_DoResetConfig", "a00054.html#ga3d6911f2f9ad7fd2fdf93c3b5d1e80f7", null ],
    [ "LPADC_GetStatusFlags", "a00054.html#gaa2793911f11091179bfcd8e9b1860300", null ],
    [ "LPADC_ClearStatusFlags", "a00054.html#ga77ba72fad1e0f84b42809da6918e6532", null ],
    [ "LPADC_EnableInterrupts", "a00054.html#ga2c147558843514556795fd52f86aca56", null ],
    [ "LPADC_DisableInterrupts", "a00054.html#ga5c633af2aeab39c215d3b5aa6172ccb7", null ],
    [ "LPADC_EnableFIFOWatermarkDMA", "a00054.html#gacaaa33bc4d9cfd8fad4fac8f1475d97c", null ],
    [ "LPADC_GetConvResultCount", "a00054.html#ga51b39db4ab530f900aa3a88a3047daf3", null ],
    [ "LPADC_GetConvResult", "a00054.html#ga1862883b3c4ce8365ad1f1a6c64043cc", null ],
    [ "LPADC_SetConvTriggerConfig", "a00054.html#ga67927722ab525eda9e31d1f7554f02a4", null ],
    [ "LPADC_GetDefaultConvTriggerConfig", "a00054.html#ga44e286bc27b1e8fee3f4796ca9196a27", null ],
    [ "LPADC_DoSoftwareTrigger", "a00054.html#gacaef94ac48a3e0a34f9235f88767b7a3", null ],
    [ "LPADC_SetConvCommandConfig", "a00054.html#ga500d8b7b0323136353e10fdfad2e40c7", null ],
    [ "LPADC_GetDefaultConvCommandConfig", "a00054.html#gaa3f7fd90bdbd65fbc4e5b88e904173f9", null ]
];