var a00046 =
[
    [ "rtc_datetime_t", "a00046.html#a00390", [
      [ "year", "a00046.html#a9d9b3f17954a41223c07440f08edaf74", null ],
      [ "month", "a00046.html#a1621f010a30ff4e06636f08cdcb9a0b0", null ],
      [ "day", "a00046.html#a6be9f8cd55c5ccc80f6bb3a0fa1a9d0f", null ],
      [ "hour", "a00046.html#af01da84e5dd15ca3713b29083a6893d2", null ],
      [ "minute", "a00046.html#aaaeaa89246dcbf7a37b46ad854165285", null ],
      [ "second", "a00046.html#a35ffc6bb746f813d06f2814c6d5d9d83", null ]
    ] ],
    [ "rtc_config_t", "a00046.html#a00389", [
      [ "wakeupSelect", "a00046.html#ab131dda45f974f96ffc809827e76f908", null ],
      [ "updateMode", "a00046.html#a211ab51b5aa34ab309954879f0c6c963", null ],
      [ "supervisorAccess", "a00046.html#a4cdb560a5062b3869a0172e08c856b15", null ],
      [ "compensationInterval", "a00046.html#aa378ae2e19870f88b02bb37d2eb2a410", null ],
      [ "compensationTime", "a00046.html#ab7345f3eb3998ceb00a3a5262ae1ec0e", null ]
    ] ],
    [ "FSL_RTC_DRIVER_VERSION", "a00046.html#gaea33cb4ca030fb967d99babaec611ff9", null ],
    [ "rtc_interrupt_enable_t", "a00046.html#gabed8712e00907f44b420d274fd368738", [
      [ "kRTC_TimeInvalidInterruptEnable", "a00046.html#ggabed8712e00907f44b420d274fd368738ac8f25a2201dc4dd638c28d049d7d0913", null ],
      [ "kRTC_TimeOverflowInterruptEnable", "a00046.html#ggabed8712e00907f44b420d274fd368738a6a518f6ebb6b1c04ae42ebc5003ea6be", null ],
      [ "kRTC_AlarmInterruptEnable", "a00046.html#ggabed8712e00907f44b420d274fd368738abd7ca2093bc2df536c828d8c8ea3dab4", null ],
      [ "kRTC_SecondsInterruptEnable", "a00046.html#ggabed8712e00907f44b420d274fd368738afa4307c8ee86fd861d6ba4f1bc43c782", null ],
      [ "kRTC_MonotonicOverflowInterruptEnable", "a00046.html#ggabed8712e00907f44b420d274fd368738aeceb89fcf0536d69d22d8bbb1f6a45e4", null ]
    ] ],
    [ "rtc_status_flags_t", "a00046.html#gaa5edfafe0da586a9411fe4bafe32d9c5", [
      [ "kRTC_TimeInvalidFlag", "a00046.html#ggaa5edfafe0da586a9411fe4bafe32d9c5a6af0515ca5fae5c36599d2104f9c8fc7", null ],
      [ "kRTC_TimeOverflowFlag", "a00046.html#ggaa5edfafe0da586a9411fe4bafe32d9c5a9223509757776dfaf2ea741afef1bf90", null ],
      [ "kRTC_AlarmFlag", "a00046.html#ggaa5edfafe0da586a9411fe4bafe32d9c5a12101de586ebe90b1662305f467d88b2", null ],
      [ "kRTC_MonotonicOverflowFlag", "a00046.html#ggaa5edfafe0da586a9411fe4bafe32d9c5a7d00f2b4175b2ec2513fd93b0859facb", null ]
    ] ],
    [ "rtc_osc_cap_load_t", "a00046.html#gaba10feedc5f3b239c134547709c82794", [
      [ "kRTC_Capacitor_2p", "a00046.html#ggaba10feedc5f3b239c134547709c82794af1ad702ff4f5fe2f7943928c437cdbca", null ],
      [ "kRTC_Capacitor_4p", "a00046.html#ggaba10feedc5f3b239c134547709c82794a1587d2d62aeaa6ff452ae2cc4dd58382", null ],
      [ "kRTC_Capacitor_8p", "a00046.html#ggaba10feedc5f3b239c134547709c82794a8d37764da4f07e501abae2c62eea8b27", null ],
      [ "kRTC_Capacitor_16p", "a00046.html#ggaba10feedc5f3b239c134547709c82794a6c70fa6e1d1a3de36abeca466bc81c84", null ]
    ] ],
    [ "RTC_Init", "a00046.html#ga9d13df422bcf270898ad8ce6f6e158fd", null ],
    [ "RTC_Deinit", "a00046.html#ga0f365b0df0c2e61b07a7e178bd0f1581", null ],
    [ "RTC_GetDefaultConfig", "a00046.html#ga27bf0709ea4fec76bc016f19bb729218", null ],
    [ "RTC_SetDatetime", "a00046.html#ga5ddf5a972712795ee53468c3cc35b2de", null ],
    [ "RTC_GetDatetime", "a00046.html#gabd709e50b2b6129bd429352282890720", null ],
    [ "RTC_SetAlarm", "a00046.html#gab2d6ccb1f963f5c38166bfe0a3e90955", null ],
    [ "RTC_GetAlarm", "a00046.html#ga197e1135863c96d064eb1828a32487b2", null ],
    [ "RTC_EnableInterrupts", "a00046.html#gafd4fb8344c96573d14cc5de6d15ba7a6", null ],
    [ "RTC_DisableInterrupts", "a00046.html#ga6f94f60e53e4ae9aef8747fce94bde82", null ],
    [ "RTC_GetEnabledInterrupts", "a00046.html#gaa47a4c1d6ebb5f75404998944b1b3d12", null ],
    [ "RTC_GetStatusFlags", "a00046.html#gaced367641064bb3751c23eab56bb2760", null ],
    [ "RTC_ClearStatusFlags", "a00046.html#ga735b784e8d4027022e8f0370430e2837", null ],
    [ "RTC_SetClockSource", "a00046.html#gaf7009d11f7eeed9f5bc0a2c3249c0ea8", null ],
    [ "RTC_GetTamperTimeSeconds", "a00046.html#ga74278c0a2320d06c2f616a198608f594", null ],
    [ "RTC_StartTimer", "a00046.html#gae8e7b09ad38776c0e360debf20145d78", null ],
    [ "RTC_StopTimer", "a00046.html#ga57f9d747f6972fd0a20e6ddccb42f084", null ],
    [ "RTC_SetOscCapLoad", "a00046.html#gab8bc74661dc39d0ddd0ccc517a27f847", null ],
    [ "RTC_Reset", "a00046.html#ga5bf2f1b9b635918d74c5f4bb5b027d6c", null ],
    [ "RTC_GetMonotonicCounter", "a00046.html#gad467a904a897a1105702244667cc89b8", null ],
    [ "RTC_SetMonotonicCounter", "a00046.html#ga124113b579e89b0d5bd15d9ecca774ee", null ],
    [ "RTC_IncrementMonotonicCounter", "a00046.html#gab07f4f8da8a52e8d7afb358f8117dad7", null ],
    [ "RTC_EnableWakeUpPin", "a00046.html#gacb98d30e798458d683372652b137019f", null ]
];