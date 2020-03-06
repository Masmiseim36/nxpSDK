/*
 * Copyright 2017, Cypress Semiconductor Corporation or a subsidiary of 
 * Cypress Semiconductor Corporation. All Rights Reserved.
 * 
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file
 *
 */

#include <stdlib.h>
#include "wwd_debug.h"
#include "wwd_assert.h"
#include "wwd_events.h"
#include "string.h"
#include "wwd_wifi.h"
#include "wwd_management.h"
#include "internal/wwd_internal.h"
#include "wwd_wifi_sleep.h"
#include "internal/wwd_sdpcm.h"
#include "network/wwd_buffer_interface.h"
#include "wwd_wiced.h"
#include "wwd.h"

#define WICED_WIFI_ROAMING_TRIGGER_MODE             ( WICED_WIFI_OPTIMIZE_BANDWIDTH_ROAMING_TRIGGER )
#define WICED_WIFI_ROAMING_TRIGGER_DELTA_IN_DBM     ( 5 )
#define WICED_WIFI_ROAMING_SCAN_PERIOD_IN_SECONDS   ( 10 )

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define WLC_EVENT_MSG_LINK      ( 0x0001 )  /** link is up */

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/
static void*          wiced_link_events_handler ( const wwd_event_header_t* event_header, const uint8_t* event_data, void* handler_user_data );
void (*link_fun_ptr)(int);
/******************************************************
 *               Variable Definitions
 ******************************************************/

/* Link management variables */
static const wwd_event_num_t        link_events[]           = { WLC_E_LINK, WLC_E_DEAUTH_IND, WLC_E_DISASSOC_IND, WLC_E_PSK_SUP, WLC_E_NONE };
static wiced_security_t             WICED_DEEP_SLEEP_SAVED_VAR( wiced_sta_security_type ) = WICED_SECURITY_UNKNOWN;
static wiced_bool_t                 link_up_event_received  = WICED_FALSE;
static wiced_country_code_t         WICED_DEEP_SLEEP_SAVED_VAR( country_code ) = WICED_COUNTRY_UNITED_STATES;

/******************************************************
 *               Function Definitions
 ******************************************************/

wiced_result_t wiced_wlan_connectivity_init( void )
{
    wiced_result_t              result;
    uint32_t                    wlan_rand;
    wiced_bool_t                random_seed_needed = WICED_TRUE;
    wiced_mac_t  mac;
    char         version[200];

    result = ( wiced_result_t )wwd_management_wifi_on( country_code );
    /* e.g. user requested an abort or other error */
    if ( result != WICED_SUCCESS )
    {
        WPRINT_NETWORK_DEBUG( ( "WWD " BUS " interface initialization FAILED: result %d\n", result) );
        return result;
    }

    /* get a seed, using hardware on wlan module (if supported); otherwise, seed comes due to difference in cycles for wiced_init completion */
    result = ( wiced_result_t )wwd_wifi_get_iovar_value( IOVAR_STR_RAND, &wlan_rand, WWD_STA_INTERFACE );

    /* Not always a supported operation */
    if ( result == WICED_SUCCESS )
    {
        random_seed_needed = WICED_FALSE;
        //wiced_crypto_add_entropy( &wlan_rand, sizeof( wlan_rand ) );
    }
    /* else generate a seed using elapsed time of wiced_init; at end of function */

    memset( &wlan_rand, 0, sizeof( wlan_rand ) );

    wwd_wifi_get_and_cache_mac_address( WWD_STA_INTERFACE );

    if ( wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE ) == WWD_SUCCESS )
    {
        WPRINT_WWD_INFO(("WLAN MAC Address : %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]));
    }

    if ( wwd_wifi_get_wifi_version( version, sizeof( version ) ) == WWD_SUCCESS )
    {
        WPRINT_WWD_INFO( ("WLAN Firmware    : %s\r\n", version ) );
    }

    /* minimize bootloader usage and start time from UART output */
    if ( wwd_wifi_get_clm_version( version, sizeof( version )) == WWD_SUCCESS )
    {
        WPRINT_WWD_INFO( ("WLAN CLM         : %s\r\n", version ) );
    }

    /* Configure roaming parameters */
    wwd_wifi_set_roam_trigger( WICED_WIFI_ROAMING_TRIGGER_MODE );
    wwd_wifi_set_roam_delta( WICED_WIFI_ROAMING_TRIGGER_DELTA_IN_DBM );
    wwd_wifi_set_roam_scan_period( WICED_WIFI_ROAMING_SCAN_PERIOD_IN_SECONDS );


    /* if unable to get random from Wi-Fi iovar, then do seeding here */
    if ( WICED_TRUE == random_seed_needed )
    {
        /* Small amount of variance in cycles taken to complete wiced_init; seed of PRNG */
        //wiced_crypto_prng_add_low_variability_entropy( host_platform_get_cycle_count( ) - cycles );
    }

    return WICED_SUCCESS;
}

static void* wiced_link_events_handler( const wwd_event_header_t* event_header, const uint8_t* event_data, void* handler_user_data )
{
    UNUSED_PARAMETER( event_data );

    if ( event_header->interface != (uint8_t) WWD_STA_INTERFACE)
    {
        return handler_user_data;
    }

    WPRINT_NETWORK_DEBUG( ("Link event (type, status, reason, flags) %u %u %u %u\n", (unsigned int)event_header->event_type, (unsigned int)event_header->status,
            (unsigned int)event_header->reason, (unsigned int)event_header->flags ) );

    WWD_IOCTL_LOG_ADD_EVENT(event_header->event_type, event_header->flags);

    switch ( event_header->event_type )
    {
        case WLC_E_LINK:
            if ( ( event_header->flags & WLC_EVENT_MSG_LINK ) != 0 )
            {
                switch ( wiced_sta_security_type )
                {
                    case WICED_SECURITY_OPEN:
                    case WICED_SECURITY_IBSS_OPEN:
                    case WICED_SECURITY_WPS_OPEN:
                    case WICED_SECURITY_WPS_SECURE:
                    case WICED_SECURITY_WEP_PSK:
                    case WICED_SECURITY_WEP_SHARED:
                    {
                        /* Advertise link-up immediately as no EAPOL is required */
                        if(link_fun_ptr != NULL)
                            (*link_fun_ptr)(1);
                        break;
                    }

                    case WICED_SECURITY_WPA_TKIP_PSK:
                    case WICED_SECURITY_WPA_AES_PSK:
                    case WICED_SECURITY_WPA_MIXED_PSK:
                    case WICED_SECURITY_WPA2_AES_PSK:
                    case WICED_SECURITY_WPA2_TKIP_PSK:
                    case WICED_SECURITY_WPA2_MIXED_PSK:
                    case WICED_SECURITY_WPA_TKIP_ENT:
                    case WICED_SECURITY_WPA_AES_ENT:
                    case WICED_SECURITY_WPA_MIXED_ENT:
                    case WICED_SECURITY_WPA2_TKIP_ENT:
                    case WICED_SECURITY_WPA2_AES_ENT:
                    case WICED_SECURITY_WPA2_MIXED_ENT:
                    {
                        link_up_event_received = WICED_TRUE;
                        break;
                    }

                    case WICED_SECURITY_UNKNOWN:
                    case WICED_SECURITY_FORCE_32_BIT:
                    default:
                    {
                        wiced_assert( "Bad Security type\r\n", 0 != 0 );
                        break;
                    }
                }
            }
            else
            {
                if(link_fun_ptr != NULL)
                    (*link_fun_ptr)(0);
            }
            break;

        case WLC_E_DEAUTH_IND:
        case WLC_E_DISASSOC_IND:
            if(link_fun_ptr != NULL)
                (*link_fun_ptr)(0);
            break;

        case WLC_E_PSK_SUP:
            {
                /* WLC_E_PSK_SUP is received. Check for status and reason. */
                if ( event_header->status == WLC_SUP_KEYED && event_header->reason == WLC_E_SUP_OTHER && link_up_event_received == WICED_TRUE )
                {
                    if(link_fun_ptr != NULL)
                        (*link_fun_ptr)(1);
                    link_up_event_received = WICED_FALSE;
                }
                break;
            }

        /* Note - These are listed to keep gcc pedantic checking happy */
        case WLC_E_NONE:
        case WLC_E_ROAM:
        case WLC_E_SET_SSID:
        case WLC_E_DEAUTH:
        case WLC_E_DISASSOC:
        case WLC_E_JOIN:
        case WLC_E_START:
        case WLC_E_AUTH:
        case WLC_E_AUTH_IND:
        case WLC_E_ASSOC:
        case WLC_E_ASSOC_IND:
        case WLC_E_REASSOC:
        case WLC_E_REASSOC_IND:
        case WLC_E_QUIET_START:
        case WLC_E_QUIET_END:
        case WLC_E_BEACON_RX:
        case WLC_E_MIC_ERROR:
        case WLC_E_NDIS_LINK:
        case WLC_E_TXFAIL:
        case WLC_E_PMKID_CACHE:
        case WLC_E_RETROGRADE_TSF:
        case WLC_E_PRUNE:
        case WLC_E_AUTOAUTH:
        case WLC_E_EAPOL_MSG:
        case WLC_E_SCAN_COMPLETE:
        case WLC_E_ADDTS_IND:
        case WLC_E_DELTS_IND:
        case WLC_E_BCNSENT_IND:
        case WLC_E_BCNRX_MSG:
        case WLC_E_BCNLOST_MSG:
        case WLC_E_ROAM_PREP:
        case WLC_E_PFN_NET_FOUND:
        case WLC_E_PFN_NET_LOST:
        case WLC_E_RESET_COMPLETE:
        case WLC_E_JOIN_START:
        case WLC_E_ROAM_START:
        case WLC_E_ASSOC_START:
        case WLC_E_IBSS_ASSOC:
        case WLC_E_RADIO:
        case WLC_E_PSM_WATCHDOG:
        case WLC_E_CCX_ASSOC_START:
        case WLC_E_CCX_ASSOC_ABORT:
        case WLC_E_PROBREQ_MSG:
        case WLC_E_SCAN_CONFIRM_IND:
        case WLC_E_COUNTRY_CODE_CHANGED:
        case WLC_E_EXCEEDED_MEDIUM_TIME:
        case WLC_E_ICV_ERROR:
        case WLC_E_UNICAST_DECODE_ERROR:
        case WLC_E_MULTICAST_DECODE_ERROR:
        case WLC_E_TRACE:
        case WLC_E_BTA_HCI_EVENT:
        case WLC_E_IF:
        case WLC_E_P2P_DISC_LISTEN_COMPLETE:
        case WLC_E_RSSI:
        case WLC_E_PFN_SCAN_COMPLETE:
        case WLC_E_EXTLOG_MSG:
        case WLC_E_ACTION_FRAME:
        case WLC_E_ACTION_FRAME_COMPLETE:
        case WLC_E_PRE_ASSOC_IND:
        case WLC_E_PRE_REASSOC_IND:
        case WLC_E_CHANNEL_ADOPTED:
        case WLC_E_AP_STARTED:
        case WLC_E_DFS_AP_STOP:
        case WLC_E_DFS_AP_RESUME:
        case WLC_E_WAI_STA_EVENT:
        case WLC_E_WAI_MSG:
        case WLC_E_ESCAN_RESULT:
        case WLC_E_ACTION_FRAME_OFF_CHAN_COMPLETE:
        case WLC_E_PROBRESP_MSG:
        case WLC_E_P2P_PROBREQ_MSG:
        case WLC_E_DCS_REQUEST:
        case WLC_E_FIFO_CREDIT_MAP:
        case WLC_E_ACTION_FRAME_RX:
        case WLC_E_WAKE_EVENT:
        case WLC_E_RM_COMPLETE:
        case WLC_E_HTSFSYNC:
        case WLC_E_OVERLAY_REQ:
        case WLC_E_CSA_COMPLETE_IND:
        case WLC_E_EXCESS_PM_WAKE_EVENT:
        case WLC_E_PFN_SCAN_NONE:
        case WLC_E_PFN_SCAN_ALLGONE:
        case WLC_E_GTK_PLUMBED:
        case WLC_E_ASSOC_IND_NDIS:
        case WLC_E_REASSOC_IND_NDIS:
        case WLC_E_ASSOC_REQ_IE:
        case WLC_E_ASSOC_RESP_IE:
        case WLC_E_ASSOC_RECREATED:
        case WLC_E_ACTION_FRAME_RX_NDIS:
        case WLC_E_AUTH_REQ:
        case WLC_E_TDLS_PEER_EVENT:
        case WLC_E_SPEEDY_RECREATE_FAIL:
        case WLC_E_NATIVE:
        case WLC_E_PKTDELAY_IND:
        case WLC_E_AWDL_AW:
        case WLC_E_AWDL_ROLE:
        case WLC_E_AWDL_EVENT:
        case WLC_E_NIC_AF_TXS:
        case WLC_E_NAN:
        case WLC_E_BEACON_FRAME_RX:
        case WLC_E_SERVICE_FOUND:
        case WLC_E_GAS_FRAGMENT_RX:
        case WLC_E_GAS_COMPLETE:
        case WLC_E_P2PO_ADD_DEVICE:
        case WLC_E_P2PO_DEL_DEVICE:
        case WLC_E_WNM_STA_SLEEP:
        case WLC_E_TXFAIL_THRESH:
        case WLC_E_PROXD:
        case WLC_E_IBSS_COALESCE:
        case WLC_E_AWDL_RX_PRB_RESP:
        case WLC_E_AWDL_RX_ACT_FRAME:
        case WLC_E_AWDL_WOWL_NULLPKT:
        case WLC_E_AWDL_PHYCAL_STATUS:
        case WLC_E_AWDL_OOB_AF_STATUS:
        case WLC_E_AWDL_SCAN_STATUS:
        case WLC_E_AWDL_AW_START:
        case WLC_E_AWDL_AW_END:
        case WLC_E_AWDL_AW_EXT:
        case WLC_E_AWDL_PEER_CACHE_CONTROL:
        case WLC_E_CSA_START_IND:
        case WLC_E_CSA_DONE_IND:
        case WLC_E_CSA_FAILURE_IND:
        case WLC_E_CCA_CHAN_QUAL:
        case WLC_E_BSSID:
        case WLC_E_TX_STAT_ERROR:
        case WLC_E_BCMC_CREDIT_SUPPORT:
        case WLC_E_PSTA_PRIMARY_INTF_IND:
        case WLC_E_RRM:
        case WLC_E_ULP:
        case WLC_E_LAST:
        case WLC_E_FORCE_32_BIT:
        case WLC_E_BT_WIFI_HANDOVER_REQ:
        case WLC_E_PFN_BEST_BATCHING:
        case WLC_E_SPW_TXINHIBIT:
        case WLC_E_FBT_AUTH_REQ_IND:
        case WLC_E_RSSI_LQM:
        case WLC_E_PFN_GSCAN_FULL_RESULT:
        case WLC_E_PFN_SWC:
        case WLC_E_AUTHORIZED:
        case WLC_E_PROBREQ_MSG_RX:
        case WLC_E_RMC_EVENT:
        case WLC_E_DPSTA_INTF_IND:
        default:
            wiced_assert( "Received event which was not registered\n", 0 != 0 );
            break;
    }
    return handler_user_data;
}

wiced_result_t register_link_events(wiced_security_t security, void * callback_fun_addr)
{
    wiced_sta_security_type = security;
    link_fun_ptr = (void(*)(int))callback_fun_addr;

    return wwd_management_set_event_handler( link_events, wiced_link_events_handler, NULL, WWD_STA_INTERFACE );
}

