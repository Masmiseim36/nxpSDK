var a00032 =
[
    [ "_flexio_i2c_type", "a00032.html#a00398", [
      [ "flexioBase", "a00032.html#a8c612e804a704d10af7495010447506d", null ],
      [ "SDAPinIndex", "a00032.html#a0dd4c317807ba1701e561d3f6b9131ce", null ],
      [ "SCLPinIndex", "a00032.html#a90f9f1c5d3181e6ac34b475c6eb0da56", null ],
      [ "shifterIndex", "a00032.html#ae1646b8581dea1779059dd37ad4c8515", null ],
      [ "timerIndex", "a00032.html#a00c9a97524f0d5c42585aacf4c4de4ca", null ],
      [ "baudrate", "a00032.html#ae684d7f95a201dbb4104801e0311f247", null ]
    ] ],
    [ "_flexio_i2c_master_config", "a00032.html#a00395", [
      [ "enableMaster", "a00032.html#a91a87b7bd8d4bae9e406390ae9776155", null ],
      [ "enableInDoze", "a00032.html#ae4c1517363576758cff67f9537927b88", null ],
      [ "enableInDebug", "a00032.html#a7f9e31f1448e28351002384106e332f0", null ],
      [ "enableFastAccess", "a00032.html#a689dee0d3ab98623b8f5c0b5bd83183d", null ],
      [ "baudRate_Bps", "a00032.html#abf5d294be79f4687caa730c2674de22e", null ]
    ] ],
    [ "_flexio_i2c_master_transfer", "a00032.html#a00397", [
      [ "flags", "a00032.html#aa74dd7104b75a95d9128fce2f9731628", null ],
      [ "slaveAddress", "a00032.html#ad40d580b11006c7b47931d5a16482a01", null ],
      [ "direction", "a00032.html#a3a0e94bf1d8f89f9a0f3c36b919e6a09", null ],
      [ "subaddress", "a00032.html#a3404710ebddd14a8b17023b52bb9879b", null ],
      [ "subaddressSize", "a00032.html#af41d8d9c777f5e6413561a92601ec930", null ],
      [ "data", "a00032.html#a0104b33e9efa445c26bde6cc0be3ddce", null ],
      [ "dataSize", "a00032.html#a8539ca8b74e1d6335754daf6e819fd47", null ]
    ] ],
    [ "_flexio_i2c_master_handle", "a00032.html#a00396", [
      [ "transfer", "a00032.html#a5532e06e5a1f46f2ee953951d6ddb45e", null ],
      [ "transferSize", "a00032.html#aa8453802324d3032c21902f09bd05382", null ],
      [ "state", "a00032.html#a3b70729eb68c9344be4f40542dfa248f", null ],
      [ "completionCallback", "a00032.html#a4b7bc5ea292a14712aec27ee849b34bd", null ],
      [ "userData", "a00032.html#a7727d87a3ccfd45513d77026b3c3958e", null ],
      [ "needRestart", "a00032.html#af59e068af6c05ffa9e2906aa992ef6cf", null ]
    ] ],
    [ "I2C_RETRY_TIMES", "a00032.html#gaac288786aaa7d0cb947c91591baf42d7", null ],
    [ "flexio_i2c_direction_t", "a00032.html#ga038697c86a5538e572c14a8cede654b3", null ],
    [ "FLEXIO_I2C_Type", "a00032.html#ga79ef07860cbf29ed2c2ebce2876f8186", null ],
    [ "flexio_i2c_master_config_t", "a00032.html#ga241764390b12485a028350187b87e3b0", null ],
    [ "flexio_i2c_master_transfer_t", "a00032.html#gad8cb291a190361416d2281573c658220", null ],
    [ "flexio_i2c_master_handle_t", "a00032.html#ga5f9254ffdddf26ddcd5838437c07c7f2", null ],
    [ "flexio_i2c_master_transfer_callback_t", "a00032.html#ga4a92b2aafd0ea1e7746dc23a7b0f0691", [
      [ "kStatus_FLEXIO_I2C_Busy", "a00032.html#gga458e651af6690959efa2afb96be7d609a8ab8145bc386f3630f13b87c38496331", null ],
      [ "kStatus_FLEXIO_I2C_Idle", "a00032.html#gga458e651af6690959efa2afb96be7d609a2f105638fc06422cc54cf5337d507873", null ],
      [ "kStatus_FLEXIO_I2C_Nak", "a00032.html#gga458e651af6690959efa2afb96be7d609af5e4a932f6f9e6ae30a42dc31fab8bd5", null ],
      [ "kStatus_FLEXIO_I2C_Timeout", "a00032.html#gga458e651af6690959efa2afb96be7d609a3d8bcc4ffd332948cb8b28f7982ac4d1", null ]
    ] ],
    [ "_flexio_i2c_master_interrupt", "a00032.html#gaa4597b532bb2a564c329076dc5da8155", [
      [ "kFLEXIO_I2C_TxEmptyInterruptEnable", "a00032.html#ggaa4597b532bb2a564c329076dc5da8155ac34fefcbb2d45dd7bfd385586439923b", null ],
      [ "kFLEXIO_I2C_RxFullInterruptEnable", "a00032.html#ggaa4597b532bb2a564c329076dc5da8155a46a53a6077c43dfb924d46e9a218eb6b", null ]
    ] ],
    [ "_flexio_i2c_master_status_flags", "a00032.html#gafc92480bf28b1161b62a1f10a23e7458", [
      [ "kFLEXIO_I2C_TxEmptyFlag", "a00032.html#ggafc92480bf28b1161b62a1f10a23e7458a5eb41467be294db469f4604a010405e3", null ],
      [ "kFLEXIO_I2C_RxFullFlag", "a00032.html#ggafc92480bf28b1161b62a1f10a23e7458a4eba9f70c326fcea72a255cba80dc42d", null ],
      [ "kFLEXIO_I2C_ReceiveNakFlag", "a00032.html#ggafc92480bf28b1161b62a1f10a23e7458a8774a1fedf2a596d2647864bd4f27664", null ]
    ] ],
    [ "_flexio_i2c_direction", "a00032.html#gabe134561df742c0ed1d6c56196255ccb", [
      [ "kFLEXIO_I2C_Write", "a00032.html#ggabe134561df742c0ed1d6c56196255ccba666a4c181486b773264ce1e038e14850", null ],
      [ "kFLEXIO_I2C_Read", "a00032.html#ggabe134561df742c0ed1d6c56196255ccbaeac2a6bd710680e5f644763dea0c89c4", null ]
    ] ],
    [ "FLEXIO_I2C_CheckForBusyBus", "a00032.html#ga2ded19f42c2b7e09764e6e926c501b67", null ],
    [ "FLEXIO_I2C_MasterInit", "a00032.html#gaaacd8cca7de1ff3a8c33c86d4f7f88fd", null ],
    [ "FLEXIO_I2C_MasterDeinit", "a00032.html#ga2da731bfa4e1e36b5d6d0235898b26dd", null ],
    [ "FLEXIO_I2C_MasterGetDefaultConfig", "a00032.html#gaa85599771e53091102f0823217ad0bcc", null ],
    [ "FLEXIO_I2C_MasterEnable", "a00032.html#ga287dc7896229cd705bb8abbf51ca63a7", null ],
    [ "FLEXIO_I2C_MasterGetStatusFlags", "a00032.html#ga5ac748fc7ff593cfa7a9b9a64cd479fa", null ],
    [ "FLEXIO_I2C_MasterClearStatusFlags", "a00032.html#gad7509c6f4a8885650c313bb0b56e6bfc", null ],
    [ "FLEXIO_I2C_MasterEnableInterrupts", "a00032.html#ga07198112aef949ac73874e7d446f7d38", null ],
    [ "FLEXIO_I2C_MasterDisableInterrupts", "a00032.html#ga3fcce197e268d6e7310a0087d9288b08", null ],
    [ "FLEXIO_I2C_MasterSetBaudRate", "a00032.html#gac1cb6e0f52c2e9e2b4373415dff1f24c", null ],
    [ "FLEXIO_I2C_MasterStart", "a00032.html#ga6f9def4b064e4747675dfc16e9fdde00", null ],
    [ "FLEXIO_I2C_MasterStop", "a00032.html#ga6b921453ee8e263f9a2a198b497c88ec", null ],
    [ "FLEXIO_I2C_MasterRepeatedStart", "a00032.html#ga8c2b3776c1630e0bf8e64927a0012b2f", null ],
    [ "FLEXIO_I2C_MasterAbortStop", "a00032.html#ga20efb0f28522d945694d509b5e4bf52d", null ],
    [ "FLEXIO_I2C_MasterEnableAck", "a00032.html#gaca2e797a84bc5b68a75ba10cf2750fc0", null ],
    [ "FLEXIO_I2C_MasterSetTransferCount", "a00032.html#ga6239ded74ab146f5f04c6355edf6e64a", null ],
    [ "FLEXIO_I2C_MasterWriteByte", "a00032.html#ga7e82edf243f26522ea3ecedddb4760ba", null ],
    [ "FLEXIO_I2C_MasterReadByte", "a00032.html#ga2a790d99cbabf8bf7df3216b6da904e5", null ],
    [ "FLEXIO_I2C_MasterWriteBlocking", "a00032.html#ga13c8be777cb7336878e161ab32fba000", null ],
    [ "FLEXIO_I2C_MasterReadBlocking", "a00032.html#ga5713e8dca1e7e896c3aed89e8961547a", null ],
    [ "FLEXIO_I2C_MasterTransferBlocking", "a00032.html#ga7b4b3c280a860777e98b5065225a3150", null ],
    [ "FLEXIO_I2C_MasterTransferCreateHandle", "a00032.html#ga2d26c2cb21b034487ba8b8d4c5da2ef5", null ],
    [ "FLEXIO_I2C_MasterTransferNonBlocking", "a00032.html#ga4e21323685831e0ee3f88d50e4c8872c", null ],
    [ "FLEXIO_I2C_MasterTransferGetCount", "a00032.html#ga79fed99f5f4524619e2c2e28ad037544", null ],
    [ "FLEXIO_I2C_MasterTransferAbort", "a00032.html#ga75ecfe909fcd0ae47303eb90af39dea2", null ],
    [ "FLEXIO_I2C_MasterTransferHandleIRQ", "a00032.html#ga29129d78b9166b7bcb585e336746bec6", null ]
];