
/**
 *  \file BT_common_pl.c
 *
 *  Common routines and start-up initialization & shutdown handlers
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* It means that edgefast is using ethermind if CONFIG_BT_BREDR is defined */
#if (defined (CONFIG_BT_BREDR) && (CONFIG_BT_BREDR))
#ifdef BT_BCSP
#undef BT_BCSP
#endif

#ifdef BNEP
#undef BNEP
#endif

#if !(defined (CONFIG_BT_A2DP) && (CONFIG_BT_A2DP))
#ifdef AVDTP
#undef AVDTP
#endif
#endif

#if !(defined (CONFIG_BT_AVRCP) && (CONFIG_BT_AVRCP))
#ifdef AVCTP
#undef AVCTP
#endif
#endif

#ifdef MCAP
#undef MCAP
#endif

#if !(defined (CONFIG_BT_AVRCP) && (CONFIG_BT_AVRCP))
#ifdef OBEX
#undef OBEX
#endif
#endif

#ifdef HCRP
#undef HCRP
#endif
#endif

/* It means that edgefast is using ethermind if CONFIG_BT_BLE_DISABLE is defined */
#if (defined (CONFIG_BT_BLE_DISABLE) && (CONFIG_BT_BLE_DISABLE))
#ifdef ATT
#undef ATT
#endif
#endif

#ifdef CLASSIC_SEC_MANAGER
#include "sm.h"
#endif /* CLASSIC_SEC_MANAGER */

#ifdef BT_UART
    #include "hci_uart.h"
#endif /* BT_UART */

#ifdef BT_USB
    #include "hci_usb.h"
#endif /* BT_USB */

#ifdef BT_BCSP
    #include "bcsp.h"
#endif /* BT_BCSP */

#ifdef BT_SOCKET
    #include "hci_socket.h"
#endif /* BT_SOCKET */

#ifdef BT_PLATFORM
    #include "hci_platform.h"
#endif /* BT_PLATFORM */

#ifdef SDP
    #include "sdp.h"
    #include "database.h"
#endif /* SDP */

#ifdef RFCOMM
    #include "rfcomm.h"
#endif /* RFCOMM */

#ifdef BNEP
    #include "bnep.h"
#endif /* BNEP */

#ifdef AVDTP
    #include "avdtp.h"
#endif /* AVDTP */

#ifdef AVCTP
    #include "avctp.h"
#endif /* AVCTP */

#ifdef MCAP
    #include "mcap.h"
#endif /* MCAP */

#ifdef OBEX
    #include "obex.h"
#endif /* OBEX */

#ifdef HCRP
    #include "hcrp.h"
#endif /* HCRP */

#ifdef SMP
    #include "smp.h"
#endif /* SMP */

#ifdef ATT
    #include "att.h"
#endif /* ATT */

/* ------------------------------------------- External Global Variables */

/* ------------------------------------------- External Global Variables */

/* ------------------------------------------- Static Global Variables */
#ifdef BT_COMMON_PL_SUPPORT_UL_CB
DECL_STATIC BT_COMMON_UL_INIT_CB_PL   ul_bt_on_init_cb;
DECL_STATIC BT_COMMON_UL_DEINIT_CB_PL ul_bt_off_deinit_cb;
#endif /* BT_COMMON_PL_SUPPORT_UL_CB */

/* ------------------------------------------- Functions */
/* EtherMind-Init: Platform Lower Handler */
void ethermind_init_lower_pl (void)
{
#ifdef BT_UART
    /* UART Initialization */
    hci_uart_init();
#endif /* BT_UART */

#ifdef BT_USB
    /* USB Initialization */
    hci_usb_init();
#endif /* BT_USB */

#ifdef BT_BCSP
    /* BCSP Initialization */
    bcsp_init();
#endif /* BT_BCSP */

#ifdef BT_SOCKET
    /* Socket Initialization */
    hci_socket_init();
#endif /* BT_SOCKET */

#ifdef BT_PLATFORM
    /* PLATFORM Initialization */
    hci_platform_init();
#endif /* BT_PLATFORM */
}


/* EtherMind-Init: Platform Upper Handler */
void ethermind_init_upper_pl (void)
{
#ifdef CLASSIC_SEC_MANAGER
    /* Security Manager Initialization */
    em_sm_init();
#endif /* CLASSIC_SEC_MANAGER */

#ifdef SDP
    /* SDP Initialization */
    em_sdp_init();

#ifdef SDP_SERVER
    /* SDDB SDP Interface Layer Initialization */
    em_db_sdp_init();
#endif /* SDP_SERVER */

#endif /* SDP */

#ifdef RFCOMM
    /* RFCOMM Initialization */
    em_rfcomm_init();
#endif /* RFCOMM */

#ifdef BNEP
    /* BNEP Initialization */
    em_bnep_init();
#endif /* BNEP */

#ifdef AVDTP
    /* AVDTP Initialization */
    em_avdtp_init();
#endif /* AVDTP */

#ifdef AVCTP
    /* AVCTP Initialization */
    em_avctp_init();
#endif /* AVCTP */

#ifdef MCAP
    /* MCAP Initialization */
    em_mcap_init();
#endif /* MCAP */

#ifdef OBEX
    /* OBEX Initialization */
    em_obex_init();
#endif /* OBEX */

#ifdef HCRP
    /* HCRP Initialization */
    em_hcrp_init();
#endif /* HCRP */

#ifdef SMP
    em_smp_init();
#endif /* SMP */

#ifdef ATT
    em_att_init();
#endif /* ATT */

#ifdef BT_COMMON_PL_SUPPORT_UL_CB
    ul_bt_on_init_cb    = NULL;
    ul_bt_off_deinit_cb = NULL;
#endif /* BT_COMMON_PL_SUPPORT_UL_CB */
}

#ifdef BT_HAVE_SHUTDOWN
/* EtherMind-De-Init: Platform Lower Handler */
void ethermind_shutdown_lower_pl(void)
{
#ifdef BT_UART
    /* UART De-Initialization */
    hci_uart_shutdown();
#endif /* BT_UART */

#ifdef BT_USB
    /* USB De-Initialization */
    hci_usb_shutdown();
#endif /* BT_USB */

#ifdef BT_BCSP
    /* BCSP De-Initialization */
    bcsp_shutdown();
#endif /* BT_BCSP */

#ifdef BT_SOCKET
    /* Socket De-Initialization */
    hci_socket_shutdown();
#endif /* BT_SOCKET */
}

/* EtherMind-De-Init: Platform Upper Handler */
void ethermind_shutdown_upper_pl(void)
{
    /* Security Manager De-Initialization */
    em_sm_shutdown();

#ifdef SDP
    /* SDP De-Initialization */
    em_sdp_shutdown();

#ifdef SDP_SERVER
    /* SDDB SDP Interface Layer De-Initialization */
    em_db_sdp_shutdown();
#endif /* SDP_SERVER */

#endif /* SDP */

#ifdef RFCOMM
    /* RFCOMM De-Initialization */
    em_rfcomm_shutdown();
#endif /* RFCOMM */

#ifdef BNEP
    /* BNEP Initialization */
    em_bnep_shutdown();
#endif /* BNEP */

#ifdef AVDTP
    /* AVDTP De-Initialization */
    em_avdtp_shutdown();
#endif /* AVDTP */

#ifdef AVCTP
    /* AVCTP De-Initialization */
    em_avctp_shutdown();
#endif /* AVCTP */

#ifdef MCAP
    /* MCAP Initialization */
    em_mcap_shutdown();
#endif /* MCAP */

#ifdef OBEX
    /* OBEX De-Initialization */
    em_obex_shutdown();
#endif /* OBEX */

#ifdef HCRP
    /* HCRP Initialization */
    em_hcrp_shutdown();
#endif /* HCRP */

#ifdef SMP
    em_smp_shutdown();
#endif /* SMP */

#ifdef ATT
    em_att_shutdown();
#endif /* ATT */
}
#endif /* BT_HAVE_SHUTDOWN */

/* Bluetooth-ON: Platform Lower Handler */
void bluetooth_on_lower_pl (void)
{
#ifdef BT_UART
    /* UART BT Init */
    hci_uart_bt_init();
#endif /* BT_UART */

#ifdef BT_USB
    /* USB BT Init */
    hci_usb_bt_init();
#endif /* BT_USB */

#ifdef BT_BCSP
    /* BCSP BT Init */
    bcsp_bt_init();
#endif /* BT_BCSP */

#ifdef BT_SOCKET
    /* Socket BT Init */
    hci_socket_bt_init();
#endif /* BT_SOCKET */

#ifdef BT_PLATFORM
    /* PLATFORM BT Init */
    hci_platform_bt_init();
#endif /* BT_PLATFORM */
}


/* Bluetooth-ON: Platform Upper Handler */
void bluetooth_on_upper_pl (void)
{
#ifdef CLASSIC_SEC_MANAGER
    /* Security Manager BT Init */
    sm_bt_init();
#endif /* CLASSIC_SEC_MANAGER */

#ifdef SDP
    /* SDP BT Init */
    sdp_bt_init();
#endif /* SDP */

#ifdef RFCOMM
    /* RFCOMM BT Init */
    rfcomm_bt_init();
#endif /* RFCOMM */

#ifdef BNEP
    /* BNEP BT Init */
    bnep_bt_init();
#endif /* BNEP */

#ifdef AVDTP
    /* AVDTP BT Init */
    avdtp_bt_init();
#endif /* AVDTP */

#ifdef AVCTP
    /* AVCTP BT Init */
    avctp_bt_init();
#endif /* AVCTP */

#ifdef MCAP
    /* MCAP BT Init */
    mcap_bt_init();
#endif /* MCAP */

#ifdef OBEX
    /* OBEX BT init */
    obex_bt_init();
#endif /* OBEX */

#ifdef HCRP
    /* HCRP BT Init */
    hcrp_bt_init();
#endif /* HCRP */

#ifdef SMP
    smp_bt_init();
#endif /* SMP */

#ifdef ATT
    att_bt_init ();
#endif /* ATT */

#ifdef BT_COMMON_PL_SUPPORT_UL_CB
    if (NULL != ul_bt_on_init_cb)
    {
        ul_bt_on_init_cb();
    }
#endif /* BT_COMMON_PL_SUPPORT_UL_CB */
}


#ifndef BT_NO_BLUETOOTH_OFF

/* Bluetooth-OFF: Platform Lower Handler */
void bluetooth_off_lower_pl (void)
{
#ifdef BT_PLATFORM
    /* BT_PLATFORM BT Shutdown */
    hci_platform_bt_shutdown();
#endif /* BT_PLATFORM */

#ifdef BT_SOCKET
    /* SOCKET BT Shutdown */
    hci_socket_bt_shutdown();
#endif /* BT_SOCKET */

#ifdef BT_BCSP
    /* BCSP BT Shutdown */
    bcsp_bt_shutdown();
#endif /* BT_BCSP */

#ifdef BT_USB
    /* USB BT Shutdown */
    hci_usb_bt_shutdown();
#endif /* BT_USB */

#ifdef BT_UART
    /* UART BT Shutdown */
    hci_uart_bt_shutdown();
#endif /* BT_UART */
}


/* Bluetooth-OFF: Platform Upper Handler */
void bluetooth_off_upper_pl (void)
{
#ifdef BT_COMMON_PL_SUPPORT_UL_CB
    if (NULL != ul_bt_off_deinit_cb)
    {
        ul_bt_off_deinit_cb();
    }
#endif /* BT_COMMON_PL_SUPPORT_UL_CB */

#ifdef ATT
    /* Attribute Protocol BT Shutdown */
    att_bt_shutdown ();
#endif /* ATT */

#ifdef SMP
    smp_bt_shutdown ();
#endif /* SMP */

#ifdef OBEX
    /* OBEX BT Shutdown */
    obex_bt_shutdown();
#endif /* OBEX */

#ifdef MCAP
    /* MCAP BT Shutdown */
    mcap_bt_shutdown();
#endif /* MCAP */

#ifdef AVCTP
    /* AVCTP BT Shutdown */
    avctp_bt_shutdown();
#endif /* AVCTP */

#ifdef AVDTP
    /* AVDTP BT Shutdown */
    avdtp_bt_shutdown();
#endif /* AVDTP */

#ifdef BNEP
    /* BNEP BT Shutdown */
    bnep_bt_shutdown();
#endif /* BNEP */

#ifdef RFCOMM
    /* RFCOMM BT Shutdown */
    rfcomm_bt_shutdown();
#endif /* RFCOMM */

#ifdef SDP
    /* SDP BT Shutdown */
    sdp_bt_shutdown();
#endif /* SDP */

#ifdef CLASSIC_SEC_MANAGER
    /* Security Manager BT Shutdown */
    sm_bt_shutdown();
#endif /* CLASSIC_SEC_MANAGER */
}

#endif /* BT_NO_BLUETOOTH_OFF */

#ifdef BT_COMMON_PL_SUPPORT_UL_CB
void BT_ethermind_register_ul_cb_pl
     (
         /* IN */ BT_COMMON_UL_INIT_CB_PL   bt_on_init_cb,
         /* IN */ BT_COMMON_UL_DEINIT_CB_PL bt_off_deinit_cb
     )
{
    if (NULL != bt_on_init_cb)
    {
        ul_bt_on_init_cb = bt_on_init_cb;
    }

    if (NULL != bt_off_deinit_cb)
    {
        ul_bt_off_deinit_cb = bt_off_deinit_cb;
    }
}
#endif /* BT_COMMON_PL_SUPPORT_UL_CB */




