var a00032 =
[
    [ "pwt_config_t", "a00032.html#a00225", [
      [ "clockSource", "a00032.html#a8e8c2395e552a5fa955b77d336e6d1e4", null ],
      [ "prescale", "a00032.html#a4f5359424d98c318500b92d70245086c", null ],
      [ "inputSelect", "a00032.html#aa0ee7f908258aa1cd79d882c33f06b65", null ],
      [ "enableFirstCounterLoad", "a00032.html#adf0d92191658b9cb7399816641d1a721", null ]
    ] ],
    [ "FSL_PWT_DRIVER_VERSION", "a00032.html#gaa5aa276b4262c262e5daad6d45259010", null ],
    [ "pwt_clock_source_t", "a00032.html#gaf7e04efc31ada132c7548e4fa9f5477c", [
      [ "kPWT_BusClock", "a00032.html#ggaf7e04efc31ada132c7548e4fa9f5477ca9b13b9e1013ded1381071a351aac0e68", null ],
      [ "kPWT_AlternativeClock", "a00032.html#ggaf7e04efc31ada132c7548e4fa9f5477caa72fcf998375e7f5ec5a1a84e7470a04", null ]
    ] ],
    [ "pwt_clock_prescale_t", "a00032.html#ga40748f4e80117da11499ee30e1497906", [
      [ "kPWT_Prescale_Divide_1", "a00032.html#gga40748f4e80117da11499ee30e1497906a46a9c17e96eb89b5b353f7fca4282840", null ],
      [ "kPWT_Prescale_Divide_2", "a00032.html#gga40748f4e80117da11499ee30e1497906af421a12977afd640cbf32be28d1838b5", null ],
      [ "kPWT_Prescale_Divide_4", "a00032.html#gga40748f4e80117da11499ee30e1497906a61f459b1e9c700794605cbf911f12712", null ],
      [ "kPWT_Prescale_Divide_8", "a00032.html#gga40748f4e80117da11499ee30e1497906a7da164533153d1f2a444c899deb11592", null ],
      [ "kPWT_Prescale_Divide_16", "a00032.html#gga40748f4e80117da11499ee30e1497906a806a7e0b593aef8dc532ae1967d8553f", null ],
      [ "kPWT_Prescale_Divide_32", "a00032.html#gga40748f4e80117da11499ee30e1497906a1ef6ddbb72a0d4dcc7a48eb57070847c", null ],
      [ "kPWT_Prescale_Divide_64", "a00032.html#gga40748f4e80117da11499ee30e1497906a88f3f383d2bc880c30c3cb952670d532", null ],
      [ "kPWT_Prescale_Divide_128", "a00032.html#gga40748f4e80117da11499ee30e1497906a6cf6510044556a41d01b3eb7ea27af34", null ]
    ] ],
    [ "pwt_input_select_t", "a00032.html#ga27ba9920e57fe6392157a60e10166d91", [
      [ "kPWT_InputPort_0", "a00032.html#gga27ba9920e57fe6392157a60e10166d91ab1c52e4446c907ea1c741b8373d828ba", null ],
      [ "kPWT_InputPort_1", "a00032.html#gga27ba9920e57fe6392157a60e10166d91a800898148511b5d7a57eb0a9722bf25a", null ],
      [ "kPWT_InputPort_2", "a00032.html#gga27ba9920e57fe6392157a60e10166d91ae677a1d60bb5b9b021179b4f9eb9cd58", null ],
      [ "kPWT_InputPort_3", "a00032.html#gga27ba9920e57fe6392157a60e10166d91afa801e8837be1cace41920c4f6f95693", null ]
    ] ],
    [ "_pwt_interrupt_enable", "a00032.html#gade88232488a6d70f29ceba870a0ddb5f", [
      [ "kPWT_PulseWidthReadyInterruptEnable", "a00032.html#ggade88232488a6d70f29ceba870a0ddb5fa9279b5dfb5559cd7bf48d5642d416707", null ],
      [ "kPWT_CounterOverflowInterruptEnable", "a00032.html#ggade88232488a6d70f29ceba870a0ddb5fa2255ba78f77ca5ddd214414256a26b2d", null ]
    ] ],
    [ "_pwt_status_flags", "a00032.html#gac1c6824eea1b3d26d8a81be33fd6c9e2", [
      [ "kPWT_CounterOverflowFlag", "a00032.html#ggac1c6824eea1b3d26d8a81be33fd6c9e2a6d35fc0b22c21e4ef0681c6a11a6cad0", null ],
      [ "kPWT_PulseWidthValidFlag", "a00032.html#ggac1c6824eea1b3d26d8a81be33fd6c9e2a9e3433588afecff2253fd3acbb50120d", null ]
    ] ],
    [ "PWT_Init", "a00032.html#ga42a91758a981d1d695ef8a61b797e638", null ],
    [ "PWT_Deinit", "a00032.html#ga49498e824a29181ef9b58af0faa32ea5", null ],
    [ "PWT_GetDefaultConfig", "a00032.html#gad9437f9146a02570bcff1cc5d555027d", null ],
    [ "PWT_EnableInterrupts", "a00032.html#gaf01ee07331774d26039962161f9a5f23", null ],
    [ "PWT_DisableInterrupts", "a00032.html#gaa1debabaeaa8272c828fd590cabdcc74", null ],
    [ "PWT_GetEnabledInterrupts", "a00032.html#ga87efd2ce1099739e4ef81c8c7d7d31bd", null ],
    [ "PWT_GetStatusFlags", "a00032.html#gad77c20c794f5204383a98c987fa518b6", null ],
    [ "PWT_ClearStatusFlags", "a00032.html#gaa225dd7808c108e4f0efe7ebb5040f24", null ],
    [ "PWT_StartTimer", "a00032.html#ga882061ed0cc726b55006004d01bcc11d", null ],
    [ "PWT_StopTimer", "a00032.html#ga7fe6101487e9cd088efab6ecc49add5d", null ],
    [ "PWT_GetCurrentTimerCount", "a00032.html#ga78a5705efd5a2b1545e350725fab9539", null ],
    [ "PWT_ReadPositivePulseWidth", "a00032.html#gac9012b5e9f248a69d599024c452323db", null ],
    [ "PWT_ReadNegativePulseWidth", "a00032.html#gab558baa8fac6f8e1ca52cd117fdc34c3", null ],
    [ "PWT_Reset", "a00032.html#ga0bfa3380355abe41a2e21a9213a9f0e8", null ]
];