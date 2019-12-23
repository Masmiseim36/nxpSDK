/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
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
#include "mcdrv_lpspi_driver3ph.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

static bool_t MCDRV_Driver3PhSendCmd(mcdrv_spi_drv3ph_t *this, 
                                     uint8_t *pui8TxData, 
                                     uint8_t *pui8RxData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static bool_t s_statusPass;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initialization function of MC33937 SPI driver
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhInit(mcdrv_spi_drv3ph_t *this, mcdrv_spi_drv3ph_init_t *init)
{
    s_statusPass = TRUE;

    /* configure GPIO pads for MC33937 signals */
    this->sSpiData.pSpiBase = init->pSpiBase;      /* SPI Base Address */
    this->sSpiData.ui32Pcs = (1 << init->ui32Pcs); /* PCS number */

    /* if enabled, set over-current PIN & PORT - INPUT */
    if (init->bFaultOcEnabled)
    {
        this->sSpiData.pGpioOcBase = init->pGpioOcBase;     /* base Address */
        this->sSpiData.ui32GpioOcPin = init->ui32GpioOcPin; /* pin number for over current */
    }

    this->sSpiData.pGpioIntBase = init->pGpioIntBase;     /* base Address */
    this->sSpiData.ui32GpioIntPin = init->ui32GpioIntPin; /* pin number for interrupt detection */

    this->sSpiData.pGpioEnBase = init->pGpioEnBase;     /* base Address */
    this->sSpiData.ui32GpioEnPin = init->ui32GpioEnPin; /* pin number for driver enabled */

    this->sSpiData.pGpioResetBase = init->pGpioResetBase;     /* base Address */
    this->sSpiData.ui32GpioResetPin = init->ui32GpioResetPin; /* pin number for driver reset */

    this->sSpiData.bResetControl = init->bResetControl;

    return (s_statusPass);
}

/*!
 * @brief Function send SPI command to MC33937
 *
 * @param this          Pointer to the current object
 * @param pui8TxData    Pointer to data which be send via SPI
 * @param pui8RxData    Pointer to data which be read via SPI
 *
 * @return boot_t true on success
 */
static bool_t MCDRV_Driver3PhSendCmd(mcdrv_spi_drv3ph_t *this, uint8_t *pui8TxData, uint8_t *pui8RxData)
{
    uint16_t ui16SafetyCounter = 0; /* eliminate code freezing in while loop */

    s_statusPass = TRUE;

    /* clear TCF bit */
    this->sSpiData.pSpiBase->SR |= (LPSPI_SR_TCF_MASK);

    /* push data */
    LPSPI_WriteData(this->sSpiData.pSpiBase, *pui8TxData);

    /* wait till TCF sets */
    while (((this->sSpiData.pSpiBase->SR & (LPSPI_SR_TCF_MASK)) == 0) && (ui16SafetyCounter < 1000))
    {
        ui16SafetyCounter++;
    }

    /* load value to variable from register */
    *pui8RxData = (uint8_t)LPSPI_ReadData(this->sSpiData.pSpiBase);
    return (s_statusPass);
}

/*!
 * @brief Function read MC33937 over current pin
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhReadOc(mcdrv_spi_drv3ph_t *this)
{
    s_statusPass = TRUE;

    /* read over-current pin */
    s_statusPass = ((this->sSpiData.pGpioOcBase->PDIR) & (1 << this->sSpiData.ui32GpioOcPin));

    return (s_statusPass);
}

/*!
 * @brief Function read MC33937 interrupt pin
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhReadInt(mcdrv_spi_drv3ph_t *this)
{
    s_statusPass = TRUE;

    /* read interrupt pin */
    s_statusPass = ((this->sSpiData.pGpioIntBase->PDIR) & (1 << this->sSpiData.ui32GpioIntPin));

    return (s_statusPass);
}

/*!
 * @brief Function clear MC33937 reset pin
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhClearRst(mcdrv_spi_drv3ph_t *this)
{
    s_statusPass = TRUE;

    /* clear pin output for 3PPA driver reset */
    (this->sSpiData.pGpioResetBase->PCOR) |= (1 << this->sSpiData.ui32GpioResetPin);

    return (s_statusPass);
}

/*!
 * @brief Function set MC33937 reset pin
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhSetRst(mcdrv_spi_drv3ph_t *this)
{
    s_statusPass = TRUE;

    /* set pin output for 3PPA driver reset */
    (this->sSpiData.pGpioResetBase->PSOR) |= (1 << this->sSpiData.ui32GpioResetPin);

    return (s_statusPass);
}

/*!
 * @brief Function clear MC33937 enable pin
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhClearEn(mcdrv_spi_drv3ph_t *this)
{
    s_statusPass = TRUE;

    (this->sSpiData.pGpioEnBase->PCOR) |= (1 << this->sSpiData.ui32GpioEnPin);

    return (s_statusPass);
}

/*!
 * @brief Function set MC33937 enable pin
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhSetEn(mcdrv_spi_drv3ph_t *this)
{
    s_statusPass = TRUE;

    (this->sSpiData.pGpioEnBase->PSOR) |= (1 << this->sSpiData.ui32GpioEnPin);

    return (s_statusPass);
}

/*!
 * @brief Function set MC33937 deadtime
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhSetDeadtime(mcdrv_spi_drv3ph_t *this)
{
    uint8_t ui8TxData, ui8RxData;

    s_statusPass = TRUE;

    /* zero deadtime calibration */
    /* send ZERODEADTIME command for dead time configuration with zero
       calibration */
    ui8TxData = 0x80;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &ui8RxData);

    return (s_statusPass);
}

/*!
 * @brief Function clear MC33937 flag register
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhClearFlags(mcdrv_spi_drv3ph_t *this)
{
    uint8_t ui8TxData, ui8RxData;

    s_statusPass = TRUE;

    /* CLINT0_COMMAND = 0x6F */
    ui8TxData = 0x6F;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &ui8RxData);

    /* CLINT1_COMMAND = 0x7F */
    ui8TxData = 0x7F;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &ui8RxData);

    return (s_statusPass);
}

/*!
 * @brief Function read MC33937 status register 0
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhGetSr0(mcdrv_spi_drv3ph_t *this)
{
    uint8_t ui8TxData;

    s_statusPass = TRUE;

    /* status Register 0 reading = 0x00 */
    ui8TxData = 0x00;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &this->sSr0.R);

    /* status Register 0 write to read SR0 result */
    ui8TxData = 0x00;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &this->sSr0.R);

    return (s_statusPass);
}

/*!
 * @brief Function read MC33937 status register 1
 *
 * @param this Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhGetSr1(mcdrv_spi_drv3ph_t *this)
{
    uint8_t ui8TxData;

    s_statusPass = TRUE;

    /* status Register 1 reading = 0x01 */
    ui8TxData = 0x01;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &this->sSr0.R);

    /* status Register 0 write to read SR1 result */
    ui8TxData = 0x00;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &this->sSr1.R);

    return (s_statusPass);
}

/*!
 * @brief Function read MC33937 status register 2
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhGetSr2(mcdrv_spi_drv3ph_t *this)
{
    uint8_t ui8TxData;

    s_statusPass = TRUE;

    /* status Register 2 reading = 0x02 */
    ui8TxData = 0x02;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &this->sSr0.R);

    /* status Register 0 write to read SR2 result */
    ui8TxData = 0x00;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &this->sSr2.R);

    return (s_statusPass);
}

/*!
 * @brief Function read MC33937 status register 3
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhGetSr3(mcdrv_spi_drv3ph_t *this)
{
    uint8_t ui8TxData;

    s_statusPass = TRUE;

    /* status Register 3 reading = 0x03 */
    ui8TxData = 0x03;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &this->sSr0.R);

    /* status Register 0 write to read SR3 result */
    ui8TxData = 0x00;
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &this->sSr3);

    return (s_statusPass);
}

/*!
 * @brief Function configure MC33937 pre-driver
 *
 * @param this          Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Driver3PhConfig(mcdrv_spi_drv3ph_t *this)
{
    uint8_t ui8TxData, ui8RxData;
    register uint32_t ui32loop_cnt;

    s_statusPass = TRUE;

    /* EN1 and EN2 are still low */
    s_statusPass &= MCDRV_Driver3PhClearEn(this);

    if (this->sSpiData.bResetControl)
    {
        /* set RST high */
        s_statusPass &= MCDRV_Driver3PhSetRst(this);
    }
    /* required start-up delay between the RST going up and initialization
       routine at 75 MHz the delay will be 50 ms , at lower CPU clocks will be
       more than 2 ms */
    for (ui32loop_cnt = 0; ui32loop_cnt < 1500000; ui32loop_cnt++)
    {
        __asm("nop");
    }

    /* clear all faults */
    s_statusPass &= MCDRV_Driver3PhClearFlags(this);

    /* initialize MASK register 0 */
    ui8TxData = 0x20 | (uint8_t)(this->sInterruptEnable.R);
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &ui8RxData);

    /* initialize MASK register 1 */
    ui8TxData = 0x30 | (uint8_t)(this->sInterruptEnable.R >> 4);
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &ui8RxData);

    /* initialize MODE_COMMAND register  */
    ui8TxData = 0x40 | (uint8_t)(this->sMode.R);
    s_statusPass &= MCDRV_Driver3PhSendCmd(this, &ui8TxData, &ui8RxData);

    /* set dead time, ONLY ZERO DT AVAILABLE - TBD */
    s_statusPass &= MCDRV_Driver3PhSetDeadtime(this);

    /* clear all faults */
    s_statusPass &= MCDRV_Driver3PhClearFlags(this);

    return (s_statusPass);
}

