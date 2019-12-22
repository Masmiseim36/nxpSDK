/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 */
#include "wwd_constants.h"
#include "wwd_wifi.h"
#include "internal/wwd_internal.h"
#include "internal/bus_protocols/wwd_bus_protocol_interface.h"
#include "internal/wwd_wifi_chip_common.h"

/******************************************************
 *                      Macros
 ******************************************************/

#define VERIFY_RESULT( x )     { wwd_result_t verify_result; verify_result = ( x ); if ( verify_result != WWD_SUCCESS ) return verify_result; }


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

    wlan_shared_address += ATCM_RAM_BASE_ADDRESS;
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
    uint32_t val1 = 0;
    uint32_t count = 0;

    UNUSED_PARAMETER( wake_from_firmware );
    UNUSED_PARAMETER( wake_event_indication_addr );
    UNUSED_PARAMETER( wake_indication_addr );
    UNUSED_PARAMETER( sdio_control_addr );

    WPRINT_WWD_DEBUG(("%s: enter\n", __FUNCTION__));

    VERIFY_RESULT( wwd_bus_write_register_value( BUS_FUNCTION, SDIOD_CCCR_IOEN, (uint8_t) 1, SDIO_FUNC_ENABLE_1 ) );
    WPRINT_WWD_DEBUG(("%s: CCCR written\n", __FUNCTION__));
    /* Read DS1 CTRL STATUS SHM to Check whether HOST MAIL BOX interrupt is triggered due to TX/RX
      * For RX Cases , Value will be DS1_SLEEP
      * For TX Cases, Value will be MAC ON or RADIO_PHY_ON
      */
    VERIFY_RESULT(wwd_bus_read_backplane_value( D11SHM_ADDR(M_DS1_CTRL_STATUS), (uint8_t) 2, (uint8_t*)&val ));
    if (val == DS1_SLEEP)
    {
       /* HUDI communication to inform D11 to Process and Exit DS1 not required for D11 Wake cases
             * Simply update PMU Resource Mask and Re-download firmware for this case
            */
       VERIFY_RESULT( wwd_bus_write_backplane_value(PMU_MINRESMASK, 1, DEFAULT_43012_MIN_RES_MASK ) );
       return WWD_SUCCESS;
    }
    VERIFY_RESULT( wwd_bus_write_backplane_value( D11SHM_ADDR(M_DS1_CTRL_SDIO), 2, C_DS1_CTRL_SDIO_DS1_EXIT | C_DS1_CTRL_REQ_VALID ) );
    WPRINT_WWD_DEBUG(("%s: SDIO ctl written\n", __FUNCTION__));

    VERIFY_RESULT( wwd_bus_read_backplane_value( D11SHM_ADDR(M_DS1_CTRL_SDIO), (uint8_t) 2, (uint8_t*)&val ) );
    WPRINT_WWD_DEBUG(("%s: M_DS1_CTRL_SDIO = [%x]\n", __FUNCTION__, (int)val));

    VERIFY_RESULT( wwd_bus_read_backplane_value( D11_MACCONTROL_REG, (uint8_t) 2, (uint8_t*)&val1 ) );
    WPRINT_WWD_DEBUG(("%s: before setting wake = [%x]\n", __FUNCTION__, (int)val1));
    VERIFY_RESULT( wwd_bus_write_backplane_value(D11_MACCONTROL_REG, 2, val1 | D11_MACCONTROL_REG_WAKE ) );
    WPRINT_WWD_DEBUG(("%s: After setting wake= [%x]\n", __FUNCTION__, (int)val1));

    while (!(val & C_DS1_CTRL_PROC_DONE)) {
        wwd_bus_read_backplane_value( D11SHM_ADDR(M_DS1_CTRL_SDIO), (uint8_t) 2, (uint8_t*)&val );
        count++;
        if (count == 1000)
        {
            WPRINT_WWD_ERROR(("%s: sdio timed out! Aborting\n", __FUNCTION__));
            return WWD_TIMEOUT;
        }
    }

    VERIFY_RESULT( wwd_bus_read_backplane_value( D11SHM_ADDR(M_DS1_CTRL_SDIO), (uint8_t) 2, (uint8_t*)&val ) );
    WPRINT_WWD_DEBUG(("%s: M_DS1_CTRL_SDIO after poll = [%x]\n", __FUNCTION__, (int)val));

    VERIFY_RESULT( wwd_bus_read_backplane_value( D11_MACCONTROL_REG, (uint8_t) 2, (uint8_t*)&val1 ) );
    WPRINT_WWD_DEBUG(("%s: D11_MACCONTROL_REG before Clear = [%x]\n", __FUNCTION__, (int)val1));

    VERIFY_RESULT( wwd_bus_write_backplane_value(D11_MACCONTROL_REG, 2, val1 & D11_MACCONTROL_CLEAR_WAKE ) );
    WPRINT_WWD_DEBUG(("%s: D11_MACCONTROL_REG after Clear = [%x]\n", __FUNCTION__, (int)val1));

    VERIFY_RESULT( wwd_bus_write_backplane_value(PMU_MINRESMASK, 1, DEFAULT_43012_MIN_RES_MASK ) );
    VERIFY_RESULT( wwd_bus_read_backplane_value( PMU_MINRESMASK, (uint8_t) 1, (uint8_t*)&val1 ) );
    WPRINT_WWD_DEBUG(("%s: PMU_MINRESMASK = [%x]\n", __FUNCTION__, (int)val1));
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
    uint32_t     retention_ctl = 0;
    wiced_bool_t save_restore_capable = WICED_FALSE;

    /* check if fw initialized sr engine */
    VERIFY_RESULT( wwd_bus_read_backplane_value( (uint32_t) RETENTION_CTL, (uint8_t)sizeof(retention_ctl), (uint8_t*)&retention_ctl ));
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
