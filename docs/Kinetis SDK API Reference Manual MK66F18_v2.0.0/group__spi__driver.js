var group__spi__driver =
[
    [ "spi_master_config_t", "group__spi__driver.html#structspi__master__config__t", [
      [ "enableMaster", "group__spi__driver.html#aa033bd20cfbb1a14f0fd43f4b31bb27e", null ],
      [ "enableStopInWaitMode", "group__spi__driver.html#a72eacf86c213c982a8c3677c995cf955", null ],
      [ "polarity", "group__spi__driver.html#ac8d9a8c7a526b96cbe3105d6e06b1cce", null ],
      [ "phase", "group__spi__driver.html#a21b6f74dbeb89a81460cd618b7da042b", null ],
      [ "direction", "group__spi__driver.html#aace9261acfad8526f3d5b0b59346629a", null ],
      [ "dataMode", "group__spi__driver.html#a50f8b30e4a50a8be2b136175ff327a17", null ],
      [ "txWatermark", "group__spi__driver.html#a63118dfe890616b78343c46c4ea899b1", null ],
      [ "rxWatermark", "group__spi__driver.html#adce084d034655724c6ae61ab3836eb2e", null ],
      [ "outputMode", "group__spi__driver.html#a4bef3df530a9f4209409ab332c6cf99f", null ],
      [ "pinMode", "group__spi__driver.html#adae6cf2b677f553a394f8ec3282a5531", null ],
      [ "baudRate_Bps", "group__spi__driver.html#ae7695987e044d80983fd98a43812b1ea", null ]
    ] ],
    [ "spi_slave_config_t", "group__spi__driver.html#structspi__slave__config__t", [
      [ "enableSlave", "group__spi__driver.html#a9fcd3fae6d886c7dfd0c3fd4cd51e6fe", null ],
      [ "enableStopInWaitMode", "group__spi__driver.html#a05bf5a3d9a06ece1c2e45f532ebadabd", null ],
      [ "polarity", "group__spi__driver.html#a32c240620581fcb133877d4fbda21226", null ],
      [ "phase", "group__spi__driver.html#ac6d76d6b354491d5b43756fdf8ef07d6", null ],
      [ "direction", "group__spi__driver.html#ad4eb458a0c1a0c0934eec53d71b78ffb", null ],
      [ "dataMode", "group__spi__driver.html#a36035f428e425565cb600a9bcbc57a8c", null ],
      [ "txWatermark", "group__spi__driver.html#a1a5ea2f93ef1f280d53f821c2f67995a", null ],
      [ "rxWatermark", "group__spi__driver.html#a909bef9a8771b2735d1792546bc1b45d", null ]
    ] ],
    [ "spi_transfer_t", "group__spi__driver.html#structspi__transfer__t", [
      [ "txData", "group__spi__driver.html#addc4f20760a1a62c4d20cdf9443f3af3", null ],
      [ "rxData", "group__spi__driver.html#a28b878b99d5ec790b5c8f3bafe140da0", null ],
      [ "dataSize", "group__spi__driver.html#ae003337c9398e5f5e3189f5b2ba6335b", null ],
      [ "flags", "group__spi__driver.html#ac2c944a611d5bd3267aef7658d06147b", null ]
    ] ],
    [ "spi_master_handle_t", "group__spi__driver.html#struct__spi__master__handle", [
      [ "txData", "group__spi__driver.html#a6cb4626f6b63c70fa21fe2e3338dc915", null ],
      [ "rxData", "group__spi__driver.html#a69f260bde2e1728233f835a148b2f51b", null ],
      [ "txRemainingBytes", "group__spi__driver.html#a62fa101ddfc970e7b6bcba0b2eb6b869", null ],
      [ "rxRemainingBytes", "group__spi__driver.html#a23b2531ea36b53e42cd6b2a3780d3017", null ],
      [ "state", "group__spi__driver.html#ae7933252a37be998d127217f34f6fd16", null ],
      [ "transferSize", "group__spi__driver.html#a415306f31fa29b548ee21a0f9aacc096", null ],
      [ "bytePerFrame", "group__spi__driver.html#ac5bb58dcd72b94be9f29c8072018cd5c", null ],
      [ "watermark", "group__spi__driver.html#a9b3ccbbfc594b100114784b63dc08040", null ],
      [ "callback", "group__spi__driver.html#a445e70d437c917e6af1b4037bdbb6a3f", null ],
      [ "userData", "group__spi__driver.html#ab8d01b85149d749ab1c748bb5116b90e", null ]
    ] ],
    [ "FSL_SPI_DRIVER_VERSION", "group__spi__driver.html#gaeb6046de02dffe9248de71332fac246e", null ],
    [ "SPI_DUMMYDATA", "group__spi__driver.html#ga1541129ba8262e1649592b5109e2074c", null ],
    [ "spi_slave_handle_t", "group__spi__driver.html#gad267cfee3a876b2860217ff94f03f574", null ],
    [ "spi_master_callback_t", "group__spi__driver.html#gae9bd140aeb645efab6c7552b3994e01a", null ],
    [ "spi_slave_callback_t", "group__spi__driver.html#ga86b45b85e036adc762eed5bcd2a0491d", null ],
    [ "_spi_status", "group__spi__driver.html#ga3fa79a6717ea4e1e74de2dadaa468edd", [
      [ "kStatus_SPI_Busy", "group__spi__driver.html#gga3fa79a6717ea4e1e74de2dadaa468edda703abdf7900047c4d13536480f3463ab", null ],
      [ "kStatus_SPI_Idle", "group__spi__driver.html#gga3fa79a6717ea4e1e74de2dadaa468edda4e32c5b06dccaf4b81e2fd1679e1b560", null ],
      [ "kStatus_SPI_Error", "group__spi__driver.html#gga3fa79a6717ea4e1e74de2dadaa468edda2ff91d774e93aed936b87ffaa18aaf9e", null ]
    ] ],
    [ "spi_clock_polarity_t", "group__spi__driver.html#ga3e5a7cd043c9596779bc23b34cb3d1f9", [
      [ "kSPI_ClockPolarityActiveHigh", "group__spi__driver.html#gga3e5a7cd043c9596779bc23b34cb3d1f9a031c0ee2693cf1fd621e8b8cba676629", null ],
      [ "kSPI_ClockPolarityActiveLow", "group__spi__driver.html#gga3e5a7cd043c9596779bc23b34cb3d1f9acfc8257079c9604086622df6de326aea", null ]
    ] ],
    [ "spi_clock_phase_t", "group__spi__driver.html#ga9ad313685ade497f5cbcb71c74a1b4dc", [
      [ "kSPI_ClockPhaseFirstEdge", "group__spi__driver.html#gga9ad313685ade497f5cbcb71c74a1b4dcad15d61c6fd642f13101edbf401d0c72e", null ],
      [ "kSPI_ClockPhaseSecondEdge", "group__spi__driver.html#gga9ad313685ade497f5cbcb71c74a1b4dca18a98985c1f7dd56175e4a2724db3675", null ]
    ] ],
    [ "spi_shift_direction_t", "group__spi__driver.html#gaa68518c16202382c2e1f1c7c66a9d53d", [
      [ "kSPI_MsbFirst", "group__spi__driver.html#ggaa68518c16202382c2e1f1c7c66a9d53dae7b997e8cff761aab865cbd42c4c3989", null ],
      [ "kSPI_LsbFirst", "group__spi__driver.html#ggaa68518c16202382c2e1f1c7c66a9d53da20ac53684c3e1ad338553cecbfab94a9", null ]
    ] ],
    [ "spi_ss_output_mode_t", "group__spi__driver.html#ga2e5a238c65f35a2d82cc1c55927030fe", [
      [ "kSPI_SlaveSelectAsGpio", "group__spi__driver.html#gga2e5a238c65f35a2d82cc1c55927030fea3825177650b5d4bfb5dc603f0ce41381", null ],
      [ "kSPI_SlaveSelectFaultInput", "group__spi__driver.html#gga2e5a238c65f35a2d82cc1c55927030feafdcbdc624d6c2f057147e2aa3cae4635", null ],
      [ "kSPI_SlaveSelectAutomaticOutput", "group__spi__driver.html#gga2e5a238c65f35a2d82cc1c55927030fead724fc04589d03b524ef2dfef6b0f4a1", null ]
    ] ],
    [ "spi_pin_mode_t", "group__spi__driver.html#gab55fee4d636428392047e3355f33a36f", [
      [ "kSPI_PinModeNormal", "group__spi__driver.html#ggab55fee4d636428392047e3355f33a36fa7ba3d5ffce8458f91a9594e7ffe2b32d", null ],
      [ "kSPI_PinModeInput", "group__spi__driver.html#ggab55fee4d636428392047e3355f33a36fad778c9cc9e3d5a5c4e8061793cf50fdc", null ],
      [ "kSPI_PinModeOutput", "group__spi__driver.html#ggab55fee4d636428392047e3355f33a36fad5b79c8eba688dbfb4b9973348521de7", null ]
    ] ],
    [ "spi_data_bitcount_mode_t", "group__spi__driver.html#gaa7bfe75c1f0ea3b349f19a3de19b24e4", [
      [ "kSPI_8BitMode", "group__spi__driver.html#ggaa7bfe75c1f0ea3b349f19a3de19b24e4a1a0723fd8cd73f361ca2770ad74a6324", null ],
      [ "kSPI_16BitMode", "group__spi__driver.html#ggaa7bfe75c1f0ea3b349f19a3de19b24e4a740c43589ebe839f3adc77b8ca484ea4", null ]
    ] ],
    [ "_spi_interrupt_enable", "group__spi__driver.html#gaedd690a0f91a0a9eb0fd573b57e31f67", [
      [ "kSPI_RxFullAndModfInterruptEnable", "group__spi__driver.html#ggaedd690a0f91a0a9eb0fd573b57e31f67aa317b77c827ed1462b766dddd93f7f10", null ],
      [ "kSPI_TxEmptyInterruptEnable", "group__spi__driver.html#ggaedd690a0f91a0a9eb0fd573b57e31f67abe157750a1660d839a66197c56ac5096", null ],
      [ "kSPI_MatchInterruptEnable", "group__spi__driver.html#ggaedd690a0f91a0a9eb0fd573b57e31f67a95cc9167ada10046534dc22549c6a99e", null ],
      [ "kSPI_RxFifoNearFullInterruptEnable", "group__spi__driver.html#ggaedd690a0f91a0a9eb0fd573b57e31f67a081a753d8d51b97b289103c698703537", null ],
      [ "kSPI_TxFifoNearEmptyInterruptEnable", "group__spi__driver.html#ggaedd690a0f91a0a9eb0fd573b57e31f67a299b7d97e4106d57bf06a6654ce4f3dd", null ]
    ] ],
    [ "_spi_flags", "group__spi__driver.html#gaea776f478792865a85b7311e6ff5896c", [
      [ "kSPI_RxBufferFullFlag", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896cacc0e5c67e296cc20197afaca46f22953", null ],
      [ "kSPI_MatchFlag", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca9c24fe83960ed64638f06eeb17643da5", null ],
      [ "kSPI_TxBufferEmptyFlag", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca07934a27954d951831e7cdb96c39afd7", null ],
      [ "kSPI_ModeFaultFlag", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca9eca4e759c879e3b264d052c234a09d1", null ],
      [ "kSPI_RxFifoNearFullFlag", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca840b49c37a729e29d6c144650f31dc88", null ],
      [ "kSPI_TxFifoNearEmptyFlag", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca85cc9ec1f4395db09fc288e293e5dcb1", null ],
      [ "kSPI_TxFifoFullFlag", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca73bf0494fbb812cfd36b526165258b89", null ],
      [ "kSPI_RxFifoEmptyFlag", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca98e5061d516b7a7cb5abdf73d0116fbd", null ],
      [ "kSPI_TxFifoError", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896cab1d47ff1dc06aa27a41cf90715f203aa", null ],
      [ "kSPI_RxFifoError", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca46b6a3bffe664d731bfbfa38f24a5797", null ],
      [ "kSPI_TxOverflow", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca553c7d65e38657565d1818fdfddd600e", null ],
      [ "kSPI_RxOverflow", "group__spi__driver.html#ggaea776f478792865a85b7311e6ff5896ca92f9ace674410eacc7f5a1bdbbdac23c", null ]
    ] ],
    [ "spi_w1c_interrupt_t", "group__spi__driver.html#gad61329dc7edac1f4ef75e0746e08b3d1", [
      [ "kSPI_RxFifoFullClearInterrupt", "group__spi__driver.html#ggad61329dc7edac1f4ef75e0746e08b3d1a1ee9a347a93f3a74a8c72786747f2a22", null ],
      [ "kSPI_TxFifoEmptyClearInterrupt", "group__spi__driver.html#ggad61329dc7edac1f4ef75e0746e08b3d1adc9f4730672a8501f0ae14b25f044567", null ],
      [ "kSPI_RxNearFullClearInterrupt", "group__spi__driver.html#ggad61329dc7edac1f4ef75e0746e08b3d1af3a8b77f84cf2406ed1365e02e74fa8a", null ],
      [ "kSPI_TxNearEmptyClearInterrupt", "group__spi__driver.html#ggad61329dc7edac1f4ef75e0746e08b3d1ac5358e4b6907d9db9ec2eb5122f1e15c", null ]
    ] ],
    [ "spi_txfifo_watermark_t", "group__spi__driver.html#ga9a479127a2051ded7945115657adc162", [
      [ "kSPI_TxFifoOneFourthEmpty", "group__spi__driver.html#gga9a479127a2051ded7945115657adc162a9349658226a7cb7eb0629ae38853030d", null ],
      [ "kSPI_TxFifoOneHalfEmpty", "group__spi__driver.html#gga9a479127a2051ded7945115657adc162a1460af596f3d1a6a2bbedd9624975ed5", null ]
    ] ],
    [ "spi_rxfifo_watermark_t", "group__spi__driver.html#ga79f862820ec2bcbb8184ce9de72949a6", [
      [ "kSPI_RxFifoThreeFourthsFull", "group__spi__driver.html#gga79f862820ec2bcbb8184ce9de72949a6ad045f28c12abb7d7c9c69f067d03efb2", null ],
      [ "kSPI_RxFifoOneHalfFull", "group__spi__driver.html#gga79f862820ec2bcbb8184ce9de72949a6aa10222f6949d379dfaf8f408bbb87c42", null ]
    ] ],
    [ "_spi_dma_enable_t", "group__spi__driver.html#ga8ab1fba361ce972e7e249ba1ea318e20", [
      [ "kSPI_TxDmaEnable", "group__spi__driver.html#gga8ab1fba361ce972e7e249ba1ea318e20a0bbf1c58480329e906e2b80a05121cc5", null ],
      [ "kSPI_RxDmaEnable", "group__spi__driver.html#gga8ab1fba361ce972e7e249ba1ea318e20a0addd2d49971cb0f0caa057510f5d9f1", null ],
      [ "kSPI_DmaAllEnable", "group__spi__driver.html#gga8ab1fba361ce972e7e249ba1ea318e20aa157e1fb5d95f1488f6f03fb4baaa7f2", null ]
    ] ],
    [ "SPI_MasterGetDefaultConfig", "group__spi__driver.html#ga45c08fc078ae334b79fb844379140838", null ],
    [ "SPI_MasterInit", "group__spi__driver.html#ga3fe5b421b088cc98222b8a214069574b", null ],
    [ "SPI_SlaveGetDefaultConfig", "group__spi__driver.html#gac000b63ed033f57a9eee342a8c8e67f2", null ],
    [ "SPI_SlaveInit", "group__spi__driver.html#ga91b214b5392fc8d5b73a5ebadc5ca363", null ],
    [ "SPI_Deinit", "group__spi__driver.html#gaac0bc2b87ea2eb7eeba78d9449d4dbbf", null ],
    [ "SPI_Enable", "group__spi__driver.html#ga6d32506242b9596989efbbb2d030e997", null ],
    [ "SPI_GetStatusFlags", "group__spi__driver.html#gaaa5a317a68b409dfe20397b45e48d1f0", null ],
    [ "SPI_ClearInterrupt", "group__spi__driver.html#ga57d32103b22e375efde0d1a392f125ea", null ],
    [ "SPI_EnableInterrupts", "group__spi__driver.html#gac12d144816080c40ca86d762e31aacc4", null ],
    [ "SPI_DisableInterrupts", "group__spi__driver.html#ga1c3de83156f45d3f91540a1ec5f92826", null ],
    [ "SPI_EnableDMA", "group__spi__driver.html#ga552ce373bfe655a37a22c26fdb071e0b", null ],
    [ "SPI_GetDataRegisterAddress", "group__spi__driver.html#ga740215d83c1238818dfd4689163f71de", null ],
    [ "SPI_MasterSetBaudRate", "group__spi__driver.html#ga3ebe045018782edb3624026795d6269a", null ],
    [ "SPI_SetMatchData", "group__spi__driver.html#ga981b773ced8cc0df2493691842a14e05", null ],
    [ "SPI_EnableFIFO", "group__spi__driver.html#ga73f02aa76d813feff22fb3855105e023", null ],
    [ "SPI_WriteBlocking", "group__spi__driver.html#gae28a41ba015e3bc92936459f83500cd0", null ],
    [ "SPI_WriteData", "group__spi__driver.html#ga74d8732386a607c263bd16c8294b08d6", null ],
    [ "SPI_ReadData", "group__spi__driver.html#gaf23d3779b122e78fc07562bf5dbf3ac3", null ],
    [ "SPI_MasterTransferCreateHandle", "group__spi__driver.html#ga6cf3e807307ecb22578cc9088adafcf2", null ],
    [ "SPI_MasterTransferBlocking", "group__spi__driver.html#ga5b05095245ecf01258e64cc0458fb2e9", null ],
    [ "SPI_MasterTransferNonBlocking", "group__spi__driver.html#ga820c78a32fa29735168d2e517f4881a1", null ],
    [ "SPI_MasterTransferGetCount", "group__spi__driver.html#gabde0dccfb0783d103b9cf57e0202582f", null ],
    [ "SPI_MasterTransferAbort", "group__spi__driver.html#gae90962a54978acf05ca7780489301eb4", null ],
    [ "SPI_MasterTransferHandleIRQ", "group__spi__driver.html#ga645d041a92bde312d6cf64517c618c41", null ],
    [ "SPI_SlaveTransferCreateHandle", "group__spi__driver.html#gab7e0ae7d0a88e7e1059b2cb0ed878b2e", null ],
    [ "SPI_SlaveTransferNonBlocking", "group__spi__driver.html#gae37917cb354e36acd1b1ca906d2ffe1c", null ],
    [ "SPI_SlaveTransferGetCount", "group__spi__driver.html#ga0dcd2ed8373d992ff93bb500e1442815", null ],
    [ "SPI_SlaveTransferAbort", "group__spi__driver.html#gae719048431a3e4fb334f83bd824a407b", null ],
    [ "SPI_SlaveTransferHandleIRQ", "group__spi__driver.html#gaac1ff8fc728c925d78b1237949ebf7c3", null ]
];