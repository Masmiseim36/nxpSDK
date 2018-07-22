/*-----------------------------------------------------------------------------
 *      Name:         DV_Config.h
 *      Purpose:      DV Config header
 *----------------------------------------------------------------------------
 *      Copyright(c) KEIL - An ARM Company
 *----------------------------------------------------------------------------*/
#ifndef __DV_CONFIG_H
#define __DV_CONFIG_H

#include <stdint.h>
#include "RTE_Components.h"

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h> Common Test Settings
// <o> Print Output Format <0=> Plain Text <1=> XML
// <i> Set the test results output format to plain text or XML
#ifndef PRINT_XML_REPORT
#define PRINT_XML_REPORT 0
#endif
// <o> Buffer size for assertions results
// <i> Set the buffer size for assertions results buffer
#define BUFFER_ASSERTIONS 128
// <h> Buffer sizes
// <i> Enable/disable buffer sizes (bytes)
// <q> 1 - 32
#define BUFFER_ELEM_1_32 0
// <q> 512
#define BUFFER_ELEM_512 1
// <q> 1024
#define BUFFER_ELEM_1024 0
// <q> 4096
#define BUFFER_ELEM_4096 0
// <q> 32768
#define BUFFER_ELEM_32768 0
// </h>
// <o> Buffer size for baudrate test
// <i> Set the buffer sizes (bytes)
#define BUFFER_SIZE_BR 512
// <o> Buffer pattern <0=> 0x55, 0xAA <1=> 0x12, 0x34, 0x56, 0x78
// <i> Set the buffer pattern
#define BUFFER_PATTERN_INDEX 0
#if (BUFFER_PATTERN_INDEX == 0)
#define BUFFER_PATTERN \
    {                  \
        0x55, 0xAA     \
    }
#endif
// </h>

// <h> SPI
// <o> Driver_SPI# <0-255>
// <i> Choose the Driver_SPI# instance to be tested
#define DRV_SPI 0
// <h> SPI bus speed
// <i> Set the SPI bus speed (kHz)
// <i> Value zero is ignored
// <o> SPI bus speed 1
#define SPI_BR_1 400
// <o> SPI bus speed 2
#define SPI_BR_2 1000
// <o> SPI bus speed 3
#define SPI_BR_3 4000
// <o> SPI bus speed 4
#define SPI_BR_4 10000
// <o> SPI bus speed 5
#define SPI_BR_5 25000
// <o> SPI bus speed 6
#define SPI_BR_6 50000
// </h>
// <o> Percentual trigger for bus speed test
// <i> Set the minimum margin between measured and expected bus speeds (%)
#define MIN_BUS_SPEED 5
// <o> SPI data bits
// <i> Set the SPI data bits
#define SPI_DATA_BITS 8
// <o> Transfer timeout
// <i> Set the transfer timeout (us)
#define SPI_TRANSFER_TIMEOUT 1000000
// <h> Test Cases
// <i> Enable/disable test cases
// <q> SPI_GetCapabilities
#define SPI_GETCAPABILITIES_EN 1
// <q> SPI_Initialization
#define SPI_INITIALIZATION_EN 1
// <q> SPI_PowerControl
#define SPI_POWERCONTROL_EN 1
// <q> SPI_Config_PolarityPhase
#define SPI_CONFIG_POLARITYPHASE_EN 1
// <q> SPI_Config_DataBits
#define SPI_CONFIG_DATABITS_EN 1
// <q> SPI_Config_BitOrder
#define SPI_CONFIG_BITORDER_EN 1
// <q> SPI_Config_SSMode
#define SPI_CONFIG_SSMODE_EN 1
// <q> SPI_Config_BusSpeed
#define SPI_CONFIG_BUSSPEED_EN 1
// <q> SPI_Config_CommonParams
#define SPI_CONFIG_COMMONPARAMS_EN 1
// <q> SPI_Send
#define SPI_SEND_EN 1
// <q> SPI_Receive
#define SPI_RECEIVE_EN 1
// <q> SPI_Loopback_CheckBusSpeed
#define SPI_LOOPBACK_CHECKBUSSPEED_EN 1
// <q> SPI_Loopback_Transfer
#define SPI_LOOPBACK_TRANSFER_EN 1
// <q> SPI_CheckInvalidInit
#define SPI_CHECKINVALIDINIT_EN 1
// </h>
// </h>

// <h> USART
// <o> Driver_USART# <0-255>
// <i> Choose the Driver_USART# instance to be tested
#define DRV_USART 0
// <h> USART baudrates
// <i> Set the USART baudrates (bps)
// <i> Value zero is ignored
// <o> USART baudrate 1
#define USART_BR_1 9600
// <o> USART baudrate 2
#define USART_BR_2 19200
// <o> USART baudrate 3
#define USART_BR_3 38400
// <o> USART baudrate 4
#define USART_BR_4 57600
// <o> USART baudrate 5
#define USART_BR_5 115200
// <o> USART baudrate 6
#define USART_BR_6 921600
// </h>
// <o> Percentual tolerance for baudrate test
// <i> Set the tolerance between measured and expected baudrates (%)
#define TOLERANCE_BR 5
// <o> USART data bits
// <i> Set the USART data bits
#define USART_DATA_BITS 8
// <o> Transfer timeout
// <i> Set the transfer timeout (us)
#define USART_TRANSFER_TIMEOUT 1000000
// <h> Test Cases
// <i> Enable/disable test cases
// <q> USART_GetCapabilities
#define USART_GETCAPABILITIES_EN 1
// <q> USART_Initialization
#define USART_INITIALIZATION_EN 1
// <q> USART_PowerControl
#define USART_POWERCONTROL_EN 1
// <q> USART_Config_PolarityPhase
#define USART_CONFIG_POLARITYPHASE_EN 1
// <q> USART_Config_DataBits
#define USART_CONFIG_DATABITS_EN 1
// <q> USART_Config_StopBits
#define USART_CONFIG_STOPBITS_EN 1
// <q> USART_Config_Parity
#define USART_CONFIG_PARITY_EN 1
// <q> USART_Config_Baudrate
#define USART_CONFIG_BAUDRATE_EN 1
// <q> USART_Config_CommonParams
#define USART_CONFIG_COMMONPARAMS_EN 1
// <q> USART_Send
#define USART_SEND_EN 1
// <q> USART_Loopback_CheckBaudrate
#define USART_LOOPBACK_CHECKBAUDRATE_EN 1
// <q> USART_Loopback_Transfer
#define USART_LOOPBACK_TRANSFER_EN 1
// <q> USART_CheckInvalidInit
#define USART_CHECKINVALIDINIT_EN 1
// </h>
// </h>

// <h> Ethernet
// <o> Driver_ETH_MAC# <0-255>
// <i> Choose the Driver_ETH_MAC# instance to be tested
#define DRV_ETH 0
// <o> Link timeout
// <i> Set the Ethernet link timeout (us)
#define ETH_LINK_TIMEOUT 3000000
// <o> Transfer timeout
// <i> Set the transfer timeout (us)
#define ETH_TRANSFER_TIMEOUT 1000000
// <o> Time duration for PTP Control Time
// <i> Set time duration for Control Time tests (ms)
#define ETH_PTP_TIME_REF 1000
// <o> Tolerance for PTP Control Time
// <i> Set tolerance for Control Time tests (ns)
#define ETH_PTP_TOLERANCE 0
// <h> Test Cases
// <i> Enable/disable test cases
// <q> ETH_MAC_GetCapabilities
#define ETH_MAC_GETCAPABILITIES_EN 1
// <q> ETH_MAC_Initialization
#define ETH_MAC_INITIALIZATION_EN 1
// <q> ETH_MAC_PowerControl
#define ETH_MAC_POWERCONTROL_EN 1
// <q> ETH_MAC_SetBusSpeed
#define ETH_MAC_SETBUSSPEED_EN 1
// <q> ETH_MAC_Config_Mode
#define ETH_MAC_CONFIG_MODE_EN 1
// <q> ETH_MAC_Config_CommonParams
#define ETH_MAC_CONFIG_COMMONPARAMS_EN 1
// <q> ETH_MAC_PTP_ControlTimer
#define ETH_MAC_PTP_CONTROLTIMER_EN 1
// <q> ETH_PHY_Initialization
#define ETH_PHY_INITIALIZATION_EN 1
// <q> ETH_PHY_PowerControl
#define ETH_PHY_POWERCONTROL_EN 1
// <q> ETH_PHY_Config
#define ETH_PHY_CONFIG_EN 1
// <q> ETH_Loopback_Transfer
#define ETH_LOOPBACK_TRANSFER_EN 1
// <q> ETH_Loopback_PTP
#define ETH_LOOPBACK_PTP_EN 1
// <q> ETH_PHY_CheckInvalidInit
#define ETH_PHY_CHECKINVALIDINIT_EN 1
// <q> ETH_MAC_CheckInvalidInit
#define ETH_MAC_CHECKINVALIDINIT_EN 1
// </h>
// </h>

// <h> I2C
// <o> Driver_I2C# <0-255>
// <i> Choose the Driver_I2C# instance to be tested
#define DRV_I2C 1
// <h> Test Cases
// <i> Enable/disable test cases
// <q> I2C_GetCapabilities
#define I2C_GETCAPABILITIES_EN 1
// <q> I2C_Initialization
#define I2C_INITIALIZATION_EN 1
// <q> I2C_PowerControl
#define I2C_POWERCONTROL_EN 1
// <q> I2C_SetBusSpeed
#define I2C_SETBUSSPEED_EN 1
// <q> I2C_SetOwnAddress
#define I2C_SETOWNADDRESS_EN 0
// <q> I2C_BusClear
#define I2C_BUSCLEAR_EN 0
// <q> I2C_AbortTransfer
#define I2C_ABORTTRANSFER_EN 1
// <q> I2C_CheckInvalidInit
#define I2C_CHECKINVALIDINIT_EN 0
// </h>
// </h>

// <h> MCI
// <o> Driver_MCI# <0-255>
// <i> Choose the Driver_MCI# instance to be tested
#define DRV_MCI 0
// <h> Test Cases
// <i> Enable/disable test cases
// <q> MCI_GetCapabilities
#define MCI_GETCAPABILITIES_EN 1
// <q> MCI_Initialization
#define MCI_INITIALIZATION_EN 1
// <q> MCI_PowerControl
#define MCI_POWERCONTROL_EN 1
// <q> MCI_SetBusSpeedMode
#define MCI_SETBUSSPEEDMODE_EN 1
// <q> MCI_Config_DataWidth
#define MCI_CONFIG_DATAWIDTH_EN 1
// <q> MCI_Config_CmdLineMode
#define MCI_CONFIG_CMDLINEMODE_EN 1
// <q> MCI_Config_DriverStrength
#define MCI_CONFIG_DRIVERSTRENGTH_EN 1
// <q> MCI_CheckInvalidInit
#define MCI_CHECKINVALIDINIT_EN 1
// </h>
// </h>

// <h> USB Device
// <o> Driver_USBD# <0-255>
// <i> Choose the Driver_USBD# instance to be tested
#define DRV_USBD 0
// <h> Test Cases
// <i> Enable/disable test cases
// <q> USBD_GetCapabilities
#define USBD_GETCAPABILITIES_EN 1
// <q> USBD_Initialization
#define USBD_INITIALIZATION_EN 1
// <q> USBD_PowerControl
#define USBD_POWERCONTROL_EN 1
// <q> USBD_CheckInvalidInit
#define USBD_CHECKINVALIDINIT_EN 1
// </h>
// </h>

// <h> USB Host
// <o> Driver_USBH# <0-255>
// <i> Choose the Driver_USBH# instance to be tested
#define DRV_USBH 0
// <h> Test Cases
// <i> Enable/disable test cases
// <q> USBH_GetCapabilities
#define USBH_GETCAPABILITIES_EN 1
// <q> USBH_Initialization
#define USBH_INITIALIZATION_EN 1
// <q> USBH_PowerControl
#define USBH_POWERCONTROL_EN 1
// <q> USBH_CheckInvalidInit
#define USBH_CHECKINVALIDINIT_EN 1
// </h>
// </h>

// <h> CAN
// <o> Driver_CAN# <0-255>
// <i> Choose the Driver_USBD# instance to be tested
#define DRV_CAN 0
// <h> CAN bitrates
// <i> Set the CAN bitrates (kbit/s)
// <i> Value zero is ignored
// <o> CAN bitrate 1
#define CAN_BR_1 125
// <o> CAN bitrate 2
#define CAN_BR_2 250
// <o> CAN bitrate 3
#define CAN_BR_3 500
// <o> CAN bitrate 4
#define CAN_BR_4 1000
// <o> CAN bitrate 5
#define CAN_BR_5 0
// <o> CAN bitrate 6
#define CAN_BR_6 0
// </h>
// <o> Ratio data/arbitration bitrate
// <i> Set the ratio between data and arbitration bitrate for CAN FD
#define CAN_DATA_ARB_RATIO 8
// <o> Percentual trigger for bitrate test
// <i> Set the minimum margin between measured and expected birates (%)
#define MIN_BITRATE 10
// <o> Transfer timeout
// <i> Set the transfer timeout (us)
#define CAN_TRANSFER_TIMEOUT 1000000
// <h> Test Cases
// <i> Enable/disable test cases
// <q> CAN_GetCapabilities
#define CAN_GETCAPABILITIES_EN 1
// <q> CAN_Initialization
#define CAN_INITIALIZATION_EN 1
// <q> CAN_PowerControl
#define CAN_POWERCONTROL_EN 1
// <q> CAN_Loopback_CheckBitrate
#define CAN_LOOPBACK_CHECK_BR_EN 1
// <q> CAN_Loopback_CheckBitrateFD
#define CAN_LOOPBACK_CHECK_BR_FD_EN 1
// <q> CAN_Loopback_Transfer
#define CAN_LOOPBACK_TRANSFER_EN 1
// <q> CAN_Loopback_TransferFD
#define CAN_LOOPBACK_TRANSFER_FD_EN 1
// <q> CAN_CheckInvalidInit
#define CAN_CHECKINVALIDINIT_EN 1
// </h>
// </h>

//------------------------------------------------------------------------------

// Device specific local loopback settings
#define SPI_LOCAL_LOOPBACK()
#define USART_LOCAL_LOOPBACK()

#endif /* __DV_CONFIG_H */
