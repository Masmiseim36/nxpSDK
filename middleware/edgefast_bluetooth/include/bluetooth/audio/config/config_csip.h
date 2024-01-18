/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_CSIP_H__
#define __CONFIG_CSIP_H__

//#################### Coordinated Set Identification Service ####################

/*! @brief Coordinated Set Identification Profile Set Member support
 * This option enables support for Coordinated Set Identification
 * Profile Set Member role and the Coordinated Set Identification
 * Service.
 */
#ifndef CONFIG_BT_CSIP_SET_MEMBER
    #define CONFIG_BT_CSIP_SET_MEMBER 0
#endif

#if (defined(CONFIG_BT_CSIP_SET_MEMBER) && (CONFIG_BT_CSIP_SET_MEMBER > 0))

/*! @brief Coordinated Set Identification Service Test Sample Data
 * Enable the use of the sample data defined by the CSIS spec SIRK.
 * This will use the sample SIRK, prand and LTK.   
 * 
 * WARNING: This option enables anyone to track and decrypt the SIRK
 * (if encrypted) using public sample data.
 * Should not be used for production builds.
 */
#ifndef CONFIG_BT_CSIP_SET_MEMBER_TEST_SAMPLE_DATA
    #define CONFIG_BT_CSIP_SET_MEMBER_TEST_SAMPLE_DATA 0
#endif

/*! @brief Support for encrypted SIRK
 * Enables support encrypting the SIRK.
 */
#ifndef CONFIG_BT_CSIP_SET_MEMBER_ENC_SIRK_SUPPORT
    #define CONFIG_BT_CSIP_SET_MEMBER_ENC_SIRK_SUPPORT 1
#endif

/*! @brief Coordinated Set Identification Service max instance count
 * This option sets the maximum number of instances of Coordinated Set
 * Identification Services. If the service is declared as primary service
 * then only a single instance is possible.
 *
 * Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_CSIP_SET_MEMBER_MAX_INSTANCE_COUNT
    #define CONFIG_BT_CSIP_SET_MEMBER_MAX_INSTANCE_COUNT 1
#endif

#endif /* CONFIG_BT_CSIP_SET_MEMBER */

//#################### Coordinated Set Identification Client ####################

/*! @brief Coordinated Set Identification Profile Set Coordinator Support [EXPERIMENTAL]
 * This option enables support for Coordinated Set Identification Profile Set Coordinator.
 */
#ifndef CONFIG_BT_CSIP_SET_COORDINATOR
    #define CONFIG_BT_CSIP_SET_COORDINATOR 0
#endif

#if (defined(CONFIG_BT_CSIP_SET_COORDINATOR) && (CONFIG_BT_CSIP_SET_COORDINATOR > 0))

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_CSIP_SET_COORDINATOR depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_CSIP_SET_COORDINATOR depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

#endif /* CONFIG_BT_CSIP_SET_COORDINATOR */

#if (defined(CONFIG_BT_CSIP_SET_COORDINATOR) && (CONFIG_BT_CSIP_SET_COORDINATOR > 0))

/*! @brief Coordinated Set Identification Service Test Sample Data
 * Enable the use of the sample data defined by the CSIS spec.
 * This will use the sample SIRK, prand and LTK.
 * 
 * WARNING: This option enables the client to use the sample data
 * to locate members and decrypt the SIRK, and thus won't work
 * with set members that are not using the sample data.
 * Should not be used for production builds.
 */
#ifndef CONFIG_BT_CSIP_SET_COORDINATOR_TEST_SAMPLE_DATA
    #define CONFIG_BT_CSIP_SET_COORDINATOR_TEST_SAMPLE_DATA 0
#endif

/*! @brief Coordinated Set Identification Service Count
 * Sets the number of service instances, which corresponds to the number
 * of different sets the peer device may be in.
 *
 * Valid range 1 ~ 3
 */
#ifndef CONFIG_BT_CSIP_SET_COORDINATOR_MAX_CSIS_INSTANCES
    #define CONFIG_BT_CSIP_SET_COORDINATOR_MAX_CSIS_INSTANCES 1
#endif

/*! @brief Support for encrypted SIRK
 * Enables support encrypting the SIRK.
 */
#ifndef CONFIG_BT_CSIP_SET_COORDINATOR_ENC_SIRK_SUPPORT
    #define CONFIG_BT_CSIP_SET_COORDINATOR_ENC_SIRK_SUPPORT 1
#endif

#endif /* CONFIG_BT_CSIP_SET_COORDINATOR */

#endif /* __CONFIG_CSIP_H__ */