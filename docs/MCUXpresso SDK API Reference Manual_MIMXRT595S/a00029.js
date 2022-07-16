var a00029 =
[
    [ "FLEXIO_UART_Type", "a00029.html#a00359", [
      [ "flexioBase", "a00029.html#ad80f1e208b704cc3719933e2448826c4", null ],
      [ "TxPinIndex", "a00029.html#ab6df247340eafe9076c6ac3a20f0d81c", null ],
      [ "RxPinIndex", "a00029.html#a21879d73921f1cd42601b0a80d029a6a", null ],
      [ "shifterIndex", "a00029.html#a36f31f0641373f903d2371aafe42ee67", null ],
      [ "timerIndex", "a00029.html#a69c6696d06f94527af4280dc2921022b", null ]
    ] ],
    [ "flexio_uart_config_t", "a00029.html#a00357", [
      [ "enableUart", "a00029.html#aa2daf1e1468130631b21f73e9dcc2e95", null ],
      [ "enableInDoze", "a00029.html#ad2752bcddb3b5e9d987eb9d74f3e034b", null ],
      [ "enableInDebug", "a00029.html#ac5bf2792d85a76b3652dee457e580218", null ],
      [ "enableFastAccess", "a00029.html#a51bdfc0b1d8493123f1be71343c9f070", null ],
      [ "baudRate_Bps", "a00029.html#a082e38cf0edf524602d1d7a578949fd6", null ],
      [ "bitCountPerChar", "a00029.html#a31835537405f9f15cd37a10b17211990", null ]
    ] ],
    [ "flexio_uart_transfer_t", "a00029.html#a00358", [
      [ "data", "a00029.html#a38416ca93058d1875ae13843c6a63827", null ],
      [ "rxData", "a00029.html#a0bd75aa05139bc278313efc2871c9d2e", null ],
      [ "txData", "a00029.html#abe751737642b273833ad3abdd4f2fc81", null ],
      [ "dataSize", "a00029.html#a4d962516ae6cc6a2358c4175c154b5b8", null ]
    ] ],
    [ "flexio_uart_handle_t", "a00029.html#a00290", [
      [ "txData", "a00029.html#a1569356e818eea29940ad3f087a438b2", null ],
      [ "txDataSize", "a00029.html#ad464d30fa2304bbd122747ce92d88268", null ],
      [ "rxData", "a00029.html#aed331dee4e91b4554acc26b625cbe63f", null ],
      [ "rxDataSize", "a00029.html#a0aa6001272b9d5637fa272208489837b", null ],
      [ "txDataSizeAll", "a00029.html#ad4b82a12ba3e8d747eb78819e2862252", null ],
      [ "rxDataSizeAll", "a00029.html#a1ff5b94af87893988884a53ff8b212ce", null ],
      [ "rxRingBuffer", "a00029.html#af29ffdc8a03366ed88354e23466a3f83", null ],
      [ "rxRingBufferSize", "a00029.html#ac1e6450bbc80511733aaa4b1839c7479", null ],
      [ "rxRingBufferHead", "a00029.html#ac4be17b9c7516934956489b6c65f35f1", null ],
      [ "rxRingBufferTail", "a00029.html#a1b27a83bb89be05c44cf32184ad9bfb2", null ],
      [ "callback", "a00029.html#ae19e57fc92931bc751bc4c142c6335a6", null ],
      [ "userData", "a00029.html#ae547f7e39c3448956c7496801a7a853a", null ],
      [ "txState", "a00029.html#a8cb8e5214de0990c540ae29202773a72", null ],
      [ "rxState", "a00029.html#a7f4b0bf16597c2799d5f97eaa14168c7", null ]
    ] ],
    [ "FSL_FLEXIO_UART_DRIVER_VERSION", "a00029.html#gaa668f10e7e0820c4d504f124ef506b59", null ],
    [ "UART_RETRY_TIMES", "a00029.html#gac0f88b67f77fd05f186a5ec940c340bd", null ],
    [ "flexio_uart_transfer_callback_t", "a00029.html#ga47ba0f819db72b1dc4c043205ba7cb18", [
      [ "kStatus_FLEXIO_UART_TxBusy", "a00029.html#ggaba01db17f4a2bfbc3db60dc172972a25a741e184e1ca8ef870d70da2be537a6ab", null ],
      [ "kStatus_FLEXIO_UART_RxBusy", "a00029.html#ggaba01db17f4a2bfbc3db60dc172972a25a8dcfa64158a113e9ed598ac23c9df021", null ],
      [ "kStatus_FLEXIO_UART_TxIdle", "a00029.html#ggaba01db17f4a2bfbc3db60dc172972a25a828da21609507716299bfc8640c57ec3", null ],
      [ "kStatus_FLEXIO_UART_RxIdle", "a00029.html#ggaba01db17f4a2bfbc3db60dc172972a25a451ebbdc82bffc6c0205c7c9bd539046", null ],
      [ "kStatus_FLEXIO_UART_ERROR", "a00029.html#ggaba01db17f4a2bfbc3db60dc172972a25a3b7dca7a2a8364d24c686bbfc22aa2bc", null ],
      [ "kStatus_FLEXIO_UART_RxRingBufferOverrun", "a00029.html#ggaba01db17f4a2bfbc3db60dc172972a25a565cdecf3ec56df0f889e2e49d31b34f", null ],
      [ "kStatus_FLEXIO_UART_RxHardwareOverrun", "a00029.html#ggaba01db17f4a2bfbc3db60dc172972a25ab892c50df6c586138fe1b37fde07e4ca", null ],
      [ "kStatus_FLEXIO_UART_Timeout", "a00029.html#ggaba01db17f4a2bfbc3db60dc172972a25a71d490f8194635bf02969343de023a71", null ],
      [ "kStatus_FLEXIO_UART_BaudrateNotSupport", "a00029.html#ggaba01db17f4a2bfbc3db60dc172972a25a0b5de1470583ff444473745f1888fa14", null ]
    ] ],
    [ "flexio_uart_bit_count_per_char_t", "a00029.html#ga3298baaedfdeb4df8ad44ce716abca7e", [
      [ "kFLEXIO_UART_7BitsPerChar", "a00029.html#gga3298baaedfdeb4df8ad44ce716abca7ead9818673f2a8abacdb73e6f323fa4c42", null ],
      [ "kFLEXIO_UART_8BitsPerChar", "a00029.html#gga3298baaedfdeb4df8ad44ce716abca7eaa2563f1b9bbacb425bd892eedda0a161", null ],
      [ "kFLEXIO_UART_9BitsPerChar", "a00029.html#gga3298baaedfdeb4df8ad44ce716abca7ea8f9d374fa26cfa27dff950fca3e404a5", null ]
    ] ],
    [ "_flexio_uart_interrupt_enable", "a00029.html#gaacbc60527c13c7135ab8cdeb379e72fb", [
      [ "kFLEXIO_UART_TxDataRegEmptyInterruptEnable", "a00029.html#ggaacbc60527c13c7135ab8cdeb379e72fba41d4cc3f3cf42008e68fe819a9a3da14", null ],
      [ "kFLEXIO_UART_RxDataRegFullInterruptEnable", "a00029.html#ggaacbc60527c13c7135ab8cdeb379e72fba8228cbdfa6b84d76cd2e658ae4df09c0", null ]
    ] ],
    [ "_flexio_uart_status_flags", "a00029.html#gab80096f9e757f4083565dd183ce8c491", [
      [ "kFLEXIO_UART_TxDataRegEmptyFlag", "a00029.html#ggab80096f9e757f4083565dd183ce8c491a8fd298132aa5ee69800dfba7a224a252", null ],
      [ "kFLEXIO_UART_RxDataRegFullFlag", "a00029.html#ggab80096f9e757f4083565dd183ce8c491a3bc49d5b480f3a37acfcf82234c6f48b", null ],
      [ "kFLEXIO_UART_RxOverRunFlag", "a00029.html#ggab80096f9e757f4083565dd183ce8c491a139fb91d122113f9fde834253d930e26", null ]
    ] ],
    [ "FLEXIO_UART_Init", "a00029.html#ga8c9e0ca8c3517e21a27f7710d7d4f580", null ],
    [ "FLEXIO_UART_Deinit", "a00029.html#ga641ed5c6b8588064d82e5690e1994724", null ],
    [ "FLEXIO_UART_GetDefaultConfig", "a00029.html#gab705f673f07da7b1b385be7edfd2385e", null ],
    [ "FLEXIO_UART_GetStatusFlags", "a00029.html#gaefea3702f5a53a82246aaba191605716", null ],
    [ "FLEXIO_UART_ClearStatusFlags", "a00029.html#ga22b879f2d9e08524999a98c7a743c3b0", null ],
    [ "FLEXIO_UART_EnableInterrupts", "a00029.html#ga7f48f0a283e6458db7ff72c433e8a325", null ],
    [ "FLEXIO_UART_DisableInterrupts", "a00029.html#gaf4e18aa7772ae63d2eef7030cfa67175", null ],
    [ "FLEXIO_UART_GetTxDataRegisterAddress", "a00029.html#ga06b2b751b69190006ddbd4783abd5135", null ],
    [ "FLEXIO_UART_GetRxDataRegisterAddress", "a00029.html#gaa2b438423a09911bdc122109f2d11ced", null ],
    [ "FLEXIO_UART_EnableTxDMA", "a00029.html#gaa23cdab45bc3ed89eb9ac2998f7f339b", null ],
    [ "FLEXIO_UART_EnableRxDMA", "a00029.html#ga0d005d0a785cdc80bdc77a0232533706", null ],
    [ "FLEXIO_UART_Enable", "a00029.html#ga23a9bf703cf1c371fd362759ad1c66f9", null ],
    [ "FLEXIO_UART_WriteByte", "a00029.html#gaca10cded3e2fe4fb41af031874385775", null ],
    [ "FLEXIO_UART_ReadByte", "a00029.html#ga19a7990926d6e3ed6e5799ce9c060c7d", null ],
    [ "FLEXIO_UART_WriteBlocking", "a00029.html#gaa97558027d7fdf1475e46b65f0dd4d76", null ],
    [ "FLEXIO_UART_ReadBlocking", "a00029.html#ga8c6339871092770c2e7409eac3803fa8", null ],
    [ "FLEXIO_UART_TransferCreateHandle", "a00029.html#gadf3c35f0256cb25b15cb4005393b3431", null ],
    [ "FLEXIO_UART_TransferStartRingBuffer", "a00029.html#ga1c52d22628dd2ed7e843fc1cd5fb304c", null ],
    [ "FLEXIO_UART_TransferStopRingBuffer", "a00029.html#gaaabdc1cafc996c709cb5fc29b5de31bc", null ],
    [ "FLEXIO_UART_TransferSendNonBlocking", "a00029.html#gab1b90ca93c8e65eac0e1e62ce93e3b9d", null ],
    [ "FLEXIO_UART_TransferAbortSend", "a00029.html#ga141086618b4642de97e46fb92ce297be", null ],
    [ "FLEXIO_UART_TransferGetSendCount", "a00029.html#ga3e5937c17d139c33470e80c33db082bd", null ],
    [ "FLEXIO_UART_TransferReceiveNonBlocking", "a00029.html#gab42bbc26c0a6a8186d7b0d7373861bd4", null ],
    [ "FLEXIO_UART_TransferAbortReceive", "a00029.html#ga3530ff683b0a4a0c2c2822c1e3626aab", null ],
    [ "FLEXIO_UART_TransferGetReceiveCount", "a00029.html#ga3bc3658a7fb5825e12d2a74e879b9930", null ],
    [ "FLEXIO_UART_TransferHandleIRQ", "a00029.html#ga3c30d66f4471bfebd2021a8a3f1a1f04", null ]
];