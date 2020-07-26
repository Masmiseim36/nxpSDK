/*
* Copyright 2018-2019 NXP
* All rights reserved. 
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "wwd.h"
#include "wwd_wiced.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CIRCULAR_RESULT_BUFF_SIZE (40)

/* Macros for comparing MAC addresses */
#define CMP_MAC(a, b)                                            \
    (((((unsigned char *)a)[0]) == (((unsigned char *)b)[0])) && \
     ((((unsigned char *)a)[1]) == (((unsigned char *)b)[1])) && \
     ((((unsigned char *)a)[2]) == (((unsigned char *)b)[2])) && \
     ((((unsigned char *)a)[3]) == (((unsigned char *)b)[3])) && \
     ((((unsigned char *)a)[4]) == (((unsigned char *)b)[4])) && \
     ((((unsigned char *)a)[5]) == (((unsigned char *)b)[5])))

/*******************************************************************************
 * Variables
 ******************************************************************************/
static wiced_mac_t bssid_list[200]; /* List of BSSID (AP MAC addresses) that have been scanned */
static host_semaphore_type_t num_scan_results_semaphore;
static wiced_scan_result_t result_buff[CIRCULAR_RESULT_BUFF_SIZE];
static uint16_t result_buff_write_pos = 0;
static uint16_t result_buff_read_pos = 0;
#ifdef SAVE_SSIDS
static char scanned_ssids[320] = {'\0'};
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void print_records();
wwd_result_t test_scan();
void print_record(wiced_scan_result_t *record, int record_number);
#ifdef SAVE_SSIDS
char* get_saved_ssids();
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
void scan_results_handler(wiced_scan_result_t **result_ptr, void *user_data, wiced_scan_status_t status)
{
    if (result_ptr == NULL)
    {
        /* finished */
        result_buff[result_buff_write_pos].channel = 0xff;
        host_rtos_set_semaphore(&num_scan_results_semaphore, WICED_FALSE);
        WPRINT_WWD_INFO(("scan completed\r\n"));
        return;
    }
    if (result_buff_write_pos >= CIRCULAR_RESULT_BUFF_SIZE)
    {
        WPRINT_WWD_INFO(("scan result overflow\r\n"));
        return;
    }

    wiced_scan_result_t *record = (*result_ptr);

    /* Check the list of BSSID values which have already been printed */
    wiced_mac_t *tmp_mac = bssid_list;
    while (NULL_MAC(tmp_mac->octet) == WICED_FALSE)
    {
        if (CMP_MAC(tmp_mac->octet, record->BSSID.octet) == WICED_TRUE)
        {
            /* already seen this BSSID */
            return;
        }
        tmp_mac++;
    }

    /* New BSSID - add it to the list */
    memcpy(&tmp_mac->octet, record->BSSID.octet, sizeof(wiced_mac_t));

    /* Add the result to the list and set the pointer for the next result */
    result_buff_write_pos++;
    *result_ptr = &result_buff[result_buff_write_pos];
    // host_rtos_set_semaphore( &num_scan_results_semaphore, WICED_FALSE );

    wiced_assert("Circular result buffer overflow", result_buff_write_pos != result_buff_read_pos);
}

wwd_result_t test_scan()
{
    wwd_result_t result;
    static wiced_scan_result_t *result_ptr = (wiced_scan_result_t *)&result_buff;
    PRINTF("Scanning available networks...\r\n");

    if (host_rtos_init_binary_semaphore(&num_scan_results_semaphore) != WWD_SUCCESS)
    {
        PRINTF("Error initializing scan complete semaphore\r\n");
    }

    memset(bssid_list, 0, sizeof(bssid_list));
    wwd_wifi_set_scan_suppress(0);
    result_buff_write_pos = 0;
    result_buff_read_pos = 0;
    result = wwd_wifi_scan(WICED_SCAN_TYPE_ACTIVE, WICED_BSS_TYPE_ANY, NULL, NULL, NULL, NULL, scan_results_handler,
                           (wiced_scan_result_t **)&result_ptr, NULL, WWD_STA_INTERFACE);

    if (result != WWD_SUCCESS)
    {
        WPRINT_WWD_INFO(("Error starting scan\r\n"));
        return 1;
    }

    result = host_rtos_get_semaphore(&num_scan_results_semaphore, 10000, WICED_FALSE);
    print_records();
    return result;
}

void print_record(wiced_scan_result_t *record, int record_number)
{
    int k;

    /* Print SSID */
    PRINTF("\r\n#%03d SSID          : ", record_number);
    for (k = 0; k < (int)record->SSID.length; k++)
    {
        PRINTF("%c", (char)record->SSID.value[k]);

    }
#ifdef SAVE_SSIDS
    if((strlen(scanned_ssids) + record->SSID.length) >= sizeof(scanned_ssids)-1){
            PRINTF("[!] Cannot save another ssid!\r\n");
            __BKPT(0);
    } else {
        strcat(scanned_ssids, record->SSID.value);
        scanned_ssids[strlen(scanned_ssids)] = ';';
        scanned_ssids[strlen(scanned_ssids)+1] = '\0';
    }    
#endif
    PRINTF("\r\n");

    wiced_assert("error",
                 (record->bss_type == WICED_BSS_TYPE_INFRASTRUCTURE) || (record->bss_type == WICED_BSS_TYPE_ADHOC));

    /* Print other network characteristics */
    PRINTF("     BSSID         : %02X:%02X:%02X:%02X:%02X:%02X\r\n", (unsigned int)record->BSSID.octet[0],
           (unsigned int)record->BSSID.octet[1], (unsigned int)record->BSSID.octet[2],
           (unsigned int)record->BSSID.octet[3], (unsigned int)record->BSSID.octet[4],
           (unsigned int)record->BSSID.octet[5]);
    PRINTF("     RSSI          : %ddBm", (int)record->signal_strength);
    if (record->flags & WICED_SCAN_RESULT_FLAG_RSSI_OFF_CHANNEL)
    {
        PRINTF(" (off-channel)\r\n");
    }
    else
    {
        PRINTF("\r\n");
    }
    PRINTF("     Max Data Rate : %d Mbits/s\r\n", record->max_data_rate / 1000);
    PRINTF("     Network Type  : %s\r\n", (record->bss_type == WICED_BSS_TYPE_INFRASTRUCTURE) ?
                                            "Infrastructure" :
                                            (record->bss_type == WICED_BSS_TYPE_ADHOC) ? "Ad hoc" : "Unknown");
    PRINTF("     Security      : %s\r\n",
           (record->security == WICED_SECURITY_OPEN) ? "Open" : (record->security == WICED_SECURITY_WEP_PSK) ?
                                                       "WEP" :
                                                       (record->security == WICED_SECURITY_WPA_TKIP_PSK) ?
                                                       "WPA" :
                                                       (record->security == WICED_SECURITY_WPA2_AES_PSK) ?
                                                       "WPA2 AES" :
                                                       (record->security == WICED_SECURITY_WPA2_MIXED_PSK) ?
                                                       "WPA2 Mixed" :
                                                       "Unknown");
    PRINTF("     Radio Band    : %s\r\n", (record->band == WICED_802_11_BAND_5GHZ) ? "5GHz" : "2.4GHz");
    PRINTF("     Channel       : %d\r\n", (int)record->channel);
    result_buff_read_pos++;
    if (result_buff_read_pos >= (uint16_t)CIRCULAR_RESULT_BUFF_SIZE)
    {
        result_buff_read_pos = 0;
    }
    record_number++;
}

#ifdef SAVE_SSIDS
char* get_saved_ssids(){
  return scanned_ssids;
}
#endif

void print_records()
{
    int ii;
    for (ii = 0; ii < result_buff_write_pos; ii++)
    {
        print_record(&result_buff[ii], ii + 1);
    }
    
}

