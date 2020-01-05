/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "utilities/fsl_assert.h"
#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "packet/command_packet.h"
#include "fsl_mscan.h"
#include "fsl_device_registers.h"
#include "packet/serial_packet.h"
#include "utilities/fsl_rtos_abstraction.h"

#if BL_CONFIG_CAN

//! @addtogroup mscan_peripheral
//! @{

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief Internal driver state information.
 *
 * @note The contents of this structure are internal to the driver and should not be
 *      modified by users. Also, contents of the structure are subject to change in
 *      future releases.
 */
typedef struct MsCANState
{
    volatile uint32_t rx_mb_idx; /*!< Index of the message buffer for receiving*/
    volatile uint32_t tx_mb_idx; /*!< Index of the message buffer for transmitting*/
    semaphore_t txIrqSync;       /*!< Used to wait for ISR to complete its TX business.*/
    semaphore_t rxIrqSync;       /*!< Used to wait for ISR to complete its RX business.*/
} mscan_state_t;

/*! @brief MsCAN data info from user*/
typedef struct MsCANDataInfo
{
    mscan_frame_format_t msg_id_type; /*!< Type of message ID (standard or extended)*/
    uint32_t data_length;               /*!< Length of Data in Bytes*/
} mscan_data_info_t;

/*! @brief MsCAN operation modes*/
typedef enum _mscan_operation_modes
{
    kMsCanNormalMode,     /*!< Normal mode or user mode*/
    kMsCanListenOnlyMode, /*!< Listen-only mode*/
    kMsCanLoopBackMode,   /*!< Loop-back mode*/
    kMsCanInitializeMode,     /*!< Initialize mode*/
    kMsCanDisableMode,    /*!< Module disable mode*/
} mscan_operation_modes_t;

//! @brief msCAN global information structure.
typedef struct _mscan_transfer_info
{
    mscan_state_t state;       //!< state
    mscan_data_info_t rx_info; //!< tx info
    mscan_data_info_t tx_info; //!< tx info
    int8_t baudrate;             //!< baudrate (index)
    mscan_operation_modes_t mode;
    bool baudrateDetect; //!< auto baudrate detection
    uint16_t txId;
    uint16_t rxId;
    void (*data_source)(uint8_t *source_byte);               // !< Callback used to get byte to transmit.
    void (*data_sink)(uint8_t sink_byte, uint32_t instance); // !< Callback used to put received byte.
} mscan_transfer_info_t;

//! @brief msCAN enums.
#define MSCAN_MAX_SPEED 5
#define MSCAN_DEFAULT_RX_ID 0x321 // default rxid 801
#define MSCAN_DEFAULT_TX_ID 0x123 // default txid 291
//#define FLEXCAN_DEFAULT_SPEED 4

// config1 8 bit
#define MSCAN_SPEED_MASK 0x0F       // bit[3:0]
#define MSCAN_SPEED_SPEC_MASK 0x08  // bit[3:3]
#define MSCAN_SPEED_INDEX_MASK 0x07 // bit[2:0]
#define MSCAN_CLKSEL_MASK 0x80      // bit[7:7]

#define MSCAN_SAMPLE_MASK 0x70 // bit[6:4]
#define MSCAN_SAMPLE_SHIFT 4

// config2 16 bit
#define MSCAN_PRESCALER_MASK 0xFF00 // bit[15:8]
#define MSCAN_PRESCALER_SHIFT 8
#define MSCAN_PSEG1_MASK 0x00E0 // bit[7:5]
#define MSCAN_PSEG1_SHIFT 5
#define MSCAN_PSEG2_MASK 0x001C // bit[4:2]
#define MSCAN_PSEG2_SHIFT 2
#define MSCAN_RJW_MASK 0x0003 // bit[1:0]
#define MSCAN_RJW_SHIFT 0


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

//! @brief msCAN poll for activity function
static bool mscan_poll_for_activity(const peripheral_descriptor_t *self);
//! @brief msCAN init function
static status_t mscan_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function);
//! @brief msCAN shutdown function
static void mscan_full_shutdown(const peripheral_descriptor_t *self);
//! @brief msCAN receiving first byte data sink function
static void mscan_initial_data_sink(uint8_t sink_byte, uint32_t instance);
//! @brief msCAN receiving data sink function
static void mscan_data_sink(uint8_t sink_byte, uint32_t instance);
//! @brief msCAN internal init function
static void mscan_peripheral_init(uint32_t instance);
//! @brief msCAN sending data function
static status_t mscan_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount);
static status_t MSCAN_Send(uint8_t instance,
                             uint32_t mb_idx,
                             mscan_data_info_t *tx_info,
                             uint32_t msg_id,
                             uint8_t *mb_data,
                             uint32_t timeout_ms);
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
volatile bool txComplete = false;
volatile bool rxComplete = false;
static mscan_handle_t mscanHandle;
volatile uint32_t txIdentifier;
volatile uint32_t rxIdentifier;

enum _mscan_state
{
    kMSCAN_StateIdle = 0x0,     /*!< MB/RxFIFO idle.*/
    kMSCAN_StateRxData = 0x1,   /*!< MB receiving.*/
    kMSCAN_StateRxRemote = 0x2, /*!< MB receiving remote reply.*/
    kMSCAN_StateTxData = 0x3,   /*!< MB transmitting.*/
    kMSCAN_StateTxRemote = 0x4, /*!< MB transmitting remote request.*/
    kMSCAN_StateRxFifo = 0x5,   /*!< RxFIFO receiving.*/
};

/*!
 * @brief The table contains pre_divider, rjw, pseg1, pseg2 and samp.
 */
#ifdef KE16Z4_SERIES
/* 8Mhz internal clock based */
const mscan_timing_config_t bit_rate_table_8Mhz[MSCAN_MAX_SPEED] = {
    { 7, 2, 2, 2, 0 }, /* 125 kHz */
    { 3, 2, 2, 2, 0 }, /* 250 kHz */
    { 1, 1, 1, 1, 2 }, /* 500 kHz */
    { 0, 2, 2, 2, 2 }, /* 750 kHz */
    { 0, 1, 1, 1, 2 }  /* 1   MHz */
};

/* 8Mhz osc clock based */
const mscan_timing_config_t bit_rate_table[] = {
    { 7, 0, 3, 2, 0 }, /* 125 kHz */
    { 3, 0, 3, 2, 0 }, /* 250 kHz */
//    { 1, 0, 8, 5, 0 }, /* 500 kHz */
    { 1, 0, 3, 2, 0 }, /* 500 kHz */
    { 15, 0, 7, 5, 0 },  /* 750 kHz */
    { 0, 0, 3, 2, 0 }  /* 1   MHz */
};
#endif


/*!
 * @brief msCAN control interface information
 */
const peripheral_control_interface_t g_mscanControlInterface = {.pollForActivity = mscan_poll_for_activity,
                                                                  .init = mscan_full_init,
                                                                  .shutdown = mscan_full_shutdown,
                                                                  .pump = 0 };

/*!
 * @brief msCAN byte interface information
 */
const peripheral_byte_inteface_t g_mscanByteInterface = {
    .init = NULL, .write = mscan_write,
};

//! @brief Global state for the MSCAN peripheral interface.
mscan_transfer_info_t s_mscanInfo;

//! @brief Flag for mscan intialization state
static bool s_mscanIntialized[FSL_FEATURE_SOC_MSCAN_COUNT] = { false };

static bool s_mscanActivity[FSL_FEATURE_SOC_MSCAN_COUNT] = { false };

/*!
 * @brief msCAN receiving data call back function
 */
static serial_byte_receive_func_t s_mscan_app_data_sink_callback;
const static uint32_t g_mscanBaseAddr[] = MSCAN_BASE_ADDRS;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_GetRxFlag
 * Description   : Get MSCAN receiver Flag.
 * This function will Get MSCAN receiver Flag.
 *
 *END**************************************************************************/
static inline uint8_t MSCAN_GetRxFlag(MSCAN_Type *base)
{
    return base->CANRFLG;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_GetRxInerruptFlag
 * Description   : Get MSCAN receiver interrupt Flag.
 * This function will MSCAN receiver interrupt Flag.
 *
 *END**************************************************************************/
static inline uint8_t MSCAN_GetRxInerruptFlag(MSCAN_Type *base)
{
    return base->CANRIER;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_ClearRxFlag
 * Description   : Clear MSCAN receiver Flag.
 * This function will Clear MSCAN receiver Flag.
 *
 *END**************************************************************************/
static inline uint8_t MSCAN_ClearRxFlag(MSCAN_Type *base, uint8_t mask)
{
    return base->CANRFLG |= mask;
}
/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_ClearRxInerruptFlag
 * Description   : Clear MSCAN receiver interrupt Flag.
 * This function will Clear MSCAN receiver interrupt Flag.
 *
 *END**************************************************************************/
static inline uint8_t MSCAN_ClearRxInerruptFlag(MSCAN_Type *base, uint8_t mask)
{
    return base->CANRIER |= mask;
}

/*!
 * @brief MSCAN Call Back function
 */
static void mscan_callback(MSCAN_Type *base, mscan_handle_t *handle, status_t status, void *userData)
{
    switch (status)
    {
        /* Process MSCAN Rx event. */
        case kStatus_MSCAN_RxIdle:
            rxComplete = true;
            break;

        /* Process MSCAN Tx event. */
        case kStatus_MSCAN_TxIdle:
            txComplete = true;
            break;

        default:
            break;
    }
}
/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_WakeUp
 * Description   : Perform a wake up to MsCAN.
 * This function will perfomr wake up to MsCAN.
 *
 *END**************************************************************************/
void MSCAN_WakeUp(MSCAN_Type *baseAddr)
{
    baseAddr->CANCTL1 |= MSCAN_CANCTL1_WUPM_MASK;
    baseAddr->CANCTL0 |= MSCAN_CANCTL0_WUPE_MASK;

    /* Wait till exit of wake up*/
    while (baseAddr->CANCTL0 & MSCAN_CANCTL0_WUPE_MASK)
    {
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_ExitInitializeMode
 * Description   : Exit of initialize mode.
 *
 *END**************************************************************************/
void MSCAN_ExitInitializeMode(MSCAN_Type *baseAddr)
{
    baseAddr->CANCTL0 &= ~MSCAN_CANCTL0_INITRQ_MASK;

    /* Wait till exit initialize mode*/
    while (baseAddr->CANCTL1 & MSCAN_CANCTL1_INITAK_MASK)
    {
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_EnterInitializeMode
 * Description   : Enter the initialize mode.
 *
 *END**************************************************************************/
void MSCAN_EnterInitializeMode(MSCAN_Type *baseAddr)
{
    baseAddr->CANCTL0 |= MSCAN_CANCTL0_INITRQ_MASK;

    /* Wait for entering the initialize mode*/
    while (!(baseAddr->CANCTL1 & MSCAN_CANCTL1_INITAK_MASK))
    {
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_EnableOperationMode
 * Description   : Enable a MsCAN operation mode.
 * This function will enable one of the modes listed in mscan_operation_modes_t.
 *
 *END**************************************************************************/
void MSCAN_EnableOperationMode(MSCAN_Type *baseAddr, mscan_operation_modes_t mode)
{
    if (mode == kMsCanInitializeMode)
    {
        /* Debug mode, Halt and Initialize*/
        MSCAN_EnterInitializeMode(baseAddr);
    }
    else if (mode == kMsCanDisableMode)
    {
        /* Debug mode, Halt and Initialize*/
        baseAddr->CANCTL1 |= MSCAN_CANCTL1_CANE_MASK;
    }

    /* Set Initialize mode*/
    MSCAN_EnterInitializeMode(baseAddr);

    if (mode == kMsCanNormalMode)
    {
        baseAddr->CANCTL0 &= ~MSCAN_CANCTL0_INITRQ_MASK;
        baseAddr->CANCTL1 &= ~MSCAN_CANCTL1_LISTEN_MASK;
    }
    else if (mode == kMsCanListenOnlyMode)
    {
        baseAddr->CANCTL1 |= MSCAN_CANCTL1_LISTEN_MASK;
    }
    else if (mode == kMsCanLoopBackMode)
    {
        baseAddr->CANCTL1 |= MSCAN_CANCTL1_LOOPB_MASK;
    }

    /* De-assert Initialize Mode*/
    MSCAN_ExitInitializeMode(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_DisableOperationMode
 * Description   : Disable a MsCAN operation mode.
 * This function will disable one of the modes listed in mscan_operation_modes_t.
 *
 *END**************************************************************************/
void MSCAN_DisableOperationMode(MSCAN_Type *baseAddr, mscan_operation_modes_t mode)
{
    if (mode == kMsCanInitializeMode)
    {
        /* De-assert Initialize Mode*/
        MSCAN_ExitInitializeMode(baseAddr);
    }
    else if (mode == kMsCanDisableMode)
    {
        /* Disable module mode*/
        baseAddr->CANCTL0 &= ~MSCAN_CANCTL1_CANE_MASK;
    }

    /* Set Initialize mode*/
    MSCAN_EnterInitializeMode(baseAddr);

    if (mode == kMsCanNormalMode)
    {
        baseAddr->CANCTL0 |= MSCAN_CANCTL0_INITRQ_MASK;
    }
    else if (mode == kMsCanListenOnlyMode)
    {
        baseAddr->CANCTL1 &= ~MSCAN_CANCTL1_LISTEN_MASK;
    }
    else if (mode == kMsCanLoopBackMode)
    {
        baseAddr->CANCTL1 &= ~MSCAN_CANCTL1_LOOPB_MASK;
    }

    /* De-assert Initialize Mode*/
    MSCAN_ExitInitializeMode(baseAddr);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : mscan_poll_for_activity
 * Description   : Polling for msCAN activities
 *
 *END**************************************************************************/
bool mscan_poll_for_activity(const peripheral_descriptor_t *self)
{
    MSCAN_Type *base = (MSCAN_Type *)g_mscanBaseAddr[self->instance];
    return s_mscanActivity[self->instance];
}

/*FUNCTION**********************************************************************
 *
 * Function Name : mscan_initial_data_sink
 * Description   : Receiving first byte data sink function
 *
 *END**************************************************************************/
void mscan_initial_data_sink(uint8_t sink_byte, uint32_t instance)
{
    if (sink_byte == kFramingPacketStartByte)
    {
        s_mscanActivity[instance] = true;
        s_mscanInfo.data_sink = mscan_data_sink;
        s_mscan_app_data_sink_callback(sink_byte);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : mscan_data_sink
 * Description   : Receiving data sink function
 *
 *END**************************************************************************/
void mscan_data_sink(uint8_t sink_byte, uint32_t instance)
{
    s_mscan_app_data_sink_callback(sink_byte);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : mscan_peripheral_init
 * Description   : Internal init function
 *
 *END**************************************************************************/
void mscan_peripheral_init(uint32_t instance)
{
    mscan_config_t config;
    uint8_t canConfig1;
    uint16_t canConfig2;
    uint16_t canId;
    uint32_t baseAddr = g_mscanBaseAddr[instance];
    osa_status_t syncStatus;
    MSCAN_GetDefaultConfig(&config);
    config.clkSrc = kMSCAN_ClkSrcOsc;

    mscan_timing_config_t bit_rate_table1;
    
    // Read the can txId. If it is not set, i.e. 0xff,
    canId = g_bootloaderContext.propertyInterface->store->configurationData.canTxId;
    if (canId != 0xffff)
    {
        s_mscanInfo.txId = canId & 0x7ff; // support 11bit std id format
    }
    else
    {
        s_mscanInfo.txId = MSCAN_DEFAULT_TX_ID;
    }

    // Read the can rxId. If it is not set, i.e. 0xff,
    canId = g_bootloaderContext.propertyInterface->store->configurationData.canRxId;
    if (canId != 0xffff)
    {
        s_mscanInfo.rxId = canId & 0x7ff; // support 11bit std id format
    }
    else
    {
        s_mscanInfo.rxId = MSCAN_DEFAULT_RX_ID;
    }

    // Read the can config1. If it is not set, i.e. 0xff,
    canConfig1 = g_bootloaderContext.propertyInterface->store->configurationData.canConfig1;

    // check baud rate config
    if ((canConfig1 & MSCAN_SPEED_MASK) == 0x0f)
    {
        // not specified, go with auto detection feature enabled
        s_mscanInfo.baudrateDetect = true;

        // select default baud rate 1M
        s_mscanInfo.baudrate = MSCAN_MAX_SPEED - 1;
    }
    else
    {
        // baud rate config specified
        s_mscanInfo.baudrateDetect = false;

        if (canConfig1 & MSCAN_SPEED_SPEC_MASK)
        {
            // specified baud rate setting directly
            s_mscanInfo.baudrate = MSCAN_MAX_SPEED;

            // get config data from BCA area
            canConfig2 = g_bootloaderContext.propertyInterface->store->configurationData.canConfig2;

            // prepare specified config data
            bit_rate_table1.priDiv = (canConfig2 & MSCAN_PRESCALER_MASK) >> MSCAN_PRESCALER_SHIFT;
            bit_rate_table1.timeSeg1 = (canConfig2 & MSCAN_PSEG1_MASK) >> MSCAN_PSEG1_SHIFT;
            bit_rate_table1.timeSeg2 = (canConfig2 & MSCAN_PSEG2_MASK) >> MSCAN_PSEG2_SHIFT;
            bit_rate_table1.sJumpwidth = canConfig2 & MSCAN_RJW_MASK;
            bit_rate_table1.samp = (canConfig1 & MSCAN_SAMPLE_MASK) >> MSCAN_SAMPLE_SHIFT;
        }
        else
        {
            // specified speed index, baud rate settings from bootloader
            s_mscanInfo.baudrate = canConfig1 & MSCAN_SPEED_INDEX_MASK;

            if (s_mscanInfo.baudrate >= MSCAN_MAX_SPEED)
            {
                s_mscanInfo.baudrate = MSCAN_MAX_SPEED - 1;
            }
        }
    }

    s_mscanInfo.data_sink = mscan_initial_data_sink;

    switch (s_mscanInfo.baudrate)
    {
        case 0:
            config.baudRate = 125000;
            break;
        case 1:
            config.baudRate = 256000;
            break;
        case 2:
            config.baudRate = 500000;
            break;
        case 3:
        case 4:
        default:
            config.baudRate = 1000000;
            break;
    }

    /* Init the interrupt sync object.*/
    OSA_SemaCreate(&s_mscanInfo.state.txIrqSync, 0);
    OSA_SemaCreate(&s_mscanInfo.state.rxIrqSync, 0);

    config.enableTimer = true;

    /* Acceptance filter configuration. */
    config.filterConfig.u32IDAR0 = MSCAN_RX_MB_STD_MASK(s_mscanInfo.rxId);
    config.filterConfig.u32IDAR1 = MSCAN_RX_MB_STD_MASK(s_mscanInfo.rxId);
    /* To receive standard identifiers in 32-bit filter mode, program the last three bits ([2:0]) in the mask
        registers CANIDMR1 and CANIDMR5 to don't care. */
    config.filterConfig.u32IDMR0 = 0x00070000;
    config.filterConfig.u32IDMR1 = 0x00070000;
    /* Initialize MSCAN module. */
    MSCAN_Init((MSCAN_Type *)baseAddr, &config, CLOCK_GetFreq(kCLOCK_ScgSysOscAsyncDiv2Clk));

//    MSCAN_EnableInterrupts((MSCAN_Type *)baseAddr, MSCAN_CANRIER_CSCIE_MASK);

    if (s_mscanInfo.baudrate == MSCAN_MAX_SPEED)
    {
        // specified baud rate settings directly, need to get other config data
        s_mscanInfo.baudrate = MSCAN_MAX_SPEED - 1;

        // using provided settings
        MSCAN_SetTimingConfig((MSCAN_Type *)baseAddr, &bit_rate_table1);
    }
    else
    {
        // using setting table
        MSCAN_SetTimingConfig((MSCAN_Type *)baseAddr, &bit_rate_table[s_mscanInfo.baudrate]);
    }
       
    /* Create MSCAN handle structure and set call back function. */
    MSCAN_TransferCreateHandle((MSCAN_Type *)baseAddr, &mscanHandle, mscan_callback, NULL);
    
    MSCAN_EnableRxInterrupts((MSCAN_Type *)baseAddr, kMSCAN_RxFullInterruptEnable | kMSCAN_RxStatusChangeInterruptEnable);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : mscan_full_init
 * Description   : full init function
 *
 *END**************************************************************************/
status_t mscan_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function)
{
    s_mscan_app_data_sink_callback = function;
    //CLOCK_EnableClock(kCLOCK_PortC);
    // Configure selected pin as flexcan peripheral interface
    self->pinmuxConfig(self->instance, kPinmuxType_Peripheral);

    mscan_peripheral_init(self->instance);

    s_mscanIntialized[self->instance] = true;
    return kStatus_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : mscan_full_shutdown
 * Description   : full shutdown function
 *
 *END**************************************************************************/
void mscan_full_shutdown(const peripheral_descriptor_t *self)
{
    if (s_mscanIntialized[self->instance])
    {
        uint32_t baseAddr = g_mscanBaseAddr[self->instance];
        MSCAN_Deinit((MSCAN_Type *)baseAddr);

        // Restore selected pin to default state to reduce IDD.
        self->pinmuxConfig(self->instance, kPinmuxType_Default);

        s_mscanIntialized[self->instance] = false;
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : mscan_write
 * Description   : Send data function
 *
 *END**************************************************************************/
status_t mscan_write(const peripheral_descriptor_t *self, const uint8_t *buffer, uint32_t byteCount)
{
    uint32_t sentCnt = 0;
    uint8_t *sendPtr = (uint8_t *)buffer;

    while (sentCnt < byteCount)
    {
        if ((byteCount - sentCnt) <= 8)
        {
            s_mscanInfo.tx_info.data_length = byteCount - sentCnt; // remain number of bytes to be sent
            sentCnt += byteCount - sentCnt;
        }
        else
        {
            s_mscanInfo.tx_info.data_length = 8; // number of bytes to be sent
            sentCnt += 8;
        }

        MSCAN_Send(self->instance, 0, &s_mscanInfo.tx_info, s_mscanInfo.txId, (uint8_t *)sendPtr,
                     10);
        sendPtr += s_mscanInfo.tx_info.data_length;
    }

    return kStatus_Success;
}

status_t MSCAN_Send(uint8_t instance,
                      uint32_t mb_idx,
                      mscan_data_info_t *tx_info,
                      uint32_t msg_id,
                      uint8_t *mb_data,
                      uint32_t timeout_ms)
{
    MSCAN_Type *baseAddr = (MSCAN_Type *)g_mscanBaseAddr[instance];
    status_t status;
    uint8_t i;
    osa_status_t syncStatus;
    mscan_frame_t frame;
    frame.format = kMSCAN_FrameFormatStandard;
    frame.DLR = tx_info->data_length;
    frame.ID_Type.ID = msg_id;
    frame.type = kMSCAN_FrameTypeData;
    /* Copy user's buffer into the message buffer data area*/
    if (mb_data != NULL)
    {
        frame.dataWord0 = 0x0;
        frame.dataWord1 = 0x0;
        for (i = 0; i < tx_info->data_length; i++)
        {
            uint32_t temp, temp1;
            temp1 = (*(mb_data + i));
            if (i < 4)
            {
                temp = temp1 << (i * 8);
                frame.dataWord0 |= temp;
            }
            else
            {
                temp = temp1 << ((i - 4) * 8);
                frame.dataWord1 |= temp;
            }
        }
    }

    status = MSCAN_TransferSendBlocking((MSCAN_Type *)baseAddr, &frame);

    do
    {
       syncStatus = OSA_SemaWait(&s_mscanInfo.state.txIrqSync, timeout_ms);
       
    } while (syncStatus == kStatus_OSA_Idle);
    MSCAN_EnableRxInterrupts((MSCAN_Type *)baseAddr, kMSCAN_RxFullInterruptEnable);  
     
    return status;

}

/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_IRQBusoffHandler
 * Description   : Busoff interrupt handler
 *
 *END**************************************************************************/
void MSCAN_IRQBusoffHandler(uint8_t instance)
{
    MSCAN_Type *baseAddr = (MSCAN_Type *)g_mscanBaseAddr[instance];
    MSCAN_ClearRxFlag(baseAddr,MSCAN_CANRFLG_OVRIF_MASK | MSCAN_CANRFLG_CSCIF_MASK);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : MSCAN_IRQErrorHandler
 * Description   : General error interrupt handler including auto baud rate
 *                 detection processing
 *
 *END**************************************************************************/
void MSCAN_IRQErrorHandler(uint8_t instance)
{
    MSCAN_Type *baseAddr = (MSCAN_Type *)g_mscanBaseAddr[instance];
    
    if (s_mscanInfo.baudrateDetect)
    {    

      s_mscanInfo.baudrate--;

      if (s_mscanInfo.baudrate < 0)
      {
          s_mscanInfo.baudrate = MSCAN_MAX_SPEED - 1;
      }

      MSCAN_SetTimingConfig(baseAddr, &bit_rate_table[s_mscanInfo.baudrate]);           

      MSCAN_ClearRxFlag(baseAddr,MSCAN_CANRFLG_OVRIF_MASK | MSCAN_CANRFLG_CSCIF_MASK);
      //MSCAN_ClearRxInerruptFlag(baseAddr, MSCAN_CANRIER_OVRIE_MASK |
      //MSCAN_CANRIER_RSTATE_MASK | MSCAN_CANRIER_CSCIE_MASK);
      MSCAN_ClearRxInerruptFlag(baseAddr, MSCAN_CANRIER_RXFIE_MASK);
      MSCAN_ClearRxFlag(baseAddr, MSCAN_CANRFLG_RXF_MASK);
    }
    else
    {
      mscan_peripheral_init(instance);
    }

}

void MSCAN_Rx_IRQHandler(void)
{
    MSCAN_Type *baseAddr = (MSCAN_Type *)g_mscanBaseAddr[0];
    status_t status;
    osa_status_t syncStatus;
    /* Get current State of Message Buffer. */
    while (MSCAN_GetRxBufferFullFlag(baseAddr))
    {

             OSA_SemaPost(&s_mscanInfo.state.rxIrqSync);
             uint8_t i;
             uint8_t sink_byte = 0;
             mscan_frame_t rxFrame;
            /* Solve Data Frame. */
             status = MSCAN_ReadRxMb(baseAddr, &rxFrame);
             if (kStatus_Success == status)
             {
                  for (i = 0; i < rxFrame.DLR; i++)
                {
                    switch (i)
                    {
                        case 0:
                            sink_byte = rxFrame.dataByte0;
                            break;
                        case 1:
                            sink_byte = rxFrame.dataByte1;
                            break;
                        case 2:
                            sink_byte = rxFrame.dataByte2;
                            break;
                        case 3:
                            sink_byte = rxFrame.dataByte3;
                            break;
                        case 4:
                            sink_byte = rxFrame.dataByte4;
                            break;
                        case 5:
                            sink_byte = rxFrame.dataByte5;
                            break;
                        case 6:
                            sink_byte = rxFrame.dataByte6;
                            break;
                        case 7:
                            sink_byte = rxFrame.dataByte7;
                            break;
                        default:
                            break;
                    }
                    s_mscanInfo.data_sink(sink_byte, 0);
                }  
             }
            // OSA_SemaPost(&s_mscanInfo.state.rxIrqSync);
             MSCAN_TransferAbortReceive(baseAddr, &mscanHandle, kMSCAN_RxFullInterruptEnable);
        
        MSCAN_ClearRxBufferFullFlag(baseAddr);
        do
        {
           syncStatus = OSA_SemaWait(&s_mscanInfo.state.txIrqSync, 10);
           
        } while (syncStatus == kStatus_OSA_Idle);

    }

    return;
}

void MSCAN_ORed_IRQHandler(void)
{
    MSCAN_Type *base = (MSCAN_Type *)g_mscanBaseAddr[0];
    status_t status = kStatus_MSCAN_UnHandled;
    osa_status_t syncStatus;
    uint8_t Rxflag1 = MSCAN_GetRxFlag(base);
    uint8_t Rxflag2 = MSCAN_GetRxInerruptFlag(base);
    if ((Rxflag1 & (MSCAN_CANRFLG_OVRIF_MASK | MSCAN_CANRFLG_RSTAT_MASK | MSCAN_CANRFLG_CSCIF_MASK)) &&
        (Rxflag2 & (MSCAN_CANRIER_OVRIE_MASK | MSCAN_CANRIER_RSTATE_MASK)))
    {
      MSCAN_IRQErrorHandler(0);
    }
  
    /* Get current State of Message Buffer. */
    if (MSCAN_GetRxBufferFullFlag(base))
    {
        switch (mscanHandle.mbStateRx)
        {
            /* Solve Rx Data Frame. */
            case kMSCAN_StateRxData:
                status = MSCAN_ReadRxMb(base, mscanHandle.mbFrameBuf);
                if (kStatus_Success == status)
                {
                    status = kStatus_MSCAN_RxIdle;
                }
                MSCAN_TransferAbortReceive(base, &mscanHandle, kMSCAN_RxFullInterruptEnable);
                break;

            /* Solve Rx Remote Frame. */
            case kMSCAN_StateRxRemote:
                status = MSCAN_ReadRxMb(base, mscanHandle.mbFrameBuf);
                if (kStatus_Success == status)
                {
                    status = kStatus_MSCAN_RxIdle;
                }
                MSCAN_TransferAbortReceive(base, &mscanHandle, kMSCAN_RxFullInterruptEnable);
                break;
        }
        MSCAN_ClearRxBufferFullFlag(base);
    }
    else
    {
        switch (mscanHandle.mbStateTx)
        {
            /* Solve Tx Data Frame. */
            case kMSCAN_StateTxData:
                status = kStatus_MSCAN_TxIdle;
                MSCAN_TransferAbortSend(base, &mscanHandle, kMSCAN_TxEmptyInterruptEnable);
                break;

            /* Solve Tx Remote Frame. */
            case kMSCAN_StateTxRemote:
                mscanHandle.mbStateRx = kMSCAN_StateRxRemote;
                status = kStatus_MSCAN_TxSwitchToRx;
                break;

            default:
                status = kStatus_MSCAN_UnHandled;
                break;
        }
    }

    mscanHandle.callback(base, &mscanHandle, status, mscanHandle.userData);
  return;
}



//! @}

#endif // BL_CONFIG_CAN

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
