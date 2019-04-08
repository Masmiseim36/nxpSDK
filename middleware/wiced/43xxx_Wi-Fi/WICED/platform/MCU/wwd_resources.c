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
 * Defines WWD resource functions for BCM943340WCD1 platform
 */
#include "resources.h"
#include "wifi_nvram_image.h"
#include "platform/wwd_resource_interface.h"
#include "wiced_resource.h"
#include "wwd_assert.h"
//#include "wiced_result.h"
//#include "platform_dct.h"
//#include "wiced_waf_common.h"
#include "wwd_wiced.h"
/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#if defined( WWD_DYNAMIC_NVRAM )
#define NVRAM_SIZE             dynamic_nvram_size
#define NVRAM_IMAGE_VARIABLE   dynamic_nvram_image
#else
#define NVRAM_SIZE             sizeof( wifi_nvram_image )
#define NVRAM_IMAGE_VARIABLE   wifi_nvram_image
#endif

//#define WWD_DIRECT_RESOURCES

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

/******************************************************
 *               Variable Definitions
 ******************************************************/

extern const resource_hnd_t wifi_firmware_image;

#if defined( WWD_DYNAMIC_NVRAM )
uint32_t dynamic_nvram_size  = sizeof( wifi_nvram_image );
void*    dynamic_nvram_image = &wifi_nvram_image;
#endif

/******************************************************
 *               Function Definitions
 ******************************************************/
resource_result_t resource_read ( const resource_hnd_t* resource, uint32_t offset, uint32_t maxsize, uint32_t* size, void* buffer )
{
	 if ( offset > resource->size )
	    {
	        return RESOURCE_OFFSET_TOO_BIG;
	    }

	    *size = MIN( maxsize, resource->size - offset );

	    if (resource->location == RESOURCE_IN_MEMORY)
	    {
	        memcpy(buffer, &resource->val.mem.data[offset], *size);
	    }
	#ifdef USES_RESOURCES_IN_EXTERNAL_STORAGE
	    else if ( resource->location == RESOURCE_IN_EXTERNAL_STORAGE )
	    {
	        return platform_read_external_resource( resource, offset, maxsize, size, buffer );
	    }
	#endif
	#ifdef USES_RESOURCE_GENERIC_FILESYSTEM
	    else
	    {
	        wiced_file_t file_handle;
	        uint64_t size64;
	        uint64_t maxsize64 =  maxsize;
	        if (WICED_SUCCESS != wiced_filesystem_file_open ( &resource_fs_handle, &file_handle, resource->val.fs.filename, WICED_FILESYSTEM_OPEN_FOR_READ ) )
	        {
	            return RESOURCE_FILE_OPEN_FAIL;
	        }
	        if ( WICED_SUCCESS != wiced_filesystem_file_seek ( &file_handle, (offset + resource->val.fs.offset), SEEK_SET) )
	        {
	            return RESOURCE_FILE_SEEK_FAIL;
	        }
	        if ( WICED_SUCCESS != wiced_filesystem_file_read ( &file_handle, buffer, maxsize64, &size64) )
	        {
	            wiced_filesystem_file_close ( &file_handle );
	            return RESOURCE_FILE_READ_FAIL;
	        }
	        *size = (uint32_t)size64;
	        wiced_filesystem_file_close ( &file_handle );
	    }
	#else
	#ifdef USES_RESOURCE_FILESYSTEM
	    else
	    {
	        wicedfs_file_t file_hnd;

	        if ( 0 != wicedfs_fopen( &resource_fs_handle, &file_hnd, resource->val.fs.filename ) )
	        {
	            return RESOURCE_FILE_OPEN_FAIL;
	        }

	        if ( 0 != wicedfs_fseek( &file_hnd, (long)(offset + resource->val.fs.offset), SEEK_SET ) )
	        {
	            wicedfs_fclose( &file_hnd );
	            return RESOURCE_FILE_SEEK_FAIL;
	        }

	        if ( *size != wicedfs_fread( buffer, 1, *size, &file_hnd ) )
	        {
	            wicedfs_fclose( &file_hnd );
	            return RESOURCE_FILE_READ_FAIL;
	        }

	        wicedfs_fclose( &file_hnd );
	    }
	#endif /* ifdef USES_RESOURCE_FILESYSTEM */
	#endif /* USES_RESOURCE_GENERIC_FILESYSTEM */
	 return RESOURCE_SUCCESS;
}


wwd_result_t host_platform_resource_size( wwd_resource_t resource, uint32_t* size_out )
{
    if ( resource == WWD_RESOURCE_WLAN_FIRMWARE )
    {
#ifdef NO_WIFI_FIRMWARE
        wiced_assert("Request firmware in a no wifi firmware application", 0 == 1);
        *size_out = 0;
#else
#ifdef WIFI_FIRMWARE_IN_MULTI_APP
        wiced_app_t wifi_app;

        *size_out = 0;
        if ( wiced_waf_app_open( DCT_WIFI_FIRMWARE_INDEX, &wifi_app ) != WICED_SUCCESS )
        {
            return ( wwd_result_t ) RESOURCE_UNSUPPORTED;
        }
        wiced_waf_app_get_size( &wifi_app, size_out );
#else
        *size_out = (uint32_t) resource_get_size( &wifi_firmware_image );
#endif
#endif

    }
    else
    {
        *size_out = NVRAM_SIZE;
    }
    return WWD_SUCCESS;
}

#if defined( WWD_DIRECT_RESOURCES )
wwd_result_t host_platform_resource_read_direct( wwd_resource_t resource, const void** ptr_out )
{
    if ( resource == WWD_RESOURCE_WLAN_FIRMWARE )
    {
#ifndef NO_WIFI_FIRMWARE
        *ptr_out = wifi_firmware_image.val.mem.data;
#else
        wiced_assert("Request firmware in a no wifi firmware application", 0 == 1);
        *ptr_out = NULL;
#endif
    }
    else
    {
        *ptr_out = NVRAM_IMAGE_VARIABLE;
    }
    return WWD_SUCCESS;
}
#else /* ! defined( WWD_DIRECT_RESOURCES ) */
wwd_result_t host_platform_resource_read_indirect( wwd_resource_t resource, uint32_t offset, void* buffer, uint32_t buffer_size, uint32_t* size_out )
{
    if ( resource == WWD_RESOURCE_WLAN_FIRMWARE )
    {
#ifdef NO_WIFI_FIRMWARE
        wiced_assert("Request firmware in a no wifi firmware application", 0 == 1);
        return ( wwd_result_t ) RESOURCE_UNSUPPORTED;
#else
#ifdef WIFI_FIRMWARE_IN_MULTI_APP
        wiced_app_t wifi_app;
        if ( wiced_waf_app_open( DCT_WIFI_FIRMWARE_INDEX, &wifi_app ) != WICED_SUCCESS )
        {
            return ( wwd_result_t ) RESOURCE_UNSUPPORTED;
        }
        if ( wiced_waf_app_read_chunk( &wifi_app, offset, buffer, buffer_size ) == WICED_SUCCESS )
        {
            *size_out = buffer_size;
        }
        else
        {
            *size_out = 0;
        }
        return WWD_SUCCESS;
#else
        return resource_read( &wifi_firmware_image, offset, buffer_size, size_out, buffer );
#endif
#endif

    }
    else
    {
        *size_out = MIN( buffer_size, NVRAM_SIZE - offset );
        memcpy( buffer, &NVRAM_IMAGE_VARIABLE[ offset ], *size_out );
        return WWD_SUCCESS;
    }
}
#endif /* if defined( WWD_DIRECT_RESOURCES ) */
