/*
 * wpa_supplicant/hostapd - Build time configuration defines
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This header file can be used to define configuration defines that were
 * originally defined in Makefile. This is mainly meant for IDE use or for
 * systems that do not have suitable 'make' tool. In these cases, it may be
 * easier to have a single place for defining all the needed C pre-processor
 * defines.
 */

#ifndef BUILD_CONFIG_H
#define BUILD_CONFIG_H

#ifndef CONFIG_WPA_SUPP
#define CONFIG_WPA_SUPP 1
#endif

#if defined(USE_RTOS) && defined(SDK_OS_FREE_RTOS)
#define CONFIG_FREERTOS 1
#endif

#define CONFIG_WPA_SUPP_CRYPTO   1
#define CONFIG_WPA_SUPP_AP       1
#define CONFIG_HOSTAPD           1

#define WPA_SUPPLICANT_CLEANUP_INTERVAL 120

#define HOSTAPD_CLEANUP_INTERVAL 120

#if !defined(CONFIG_WPA_SUPP_DEBUG_LEVEL) || CONFIG_WPA_SUPP_DEBUG_LEVEL > 5

#define CONFIG_WPA_SUPP_DEBUG_LEVEL 5

#endif

#endif /* BUILD_CONFIG_H */
