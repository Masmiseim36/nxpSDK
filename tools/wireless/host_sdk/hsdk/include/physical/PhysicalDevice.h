/*
 * \file PhysicalDevice.h
 * This is the header file for the PhysicalDevice module.
 *
 * The Clear BSD License
 * Copyright 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the
 * disclaimer below) provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 * GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __PHYS_DEVICE__
#define __PHYS_DEVICE__

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include <stdint.h>

#include "EventManager.h"
#include "hsdkOSCommon.h"
#include "MessageQueue.h"
#include "utils.h"

#ifdef _WINDLL
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/**
 * @brief Types of devices supported.
 */
typedef enum {
    UART,
    USB,
    PCAP,
    SPI,
    BT
} DeviceType;

/**
 * @brief Status of the device.
 */
typedef enum {
    PHYS_CLOSED,
    PHYS_OPENED,
    PHYS_ERROR
} DeviceStatus;

/**
* @brief Available policies for FSCI ACK synchronization.
*/
typedef enum {
    NONE,
    TX,
    RX,
    TX_RX,
    GLOBAL
} FsciAckPolicy;

/**
 * @brief Generic structure for interfacing with the lower level hardware.
 */
typedef struct {
    DeviceType type;            /**< The type of the device. */
    DeviceStatus status;        /**< Status of the physical device. */
    void *configurationData;    /**< Generic pointer to the specific device structure. */
    ConfigParams *configParams; /**< Pointer to the configuration parameters. */
    MessageQueue *inMessages;   /**< An inbox message queue for the device from which to send to the hardware. */
    EventManager *evtManager;   /**< Subscription based event handler to notify all registered components of an event. */
    void *deviceHandle;         /**< A generic handle for the device to send and receive data. */
    Thread eventThread;         /**< The thread to wait for events from the device. */
    Event startThread;          /**< An event used to synchronize the main thread and the eventThread. */
    Event stopThread;           /**< An event used to signal the eventThread to stop. */

    int(*open) (void *, void *);                /**< Function pointer for the device specific open function. It passes specificData as an argument. */
    int(*close) (void *);                       /**< Function pointer for the device specific close function. */
    int(*write) (void *, uint8_t *, uint32_t);  /**< Function pointer to the device specific function to write data into it. */
    int(*read) (void *, uint8_t *, uint32_t *); /**< Function pointer to the device specific function for reading data from it. */
    int(*initialize) (void *, uint8_t);         /**< SPI specific: read data available on the bus at thread start. */
    int(*configure) (void *, void *);           /**< Configuration function. */

    Event (*waitable) (void *, void **); /**< A pointer to a function that returns an event that is waitable until the data has arrived to be read. */
} PhysicalDevice;


/*! *********************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
DLLEXPORT PhysicalDevice *InitPhysicalDevice(DeviceType, void *, char *, FsciAckPolicy);
DLLEXPORT int DestroyPhysicalDevice(PhysicalDevice *);
DLLEXPORT int OpenPhysicalDevice(PhysicalDevice *);
DLLEXPORT int ClosePhysicalDevice(PhysicalDevice *);
DLLEXPORT int ConfigurePhysicalDevice(PhysicalDevice *, void *);
DLLEXPORT int WritePhysicalDevice(void *, uint8_t *, uint32_t);
DLLEXPORT void AttachToPhysicalDevice(void *, void *, void(*Callback)(void *, void *));
DLLEXPORT void DetachFromPhysicalDevice(void *, void *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
