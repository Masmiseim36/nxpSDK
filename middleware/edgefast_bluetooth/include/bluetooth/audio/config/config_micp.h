/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_MICP_H__
#define __CONFIG_MICP_H__

//##################### Microphone Input Control Profile Microphone Device #####################

/*! @brief Microphone Input Control Profile Microphone Device Support
 * This option enables support for Microphone Input Control Profile Microphone Device.
 */
#ifndef CONFIG_BT_MICP_MIC_DEV
    #define CONFIG_BT_MICP_MIC_DEV 0
#endif

#if (defined(CONFIG_BT_MICP_MIC_DEV) && (CONFIG_BT_MICP_MIC_DEV > 0))

/*! @brief Audio Input Control Service instance count for Microphone Input Control Service Microphone Device
 * This option sets the number of instances of Audio Input Control Services for Microphone Input Control Profile Microphone Device.
 *
 * Valid range 0 ~ CONFIG_BT_AICS_MAX_INSTANCE_COUNT
 */
#ifndef CONFIG_BT_MICP_MIC_DEV_AICS_INSTANCE_COUNT
    #define CONFIG_BT_MICP_MIC_DEV_AICS_INSTANCE_COUNT 0
#endif

//This hidden option makes it possible to easily check if AICS is enabled for Microphone Input Control Profile Microphone Device.
#if (defined(CONFIG_BT_MICP_MIC_DEV_AICS_INSTANCE_COUNT) && (CONFIG_BT_MICP_MIC_DEV_AICS_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_MICP_MIC_DEV_AICS
        #define CONFIG_BT_MICP_MIC_DEV_AICS 1
    #endif /* CONFIG_BT_MICP_MIC_DEV_AICS */
#endif /* CONFIG_BT_MICP_MIC_DEV_AICS_INSTANCE_COUNT */

#endif /* CONFIG_BT_MICP_MIC_DEV */

//##################### Microphone Input Control Profile Microphone Controller #####################

/*! @brief Microphone Input Control Profile Microphone Controller Support
 * This option enables support for the Microphone Input Control Profile Microphone Controller role
 */
#ifndef CONFIG_BT_MICP_MIC_CTLR
    #define CONFIG_BT_MICP_MIC_CTLR 0
#endif

#if (defined(CONFIG_BT_MICP_MIC_CTLR) && (CONFIG_BT_MICP_MIC_CTLR > 0))

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_MICP_MIC_CTLR depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_MICP_MIC_CTLR depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

#endif /* CONFIG_BT_MICP_MIC_CTLR */

#if (defined(CONFIG_BT_MICP_MIC_CTLR) && (CONFIG_BT_MICP_MIC_CTLR > 0))

/*! @brief Maximum number of Audio Input Control Service instances to setup
 * Sets the maximum number of Audio Input Control Service (AICS) instances to setup and use.
 *
 * Valid range 0 ~ CONFIG_BT_AICS_CLIENT_MAX_INSTANCE_COUNT
 */
#ifndef CONFIG_BT_MICP_MIC_CTLR_MAX_AICS_INST
    #define CONFIG_BT_MICP_MIC_CTLR_MAX_AICS_INST 0
#endif

//This hidden option makes it possible to easily check if AICS is enabled for MICP client.
#if (defined(CONFIG_BT_MICP_MIC_CTLR_MAX_AICS_INST) && (CONFIG_BT_MICP_MIC_CTLR_MAX_AICS_INST > 0))
    #ifndef CONFIG_BT_MICP_MIC_CTLR_AICS
        #define CONFIG_BT_MICP_MIC_CTLR_AICS 1
    #endif /* CONFIG_BT_MICP_MIC_CTLR_AICS */
#endif /* CONFIG_BT_MICP_MIC_CTLR_MAX_AICS_INST */

#endif /* CONFIG_BT_MICP_MIC_CTLR */

#endif /* __CONFIG_MICP_H__ */