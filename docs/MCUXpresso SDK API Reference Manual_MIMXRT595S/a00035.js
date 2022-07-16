var a00035 =
[
    [ "i2c_master_config_t", "a00035.html#a00371", [
      [ "enableMaster", "a00035.html#a2419131a10906475fd31950f17ac8895", null ],
      [ "baudRate_Bps", "a00035.html#a2186844dc87bcde999fc12005f4c550a", null ],
      [ "enableTimeout", "a00035.html#a1c6831135c83c013d2a3cfc3e7210357", null ],
      [ "timeout_Ms", "a00035.html#a4f981abbbbd75a8a9ca923fafe76a1a3", null ]
    ] ],
    [ "i2c_master_transfer_t", "a00035.html#a00296", [
      [ "flags", "a00035.html#a8835787e1b0f9a4b8868e7cbe53e45d5", null ],
      [ "slaveAddress", "a00035.html#a3b9c4ae818b1194955db51de0f67795f", null ],
      [ "direction", "a00035.html#ac5b89dc9115b7760431c981e0c1fd443", null ],
      [ "subaddress", "a00035.html#ae7facb612714785d4e143e57d47a5af3", null ],
      [ "subaddressSize", "a00035.html#aeec8dccf4a49f03ff9a40f5982a24796", null ],
      [ "data", "a00035.html#a8c7f63bd1e7de04e40f5cd733f5c7388", null ],
      [ "dataSize", "a00035.html#a68e2dd69e1e34b1b505270e1e07e8a34", null ]
    ] ],
    [ "i2c_master_handle_t", "a00035.html#a00295", [
      [ "state", "a00035.html#add7ec18bc8239c5c87ffcec2fbcf5dd8", null ],
      [ "transferCount", "a00035.html#a5943d16f5ed6e7c4ebd334cdbc0e2afc", null ],
      [ "remainingBytes", "a00035.html#aca481b5d3ca6dce53cf440b534ff80c2", null ],
      [ "buf", "a00035.html#a9577474c0c4395355174df2b016108de", null ],
      [ "checkAddrNack", "a00035.html#ac9cd27efebaa02f0f61af7f43e8ff163", null ],
      [ "transfer", "a00035.html#a6858d3525f762d7aded20e6c95eb19fc", null ],
      [ "completionCallback", "a00035.html#a15b84b8a94c2b2e5ace0a695c79edd84", null ],
      [ "userData", "a00035.html#aad7df570c53adb2e80acd2ba0d39d109", null ]
    ] ],
    [ "i2c_master_transfer_callback_t", "a00035.html#gad292a48f957a9b76593c1779d9dce497", null ],
    [ "i2c_direction_t", "a00035.html#gab49c827b45635206f06e5737606e4611", [
      [ "kI2C_Write", "a00035.html#ggab49c827b45635206f06e5737606e4611a93b476b469c2a4bfed5916b458ae9bb0", null ],
      [ "kI2C_Read", "a00035.html#ggab49c827b45635206f06e5737606e4611a3a7bb24dc8d1c1be8925603eeafe9b30", null ]
    ] ],
    [ "_i2c_master_transfer_flags", "a00035.html#ga87ea07668194cfb46c7c368d2cb42433", [
      [ "kI2C_TransferDefaultFlag", "a00035.html#gga87ea07668194cfb46c7c368d2cb42433ae80f7b768b1621e42ac965d3d23de5e2", null ],
      [ "kI2C_TransferNoStartFlag", "a00035.html#gga87ea07668194cfb46c7c368d2cb42433a5cb44bf860c0482c0ca0165cf0d31d8a", null ],
      [ "kI2C_TransferRepeatedStartFlag", "a00035.html#gga87ea07668194cfb46c7c368d2cb42433aadf112471ef12f194985a093cd9b9721", null ],
      [ "kI2C_TransferNoStopFlag", "a00035.html#gga87ea07668194cfb46c7c368d2cb42433afb8aeea71b5b7475e3d3df86220db566", null ]
    ] ],
    [ "_i2c_transfer_states", "a00035.html#gab08c1a0d50859637b4305687278941ee", null ],
    [ "I2C_MasterGetDefaultConfig", "a00035.html#gad69f2d63ea756dda60749ff6b09f587d", null ],
    [ "I2C_MasterInit", "a00035.html#ga29f967b3ae8487a36ba2a58deb01ccae", null ],
    [ "I2C_MasterDeinit", "a00035.html#gadcf7122f0a38d4d9da0f052fcb167957", null ],
    [ "I2C_GetInstance", "a00035.html#ga07cc6bd20f700249c335893427bf462a", null ],
    [ "I2C_MasterReset", "a00035.html#ga09137caccbe9a57f6b2ca5e1ce9bdc50", null ],
    [ "I2C_MasterEnable", "a00035.html#ga164d97e40eb5ad625a41ad43cf938153", null ],
    [ "I2C_GetStatusFlags", "a00035.html#ga30ea754c9685d10f394989ea19a6be22", null ],
    [ "I2C_ClearStatusFlags", "a00035.html#ga92a916f2d297de52d3a54f8bd0befb07", null ],
    [ "I2C_MasterClearStatusFlags", "a00035.html#ga437271ffb955b77df0a5dee9ea80cc63", null ],
    [ "I2C_EnableInterrupts", "a00035.html#ga0f6f1f2fe150661f8eb0a072665a9020", null ],
    [ "I2C_DisableInterrupts", "a00035.html#gac3e053611231cc886bfa38831a1ac26e", null ],
    [ "I2C_GetEnabledInterrupts", "a00035.html#gae652236a25f1b4828d596f32190655f8", null ],
    [ "I2C_MasterSetBaudRate", "a00035.html#gafeab1d5249a8b39c8d9e1a54a85c23f0", null ],
    [ "I2C_MasterSetTimeoutValue", "a00035.html#gae0d0c95b28e307b2d4a6ea420809b9e4", null ],
    [ "I2C_MasterGetBusIdleState", "a00035.html#ga4b7617034ab51d77786141873fd6ee1c", null ],
    [ "I2C_MasterStart", "a00035.html#ga7664234966e4162e952b6d57efcaa8a6", null ],
    [ "I2C_MasterStop", "a00035.html#gaf46a8cc094fc18c6cadea8de71448723", null ],
    [ "I2C_MasterRepeatedStart", "a00035.html#ga8137f7b333aafe1e3ff6c9d3852b7dbd", null ],
    [ "I2C_MasterWriteBlocking", "a00035.html#ga3b4d71b59e118adc63afbc5434e5e7dd", null ],
    [ "I2C_MasterReadBlocking", "a00035.html#gaba04c62ee636dd8317c2235c3c82aa3b", null ],
    [ "I2C_MasterTransferBlocking", "a00035.html#ga35cae3a5b4aa50ce8db28e3eb703a027", null ],
    [ "I2C_MasterTransferCreateHandle", "a00035.html#ga31e3de02b57801c6896e6045c4a31268", null ],
    [ "I2C_MasterTransferNonBlocking", "a00035.html#gac7f76a04b6f6873c52ad176d58901fe9", null ],
    [ "I2C_MasterTransferGetCount", "a00035.html#ga7d720842e68161d2d52d56d119f20665", null ],
    [ "I2C_MasterTransferAbort", "a00035.html#gabb386e53b2bc64e29b2915bdfac36800", null ],
    [ "I2C_MasterTransferHandleIRQ", "a00035.html#gafbf792484d11820561fa22bb27f73284", null ]
];