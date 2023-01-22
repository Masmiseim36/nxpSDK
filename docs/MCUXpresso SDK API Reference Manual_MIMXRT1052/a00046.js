var a00046 =
[
    [ "lpspi_master_config_t", "a00046.html#a00431", [
      [ "baudRate", "a00046.html#a01254e88cf6feef124ab50f204f65c0f", null ],
      [ "bitsPerFrame", "a00046.html#afc83880a5e4d763e80cd757514c96167", null ],
      [ "cpol", "a00046.html#a3c540678d6c2cce2664ba89cc998a1e2", null ],
      [ "cpha", "a00046.html#ab18c81f8ad0554de46b971ca5cbe64e3", null ],
      [ "direction", "a00046.html#a94d466823ac6dabda080afcf564bccf2", null ],
      [ "pcsToSckDelayInNanoSec", "a00046.html#a672a67f6598757ad94adbfdbdec8cf02", null ],
      [ "lastSckToPcsDelayInNanoSec", "a00046.html#a351819436a1a3ac8703d68929dd37bcf", null ],
      [ "betweenTransferDelayInNanoSec", "a00046.html#a7f9e3aa5c501504d86049e20d69d53eb", null ],
      [ "whichPcs", "a00046.html#ac5514f7f0b043d78c956874d968c95f4", null ],
      [ "pcsActiveHighOrLow", "a00046.html#a7da7b2e2a9a0e959b0f4c5f7b1618c48", null ],
      [ "pinCfg", "a00046.html#a52fed669c402348f070df8322e4bc8b1", null ],
      [ "dataOutConfig", "a00046.html#acc0422bcb70e685bb66f3a216aa66088", null ],
      [ "enableInputDelay", "a00046.html#a1b0faf93c78ceb12d7e12dc036c930c0", null ]
    ] ],
    [ "lpspi_slave_config_t", "a00046.html#a00432", [
      [ "bitsPerFrame", "a00046.html#a0f74a7ed1b4226f612a3785b07d09c86", null ],
      [ "cpol", "a00046.html#aea9800ee270ac89ec3deb4f2399a92a9", null ],
      [ "cpha", "a00046.html#ac5063bcaacbddbd5753ffe6a2df2a7df", null ],
      [ "direction", "a00046.html#a50f59c5c9ac64edcd21948b96a64aaf6", null ],
      [ "whichPcs", "a00046.html#a05851922df80227e2063e97a9f4bfe4e", null ],
      [ "pcsActiveHighOrLow", "a00046.html#a13cba6329bfbee143681307b77b1a858", null ],
      [ "pinCfg", "a00046.html#ab1b812e03a9b2785efd4e9481c20730c", null ],
      [ "dataOutConfig", "a00046.html#af8879b89ada464a0d8b16c33639e87cf", null ]
    ] ],
    [ "lpspi_transfer_t", "a00046.html#a00433", [
      [ "txData", "a00046.html#ad05c41e2213388411ba65b74b2e296e8", null ],
      [ "rxData", "a00046.html#adcb9063db0070da8c7b2be3aad07a901", null ],
      [ "dataSize", "a00046.html#a00cc53b0e0d0013871c255e17a8fe9b0", null ],
      [ "configFlags", "a00046.html#acea70ac01d69df22aa840e5ed44f9c97", null ]
    ] ],
    [ "lpspi_master_handle_t", "a00046.html#a00321", [
      [ "isPcsContinuous", "a00046.html#a9615ae8b8bdc4f2bd7d22b3b96d0ced4", null ],
      [ "writeTcrInIsr", "a00046.html#a088fc657556d03d009908496438a1e89", null ],
      [ "isByteSwap", "a00046.html#ae03069cfdcf680ee5fd81e077b81bc18", null ],
      [ "isTxMask", "a00046.html#a760fd7a5668b0b06e866cef22ddf916d", null ],
      [ "bytesPerFrame", "a00046.html#a3206fac15c5c0300ea31c5a5b3b9e6a7", null ],
      [ "fifoSize", "a00046.html#a5394b495a52e7e6b25cca18e7cc675d8", null ],
      [ "rxWatermark", "a00046.html#a765fce2e32c6c69d35cc96e2f1302437", null ],
      [ "bytesEachWrite", "a00046.html#a96b6240609ba91a5de282789388aadc4", null ],
      [ "bytesEachRead", "a00046.html#a5801d46c80390b39a0773d4b7fed351c", null ],
      [ "txData", "a00046.html#ae9740763483ca12b7256968bf862bb44", null ],
      [ "rxData", "a00046.html#a495acfab6f6174047897b9ae65bc083d", null ],
      [ "txRemainingByteCount", "a00046.html#a11a011a2ae2fa26ce188248a388dffe5", null ],
      [ "rxRemainingByteCount", "a00046.html#a61a204896d0204b4438fc50f54d182ac", null ],
      [ "writeRegRemainingTimes", "a00046.html#a3ddcbddf19f549c5985893a59ac15461", null ],
      [ "readRegRemainingTimes", "a00046.html#a448c0c07c192ec4804485b94fa770d55", null ],
      [ "totalByteCount", "a00046.html#aed87faabce1af157c8b29194ee279323", null ],
      [ "txBuffIfNull", "a00046.html#ac0530d40b36d139cf304110c5e7562ca", null ],
      [ "state", "a00046.html#aca16074b4a19b2e52e32bbcc3b42d1c0", null ],
      [ "callback", "a00046.html#aaa5468f579c64b865d2870a6e28608fb", null ],
      [ "userData", "a00046.html#ad06b10280d203a26ee97ad860360cb3b", null ]
    ] ],
    [ "lpspi_slave_handle_t", "a00046.html#a00323", [
      [ "isByteSwap", "a00046.html#aed92e8549294bcdf1ef261ce8f261983", null ],
      [ "fifoSize", "a00046.html#abd595c5df400d5761c3fe88624524bd7", null ],
      [ "rxWatermark", "a00046.html#a0edea4c3bad24d2dc0a0669ca6acc16c", null ],
      [ "bytesEachWrite", "a00046.html#aefa8c0b258a39a808514b1a129b71cf7", null ],
      [ "bytesEachRead", "a00046.html#a360b1761ed7190a69b7378cb26cc5f7b", null ],
      [ "txData", "a00046.html#a6f303a57110b7f721f321a32048c79f5", null ],
      [ "rxData", "a00046.html#ac6e8aaec21e77721e3a3c451ecfc67d5", null ],
      [ "txRemainingByteCount", "a00046.html#a93e4e7adeea732e422ff94a30665584d", null ],
      [ "rxRemainingByteCount", "a00046.html#afcdbb7552d9e668ff7bcc6f49e1c46bf", null ],
      [ "writeRegRemainingTimes", "a00046.html#aeda43561d34b6c55c3f4fbf0ffef0991", null ],
      [ "readRegRemainingTimes", "a00046.html#a3275c8fb058a5575fa6ba2a4c7e78ea6", null ],
      [ "totalByteCount", "a00046.html#a0bfb15cc668eccd54a0601c62040bb96", null ],
      [ "state", "a00046.html#a5fe4d552b14da651409e50f60c9fa154", null ],
      [ "errorCount", "a00046.html#aef7cd081368ada64bb02b319fde9d623", null ],
      [ "callback", "a00046.html#ab4962e48e3f344b0f18d6575868a95e5", null ],
      [ "userData", "a00046.html#afc8a4565bdde97f5a7db4e6e64dd430a", null ]
    ] ],
    [ "FSL_LPSPI_DRIVER_VERSION", "a00046.html#gad5ff034e3f900bf381ebfe245b10070b", null ],
    [ "LPSPI_DUMMY_DATA", "a00046.html#ga6778201230734cd2564d1860bf8238c9", null ],
    [ "SPI_RETRY_TIMES", "a00046.html#gaa4717c15ca9604c505a5e5c60df29ebe", null ],
    [ "LPSPI_MASTER_PCS_SHIFT", "a00046.html#gaa83009bb8d3485984bbda804eef3a1e5", null ],
    [ "LPSPI_MASTER_PCS_MASK", "a00046.html#ga00b2709c8cb35f28de79e13a46f625ef", null ],
    [ "LPSPI_SLAVE_PCS_SHIFT", "a00046.html#gab14deb887bd54221e34f013a2e7afcf7", null ],
    [ "LPSPI_SLAVE_PCS_MASK", "a00046.html#gac143084740a5ba788e1ac73fefc6f5e4", null ],
    [ "lpspi_master_transfer_callback_t", "a00046.html#gaeecf1622c161a2d202eace4a4699dc3a", null ],
    [ "lpspi_slave_transfer_callback_t", "a00046.html#ga4bf812d09749691f41bf7cbcb62c05a8", [
      [ "kStatus_LPSPI_Busy", "a00046.html#ggac205be2172292384dd687b5471a87edda7f12633826bd7df839619b14c0ca28cc", null ],
      [ "kStatus_LPSPI_Error", "a00046.html#ggac205be2172292384dd687b5471a87edda3faf60052aef160c9e665b923f6560f0", null ],
      [ "kStatus_LPSPI_Idle", "a00046.html#ggac205be2172292384dd687b5471a87edda44467ff9577a1be6b13121e8f075d92a", null ],
      [ "kStatus_LPSPI_OutOfRange", "a00046.html#ggac205be2172292384dd687b5471a87edda2f2d2fc26087dd6282b229e1c7471758", null ],
      [ "kStatus_LPSPI_Timeout", "a00046.html#ggac205be2172292384dd687b5471a87edda121095b6580099f086f494d25186a45d", null ]
    ] ],
    [ "_lpspi_flags", "a00046.html#gad97e399bf4883a243d83fc7cffb19655", [
      [ "kLPSPI_TxDataRequestFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655a3947665fd37fb039123f594df926cff1", null ],
      [ "kLPSPI_RxDataReadyFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655a5bdd7020ceafc2b5cbfa6e9ffa72b1cb", null ],
      [ "kLPSPI_WordCompleteFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655ad84b2113adf471177ecee7d7de5f82e0", null ],
      [ "kLPSPI_FrameCompleteFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655acec0a8ef85d641ef6589d4deea14ce21", null ],
      [ "kLPSPI_TransferCompleteFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655a4c1300adf80a19668a729a9b9f1c3660", null ],
      [ "kLPSPI_TransmitErrorFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655a240267c0100041bbeb6acbae45a37b15", null ],
      [ "kLPSPI_ReceiveErrorFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655aad86acfadef3dc8df91849884ec355e3", null ],
      [ "kLPSPI_DataMatchFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655a53fa68713ae51b053362e99930b1f841", null ],
      [ "kLPSPI_ModuleBusyFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655a71a15d927635fc51125dadf7ad1167a8", null ],
      [ "kLPSPI_AllStatusFlag", "a00046.html#ggad97e399bf4883a243d83fc7cffb19655af29f28d13e798c1fcbce009e203a1fed", null ]
    ] ],
    [ "_lpspi_interrupt_enable", "a00046.html#gab80d8a605a7ba992043ba4e6afa501a3", [
      [ "kLPSPI_TxInterruptEnable", "a00046.html#ggab80d8a605a7ba992043ba4e6afa501a3a2d20e000503ba5c5a0c9f67c60a714cd", null ],
      [ "kLPSPI_RxInterruptEnable", "a00046.html#ggab80d8a605a7ba992043ba4e6afa501a3a2b9ff66ccb9229355668bdcc05f77dc9", null ],
      [ "kLPSPI_WordCompleteInterruptEnable", "a00046.html#ggab80d8a605a7ba992043ba4e6afa501a3a73f0e5f45806e247e46bf7244695693e", null ],
      [ "kLPSPI_FrameCompleteInterruptEnable", "a00046.html#ggab80d8a605a7ba992043ba4e6afa501a3a0c0dde3048f3d25c56eac65acaceb49b", null ],
      [ "kLPSPI_TransferCompleteInterruptEnable", "a00046.html#ggab80d8a605a7ba992043ba4e6afa501a3ac0d5fa696784facb2fbaf2c01e7569bd", null ],
      [ "kLPSPI_TransmitErrorInterruptEnable", "a00046.html#ggab80d8a605a7ba992043ba4e6afa501a3a23f8ac8e04b9cd32f458727ec1ed38ad", null ],
      [ "kLPSPI_ReceiveErrorInterruptEnable", "a00046.html#ggab80d8a605a7ba992043ba4e6afa501a3a0433263f892d70eb2859746b075c14e7", null ],
      [ "kLPSPI_DataMatchInterruptEnable", "a00046.html#ggab80d8a605a7ba992043ba4e6afa501a3a6032909768af91efcdb432843f48047b", null ],
      [ "kLPSPI_AllInterruptEnable", "a00046.html#ggab80d8a605a7ba992043ba4e6afa501a3a2a8a671a096982bdf93ddba6d146d5d6", null ]
    ] ],
    [ "_lpspi_dma_enable", "a00046.html#gafed26b4ba584824e51e91b4040591cf2", [
      [ "kLPSPI_TxDmaEnable", "a00046.html#ggafed26b4ba584824e51e91b4040591cf2a527e96eef74618b0b40c30cf6a1a2828", null ],
      [ "kLPSPI_RxDmaEnable", "a00046.html#ggafed26b4ba584824e51e91b4040591cf2aea817767e6bd800ff275458f07f363db", null ]
    ] ],
    [ "lpspi_master_slave_mode_t", "a00046.html#gae28cea6ce513555e39a3ac343d05ad52", [
      [ "kLPSPI_Master", "a00046.html#ggae28cea6ce513555e39a3ac343d05ad52a5f5755c293c4eea06b35528c7dadb505", null ],
      [ "kLPSPI_Slave", "a00046.html#ggae28cea6ce513555e39a3ac343d05ad52ac2fbf07d3dcc5fd91629c58bec8aa5c4", null ]
    ] ],
    [ "lpspi_which_pcs_t", "a00046.html#ga90ad4c281c99433627f8cccf56b2a653", [
      [ "kLPSPI_Pcs0", "a00046.html#gga90ad4c281c99433627f8cccf56b2a653ad0a33b6673cd3a9e54ab55f5d5138871", null ],
      [ "kLPSPI_Pcs1", "a00046.html#gga90ad4c281c99433627f8cccf56b2a653a276b09136d3d15a0fe0dc5d0bab9b3f1", null ],
      [ "kLPSPI_Pcs2", "a00046.html#gga90ad4c281c99433627f8cccf56b2a653a2d4d2611c76b7832b346d00072f49561", null ],
      [ "kLPSPI_Pcs3", "a00046.html#gga90ad4c281c99433627f8cccf56b2a653a91ecfb74e3c582725faea8aa28700590", null ]
    ] ],
    [ "lpspi_pcs_polarity_config_t", "a00046.html#ga6303bce5095876c64d47e3b19e76bd97", [
      [ "kLPSPI_PcsActiveHigh", "a00046.html#gga6303bce5095876c64d47e3b19e76bd97a5be0e938a788cff5717c2c2590e52c59", null ],
      [ "kLPSPI_PcsActiveLow", "a00046.html#gga6303bce5095876c64d47e3b19e76bd97a4a1448bee9f6f3d4383fe13cdc1100ce", null ]
    ] ],
    [ "_lpspi_pcs_polarity", "a00046.html#ga69d39d2c7c0054aa76e8f3282c98cc43", [
      [ "kLPSPI_Pcs0ActiveLow", "a00046.html#gga69d39d2c7c0054aa76e8f3282c98cc43a39ba76078eacda5cd1067083e1424219", null ],
      [ "kLPSPI_Pcs1ActiveLow", "a00046.html#gga69d39d2c7c0054aa76e8f3282c98cc43a9fba36c05ea23b988a6d6eea57111cc4", null ],
      [ "kLPSPI_Pcs2ActiveLow", "a00046.html#gga69d39d2c7c0054aa76e8f3282c98cc43a59994c42bc8d6a84754db3eb615b9a52", null ],
      [ "kLPSPI_Pcs3ActiveLow", "a00046.html#gga69d39d2c7c0054aa76e8f3282c98cc43a7d590e420c3d77192425d459ef47cb18", null ],
      [ "kLPSPI_PcsAllActiveLow", "a00046.html#gga69d39d2c7c0054aa76e8f3282c98cc43a7bfc7ab3ae077df8f7dedd69caefa159", null ]
    ] ],
    [ "lpspi_clock_polarity_t", "a00046.html#gaafc42f4a3c8eb16d4777f3c6f0f8654c", [
      [ "kLPSPI_ClockPolarityActiveHigh", "a00046.html#ggaafc42f4a3c8eb16d4777f3c6f0f8654ca5f5af0b1696c75bddcc45548dc6eb4e5", null ],
      [ "kLPSPI_ClockPolarityActiveLow", "a00046.html#ggaafc42f4a3c8eb16d4777f3c6f0f8654ca8d0257641a93e1539be2284aa8dea2c2", null ]
    ] ],
    [ "lpspi_clock_phase_t", "a00046.html#ga00a7c64b74442ff1961346c0297f9ab3", [
      [ "kLPSPI_ClockPhaseFirstEdge", "a00046.html#gga00a7c64b74442ff1961346c0297f9ab3a1c127121f13c6756c19c6640a1a3f66f", null ],
      [ "kLPSPI_ClockPhaseSecondEdge", "a00046.html#gga00a7c64b74442ff1961346c0297f9ab3a69541d5ec76b63987c1523f67f281039", null ]
    ] ],
    [ "lpspi_shift_direction_t", "a00046.html#ga4116edfccfda3a0788a9b68bb9b50fcd", [
      [ "kLPSPI_MsbFirst", "a00046.html#gga4116edfccfda3a0788a9b68bb9b50fcda6cfcf70021ed17b06148973a577b06b0", null ],
      [ "kLPSPI_LsbFirst", "a00046.html#gga4116edfccfda3a0788a9b68bb9b50fcdaa258be180c87a702892e74e80612bcac", null ]
    ] ],
    [ "lpspi_host_request_select_t", "a00046.html#ga9179eac6fe5a70f9350df669a4547b57", [
      [ "kLPSPI_HostReqExtPin", "a00046.html#gga9179eac6fe5a70f9350df669a4547b57a1ecf2a9ee61a10a6abfd3b003ecded95", null ],
      [ "kLPSPI_HostReqInternalTrigger", "a00046.html#gga9179eac6fe5a70f9350df669a4547b57ae7bcf394055c5c96962fbdeb59473b53", null ]
    ] ],
    [ "lpspi_match_config_t", "a00046.html#gac44bb22e39759dab2695974b9e232396", [
      [ "kLPSI_MatchDisabled", "a00046.html#ggac44bb22e39759dab2695974b9e232396a05d1b5c903dfe4898c9b1fd684250294", null ],
      [ "kLPSI_1stWordEqualsM0orM1", "a00046.html#ggac44bb22e39759dab2695974b9e232396a17bfedfd52cb4ad0b4806b0867a04e89", null ],
      [ "kLPSI_AnyWordEqualsM0orM1", "a00046.html#ggac44bb22e39759dab2695974b9e232396ae98eab6b132ff11c1df102e8e6cdd24c", null ],
      [ "kLPSI_1stWordEqualsM0and2ndWordEqualsM1", "a00046.html#ggac44bb22e39759dab2695974b9e232396a11c22b22c672c03591c560a6c8faf428", null ],
      [ "kLPSI_AnyWordEqualsM0andNxtWordEqualsM1", "a00046.html#ggac44bb22e39759dab2695974b9e232396a67938e63bf8168ce35e090a2b210ad2e", null ],
      [ "kLPSI_1stWordAndM1EqualsM0andM1", "a00046.html#ggac44bb22e39759dab2695974b9e232396a20ae6796a58bde43ba80737927e905ab", null ],
      [ "kLPSI_AnyWordAndM1EqualsM0andM1", "a00046.html#ggac44bb22e39759dab2695974b9e232396a02b507b2df58ba42c650727fc0a1cfa5", null ]
    ] ],
    [ "lpspi_pin_config_t", "a00046.html#gae5ea17a161935f47c786934c524bb2af", [
      [ "kLPSPI_SdiInSdoOut", "a00046.html#ggae5ea17a161935f47c786934c524bb2afa25d21ca2f0397f7bf6c23a1b1c6691f7", null ],
      [ "kLPSPI_SdiInSdiOut", "a00046.html#ggae5ea17a161935f47c786934c524bb2afa1e5af86e47569305db258791cb467d30", null ],
      [ "kLPSPI_SdoInSdoOut", "a00046.html#ggae5ea17a161935f47c786934c524bb2afa33c599830166b775f368ee16ff0682d5", null ],
      [ "kLPSPI_SdoInSdiOut", "a00046.html#ggae5ea17a161935f47c786934c524bb2afa43f7557904b7103eeb4516624d0c995c", null ]
    ] ],
    [ "lpspi_data_out_config_t", "a00046.html#ga134cbe42b20647e752e66aaeabbc10d4", [
      [ "kLpspiDataOutRetained", "a00046.html#gga134cbe42b20647e752e66aaeabbc10d4a82a3d2bf52e339c016ea5b55b6e5a0b5", null ],
      [ "kLpspiDataOutTristate", "a00046.html#gga134cbe42b20647e752e66aaeabbc10d4ad88e6541ea02827476f10ca3a7906129", null ]
    ] ],
    [ "lpspi_transfer_width_t", "a00046.html#gaa5d9038e390c8a7610dc72375da252a7", [
      [ "kLPSPI_SingleBitXfer", "a00046.html#ggaa5d9038e390c8a7610dc72375da252a7a56986005418db6d0910782c58ed92dc0", null ],
      [ "kLPSPI_TwoBitXfer", "a00046.html#ggaa5d9038e390c8a7610dc72375da252a7aeb5365dd5861043f098db28e9945c191", null ],
      [ "kLPSPI_FourBitXfer", "a00046.html#ggaa5d9038e390c8a7610dc72375da252a7a591c0c33b13b3fc9fe8d05bb14e5176a", null ]
    ] ],
    [ "lpspi_delay_type_t", "a00046.html#gae5e0bf71f3d28f35327f0268039d0c0c", [
      [ "kLPSPI_PcsToSck", "a00046.html#ggae5e0bf71f3d28f35327f0268039d0c0ca3d71d8a9758a52226844e7b721f3dfb8", null ],
      [ "kLPSPI_LastSckToPcs", "a00046.html#ggae5e0bf71f3d28f35327f0268039d0c0ca4c475db384f87476967ad8318fc8e941", null ],
      [ "kLPSPI_BetweenTransfer", "a00046.html#ggae5e0bf71f3d28f35327f0268039d0c0caec200ab5e56e72854f694bc65846e015", null ]
    ] ],
    [ "_lpspi_transfer_config_flag_for_master", "a00046.html#ga8c2566783079ecdbb478f06658375005", [
      [ "kLPSPI_MasterPcs0", "a00046.html#gga8c2566783079ecdbb478f06658375005ad472cdb233d16b4b9e5372c214e34a3c", null ],
      [ "kLPSPI_MasterPcs1", "a00046.html#gga8c2566783079ecdbb478f06658375005a1b57aa21038a9d2fe7affdda155a21d0", null ],
      [ "kLPSPI_MasterPcs2", "a00046.html#gga8c2566783079ecdbb478f06658375005abd22976ae5150a0f57f27f439d25e6d0", null ],
      [ "kLPSPI_MasterPcs3", "a00046.html#gga8c2566783079ecdbb478f06658375005a38ce619f68339f607e63affd7ce012a5", null ],
      [ "kLPSPI_MasterPcsContinuous", "a00046.html#gga8c2566783079ecdbb478f06658375005a81e8942cb9a3dc01b6900fa0fc44a807", null ],
      [ "kLPSPI_MasterByteSwap", "a00046.html#gga8c2566783079ecdbb478f06658375005aada04eea6a730a7536682944531385bb", null ]
    ] ],
    [ "_lpspi_transfer_config_flag_for_slave", "a00046.html#gaf3ef07696513a970ce8195b5b0f1a8c1", [
      [ "kLPSPI_SlavePcs0", "a00046.html#ggaf3ef07696513a970ce8195b5b0f1a8c1ab1faad46d523e037048a3064893ae07c", null ],
      [ "kLPSPI_SlavePcs1", "a00046.html#ggaf3ef07696513a970ce8195b5b0f1a8c1a0e6aee8fe7dfd9c2d66b505d73c37232", null ],
      [ "kLPSPI_SlavePcs2", "a00046.html#ggaf3ef07696513a970ce8195b5b0f1a8c1ae64e50be4483554dc3ebc66c379de03e", null ],
      [ "kLPSPI_SlavePcs3", "a00046.html#ggaf3ef07696513a970ce8195b5b0f1a8c1a6d5cce71f6a0551128e551100bc9f39f", null ],
      [ "kLPSPI_SlaveByteSwap", "a00046.html#ggaf3ef07696513a970ce8195b5b0f1a8c1a42d91e6c56fda80853954fdf01365998", null ]
    ] ],
    [ "_lpspi_transfer_state", "a00046.html#ga7718265fdc39522b101e1deb430ab1f7", [
      [ "kLPSPI_Idle", "a00046.html#gga7718265fdc39522b101e1deb430ab1f7ad1366ce0c1c0c8a66a6ee9611cb4ae8c", null ],
      [ "kLPSPI_Busy", "a00046.html#gga7718265fdc39522b101e1deb430ab1f7a81465b4e8946a9a8b3634e4fc04933fd", null ],
      [ "kLPSPI_Error", "a00046.html#gga7718265fdc39522b101e1deb430ab1f7a9f52ab17cca4320266e1b7c194df33ec", null ]
    ] ],
    [ "LPSPI_MasterInit", "a00046.html#ga22ae05bf7948761752a425f273423af4", null ],
    [ "LPSPI_MasterGetDefaultConfig", "a00046.html#gaf7c865e3ddf8b0e8b39eaa044ebdd7a9", null ],
    [ "LPSPI_SlaveInit", "a00046.html#ga2f690f608d984b845a7bee7a1efae45f", null ],
    [ "LPSPI_SlaveGetDefaultConfig", "a00046.html#ga5c8896fd0d8c76434b74698487f0f838", null ],
    [ "LPSPI_Deinit", "a00046.html#gaec14fb9cf6f33a9482cad1ac3367ceeb", null ],
    [ "LPSPI_Reset", "a00046.html#ga65c6f36d3b7c051b2f4c4b3c0f4e67a2", null ],
    [ "LPSPI_GetInstance", "a00046.html#gab7a9071086493c51a37c066ba22a8984", null ],
    [ "LPSPI_Enable", "a00046.html#ga7652e02b7e15dbbe611bf1b88aa6f658", null ],
    [ "LPSPI_GetStatusFlags", "a00046.html#ga65f275a2b7c4c85e65f66dc205ccaba2", null ],
    [ "LPSPI_GetTxFifoSize", "a00046.html#ga166d9cc3fe51574416ec1242671f8c70", null ],
    [ "LPSPI_GetRxFifoSize", "a00046.html#ga3e3302a39741e9f305b73e484a470cf2", null ],
    [ "LPSPI_GetTxFifoCount", "a00046.html#gaad302d78079501338706a15b2a25b627", null ],
    [ "LPSPI_GetRxFifoCount", "a00046.html#gaccf9caba656fbab8c2baaf9c064dfd18", null ],
    [ "LPSPI_ClearStatusFlags", "a00046.html#ga46f471b0b317dbcefb753ac1eca220cc", null ],
    [ "LPSPI_EnableInterrupts", "a00046.html#ga4808761455aaf83e91fd8379874946ed", null ],
    [ "LPSPI_DisableInterrupts", "a00046.html#ga40a046e84442cb6f187f37b676a026cc", null ],
    [ "LPSPI_EnableDMA", "a00046.html#ga962cc835d0c6133d3b5ca97103ffb3fb", null ],
    [ "LPSPI_DisableDMA", "a00046.html#gae40bb751eb579a3ba6f0df9d2b128074", null ],
    [ "LPSPI_GetTxRegisterAddress", "a00046.html#gad411d09a6ece1909f3fa0c6eb6972a27", null ],
    [ "LPSPI_GetRxRegisterAddress", "a00046.html#ga441f4a34fc1014e80df6dbdd3ccef49e", null ],
    [ "LPSPI_CheckTransferArgument", "a00046.html#gaefe8c48ef3e058a6b9d6d70671c56dda", null ],
    [ "LPSPI_SetMasterSlaveMode", "a00046.html#ga31dd4286061d9827893b20da53fbe711", null ],
    [ "LPSPI_SelectTransferPCS", "a00046.html#gade9addef6eac06f9246c7fcf40ef58ae", null ],
    [ "LPSPI_SetPCSContinous", "a00046.html#ga454231213b296c3b0f3223875321e308", null ],
    [ "LPSPI_IsMaster", "a00046.html#gaf0a22161d44cf08f3609ce0371190b43", null ],
    [ "LPSPI_FlushFifo", "a00046.html#ga1388a83ff99459ea04927a951fe4ba64", null ],
    [ "LPSPI_SetFifoWatermarks", "a00046.html#ga9036ebfaa63448544b3a15731bf64afb", null ],
    [ "LPSPI_SetAllPcsPolarity", "a00046.html#gac44e343cc612a8f9662dca0124e01375", null ],
    [ "LPSPI_SetFrameSize", "a00046.html#ga1c04805f36ab2d8f3dcf8204ad4e6fc8", null ],
    [ "LPSPI_MasterSetBaudRate", "a00046.html#ga6e952fee44ee7107dd22be1fcca41599", null ],
    [ "LPSPI_MasterSetDelayScaler", "a00046.html#ga9b69797908deb95b2b96e76bc7f8e4f9", null ],
    [ "LPSPI_MasterSetDelayTimes", "a00046.html#ga35f5b5317e3faffe8307221c24c2eeb3", null ],
    [ "LPSPI_WriteData", "a00046.html#gabee55ef2145352df8e2f86d9a9feb505", null ],
    [ "LPSPI_ReadData", "a00046.html#gaa7d2c9dd92d40376edb34d7ccd60936f", null ],
    [ "LPSPI_SetDummyData", "a00046.html#ga2fa502e9763b00eac5fee371fb5572f2", null ],
    [ "LPSPI_MasterTransferCreateHandle", "a00046.html#gae540270075fa16621181847b0ba8fcaa", null ],
    [ "LPSPI_MasterTransferBlocking", "a00046.html#ga7397508d8f35bd53d48d2bd400b8855a", null ],
    [ "LPSPI_MasterTransferNonBlocking", "a00046.html#ga962d0a70e661183c6925f87d97ab71d0", null ],
    [ "LPSPI_MasterTransferGetCount", "a00046.html#gaff7f9bcd612f6a354843e025b60fed95", null ],
    [ "LPSPI_MasterTransferAbort", "a00046.html#gaf7b2603f131815ba94f8f03e54ad2142", null ],
    [ "LPSPI_MasterTransferHandleIRQ", "a00046.html#gab04dab63be42f85b747d7987e3a29476", null ],
    [ "LPSPI_SlaveTransferCreateHandle", "a00046.html#ga795c68dcf004c34e3d43d52203243aa5", null ],
    [ "LPSPI_SlaveTransferNonBlocking", "a00046.html#ga41d2ccaef2f31a13ce2baef806a7aa2b", null ],
    [ "LPSPI_SlaveTransferGetCount", "a00046.html#ga16197357d88a58ecc4ad0365ac360714", null ],
    [ "LPSPI_SlaveTransferAbort", "a00046.html#gad7ada4345cd33086f7dff776123f9477", null ],
    [ "LPSPI_SlaveTransferHandleIRQ", "a00046.html#gab03f7964e02570aec3ca7c990200c7ed", null ],
    [ "g_lpspiDummyData", "a00046.html#ga95e4847cd333277614975d46280df9dd", null ]
];