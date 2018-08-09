/*
 * \file SPIDevice.h
 * This is the header file for the SPIDevice module.
 *
 * The Clear BSD License
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

#ifndef __SPI_DEV__
#define __SPI_DEV__

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include <fcntl.h>
#include <unistd.h>
#include <linux/types.h>

#include "hsdkOSCommon.h"
#include "PhysicalDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/**
 * @brief Structure to identify a SPI port.
 */
typedef struct {
    /*/dev/spidevB.C ... character special device, major number 153 with
    a dynamically chosen minor device number.  This is the node
    that userspace programs will open, created by "udev" or "mdev". */
    char *deviceName;
    /* The file descriptor abstraction of the device in the operating system. */
    File portHandle;
    /* Used to block for interrupts. */
    File uioPortHandle;
    /* Used to read packets in two chunks: header + payload and CRC. */
    uint8_t lengthFieldSize;
} SPIHandle;

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
#define DK07_HOST       (1)
#define IMX6ULEVK_HOST  (0)

#if DK07_HOST
    #define SPI_DAP_DEV "/sys/class/gpio/gpio42/value"
    #define SPI_CS_DEV  "/sys/class/gpio/gpio45/value"
#elif IMX6ULEVK_HOST
    #define SPI_DAP_DEV "/dev/uio0"
#else
    #error "Please select an SPI master host!"
#endif

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
int AttachToSPIDevice(PhysicalDevice *pDevice, char *deviceName);
int DetachFromSPIDevice(PhysicalDevice *pDevice);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
