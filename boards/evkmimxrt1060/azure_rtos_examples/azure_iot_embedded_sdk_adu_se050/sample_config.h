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

#ifndef SAMPLE_CONFIG_H
#define SAMPLE_CONFIG_H

#ifdef __cplusplus
extern   "C" {
#endif

/* @TEST_ANCHOR */

/* Define USE_DEVICE_CERTIFICATE and set your device certificate to connect to IoT Hub
   with x509 certificate. Set up X.509 security in your Azure IoT Hub,
   refer to https://learn.microsoft.com/en-us/azure/iot-hub/tutorial-x509-introduction */

#define USE_DEVICE_CERTIFICATE          1

/* Defined, DPS is enabled. */
/* #define ENABLE_DPS_SAMPLE */

/* Defined, telemetry is disabled. */
/* #define DISABLE_TELEMETRY_SAMPLE */

/* Defined, C2D is disabled. */
/* #define DISABLE_C2D_SAMPLE */

/* Defined, Direct method is disabled. */
/* #define DISABLE_DIRECT_METHOD_SAMPLE */

/* Defined, Device twin is disabled. */
/* #define DISABLE_DEVICE_TWIN_SAMPLE */

#ifndef ENABLE_DPS_SAMPLE

/* Required when DPS is not used.  */
/* These values can be picked from device connection string which is of format:
   HostName=<host1>;DeviceId=<device1>;SharedAccessKey=<key1>
   HOST_NAME can be set to <host1>,
   DEVICE_ID can be set to <device1>,
   DEVICE_SYMMETRIC_KEY can be set to <key1>.  */

#ifndef HOST_NAME
#define HOST_NAME                                   ""
#endif /* HOST_NAME */

#ifndef DEVICE_ID
#define DEVICE_ID                                   ""
#endif /* DEVICE_ID */

#else /* !ENABLE_DPS_SAMPLE */

/* Required when DPS is used.  */
#ifndef ENDPOINT
#define ENDPOINT                                    "global.azure-devices-provisioning.net"
#endif /* ENDPOINT */

#ifndef ID_SCOPE
#define ID_SCOPE                                    ""
#endif /* ID_SCOPE */

#ifndef REGISTRATION_ID
#define REGISTRATION_ID                             ""
#endif /* REGISTRATION_ID */

#endif /* ENABLE_DPS_SAMPLE */

/* Optional SYMMETRIC KEY.  */
#ifndef DEVICE_SYMMETRIC_KEY
#define DEVICE_SYMMETRIC_KEY                        ""
#endif /* DEVICE_SYMMETRIC_KEY */

/* Optional module ID.  */
#ifndef MODULE_ID
#define MODULE_ID                                   ""
#endif /* MODULE_ID */

#if (USE_DEVICE_CERTIFICATE == 1)

/* Using X509 certificate authenticate to connect to IoT Hub,
   set the device certificate as your device.  */

/* Device Key type. */
#ifndef DEVICE_KEY_TYPE
#define DEVICE_KEY_TYPE                             NX_SECURE_X509_KEY_TYPE_HARDWARE  // Use SE05x
#endif /* DEVICE_KEY_TYPE */

#endif /* USE_DEVICE_CERTIFICATE */

/* Define the Azure RTOS IOT thread stack and priority.  */
#ifndef NX_AZURE_IOT_STACK_SIZE
#define NX_AZURE_IOT_STACK_SIZE                     (4096)
#endif /* NX_AZURE_IOT_STACK_SIZE */

#ifndef NX_AZURE_IOT_THREAD_PRIORITY
#define NX_AZURE_IOT_THREAD_PRIORITY                (4)
#endif /* NX_AZURE_IOT_THREAD_PRIORITY */

#ifndef SAMPLE_MAX_BUFFER
#define SAMPLE_MAX_BUFFER                           (256)
#endif /* SAMPLE_MAX_BUFFER */

/* Device properties.  */
#define SAMPLE_DEVICE_MANUFACTURER                                      "NXP"
#define SAMPLE_DEVICE_MODEL                                             "MIMXRT1xxx"
#define SAMPLE_DEVICE_FIRMWARE_VERSION                                  "1.0.0"

#ifdef __cplusplus
}
#endif
#endif /* SAMPLE_CONFIG_H */
