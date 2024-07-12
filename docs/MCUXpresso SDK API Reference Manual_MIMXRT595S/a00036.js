var a00036 =
[
    [ "_i2c_master_config", "a00036.html#a00360", [
      [ "enableMaster", "a00036.html#a7a58fd313d0615ac1ca70d2b4f17ae9a", null ],
      [ "baudRate_Bps", "a00036.html#a282b6a9bd8e98bf35e8183665488d98c", null ],
      [ "enableTimeout", "a00036.html#ae638534f9f5735a294e951652c1d650b", null ],
      [ "timeout_Ms", "a00036.html#a7e002b275a9aeb2eca99c3c9e6f95416", null ]
    ] ],
    [ "_i2c_master_transfer", "a00036.html#a00363", [
      [ "flags", "a00036.html#a84861e140902bd67ae901b2f4c53bfc0", null ],
      [ "slaveAddress", "a00036.html#aa6d93caa78fc9bf0f0aac10f66b8eba1", null ],
      [ "direction", "a00036.html#a7bd3afcf89b78d9397b2f0c55f9deb27", null ],
      [ "subaddress", "a00036.html#a7a4f43a16e63d9f286bc41b6255b5e3f", null ],
      [ "subaddressSize", "a00036.html#a0c2c6dde8d11db7793fdea90f1b34afb", null ],
      [ "data", "a00036.html#a083727a4268398628225a68cad8c4a50", null ],
      [ "dataSize", "a00036.html#a7034b157201c46c116dc251e3b56412d", null ]
    ] ],
    [ "_i2c_master_handle", "a00036.html#a00362", [
      [ "state", "a00036.html#ade67a2d6339836711ee255a18405e750", null ],
      [ "transferCount", "a00036.html#ab08095b4638786e1b364c1eed398fce4", null ],
      [ "remainingBytes", "a00036.html#aa041075cd1ecaf90bcc73d96478e94cf", null ],
      [ "buf", "a00036.html#a62ffa1c0fea595fb26ac5604c437a549", null ],
      [ "checkAddrNack", "a00036.html#ab5d215cd09ce55b830be46d856764684", null ],
      [ "transfer", "a00036.html#a403d5da02efab45e1ebb8bef2ae063d5", null ],
      [ "completionCallback", "a00036.html#a0c6862e22cf2047661ce05975756093a", null ],
      [ "userData", "a00036.html#a862e1a312eb8b2bd537f1a22a48f455f", null ]
    ] ],
    [ "i2c_direction_t", "a00036.html#ga4bf954d998f086594eece268c780bec7", null ],
    [ "i2c_master_config_t", "a00036.html#gad739710b6b65e12ca5ba9dc2a2a2b463", null ],
    [ "i2c_master_transfer_t", "a00036.html#ga7edd253f4df51a13bd0a6a0461b9f93a", null ],
    [ "i2c_master_handle_t", "a00036.html#ga2fd51f58b8cfd309be985bce4b817fdc", null ],
    [ "i2c_master_transfer_callback_t", "a00036.html#gad292a48f957a9b76593c1779d9dce497", null ],
    [ "_i2c_direction", "a00036.html#ga1625497320644ba123718bafdd2c1f39", [
      [ "kI2C_Write", "a00036.html#gga1625497320644ba123718bafdd2c1f39a93b476b469c2a4bfed5916b458ae9bb0", null ],
      [ "kI2C_Read", "a00036.html#gga1625497320644ba123718bafdd2c1f39a3a7bb24dc8d1c1be8925603eeafe9b30", null ]
    ] ],
    [ "_i2c_master_transfer_flags", "a00036.html#ga87ea07668194cfb46c7c368d2cb42433", [
      [ "kI2C_TransferDefaultFlag", "a00036.html#gga87ea07668194cfb46c7c368d2cb42433ae80f7b768b1621e42ac965d3d23de5e2", null ],
      [ "kI2C_TransferNoStartFlag", "a00036.html#gga87ea07668194cfb46c7c368d2cb42433a5cb44bf860c0482c0ca0165cf0d31d8a", null ],
      [ "kI2C_TransferRepeatedStartFlag", "a00036.html#gga87ea07668194cfb46c7c368d2cb42433aadf112471ef12f194985a093cd9b9721", null ],
      [ "kI2C_TransferNoStopFlag", "a00036.html#gga87ea07668194cfb46c7c368d2cb42433afb8aeea71b5b7475e3d3df86220db566", null ]
    ] ],
    [ "_i2c_transfer_states", "a00036.html#gab08c1a0d50859637b4305687278941ee", null ],
    [ "I2C_MasterGetDefaultConfig", "a00036.html#gad69f2d63ea756dda60749ff6b09f587d", null ],
    [ "I2C_MasterInit", "a00036.html#ga29f967b3ae8487a36ba2a58deb01ccae", null ],
    [ "I2C_MasterDeinit", "a00036.html#gadcf7122f0a38d4d9da0f052fcb167957", null ],
    [ "I2C_GetInstance", "a00036.html#ga07cc6bd20f700249c335893427bf462a", null ],
    [ "I2C_MasterReset", "a00036.html#ga09137caccbe9a57f6b2ca5e1ce9bdc50", null ],
    [ "I2C_MasterEnable", "a00036.html#ga164d97e40eb5ad625a41ad43cf938153", null ],
    [ "I2C_GetStatusFlags", "a00036.html#ga30ea754c9685d10f394989ea19a6be22", null ],
    [ "I2C_ClearStatusFlags", "a00036.html#ga92a916f2d297de52d3a54f8bd0befb07", null ],
    [ "I2C_MasterClearStatusFlags", "a00036.html#ga437271ffb955b77df0a5dee9ea80cc63", null ],
    [ "I2C_EnableInterrupts", "a00036.html#ga0f6f1f2fe150661f8eb0a072665a9020", null ],
    [ "I2C_DisableInterrupts", "a00036.html#gac3e053611231cc886bfa38831a1ac26e", null ],
    [ "I2C_GetEnabledInterrupts", "a00036.html#gae652236a25f1b4828d596f32190655f8", null ],
    [ "I2C_MasterSetBaudRate", "a00036.html#gafeab1d5249a8b39c8d9e1a54a85c23f0", null ],
    [ "I2C_MasterSetTimeoutValue", "a00036.html#gae0d0c95b28e307b2d4a6ea420809b9e4", null ],
    [ "I2C_MasterGetBusIdleState", "a00036.html#ga4b7617034ab51d77786141873fd6ee1c", null ],
    [ "I2C_MasterStart", "a00036.html#ga7664234966e4162e952b6d57efcaa8a6", null ],
    [ "I2C_MasterStop", "a00036.html#gaf46a8cc094fc18c6cadea8de71448723", null ],
    [ "I2C_MasterRepeatedStart", "a00036.html#ga8137f7b333aafe1e3ff6c9d3852b7dbd", null ],
    [ "I2C_MasterWriteBlocking", "a00036.html#ga3b4d71b59e118adc63afbc5434e5e7dd", null ],
    [ "I2C_MasterReadBlocking", "a00036.html#gaba04c62ee636dd8317c2235c3c82aa3b", null ],
    [ "I2C_MasterTransferBlocking", "a00036.html#ga35cae3a5b4aa50ce8db28e3eb703a027", null ],
    [ "I2C_MasterTransferCreateHandle", "a00036.html#ga31e3de02b57801c6896e6045c4a31268", null ],
    [ "I2C_MasterTransferNonBlocking", "a00036.html#gac7f76a04b6f6873c52ad176d58901fe9", null ],
    [ "I2C_MasterTransferGetCount", "a00036.html#ga7d720842e68161d2d52d56d119f20665", null ],
    [ "I2C_MasterTransferAbort", "a00036.html#gabb386e53b2bc64e29b2915bdfac36800", null ],
    [ "I2C_MasterTransferHandleIRQ", "a00036.html#gafbf792484d11820561fa22bb27f73284", null ]
];