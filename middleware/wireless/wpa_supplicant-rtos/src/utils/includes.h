/*
 * wpa_supplicant/hostapd - Default include files
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This header file is included into all C files so that commonly used header
 * files can be selected with OS specific ifdef blocks in one place instead of
 * having to have OS/C library specific selection in many files.
 */

#ifndef INCLUDES_H
#define INCLUDES_H

/* Include possible build time configuration before including anything else */
#include "build_config.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#ifndef _WIN32_WCE
//#include <signal.h>
#ifdef CONFIG_ZEPHYR
#include <sys/types.h>
#include <errno.h>
#endif
#endif /* _WIN32_WCE */
#include <ctype.h>

#if !(defined(MSC_VER) || defined(CONFIG_ZEPHYR) || defined(CONFIG_FREERTOS))
#include <unistd.h>
#endif /* _MSC_VER */

#if !(defined(CONFIG_NATIVE_WINDOWS) || defined(CONFIG_ZEPHYR) || defined(CONFIG_FREERTOS))
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef __vxworks
#include <sys/uio.h>
#include <sys/time.h>
#endif /* __vxworks */
#endif /* CONFIG_NATIVE_WINDOWS */

#if defined(CONFIG_ZEPHYR)
#if defined(CONFIG_POSIX_API)
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/sys/select.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/unistd.h>
#else /* defined(CONFIG_POSIX_API) */
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#endif /* defined(CONFIG_POSIX_API) */
#include <zephyr/shell/shell.h>
#endif /* defined(CONFIG_ZEPHYR) */

#include <wm_net.h>

#if defined(CONFIG_FREERTOS)

#if defined(CONFIG_POSIX_API)
#include <posix/arpa/inet.h>
//#include <posix/sys/select.h>
#include <posix/sys/socket.h>
//#include <posix/unistd.h>
#else /* defined(CONFIG_POSIX_API) */
#include <lwip/inet.h>
#include <lwip/sockets.h>
#endif /* defined(CONFIG_POSIX_API) */
//#include <zephyr/shell/shell.h>
#include <fsl_debug_console.h>
#endif /* defined(CONFIG_FREERTOS) */

#ifdef CONFIG_WPA_SUPP_CRYPTO

#include <mbedtls/version.h>

#ifndef MBEDTLS_NIST_KW_C
#define CONFIG_INTERNAL_AES_WRAP
#define CONFIG_INTERNAL_AES_UNWRAP
#endif

#ifndef MBEDTLS_CMAC_C
#define CONFIG_INTERNAL_OMAC1_AES
#endif

#ifndef MBEDTLS_ARC4_C
#define CONFIG_INTERNAL_RC4
#endif

#ifndef MBEDTLS_MD4_C
#define CONFIG_INTERNAL_MD4
#endif

#endif

#ifdef CONFIG_FIPS
#define CONFIG_INTERNAL_MD4
#define CONFIG_INTERNAL_MD5
#define CONFIG_INTERNAL_AES_WRAP
#define CONFIG_INTERNAL_AES_UNWRAP
#define CONFIG_INTERNAL_DES
#endif

#define CONFIG_SHA256
#ifdef CONFIG_WPA_SUPP_CRYPTO
#define CONFIG_SHA384
#define CONFIG_SHA512
#define CONFIG_DES
#define CONFIG_AES
#endif

#ifndef CONFIG_AES
#define CONFIG_INTERNAL_AES
#endif

#ifdef CONFIG_11R
#define CONFIG_IEEE80211R
#ifdef CONFIG_WPA_SUPP_AP
#define CONFIG_IEEE80211R_AP
#endif
#endif

#define CONFIG_SME
// #define CONFIG_NO_TKIP
#define CONFIG_NO_CONFIG_WRITE
#define CONFIG_CTRL_IFACE
#define CONFIG_CTRL_IFACE_UDP
#define CONFIG_NO_RANDOM_POOL
#define CONFIG_MBO
#define CONFIG_WNM
#define IEEE8021X_EAPOL

#ifdef CONFIG_WPA_SUPP_AP
#define CONFIG_AP
#define CONFIG_NO_RADIUS
#define CONFIG_NO_VLAN
#define CONFIG_NO_ACCOUNTING
#define NEED_AP_MLME
#define HOSTAPD
#define CONFIG_IEEE80211AX
#define CONFIG_ACS
#define NEED_RSN_AUTHENTICATOR
#define CONFIG_IEEE80211AC
#define CONFIG_WNM_AP
#define CONFIG_ETH_P_OUI
#endif

#ifndef CONFIG_WPA_SUPP_CRYPTO
#define CONFIG_CRYPTO_INTERNAL
#define CONFIG_INTERNAL_OMAC1_AES
#define CONFIG_INTERNAL_MD4
#define CONFIG_INTERNAL_MD5
#define CONFIG_INTERNAL_AES_WRAP
#define CONFIG_INTERNAL_AES_UNWRAP
#define CONFIG_INTERNAL_DES
#define CONFIG_INTERNAL_AES
#define CONFIG_INTERNAL_RC4
#define CONFIG_INTERNAL_SHA1
#define CONFIG_INTERNAL_SHA256

//#define CONFIG_TLS_INTERNAL_CLIENT
//#define CONFIG_TLS_INTERNAL_SERVER
#define CONFIG_SHA256
//#define CONFIG_SHA384
//#define CONFIG_HMAC_SHA384_KDF
//#define CONFIG_INTERNAL_SHA384
#endif

#ifdef CONFIG_WPA_SUPP_WPA3
#define CONFIG_SAE
//#define CONFIG_SAE_PK
#define CONFIG_ECC
#endif

#ifdef CONFIG_WPA_SUPP_WPS
#define CONFIG_WPS
#define EAP_WSC
#define IEEE8021X_EAPOL
#define NEED_DH_GROUPS
#define NEED_BASE64
#define NEED_AES_CBC
#define EAP_SERVER_WSC
#endif

#ifdef CONFIG_WPA_SUPP_P2P
#define CONFIG_P2P
#define CONFIG_GAS
#define CONFIG_OFFCHANNEL
#endif

#ifdef CONFIG_WPA_SUPP_DPP
#define CONFIG_DPP
#ifdef CONFIG_WPA_SUPP_DPP2
#define CONFIG_DPP2
#endif
#ifdef CONFIG_WPA_SUPP_DPP3
#define CONFIG_DPP3
#endif
#define CONFIG_GAS
#define CONFIG_OFFCHANNEL
#define CONFIG_GAS_SERVER
#ifdef CONFIG_WPA_SUPP_AP
#define CONFIG_INTERWORKING
#endif /* CONFIG_WPA_SUPP_AP */
#endif

#if defined(CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE) || defined(CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE)

#define CONFIG_SUITEB
#define CONFIG_SUITEB192

#define CONFIG_TLSV11
#define CONFIG_TLSV12
#define TLS_FUNCS
#define EAP_TLS_FUNCS
#define MS_FUNCS
#define NEED_DES
#define NEED_MD4
#define NEED_TLS_PRF
#define NEED_TLS_PRF_SHA256
#define NEED_SHA1
#define CHAP
#define NEED_AES_CBC
#define NEED_AES
#define NEED_AES_ENCBLOCK
#define NEED_AES_EAX
#define NEED_FIPS186_2_PRF
#define NEED_T_PRF
#define NEED_SHA384
#define NEED_TLS_PRF_SHA256
#define NEED_TLS_PRF_SHA384
#define NEED_DRAGONFLY
#define NEED_CIPHER
//#define PCSC_FUNCS
#define NEED_ASN1
#define NEED_AES_EAX
#define NEED_AES_SIV
#define NEED_AES_CTR
#define CONFIG_SIM_SIMULATOR
#define CONFIG_USIM_SIMULATOR
#define NEED_MILENAGE
#define NEED_AES_ENC
#define IEEE8021X_EAPOL

#ifdef CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE
#ifdef CONFIG_EAP_TLS
#define EAP_TLS
#endif
#ifdef CONFIG_EAP_PEAP
#define EAP_PEAP
#endif
#ifdef CONFIG_EAP_TTLS
#define EAP_TTLS
#endif
//#define EAP_MD5
//#define EAP_LEAP
//#define EAP_PSK
#if !defined(CONFIG_FIPS)
#ifdef CONFIG_EAP_FAST
#define EAP_FAST
#endif
#endif
//#define EAP_PAX
//#define EAP_SAKE
//#define EAP_GPSK
//#define EAP_PWD
//#define EAP_EKE
//#define EAP_IKEv2
#ifdef CONFIG_EAP_SIM
#define EAP_SIM
#define CONFIG_EAP_SIM_COMMON
#endif
#ifdef CONFIG_EAP_AKA_PRIME
#define EAP_AKA_PRIME
#define CONFIG_EAP_AKA
#endif
#ifdef CONFIG_EAP_AKA
#define EAP_AKA
#define CONFIG_EAP_SIM_COMMON
#endif
#ifdef CONFIG_EAP_MSCHAPV2
#define EAP_MSCHAPv2
#endif
#ifdef CONFIG_EAP_GTC
#define EAP_GTC
#endif
#endif

#ifdef CONFIG_WPA_SUPP_AP
#ifdef CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE
#define RADIUS_SERVER
#define EAP_SERVER
#define EAP_SERVER_IDENTITY
#ifdef CONFIG_EAP_TLS
#define EAP_SERVER_TLS
#endif
#ifdef CONFIG_EAP_PEAP
#define EAP_SERVER_PEAP
#endif
#ifdef CONFIG_EAP_TTLS
#define EAP_SERVER_TTLS
#endif
#define EAP_SIM_DB
#ifdef CONFIG_EAP_SIM
#define EAP_SERVER_SIM
#endif
#ifdef CONFIG_EAP_AKA_PRIME
#define EAP_SERVER_AKA_PRIME
#define CONFIG_EAP_AKA
#endif
#ifdef CONFIG_EAP_AKA
#define EAP_SERVER_AKA
#define CONFIG_EAP_SIM_COMMON
#endif
#if !defined(CONFIG_FIPS)
#ifdef CONFIG_EAP_FAST
#define EAP_SERVER_FAST
#endif
#endif
#ifdef CONFIG_EAP_MSCHAPV2
#define EAP_SERVER_MSCHAPV2
#endif
#ifdef CONFIG_EAP_GTC
#define EAP_SERVER_GTC
#endif
#endif
#endif

#endif /* CONFIG_WPA_SUPP_CRYPTO_ENTERPRISE || CONFIG_WPA_SUPP_CRYPTO_AP_ENTERPRISE */

#endif /* INCLUDES_H */
