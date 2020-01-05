var group__rtc =
[
    [ "rtc_datetime_t", "group__rtc.html#structrtc__datetime__t", [
      [ "year", "group__rtc.html#a9d9b3f17954a41223c07440f08edaf74", null ],
      [ "month", "group__rtc.html#a1621f010a30ff4e06636f08cdcb9a0b0", null ],
      [ "day", "group__rtc.html#a6be9f8cd55c5ccc80f6bb3a0fa1a9d0f", null ],
      [ "hour", "group__rtc.html#af01da84e5dd15ca3713b29083a6893d2", null ],
      [ "minute", "group__rtc.html#aaaeaa89246dcbf7a37b46ad854165285", null ],
      [ "second", "group__rtc.html#a35ffc6bb746f813d06f2814c6d5d9d83", null ]
    ] ],
    [ "rtc_config_t", "group__rtc.html#structrtc__config__t", [
      [ "wakeupSelect", "group__rtc.html#ab131dda45f974f96ffc809827e76f908", null ],
      [ "updateMode", "group__rtc.html#a211ab51b5aa34ab309954879f0c6c963", null ],
      [ "supervisorAccess", "group__rtc.html#a4cdb560a5062b3869a0172e08c856b15", null ],
      [ "compensationInterval", "group__rtc.html#aa378ae2e19870f88b02bb37d2eb2a410", null ],
      [ "compensationTime", "group__rtc.html#ab7345f3eb3998ceb00a3a5262ae1ec0e", null ]
    ] ],
    [ "FSL_RTC_DRIVER_VERSION", "group__rtc.html#gaea33cb4ca030fb967d99babaec611ff9", null ],
    [ "rtc_interrupt_enable_t", "group__rtc.html#gabed8712e00907f44b420d274fd368738", [
      [ "kRTC_TimeInvalidInterruptEnable", "group__rtc.html#ggabed8712e00907f44b420d274fd368738ac8f25a2201dc4dd638c28d049d7d0913", null ],
      [ "kRTC_TimeOverflowInterruptEnable", "group__rtc.html#ggabed8712e00907f44b420d274fd368738a6a518f6ebb6b1c04ae42ebc5003ea6be", null ],
      [ "kRTC_AlarmInterruptEnable", "group__rtc.html#ggabed8712e00907f44b420d274fd368738abd7ca2093bc2df536c828d8c8ea3dab4", null ],
      [ "kRTC_SecondsInterruptEnable", "group__rtc.html#ggabed8712e00907f44b420d274fd368738afa4307c8ee86fd861d6ba4f1bc43c782", null ]
    ] ],
    [ "rtc_status_flags_t", "group__rtc.html#gaa5edfafe0da586a9411fe4bafe32d9c5", [
      [ "kRTC_TimeInvalidFlag", "group__rtc.html#ggaa5edfafe0da586a9411fe4bafe32d9c5a6af0515ca5fae5c36599d2104f9c8fc7", null ],
      [ "kRTC_TimeOverflowFlag", "group__rtc.html#ggaa5edfafe0da586a9411fe4bafe32d9c5a9223509757776dfaf2ea741afef1bf90", null ],
      [ "kRTC_AlarmFlag", "group__rtc.html#ggaa5edfafe0da586a9411fe4bafe32d9c5a12101de586ebe90b1662305f467d88b2", null ]
    ] ],
    [ "rtc_osc_cap_load_t", "group__rtc.html#gaba10feedc5f3b239c134547709c82794", [
      [ "kRTC_Capacitor_2p", "group__rtc.html#ggaba10feedc5f3b239c134547709c82794af1ad702ff4f5fe2f7943928c437cdbca", null ],
      [ "kRTC_Capacitor_4p", "group__rtc.html#ggaba10feedc5f3b239c134547709c82794a1587d2d62aeaa6ff452ae2cc4dd58382", null ],
      [ "kRTC_Capacitor_8p", "group__rtc.html#ggaba10feedc5f3b239c134547709c82794a8d37764da4f07e501abae2c62eea8b27", null ],
      [ "kRTC_Capacitor_16p", "group__rtc.html#ggaba10feedc5f3b239c134547709c82794a6c70fa6e1d1a3de36abeca466bc81c84", null ]
    ] ],
    [ "RTC_Init", "group__rtc.html#ga9d13df422bcf270898ad8ce6f6e158fd", null ],
    [ "RTC_Deinit", "group__rtc.html#ga0f365b0df0c2e61b07a7e178bd0f1581", null ],
    [ "RTC_GetDefaultConfig", "group__rtc.html#ga27bf0709ea4fec76bc016f19bb729218", null ],
    [ "RTC_SetDatetime", "group__rtc.html#ga5ddf5a972712795ee53468c3cc35b2de", null ],
    [ "RTC_GetDatetime", "group__rtc.html#gabd709e50b2b6129bd429352282890720", null ],
    [ "RTC_SetAlarm", "group__rtc.html#gab2d6ccb1f963f5c38166bfe0a3e90955", null ],
    [ "RTC_GetAlarm", "group__rtc.html#ga197e1135863c96d064eb1828a32487b2", null ],
    [ "RTC_EnableInterrupts", "group__rtc.html#gac7de16884fa268bf9d1d1f4b983fc2d4", null ],
    [ "RTC_DisableInterrupts", "group__rtc.html#gad12d4e10c5601f53d1626a5ae71691b9", null ],
    [ "RTC_GetEnabledInterrupts", "group__rtc.html#gad0a0f005f9deff51c6a686901b62cf56", null ],
    [ "RTC_GetStatusFlags", "group__rtc.html#ga8e0dbfd23fc268adfcaa5c6d0c7b6650", null ],
    [ "RTC_ClearStatusFlags", "group__rtc.html#ga735b784e8d4027022e8f0370430e2837", null ],
    [ "RTC_StartTimer", "group__rtc.html#gae8e7b09ad38776c0e360debf20145d78", null ],
    [ "RTC_StopTimer", "group__rtc.html#ga57f9d747f6972fd0a20e6ddccb42f084", null ],
    [ "RTC_SetOscCapLoad", "group__rtc.html#gab8bc74661dc39d0ddd0ccc517a27f847", null ],
    [ "RTC_Reset", "group__rtc.html#ga5bf2f1b9b635918d74c5f4bb5b027d6c", null ]
];