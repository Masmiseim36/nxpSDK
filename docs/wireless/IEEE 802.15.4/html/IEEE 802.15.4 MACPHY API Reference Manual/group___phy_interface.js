var group___phy_interface =
[
    [ "pdDataReq_t", "group___phy_interface.html#structpd_data_req__t", [
      [ "startTime", "group___phy_interface.html#a9b6b432d3dd37fc7cbd75afacfda459d", null ],
      [ "txDuration", "group___phy_interface.html#afb15af6624cad3e1ca51b254935b1cc0", null ],
      [ "slottedTx", "group___phy_interface.html#ae3e271cb1c5aa6b5e66212c95a5f6456", null ],
      [ "CCABeforeTx", "group___phy_interface.html#a4726a5414ad2f618d3549dbd79315c30", null ],
      [ "ackRequired", "group___phy_interface.html#a805852767d1292b115a5a2692effb1f6", null ],
      [ "psduLength", "group___phy_interface.html#a3e5fa7a308f858c6379f16c249da2a60", null ],
      [ "pPsdu", "group___phy_interface.html#add78399fdb80c94526980f78b6d9cf82", null ]
    ] ],
    [ "pdDataCnf_t", "group___phy_interface.html#structpd_data_cnf__t", [
      [ "status", "group___phy_interface.html#a7207eb17b653a3705f4f0f76392240bc", null ]
    ] ],
    [ "pdDataInd_t", "group___phy_interface.html#structpd_data_ind__t", [
      [ "timeStamp", "group___phy_interface.html#a3967eb7858a13ba7b76ace9627d6b9d4", null ],
      [ "ppduLinkQuality", "group___phy_interface.html#aefd2dcc4360fa5ab61ec21ca279509bc", null ],
      [ "psduLength", "group___phy_interface.html#ae4b0ada7b480e782e72663e0b75c5c1b", null ],
      [ "pPsdu", "group___phy_interface.html#a1d7d52b7f2e80359f60e79dcab3e534c", null ]
    ] ],
    [ "pdIndQueueInsertReq_t", "group___phy_interface.html#structpd_ind_queue_insert_req__t", [
      [ "index", "group___phy_interface.html#ac23b7bff6233119ec1a599430058ad73", null ],
      [ "checksum", "group___phy_interface.html#aa2ae5f6481c8990b5914fc39837c0646", null ]
    ] ],
    [ "pdIndQueueInsertCnf_t", "group___phy_interface.html#structpd_ind_queue_insert_cnf__t", [
      [ "status", "group___phy_interface.html#a4ad118eb0e3c5a567c5aff4418f6bc05", null ]
    ] ],
    [ "pdIndQueueRemoveReq_t", "group___phy_interface.html#structpd_ind_queue_remove_req__t", [
      [ "index", "group___phy_interface.html#a2771121ef96554927b7b648472fa1546", null ]
    ] ],
    [ "plmeEdReq_t", "group___phy_interface.html#structplme_ed_req__t", [
      [ "startTime", "group___phy_interface.html#ab2f1bd90bf01a2c07f3d0acf035c63f8", null ]
    ] ],
    [ "plmeCcaReq_t", "group___phy_interface.html#structplme_cca_req__t", [
      [ "ccaType", "group___phy_interface.html#a0181d49b95e7ad9f1920c9cb7df8d94a", null ],
      [ "contCcaMode", "group___phy_interface.html#a725d6c8fa700ddf1af0b8039eedb0d23", null ]
    ] ],
    [ "plmeCcaCnf_t", "group___phy_interface.html#structplme_cca_cnf__t", [
      [ "status", "group___phy_interface.html#a285e0301e2c2587bc9d4bda15814ff08", null ]
    ] ],
    [ "plmeEdCnf_t", "group___phy_interface.html#structplme_ed_cnf__t", [
      [ "status", "group___phy_interface.html#a7535dcb9fd9b563f8975173d0fb1895f", null ],
      [ "energyLevel", "group___phy_interface.html#acfe2ea2f368dc0f076c23494ef88fc7b", null ],
      [ "energyLeveldB", "group___phy_interface.html#a48d5b1f19fe28213e8ac5bbabcc7de8e", null ]
    ] ],
    [ "plmeSetTRxStateReq_t", "group___phy_interface.html#structplme_set_t_rx_state_req__t", [
      [ "state", "group___phy_interface.html#a14416310a582795ac7e8e5df787a166e", null ],
      [ "slottedMode", "group___phy_interface.html#ad0349cebeaa292b7d2962772a0e2b270", null ],
      [ "startTime", "group___phy_interface.html#ad41f8193be2624a5cd3c936808d72248", null ],
      [ "rxDuration", "group___phy_interface.html#a732c748b77c18e57a811abd0d9889af4", null ]
    ] ],
    [ "phyTimeEvent_t", "group___phy_interface.html#structphy_time_event__t", [
      [ "timestamp", "group___phy_interface.html#a9fdbbae7e4b21390b69860b20e3e243c", null ],
      [ "callback", "group___phy_interface.html#a3effa262994d3610180428d32a179186", null ],
      [ "parameter", "group___phy_interface.html#a886707c586705cc754d30d59e89fa4fb", null ]
    ] ],
    [ "plmeSetTRxStateCnf_t", "group___phy_interface.html#structplme_set_t_rx_state_cnf__t", [
      [ "status", "group___phy_interface.html#a5f790a3ce2625c8df004c264303878a2", null ]
    ] ],
    [ "plmeSetReq_t", "group___phy_interface.html#structplme_set_req__t", [
      [ "PibAttribute", "group___phy_interface.html#a2dc60d3f3b83415824e207ff74c757b5", null ],
      [ "PibAttributeValue", "group___phy_interface.html#a85c285a8a19394bb6a9d66eb47c8a199", null ]
    ] ],
    [ "plmeSetCnf_t", "group___phy_interface.html#structplme_set_cnf__t", [
      [ "status", "group___phy_interface.html#a430aaa49b5bd6003d155b7f68090a291", null ],
      [ "PibAttribute", "group___phy_interface.html#a1b889a232063fdd1903526fe3050e6ef", null ]
    ] ],
    [ "plmeGetReq_t", "group___phy_interface.html#structplme_get_req__t", [
      [ "PibAttribute", "group___phy_interface.html#a6920637c93558351da3536f1145c304a", null ],
      [ "pPibAttributeValue", "group___phy_interface.html#a155e50c2af1432002be7b17b4b0dc835", null ]
    ] ],
    [ "plmeGetCnf_t", "group___phy_interface.html#structplme_get_cnf__t", [
      [ "status", "group___phy_interface.html#a3c803dca07c9c8669439dcfa77552dfb", null ],
      [ "PibAttribute", "group___phy_interface.html#a01676c03fe335e1423770fcacbe9f279", null ],
      [ "PibAttributeValue", "group___phy_interface.html#acd2162029097fe792292c5cb9f85b2a3", null ]
    ] ],
    [ "macToPlmeMessage_t", "group___phy_interface.html#structmac_to_plme_message__t", [
      [ "msgType", "group___phy_interface.html#ac67634673d9eec2620e5a26be591f103", null ],
      [ "macInstance", "group___phy_interface.html#a47089373dfcd87d385f8291b80cda72a", null ],
      [ "msgData", "group___phy_interface.html#a6346b287ef42dc4ac121bb1b29c58223", null ]
    ] ],
    [ "macToPlmeMessage_t.msgData", "group___phy_interface.html#unionmac_to_plme_message__t_8msg_data", [
      [ "edReq", "group___phy_interface.html#a91375d0c92f0ec09717f55a2e2fba00f", null ],
      [ "ccaReq", "group___phy_interface.html#ab75a9dbc894eb548203b55a5f2714b3e", null ],
      [ "setTRxStateReq", "group___phy_interface.html#a3e48ba7a0730c04dce701de0d11fb129", null ],
      [ "setReq", "group___phy_interface.html#abe4301eb042357c5c62c269f94349cd7", null ],
      [ "getReq", "group___phy_interface.html#aa692af16daea3182d92166b442b4cd8c", null ]
    ] ],
    [ "macToPdDataMessage_t", "group___phy_interface.html#structmac_to_pd_data_message__t", [
      [ "msgType", "group___phy_interface.html#a6f2265d1dc0e8b17b56c159e6e692344", null ],
      [ "macInstance", "group___phy_interface.html#aa2f3ca8d325a3b5f493a0d5cdf4ef253", null ],
      [ "msgData", "group___phy_interface.html#aff77fc12c78a0526f6bb24aef34fec1a", null ]
    ] ],
    [ "macToPdDataMessage_t.msgData", "group___phy_interface.html#unionmac_to_pd_data_message__t_8msg_data", [
      [ "dataReq", "group___phy_interface.html#a7b15c5f91b1a0fb6257a75e7258337b2", null ],
      [ "indQueueInsertReq", "group___phy_interface.html#a91c189fb4788ace793117f5c3cc2ba1e", null ],
      [ "indQueueRemoveReq", "group___phy_interface.html#aa2992e4fb0570c19aaf1a1109f299da9", null ]
    ] ],
    [ "plmeToMacMessage_t", "group___phy_interface.html#structplme_to_mac_message__t", [
      [ "msgType", "group___phy_interface.html#add07e408ce8441e2b2ce83eb22f5556c", null ],
      [ "macInstance", "group___phy_interface.html#a35a8e6336b5b5b8e57e515d3f08fdcb8", null ],
      [ "msgData", "group___phy_interface.html#a38ed7c279cdbe6f09c1f7ec287150b08", null ]
    ] ],
    [ "plmeToMacMessage_t.msgData", "group___phy_interface.html#unionplme_to_mac_message__t_8msg_data", [
      [ "ccaCnf", "group___phy_interface.html#a36a9c0282fac2e67fd27efce093f5752", null ],
      [ "edCnf", "group___phy_interface.html#a0a962c065a0338810dc1f28a8fc829c0", null ]
    ] ],
    [ "pdDataToMacMessage_t", "group___phy_interface.html#structpd_data_to_mac_message__t", [
      [ "msgType", "group___phy_interface.html#aba15a16536697c5f53d3f4eb581143e4", null ],
      [ "macInstance", "group___phy_interface.html#addf22315c05d6c15d56dca98e62b16c0", null ],
      [ "msgData", "group___phy_interface.html#a737151525a151a1295a54c90ed51a6ef", null ]
    ] ],
    [ "pdDataToMacMessage_t.msgData", "group___phy_interface.html#unionpd_data_to_mac_message__t_8msg_data", [
      [ "dataCnf", "group___phy_interface.html#a9e1d73b7cd44c7df38363104d35b6c45", null ],
      [ "dataInd", "group___phy_interface.html#a79545cbec251841820b3e2147d72df72", null ]
    ] ],
    [ "phyMessageHeader_t", "group___phy_interface.html#structphy_message_header__t", [
      [ "msgType", "group___phy_interface.html#a90fcc312d505edbde3491c18405b5947", null ],
      [ "macInstance", "group___phy_interface.html#a1bc178c2a3f5c1c05eb18d8440c3733a", null ]
    ] ],
    [ "phyRxParams_t", "group___phy_interface.html#structphy_rx_params__t", [
      [ "timeStamp", "group___phy_interface.html#a11acb63894965eed0916fe1e2fd5a6d0", null ],
      [ "psduLength", "group___phy_interface.html#a6b4a746a323c11b50ee262b0cff7930e", null ],
      [ "linkQuality", "group___phy_interface.html#ada6158f173aa0cef1939855461931f87", null ],
      [ "duration", "group___phy_interface.html#aff8502dee28176f2e1f311a6bb156424", null ],
      [ "pRxData", "group___phy_interface.html#a7025cafd6fb065a889c4fc51f864b618", null ],
      [ "pDataPending", "group___phy_interface.html#af6d11beba82545587423544bd22f34f6", null ],
      [ "phyRxMode", "group___phy_interface.html#acd1f10f55725a9e4c7b38766acd66a53", null ]
    ] ],
    [ "phyChannelParams_t", "group___phy_interface.html#structphy_channel_params__t", null ],
    [ "phyChannelParams_t.__unnamed__", "group___phy_interface.html#unionphy_channel_params__t_8____unnamed____", [
      [ "channelStatus", "group___phy_interface.html#a9b45399e6f7481a81d5a328adc579079", null ],
      [ "energyLeveldB", "group___phy_interface.html#a233f051851ddb1c163ce6ded1c905736", null ]
    ] ],
    [ "phyTxParams_t", "group___phy_interface.html#structphy_tx_params__t", [
      [ "numOfCca", "group___phy_interface.html#a105d11d73cd421a1f2eb05cdcfa163ce", null ],
      [ "ackRequired", "group___phy_interface.html#aa2444bf8590d320ac584d059b56c6767", null ]
    ] ],
    [ "gPhyInstancesCnt_c", "group___phy_interface.html#ga64a3349d2a144c0ac3cfec18306db903", null ],
    [ "gPhyTaskStackSize_c", "group___phy_interface.html#ga839b08e4c8bf048b2bd801dc9a3cfe88", null ],
    [ "gPhyTaskPriority_c", "group___phy_interface.html#gafaa7c98b6328adc011c98a4643b86419", null ],
    [ "gAfcEnabled_d", "group___phy_interface.html#ga5df040e1a3f162643d346fd48c8b92e4", null ],
    [ "gMaxPhyTimers_c", "group___phy_interface.html#ga0f93fde3cc61eb1a4578106e92c3416c", null ],
    [ "gPhySeqStartAsap_c", "group___phy_interface.html#ga12e984d101fdb6e344662244957b36e6", null ],
    [ "gPhyPoolId_d", "group___phy_interface.html#ga447de4678e678e773c6323d3c99769ba", null ],
    [ "PD_MAC_SapHandler_t", "group___phy_interface.html#gab1ca640cc835a664e9239a1b43e9ea35", null ],
    [ "PLME_MAC_SapHandler_t", "group___phy_interface.html#ga2bffefba105f4af564b49f260dc93f8c", null ],
    [ "phyPwrMode_t", "group___phy_interface.html#gaf56d54a3400ecab3bb4b46d94abce02d", null ],
    [ "Phy_Init", "group___phy_interface.html#gaa65178ec818903afdb21e7d23dba428c", null ],
    [ "BindToPHY", "group___phy_interface.html#ga6ed6b33c8be28851bafedb435f00a7cd", null ],
    [ "Phy_RegisterSapHandlers", "group___phy_interface.html#gaf96a0316e8bb915f1c40375d3ba0ecd9", null ],
    [ "MAC_PD_SapHandler", "group___phy_interface.html#gaa25baac90f72954527cae3d8dd5087c2", null ],
    [ "MAC_PLME_SapHandler", "group___phy_interface.html#ga88e44861ecef609a110fe100650f7694", null ],
    [ "PhyTime_Maintenance", "group___phy_interface.html#ga775ae2100448ea427e96c296354cbd4d", null ],
    [ "PhyTime_RunCallback", "group___phy_interface.html#ga091fe9b1481bf4a3b857e5e97f3ae04e", null ],
    [ "PhyTime_ISR", "group___phy_interface.html#ga97ae9401c1a7bea14e544e3d070e7532", null ],
    [ "PhyTime_TimerInit", "group___phy_interface.html#gab1b45215106681436417ea315ba16484", null ],
    [ "PhyTime_GetTimestamp", "group___phy_interface.html#ga7b7dc1489de242c7f19073e2567d024d", null ],
    [ "PhyTime_ScheduleEvent", "group___phy_interface.html#ga959272e3eff35aa9355060fed1788fbe", null ],
    [ "PhyTime_CancelEvent", "group___phy_interface.html#gac7f99c87e53eeb3c1fb68a477791d1eb", null ],
    [ "PhyTime_CancelEventsWithParam", "group___phy_interface.html#ga227d515ad486b9fa403c22f6908a4cf8", null ],
    [ "PhyPlmeSetPwrState", "group___phy_interface.html#ga6f3e88fc7fb192c9aa5c3d0b70c5d0ce", null ]
];