/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 */
#include <string.h>
#include "wwd_constants.h"
#include "wwd_wifi.h"
#include "internal/wwd_internal.h"
#include "internal/bus_protocols/wwd_bus_protocol_interface.h"
#include "internal/wwd_wifi_chip_common.h"
//#include "wiced_utilities.h"

/******************************************************
 *                      Macros
 ******************************************************/

#define VERIFY_RESULT( x )     { wwd_result_t verify_result; verify_result = ( x ); if ( verify_result != WWD_SUCCESS ){ printf("func=%s line:%d\n", __func__, __LINE__); return verify_result; } }

/******************************************************
 *                    Constants
 ******************************************************/
#define PLATFORM_WLAN_RAM_BASE  ( 0x0 )
#define PLATFORM_WLAN_RAM_SIZE  ( CHIP_RAM_SIZE )

#define WLAN_BUS_UP_ATTEMPTS    ( 1000 )
#define HT_AVAIL_WAIT_MS        ( 1 )
#define KSO_WAIT_MS             ( 1 )
#define KSO_WAKE_MS             ( 3 )
#define MAX_KSO_ATTEMPTS        ( 64 )

#define WPRINT_WWD_DEBUG_DS( args ) WPRINT_APP_INFO( args )

/******************************************************
 *                   Enumerations
 ******************************************************/

#ifndef WWD_DISABLE_SAVE_RESTORE
static wwd_result_t wwd_enable_save_restore( void );
#endif /* !WWD_DISABLE_SAVE_RESTORE */
static wiced_bool_t wwd_is_fw_sr_capable( void );
static wwd_result_t wwd_kso_enable( wiced_bool_t enable );

/******************************************************
 *               Variables Definitions
 ******************************************************/

static wiced_bool_t bus_is_up               = WICED_FALSE;
static wiced_bool_t save_restore_enable     = WICED_FALSE;

/******************************************************
 *               Function Definitions
 ******************************************************/
wiced_bool_t wwd_bus_is_up( void )
{
    return bus_is_up;
}

wwd_result_t wwd_wifi_read_wlan_log( char* buffer, uint32_t buffer_size )
{
    wwd_result_t result;
    uint32_t wlan_shared_address;

    /* Backplane access needs HT clock. So, disabling bus sleep */
    WWD_WLAN_KEEP_AWAKE();

    /* FW populates the last word of RAM with wlan_shared_t struct address */
    if ( wwd_is_fw_sr_capable() == WICED_TRUE )
    {
        wlan_shared_address = PLATFORM_WLAN_RAM_BASE + PLATFORM_WLAN_RAM_SIZE - SOCSRAM_SRMEM_SIZE - 4;
    }
    else
    {
        wlan_shared_address = PLATFORM_WLAN_RAM_BASE + PLATFORM_WLAN_RAM_SIZE - 4;
    }

    result = wwd_wifi_read_wlan_log_unsafe( wlan_shared_address, buffer, buffer_size );

    WWD_WLAN_LET_SLEEP();

    return result;
}

wwd_result_t wwd_wifi_set_custom_country_code( const wiced_country_info_t* country_code )
{
    UNUSED_PARAMETER(country_code);
    return WWD_UNSUPPORTED;
}

wwd_result_t wwd_chip_specific_init( void )
{
#ifndef WWD_DISABLE_SAVE_RESTORE
    return wwd_enable_save_restore();
#else
    return WWD_SUCCESS;
#endif
}

wwd_result_t wwd_chip_specific_socsram_init( void )
{
    return WWD_SUCCESS;
}

wwd_result_t wwd_ensure_wlan_bus_is_up( void )
{
    /* Ensure HT clock is up */
    if ( bus_is_up == WICED_TRUE )
    {
        return WWD_SUCCESS;
    }

    if ( save_restore_enable == WICED_FALSE )
    {
        uint8_t csr = 0;
        uint32_t attempts = (uint32_t) WLAN_BUS_UP_ATTEMPTS;

        VERIFY_RESULT( wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_CHIP_CLOCK_CSR, (uint8_t) 1, (uint32_t) SBSDIO_HT_AVAIL_REQ ) );

        do
        {
            VERIFY_RESULT( wwd_bus_read_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_CHIP_CLOCK_CSR, (uint8_t) sizeof( csr ), &csr ) );
            --attempts;
        }
        while ( ( ( csr & SBSDIO_HT_AVAIL ) == 0 ) &&
                ( attempts != 0 ) &&
                ( host_rtos_delay_milliseconds( (uint32_t) HT_AVAIL_WAIT_MS ), 1==1 ) );

        if ( attempts == 0 )
        {
            return WWD_SDIO_BUS_UP_FAIL;
        }
        else
        {
            bus_is_up = WICED_TRUE;
            return WWD_SUCCESS;
        }
    }
    else
    {
        if ( wwd_kso_enable( WICED_TRUE ) == WWD_SUCCESS )
        {
            bus_is_up = WICED_TRUE;
            return WWD_SUCCESS;
        }
        else
        {
            return WWD_SDIO_BUS_UP_FAIL;
        }
    }
}

wwd_result_t wwd_wlan_bus_complete_ds_wake( wiced_bool_t wake_from_firmware, uint32_t wake_event_indication_addr, uint32_t wake_indication_addr, uint32_t sdio_control_addr )
{
    uint32_t val = 0;
    uint16_t val2 = 0;
    uint32_t val1 = 0;
    uint32_t count = 0;
    int32_t timeout = 0;

    WPRINT_WWD_DEBUG_DS(("%s: enter: wake_fr_fw=%d\n", __func__, (int)wake_from_firmware ));

    /* Need to do for all wake causes */
    while ( WWD_SUCCESS != wwd_bus_write_register_value( BUS_FUNCTION, SDIOD_CCCR_IOEN, (uint8_t) 1, SDIO_FUNC_ENABLE_1 ) )
    {
        WPRINT_WWD_DEBUG(("Retry IOEN write\n"));
        WPRINT_APP_INFO(("Retry IOEN write\n"));
        host_rtos_delay_milliseconds(10);
    }

    WWD_WLAN_SHARED_MEMORY_READ( D11SHM_ADDR(wake_event_indication_addr), 2, &val );
    WWD_WLAN_SHARED_MEMORY_READ( D11SHM_ADDR(wake_indication_addr), 2, &val );

    if ( WICED_TRUE == wake_from_firmware )
    {

        WPRINT_WWD_DEBUG_DS(("%s: CCCR written\n", __func__));

        /* Read DS1 CTRL STATUS SHM to Check whether HOST MAIL BOX interrupt is triggered due to TX/RX
              * For RX Cases , Value will be DS1_SLEEP
              * For TX Cases, Value will be MAC ON or RADIO_PHY_ON
              */
        VERIFY_RESULT(wwd_bus_read_backplane_value( D11SHM_ADDR(M_DS1_CTRL_STATUS), (uint8_t) 2, (uint8_t*)&val ));
        if ( val == DS1_SLEEP )
        {
            /* HUDI communication to inform D11 to Process and Exit DS1 not required for D11 Wake cases
             * Simply update PMU Resource Mask and Re-download firmware for this case
             */
            WPRINT_WWD_DEBUG_DS(("%s: D11 wake detected\n", __func__));
            WPRINT_APP_INFO(("%s: D11 wake detected\n", __func__));

            VERIFY_RESULT( wwd_bus_write_backplane_value(PMU_MINRESMASK, 1, DEFAULT_43012_MIN_RES_MASK ) );

            return WWD_SUCCESS;
        }

        VERIFY_RESULT( wwd_bus_write_backplane_value( D11SHM_ADDR(M_DS1_CTRL_SDIO), 2, C_DS1_CTRL_SDIO_DS1_EXIT | C_DS1_CTRL_REQ_VALID ) );
        WPRINT_WWD_DEBUG_DS(("%s: SDIO ctl written\n", __func__));

        VERIFY_RESULT( wwd_bus_read_backplane_value( D11SHM_ADDR(M_DS1_CTRL_SDIO), (uint8_t) 2, (uint8_t*)&val ) );
        WPRINT_WWD_DEBUG_DS(("%s: M_DS1_CTRL_SDIO = [%x]\n", __func__, (int)val));

        VERIFY_RESULT( wwd_bus_read_backplane_value( D11_MACCONTROL_REG, (uint8_t) 2, (uint8_t*)&val1 ) );
        WPRINT_WWD_DEBUG_DS(("%s: before setting wake = [%x]\n", __func__, (int)val1));

        VERIFY_RESULT( wwd_bus_write_backplane_value(D11_MACCONTROL_REG, 2, val1 | D11_MACCONTROL_REG_WAKE ) );
        WPRINT_WWD_DEBUG_DS(("%s: After setting wake= [%x]\n", __func__, (int)val1));

        while (!(val2 & C_DS1_CTRL_PROC_DONE)) {
            wwd_bus_read_backplane_value( D11SHM_ADDR(M_DS1_CTRL_SDIO), (uint8_t) 2, (uint8_t*)&val2 );
            count++;
            if (count == 1000)
            {
                WPRINT_WWD_ERROR(("%s: sdio timed out! Aborting\n", __func__));
                return WWD_TIMEOUT;
            }
        }

        VERIFY_RESULT( wwd_bus_read_backplane_value( D11SHM_ADDR(M_DS1_CTRL_SDIO), (uint8_t) 2, (uint8_t*)&val ) );
        WPRINT_WWD_DEBUG_DS(("%s: M_DS1_CTRL_SDIO after poll = [%x]\n", __func__, (int)val));

        VERIFY_RESULT( wwd_bus_read_backplane_value( D11_MACCONTROL_REG, (uint8_t) 2, (uint8_t*)&val1 ) );
        WPRINT_WWD_DEBUG_DS(("%s: D11_MACCONTROL_REG before Clear = [%x]\n", __func__, (int)val1));

        VERIFY_RESULT( wwd_bus_write_backplane_value(D11_MACCONTROL_REG, 2, val1 & D11_MACCONTROL_CLEAR_WAKE ) );
        WPRINT_WWD_DEBUG_DS(("%s: D11_MACCONTROL_REG after Clear = [%x]\n", __func__, (int)val1));

        VERIFY_RESULT( wwd_bus_write_backplane_value( PMU_MINRESMASK, 1, DEFAULT_43012_MIN_RES_MASK ) );
        VERIFY_RESULT( wwd_bus_read_backplane_value( PMU_MINRESMASK, (uint8_t) 1, (uint8_t*)&val1 ) );

        WPRINT_WWD_DEBUG_DS(("%s: PMU_MINRESMASK = [%x]\n", __func__, (int)val1));

    }
    else
    {
        while ( WWD_SUCCESS != wwd_ensure_wlan_bus_is_up( ) )
        {
            WPRINT_WWD_DEBUG_DS(("Retrying bus is up\n"));
        }

        WPRINT_WWD_DEBUG_DS(("write backplane\n"));

        wwd_bus_read_backplane_value( D11SHM_ADDR(sdio_control_addr), 2, (uint8_t*)&val );

        while ( WWD_SUCCESS != wwd_bus_write_backplane_value( D11SHM_ADDR(sdio_control_addr), 2, val | C_DS1_CTRL_SDIO_DS1_EXIT | C_DS1_CTRL_REQ_VALID ) )
        {
            WPRINT_WWD_DEBUG_DS(("Retrying backplane write; addr=%x\n", (unsigned int)D11SHM_ADDR(sdio_control_addr)));
            host_rtos_delay_milliseconds(100);
        }

        WPRINT_WWD_DEBUG_DS(("wake host\n"));
        WWD_WLAN_WAKE_FROM_HOST( );

        WPRINT_WWD_DEBUG_DS(("poll for timeout\n"));
        /* Poll for PROC_DONE to be set by ucode; timeout after a good amount of time */
        for ( timeout = WWD_WLAN_WAKE_TIMEOUT * 10, val = 0 ; timeout >= 0 ; timeout -= WWD_SHARED_MEMORY_POLLING_DELAY )
        {
            wwd_result_t read_result = wwd_bus_read_backplane_value( D11SHM_ADDR(sdio_control_addr), 2, (uint8_t*)&val );

            if ( WWD_SUCCESS != read_result || !( ( val ) & C_DS1_CTRL_PROC_DONE ) )
            {
                host_rtos_delay_milliseconds( WWD_SHARED_MEMORY_POLLING_DELAY );
            }
            else
            {
                /* success! */
                WPRINT_WWD_DEBUG_DS(("Ucode has posted DONE\n"));
                break;
            }
        }

        WPRINT_WWD_DEBUG_DS(("%s: %d: 0x%08x\n", __func__, __LINE__, (unsigned int)val));

        if( timeout < 0 && !( val & C_DS1_CTRL_PROC_DONE ) ) {
            WPRINT_WWD_ERROR(("%s: Failed to enter DS1 Exit state!\n", __func__));
            return WWD_TIMEOUT;
        }

    }
    WPRINT_WWD_DEBUG_DS(("successfully completed DS wake sequence\n"));
    return WWD_SUCCESS;
}

wwd_result_t wwd_allow_wlan_bus_to_sleep( void )
{
    /* Clear HT clock request */
    if ( bus_is_up == WICED_TRUE )
    {
        bus_is_up = WICED_FALSE;
        if ( save_restore_enable == WICED_FALSE )
        {
            return wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_CHIP_CLOCK_CSR, (uint8_t) 1, 0 );
        }
        else
        {
           return wwd_kso_enable( WICED_FALSE );
        }
    }
    else
    {
        return WWD_SUCCESS;
    }
}

#ifndef WWD_DISABLE_SAVE_RESTORE
static wwd_result_t wwd_enable_save_restore( void )
{
    uint8_t  data;

    if ( wwd_is_fw_sr_capable() == WICED_TRUE )
    {
        /* Configure WakeupCtrl register to set AlpAvail request bit in chipClockCSR register
         * after the sdiod core is powered on.
         */
        VERIFY_RESULT( wwd_bus_read_register_value( BACKPLANE_FUNCTION, (uint32_t ) SDIO_WAKEUP_CTRL, (uint8_t ) sizeof( data ), &data ) );
        data |= SBSDIO_WCTRL_WAKE_TILL_ALP_AVAIL;
        VERIFY_RESULT( wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t ) SDIO_WAKEUP_CTRL, (uint8_t ) sizeof( data ), data ) );

        /* Set brcmCardCapability to noCmdDecode mode.
         * It makes sdiod_aos to wakeup host for any activity of cmd line, even though
         * module won't decode cmd or respond
         */
        VERIFY_RESULT( wwd_bus_write_register_value( BUS_FUNCTION, (uint32_t ) SDIOD_CCCR_BRCM_CARDCAP, (uint8_t ) 1, SDIOD_CCCR_BRCM_CARDCAP_CMD_NODEC ) );

        VERIFY_RESULT( wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_CHIP_CLOCK_CSR, (uint8_t) 1, SBSDIO_HT_AVAIL_REQ ) );

        /* Enable KeepSdioOn (KSO) bit for normal operation */
        VERIFY_RESULT( wwd_bus_read_register_value( BACKPLANE_FUNCTION, (uint32_t ) SDIO_SLEEP_CSR, (uint8_t ) sizeof( data ), &data ) );
        if ( ( data & SBSDIO_SLPCSR_KEEP_SDIO_ON ) == 0 )
        {
            data |= SBSDIO_SLPCSR_KEEP_SDIO_ON;
            VERIFY_RESULT( wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t ) SDIO_SLEEP_CSR, (uint8_t ) sizeof( data ), data ) );
        }

        /* SPI bus can be configured for sleep by default.
         * KSO bit solely controls the wlan chip sleep
         */
        VERIFY_RESULT( wwd_bus_specific_sleep( ) );

        save_restore_enable = WICED_TRUE;
    }
    else
    {
        save_restore_enable = WICED_FALSE;
    }

    return WWD_SUCCESS;
}
#endif /* !WWD_DISABLE_SAVE_RESTORE */

static wiced_bool_t wwd_is_fw_sr_capable( void )
{
    uint32_t retention_ctl = 0;
    wiced_bool_t save_restore_capable = WICED_FALSE;

    /* check if fw initialized sr engine */
    VERIFY_RESULT( wwd_bus_read_backplane_value( (uint32_t) RETENTION_CTL, (uint8_t) sizeof(retention_ctl), (uint8_t*)&retention_ctl ));

    if (( retention_ctl & ( RCTL_MACPHY_DISABLE | RCTL_LOGIC_DISABLE )) == 0 )
    {
        save_restore_capable = WICED_TRUE;
    }

    return save_restore_capable;
}

static wwd_result_t wwd_kso_enable( wiced_bool_t enable )
{
    uint8_t write_value = 0;
    uint8_t read_value = 0;
    uint8_t compare_value;
    uint8_t bmask;
    uint32_t attempts = ( uint32_t ) MAX_KSO_ATTEMPTS;
    wwd_result_t result;

    if ( enable == WICED_TRUE )
    {
        write_value |= SBSDIO_SLPCSR_KEEP_SDIO_ON;
    }

    /* 1st KSO write goes to AOS wake up core if device is asleep  */
    /* Possibly device might not respond to this cmd. So, don't check return value here */
    wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_SLEEP_CSR, (uint8_t) 1, write_value );

    /* In case of 43012 chip, the chip could go down immediately after KSO bit is cleared.
     * So the further reads of KSO register could fail. Thereby just bailing out immediately
     * after clearing KSO bit, to avoid polling of KSO bit.
     */
    if ( enable == WICED_FALSE )
    {
        return WWD_SUCCESS;
    }

    /* 2 Sequential writes to KSO bit are required for SR module to wakeup */
    wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_SLEEP_CSR, (uint8_t) 1, write_value );

    /* device WAKEUP through KSO:
     * write bit 0 & read back until
     * both bits 0(kso bit) & 1 (dev on status) are set
     */
    compare_value = SBSDIO_SLPCSR_KEEP_SDIO_ON | SBSDIO_SLPCSR_DEVICE_ON;
    bmask = compare_value;

    while ( attempts != 0 )
    {
        /* Reliable KSO bit set/clr:
         * Sdiod sleep write access appears to be in sync with PMU 32khz clk
         * just one write attempt may fail,(same is with read ?)
         * in any case, read it back until it matches written value
         */
        result = wwd_bus_read_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_SLEEP_CSR, (uint8_t) 1, &read_value );
        if ( ( ( read_value & bmask ) == compare_value ) && ( result == WWD_SUCCESS ) )
        {
            break;
        }

        host_rtos_delay_milliseconds( (uint32_t) KSO_WAIT_MS );

        wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_SLEEP_CSR, (uint8_t) 1, write_value );
        attempts--;
    }

    if ( attempts == 0 )
    {
        return WWD_SDIO_BUS_UP_FAIL;
    }
    else
    {
        return WWD_SUCCESS;
    }
}

wwd_result_t wwd_unpack_wl_counters_xtlv( wiced_counters_t* data, reinit_rsns_t* reinit_rsns, wl_cnt_wlc_t* wl_cnt_wlc, wl_cnt_ge40mcst_v1_t* wl_cnt_ge40 )
{
    uint8_t* xtlvptr = NULL;
    int32_t length = 0;
    wwd_xtlv_t* xtlvdesc = (wwd_xtlv_t *) &data->data[0];
    length = (int16_t) data->datalen;

    WPRINT_WWD_DEBUG (("data->version:%d data->datalen:%d\n", data->version, data->datalen ));
    do
    {
      switch (xtlvdesc->id)
      {
        case WL_CNT_XTLV_WLC:
             WPRINT_WWD_DEBUG (("WL_CNT_XTLV_WLC length:%d xtlvdesc->id:%d xtlvdesc->len:%d\n", (int)length, xtlvdesc->id, xtlvdesc->len ));
             memcpy(wl_cnt_wlc, xtlvdesc->data, xtlvdesc->len );
             break;

        case WL_CNT_XTLV_WLC_RINIT_RSN:
             WPRINT_WWD_DEBUG ((" WL_CNT_XTLV_WLC_RINIT_RSN length:%d xtlvdesc->id:%d xtlvdesc->len:%d\n", (int)length, xtlvdesc->id, xtlvdesc->len ));
             memcpy(reinit_rsns, xtlvdesc->data, xtlvdesc->len );
             break;

        case WL_CNT_XTLV_CNTV_LE10_UCODE:
             WPRINT_WWD_DEBUG ((" WL_CNT_XTLV_CNTV_LE10_UCODE length:%d xtlvdesc->id:%d xtlvdesc->len:%d\n", (int) length, xtlvdesc->id, xtlvdesc->len ));
             break;

        case WL_CNT_XTLV_LT40_UCODE_V1:
             WPRINT_WWD_DEBUG ((" WL_CNT_XTLV_LT40_UCODE_V1 length:%d xtlvdesc->id:%d xtlvdesc->len:%d\n", (int)length, xtlvdesc->id, xtlvdesc->len ));
             break;

        case WL_CNT_XTLV_GE40_UCODE_V1:
             WPRINT_WWD_DEBUG ((" WL_CNT_XTLV_GE40_UCODE_V1 length:%d xtlvdesc->id:%d xtlvdesc->len:%d\n", (int)length, xtlvdesc->id, xtlvdesc->len ));
             memcpy(wl_cnt_ge40, xtlvdesc->data, xtlvdesc->len );
             break;

        case WL_CNT_XTLV_GE64_UCODEX_V1:
             WPRINT_WWD_DEBUG ((" WL_CNT_XTLV_GE64_UCODEX_V1 length:%d xtlvdesc->id:%d xtlvdesc->len:%d\n", (int)length, xtlvdesc->id, xtlvdesc->len ));
             break;

        default:
             WPRINT_WWD_DEBUG ((" DEFAULT length:%d xtlvdesc->id:%d xtlvdesc->len:%d\n", (int)length, xtlvdesc->id, xtlvdesc->len ));
             break;
        }

        length -= (int32_t)((offsetof(wwd_xtlv_t, data )) + xtlvdesc->len);
        xtlvptr = (uint8_t*)((uint8_t*)xtlvdesc + (offsetof(wwd_xtlv_t, data )) + xtlvdesc->len);
        xtlvdesc = (wwd_xtlv_t*)xtlvptr;

    } while( length >= (int32_t)((offsetof(wwd_xtlv_t, data )) + xtlvdesc->len ));

    return WWD_SUCCESS;
}
