var group__flexio__i2c__master =
[
    [ "FLEXIO_I2C_Type", "group__flexio__i2c__master.html#structFLEXIO__I2C__Type", [
      [ "flexioBase", "group__flexio__i2c__master.html#a85065e51da3c0158d4b1d9254d691d26", null ],
      [ "SDAPinIndex", "group__flexio__i2c__master.html#acf0bab2399a7d7013f3708feb4d74ff6", null ],
      [ "SCLPinIndex", "group__flexio__i2c__master.html#a8ffb4cb7eac22b561f34ebcf4c309db9", null ],
      [ "shifterIndex", "group__flexio__i2c__master.html#ab1a64e477a45a6f98cd835644c6699ec", null ],
      [ "timerIndex", "group__flexio__i2c__master.html#a8350ae555c10de40f51f46479f43580e", null ]
    ] ],
    [ "flexio_i2c_master_config_t", "group__flexio__i2c__master.html#structflexio__i2c__master__config__t", [
      [ "enableMaster", "group__flexio__i2c__master.html#a4dcbed2930b467df1978f9bf2b71c128", null ],
      [ "enableInDoze", "group__flexio__i2c__master.html#ad73b88c0700d82721ccbf0744d1b6780", null ],
      [ "enableInDebug", "group__flexio__i2c__master.html#acf98ed2c52a014d2ed825a38293001d4", null ],
      [ "enableFastAccess", "group__flexio__i2c__master.html#a89a0daa456c7d99a7144d5245f422bbe", null ],
      [ "baudRate_Bps", "group__flexio__i2c__master.html#a446dee2e270776aa6d462710e85b7454", null ]
    ] ],
    [ "flexio_i2c_master_transfer_t", "group__flexio__i2c__master.html#structflexio__i2c__master__transfer__t", [
      [ "flags", "group__flexio__i2c__master.html#a2695cc7566c8a4cbbcf837d1f7209b6a", null ],
      [ "slaveAddress", "group__flexio__i2c__master.html#ad9a4b7694d338eaa4d7b6b2e8ebcf6c5", null ],
      [ "direction", "group__flexio__i2c__master.html#a756cb7ae9db1832fcffcd736a7039369", null ],
      [ "subaddress", "group__flexio__i2c__master.html#a60552386bd16b5d17197660571638898", null ],
      [ "subaddressSize", "group__flexio__i2c__master.html#a17d04e1156e9dd99026a4b527e487a96", null ],
      [ "data", "group__flexio__i2c__master.html#a94b0a6518c96392dd0497350af675ada", null ],
      [ "dataSize", "group__flexio__i2c__master.html#a5bd368a720ef8ed7df1601d7cac19ae2", null ]
    ] ],
    [ "flexio_i2c_master_handle_t", "group__flexio__i2c__master.html#struct__flexio__i2c__master__handle", [
      [ "transfer", "group__flexio__i2c__master.html#a60bd61f7bad1102b8ae17c147ebe7562", null ],
      [ "transferSize", "group__flexio__i2c__master.html#a48d8d3dd2b37359e54ee1ac10d68bfa5", null ],
      [ "state", "group__flexio__i2c__master.html#abdeb9df90f61127f95734a9ad2226de8", null ],
      [ "completionCallback", "group__flexio__i2c__master.html#a1b3ef650c54e91d7ae83998a7a3cb061", null ],
      [ "userData", "group__flexio__i2c__master.html#affef8570c9455a63c5b0d67595f1dc6a", null ]
    ] ],
    [ "FSL_FLEXIO_I2C_MASTER_DRIVER_VERSION", "group__flexio__i2c__master.html#ga869dccb29d32c1ccd2057003826d07cc", null ],
    [ "flexio_i2c_master_transfer_callback_t", "group__flexio__i2c__master.html#ga4a92b2aafd0ea1e7746dc23a7b0f0691", null ],
    [ "_flexio_i2c_status", "group__flexio__i2c__master.html#gadba7c89564ab90e58acd911686edb637", [
      [ "kStatus_FLEXIO_I2C_Busy", "group__flexio__i2c__master.html#ggadba7c89564ab90e58acd911686edb637a8ab8145bc386f3630f13b87c38496331", null ],
      [ "kStatus_FLEXIO_I2C_Idle", "group__flexio__i2c__master.html#ggadba7c89564ab90e58acd911686edb637a2f105638fc06422cc54cf5337d507873", null ],
      [ "kStatus_FLEXIO_I2C_Nak", "group__flexio__i2c__master.html#ggadba7c89564ab90e58acd911686edb637af5e4a932f6f9e6ae30a42dc31fab8bd5", null ]
    ] ],
    [ "_flexio_i2c_master_interrupt", "group__flexio__i2c__master.html#gaa4597b532bb2a564c329076dc5da8155", [
      [ "kFLEXIO_I2C_TxEmptyInterruptEnable", "group__flexio__i2c__master.html#ggaa4597b532bb2a564c329076dc5da8155ac34fefcbb2d45dd7bfd385586439923b", null ],
      [ "kFLEXIO_I2C_RxFullInterruptEnable", "group__flexio__i2c__master.html#ggaa4597b532bb2a564c329076dc5da8155a46a53a6077c43dfb924d46e9a218eb6b", null ]
    ] ],
    [ "_flexio_i2c_master_status_flags", "group__flexio__i2c__master.html#gafc92480bf28b1161b62a1f10a23e7458", [
      [ "kFLEXIO_I2C_TxEmptyFlag", "group__flexio__i2c__master.html#ggafc92480bf28b1161b62a1f10a23e7458a5eb41467be294db469f4604a010405e3", null ],
      [ "kFLEXIO_I2C_RxFullFlag", "group__flexio__i2c__master.html#ggafc92480bf28b1161b62a1f10a23e7458a4eba9f70c326fcea72a255cba80dc42d", null ],
      [ "kFLEXIO_I2C_ReceiveNakFlag", "group__flexio__i2c__master.html#ggafc92480bf28b1161b62a1f10a23e7458a8774a1fedf2a596d2647864bd4f27664", null ]
    ] ],
    [ "flexio_i2c_direction_t", "group__flexio__i2c__master.html#gad7cd7cc57788188d0f804b5e3a549303", [
      [ "kFLEXIO_I2C_Write", "group__flexio__i2c__master.html#ggad7cd7cc57788188d0f804b5e3a549303a666a4c181486b773264ce1e038e14850", null ],
      [ "kFLEXIO_I2C_Read", "group__flexio__i2c__master.html#ggad7cd7cc57788188d0f804b5e3a549303aeac2a6bd710680e5f644763dea0c89c4", null ]
    ] ],
    [ "FLEXIO_I2C_MasterInit", "group__flexio__i2c__master.html#ga2eaa70779023d5c04cbe1a1fcd85063e", null ],
    [ "FLEXIO_I2C_MasterDeinit", "group__flexio__i2c__master.html#ga2da731bfa4e1e36b5d6d0235898b26dd", null ],
    [ "FLEXIO_I2C_MasterGetDefaultConfig", "group__flexio__i2c__master.html#gaa85599771e53091102f0823217ad0bcc", null ],
    [ "FLEXIO_I2C_MasterEnable", "group__flexio__i2c__master.html#ga287dc7896229cd705bb8abbf51ca63a7", null ],
    [ "FLEXIO_I2C_MasterGetStatusFlags", "group__flexio__i2c__master.html#ga5ac748fc7ff593cfa7a9b9a64cd479fa", null ],
    [ "FLEXIO_I2C_MasterClearStatusFlags", "group__flexio__i2c__master.html#gad7509c6f4a8885650c313bb0b56e6bfc", null ],
    [ "FLEXIO_I2C_MasterEnableInterrupts", "group__flexio__i2c__master.html#ga07198112aef949ac73874e7d446f7d38", null ],
    [ "FLEXIO_I2C_MasterDisableInterrupts", "group__flexio__i2c__master.html#ga3fcce197e268d6e7310a0087d9288b08", null ],
    [ "FLEXIO_I2C_MasterSetBaudRate", "group__flexio__i2c__master.html#gac1cb6e0f52c2e9e2b4373415dff1f24c", null ],
    [ "FLEXIO_I2C_MasterStart", "group__flexio__i2c__master.html#ga6f9def4b064e4747675dfc16e9fdde00", null ],
    [ "FLEXIO_I2C_MasterStop", "group__flexio__i2c__master.html#ga6b921453ee8e263f9a2a198b497c88ec", null ],
    [ "FLEXIO_I2C_MasterRepeatedStart", "group__flexio__i2c__master.html#ga8c2b3776c1630e0bf8e64927a0012b2f", null ],
    [ "FLEXIO_I2C_MasterAbortStop", "group__flexio__i2c__master.html#ga20efb0f28522d945694d509b5e4bf52d", null ],
    [ "FLEXIO_I2C_MasterEnableAck", "group__flexio__i2c__master.html#gaca2e797a84bc5b68a75ba10cf2750fc0", null ],
    [ "FLEXIO_I2C_MasterSetTransferCount", "group__flexio__i2c__master.html#gafbb79071dcdfdfe0102ea0d61cb5a5c8", null ],
    [ "FLEXIO_I2C_MasterWriteByte", "group__flexio__i2c__master.html#ga7e82edf243f26522ea3ecedddb4760ba", null ],
    [ "FLEXIO_I2C_MasterReadByte", "group__flexio__i2c__master.html#ga2a790d99cbabf8bf7df3216b6da904e5", null ],
    [ "FLEXIO_I2C_MasterWriteBlocking", "group__flexio__i2c__master.html#ga13c8be777cb7336878e161ab32fba000", null ],
    [ "FLEXIO_I2C_MasterReadBlocking", "group__flexio__i2c__master.html#ga029bc6387055ec2e878389217e484828", null ],
    [ "FLEXIO_I2C_MasterTransferBlocking", "group__flexio__i2c__master.html#ga7b4b3c280a860777e98b5065225a3150", null ],
    [ "FLEXIO_I2C_MasterTransferCreateHandle", "group__flexio__i2c__master.html#ga2d26c2cb21b034487ba8b8d4c5da2ef5", null ],
    [ "FLEXIO_I2C_MasterTransferNonBlocking", "group__flexio__i2c__master.html#ga4e21323685831e0ee3f88d50e4c8872c", null ],
    [ "FLEXIO_I2C_MasterTransferGetCount", "group__flexio__i2c__master.html#ga79fed99f5f4524619e2c2e28ad037544", null ],
    [ "FLEXIO_I2C_MasterTransferAbort", "group__flexio__i2c__master.html#ga75ecfe909fcd0ae47303eb90af39dea2", null ],
    [ "FLEXIO_I2C_MasterTransferHandleIRQ", "group__flexio__i2c__master.html#ga29129d78b9166b7bcb585e336746bec6", null ]
];