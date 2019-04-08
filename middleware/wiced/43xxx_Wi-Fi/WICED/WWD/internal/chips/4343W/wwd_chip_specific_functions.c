/*
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of 
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
#include "wwd_constants.h"
#include "wwd_wifi.h"
#include "internal/wwd_internal.h"
#include "internal/bus_protocols/wwd_bus_protocol_interface.h"

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
#define KSO_WAIT_MS             ( 1 )
#define MAX_KSO_ATTEMPTS        ( 64 )

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
 *               Function Declarations
 ******************************************************/

#ifndef WWD_DISABLE_SAVE_RESTORE
static wwd_result_t wwd_enable_save_restore( void );
#endif /* WWD_DISABLE_SAVE_RESTORE */
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
    /* Disable remap for SRAM_3. Required only for 4343x */
    VERIFY_RESULT( wwd_bus_write_backplane_value( SOCSRAM_BANKX_INDEX, 4, 0x3 ));
    VERIFY_RESULT( wwd_bus_write_backplane_value( SOCSRAM_BANKX_PDA, 4, 0 ));
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
            VERIFY_RESULT( wwd_bus_read_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_CHIP_CLOCK_CSR, (uint8_t) 1, &csr ) );
            --attempts;
        }
        while ( ( ( csr & SBSDIO_HT_AVAIL ) == 0 ) &&
                ( attempts != 0 ) &&
                ( host_rtos_delay_milliseconds( (uint32_t) 1 ), 1==1 ) );

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
        /* Configure WakeupCtrl register to set HtAvail request bit in chipClockCSR register
         * after the sdiod core is powered on.
         */
        VERIFY_RESULT( wwd_bus_read_register_value( BACKPLANE_FUNCTION, (uint32_t ) SDIO_WAKEUP_CTRL, (uint8_t ) 1, &data ) );
        data |= SBSDIO_WCTRL_WAKE_TILL_HT_AVAIL;
        VERIFY_RESULT( wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t ) SDIO_WAKEUP_CTRL, (uint8_t ) 1, data ) );

        /* Set brcmCardCapability to noCmdDecode mode.
         * It makes sdiod_aos to wakeup host for any activity of cmd line, even though
         * module won't decode cmd or respond
         */
        VERIFY_RESULT( wwd_bus_write_register_value( BUS_FUNCTION, (uint32_t ) SDIOD_CCCR_BRCM_CARDCAP, (uint8_t ) 1, SDIOD_CCCR_BRCM_CARDCAP_CMD_NODEC ) );

        VERIFY_RESULT( wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_CHIP_CLOCK_CSR, (uint8_t) 1, (uint32_t) SBSDIO_FORCE_HT ) );

        /* Enable KeepSdioOn (KSO) bit for normal operation */
        VERIFY_RESULT( wwd_bus_read_register_value( BACKPLANE_FUNCTION, (uint32_t ) SDIO_SLEEP_CSR, (uint8_t ) 1, &data ) );
        if ( ( data & SBSDIO_SLPCSR_KEEP_SDIO_ON ) == 0 )
        {
            data |= SBSDIO_SLPCSR_KEEP_SDIO_ON;
            VERIFY_RESULT( wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t ) SDIO_SLEEP_CSR, (uint8_t ) 1, data ) );
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
#endif /* WWD_DISABLE_SAVE_RESTORE */

static wiced_bool_t wwd_is_fw_sr_capable( void )
{
    uint32_t srctrl = 0;
    /* check if fw initialized sr engine */
    if( wwd_bus_read_backplane_value( (uint32_t) CHIPCOMMON_SR_CONTROL1, (uint8_t) 4, (uint8_t*)&srctrl ) != WWD_SUCCESS)
    {
        return WICED_FALSE;
    }

    if ( srctrl != 0 )
    {
        return WICED_TRUE;
    }
    else
    {
        return WICED_FALSE;
    }
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
    /* 2 Sequential writes to KSO bit are required for SR module to wakeup */
    wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_SLEEP_CSR, (uint8_t) 1, write_value );
    wwd_bus_write_register_value( BACKPLANE_FUNCTION, (uint32_t) SDIO_SLEEP_CSR, (uint8_t) 1, write_value );

    if ( enable == WICED_TRUE )
    {
        /* device WAKEUP through KSO:
         * write bit 0 & read back until
         * both bits 0(kso bit) & 1 (dev on status) are set
         */
        compare_value = SBSDIO_SLPCSR_KEEP_SDIO_ON | SBSDIO_SLPCSR_DEVICE_ON;
        bmask = compare_value;
    }
    else
    {
        /* Put device to sleep, turn off  KSO  */
        compare_value = 0;
        /* Check for bit0 only, bit1(devon status) may not get cleared right away */
        bmask = SBSDIO_SLPCSR_KEEP_SDIO_ON;
    }

    do
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
    } while ( attempts != 0 );

    if ( attempts == 0 )
    {
        return WWD_SDIO_BUS_UP_FAIL;
    }
    else
    {
        return WWD_SUCCESS;
    }
}
