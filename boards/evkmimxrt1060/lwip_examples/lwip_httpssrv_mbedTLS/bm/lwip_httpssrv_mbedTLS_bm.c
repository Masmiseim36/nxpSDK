/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lwip/opt.h"

#if LWIP_TCP

#include "lwip/apps/httpd.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "netif/ethernet.h"
#include "ethernetif.h"

#include "board.h"

#include "ksdk_mbedtls.h"
#include "httpd_mbedtls.h"

#include "mbedtls/entropy.h"

#include "mbedtls/ctr_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/debug.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 0
#define configIP_ADDR3 102

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 0
#define configGW_ADDR3 100

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

#define TRNG0 TRNG

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

const char *pers = "ssl_server";
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_ssl_context ssl;
mbedtls_ssl_config conf;
mbedtls_x509_crt srvcert;
mbedtls_pk_context pkey;
#if defined(MBEDTLS_SSL_CACHE_C)
mbedtls_ssl_cache_context cache;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}


static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void)level);

    PRINTF("\r\n%s, at line %d in file %s\n", str, line, file);
}

/*!
 * @brief Interrupt service for SysTick timer.
 */
void SysTick_Handler(void)
{
    time_isr();
}

/*!
 * @brief Main function
 */
int main(void)
{
    struct netif fsl_netif0;
    int ret;
    ip4_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
    ethernetif_config_t fsl_enet_config0 = {
        .phyAddress = EXAMPLE_PHY_ADDRESS, .clockName = EXAMPLE_CLOCK_NAME, .macAddress = configMAC_ADDR,
    };

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    /* Data cache must be temporarily disabled to be able to use sdram */
    SCB_DisableDCache();

    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    delay();
    GPIO_WritePinOutput(GPIO1, 9, 1);

    time_init();

    CRYPTO_InitHardware();

    IP4_ADDR(&fsl_netif0_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&fsl_netif0_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&fsl_netif0_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    lwip_init();

    netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, &fsl_enet_config0, ethernetif0_init,
              ethernet_input);
    netif_set_default(&fsl_netif0);
    netif_set_up(&fsl_netif0);

    /*
     * mbedTLS - setup
     */

    mbedtls_ssl_init(&ssl);
    mbedtls_ssl_config_init(&conf);
#if defined(MBEDTLS_SSL_CACHE_C)
    mbedtls_ssl_cache_init(&cache);
#endif
    mbedtls_x509_crt_init(&srvcert);
    mbedtls_pk_init(&pkey);
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

#if defined(MBEDTLS_DEBUG_C)
    mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif
    /*
     * 1. Load the certificates and private RSA key
     */

    /*
     * This demonstration program uses embedded test certificates.
     * Instead, you may want to use mbedtls_x509_crt_parse_file() to read the
     * server and CA certificates, as well as mbedtls_pk_parse_keyfile().
     */
    ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)mbedtls_test_srv_crt, mbedtls_test_srv_crt_len);
    if (ret != 0)
    {
        PRINTF(" failed\r\n  !  mbedtls_x509_crt_parse returned %d\r\n\r\n", ret);
        goto exit;
    }

#if (!defined(MBEDTLS_ECDSA_C) || defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED))
    ret = mbedtls_x509_crt_parse(&srvcert, (const unsigned char *)mbedtls_test_cas_pem, mbedtls_test_cas_pem_len);
    if (ret != 0)
    {
        PRINTF(" failed\r\n  !  mbedtls_x509_crt_parse returned %d\r\n\r\n", ret);
        goto exit;
    }
#endif

    ret = mbedtls_pk_parse_key(&pkey, (const unsigned char *)mbedtls_test_srv_key, mbedtls_test_srv_key_len, NULL, 0);
    if (ret != 0)
    {
        PRINTF(" failed\r\n  !  mbedtls_pk_parse_key returned %d\r\n\r\n", ret);
        goto exit;
    }

    /*
     * 2. Seeding the random number generator
     */
    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers,
                                     strlen(pers))) != 0)
    {
        PRINTF(" failed\r\n  ! mbedtls_ctr_drbg_seed returned %d\r\n", ret);
        goto exit;
    }

    /*
     * 3. Setting up the SSL data.
     */
    if ((ret = mbedtls_ssl_config_defaults(&conf, MBEDTLS_SSL_IS_SERVER, MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        PRINTF(" failed\r\n  ! mbedtls_ssl_config_defaults returned %d\r\n\r\n", ret);
        goto exit;
    }

    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
    mbedtls_ssl_conf_dbg(&conf, my_debug, NULL);
#if defined(MBEDTLS_SSL_CACHE_C)
    mbedtls_ssl_conf_session_cache(&conf, &cache, mbedtls_ssl_cache_get, mbedtls_ssl_cache_set);
#endif
    mbedtls_ssl_conf_ca_chain(&conf, srvcert.next, NULL);
    if ((ret = mbedtls_ssl_conf_own_cert(&conf, &srvcert, &pkey)) != 0)
    {
        PRINTF(" failed\r\n  ! mbedtls_ssl_conf_own_cert returned %d\r\n\r\n", ret);
        goto exit;
    }

    if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0)
    {
        PRINTF(" failed\r\n  ! mbedtls_ssl_setup returned %d\r\n\r\n", ret);
        goto exit;
    }

    httpd_mbedtls_init(&ssl);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" mbedTLS HTTPS Server example\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_ipaddr)[0], ((u8_t *)&fsl_netif0_ipaddr)[1],
           ((u8_t *)&fsl_netif0_ipaddr)[2], ((u8_t *)&fsl_netif0_ipaddr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_netmask)[0], ((u8_t *)&fsl_netif0_netmask)[1],
           ((u8_t *)&fsl_netif0_netmask)[2], ((u8_t *)&fsl_netif0_netmask)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&fsl_netif0_gw)[0], ((u8_t *)&fsl_netif0_gw)[1],
           ((u8_t *)&fsl_netif0_gw)[2], ((u8_t *)&fsl_netif0_gw)[3]);
    PRINTF("************************************************\r\n");

    while (1)
    {
        /* Poll the driver, get any outstanding frames */
        ethernetif_input(&fsl_netif0);

        sys_check_timeouts(); /* Handle all system timeouts for all core protocols */
    }

exit:
    return -1;
}
#endif // LWIP_TCP
