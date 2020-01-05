/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
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

/*! \file control_lpsci.c
    \brief Defines control sub-system

    This file contains a Low power UART implementation of the control subsystem.
    This version is targeted specificially at FRDM-KL25Z, which utilizes a low
    power uart to drive both the OpenSDA and shield UART connections for
    FRDM-MULT2-B Bluetooth module.

    The low power uart utilizes a slightly different interface within KSDK, hence
    this adaptation.

    The command interpreter and streaming functions are contained in two separate
    files.  So you can easily swap those out with only minor changes here.
*/
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_uart.h"
#include "fsl_lpsci.h"
#include "fsl_port.h"
#include "sensor_fusion.h"
#include "control.h"

#define CONTROL_BAUDRATE        115200  ///< Baudrate to be used for serial communications

// global structures
uint8_t           sUARTOutputBuffer[256];             // larger than the nominal 124 byte size for outgoing packets

// direct access to sfg here is the only place in the entire library where we cannot simply
// pass a pointer.  This is because it is needed by the UART interrupt handlers.  Since this
// only occurs here, in a subsystem which is defined to be application dependent, that is
// considered acceptable.
extern SensorFusionGlobals sfg;

// Blocking function to write a single byte to a specified UART
void myUART_WriteByte(UART0_Type *base, uint8_t data)
{
    uint8_t flag = 0;
    while (!flag)
    {
        flag = base->S1 & 0x80;
    }
    LPSCI_WriteByte(base, data);
}

// Blocking function pipes specified buffer to both output UARTS
int8_t writeControlPort(ControlSubsystem *pComm, uint8_t buffer[], uint16_t nbytes)
{
    uint16_t    i;
    for (i = 0; i < nbytes; i++)
    {
        myUART_WriteByte(CONTROL_UART, buffer[i]);
    }

    return (0);
}

// initialize BlueRadios BR-LE4.0-D2A Bluetooth module
// This is required for NXP FRDM-FXS-MULT2-B boards.
void BlueRadios_Init(void)
{
	uint16_t ilen;		// command string length

	// transmit "ATSRM,2,0\r" to minimize traffic from the module
	// command "ATSRM": sets the module response mode which configures how verbose the module will be
	// 2: response mode at to minimal
	// 0: disconnected mode is command mode
	// \r: carriage return escape sequence
	strcpy((char *)sUARTOutputBuffer, "ATSRM,2,0\r");
	ilen = strlen((char *)sUARTOutputBuffer);
        writeControlPort(NULL, sUARTOutputBuffer, ilen);
 	return;
}

void CONTROL_UART_IRQHandler(void)
{
    uint8_t     data;
    static char iCommandBuffer_A[5] = "~~~~";	// 5 bytes long to include the unused terminating \0

    sfg.setStatus(&sfg, RECEIVING_WIRED);

    if ((kLPSCI_RxDataRegFullFlag)&LPSCI_GetStatusFlags(CONTROL_UART))
    {
        data = LPSCI_ReadByte(CONTROL_UART);
        DecodeCommandBytes(&sfg, iCommandBuffer_A, &data, 1);
    }

}

/// Initialize the control subsystem and all related hardware
int8_t initializeControlPort(
    ControlSubsystem *pComm  ///< pointer to the control subystem structure
)
{
    lpsci_config_t   config;
    if (pComm)
    {
        pComm->DefaultQuaternionPacketType = Q3;    // default to simplest algorithm
        pComm->QuaternionPacketType = Q3;           // default to simplest algorithm
        pComm->AngularVelocityPacketOn = true;      // transmit angular velocity packet
        pComm->DebugPacketOn = true;                // transmit debug packet
        pComm->RPCPacketOn = true;                  // transmit roll, pitch, compass packet
        pComm->AltPacketOn = true;                 // Altitude packet
        pComm->AccelCalPacketOn = 0;
        pComm->write = writeControlPort;
        pComm->stream = CreateAndSendPackets;

        /* Initialize WIRED UART pins below - currently duplicates code in pin_mux.c */
        CLOCK_SetLpsci0Clock(0x1U);
        CLOCK_EnableClock(CONTROL_UART_PORT_CLKEN);
        PORT_SetPinMux(CONTROL_UART_PORT, CONTROL_UART_RX_PIN, CONTROL_UART_MUX);
        PORT_SetPinMux(CONTROL_UART_PORT, CONTROL_UART_TX_PIN, CONTROL_UART_MUX);
        LPSCI_GetDefaultConfig(&config);

        config.baudRate_Bps = CONTROL_BAUDRATE;
        config.enableTx = true;
        config.enableRx = true;
        LPSCI_Init(CONTROL_UART, &config, CLOCK_GetFreq(CONTROL_UART_CLKSRC));
        //BlueRadios_Init();

        // Enable RX interrupt.
        LPSCI_EnableInterrupts(CONTROL_UART, kLPSCI_RxDataRegFullInterruptEnable);
        EnableIRQ(CONTROL_UART_IRQn);

        return (0);
    }
    else
    {
        return (1);
    }
}
