// Copyright (C) Firmwave Ltd., All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/xio.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/tlsio_mbedtls.h"
#include "board.h"

int platform_init(void)
{
    return 0;
}

const IO_INTERFACE_DESCRIPTION* platform_get_default_tlsio(void)
{
    return tlsio_mbedtls_get_interface_description();
}

STRING_HANDLE platform_get_platform_info(PLATFORM_INFO_OPTION options)
{
    // No applicable options, so ignoring parameter
    (void)options;
    // Expected format: "(<runtime name>; <operating system name>; <platform>)"
    return STRING_construct("(native; FreeRTOS; MSDK)");
}

void platform_deinit(void)
{
    //TRACE_INFO("Deinitializing platform \r\n");
    while(1) {};
}

