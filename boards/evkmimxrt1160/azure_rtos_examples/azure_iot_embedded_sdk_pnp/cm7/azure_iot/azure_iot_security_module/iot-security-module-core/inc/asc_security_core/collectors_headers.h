/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

#ifndef _COLLECTORS_HEADERS_H
#define _COLLECTORS_HEADERS_H

#include "asc_security_core/configuration.h"

#ifdef COLLECTOR_SYSTEM_INFORMATION_ENABLED
#include "asc_security_core/collectors/system_information.h"
#endif

#ifdef COLLECTOR_LISTENING_PORTS_ENABLED
#include "asc_security_core/collectors/listening_ports.h"
#endif

#ifdef COLLECTOR_NETWORK_ACTIVITY_ENABLED
#include "asc_security_core/collectors/network_activity.h"
#endif

#ifdef COLLECTOR_HEARTBEAT_ENABLED
#include "asc_security_core/collectors/heartbeat.h"
#endif

#ifdef COLLECTOR_BASELINE_ENABLED
#include "asc_security_core/collectors/baseline.h"
#endif

#endif /* _COLLECTORS_HEADERS_H */

