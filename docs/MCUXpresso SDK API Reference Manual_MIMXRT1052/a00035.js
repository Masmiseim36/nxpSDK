var a00035 =
[
    [ "FlexIO eDMA SPI Driver", "a00036.html", "a00036" ],
    [ "FLEXIO_SPI_Type", "a00035.html#a00416", [
      [ "flexioBase", "a00035.html#a6f884e3d820db0fa2c2974d9e8feaca1", null ],
      [ "SDOPinIndex", "a00035.html#a5df20f69be0906774c5b3d9c06821868", null ],
      [ "SDIPinIndex", "a00035.html#a176d632a86bd48ee20db504ddb5ace5d", null ],
      [ "SCKPinIndex", "a00035.html#a5bd00a1e1450d172c1c651036af261bc", null ],
      [ "CSnPinIndex", "a00035.html#af3462a62b5058e897b7d8387daf30317", null ],
      [ "shifterIndex", "a00035.html#ad2ab114592f4a5982720447f8cd9e90d", null ],
      [ "timerIndex", "a00035.html#a952204114daf59aeeb9437af1abc6eef", null ]
    ] ],
    [ "flexio_spi_master_config_t", "a00035.html#a00413", [
      [ "enableMaster", "a00035.html#abfa7b62cfa5ecc303b203c9206e8511b", null ],
      [ "enableInDoze", "a00035.html#ace1bcfad14f01d682d72126fe8977379", null ],
      [ "enableInDebug", "a00035.html#ae25034f9e6018e4fb4688ed8867c6b18", null ],
      [ "enableFastAccess", "a00035.html#aca4f2130311c4dea49126cfba678669c", null ],
      [ "baudRate_Bps", "a00035.html#a6f95fac4a0afc21630b3837787bbb353", null ],
      [ "phase", "a00035.html#a76cac3e67183266b66cf25a29c2669bf", null ],
      [ "dataMode", "a00035.html#a1ed3e04044edd43a3ab1658b83cc7a70", null ]
    ] ],
    [ "flexio_spi_slave_config_t", "a00035.html#a00414", [
      [ "enableSlave", "a00035.html#a5fcf34758ddd907b1860679305858de1", null ],
      [ "enableInDoze", "a00035.html#ab13e01169ca99c72157d7e927e9c7d6f", null ],
      [ "enableInDebug", "a00035.html#af4180104efb170139a0e4ab731e38361", null ],
      [ "enableFastAccess", "a00035.html#a431d5f78a4038ee716d38905334e558e", null ],
      [ "phase", "a00035.html#a5ba29aaf41caf774f7077f1e8fe64404", null ],
      [ "dataMode", "a00035.html#a5b7c853f42ed1b2e53e07034daf3d26f", null ]
    ] ],
    [ "flexio_spi_transfer_t", "a00035.html#a00415", [
      [ "txData", "a00035.html#adce5c034b8842ec6a9d3c3d2f12f2b72", null ],
      [ "rxData", "a00035.html#a5a1a761e9cc6c90947ad55b701324258", null ],
      [ "dataSize", "a00035.html#a5e76062efe44914a5bbb2f4fc3ca3aac", null ],
      [ "flags", "a00035.html#a890b496be606905d29d43abedf7e753b", null ]
    ] ],
    [ "flexio_spi_master_handle_t", "a00035.html#a00312", [
      [ "txData", "a00035.html#a9a8bad99d5d80699be0d248092ac8d80", null ],
      [ "rxData", "a00035.html#a6f04ebbd2ad671dd6657be16c26bd1e6", null ],
      [ "transferSize", "a00035.html#aee24e8b68706ac6bb69c501295b8f289", null ],
      [ "txRemainingBytes", "a00035.html#aeecd6ae001c1bbdfab21eb288f0ca8a9", null ],
      [ "rxRemainingBytes", "a00035.html#aee7be6e3dd2109f9d5374a1b2fb45ade", null ],
      [ "state", "a00035.html#a25ee5785454882c9d0856f3a0fb8e23a", null ],
      [ "bytePerFrame", "a00035.html#ab0c81d09878eaff0b35189d7b7d4441a", null ],
      [ "direction", "a00035.html#a8ab5ab2070ea63f0c52d1c6da2786459", null ],
      [ "callback", "a00035.html#ac4118ced7c959f43005a9b4cdc48786d", null ],
      [ "userData", "a00035.html#a7d3fb8c3a3c3b23a1908c0bb70230166", null ]
    ] ],
    [ "FSL_FLEXIO_SPI_DRIVER_VERSION", "a00035.html#gadd4392d8b86be4236270866fa214dcbe", null ],
    [ "FLEXIO_SPI_DUMMYDATA", "a00035.html#gad411a43e379e70215b462831b24c5c5b", null ],
    [ "SPI_RETRY_TIMES", "a00035.html#gaa4717c15ca9604c505a5e5c60df29ebe", null ],
    [ "FLEXIO_SPI_XFER_DATA_FORMAT", "a00035.html#ga7b59f53366b1e5bb10a0491a20543382", null ],
    [ "flexio_spi_slave_handle_t", "a00035.html#ga1950678efd37cb3665e303e094cec428", null ],
    [ "flexio_spi_master_transfer_callback_t", "a00035.html#ga9c8b509b90588dea1921de0bdb3cfec8", null ],
    [ "flexio_spi_slave_transfer_callback_t", "a00035.html#gad2accadc980b51b0d65e41d122abe261", [
      [ "kStatus_FLEXIO_SPI_Busy", "a00035.html#gga0ed680fdb405e7195d9f14032851eebba9fd2c82e84cdc65b4c2d7c3d335bc4e1", null ],
      [ "kStatus_FLEXIO_SPI_Idle", "a00035.html#gga0ed680fdb405e7195d9f14032851eebba69ccc78c05f6f6ed17f4d48e20323ec8", null ],
      [ "kStatus_FLEXIO_SPI_Error", "a00035.html#gga0ed680fdb405e7195d9f14032851eebba4ae19baeae73d05d2dcf34dc4c54c6c4", null ],
      [ "kStatus_FLEXIO_SPI_Timeout", "a00035.html#gga0ed680fdb405e7195d9f14032851eebbae001cfaa37d2b750cd88808cbc3d7522", null ]
    ] ],
    [ "flexio_spi_clock_phase_t", "a00035.html#gac059394d6eabe169d3a1ee8a8b6f7edd", [
      [ "kFLEXIO_SPI_ClockPhaseFirstEdge", "a00035.html#ggac059394d6eabe169d3a1ee8a8b6f7eddad07a920661cea5317982da4405bfd9e1", null ],
      [ "kFLEXIO_SPI_ClockPhaseSecondEdge", "a00035.html#ggac059394d6eabe169d3a1ee8a8b6f7edda9d85a76b5ce670c1d47e5f95f4a46e6c", null ]
    ] ],
    [ "flexio_spi_shift_direction_t", "a00035.html#ga25594db149526d67b23d92724487e87f", [
      [ "kFLEXIO_SPI_MsbFirst", "a00035.html#gga25594db149526d67b23d92724487e87fad6c96f26bda5bc22af19be1e7906f3f2", null ],
      [ "kFLEXIO_SPI_LsbFirst", "a00035.html#gga25594db149526d67b23d92724487e87faa2a981323f199d9359c61d39cb059795", null ]
    ] ],
    [ "flexio_spi_data_bitcount_mode_t", "a00035.html#gae4e518a5b077982fca7b6bf1025c7aaf", [
      [ "kFLEXIO_SPI_8BitMode", "a00035.html#ggae4e518a5b077982fca7b6bf1025c7aafaef0c633539e44547b47ad7b754234752", null ],
      [ "kFLEXIO_SPI_16BitMode", "a00035.html#ggae4e518a5b077982fca7b6bf1025c7aafa343c7a9db539019f0c99fb82b4adab35", null ],
      [ "kFLEXIO_SPI_32BitMode", "a00035.html#ggae4e518a5b077982fca7b6bf1025c7aafa7f4a057b24ac43b60fa4795d2a79b25a", null ]
    ] ],
    [ "_flexio_spi_interrupt_enable", "a00035.html#ga239e9a0ff79e0d36ecf6ae7c68dfd357", [
      [ "kFLEXIO_SPI_TxEmptyInterruptEnable", "a00035.html#gga239e9a0ff79e0d36ecf6ae7c68dfd357a5243ceb746eebfdeef2ba2d791e32ff9", null ],
      [ "kFLEXIO_SPI_RxFullInterruptEnable", "a00035.html#gga239e9a0ff79e0d36ecf6ae7c68dfd357a3431c6c1875a70da30cba8b4185ad183", null ]
    ] ],
    [ "_flexio_spi_status_flags", "a00035.html#ga5dec706a17cf2b92ef701f684503d4b2", [
      [ "kFLEXIO_SPI_TxBufferEmptyFlag", "a00035.html#gga5dec706a17cf2b92ef701f684503d4b2a80c0db2a058d82d018b2e8bc76f1aa6b", null ],
      [ "kFLEXIO_SPI_RxBufferFullFlag", "a00035.html#gga5dec706a17cf2b92ef701f684503d4b2aa679f6244353e98ace08ec5d0281851b", null ]
    ] ],
    [ "_flexio_spi_dma_enable", "a00035.html#ga713aa7c7a59e84b9fdafb2923de177ef", [
      [ "kFLEXIO_SPI_TxDmaEnable", "a00035.html#gga713aa7c7a59e84b9fdafb2923de177efa4cdf95c3f7a1b9ae7931166a8abe1542", null ],
      [ "kFLEXIO_SPI_RxDmaEnable", "a00035.html#gga713aa7c7a59e84b9fdafb2923de177efa002de6d16630e5d11d94b871da9a8fbd", null ],
      [ "kFLEXIO_SPI_DmaAllEnable", "a00035.html#gga713aa7c7a59e84b9fdafb2923de177efa6b4ccfcf63573b0a3b281c32c4913f49", null ]
    ] ],
    [ "_flexio_spi_transfer_flags", "a00035.html#gabf4ebfbc0d68b969e1b7dd838c45512d", [
      [ "kFLEXIO_SPI_8bitMsb", "a00035.html#ggabf4ebfbc0d68b969e1b7dd838c45512da7979c2a48d6662af1777827c1afa5537", null ],
      [ "kFLEXIO_SPI_8bitLsb", "a00035.html#ggabf4ebfbc0d68b969e1b7dd838c45512da1d31a5996c45efebdc7aeb1a894647f8", null ],
      [ "kFLEXIO_SPI_16bitMsb", "a00035.html#ggabf4ebfbc0d68b969e1b7dd838c45512dabeb4baa55114987385ab398ceda142f9", null ],
      [ "kFLEXIO_SPI_16bitLsb", "a00035.html#ggabf4ebfbc0d68b969e1b7dd838c45512dad6774d8673e5aa247be136e38c8fad91", null ],
      [ "kFLEXIO_SPI_32bitMsb", "a00035.html#ggabf4ebfbc0d68b969e1b7dd838c45512da79f81ba5984c5821bf9c42b0f3a3a4c1", null ],
      [ "kFLEXIO_SPI_32bitLsb", "a00035.html#ggabf4ebfbc0d68b969e1b7dd838c45512daca30199b74b090f722c32b03b2762ac9", null ],
      [ "kFLEXIO_SPI_csContinuous", "a00035.html#ggabf4ebfbc0d68b969e1b7dd838c45512da9ee2a3142a3bc89e1ff4d645d9118287", null ]
    ] ],
    [ "FLEXIO_SPI_MasterInit", "a00035.html#gacc68d67c46d897e6e9b2268c0fe999d2", null ],
    [ "FLEXIO_SPI_MasterDeinit", "a00035.html#gae403f64c6c1268e5dfb62e0b20bd267c", null ],
    [ "FLEXIO_SPI_MasterGetDefaultConfig", "a00035.html#ga2d99f8a94f95262fc821b42e3812ee7e", null ],
    [ "FLEXIO_SPI_SlaveInit", "a00035.html#gae14521e2d88c63c34a51a31c726bac48", null ],
    [ "FLEXIO_SPI_SlaveDeinit", "a00035.html#gab02c0d21cfe44e81a2668facccdc587c", null ],
    [ "FLEXIO_SPI_SlaveGetDefaultConfig", "a00035.html#gaa6979876d2c0b4f65deabd6f5ce16d41", null ],
    [ "FLEXIO_SPI_GetStatusFlags", "a00035.html#ga1b709c34a5861a755ef2365eca4aef6d", null ],
    [ "FLEXIO_SPI_ClearStatusFlags", "a00035.html#ga53311c201694351c61d5d56f2924d73e", null ],
    [ "FLEXIO_SPI_EnableInterrupts", "a00035.html#ga877ffb51ccf9f5a2ecd5bd5a23f208c3", null ],
    [ "FLEXIO_SPI_DisableInterrupts", "a00035.html#gac765a8199e5d7c7546d2b31c06563638", null ],
    [ "FLEXIO_SPI_EnableDMA", "a00035.html#ga584eae5056ceeee0aea9ba9d5e2302ff", null ],
    [ "FLEXIO_SPI_GetTxDataRegisterAddress", "a00035.html#gac2629d5c8821fe5a2a3496b51dbc99f5", null ],
    [ "FLEXIO_SPI_GetRxDataRegisterAddress", "a00035.html#ga061dcced42575767baf53f6b6e5da609", null ],
    [ "FLEXIO_SPI_Enable", "a00035.html#ga68656f6d77e46b59e311e1a97f3b5878", null ],
    [ "FLEXIO_SPI_MasterSetBaudRate", "a00035.html#gaa9287322d22895ac0352db5fe0dd909e", null ],
    [ "FLEXIO_SPI_WriteData", "a00035.html#gab8d90deb9e0a4ae7ee4564125218ed29", null ],
    [ "FLEXIO_SPI_ReadData", "a00035.html#ga14411234ecd9a094371d27405ad990cd", null ],
    [ "FLEXIO_SPI_WriteBlocking", "a00035.html#ga0ddf107d6d5a9d4b6b33a7a07a7679b2", null ],
    [ "FLEXIO_SPI_ReadBlocking", "a00035.html#gaeead8252ae85745aa02f56baffb4df12", null ],
    [ "FLEXIO_SPI_MasterTransferBlocking", "a00035.html#gae02f160899c2ccb93651e4bac473b65e", null ],
    [ "FLEXIO_SPI_FlushShifters", "a00035.html#ga4de03d39977efdcd5292cfb8f0da2189", null ],
    [ "FLEXIO_SPI_MasterTransferCreateHandle", "a00035.html#ga0123f1825a83093ad07377d86f4e2bf1", null ],
    [ "FLEXIO_SPI_MasterTransferNonBlocking", "a00035.html#gabcfcbee985cb49bdbd02ab1698d2c041", null ],
    [ "FLEXIO_SPI_MasterTransferAbort", "a00035.html#ga7ee4149cd9c0bab50f91eaf6322ccd45", null ],
    [ "FLEXIO_SPI_MasterTransferGetCount", "a00035.html#ga5f8d610d6492109468d6a225ef01043d", null ],
    [ "FLEXIO_SPI_MasterTransferHandleIRQ", "a00035.html#ga39784af602890dbc1aff6d23b29b7e19", null ],
    [ "FLEXIO_SPI_SlaveTransferCreateHandle", "a00035.html#ga76d12767d4023268a3aa9cde5e072b29", null ],
    [ "FLEXIO_SPI_SlaveTransferNonBlocking", "a00035.html#ga9d1ef8b373c56f48d98612f5c3047f8c", null ],
    [ "FLEXIO_SPI_SlaveTransferAbort", "a00035.html#ga34dea4bdd2c52597711068bed5c45958", null ],
    [ "FLEXIO_SPI_SlaveTransferGetCount", "a00035.html#gae3354dda8f6fdce0f6a58b1933d56ee9", null ],
    [ "FLEXIO_SPI_SlaveTransferHandleIRQ", "a00035.html#gaf620bb7c2bf87c781f93f8a5f23c49d4", null ]
];