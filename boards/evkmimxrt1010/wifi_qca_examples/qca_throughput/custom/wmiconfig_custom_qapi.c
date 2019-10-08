// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// - Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <main.h>
#include <throughput.h>
#include "qcom_api.h"
//#include "qcom_legacy.h"
#if !ENABLE_STACK_OFFLOAD
#endif
#include "fsl_device_registers.h"

// Replacing with the same function of the QCA WIFI driver
uint32_t Custom_Api_Mediactl(QCA_CONTEXT_STRUCT_PTR enet_ptr, uint32_t command_id, void *inout_param);
#define ENET_mediactl Custom_Api_Mediactl

A_STATUS is_driver_initialized(void)
{
    int32_t error, dev_status;
    _qca_handle qcahandle;

    qcahandle = get_active_device_handle();

    error = ENET_mediactl(qcahandle, QCA_MEDIACTL_IS_INITIALIZED, &dev_status);
    if (QCA_OK != error || dev_status == false)
    {
        return A_ERROR;
    }

    return A_OK;
}

extern uint8_t g_driverState;
extern uint32_t concurrent_connect_flag;
extern uint16_t ap_channel_hint;
extern uint8_t ssid_str_concurrent[MAX_SSID_LENGTH];
/*FUNCTION*-----------------------------------------------------------------
 *
 * Function Name  : check_driver_state
 * Returned Value : A_OK or A_ERROR
 * Comments       : Checks if driver is loaded or not
 *
 *END------------------------------------------------------------------*/
int32_t check_driver_state(void)
{
#if 0
    uint32_t state;
    ATH_IOCTL_PARAM_STRUCT ath_param;
    uint32_t error;

    ath_param.cmd_id = ATH_IS_DRIVER_INITIALIZED;
    ath_param.data = &state;
    error = ath_ioctl_handler (handle,&ath_param);
    if (A_OK != error)
    {
      PRINTF("Driver not initialized" NL);
    }
#else
    if (g_driverState != DRIVER_STATE_RUN)
    {
        PRINTF("Driver is still in INIT state" NL);
        return (A_ERROR);
    }
#endif
    return (A_OK);
}

A_STATUS handle_ioctl(ATH_IOCTL_PARAM_STRUCT *inout_param)
{
    _qca_handle qcahandle;

    qcahandle      = get_active_device_handle();
    A_STATUS error = (A_STATUS)ENET_mediactl(qcahandle, QCA_MEDIACTL_VENDOR_SPECIFIC, inout_param);

    return error;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : driver_updown_indicator()
 * Returned Value  : A_OK - success, A_ERROR - failure
 * Comments	: handles the state of driver either up/down
 *
 *END*-----------------------------------------------------------------*/
uint8_t driver_status(uint8_t val)
{
    static uint8_t driver_down_flag = DRIVER_UP;
    /* if the driver is up set this val to driver_up*/
    if (val == DRIVER_UP)
    {
        driver_down_flag = DRIVER_UP;
    }
    /* if the driver is down set this val to driver_down*/
    else if (val == DRIVER_DOWN)
    {
        driver_down_flag = DRIVER_DOWN;
    }
    /* we will read the driver status from reset fn
       if the driver status indicates down we will
       not allow reset
     */
    else if (val == READ_STATUS)
    {
        return driver_down_flag;
    }
    return driver_down_flag;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : NetConnect()
 * Returned Value  : 0 - success, 1 - failure
 * Comments	: Handles connection to AP after WPS
 *
 *END*-----------------------------------------------------------------*/
int32_t NetConnect(ATH_NETPARAMS *pNetparams)
{
    int32_t status = A_ERROR;
    qcom_wps_credentials_t wpsCred;
    uint32_t enet_device = get_active_device();

    A_MEMZERO(&wpsCred, sizeof(qcom_wps_credentials_t));

    do
    {
        if (pNetparams->ssid_len == 0)
        {
            // PRINTF("WPS failed" NL);
            break;
        }
        else
        {
            PRINTF("SSID received %s" NL, pNetparams->ssid);
            if (pNetparams->sec_type == QCA_MEDIACTL_SECURITY_TYPE_WPA2)
            {
                PRINTF("Security type is WPA2" NL);
                PRINTF("Passphrase %s" NL, (char *)pNetparams->u.passphrase);
            }
            else if (pNetparams->sec_type == QCA_MEDIACTL_SECURITY_TYPE_WPA)
            {
                PRINTF("Security type is WPA" NL);
                PRINTF("Passphrase %s" NL, (char *)pNetparams->u.passphrase);
            }
            else if (pNetparams->sec_type == QCA_MEDIACTL_SECURITY_TYPE_WEP)
            {
                PRINTF("Security type is WEP" NL);
                PRINTF("WEP key %s" NL, (char *)pNetparams->u.wepkey);
                PRINTF("Key index %d" NL, pNetparams->key_index);
            }
            else if (pNetparams->sec_type == QCA_MEDIACTL_SECURITY_TYPE_NONE)
            {
                PRINTF("Security type is None" NL);
            }

            set_callback();

            strncpy((char *)&wpsCred.ssid, (char *)pNetparams->ssid, sizeof(wpsCred.ssid));
            wpsCred.ssid_len = strlen(wpsCred.ssid.ssid);
            qcom_set_ssid(enet_device, &wpsCred.ssid);

            if (pNetparams->sec_type == QCA_MEDIACTL_SECURITY_TYPE_WPA2)
            {
                wpsCred.auth_type = WLAN_AUTH_WPA2_PSK;
                wpsCred.encr_type = WLAN_CRYPT_AES_CRYPT;

                if (strlen((char *)pNetparams->u.passphrase) != 64)
                {
                    QCOM_PASSPHRASE tmp_pass = {0};
                    strncpy((char *)&tmp_pass, (char *)pNetparams->u.passphrase, sizeof(tmp_pass));
                    qcom_sec_set_passphrase(enet_device, &tmp_pass);
                }
                else
                {
                    if (A_OK != qcom_sec_set_pmk(enet_device, (char *)pNetparams->u.passphrase))
                        break;
                }
            }
            else if (pNetparams->sec_type == QCA_MEDIACTL_SECURITY_TYPE_WPA)
            {
                wpsCred.auth_type = WLAN_AUTH_WPA_PSK;
                wpsCred.encr_type = WLAN_CRYPT_TKIP_CRYPT;
                if (strlen((char *)pNetparams->u.passphrase) != 64)
                {
                    QCOM_PASSPHRASE tmp_pass = {0};
                    strncpy((char *)&tmp_pass, (char *)pNetparams->u.passphrase, sizeof(tmp_pass));
                    qcom_sec_set_passphrase(enet_device, &tmp_pass);
                }
                else
                {
                    if (A_OK != qcom_sec_set_pmk(enet_device, (char *)pNetparams->u.passphrase))
                        break;
                }
            }
            else if (pNetparams->sec_type == QCA_MEDIACTL_SECURITY_TYPE_WEP)
            {
                wpsCred.key_idx = pNetparams->key_index;
                strcpy((char *)wpsCred.key, (char *)pNetparams->u.wepkey);
                wpsCred.key_len = strlen((char *)pNetparams->u.wepkey);
            }
            else if (pNetparams->sec_type == QCA_MEDIACTL_SECURITY_TYPE_NONE)
            {
                wpsCred.auth_type = WLAN_AUTH_NONE;
                //	ATH_SET_SEC_TYPE (IPCFG_default_enet_device,"none");
            }

            // iwcfg_set_mode (IPCFG_default_enet_device,"managed");
            qcom_wps_set_credentials(enet_device, &wpsCred);
            qcom_wps_connect(enet_device);
            status = 0;
        }
    } while (0);

    return status;
}

int32_t CompleteWPS(ATH_NETPARAMS *pNetparams, uint8_t block, wifimode_t mode_flag)
{
    uint32_t status = 0;
    ATH_IOCTL_PARAM_STRUCT param;
    A_STATUS error;
    uint32_t enet_device;
    QCOM_WLAN_DEV_MODE wifimode = QCOM_WLAN_DEV_MODE_INVALID;

    enet_device = get_active_device();

    pNetparams->error      = 0;
    pNetparams->dont_block = (block) ? 0 : 1;

    param.cmd_id = ATH_AWAIT_WPS_COMPLETION;
    param.data   = pNetparams;
    param.length = sizeof(*pNetparams);

    /* this will block until the WPS operation completes or times out */
    error = HANDLE_IOCTL(&param);

    if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
    {
        return A_ERROR;
    }
    if (wifimode == QCOM_WLAN_DEV_MODE_STATION)
    {
        if (error != A_OK)
        {
            pNetparams->error = pNetparams->error ? pNetparams->error : A_EBUSY;
        }
    }

    do
    {
        if (A_PENDING == error)
        {
            break;
        }

        status = 1;

        if (A_OK != error && pNetparams->error == A_OK)
        {
            pNetparams->error = error;
            PRINTF("ATH_AWAIT_WPS_COMPLETION failed!" NL);
            break;
        }

        if (pNetparams->error != 0)
        {
            switch (pNetparams->error)
            {
                case ATH_WPS_ERROR_INVALID_START_INFO:

                    PRINTF("WPS error: invalid start info" NL);
                    break;
                case ATH_WPS_ERROR_MULTIPLE_PBC_SESSIONS:
                    PRINTF("WPS error: Multiple PBC Sessions" NL);
                    break;

                case ATH_WPS_ERROR_WALKTIMER_TIMEOUT:
                    PRINTF("WPS error: Walktimer Timeout" NL);
                    break;

                case ATH_WPS_ERROR_M2D_RCVD:
                    PRINTF("WPS error: M2D RCVD" NL);
                    break;

                case ATH_WPS_ERROR_PWD_AUTH_FAIL:
                    PRINTF("WPS error: AUTH FAIL" NL);
                    break;

                case ATH_WPS_ERROR_CANCELLED:
                    PRINTF("WPS error: WPS CANCEL" NL);
                    break;

                case ATH_WPS_ERROR_INVALID_PIN:
                    PRINTF("WPS error: INVALID PIN" NL);
                    break;

                default:
                    // PRINTF("WPS error: unknown %d" NL,pNetparams->error);
                    break;
            }
        }
    } while (0);

    if (A_ERROR == qcom_op_get_mode(enet_device, &wifimode))
    {
        return A_ERROR;
    }
    if (wifimode == QCOM_WLAN_DEV_MODE_AP)
    {
        if (0x00 == pNetparams->error && A_OK == error)
        {
            PRINTF(NL "***** WPS PROFILE ****" NL "%s" NL, pNetparams->ssid);
            PRINTF("SSID received %s" NL, pNetparams->ssid);
            if (pNetparams->sec_type == QCA_MEDIACTL_SECURITY_TYPE_WPA2)
            {
                PRINTF("Security type is WPA2, Encryption Type is AES" NL);
                PRINTF("Passphrase %s" NL, (char *)pNetparams->u.passphrase);
            }
            else
            {
                PRINTF("Security type is Open" NL);
            }
            status = 0;
        }
        else
        {
            pNetparams->error = (pNetparams->error) ? pNetparams->error : A_EBUSY;
            status            = 0;
        }
    }

    return status;
}

void application_frame_cb(A_VOID *ptr)
{
    uint16_t i, print_length, j = 0;
    uint8_t *pData;

    print_length = 32;
    PRINTF("frame (%d):" NL, (int)A_NETBUF_LEN(ptr));
    /* only print the first 64 bytes of each frame */
    if (A_NETBUF_LEN(ptr) < print_length)
        print_length = A_NETBUF_LEN(ptr);

    pData = A_NETBUF_DATA(ptr);

    for (i = 0; i < print_length; i++)
    {
        PRINTF("0x%02x, ", pData[i]);
        if (j++ == 7)
        {
            j = 0;
            PRINTF(NL);
        }
    }

    if (j)
    {
        PRINTF(NL);
    }

    A_NETBUF_FREE(ptr);
}

extern uint32_t ar4XXX_boot_param;

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : ath_driver_state()
 * Returned Value  : A_OK - successful completion or
 *		   A_ERROR - failed.
 * Comments		  : Handles sriver reset functionality
 *
 *END*-----------------------------------------------------------------*/

int32_t ath_driver_state(int32_t argc, char *argv[])
{
    int32_t error;
    ATH_IOCTL_PARAM_STRUCT ath_param;
    uint32_t value;

    do
    {
        if (argc != 1)
            break;

        ath_param.cmd_id = ATH_CHIP_STATE;
        ath_param.length = sizeof(value);
        // value = atoi(argv[0]);

        if (strcmp(argv[0], "up") == 0)
        {
            value = 1;

            ath_param.data = &value;
            error          = HANDLE_IOCTL(&ath_param);

            if (error == A_OK)
            {
                driver_status(DRIVER_UP);
                return A_OK;
            }
            else
            {
                PRINTF("driver ioctl error" NL);
                return A_ERROR;
            }
        }
        else if (strcmp(argv[0], "down") == 0)
        {
            value          = 0;
            ath_param.data = &value;
            error          = HANDLE_IOCTL(&ath_param);

            if (error == A_OK)
            {
                driver_status(DRIVER_DOWN);
                return A_OK;
            }
            else
            {
                PRINTF("driver ioctl error" NL);
                return A_ERROR;
            }
        }
        else if (strcmp(argv[0], "flushdown") == 0)
        {
            /*Check if no packets are queued, if TX is pending, then wait*/
            while (get_tx_status() != ATH_TX_STATUS_IDLE)
            {
                A_MDELAY(500);
            }

            value          = 0;
            ath_param.data = &value;
            error          = HANDLE_IOCTL(&ath_param);

            if (error == A_OK)
            {
                return A_OK;
            }
            else
            {
                PRINTF("driver ioctl error" NL);
                return A_ERROR;
            }
        }
        else if (strcmp(argv[0], "raw") == 0)
        {
            ar4XXX_boot_param = AR4XXX_PARAM_RAWMODE_BOOT;
            value             = 0;
            ath_param.data    = &value;
            if ((error = HANDLE_IOCTL(&ath_param)) != A_OK)
                return A_ERROR;

            value          = 1;
            ath_param.data = &value;
            if ((error = HANDLE_IOCTL(&ath_param)) != A_OK)
                return A_ERROR;
        }
        else if (strcmp(argv[0], "rawq") == 0)
        {
            PRINTF("Not supported" NL);
            return A_OK;
        }
        else if (strcmp(argv[0], "quad") == 0)
        {
            PRINTF("Not supported" NL);
            return A_OK;
        }
        else if (strcmp(argv[0], "normal") == 0)
        {
            ar4XXX_boot_param = AR4XXX_PARAM_MODE_NORMAL;
            value             = 0;
            ath_param.data    = &value;
            if ((error = HANDLE_IOCTL(&ath_param)) != A_OK)
                return A_ERROR;

            value          = 1;
            ath_param.data = &value;
            if ((error = HANDLE_IOCTL(&ath_param)) != A_OK)
                return A_ERROR;
        }
        else
        {
            PRINTF("Invalid parameter: try wmiconfig --help" NL);
        }

        return A_OK;

    } while (0);

    PRINTF("param error: driver state" NL);
    return A_ERROR;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : ath_reg_query()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : reads / writes AR600X registers.
 *
 *END*------------------------------------------------------------------------*/
int32_t ath_reg_query(int32_t argc, char *argv[])
{
    int32_t error;
    uint8_t print_result = 0;
    ATH_IOCTL_PARAM_STRUCT ath_param;
    ATH_REGQUERY ath_regquery;
    _qca_handle qcahandle;

    qcahandle = get_active_device_handle();

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    do
    {
        if (argc < 2 || argc > 4)
            break;

        ath_regquery.operation = atoi(argv[0]);
        ath_regquery.address   = mystrtoul(argv[1], NULL, 16);

        if (ath_regquery.operation == ATH_REG_OP_READ)
        {
            if (argc != 2)
                break;

            print_result = 1;
        }
        else if (ath_regquery.operation == ATH_REG_OP_WRITE)
        {
            if (argc != 3)
                break;

            ath_regquery.value = mystrtoul(argv[2], NULL, 16);
        }
        else if (ath_regquery.operation == ATH_REG_OP_RMW)
        {
            if (argc != 4)
                break;

            ath_regquery.value = mystrtoul(argv[2], NULL, 16);
            ath_regquery.mask  = mystrtoul(argv[3], NULL, 16);
        }
        else
        {
            break;
        }

        ath_param.cmd_id = ATH_REG_QUERY;
        ath_param.data   = &ath_regquery;
        ath_param.length = sizeof(ath_regquery);

        error = ENET_mediactl(qcahandle, QCA_MEDIACTL_VENDOR_SPECIFIC, &ath_param);

        if (error == A_OK)
        {
            if (print_result)
            {
                PRINTF("value=%08x" NL, (unsigned int)ath_regquery.value);
            }
            return A_OK;
        }
        else
        {
            PRINTF("driver ioctl error" NL);
            return A_ERROR;
        }
    } while (0);

    PRINTF("param error: register query" NL);
    return A_ERROR;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : ath_mem_query()
 * Returned Value  : A_OK - on successful completion
 *					A_ERROR - on any failure.
 * Comments        : reads / writes AR600X registers.
 *
 *END*------------------------------------------------------------------------*/
int32_t ath_mem_query(int32_t argc, char *argv[])
{
    int32_t error;
    ATH_IOCTL_PARAM_STRUCT ath_param;
    ATH_MEMQUERY ath_memquery;
    _qca_handle qcahandle;

    qcahandle = get_active_device_handle();

    do
    {
        ath_param.cmd_id = ATH_MEM_QUERY;
        ath_param.data   = &ath_memquery;
        ath_param.length = sizeof(ath_memquery);

        error = ENET_mediactl(qcahandle, QCA_MEDIACTL_VENDOR_SPECIFIC, &ath_param);

        PRINTF("firmware memory stats:" NL);

        if (error == A_OK)
        {
            PRINTF("literals = %lu" NL, ath_memquery.literals);

            PRINTF("rodata = %lu" NL, ath_memquery.rodata);

            PRINTF("data = %lu" NL, ath_memquery.data);

            PRINTF("bss = %lu" NL, ath_memquery.bss);

            PRINTF("text = %lu" NL, ath_memquery.text);

            PRINTF("heap remaining = %lu" NL, ath_memquery.heap);

            return A_OK;
        }
        else
        {
            PRINTF("driver ioctl error" NL);
            return A_ERROR;
        }
    } while (0);
}

_qca_handle get_active_device_handle(void)
{
    return (p_Driver_Cxt[active_device]);
}

#if 0
/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : set_power_mode
* Returned Value  : A_OK if success else ERROR
*
* Comments        : Sets current power mode to Power-Save or Max-Perf
*
*END*-----------------------------------------------------------------*/
int32_t set_power_mode_old(char* pwr_mode)
{
	int mode;
    uint32_t enet_device;

    enet_device = get_active_device();

	mode = atoi(pwr_mode);

	if (mode != 0 && mode != 1)
	{
		PRINTF("Invalid power mode" NL);
		return A_ERROR;
	}

	return(iwcfg_set_power(enet_device,mode,0));
}
#endif

int32_t wmi_set_scan(uint32_t dev_num, void *start_scan)
{
    int32_t error, i = 0;
    QCA_SCAN_LIST param = {0};
    uint8_t temp_ssid[33];
    int16_t num_scan;

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    if (dev_num < BSP_ENET_DEVICE_COUNT)
    {
#ifndef USE_QCOM_REL_3_3_API
        error = qcom_set_scan(dev_num, (qcom_start_scan_params_t *)start_scan);
#else
        error = qcom_set_scan(dev_num);
#endif
        if (A_OK != error)
        {
            return error;
        }

        error = qcom_get_scan(dev_num, (QCOM_BSS_SCAN_INFO **)&(param.scan_info_list), &num_scan);
        if (A_OK != error)
        {
            return error;
        }
        PRINTF("Scan result count:%d" NL, num_scan);

        for (i = 0; i < num_scan; i++)
        {
            memcpy(temp_ssid, param.scan_info_list[i].ssid, param.scan_info_list[i].ssid_len);

            temp_ssid[param.scan_info_list[i].ssid_len] = '\0';

            if (param.scan_info_list[i].ssid_len == 0)
            {
                PRINTF("ssid = SSID Not available" NL);
            }
            else
            {
#if ENABLE_SCC_MODE
                if ((concurrent_connect_flag == 0x0F) &&
                    (strcmp((char const *)ssid_str_concurrent, (char const *)temp_ssid) == 0))
                {
                    ap_channel_hint = param.scan_info_list[i].channel;
                    PRINTF("scan res ch : %d" NL, ap_channel_hint);
                    return A_OK;
                }
                else if ((concurrent_connect_flag == 0x0F))
                {
                    return A_OK;
                }
                else
#endif
                {
                    PRINTF("ssid = %s" NL, temp_ssid);

                    PRINTF("bssid = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x" NL, param.scan_info_list[i].bssid[0],
                           param.scan_info_list[i].bssid[1], param.scan_info_list[i].bssid[2],
                           param.scan_info_list[i].bssid[3], param.scan_info_list[i].bssid[4],
                           param.scan_info_list[i].bssid[5]);
                    PRINTF("channel = %d" NL, param.scan_info_list[i].channel);

                    PRINTF("indicator = %d" NL, param.scan_info_list[i].rssi);

                    PRINTF("security = ");

                    if (param.scan_info_list[i].security_enabled)
                    {
                        if (param.scan_info_list[i].rsn_auth || param.scan_info_list[i].rsn_cipher)
                        {
                            PRINTF(NL);
                            PRINTF("RSN/WPA2= ");
                        }

                        if (param.scan_info_list[i].rsn_auth)
                        {
                            PRINTF(" {");
                            if (param.scan_info_list[i].rsn_auth & SECURITY_AUTH_1X)
                            {
                                PRINTF("802.1X ");
                            }

                            if (param.scan_info_list[i].rsn_auth & SECURITY_AUTH_PSK)
                            {
                                PRINTF("PSK ");
                            }
                            PRINTF("}");
                        }

                        if (param.scan_info_list[i].rsn_cipher)
                        {
                            PRINTF(" {");
                            /* AP security can support multiple options hence
                             * we check each one separately. Note rsn == wpa2 */
                            if (param.scan_info_list[i].rsn_cipher & ATH_CIPHER_TYPE_WEP)
                            {
                                PRINTF("WEP ");
                            }

                            if (param.scan_info_list[i].rsn_cipher & ATH_CIPHER_TYPE_TKIP)
                            {
                                PRINTF("TKIP ");
                            }

                            if (param.scan_info_list[i].rsn_cipher & ATH_CIPHER_TYPE_CCMP)
                            {
                                PRINTF("AES ");
                            }
                            PRINTF("}");
                        }

                        if (param.scan_info_list[i].wpa_auth || param.scan_info_list[i].wpa_cipher)
                        {
                            PRINTF(NL);
                            PRINTF("WPA= ");
                        }

                        if (param.scan_info_list[i].wpa_auth)
                        {
                            PRINTF(" {");
                            if (param.scan_info_list[i].wpa_auth & SECURITY_AUTH_1X)
                            {
                                PRINTF("802.1X ");
                            }

                            if (param.scan_info_list[i].wpa_auth & SECURITY_AUTH_PSK)
                            {
                                PRINTF("PSK ");
                            }
                            PRINTF("}");
                        }

                        if (param.scan_info_list[i].wpa_cipher)
                        {
                            PRINTF(" {");
                            if (param.scan_info_list[i].wpa_cipher & ATH_CIPHER_TYPE_WEP)
                            {
                                PRINTF("WEP ");
                            }

                            if (param.scan_info_list[i].wpa_cipher & ATH_CIPHER_TYPE_TKIP)
                            {
                                PRINTF("TKIP ");
                            }

                            if (param.scan_info_list[i].wpa_cipher & ATH_CIPHER_TYPE_CCMP)
                            {
                                PRINTF("AES ");
                            }
                            PRINTF("}");
                        }
                        /* it may be old-fashioned WEP this is identified by
                         * absent wpa and rsn ciphers */
                        if (param.scan_info_list[i].rsn_cipher == 0 && param.scan_info_list[i].wpa_cipher == 0)
                        {
                            PRINTF("WEP ");
                        }
                    }
                    else
                    {
                        PRINTF("NONE! ");
                    }
                }
            }
            if (i != param.num_scan_entries - 1)
            {
                PRINTF(NL NL);
            }
            else
            {
                ; // PRINTF(NL "shell> ");
            }
        }
        return A_OK;
    }
    PRINTF("IWCONFIG_ERROR: Invalid Device number" NL);
    return QCA_ERR_INVALID_DEVICE;
}

/*FUNCTION*--------------------------------------------------------------------
 *
 * Function Name   : pushButtonWPS()
 * Returned Value  : None
 * Comments        :Called in response to MAUI pushbutton press
 *
 *END*------------------------------------------------------------------------*/
void pushButtonWPS(void)
{
    qcom_wps_credentials_t wpsCred;
    uint32_t enet_device;

    enet_device = get_active_device();
    PRINTF("Start WPS" NL);
    wps_context.wps_in_progress = 1;
    wps_context.connect_flag    = 1;
    set_power_mode(MAX_PERF_POWER, WPS);

    A_MEMZERO(&wpsCred, sizeof(qcom_wps_credentials_t));
    qcom_wps_set_credentials(enet_device, &wpsCred);
    if (qcom_wps_start(enet_device, wps_context.connect_flag, 1, NULL) != A_OK)
    {
        PRINTF("WPS connect error" NL);
    }
}

typedef struct
{
#define SENTINAL 0xa55a1234
    uint32_t sentinal;
    int8_t ssid[33];
    uint8_t ipaddr[4];
    int8_t passphrase[128];
} FLASH_CONTENTS;

int32_t test_flash(void)
{
    return A_OK;
}
extern void __boot(void);
extern uint8_t bench_quit;

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : SystemReset()
 * Returned Value  : None
 *
 * Comments        : Sample function to depict how to safely reset the host
 *
 *END*-----------------------------------------------------------------*/

void SystemReset(void)
{
    int32_t error;
    ATH_IOCTL_PARAM_STRUCT ath_param;
    uint32_t value, dev_status;
    _qca_handle qcahandle;

    qcahandle = get_active_device_handle();
    /*
     *  If benchrx is already started and reset is given,
     * stop the benchrx thread by making bench_quit = 1 */
    bench_quit = 1;
    A_MDELAY(100);
    if (driver_status(READ_STATUS) != DRIVER_UP)
    {
        PRINTF("driver is down reset issued only in driver up state" NL);
        return;
    }

    while ((error = ENET_mediactl(qcahandle, QCA_MEDIACTL_IS_INITIALIZED, &dev_status)) != A_OK)
    {
        /*Wait for driver to be ready before proceeding*/
    }
    ath_param.cmd_id = ATH_CHIP_STATE;
    ath_param.length = sizeof(value);
    value            = 0;
    ath_param.data   = &value;
    error            = ENET_mediactl(qcahandle, QCA_MEDIACTL_VENDOR_SPECIFIC, &ath_param);
    if (error != A_OK)
    {
        PRINTF("IOCTL error" NL);
    }

    /*Wait for a short period before reset*/
    A_MDELAY(100);
    // This is the ARM CMSIS way of resetting
    NVIC_SystemReset();
}

int32_t ext_scan(void)
{
    int32_t error, i = 0;
    ATH_IOCTL_PARAM_STRUCT inout_param;
    ATH_GET_SCAN param;
    uint8_t temp_ssid[33];

    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }
    inout_param.cmd_id = ATH_START_SCAN_EXT;
    inout_param.data   = NULL;

    error = HANDLE_IOCTL(&inout_param);

    if (A_OK != error)
    {
        return error;
    }

    inout_param.cmd_id = ATH_GET_SCAN_EXT;
    inout_param.data   = (A_VOID *)&param;

    error = HANDLE_IOCTL(&inout_param);

    if (A_OK != error)
    {
        return error;
    }

    if (param.num_entries)
    {
        for (i = 0; i < param.num_entries; i++)
        {
            memcpy(temp_ssid, param.scan_list[i].ssid, param.scan_list[i].ssid_len);

            temp_ssid[param.scan_list[i].ssid_len] = '\0';

            if (param.scan_list[i].ssid_len == 0)
            {
                PRINTF("ssid = SSID Not available" NL);
            }
            else
                PRINTF("ssid = %s" NL, temp_ssid);

            PRINTF("bssid = %.2x:%.2x:%.2x:%.2x:%.2x:%.2x" NL, param.scan_list[i].bssid[0], param.scan_list[i].bssid[1],
                   param.scan_list[i].bssid[2], param.scan_list[i].bssid[3], param.scan_list[i].bssid[4],
                   param.scan_list[i].bssid[5]);
            PRINTF("channel = %d" NL, param.scan_list[i].channel);

            PRINTF("indicator = %d" NL, param.scan_list[i].rssi);
            PRINTF("security = ");

            if (param.scan_list[i].security_enabled)
            {
                if (param.scan_list[i].rsn_auth || param.scan_list[i].rsn_cipher)
                {
                    PRINTF(NL);
                    PRINTF("RSN/WPA2= ");
                }

                if (param.scan_list[i].rsn_auth)
                {
                    PRINTF(" {");
                    if (param.scan_list[i].rsn_auth & SECURITY_AUTH_1X)
                    {
                        PRINTF("802.1X ");
                    }

                    if (param.scan_list[i].rsn_auth & SECURITY_AUTH_PSK)
                    {
                        PRINTF("PSK ");
                    }
                    PRINTF("}");
                }

                if (param.scan_list[i].rsn_cipher)
                {
                    PRINTF(" {");
                    /* AP security can support multiple options hence
                     * we check each one separately. Note rsn == wpa2 */
                    if (param.scan_list[i].rsn_cipher & ATH_CIPHER_TYPE_WEP)
                    {
                        PRINTF("WEP ");
                    }

                    if (param.scan_list[i].rsn_cipher & ATH_CIPHER_TYPE_TKIP)
                    {
                        PRINTF("TKIP ");
                    }

                    if (param.scan_list[i].rsn_cipher & ATH_CIPHER_TYPE_CCMP)
                    {
                        PRINTF("AES ");
                    }
                    PRINTF("}");
                }

                if (param.scan_list[i].wpa_auth || param.scan_list[i].wpa_cipher)
                {
                    PRINTF(NL);
                    PRINTF("WPA= ");
                }

                if (param.scan_list[i].wpa_auth)
                {
                    PRINTF(" {");
                    if (param.scan_list[i].wpa_auth & SECURITY_AUTH_1X)
                    {
                        PRINTF("802.1X ");
                    }

                    if (param.scan_list[i].wpa_auth & SECURITY_AUTH_PSK)
                    {
                        PRINTF("PSK ");
                    }
                    PRINTF("}");
                }

                if (param.scan_list[i].wpa_cipher)
                {
                    PRINTF(" {");
                    if (param.scan_list[i].wpa_cipher & ATH_CIPHER_TYPE_WEP)
                    {
                        PRINTF("WEP ");
                    }

                    if (param.scan_list[i].wpa_cipher & ATH_CIPHER_TYPE_TKIP)
                    {
                        PRINTF("TKIP ");
                    }

                    if (param.scan_list[i].wpa_cipher & ATH_CIPHER_TYPE_CCMP)
                    {
                        PRINTF("AES ");
                    }
                    PRINTF("}");
                }
                /* it may be old-fashioned WEP this is identified by
                 * absent wpa and rsn ciphers */
                if (param.scan_list[i].rsn_cipher == 0 && param.scan_list[i].wpa_cipher == 0)
                {
                    PRINTF("WEP ");
                }
            }
            else
            {
                PRINTF("NONE! ");
            }

            PRINTF(NL);
            PRINTF(NL);
        }
    }
    else
    {
        PRINTF("no scan results found" NL);
    }

    return A_OK;
}

#if ENABLE_STACK_OFFLOAD

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : wmi_ping()
 * Returned Value  : None
 *
 * Comments        : Sample function to depict IPv4 ping functionality
 *                   Only used with On-Chip IP stack
 *END*-----------------------------------------------------------------*/
int wmi_ping(shell_handle_t context, int argc, char **argv)
{
    unsigned long hostaddr = 0;
    int32_t error;
    char ip_str[16];
    uint32_t count = 1, size = 64, interval = 0;
    uint32_t index;

    if ((argc < 2) || (argc % 2 == 1))
    {
        PRINTF("Incomplete parameters" NL);
        PRINTF("Usage: %s <host> -c <count> -s <size> -i <interval>" NL, argv[0]);
        PRINTF("or %s <host>" NL, argv[0]);
        PRINTF("   <host>      = host ip address or name" NL);
        PRINTF("   <count>     = number of pings" NL);
        PRINTF("   <size>      = size of ping packet" NL);
        PRINTF("   <interval>  = interval between packet(in ms)" NL);
        return A_ERROR;
    }

    for (index = 1; index < argc; index++)
    {
        if (argv[index][0] == '-')
        {
            switch (argv[index][1])
            {
                case 'c':
                    index++;
                    count = atoi(argv[index]);
                    break;
                case 'i':
                    index++;
                    interval = atoi(argv[index]);
                    break;
                case 's':
                    index++;
                    size = atoi(argv[index]);
                    if (size > CFG_PACKET_SIZE_MAX_TX)
                    {
                        PRINTF("Error: Invalid Parameter %s" NL, argv[index]);
                        PRINTF("Enter size <= %d" NL, CFG_PACKET_SIZE_MAX_TX);
                        return A_ERROR;
                    }
                    break;
                default:
                    PRINTF("Unknown option: %s" NL, argv[index]);
                    return A_ERROR;
            }
        }
        else /* It must be a IP address or host name */
        {
            error = ath_inet_aton(argv[index], (uint32_t *)&hostaddr);
            if (error != -1)
            {
#if ENABLE_DNS_CLIENT
                if (strlen(argv[index]) > 32)
                {
                    PRINTF("host name cannot be more then 32 Bytes" NL);
                    return A_ERROR;
                }
                if (ip_get_addr_for_host_name(argv[index], (uint32_t *)&hostaddr, NULL, 2) != A_OK)
#endif
                {
                    PRINTF("Invalid IP address" NL);
                    return error;
                }
#if ENABLE_DNS_CLIENT
                else
                {
                    hostaddr = A_CPU2BE32(hostaddr);
                    PRINTF("Ping %s:" NL, inet_ntoa(A_CPU2BE32(hostaddr), ip_str));
                }
#endif
            }
        }
    } /* for loop */

    /*Check if driver is loaded*/
    if (check_driver_state() == A_ERROR)
        return A_ERROR;

    for (index = 0; index < count; index++)
    {
        if (wifi_connected_flag == 0)
        {
            PRINTF("ERROR: No WiFi connection available, please connect to an Access Point" NL);
            break;
        }
        error = qcom_ping(hostaddr, size);

        if (error == 0)
            PRINTF("Ping reply from %s: time<1ms" NL, inet_ntoa(A_CPU2BE32(hostaddr), ip_str));
        else
            PRINTF("Request timed out" NL);

        if (interval)
            A_MDELAY(interval);
    }

    return A_OK;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : wmi_ping6()
 * Returned Value  : None
 *
 * Comments        : Sample function to depict IPv6 ping functionality
 *                   Only used with On-Chip IP stack
 *END*-----------------------------------------------------------------*/
int wmi_ping6(shell_handle_t context, int argc, char **argv)
{
    int32_t error      = -1;
    IP6_ADDR_T ip6addr = {0};
    char ip_str[48];
    uint32_t count, index, size;

    if (argc < 2)
    {
        PRINTF("Incomplete parameters" NL);
        PRINTF("Usage: %s <host> -c <count> -s <size>" NL, argv[0]);
        PRINTF("or %s <host>" NL, argv[0]);
        PRINTF("   <host>   = host ip address or name" NL);
        PRINTF("   <count>  = number of pings" NL);
        PRINTF("   <size>   = size of ping packet" NL);
        return A_ERROR;
    }

    error = Inet6Pton(argv[1], &ip6addr);

    if (error != 0)
    {
#if ENABLE_DNS_CLIENT
        if (strlen(argv[1]) > 32)
        {
            PRINTF("host name cannot be more then 32 Bytes" NL);
            return A_ERROR;
        }
        if (ip_get_addr_for_host_name(argv[1], NULL, (IP6_ADDR_T *)&ip6addr, 3) != A_OK)
#endif
        {
            PRINTF("Invalid IP address" NL);
            return error;
        }
#if ENABLE_DNS_CLIENT
        else
        {
            PRINTF("Ping %s" NL, inet6_ntoa((char *)&ip6addr, ip_str));
        }
#endif
    }

    /*Check if driver is loaded*/
    if (check_driver_state() == A_ERROR)
        return A_ERROR;

    if (argc == 2)
    {
        count = 1;
        size  = 64;
    }
    else if (argc == 6)
    {
        count = atoi(argv[3]);
        size  = atoi(argv[5]);
    }
    else
    {
        PRINTF("Usage: %s <host> -c <count> -s <size>" NL, argv[0]);
        PRINTF("or %s <host>" NL, argv[0]);
        return A_ERROR;
    }

    if (size > CFG_PACKET_SIZE_MAX_TX)
    {
        PRINTF("Error: Invalid Parameter %s" NL, argv[5]);
        PRINTF("Enter size <= %d" NL, CFG_PACKET_SIZE_MAX_TX);
        return A_ERROR;
    }

    for (index = 0; index < count; index++)
    {
        error = qcom_ping6((uint8_t *)&ip6addr, size);

        if (error == 0)
            PRINTF("Ping reply from %s: time<1ms" NL, inet6_ntoa((char *)&ip6addr, ip_str));
        else
            PRINTF("Request timed out" NL);
    }

    return A_OK;
}

#else

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : wmi_ping()
 * Returned Value  : None
 *
 * Comments        : Sample function to depict IPv4 ping functionality
 *                   Only used with RTCS IP stack
 *END*-----------------------------------------------------------------*/
int32_t wmi_ping(int32_t argc, char *argv[])
{
    _ip_address hostaddr;
    char hostname[MAX_HOSTNAMESIZE];
    uint32_t i;
    int32_t time, error;
    uint16_t pingid = 0;
    boolean print_usage, shorthelp = FALSE;
    int32_t return_code = A_OK;
    uint8_t num_dots = 0, param_miss = 0;
    uint8_t index = 0;
    uint32_t num_pings = 4;
    char *ping_send_buffer = NULL;
    PING_PARAM_STRUCT ping_params;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage)
    {
        if (argc > 1)
        {
            memset(hostname, 0, sizeof(hostname));

            index = 1;

            if (strcmp(argv[index], "-c") == 0)
            {
                if (argc != 4)
                {
                    PRINTF("missing parameters" NL);
                    return A_ERROR;
                }
                else
                {
                    index++;
                    num_pings = atoi(argv[index]);
                }

                index++;
            }
            for (i = 0; i < strlen(argv[index]); i++)
            {
                if (argv[index][i] == '.')
                    num_dots++;

                if (num_dots == 3)
                {
                    if (i == strlen(argv[index]) - 1)
                    {
                        param_miss = 1;
                    }
                    else
                    {
                        param_miss = 0;
                        break;
                    }
                }
            }
            if (num_dots != 3 || param_miss != 0)
            {
                PRINTF("Invalid IP address" NL);
                return A_ERROR;
            }

            RTCS_resolve_ip_address(argv[index], &hostaddr, hostname, MAX_HOSTNAMESIZE);

            if (!hostaddr)
            {
#if RTCSCFG_ENABLE_DNS | RTCSCFG_ENABLE_LWDNS
                PRINTF("Unable to resolve host" NL);
#else
                PRINTF("Unable to resolve host - dns service disabled" NL);
#endif
                return_code = A_ERROR;
            }
            else
            {
                PRINTF("Pinging %s [%ld.%ld.%ld.%ld]:" NL, hostname, IPBYTES(hostaddr));
                ping_send_buffer = RTCS_mem_alloc(1024); /* ping6_param[5].value - value of bytes in packet */
                if (ping_send_buffer)
                {
                    /* Set value of bytes of data buffer in according with patterns
                     * ping6_param[4].value - patterns.
                     * ping6_param[5].value - size of buffer.
                     */
                    memset(ping_send_buffer, 'p', 1024);
                }
                else
                {
                    PRINTF("Ping error - can not allocate ping buffer. Size is too big." NL);
                    return A_NO_MEMORY;
                }

                for (i = 0; i < num_pings; i++)
                {
                    time = 5000; /* 5 seconds */
                    _mem_zero(&ping_params, sizeof(ping_params));
                    ping_params.addr.sin_family = AF_INET;
                    ping_params.addr.sin_addr.s_addr = hostaddr;
                    ping_params.timeout = time; /* Maximum waiting time for answer */
                    ping_params.id = ++pingid;
                    ping_params.hop_limit = 255;
                    ping_params.data_buffer = ping_send_buffer;
                    ping_params.data_buffer_size = 1024;
                    error = RTCS_ping(&ping_params);
                    if (error == RTCSERR_ICMP_ECHO_TIMEOUT)
                    {
                        PRINTF("Request timed out" NL);
                    }
                    else if (error)
                    {
                        if (error == 0x1000515)
                        {
                            PRINTF("Error 0x%04lX - illegal ip address" NL, error);
                        }
                        else if (error == 0x1000510)
                        {
                            PRINTF("Error 0x%04lX - no route to host" NL, error);
                        }
                        else
                        {
                            PRINTF("Error 0x%04lX" NL, error);
                        }
                    }
                    else
                    {
                        if (time < 1)
                        {
                            PRINTF("Reply from %ld.%ld.%ld.%ld: time<1ms" NL, IPBYTES(hostaddr));
                        }
                        else
                        {
                            PRINTF("Reply from %ld.%ld.%ld.%ld: time=%ldms" NL, IPBYTES(hostaddr), time);
                        }
                        if ((time < 1000) && (i < num_pings - 1))
                        {
                            RTCS_time_delay(1000 - time);
                        }
                    }
                } /* Endfor */
                _mem_free(ping_send_buffer);
            }
        }
        else
        {
            PRINTF("Error, %s invoked with incorrect number of arguments" NL, argv[0]);
            print_usage = TRUE;
        }
    }

    if (print_usage)
    {
        if (shorthelp)
        {
            PRINTF("%s <host>" NL, argv[0]);
            PRINTF("Usage: %s -c <count> <host>" NL, argv[0]);
        }
        else
        {
            PRINTF("Usage: %s <host>" NL, argv[0]);
            PRINTF("Usage: %s -c <count> <host>" NL, argv[0]);
            PRINTF("   <host>   = host ip address or name" NL);
            PRINTF("   <count>  = number of pings" NL);
        }
    }
    return return_code;
}

/*FUNCTION*-------------------------------------------------------------
 *
 * Function Name   : wmi_ping6()
 * Returned Value  : None
 *
 * Comments        : Sample function to depict IPv6 ping functionality
 *                   Not supported on RTCS
 *END*-----------------------------------------------------------------*/
int32_t wmi_ping6(int32_t argc, char *argv[])
{
    PRINTF("ping6 command not supported ..." NL);
    return A_OK;
}
#endif
