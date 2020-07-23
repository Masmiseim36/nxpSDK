/*
 * Copyright (c) 2016-2019, NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader/bl_context.h"
#include "bootloader_common.h"
#include "bootloader_config.h"
#include "bootloader/bl_peripheral_interface.h"
#include "bootloader/bl_irq_common.h"
#include "autobaud/autobaud.h"
#include "packet/serial_packet.h"
#include "fsl_device_registers.h"
#include "utilities/fsl_assert.h"

#include "fsl_lin.h"
#include "lin_cfg.h"
#include "fsl_gpio.h"
#include "fsl_lin_lpuart.h"
#include "fsl_tpm.h"
#include "fsl_port.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SIM_LPUART_CLK_SEL_MCGFLLCLK_CLK 1U /*!< LPUART clock select: MCGFLLCLK clock */
#define SIM_TPM_CLK_SEL_MCGFLLCLK_CLK 1U    /*!< TPM clock select: MCGFLLCLK clock */
#define LIN_CLOCK_NAME kCLOCK_McgFllClk
#define TJA_WAKEUP 1u
#define TIMER_TPM 1u

#define DEMO_TPM_BASEADDR TPM0
#define DEMO_TPM_IRQn TPM0_IRQn
#define DEMO_TPM_IRQHandler TPM0_IRQHandler
#define DEMO_TPM_CH_OUT_NUM 0u
#define DEMO_TPM_CH_OUT kTPM_Chnl_0
#define DEMO_TPM_CH_IN kTPM_Chnl_2
#define DEMO_TPM_CH_OUT_FLG kTPM_Chnl0Flag
#define DEMO_TPM_CH_IN_FLG kTPM_Chnl2Flag
#define DEMO_TPM_CH_OUT_IRQ kTPM_Chnl0InterruptEnable
#define DEMO_TPM_CH_IN_IRQ kTPM_Chnl2InterruptEnable
/* timer frequency */
#define TIMER_FREQ CLOCK_GetFreq(kCLOCK_McgFllClk)
/* (timer period (us) * (timer clock frequency)(Hz)) - 1 ) */
#define MODULO_VALUE ((500U * (CLOCK_GetFreq(kCLOCK_McgFllClk) / 1000000U)) - 1U)
/* nanoseconds / timer clock frequency  */
#define TIMER_1TICK_DURATION_PS (1000000000000U / TIMER_FREQ)

#define DEMO_LIN_IRQn LPUART0_LPUART1_IRQn

#define DEMO_SLPN_GPIO GPIOC
#define DEMO_SLPN_PIN 5u
#define LI0_Slave 0x01u

#define SLAVE_INSTANCE LI0_Slave
#define HARDWARE_INSTANCE 0U

/* auto-baudrate feature requires use of very high interrupt priority for LPUART and timer */
#define DEMO_LIN_PRIO 1U
#define DEMO_TIMER_PRIO (DEMO_LIN_PRIO + 1U)

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern uint16_t g_timerOverflowCount;
uint16_t g_timerOverflowCount = 0U;
extern volatile uint32_t g_captureValue;
volatile uint32_t g_captureValue = 0U;
extern bool     g_linlpuart1_active;
bool     g_linlpuart1_active = false;

extern lin_user_config_t linUserConfigSlave;
lin_user_config_t linUserConfigSlave = {0u};
extern lin_state_t linCurrentState;
lin_state_t linCurrentState = {0u};

#define DATA_BUFFER_MAX 256u
#define DATA_BUFFER_INDEX_MASK (0xffu)

extern char g_data[DATA_BUFFER_MAX];
char g_data[DATA_BUFFER_MAX];
extern int32_t g_data_read_index;
int32_t g_data_read_index = -1;
extern int32_t g_data_write_index;
int32_t g_data_write_index = 0;
extern int32_t g_data_len;
int32_t g_data_len = 0;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * This timer returns the count of nanoseconds between two consequencing bits.
 * @param ns number of nanoseconds between two bits, return parameter
 */
void timerGetTimeIntervalCallback0(uint32_t *ns);
/*!
 * This function handles messages from low level
 * @param instance LIN instance
 * @param linState pointer to current LIN state
 */
void CallbackHandler(uint32_t instance, void *linState);
/*!
 * This function initializes master node and low level
 * @param linUserConfig pointer to user configuration structure
 * @param linCurrentState pointer to LIN low level state structure
 * @param clockSource LIN clock frequency
 */
lin_status_t SlaveInit(lin_user_config_t *linUserConfig, lin_state_t *plinCurrentState, uint32_t clockSource);
/*!
 * This function calculates register values for supported baudrates
 * @param instance LIN instance
 * @param clocksSource LIN clock frequency
 * @param baudRatesVals pointer to baudrate values
 */
void CalculateBaudrates(uint32_t instance, uint32_t clocksSource, lin_baudrate_values_t *baudRatesVals);
/*!
 * This function process a frame id. If node is publisher, response is sent. If node is subscriber, response is awaited.
 * @param instance LIN instance
 * @param id frame ID
 */
void SlaveProcessId(uint8_t instance, uint8_t id);
/*!
 * This function set bus activity timeout
 * @param instance LIN instance
 */
void SlaveTimeoutService(uint8_t instance);
/*!
 * This function handle error raised by low-level
 * @param instance LIN instance
 * @param event_id ID of LIN bus event
 * @param id frame ID
 */
void SlaveHandleError(uint8_t instance, lin_event_id_t event_id, uint8_t id);
/*!
 * Updates signal, status, and flag after response is sent.
 * @param instance LIN instance
 * @param id frame ID
 */
void SlaveUpdateTx(uint8_t instance, uint8_t id);
/*!
 * This function update signal, status, and flag after response is received.
 * @param instance LIN instance
 * @param id frame ID
 */
void SlaveUpdateRx(uint8_t instance, uint8_t id);
/*!
 * This function makes or updates unconditional frames.
 * @param instance LIN instance
 * @param id frame ID
 * @param type type of action
 */
void SlaveProcesUncdFrame(uint8_t instance, uint8_t id, uint8_t type);
/*!
 * This function returns index of array ID from RAM table.
 * @param instance LIN instance
 * @param id frame ID
 */
static inline uint8_t SlaveGetFrameIndex(uint8_t instance, uint8_t id);
/*!
 * This function returns index to baudrate values array by given baudrate
 * @param baudrate LIN baudrate
 */
static inline uint32_t CheckIndex(uint32_t baudrate);
static bool linlpuart_poll_for_activity(const peripheral_descriptor_t *self);
static status_t linlpuart_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function);
static void linlpuart_full_shutdown(const peripheral_descriptor_t *self);
static status_t linlpuart_write(const peripheral_descriptor_t *self,  const uint8_t *buffer, uint32_t byteCount);

void BOARD_InitLinLpuart(void);
void BOARD_InitPins(void);
void DEMO_TPM_IRQHandler(void);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

const peripheral_control_interface_t g_linlpuartControlInterface = {
    .pollForActivity = linlpuart_poll_for_activity, .init = linlpuart_full_init, .shutdown = linlpuart_full_shutdown, .pump = 0
};

const peripheral_byte_inteface_t g_linlpuartByteInterface = {.init = NULL, .write = linlpuart_write };

static serial_byte_receive_func_t s_linlpuart_byte_receive_callback;
static bool g_linlpuartInitStatus[FSL_FEATURE_SOC_LPUART_COUNT] = { (_Bool)false }; // not initialized.
static const uint32_t g_linlpuartBaseAddr[] = LPUART_BASE_ADDRS;

/* storage for timer counter */
static uint64_t timerCounterValue[2] = {0u};
/* number of timer overflow */
static volatile uint32_t timerOverflowInterruptCount = 0u;
/* buffer handling messages between lower and higher level communication */
static uint8_t g_linResponseBuffer[LIN_NUM_OF_IFCS][32];
/* supported baudrate for autobaudrate feature */
static uint32_t baudRateArray[LIN_NUM_OF_SUPP_BAUDRATES] = {2400u, 4800u, 9600u, 14400u, 19200u};
static lin_baudrate_values_t g_baudRatesValues[LIN_NUM_OF_SUPP_BAUDRATES];
/* maximal header duration time */
static volatile uint16_t linMaxHeaderTimeoutVal[LIN_NUM_OF_IFCS];
/* maximal response duration time */
static uint16_t linMaxResTimeoutVal[LIN_NUM_OF_IFCS];

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
static inline uint32_t CheckIndex(uint32_t baudrate)
{
    uint32_t retval;
    switch (baudrate)
    {
        case 2400u:
            retval = (uint32_t)kLIN_BAUD_2400;
            break;
        case 4800u:
            retval = (uint32_t)kLIN_BAUD_4800;
            break;
        case 9600u:
            retval = (uint32_t)kLIN_BAUD_9600;
            break;
        case 14400u:
            retval = (uint32_t)kLIN_BAUD_14400;
            break;
        case 19200u:
            retval = (uint32_t)kLIN_BAUD_19200;
            break;
        default:
            retval = 0xFFu;
            break;
    }
    return retval;
}

static inline uint8_t SlaveGetFrameIndex(uint8_t instance, uint8_t id)
{
    uint8_t retVal = 0xFFu;
    uint8_t i;

    const lin_protocol_user_config_t *prot_user_config_ptr = &g_lin_protocol_user_cfg_array[HARDWARE_INSTANCE];

    for (i = prot_user_config_ptr->number_of_configurable_frames; i > 0U; i--)
    {
        if (prot_user_config_ptr->list_identifiers_RAM_ptr[i] == id)
        {
            retVal = i + prot_user_config_ptr->frame_start - 1U;
            break;
        }
    }
    return retVal;
}

/*!
 * This interrupt routine handles LIN bus low level communication
 */
void DEMO_LIN_IRQHandler(void)
{
    LIN_IRQHandler(SLAVE_INSTANCE);
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

#if (defined(TIMER_TPM) && TIMER_TPM)
/*!
 * This interrupt checks for communication timeout and falling edge detection.
 * This interrupt is also invoked every 500us on timer overflow.
 */
void DEMO_TPM_IRQHandler(void)
{
    uint32_t counterVal;
    if ((TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & (uint32_t)DEMO_TPM_CH_IN_FLG) != 0u)
    {
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, (uint32_t)DEMO_TPM_CH_IN_FLG);
        if (LIN_AutoBaudCapture(SLAVE_INSTANCE) == LIN_SUCCESS)
        {
            TPM_DisableInterrupts(DEMO_TPM_BASEADDR, (uint32_t)DEMO_TPM_CH_IN_FLG);
        }
    }

    if ((TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & (uint32_t)DEMO_TPM_CH_OUT_FLG) != 0u)
    {
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, (uint32_t)DEMO_TPM_CH_OUT_FLG);
        counterVal                                             = (uint16_t)(DEMO_TPM_BASEADDR->CNT);
        (DEMO_TPM_BASEADDR->CONTROLS[DEMO_TPM_CH_OUT_NUM].CnV) = counterVal + MODULO_VALUE;
        SlaveTimeoutService(SLAVE_INSTANCE);
    }

    if ((TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & (uint32_t)kTPM_TimeOverflowFlag) != 0u)
    {
        /* Clear interrupt flag.*/
        TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, (uint32_t)kTPM_TimeOverflowFlag);
        /* increase number of overflow count */
        timerOverflowInterruptCount++;
    }

/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif

#if defined(TIMER_TPM) && TIMER_TPM
void timerGetTimeIntervalCallback0(uint32_t *ns)
{
    uint64_t counterMaxValue      = 0xFFFFU;
    uint64_t currentOverflowCount = timerOverflowInterruptCount;

    /* check current CNT value */
    timerCounterValue[1] = (uint64_t)DEMO_TPM_BASEADDR->CNT;
    /* calculate number of ns from current and previous count value */
    if (timerCounterValue[1] >= timerCounterValue[0])
    {
        /* Correction: Timer overflow interrupt should be delayed by other processes
         * if TOF is set, timer overflow occurred so increase the number of interrupt and clear a flag
         * */
        if ((TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & (uint32_t)kTPM_TimeOverflowFlag) != 0u)
        {
            TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, (uint32_t)kTPM_TimeOverflowFlag);
            currentOverflowCount++;
        }
        *ns = (uint32_t)((((timerCounterValue[1] - timerCounterValue[0]) * TIMER_1TICK_DURATION_PS) / 1000U) +
                         (currentOverflowCount * counterMaxValue));
    }
    else /* (timerCounterValue[1]<timerCounterValue[0]) */
    {
        *ns = ((uint32_t)(((counterMaxValue - timerCounterValue[0] + timerCounterValue[1])) * TIMER_1TICK_DURATION_PS) /
               1000U);
        if ((TPM_GetStatusFlags(DEMO_TPM_BASEADDR) & (uint32_t)kTPM_TimeOverflowFlag) != 0u)
        {
            TPM_ClearStatusFlags(DEMO_TPM_BASEADDR, (uint32_t)kTPM_TimeOverflowFlag);
            currentOverflowCount++;
        }
        if (currentOverflowCount > 0U)
        {
            *ns += (uint32_t)((currentOverflowCount - 1U) * counterMaxValue);
        }
    }

    /* set current count value to previous count value */
    timerCounterValue[0] = timerCounterValue[1];
    /* clear timerOverflowInterruptCount mark */
    timerOverflowInterruptCount = 0u;
}
#endif

void CallbackHandler(uint32_t instance, void *linState)
{
    lin_state_t *plinCurrentState     = (lin_state_t *)linState;
    lin_user_config_t *linUserConfig = g_linUserconfigPtr[instance];
    uint8_t bytesRemaining = 0u;

    const lin_protocol_user_config_t *prot_user_config_ptr = &g_lin_protocol_user_cfg_array[HARDWARE_INSTANCE];
    lin_protocol_state_t *prot_state_ptr                   = &g_lin_protocol_state_array[HARDWARE_INSTANCE];
    uint32_t index;

    /* check timeout flag */
    if (plinCurrentState->timeoutCounterFlag == (bool)1U)
    {
        /* set timeout error event id */
        plinCurrentState->currentEventId = LIN_TIMEOUT_ERROR;
    }

    /* check event id */
    switch (plinCurrentState->currentEventId)
    {
        case LIN_PID_OK:
            /* process PID of frame */
            SlaveProcessId((uint8_t)instance, plinCurrentState->currentId);
            break;
        case LIN_RECV_BREAK_FIELD_OK:
            /* reload frame timeout */
            prot_state_ptr->frame_timeout_cnt =
                linMaxResTimeoutVal[HARDWARE_INSTANCE] + linMaxHeaderTimeoutVal[HARDWARE_INSTANCE];
            break;
        case LIN_TX_COMPLETED:
            /* update protocol state */
            SlaveUpdateTx((uint8_t)instance, plinCurrentState->currentId);
            break;
        case LIN_RX_COMPLETED:
            /* update protocol state */
            SlaveUpdateRx((uint8_t)instance, plinCurrentState->currentId);
            break;
        case LIN_BUS_ACTIVITY_TIMEOUT:
        case LIN_TIMEOUT_ERROR:
            /* check for remaining bytes */
            (void)LIN_GetReceiveStatus(instance, &bytesRemaining);
            if (plinCurrentState->rxSize > bytesRemaining)
            {
                /* report timeout to higher level */
                SlaveHandleError((uint8_t)instance, LIN_NO_DATA_TIMEOUT, plinCurrentState->currentId);
            }
            break;
        case LIN_PID_ERROR:
        case LIN_FRAME_ERROR:
        case LIN_CHECKSUM_ERROR:
        case LIN_READBACK_ERROR:
        case LIN_SYNC_ERROR:
            /* report error to higher level */
            SlaveHandleError((uint8_t)instance, plinCurrentState->currentEventId, plinCurrentState->currentId);
            break;
        case LIN_BAUDRATE_ADJUSTED:
            index = CheckIndex(linUserConfig->baudRate);
            if (index != 0xFFu)
            {
                LIN_SetBaudrate(SLAVE_INSTANCE, g_baudRatesValues[index].osrValue, g_baudRatesValues[index].sbrValue);
            }
            /* change protocol baudrate */
            prot_state_ptr->baud_rate = (uint16_t)linUserConfig->baudRate;
            /* update protocol timeout values */
            prot_state_ptr->frame_timeout_cnt = (uint16_t)LIN_CalcMaxResTimeoutCnt(prot_state_ptr->baud_rate, 8U);
            break;
        case LIN_WAKEUP_SIGNAL:
            g_linlpuart1_active = true;
            break;
        case LIN_NO_EVENT:
        case LIN_SYNC_OK:
        case LIN_NO_DATA_TIMEOUT:
        default:
            /* do nothing */
            break;
    }
    prot_state_ptr->idle_timeout_cnt = prot_user_config_ptr->max_idle_timeout_cnt;
}

lin_status_t SlaveInit(lin_user_config_t *linUserConfig, lin_state_t *plinCurrentState, uint32_t clockSource)
{
    lin_status_t status                                    = LIN_SUCCESS;
    const lin_protocol_user_config_t *prot_user_config_ptr = &g_lin_protocol_user_cfg_array[HARDWARE_INSTANCE];
    lin_protocol_state_t *prot_state_ptr                   = &g_lin_protocol_state_array[HARDWARE_INSTANCE];
    /* initialize protocol variables */

    if (clockSource == 0u)
    {
        /* with no clock report error */
        status = LIN_ERROR;
    }
    else
    {
        /* initialize LIN low lovel */
        status = LIN_Init(SLAVE_INSTANCE, linUserConfig, plinCurrentState, clockSource);

        /* make hardware instance visible to lower level */
        linUserConfig->hardware_instance = HARDWARE_INSTANCE;
        /* register time interval callback */
        linUserConfig->timerGetTimeIntervalCallback = &timerGetTimeIntervalCallback0;
        /* install callback service */
        (void)LIN_InstallCallback(SLAVE_INSTANCE, CallbackHandler);

        prot_state_ptr->baud_rate           = (uint16_t)linUserConfig->baudRate;
        prot_state_ptr->response_buffer_ptr = g_linResponseBuffer[HARDWARE_INSTANCE];
        prot_state_ptr->idle_timeout_cnt    = prot_user_config_ptr->max_idle_timeout_cnt;

        if (linUserConfig->autobaudEnable == true)
        {
            /* in case of auto-baudrate feature */
            linMaxResTimeoutVal[HARDWARE_INSTANCE]    = (uint16_t)LIN_CalcMaxResTimeoutCnt(2400U, 8U);
            prot_state_ptr->frame_timeout_cnt         = linMaxResTimeoutVal[HARDWARE_INSTANCE];
            linMaxHeaderTimeoutVal[HARDWARE_INSTANCE] = (uint16_t)LIN_CalcMaxHeaderTimeoutCnt(2400U);
            /* Set the highest priority */
            NVIC_SetPriority(DEMO_LIN_IRQn, DEMO_LIN_PRIO);
        }
        else
        {
            /* if auto-baudrate feature is used, enhance timeout for sync field */
            linMaxResTimeoutVal[HARDWARE_INSTANCE]    = (uint16_t)LIN_CalcMaxResTimeoutCnt(prot_state_ptr->baud_rate, 8U);
            prot_state_ptr->frame_timeout_cnt         = linMaxResTimeoutVal[HARDWARE_INSTANCE];
            linMaxHeaderTimeoutVal[HARDWARE_INSTANCE] = (uint16_t)LIN_CalcMaxHeaderTimeoutCnt(prot_state_ptr->baud_rate);
        }
        g_lpuartInitStatus[1] = true; // initialized.

    }
    return status;
}

void CalculateBaudrates(uint32_t instance, uint32_t clocksSource, lin_baudrate_values_t *baudRatesVals)
{
    uint8_t sbrt;
    uint32_t tempBaudrate;
    uint32_t *osr;
    uint16_t *sbr;

    /* calculate OSR and SBR register valus for all supported baudrates for autobaudrate feature */
    for (sbrt = 0u; sbrt < LIN_NUM_OF_SUPP_BAUDRATES; sbrt++)
    {
        baudRatesVals[sbrt].baudRate = baudRateArray[sbrt];
        tempBaudrate = baudRatesVals[sbrt].baudRate;
        sbr = &baudRatesVals[sbrt].sbrValue;
        osr = &baudRatesVals[sbrt].osrValue;
        LIN_CalculateBaudrate(instance, tempBaudrate, clocksSource, osr, sbr);
    }
}

void SlaveProcessId(uint8_t instance, uint8_t id)
{
    uint8_t frame_index;
    uint32_t response_length;
    uint32_t lin_max_frame_res_timeout_val;
    const lin_frame_struct *lin_frame_ptr;

    const lin_protocol_user_config_t *prot_user_config_ptr = &g_lin_protocol_user_cfg_array[HARDWARE_INSTANCE];
    lin_protocol_state_t *prot_state_ptr                   = &g_lin_protocol_state_array[HARDWARE_INSTANCE];

    /* get index of current frame from RAM table */
    frame_index = SlaveGetFrameIndex(instance, id);

    if (frame_index != 0xFFu)
    {
        prot_state_ptr->go_to_sleep_flg = (bool)0U;
        /* get frame buffer pointer */
        lin_frame_ptr = &(prot_user_config_ptr->frame_tbl_ptr[frame_index]);
        /* check if id represents a supported frame */
        if ((id > 0u) && (id < 0x3Bu))
        {
            response_length               = lin_frame_ptr->frm_len;
            lin_max_frame_res_timeout_val = LIN_CalcMaxResTimeoutCnt(prot_state_ptr->baud_rate, (uint8_t)response_length);

            /* check response type */
            if (LIN_RES_PUB == lin_frame_ptr->frm_response)
            {
                /* make unconditional frame */
                SlaveProcesUncdFrame(instance, id, LIN_MAKE_UNCONDITIONAL_FRAME);

                if ((response_length <= 8U) && (response_length > 0U))
                {
                    /* Set response */
                    (void)LIN_SetResponse(instance, &(prot_state_ptr->response_buffer_ptr[0]), (uint8_t)response_length,
                                    (uint8_t)lin_max_frame_res_timeout_val);
                }
            }
            else
            {
                if ((response_length <= 8U) && (response_length != 0U))
                {
                    /* wait for response */
                    (void)LIN_RxResponse(instance, &(prot_state_ptr->response_buffer_ptr[0]), (uint8_t)response_length,
                                   (uint8_t)lin_max_frame_res_timeout_val);
                }
            }
        }
        /* frame is not supported */
        else
        {
            /* ignore frame */
            (void)LIN_IgnoreResponse(instance);
        }
    }
    /* unknown id */
    else
    {
        /* ignore frame */
        (void)LIN_IgnoreResponse(instance);
    }
}

void SlaveProcesUncdFrame(uint8_t instance, uint8_t id, uint8_t type)
{
    uint8_t frame_index;
    uint8_t *response_buffer_ptr;
    uint8_t i;
    const lin_node_attribute *node_attr_ptr;

    const lin_protocol_user_config_t *prot_user_config_ptr = &g_lin_protocol_user_cfg_array[HARDWARE_INSTANCE];
    lin_protocol_state_t *prot_state_ptr                   = &g_lin_protocol_state_array[HARDWARE_INSTANCE];

    /* get index of current frame from RAM table */
    frame_index = SlaveGetFrameIndex(instance, id);

    /* get protocol response buffer */
    if (frame_index != 0xFFu)
    {
        response_buffer_ptr = prot_state_ptr->response_buffer_ptr;
        /* get protocol response buffer length */
        prot_state_ptr->response_length = prot_user_config_ptr->frame_tbl_ptr[frame_index].frm_len;

        /* check for frame type */
        if (LIN_MAKE_UNCONDITIONAL_FRAME == type)
        {
            node_attr_ptr = &g_lin_node_attribute_array[HARDWARE_INSTANCE];

            for (i = 0U; i < node_attr_ptr->num_frame_have_esignal; i++)
            {
                /* Check if frame contains error signal */
                if (id == node_attr_ptr->resp_err_frm_id_ptr[i])
                {
                    prot_state_ptr->transmit_error_resp_sig_flg = (bool)1U;
                    break;
                }
            }

            uint8_t cnt = 0u;

            for (i = 1u; i < prot_state_ptr->response_length; i++)
            {
               if (g_data_len > 0)
               {
                   cnt++;
                   g_data_read_index++;
                   if (g_data_read_index >= (int32_t)DATA_BUFFER_MAX)
                   {
                       g_data_read_index = 0;
                   }
                   response_buffer_ptr[i] = (uint8_t)g_data[g_data_read_index];
                   g_data_len --;
               }
               else
               {
                   response_buffer_ptr[i] = 0u;
               }
            }

            response_buffer_ptr[0] = (uint8_t)cnt;
        }
        /* update unconditional frame */
        else
        {
            uint8_t cnt = response_buffer_ptr[0];
            if (cnt > prot_state_ptr->response_length - 1u)
            {
                cnt = prot_state_ptr->response_length - 1u;
            }

            if (cnt > 0u)
            {
                for (i = 1U; i <= cnt; i++)
                {
                    s_linlpuart_byte_receive_callback(response_buffer_ptr[i]);
                }
            }
        }
    }
}
void SlaveUpdateTx(uint8_t instance, uint8_t id)
{
    lin_protocol_state_t *prot_state_ptr = &g_lin_protocol_state_array[HARDWARE_INSTANCE];

    /* Set successful transfer */
    prot_state_ptr->successful_transfer = 1U;

    if (prot_state_ptr->num_of_processed_frame < 0xFFU)
    {
        /* increase a number of processed frames */
        prot_state_ptr->num_of_processed_frame++;
    }
    /* check whether error signal was transmitted */
    if ((bool)1U == prot_state_ptr->transmit_error_resp_sig_flg)
    {
        /* Set error in response */
        prot_state_ptr->error_in_response = 0U;
        /* clear error flag */
        prot_state_ptr->transmit_error_resp_sig_flg = (bool)0U;
    }
    /* check for valid id of unconditional frame */
    if ((id > 0U) && (id < 0x3BU))
    {
        /* increase a number of succesfull frames */
        prot_state_ptr->num_of_successfull_frame++;
    }
}

void SlaveUpdateRx(uint8_t instance, uint8_t id)
{
    uint8_t frame_index, flag_offset, flag_size, i;
    const lin_protocol_user_config_t *prot_user_config_ptr = &g_lin_protocol_user_cfg_array[HARDWARE_INSTANCE];
    lin_protocol_state_t *prot_state_ptr                   = &g_lin_protocol_state_array[HARDWARE_INSTANCE];

    /* Set successful transfer */
    prot_state_ptr->successful_transfer = 1U;

    if (prot_state_ptr->num_of_processed_frame < 0xFFU)
    {
        /* increase a number of processed frames */
        prot_state_ptr->num_of_processed_frame++;
    }

    frame_index = SlaveGetFrameIndex(instance, id);
    if ((id > 0U) && (id < 0x3BU))
    {
        /* increase a number of succesfull frames */
        prot_state_ptr->num_of_successfull_frame++;
        /* make unconditional frame */
        SlaveProcesUncdFrame(instance, id, LIN_UPDATE_UNCONDITIONAL_FRAME);

        /* Update rx frame flags */
        flag_offset = prot_user_config_ptr->frame_tbl_ptr[frame_index].flag_offset;
        flag_size   = prot_user_config_ptr->frame_tbl_ptr[frame_index].flag_size;
        for (i = 0U; i < flag_size; i++)
        {
            g_lin_flag_handle_tbl[flag_offset] = 0xFFU;
            flag_offset++;
        }
    }
    else
    {
        ; /* misra compliance */
    }
}

void SlaveTimeoutService(uint8_t instance)
{
    lin_state_t *linState                                  = g_linStatePtr[SLAVE_INSTANCE];
    const lin_protocol_user_config_t *prot_user_config_ptr = &g_lin_protocol_user_cfg_array[HARDWARE_INSTANCE];
    lin_protocol_state_t *prot_state_ptr                   = &g_lin_protocol_state_array[HARDWARE_INSTANCE];

    /* check if timeout occurs during communication */
    LIN_TimeoutService(instance);

    /* check for current node state */
    switch (linState->currentNodeState)
    {
        case LIN_NODE_STATE_IDLE:
            /* check if idle timeout runs out */
            if (!(prot_state_ptr->idle_timeout_cnt-- > 0U))
            {
                /* Set go to sleep flag */
                prot_state_ptr->go_to_sleep_flg = (bool)1U;

                /* Put current node to Idle state, reset idle timeout count */
                prot_state_ptr->idle_timeout_cnt = prot_user_config_ptr->max_idle_timeout_cnt;

                /* Put current node to sleep mode */
                (void)LIN_GoToSleepMode(instance);
            }
            break;
        case LIN_NODE_STATE_RECV_PID:
        case LIN_NODE_STATE_SEND_PID:
        case LIN_NODE_STATE_RECV_SYNC:
            /* timeout send has occurred - change state of the node and inform core */
            if (!(prot_state_ptr->frame_timeout_cnt-- > 0U))
            {
                /* Go to idle state */
                (void)LIN_GotoIdleState(instance);

                /* Reset frame count timeout */
                prot_state_ptr->frame_timeout_cnt = linMaxResTimeoutVal[HARDWARE_INSTANCE];
            }
            break;
        case LIN_NODE_STATE_SEND_DATA:
        case LIN_NODE_STATE_SEND_DATA_COMPLETED:
            /* timeout send has occurred - change state of the node and inform core */
            if (!(prot_state_ptr->frame_timeout_cnt-- > 0U))
            {
                /* Abort frame data transferring */
                (void)LIN_AbortTransferData(instance);

                /* Reset frame count timeout */
                prot_state_ptr->frame_timeout_cnt = linMaxResTimeoutVal[HARDWARE_INSTANCE];
            }
            break;
        case LIN_NODE_STATE_UNINIT:
        case LIN_NODE_STATE_SLEEP_MODE:
        case LIN_NODE_STATE_SEND_BREAK_FIELD:
        case LIN_NODE_STATE_RECV_DATA:
        case LIN_NODE_STATE_RECV_DATA_COMPLETED:
        default:
            /* do nothing */
            break;
    }
}

void SlaveHandleError(uint8_t instance, lin_event_id_t event_id, uint8_t id)
{
    lin_protocol_state_t *prot_state_ptr = &g_lin_protocol_state_array[HARDWARE_INSTANCE];

    /* increase a number of processed frames */
    if (prot_state_ptr->num_of_processed_frame < 0xFFU)
    {
        prot_state_ptr->num_of_processed_frame++;
    }

    switch (event_id)
    {
        /* PID error */
        case LIN_PID_ERROR:
        case LIN_FRAME_ERROR:
        case LIN_CHECKSUM_ERROR:
        case LIN_READBACK_ERROR:
        case LIN_SYNC_ERROR:
            /* Set response error */
            prot_state_ptr->error_in_response += 1U;
            break;
        case LIN_NO_DATA_TIMEOUT:
            /* Set timeout error */
            prot_state_ptr->timeout_in_response += 1U;
            break;
        default:
            /* do nothing */
            break;
    }
}

/*!
 * This function will be called from the main peripheral detection loop to drive
 * the autobaud detect for this UART instance. If it is completed the UART is
 * fully initialized and ready to use.
 */
static bool linlpuart_poll_for_activity(const peripheral_descriptor_t *self)
{
    return (g_linlpuart1_active);
}

void BOARD_InitPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTB1 (pin 17) is configured as TPM0_CH2 */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt5);

    PORTC->PCR[1] = ((PORTC->PCR[1] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_SRE_MASK | PORT_PCR_ISF_MASK)))

                     /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is
                      * configured as a digital output. */
                     | PORT_PCR_SRE(kPORT_FastSlewRate)

                     /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | PORT_PCR_PS(kPORT_PullDown));
}

void BOARD_InitLinLpuart(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t gpioc_pin41_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC5 (pin 41)  */
    GPIO_PinInit(GPIOC, 5U, &gpioc_pin41_config);

    /* PORTA17 (pin 5) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAlt3);

    /* PORTA18 (pin 6) is configured as LPUART1_TX */
    PORT_SetPinMux(PORTA, 18U, kPORT_MuxAlt3);

    /* PORTC5 (pin 41) is configured as PTC5 */
    PORT_SetPinMux(PORTC, 5U, kPORT_MuxAsGpio);

    PORTC->PCR[5] = ((PORTC->PCR[5] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                     | (uint32_t)(PORT_PCR_PE_MASK));

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK)))

                  /* LPUART1 Transmit Data Source Select: LPUART1_TX pin. */
                  | SIM_SOPT5_LPUART1TXSRC(0)

                  /* LPUART1 Receive Data Source Select: LPUART1_RX pin. */
                  | SIM_SOPT5_LPUART1RXSRC(0));
}

//! Note that we don't ungate the LPUART clock gate here. That is done only after the
//! autobaud process has completed successfully.
static status_t linlpuart_full_init(const peripheral_descriptor_t *self, serial_byte_receive_func_t function)
{
    status_t retStatus = -1;

    s_linlpuart_byte_receive_callback = function;

    // Since we are using autobaud once the detection is completed
    // it will call the UART initialization and remux the pins when it completes
    self->pinmuxConfig(self->instance, kPinmuxType_PollForActivity);

    tpm_config_t tpmInfo;

    lin_status_t status;
    uint32_t linfreq;
    gpio_pin_config_t slpn_config = {
        kGPIO_DigitalOutput,
        1u,
    };
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_InitLinLpuart();
    /* Set LIN LPUART clock */
    CLOCK_SetLpuart1Clock(SIM_LPUART_CLK_SEL_MCGFLLCLK_CLK);
#if defined(TIMER_TPM) && TIMER_TPM
    /* Set Timer LPUART clock */
    CLOCK_SetTpmClock(SIM_TPM_CLK_SEL_MCGFLLCLK_CLK);
#endif

#if defined(TJA_WAKEUP) && TJA_WAKEUP
    /* Wakeup TJA transceiver */
    GPIO_PinInit(DEMO_SLPN_GPIO, DEMO_SLPN_PIN, &slpn_config);
#endif

    /*
     * .linUserConfigSlave.autobaudEnable = true;
     * .linUserConfigSlave.baudRate = 19200u;
     * .linUserConfigSlave.nodeFunction = SLAVE;
     * .linUserConfigSlave.timerGetTimeIntervalCallback = NULL;
     */
    LIN_GetSlaveDefaultConfig(&linUserConfigSlave);
    linUserConfigSlave.autobaudEnable = false;

    /* get LIN clock frequency */
    linfreq = CLOCK_GetFreq(LIN_CLOCK_NAME);

#if defined(TIMER_TPM) && TIMER_TPM
    TPM_GetDefaultConfig(&tpmInfo);
    tpmInfo.prescale = kTPM_Prescale_Divide_1;
    /* Initialize TPM module */
    TPM_Init(DEMO_TPM_BASEADDR, &tpmInfo);
    /* Set module value */
    DEMO_TPM_BASEADDR->MOD = 0xFFFFu;
    /* Setup TPM output compare mode */
    TPM_SetupOutputCompare(DEMO_TPM_BASEADDR, DEMO_TPM_CH_OUT, kTPM_NoOutputSignal, MODULO_VALUE);
    /* Setup TPM input capture mode - capture LPUART-RX falling edge */
    TPM_SetupInputCapture(DEMO_TPM_BASEADDR, DEMO_TPM_CH_IN, kTPM_RiseAndFallEdge);
    /* Enable interrupt on overflow */
    TPM_EnableInterrupts(DEMO_TPM_BASEADDR,
                         (uint32_t)((uint32_t)kTPM_TimeOverflowInterruptEnable | (uint32_t)DEMO_TPM_CH_OUT_IRQ | (uint32_t)DEMO_TPM_CH_IN_IRQ));
    /* Set the second highest priority */
    NVIC_SetPriority(DEMO_TPM_IRQn, DEMO_TIMER_PRIO);
    /* Enable at the NVIC */
    (void)EnableIRQ(DEMO_TPM_IRQn);
#endif

    if (TIMER_FREQ != 0U)
    {
        /* Initialize slave node */
        status = SlaveInit(&linUserConfigSlave, &linCurrentState, linfreq);

        if (status == LIN_SUCCESS)
        {
            if (linUserConfigSlave.autobaudEnable == true)
            {
                /* Prepare baudrate array for autobadrate mode */
                CalculateBaudrates(SLAVE_INSTANCE, linfreq, g_baudRatesValues);
            }

            /* Go to sleep until master starts communication */
            (void)LIN_GoToSleepMode(SLAVE_INSTANCE);
            if (linCurrentState.currentNodeState != LIN_NODE_STATE_SLEEP_MODE)
            {
            }
#if defined(TIMER_TPM) && TIMER_TPM
            TPM_StartTimer(DEMO_TPM_BASEADDR, kTPM_SystemClock);
#endif

            uint32_t i;
            for (i = 0u; i < DATA_BUFFER_MAX; i++)
            {
                g_data[i] = (char)0xff;
            }
            retStatus = (int32_t)kStatus_Success;
        }
    }

    return retStatus;
}

static void linlpuart_full_shutdown(const peripheral_descriptor_t *self)
{
    uint32_t instance;
    instance = 1u;
    if (g_linlpuartInitStatus[instance])
    {
        uint32_t baseAddr = g_linlpuartBaseAddr[instance];

        // Disable LPUART interrupt
        (void)LIN_LPUART_DisableIRQ((LPUART_Type *)baseAddr);

        // Reset LPUART registers
        (void)LIN_LPUART_Deinit((LPUART_Type *)baseAddr);
    }

//! Note: if not deinit autobaud(IRQ method), user app may encounters hardfault
//! if it doesn't provide related pin interrupt service routine.
#if defined(BL_FEATURE_UART_AUTOBAUD_IRQ) && BL_FEATURE_UART_AUTOBAUD_IRQ
    // De-init autobaud detector.
    autobaud_deinit(instance);
#endif

    {
        // Restore selected pin to default state to reduce IDD.
        self->pinmuxConfig(self->instance, kPinmuxType_Default);
    }
}

static status_t linlpuart_write(const peripheral_descriptor_t *self,  const uint8_t *buffer, uint32_t byteCount)
{
    uint32_t i;

    for (i = 0u; i < byteCount; i++)
    {
        while (g_data_len >= (int32_t)DATA_BUFFER_MAX)
        {
        };

        // not full yet
        __ASM("CPSID I");
        g_data[g_data_write_index] = (char)buffer[i];
        g_data_len++;
        g_data_write_index++;
        if (g_data_write_index >= (int32_t)DATA_BUFFER_MAX)
        {
            g_data_write_index = 0;
        }
        __ASM("CPSIE I");
    }

    return (int32_t)kStatus_Success;
}
//! @}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
