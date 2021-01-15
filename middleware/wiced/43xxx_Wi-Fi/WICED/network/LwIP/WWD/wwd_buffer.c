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

#include "network/wwd_buffer_interface.h"
#include "platform/wwd_bus_interface.h"
#include "lwip/netbuf.h"
#include "lwip/memp.h"
#include <string.h> /* for NULL */
#include "wwd_assert.h"
#include "wwd_wifi.h"
#include "wwd_network_constants.h"
#include "lwip/pbuf.h"
#include "wwd_buffer.h"
#include "wwd_wiced.h"

void memp_free_notify( unsigned int type );

wwd_result_t wwd_buffer_init( /*@null@*/ /*@unused@*/ void * native_arg )
{
    UNUSED_PARAMETER( native_arg );

    return WWD_SUCCESS;
}

wwd_result_t host_buffer_check_leaked( void )
{
    wiced_assert( "pbuf TX pool Buffer leakage", memp_in_use( MEMP_PBUF_POOL_TX ) == 0 );
    wiced_assert( "pbuf RX pool Buffer leakage", memp_in_use( MEMP_PBUF_POOL_RX ) == 0 );
    wiced_assert( "pbuf ref/rom Buffer leakage", memp_in_use( MEMP_PBUF ) == 0 );
    return WWD_SUCCESS;
}

wwd_result_t internal_host_buffer_get( wiced_buffer_t * buffer, wwd_buffer_dir_t direction, unsigned short size, unsigned long timeout_ms )
{
    UNUSED_PARAMETER( direction );

    wiced_assert("Error: Invalid buffer size\n", size != 0);

    *buffer = NULL;

    if ( size > (unsigned short) WICED_LINK_MTU )
    {
        WPRINT_WWD_ERROR(("Attempt to allocate a buffer larger than the MTU of the link\n"));
        /*@-compdef@*/ /* Lint: buffer is not allocated in error case */
        return WWD_BUFFER_UNAVAILABLE_PERMANENT;
        /*@+compdef@*/
    }

    do
    {
        *buffer = pbuf_alloc( PBUF_RAW, size, ( direction == WWD_NETWORK_RX ) ? PBUF_POOL_RX : PBUF_POOL_TX );
        if ( *buffer != NULL )
        {
            break;
        }

        host_rtos_delay_milliseconds( 1 );

    } while ( timeout_ms-- > 0);

    if ( *buffer == NULL )
    {
#if 0
        WPRINT_NETWORK_DEBUG(("Failed to allocate packet buffer\n"));
#endif /* if 0 */
        WWD_STATS_CONDITIONAL_INCREMENT_VARIABLE( direction == WWD_NETWORK_TX,  tx_no_mem );
        WWD_STATS_CONDITIONAL_INCREMENT_VARIABLE( direction == WWD_NETWORK_RX,  rx_no_mem );
        /*@-compdef@*/ /* Lint: buffer is not allocated in this case */
        return WWD_BUFFER_UNAVAILABLE_TEMPORARY;
        /*@+compdef@*/
    }

    /*@-compdef@*/ /* Lint does not realise allocation has occurred */
    return WWD_SUCCESS;
    /*@+compdef@*/
}

wwd_result_t host_buffer_get( /*@special@*/ /*@out@*/ wiced_buffer_t* buffer, wwd_buffer_dir_t direction, unsigned short size, wiced_bool_t wait ) /*@allocates *buffer@*/  /*@defines **buffer@*/
{
    unsigned long wait_option = ( wait == WICED_TRUE ) ? (unsigned long) WICED_NEVER_TIMEOUT : 0;
    return internal_host_buffer_get(buffer, direction, size, wait_option);
}

void host_buffer_release( /*@only@*/ wiced_buffer_t buffer, wwd_buffer_dir_t direction )
{
    UNUSED_PARAMETER( direction );

    wiced_assert("Error: Invalid buffer\n", buffer != NULL);
    (void) pbuf_free( buffer ); /* Ignore returned number of freed segments since TCP packets will still be referenced by LWIP after relase by WICED */
}

/*@exposed@*/ uint8_t* host_buffer_get_current_piece_data_pointer( /*@temp@*/ wiced_buffer_t buffer )
{
    wiced_assert("Error: Invalid buffer\n", buffer != NULL);
    return (uint8_t*) buffer->payload;
}

uint16_t host_buffer_get_current_piece_size( /*@temp@*/ wiced_buffer_t buffer )
{
    wiced_assert("Error: Invalid buffer\n", buffer != NULL);
    return (uint16_t) buffer->len;
}

/*@exposed@*/ /*@dependent@*/ /*@null@*/ wiced_buffer_t host_buffer_get_next_piece( /*@dependent@*/ wiced_buffer_t buffer )
{
    wiced_assert("Error: Invalid buffer\n", buffer != NULL);
    return buffer->next;
}

wwd_result_t host_buffer_add_remove_at_front( wiced_buffer_t * buffer, int32_t add_remove_amount )
{
    wiced_assert("Error: Invalid buffer\n", buffer != NULL);
    if ( (u8_t) 0 != pbuf_header( *buffer, (s16_t) ( -add_remove_amount ) ) )
    {
        WPRINT_NETWORK_DEBUG(("Failed to move pointer - usually because not enough space at front of buffer\n"));
        return WWD_BUFFER_POINTER_MOVE_ERROR;
    }

    return WWD_SUCCESS;
}

wwd_result_t host_buffer_set_size( /*@temp@*/ wiced_buffer_t buffer, unsigned short size )
{
     if ( size > (unsigned short) WICED_LINK_MTU )
        {
            WPRINT_NETWORK_ERROR(("Attempt to set a length larger than the MTU of the link\n"));
            /*@-unreachable@*/ /* Reachable after hitting assert */
            return WWD_BUFFER_SIZE_SET_ERROR;
            /*@+unreachable@*/
        }
          buffer->tot_len = size;
         buffer->len = size;

         return WWD_SUCCESS;
}

wiced_bool_t host_buffer_pool_is_full( wwd_buffer_dir_t direction )
{
    wiced_bool_t full = WICED_TRUE;
    uint16_t one_byte = 1;
    wiced_buffer_t buffer;

    /* not full if we can pull even one byte long out of the pool */
    if ( WWD_SUCCESS == host_buffer_get( &buffer, direction, one_byte, WICED_NO_WAIT ) )
    {
        full = WICED_FALSE;
        host_buffer_release( buffer, direction );
    }

    return full;
}

void memp_free_notify( unsigned int type )
{
    if ( type == MEMP_PBUF_POOL_TX )
    {
        host_platform_bus_buffer_freed( WWD_NETWORK_TX );
    }
    else if ( type == MEMP_PBUF_POOL_RX )
    {
        host_platform_bus_buffer_freed( WWD_NETWORK_RX );
    }
}
