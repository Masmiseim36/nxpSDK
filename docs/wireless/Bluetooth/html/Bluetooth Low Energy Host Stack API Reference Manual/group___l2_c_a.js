var group___l2_c_a =
[
    [ "l2caLeCbConnectionRequest_t", "group___l2_c_a.html#structl2ca_le_cb_connection_request__t", [
      [ "deviceId", "group___l2_c_a.html#a4bf56a1f7a31fa880919406a7aa8df60", null ],
      [ "lePsm", "group___l2_c_a.html#a592b462fdf0ec1c469dc77cea58cfec1", null ],
      [ "peerMtu", "group___l2_c_a.html#ad38805ff10b3d604eb9c52dc77987af4", null ],
      [ "peerMps", "group___l2_c_a.html#a95dfee94e377fc75755b817a0b4636be", null ],
      [ "initialCredits", "group___l2_c_a.html#a355db84f0ca9f44fa0f3aabfc196ea6e", null ]
    ] ],
    [ "l2caLeCbConnectionComplete_t", "group___l2_c_a.html#structl2ca_le_cb_connection_complete__t", [
      [ "deviceId", "group___l2_c_a.html#a17aff7a53845f66719a10522b596d71d", null ],
      [ "cId", "group___l2_c_a.html#a5f1fbad7cc5cbe644d0835664ac9869d", null ],
      [ "peerMtu", "group___l2_c_a.html#aa41868de7089afb42702170540353594", null ],
      [ "peerMps", "group___l2_c_a.html#ad2533942a54939ea34ae6760880ea789", null ],
      [ "initialCredits", "group___l2_c_a.html#a19da8eeff3839edf4d46b761ca5729cf", null ],
      [ "result", "group___l2_c_a.html#a192c22408374792ed2a403318af060f4", null ]
    ] ],
    [ "l2caLeCbDisconnection_t", "group___l2_c_a.html#structl2ca_le_cb_disconnection__t", [
      [ "deviceId", "group___l2_c_a.html#ac7a852be9da6febed14b49ae102bd3e2", null ],
      [ "cId", "group___l2_c_a.html#a6022293c3a326ed776c475a990cc48c9", null ]
    ] ],
    [ "l2caLeCbNoPeerCredits_t", "group___l2_c_a.html#structl2ca_le_cb_no_peer_credits__t", [
      [ "deviceId", "group___l2_c_a.html#a90fadce31c7d359879f78136d7cb4937", null ],
      [ "cId", "group___l2_c_a.html#a726bd0665791f8ac3cf5a11fd4d36304", null ]
    ] ],
    [ "l2caLeCbLocalCreditsNotification_t", "group___l2_c_a.html#structl2ca_le_cb_local_credits_notification__t", [
      [ "deviceId", "group___l2_c_a.html#a9a0cf77f9ce22efe1be8fa130062f73d", null ],
      [ "cId", "group___l2_c_a.html#a759477336270c906f321ee431c70ad9d", null ],
      [ "localCredits", "group___l2_c_a.html#a9d860b1c2b1cf40c29213124f2f150c1", null ]
    ] ],
    [ "l2caLeCbError_t", "group___l2_c_a.html#structl2ca_le_cb_error__t", [
      [ "deviceId", "group___l2_c_a.html#a409f250ced34f1dcd3ef0bc6e85fdea6", null ],
      [ "result", "group___l2_c_a.html#a3e00dddd8cf9718e6a5fd1f2620fbda5", null ],
      [ "errorSource", "group___l2_c_a.html#ad9e5863c89043a68042e92621cd37d25", null ]
    ] ],
    [ "l2capControlMessage_t", "group___l2_c_a.html#structl2cap_control_message__t", [
      [ "messageType", "group___l2_c_a.html#a43c0be5a1ab6f363581b408fd067305a", null ],
      [ "messageData", "group___l2_c_a.html#a07b33451c5075138a54504c7aea4fbbf", null ]
    ] ],
    [ "l2capControlMessage_t.messageData", "group___l2_c_a.html#unionl2cap_control_message__t_8message_data", [
      [ "connectionRequest", "group___l2_c_a.html#ae3e577d1b9f178882420c1fb513600ef", null ],
      [ "connectionComplete", "group___l2_c_a.html#abce98f5cc332094957a14e615ece2fcc", null ],
      [ "disconnection", "group___l2_c_a.html#aedb9d033d7e1866da7bd79abdeca6f66", null ],
      [ "noPeerCredits", "group___l2_c_a.html#a2ca45dec08f3cc23be29ec91a4f50b44", null ],
      [ "localCreditsNotification", "group___l2_c_a.html#a0b5ff9798eabc318f210c20f8e8cd61e", null ],
      [ "error", "group___l2_c_a.html#acb5e100e5a9a3e7f6d1fd97512215282", null ]
    ] ],
    [ "L2ca_RegisterLeCbCallbacks", "group___l2_c_a.html#ga0655d819f0939145728acdfebd4309b1", null ],
    [ "L2ca_RegisterLePsm", "group___l2_c_a.html#ga5c2cc4596c6c7dc6a0a8979ce1daf3fa", null ],
    [ "L2ca_DeregisterLePsm", "group___l2_c_a.html#ga1d09857a7b7858e3c2ecd754a00e2d59", null ],
    [ "L2ca_ConnectLePsm", "group___l2_c_a.html#ga7559190b3600e0f34702a31f901c9939", null ],
    [ "L2ca_DisconnectLeCbChannel", "group___l2_c_a.html#gab8c50c1c94ef2cbf095c6f34b5b2bef3", null ],
    [ "L2ca_CancelConnection", "group___l2_c_a.html#gaeb7efea8a5b91aa2e60c95b454983586", null ],
    [ "L2ca_SendLeCbData", "group___l2_c_a.html#ga57acad48637f31c8fc104840e7abdd7f", null ],
    [ "L2ca_SendLeCredit", "group___l2_c_a.html#ga66c75ebaa71f52f3df9886ebaa2f897c", null ]
];