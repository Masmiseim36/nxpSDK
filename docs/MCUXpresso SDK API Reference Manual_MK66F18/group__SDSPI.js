var group__SDSPI =
[
    [ "sdspi_command_t", "group__SDSPI.html#structsdspi__command__t", [
      [ "index", "group__SDSPI.html#aab8904be71aac382a8d74d2f113a154b", null ],
      [ "argument", "group__SDSPI.html#aefc36d8b10a362964574b18c28dd32fb", null ],
      [ "responseType", "group__SDSPI.html#ab2ec5bb62c5e90766094d7bb23e61a71", null ],
      [ "response", "group__SDSPI.html#a0d0ba102d739a24a5cc659bf4c2692ee", null ]
    ] ],
    [ "sdspi_host_t", "group__SDSPI.html#structsdspi__host__t", [
      [ "busBaudRate", "group__SDSPI.html#a992320fa4e3a64f4f4600bb7e996c193", null ],
      [ "setFrequency", "group__SDSPI.html#ac69133eb56496fa30f5a1de66fd0d70f", null ],
      [ "exchange", "group__SDSPI.html#a28c4b16063bff43d3c1bbf5f33920992", null ],
      [ "getCurrentMilliseconds", "group__SDSPI.html#a501741440de7982b689a68ac83aa11d9", null ]
    ] ],
    [ "sdspi_card_t", "group__SDSPI.html#structsdspi__card__t", [
      [ "host", "group__SDSPI.html#abbd787527a60453185564c7ec7c60497", null ],
      [ "relativeAddress", "group__SDSPI.html#a60fac9db0662250d42f2ffedd5570bf0", null ],
      [ "flags", "group__SDSPI.html#a50d10b36a68b5de292077bf91875698d", null ],
      [ "rawCid", "group__SDSPI.html#a89303456d23e3219e55c526ec6cfed9c", null ],
      [ "rawCsd", "group__SDSPI.html#a61ec55e03b601abc63b9fb1108bc59ac", null ],
      [ "rawScr", "group__SDSPI.html#a515d12e9ba824062699fc52e95a7107d", null ],
      [ "ocr", "group__SDSPI.html#a944b85aa51494ed8de4761f7ec0528a4", null ],
      [ "cid", "group__SDSPI.html#a24dc064f3a85f584f5b8409b94efa1bf", null ],
      [ "csd", "group__SDSPI.html#ac9c6b2a3d22e7abc7780ec8a6b5f9d98", null ],
      [ "scr", "group__SDSPI.html#a246537655ae68ffd8472df9bf6255b27", null ],
      [ "blockCount", "group__SDSPI.html#a6ee807bcedf1f47a3563b738e7f2571b", null ],
      [ "blockSize", "group__SDSPI.html#a223bef1e8979e1f6780365cbfa2772a6", null ]
    ] ],
    [ "_sdspi_status", "group__SDSPI.html#ga6167a5292ff7a5574dc997b27cc55872", [
      [ "kStatus_SDSPI_SetFrequencyFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a63f54fd90d8568993990b0dd1215ebea", null ],
      [ "kStatus_SDSPI_ExchangeFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a3c922b711a1e2c63bf95b421bae19643", null ],
      [ "kStatus_SDSPI_WaitReadyFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a5d072cfe065434dd96e84b3dbfb6b763", null ],
      [ "kStatus_SDSPI_ResponseError", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a94eeae627e7f56d66a6ca9f29c22a9b4", null ],
      [ "kStatus_SDSPI_WriteProtected", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a00d3cccf7db647c2133a6fda4c3b53ae", null ],
      [ "kStatus_SDSPI_GoIdleFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a49f8f5fcaa3a3135640fa50e411a1ea8", null ],
      [ "kStatus_SDSPI_SendCommandFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a5492a48fffbd51bb8c1a40d66bbcc6c3", null ],
      [ "kStatus_SDSPI_ReadFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872ad46c8f2abdfee3d6d5dbf1e17f283832", null ],
      [ "kStatus_SDSPI_WriteFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872af52fc552192610b555b92b4a180ab0fe", null ],
      [ "kStatus_SDSPI_SendInterfaceConditionFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872ab08abfbc06ee3ad45fa4a1dd6ab68380", null ],
      [ "kStatus_SDSPI_SendOperationConditionFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a4196863dc558bb96b17721929d9f47ce", null ],
      [ "kStatus_SDSPI_ReadOcrFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a34ccd32578318f3115218282afb95d1e", null ],
      [ "kStatus_SDSPI_SetBlockSizeFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a2d5b300f06eebb9d0afd9fe30ba63957", null ],
      [ "kStatus_SDSPI_SendCsdFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a76c63c33814ba7a33b25205f70c4530f", null ],
      [ "kStatus_SDSPI_SendCidFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872ae8362579d073b197fbb5b56983d48b5a", null ],
      [ "kStatus_SDSPI_StopTransmissionFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872acb8c97a49b21cba91374e3cf9f1e41a6", null ],
      [ "kStatus_SDSPI_SendApplicationCommandFailed", "group__SDSPI.html#gga6167a5292ff7a5574dc997b27cc55872a5df3eb42a9563d3b2281f17ff6ee4ee5", null ]
    ] ],
    [ "_sdspi_card_flag", "group__SDSPI.html#ga4c4f355f5d1f428fcf7fb51a1e550556", [
      [ "kSDSPI_SupportHighCapacityFlag", "group__SDSPI.html#gga4c4f355f5d1f428fcf7fb51a1e550556a9ad962c03f87ba1ad2a87d9f311a489c", null ],
      [ "kSDSPI_SupportSdhcFlag", "group__SDSPI.html#gga4c4f355f5d1f428fcf7fb51a1e550556ac56722187769d3e1cb88aafdff88c3d1", null ],
      [ "kSDSPI_SupportSdxcFlag", "group__SDSPI.html#gga4c4f355f5d1f428fcf7fb51a1e550556aac2f60b67a34203de32aedd517526bac", null ],
      [ "kSDSPI_SupportSdscFlag", "group__SDSPI.html#gga4c4f355f5d1f428fcf7fb51a1e550556a3e947033f2e83e0e69cdd2e3d4bb9ea0", null ]
    ] ],
    [ "sdspi_response_type_t", "group__SDSPI.html#ga65399c834121bb68b275856bd02b1163", [
      [ "kSDSPI_ResponseTypeR1", "group__SDSPI.html#gga65399c834121bb68b275856bd02b1163af2d0de929527131f2c78bf601e778353", null ],
      [ "kSDSPI_ResponseTypeR1b", "group__SDSPI.html#gga65399c834121bb68b275856bd02b1163a3df9332b3af59c5fbeeea71489e0bbe6", null ],
      [ "kSDSPI_ResponseTypeR2", "group__SDSPI.html#gga65399c834121bb68b275856bd02b1163ab614558dd9e6b2592b11b4ec55168bc3", null ],
      [ "kSDSPI_ResponseTypeR3", "group__SDSPI.html#gga65399c834121bb68b275856bd02b1163a99f741eccabfd6709887cffb51001f6e", null ],
      [ "kSDSPI_ResponseTypeR7", "group__SDSPI.html#gga65399c834121bb68b275856bd02b1163ac88551d71e089ab2cfe2e22a1b29ce1d", null ]
    ] ],
    [ "SDSPI_Init", "group__SDSPI.html#ga1189771b440db3505261286c9b1ee7eb", null ],
    [ "SDSPI_Deinit", "group__SDSPI.html#ga320e97ada05d089fe26716208490cc02", null ],
    [ "SDSPI_CheckReadOnly", "group__SDSPI.html#gaf94180d46e7263a8e7f99b47e8e61a79", null ],
    [ "SDSPI_ReadBlocks", "group__SDSPI.html#gaec09ae357857fef13f7c70c34acb5c4d", null ],
    [ "SDSPI_WriteBlocks", "group__SDSPI.html#ga6c007d811f72a0854cdb74d9afb199b0", null ]
];