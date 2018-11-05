/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _MCDRV_SPI_DRIVER3PH_H_
#define _MCDRV_SPI_DRIVER3PH_H_

#include "mlib.h"
#include "mlib_types.h"
#include "fsl_dspi.h"
#include "fsl_device_registers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define GPIO_MUX (1)

typedef union _mc33937ConfigMask
{
    uint8_t R;
    struct
    {/* MC33937 faults */
        uint8_t overTemp : 1;
        uint8_t desaturation : 1;
        uint8_t lowVls : 1;
        uint8_t overCurrent : 1;
        uint8_t phaseErr : 1;
        uint8_t framingErr : 1;
        uint8_t writeErr : 1;
        uint8_t resetEvent : 1;
    } B;
} mc33937ConfigMask_t;

typedef union _mc33937Mode
{
    uint8_t R;
    struct
    {
        uint8_t lock : 1;         /* lock configuration regs */
        uint8_t enableFullOn : 1; /* enable FULL ON PWM without DT */
        uint8_t : 1;
        uint8_t disableDesat : 1; /* disable phase desaturation error */
        uint8_t : 1;
        uint8_t : 1;
        uint8_t : 1;
        uint8_t : 1;
    } B;
} mc33937Mode_t;

typedef union _mc33937SR0
{
    uint8_t R;
    struct
    {
        uint8_t overTemp : 1;     /* TLIM flag detected on any channel */
        uint8_t desaturation : 1; /* DESAT flag detected on any channel */
        uint8_t lowVls : 1;       /* Low VLS voltage flag */
        uint8_t overCurrent : 1;  /* Over-current event flag */
        uint8_t phaseErr : 1;     /* Phase error flag */
        uint8_t framingErr : 1;   /* Framing error flag */
        uint8_t writeErr : 1;     /* Write Error After the Lock flag */
        uint8_t resetEvent : 1;   /* reset event flag, is set upon exiting /RST */
    } B;
} mc33937SR0_t;

typedef union _mc33937SR1
{
    uint8_t R;
    struct
    {
        uint8_t lockbit : 1;      /* LockBit indicates the IC regs are locked */
        uint8_t fullon : 1;       /* present status of FULLON MODE */
        uint8_t : 1;              /* reserved */
        uint8_t deadtime_cal : 1; /* Deadtime calibration occurred */
        uint8_t calib_overfl : 1; /* flag for a Deadtime Calibration Overflow */
        uint8_t zds : 1;          /* Zero deadtime is commanded */
        uint8_t desat_mode : 1;   /* current state of the Desaturation/Phase Error turn-off mode */
        uint8_t : 1;              /* reserved */
    } B;
} mc33937SR1_t;

typedef union _mc33937SR2
{
    uint8_t R;
    struct
    {
        uint8_t mask0_0 : 1; /* status of the MASK0.0 bit */
        uint8_t mask0_1 : 1; /* status of the MASK0.1 bit */
        uint8_t mask0_2 : 1; /* status of the MASK0.2 bit */
        uint8_t mask0_3 : 1; /* status of the MASK0.3 bit */
        uint8_t mask1_0 : 1; /* status of the MASK1.0 bit */
        uint8_t mask1_1 : 1; /* status of the MASK1.1 bit */
        uint8_t mask1_2 : 1; /* status of the MASK1.2 bit */
        uint8_t mask1_3 : 1; /* status of the MASK1.3 bit */
    } B;
} mc33937SR2_t;

typedef struct _mc33937dspi
{
    SPI_Type *pSpiBase; /* SPI base address */

    GPIO_Type *pGpioEnBase; /* PORT status register for driver enabled */
    uint32_t ui32GpioEnPin; /* pin number for driver enabled */

    GPIO_Type *pGpioOcBase; /* PORT status register for over-current detection */
    uint32_t ui32GpioOcPin; /* pin number for over-current detection */

    GPIO_Type *pGpioIntBase; /* PORT status register for interrupt detection */
    uint32_t ui32GpioIntPin; /* pin number for interrupt detection */

    GPIO_Type *pGpioResetBase; /* PORT status register for reset */
    uint32_t ui32GpioResetPin; /* pin number for reset, driver */

    uint32_t ui32Pcs; /* Chip select pin */
} mc33937dspi_t;

typedef struct _mcdrv_spi_drv3ph_init
{
    SPI_Type *pSpiBase; /* SPI base address */

    GPIO_Type *pGpioOcBase; /* GPIO status register for over-current detection */
    PORT_Type *pPortOcBase; /* PORT status register for driver enabled */
    uint32_t ui32GpioOcPin; /* pin number for over-current detection */

    GPIO_Type *pGpioIntBase; /* GPIO status register for interrupt detection */
    PORT_Type *pPortIntBase; /* PORT status register for driver enabled */
    uint32_t ui32GpioIntPin; /* pin number for interrupt detection */

    GPIO_Type *pGpioResetBase; /* GPIO status register for reset */
    PORT_Type *pPortResetBase; /* PORT status register for driver enabled */
    uint32_t ui32GpioResetPin; /* pin number for reset, driver */

    GPIO_Type *pGpioEnBase; /* GPIO status register for driver enabled */
    PORT_Type *pPortEnBase; /* PORT status register for driver enabled */
    uint32_t ui32GpioEnPin; /* pin number for driver enabled */

    uint32_t ui32Pcs;       /* PCS pin */
    bool_t bFaultOcEnabled; /* Over current fault detection enabled on GPIO pin */
} mcdrv_spi_drv3ph_init_t;

typedef struct _mcdrv_spi_drv3ph
{
    mc33937dspi_t sSpiData;               /* HW dependent structure*/
    mc33937ConfigMask_t sInterruptEnable; /* define interrupt mask */
    mc33937Mode_t sMode;                  /* define required pre-driver mode */
    mc33937SR0_t sSr0;                    /* status register 0 */
    mc33937SR1_t sSr1;                    /* status register 1 */
    mc33937SR2_t sSr2;                    /* status register 2 */
    uint8_t sSr3;                         /* status register 3 */
    uint16_t ui16Deadtime;                /* define dead time of HS and LS transistors, value in [ns]*/
    bool_t bResetPinEnabled;              /* MC33937 is reset over pin */
} mcdrv_spi_drv3ph_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Function clear MC33937 reset pin
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhClearRst(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function read MC33937 over current pin
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhSetRst(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function clear MC33937 enable pin
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhClearEn(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function set MC33937 enable pin
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhSetEn(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function set MC33937 deadtime
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhSetDeadtime(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function configure MC33937 pre-driver
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhConfig(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Initialization function of MC33937 SPI driver
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhInit(mcdrv_spi_drv3ph_t *this, mcdrv_spi_drv3ph_init_t *init);

/*!
 * @brief Function read MC33937 over current pin
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhReadOc(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function read MC33937 interrupt pin
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhReadInt(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function clear MC33937 flag register
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhClearFlags(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function read MC33937 status register 0
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhGetSr0(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function read MC33937 status register 1
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhGetSr1(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function read MC33937 status register 2
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhGetSr2(mcdrv_spi_drv3ph_t *this);

/*!
 * @brief Function read MC33937 status register 3
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhGetSr3(mcdrv_spi_drv3ph_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_SPI_DRIVER3PH_H */

