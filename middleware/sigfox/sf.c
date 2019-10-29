/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file sf.c
 *
 * SIGFOX driver based on AML layer supporting boards based on following NXP
 * parts: OL2361, OL2385.
 *
 * This module is common for all supported models.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "sf.h"
#include "spi_aml/spi_aml.h"
#include "gpio_aml.h"
#include "wait_aml/wait_aml.h"

/* Enables basic debugging messages printed to the serial console (content
 * of sent and received SPI frames). */
/* #define DEBUG_SPI_COM */
/* Enables debugging messages related to UL frequency calculation. */
/* #define DEBUG_FREQ_CALC */

#if ((defined(DEBUG_FREQ_CALC) || defined(DEBUG_SPI_COM)) && (SDK_VERSION == SDK_KSDK))
/* Debug messages (for Kinetis SDK only). */
#include "fsl_debug_console.h"
#endif

/*******************************************************************************
 * Defines
 ******************************************************************************/
/*! Timeout used to wait for a level of ACK pin. It is intended for SPI commands
 * which do not cause transmission to SIGFOX network (i.e. Send Echo,...). */
#define SF_ACK_NOTRANS_TIMEOUT_US       5000000U

/*! Timeout used to wait for a level of ACK pin. It is intended for SPI commands
 * causing transmission to SIGFOX network (Send Payload, Send Bit,
 * Send Out of Band, Send Test). The Receive Frame command has higher timeout.
 * Note: waiting for an acknowledge frame can take several second.
 * Max. time is needed for Send Test Mode command (test mode number 5, TxSynth). */
#define SF_ACK_TRANS_TIMEOUT_US         15000000U

/*! Timeout used to wait for a level of ACK pin. It is intended for the
 * Receive Frame SPI command. The SIGFOX firmware has an internal timeout (60 s). */
#define SF_ACK_TRANS_RECV_TIMEOUT_US    60000000U

/*! Timeout used to wait for a level of ACK pin. It is applied after the reset
 * in the SF_Init function. */
#define SF_ACK_INIT_TIMEOUT_US          5000000U

/*! Max. value of timeout used to wait for a level of ACK pin. */
#define SF_ACK_TIMEOUT_MAX_US           100000000U

/*! Timeout measurement is split into steps. This macro defines value of the
 * step in us. */
#define SF_TIMEOUT_STEP_US              100U

/*! Delay applied after the Send Wake up command. */
#define SF_WAKEUP_DLY_MS                100U

/*! The payload used for the "Send Echo" command. */
#define SF_ECHO_PLD_ARRAY               {0x01U, 0x02U, 0x03U, 0x04U, 0x05U}

/*!
 * @brief This macro returns true when an I-frame has payload (data except
 * length and command code fields).
 * Otherwise it returns false.
 *
 * @param cmd Command number (see sf_spi_cmd_t enumeration).
 */
#define SF_HAS_CMD_PLD(cmd) \
    (((cmd) == sfSpiCmdSendEcho) || ((cmd) == sfSpiCmdSendPayload) || \
     ((cmd) == sfSpiCmdSendBit) || ((cmd) == sfSpiCmdSetUlFreq) || \
     ((cmd) == sfSpiCmdContWave) || ((cmd) == sfSpiCmdSendTestMode) || \
     ((cmd) == sfSpiCmdPskAskZeroSeq) || ((cmd) == sfSpiCmdPskAskAAAASeq) || \
     ((cmd) == sfSpiCmdSetFCCMacroCh) || ((cmd) == sfSpiCmdStaticFreqCal) || \
     ((cmd) == sfSpiCmdTempFreqCal) || ((cmd) == sfSpiCmdReceive) || \
     ((cmd) == sfSpiCmdSetDlFreq))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if ((defined(DEBUG_FREQ_CALC) || defined(DEBUG_SPI_COM)) && (SDK_VERSION == SDK_KSDK))
/*
 * @brief Prints a frame to the serial console output.
 *
 * @param frame Pointer to the frame.
 * @param size  Size of the frame in bytes.
 */
static void SF_PrintFrame(const uint8_t *frame, uint8_t size);
#endif

/*
 * @brief Creates the SPI frame according to the defined format.
 *
 * @param cmd        The frame command number.
 * @param payloadLen Size of the frame payload in bytes.
 * @param payload    Payload of the frame.
 * @param frame      Pointer where the resulting frame is stored. Length
 * of the array must be equal to 2 + payload length (frame header).
 */
static void SF_PackFrame(sf_spi_cmd_t cmd, uint8_t payloadLen,
        const uint8_t *payload, uint8_t *frame);

/*
 * @brief Sends data via SPI, received data are ignored.
 *
 * @param drvData    Driver run-time data.
 * @param amlSpiData SPI transfer structure.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t SF_SendSpiData(sf_drv_data_t *drvData,
        spi_aml_transfer_t *amlSpiData);

/*
 * @brief Receives data via SPI. Note that it sends dummy data
 * (all bits equal to 1).
 *
 * @param drvData    Driver run-time data.
 * @param amlSpiData SPI transfer structure.
 * @param tmoutUs    Timeout in ms. It is applied when waiting for the low level
 *                   of ACK pin.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t SF_ReceiveSpiData(sf_drv_data_t *drvData,
        spi_aml_transfer_t *amlSpiData, uint32_t tmoutUs);

/*
 * @brief Waits until a GPIO pin has desired value.
 *
 * @param gpioPin   A GPIO configuration structure.
 * @param pinValExp Desired level of the pin (1 - log. 1, 0 - log. 0).
 * @param tmoutUs   Timeout in us.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t SF_WaitPinLevel(const sf_gpio_sel_t* gpioPin, uint32_t pinValExp,
        uint32_t tmoutUs);

/*
 * @brief Creates a SPI frame, sends a SPI command to the device,
 * receives an acknowledgement (if any) and stores an error code and state value
 * from the acknowledgement.
 *
 * @param drvData    Driver run-time data.
 * @param sendLen    Length of a frame to be sent (in bytes).
 * @param recvLen    Length of a frame to be received (in bytes).
 * @param sendBuffer Pointer to a SPI send buffer.
 * @param recvBuffer Pointer to a SPI receive buffer.
 * @param tmoutUs    Timeout in ms. It is applied when waiting for the low level
 *                   of ACK pin.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t SF_SendCommandInt(sf_drv_data_t *drvData,
        uint8_t sendLen, uint8_t recvLen,
        uint8_t *sendBuffer, uint8_t *recvBuffer, uint32_t tmoutUs);

/*
 * @breif Gets UL/DL frequency divider based on provided system clock divider.
 *
 * @param sysClkDiv OL2385 system clock divider.
 *
 * @return UL/DL frequency divider.
 */
static uint32_t SF_GetFreqDiv(uint32_t sysClkDiv);

/*
 * @brief Calculates AFC_MDES value from UL/DL frequency. It is used for
 * Automatic Amplitude and Frequency Calibration implemented by the OL2385 SW.
 *
 * @param freqHz UL/DL frequency in Hz.
 *
 * @return AFC_MDES.
 */
static uint32_t SF_CalcMdes(uint32_t freqHz);

/*
 * @brief Calculates values of SIGFOX device registers FCINT and FCFRAC.
 *
 * It is common for OL2361 and OL2385.
 *
 * @param freqHz   Up-link frequency in Hz.
 * @param fcint    Pointer where the integer divider parameter FCINT is stored.
 * @param fcfrac   Pointer where the fractional divider parameter FCFRAC is stored.
 * @param mdes     Pointer where the AFC_MDES parameter is stored.
 */
static void SF_CalcFreqRegs(uint32_t freqHz, uint32_t *fcint, uint32_t *fcfrac,
        uint32_t *mdes);

/*
 * @brief Calculates the up-link frequency with use of SIGFOX
 * device registers FREQCON0 and FREQCON1.
 *
 * @param fcint  Value of the integer divider parameter FCINT.
 * @param fcfrac Value of the fractional divider parameter FCFRAC.
 *
 * @return Up-link frequency in Hz.
 */
static uint32_t SF_CalcFreq(uint32_t fcint, uint32_t fcfrac);

/*!
 * @brief Fills the SPI frame with register values.
 *
 * @param fcint  Value of FCINT register.
 * @param fcfrac Value of FCFRAC register.
 * @param mdes   Value of AFC_MDES used for the Automatic Amplitude and
 * Frequency Calibration.
 * @param frame  Pointer to the SPI frame where the register values will be stored.
 */
static void SF_StoreRegs(uint32_t fcint, uint32_t fcfrac, uint32_t mdes, uint8_t *frame);

/*!
 * @brief Fills the FCFRAC and FCINT registers using the SPI frame.
 *
 * @param frame Pointer to the SPI frame.
 * @param fcint Pointer to the FCINT register.
 * @param fcfrac Pointer to the FCFRAC register.
 */
static void SF_RestoreRegs(const uint8_t *frame, uint32_t *fcint, uint32_t *fcfrac);

/*
 * @brief Stores an error code and state values from a received
 * acknowledgement.
 *
 * @param drvData   Driver run-time data.
 * @param spiRcvBuf Pointer to a SPI buffer containing received data.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t SF_StoreDevState(sf_drv_data_t *drvData, const uint8_t *spiRcvBuf);

/*
 * @brief Stores an error code and state values from a received
 * acknowledgement.
 *
 * @param spiRcvBuf    Pointer to a SPI buffer containing received data.
 * @param recvPayload  Pointer where a resulting payload is stored.
 * @param recvPayloadBufSize Size of the paylaod buffer in bytes.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t SF_StorePayload(const uint8_t *spiRcvBuf, sf_msg_payload_t *recvPayload,
        uint8_t recvPayloadBufSize);

/*!
 * @brief Sends and receives data via SPI and checks if an acknowledgement has
 * expected size.
 *
 * @param drvData    Driver run-time data.
 * @param sendLen    Length of a frame to be sent (in bytes).
 * @param recvLen    Length of a frame to be received (in bytes).
 * @param sendBuffer Pointer to a SPI send buffer.
 * @param recvBuffer Pointer to a SPI receive buffer.
 * @param tmoutUs    Timeout in ms. It is applied when waiting for the low level
 *                   of ACK pin.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t SF_SendAndCheck(sf_drv_data_t *drvData,
        uint8_t sendLen, uint8_t recvLen,
        uint8_t *sendBuffer, uint8_t *recvBuffer, uint32_t tmoutUs);

/*!
 * @brief Sets up-link or down-link frequency of OL2385.
 *
 * @param drvData    Driver run-time data.
 * @param cmd        SPI command code.
 * @param freqHz     Frequency to be set (in Hz).
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t SF_SetFrequency(sf_drv_data_t *drvData, sf_spi_cmd_t cmd,
        uint32_t freqHz);

/*!
 * @brief Gets up-link or down-link frequency of OL2385.
 *
 * @param drvData    Driver run-time data.
 * @param cmd        SPI command code.
 * @param freqHz     Pointer where the resulting frequency valuu in Hz is stored.
 *
 * @return Status result of the function (kStatus_Success on success).
 */
static status_t SF_GetFrequency(sf_drv_data_t *drvData, sf_spi_cmd_t cmd,
        uint32_t *freqHz);

/*******************************************************************************
 * Code - internal functions
 ******************************************************************************/

#if ((defined(DEBUG_FREQ_CALC) || defined(DEBUG_SPI_COM)) && (SDK_VERSION == SDK_KSDK))
static void SF_PrintFrame(const uint8_t *frame, uint8_t size)
{
    uint8_t i = 0U;

    PRINTF("0x");
    for (i = 0U; i < size; i++)
    {
        PRINTF("%02x ", *(frame + i));
    }
    PRINTF("\r\n");
}
#endif

static void SF_PackFrame(sf_spi_cmd_t cmd, uint8_t payloadLen,
        const uint8_t *payload, uint8_t *frame)
{
    /* Preconditions. */
    AML_ASSERT((cmd >= SF_CMD_FIRST_ID) && (cmd <= SF_CMD_LAST_ID));
    AML_ASSERT(payloadLen <= SF_INF_PAYLOAD_MAX_B);
    AML_ASSERT(frame != NULL);

    *(frame + SF_INF_LENGTH_OF) = payloadLen + SF_INF_HEADER_B;
    *(frame + SF_INF_CMD_OF) = (uint8_t)cmd;

    if ((payloadLen > 0U) && (payload != NULL))
    {   /* Store the payload. */
        memcpy((void*)(frame + SF_INF_PAYLOAD_OF),
                (void*)payload,
                (size_t)payloadLen);
    }
}

static status_t SF_SendSpiData(sf_drv_data_t *drvData,
        spi_aml_transfer_t *amlSpiData)
{
    status_t      status = kStatus_Fail;
    sf_gpio_sel_t *csPin = NULL;    /* SPI CS pin settings. */

    /* Preconditions. */
    AML_ASSERT(drvData != NULL);
    AML_ASSERT(amlSpiData != NULL);
    AML_ASSERT(amlSpiData->dataSize > 0U);
    /* Previous transfer must be complete. */
    AML_ASSERT(GPIO_AML_ReadInput(drvData->gpioConfig.csPin.gpioInstance,
            drvData->gpioConfig.csPin.gpioPinNumber) != 0U);

    csPin = &(drvData->gpioConfig.csPin);

    /* Wait until ACK pin is high to be sure that the previous transmission is
     * complete. */
    status = SF_WaitPinLevel(&(drvData->gpioConfig.ackPin), 1U,
            SF_ACK_NOTRANS_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        /* Set CS pin low. */
        SPI_AML_MasterSelectDevice(csPin->gpioInstance, csPin->gpioPinNumber,
                spiPcsActiveLow);

        /* Wait until ACK pin goes low. */
        status = SF_WaitPinLevel(&(drvData->gpioConfig.ackPin), 0U,
                SF_ACK_NOTRANS_TIMEOUT_US);
    }

    if (kStatus_Success == status)
    {
        /* Send data and receive dummy data. */
        status = SPI_AML_MasterTransfer(drvData->spiConfig.spiInstance, amlSpiData);
    }

    if (kStatus_Success == status)
    {
        /* Wait until ACK goes high. */
        status = SF_WaitPinLevel(&(drvData->gpioConfig.ackPin), 1U,
                SF_ACK_NOTRANS_TIMEOUT_US);
    }

    /* Set CS pin high (even if there is an error). */
    SPI_AML_MasterUnselectDevice(csPin->gpioInstance, csPin->gpioPinNumber,
            spiPcsActiveLow);

    return status;
}

static status_t SF_ReceiveSpiData(sf_drv_data_t *drvData,
        spi_aml_transfer_t *amlSpiData, uint32_t tmoutUs)
{
    status_t      status = kStatus_Fail;
    sf_gpio_sel_t *csPin = &(drvData->gpioConfig.csPin); /* SPI CS pin settings. */
    uint8_t       dataSizeOld = 0U;    /* Expected size of a received frame */

    /* Preconditions. */
    AML_ASSERT(drvData != NULL);
    AML_ASSERT(amlSpiData != NULL);
    AML_ASSERT(amlSpiData->dataSize > 0U);
    /* Previous transfer must be complete. */
    AML_ASSERT(GPIO_AML_ReadInput(csPin->gpioInstance, csPin->gpioPinNumber) != 0U);

    dataSizeOld = amlSpiData->dataSize;

    /* Send dummy data (all bits equal to 1) */
    memset((void *)amlSpiData->txBuffer, 0xFFU, (size_t)(amlSpiData->dataSize));

    /* Wait until ACK pin goes low. It takes long time for some commands causing
     * SIGFOX communication (for example Send Payload). It is several seconds
     * in contrast to other commands (Send Echo,...) which take a few milliseconds. */
    status = SF_WaitPinLevel(&(drvData->gpioConfig.ackPin), 0U,
            tmoutUs);

    if (kStatus_Success == status)
    {
        /* Set CS pin low. */
        SPI_AML_MasterSelectDevice(csPin->gpioInstance, csPin->gpioPinNumber,
                spiPcsActiveLow);

        /* Receive the first byte with length of the frame. */
        amlSpiData->dataSize = 1U;
        status = SPI_AML_MasterTransfer(drvData->spiConfig.spiInstance, amlSpiData);
    }

    if (kStatus_Success == status)
    {
        /* Update the SPI data structure to receive the rest of the ACK frame. */
        amlSpiData->dataSize = *(amlSpiData->rxBuffer + SF_ACK_LENGTH_OF);
        if ((amlSpiData->dataSize > dataSizeOld) || (amlSpiData->dataSize == 0U))
        {   /* Size of the receive buffer is smaller than data to be received. */
            /* Set CS pin back to high. */
            SPI_AML_MasterUnselectDevice(csPin->gpioInstance, csPin->gpioPinNumber,
                    spiPcsActiveLow);
            status = kStatus_SF_SpiAckLength;
        }
    }

    if (kStatus_Success == status)
    {
        /* Length field was received, skip it. */
        amlSpiData->dataSize -= 1;
        amlSpiData->txBuffer += 1;
        amlSpiData->rxBuffer += 1;

        /* Receive the rest of the frame. */
        status = SPI_AML_MasterTransfer(drvData->spiConfig.spiInstance, amlSpiData);

        /* Set position of SPI buffers back. */
        amlSpiData->dataSize += 1;
        amlSpiData->txBuffer -= 1;
        amlSpiData->rxBuffer -= 1;
    }

    if (kStatus_Success == status)
    {
        /* Wait until ACK goes high. */
        status = SF_WaitPinLevel(&(drvData->gpioConfig.ackPin), 1U, SF_ACK_NOTRANS_TIMEOUT_US);
    }
    /* Set CS pin high (even if there is an error). */
    SPI_AML_MasterUnselectDevice(csPin->gpioInstance, csPin->gpioPinNumber,
            spiPcsActiveLow);

    return status;
}

static status_t SF_WaitPinLevel(const sf_gpio_sel_t* gpioPin, uint32_t pinValExp,
        uint32_t tmoutUs)
{
    volatile uint32_t pinVal = 0U;     /* Value of a pin. */
    uint32_t     tmoutUsTemp = 0U;     /* Current time in us. */

    AML_ASSERT(gpioPin != NULL);
    /* Max. timeout is 20 s (it covers all cases). */
    AML_ASSERT((tmoutUs > 0U) && (tmoutUs < SF_ACK_TIMEOUT_MAX_US));

    do
    {
        pinVal = GPIO_AML_ReadInput(gpioPin->gpioInstance, gpioPin->gpioPinNumber);
        tmoutUsTemp += SF_TIMEOUT_STEP_US;

        /* WaitUS function ensures the timeout value is not less than a desired
         * time (tmoutUs). */
        WAIT_AML_WaitUs(SF_TIMEOUT_STEP_US);
    }
    while ((pinVal != pinValExp) && (tmoutUs > tmoutUsTemp));

    return (tmoutUs > tmoutUsTemp) ? kStatus_Success : kStatus_SF_SpiTimeout;
}

static status_t SF_SendCommandInt(sf_drv_data_t *drvData,
        uint8_t sendLen, uint8_t recvLen,
        uint8_t *sendBuffer, uint8_t *recvBuffer, uint32_t tmoutUs)
{
    status_t           status = kStatus_Success;
    spi_aml_transfer_t amlSpiData =    /* SPI transmission structure. */
    {
            sendBuffer,
            recvBuffer,
            sendLen,
            0x00U
    };

    /* Preconditions. */
    AML_ASSERT(drvData != NULL);
    AML_ASSERT(sendLen > 0U);
    AML_ASSERT(sendBuffer != NULL);

#if defined(DEBUG_SPI_COM) && (SDK_VERSION == SDK_KSDK)
    /* Note: SF_ReceiveSpiData function changes sendBuffer (sets all bits to 1). */
    PRINTF("Sent frame: ");
    SF_PrintFrame(amlSpiData.txBuffer, sendLen);
#endif

    if ((status = SF_SendSpiData(drvData, &amlSpiData)) != kStatus_Success)
    {
        return status;
    }

    if (recvLen > 0U)
    {   /* Note: some commands do not have an acknowledge frame. */
        AML_ASSERT(recvBuffer != NULL);

        /* Receive and process an ACK frame. */
        amlSpiData.dataSize = recvLen;
        if ((status = SF_ReceiveSpiData(drvData, &amlSpiData, tmoutUs)) == kStatus_Success)
        {   /* Store the error code and state from the ACK frame. */
            status = SF_StoreDevState(drvData, amlSpiData.rxBuffer);
        }
    }

#if defined(DEBUG_SPI_COM) && (SDK_VERSION == SDK_KSDK)
    if (recvLen > 0U)
    {
        PRINTF("Received frame: ");
        SF_PrintFrame(amlSpiData.rxBuffer, amlSpiData.dataSize);
    }
#endif

    return status;
}

static uint32_t SF_GetFreqDiv(uint32_t sysClkDiv)
{
    uint32_t freqxDiv = 0U;

    switch (sysClkDiv) {
        case 0U:
            freqxDiv = 64U;
            break;

        case 1U:
            freqxDiv = 32U;
            break;

        case 2U:
            freqxDiv = 16U;
            break;

        default:
            AML_ASSERT(false);
            break;
    }

    return freqxDiv;
}

static uint32_t SF_CalcMdes(uint32_t freqHz)
{
    uint32_t mdes = 0U;
    uint32_t fvco = 0U;
    uint32_t freqSys = 0U;

    freqSys = SF_FREQ_F_X0 / (SF_FREQ_SYS_CLK_DIV + 1U);
    fvco = SF_FREQ_T_FIX * freqHz;

    mdes = (fvco / SF_FREQ_N_FIX);
    mdes = mdes / (freqSys / SF_GetFreqDiv(SF_FREQ_SYS_CLK_DIV));

    return mdes;
}

static void SF_CalcFreqRegs(uint32_t freqHz, uint32_t *fcint, uint32_t *fcfrac,
        uint32_t *mdes)
{
    AML_ASSERT((freqHz >= SF_FREQ_MIN_HZ) && (freqHz <= SF_FREQ_MAX_HZ));

    /* Calculate FCINT from an up-link frequency.
     * FCINT = roundDown(ftx / fx0)
     */
    (*fcint) = (SF_FREQ_FACTOR * freqHz) / SF_FREQ_F_X0;

    /* Calculate FCRAC from an up-link frequency.
     * Original formula:
     *   FCFRAC = (ftx - FCINT * fx0) * 2^19
     *   FCFRAC = FCFRAC / fx0
     *   Round FCFRAC to the nearest integer (implicit rounding-down used instead).
     */
    (*fcfrac) = (SF_FREQ_FACTOR * freqHz) - ((*fcint) * SF_FREQ_F_X0);
    (*fcfrac) = (uint32_t)(((uint64_t)(*fcfrac) << 19U) / SF_FREQ_F_X0);

    /* Calculate AFC_MDES value. */
    (*mdes) = SF_CalcMdes(freqHz);
}

static uint32_t SF_CalcFreq(uint32_t fcint, uint32_t fcfrac)
{
    uint32_t freqHz = 0U;   /* Up-link frequency in Hz. */

    freqHz = ((5266U * fcfrac) + 1024U) >> 13U;

    freqHz += 52U * fcfrac;
    freqHz += fcint * SF_FREQ_F_X0;

    return freqHz;
}

static void SF_StoreRegs(uint32_t fcint, uint32_t fcfrac, uint32_t mdes, uint8_t *frame)
{
    AML_ASSERT(frame != NULL);

#if SF_MODEL == SF_MODEL_OL2361

    uint16_t freqcon0 = 0U;     /* Value of register FREQCON0 (OL2361 only). */
    uint16_t freqcon1 = 0U;     /* Value of register FREQCON1 (OL2361 only). */

    /* FREQCON0[15:7] = FCRAC[8:0]. */
    freqcon0 = (fcfrac & SF_FCFRAC_LH_MASK) << SF_FREQCON0_FCFRAC_LH_SHIFT;
    /* Enable fractional part. */
    freqcon0 |= SF_FREQCON0_FCDISFRAC_MASK;

    /* FREQCON1[15:10] = FCINT[5:0]. */
    freqcon1 = (fcint & SF_FCINT_MASK) << SF_FREQCON1_FCINT_SHIFT;
    /* FREQCON1[9:8] = FCFRAC[18:17]. */
    freqcon1 |= ((fcfrac & SF_FCFRAC_HH_MASK) >>
            (SF_FCFRAC_HH_SHIFT - SF_FREQCON1_FCFRAC_HH_SHIFT));
    /* FREQCON1[7:0] = FCFRAC[16:9]. */
    freqcon1 |= ((fcfrac & SF_FCFRAC_HL_MASK) >> (SF_FCFRAC_HL_SHIFT));

    frame[SF_FREQCON0_HIGH_OF + SF_INF_PAYLOAD_OF] = SF_GET_BITS_VALUE(freqcon0, 0xFF00U, 8U);
    frame[SF_FREQCON0_LOW_OF + SF_INF_PAYLOAD_OF] = SF_GET_BITS_VALUE(freqcon0, 0xFFU, 0U);
    frame[SF_FREQCON1_HIGH_OF + SF_INF_PAYLOAD_OF] = SF_GET_BITS_VALUE(freqcon1, 0xFF00U, 8U);
    frame[SF_FREQCON1_LOW_OF + SF_INF_PAYLOAD_OF] = SF_GET_BITS_VALUE(freqcon1, 0xFFU, 0U);

#else

    frame[SF_FREQCON0_HIGH_OF + SF_INF_PAYLOAD_OF] = fcint;
    frame[SF_FREQCON0_LOW_OF + SF_INF_PAYLOAD_OF] = (fcfrac & 0xFF0000U) >> 16U;
    frame[SF_FREQCON1_HIGH_OF + SF_INF_PAYLOAD_OF] = (fcfrac & 0xFF00U) >> 8U;
    frame[SF_FREQCON1_LOW_OF + SF_INF_PAYLOAD_OF] = fcfrac & 0xFFU;

#endif

    /* MDES (common for OL2385 and OL2361). */
    frame[SF_MDES_HIGH_OF + SF_INF_PAYLOAD_OF] = (mdes & 0xFF00U) >> 8U;
    frame[SF_MDES_LOW_OF + SF_INF_PAYLOAD_OF] = mdes & 0xFFU;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SF_RestoreRegs
 * Description   : This function fills the FCFRAC and FCINT registers using
 *                 the SPI frame.
 *
 *END**************************************************************************/
static void SF_RestoreRegs(const uint8_t *frame, uint32_t *fcint, uint32_t *fcfrac)
{
    AML_ASSERT(NULL != frame);
    AML_ASSERT(NULL != fcint);
    AML_ASSERT(NULL != fcfrac);

#if SF_MODEL == SF_MODEL_OL2361

    uint16_t freqcon0 = 0U;     /* Value of register FREQCON0 (OL2361 only). */
    uint16_t freqcon1 = 0U;     /* Value of register FREQCON1 (OL2361 only). */

    /* Extract FREQCON0 and FReQCON1 register values from the frame. */
    freqcon0 = SF_GET_FREQCON0(frame + SF_ACK_PAYLOAD_OF);
    freqcon1 = SF_GET_FREQCON1(frame + SF_ACK_PAYLOAD_OF);

    /* Extract FCINT from the FREQCON1 register. */
    /* FCINT[5:0] = FREQCON1[15:10]. */
    (*fcint) = (freqcon1 & SF_FREQCON1_FCINT_MASK) >> SF_FREQCON1_FCINT_SHIFT;

    /* Extract FCFRAC from FREQCON0 and FREQCON1 registers. */
    /* FCRAC[8:0] = FREQCON0[15:7]. */
    (*fcfrac) = (freqcon0 & SF_FREQCON0_FCFRAC_LH_MASK) >> SF_FREQCON0_FCFRAC_LH_SHIFT;
    /* FCFRAC[16:9] = FREQCON1[7:0]. */
    (*fcfrac) |= ((freqcon1 & SF_FREQCON1_FCFRAC_HL_MASK) << SF_FCFRAC_HL_SHIFT);
    /* FCFRAC[18:17] = FREQCON1[9:8]. */
    (*fcfrac) |= ((freqcon1 & SF_FREQCON1_FCFRAC_HH_MASK) <<
            (SF_FCFRAC_HH_SHIFT - SF_FREQCON1_FCFRAC_HH_SHIFT));

#else

    (*fcint) = frame[SF_FREQCON0_HIGH_OF + SF_ACK_PAYLOAD_OF];
    (*fcfrac) = frame[SF_FREQCON0_LOW_OF + SF_ACK_PAYLOAD_OF] << 16U;
    (*fcfrac) |= frame[SF_FREQCON1_HIGH_OF + SF_ACK_PAYLOAD_OF] << 8U;
    (*fcfrac) |= frame[SF_FREQCON1_LOW_OF + SF_ACK_PAYLOAD_OF];

#endif
}

static status_t SF_StoreDevState(sf_drv_data_t *drvData, const uint8_t *spiRcvBuf)
{
    AML_ASSERT(drvData != NULL);
    AML_ASSERT(spiRcvBuf != NULL);

    /* Store the error code and state from the ACK frame. */
    drvData->devState = (sf_intern_state_t)(*(spiRcvBuf + SF_ACK_STATE_OF));
    drvData->errorCode = (sf_intern_error_t)(*(spiRcvBuf + SF_ACK_ERROR_OF));

    /* Note: the user can obtain a value of the error code with use
     * of the SF_GetErrorCode function. */
    return (drvData->errorCode == sfErrNone) ? kStatus_Success : kStatus_SF_SpiAckError;
}

static status_t SF_StorePayload(const uint8_t *spiRcvBuf, sf_msg_payload_t *recvPayload,
        uint8_t recvPayloadBufSize)
{
    status_t status = kStatus_Fail;

    AML_ASSERT(spiRcvBuf != NULL);
    AML_ASSERT(recvPayload != NULL);
    AML_ASSERT((recvPayload->payload) != NULL);

    /* Get payload length. */
    recvPayload->payloadLen = (uint8_t)(*(spiRcvBuf + SF_ACK_LENGTH_OF));

    /* Subtract size of the ACK header. */
    recvPayload->payloadLen -= SF_ACK_HEADER_B;

    if (recvPayload->payloadLen > recvPayloadBufSize)
    {
        status = kStatus_SF_SpiAckLength;
    }
    else
    {
        status = kStatus_Success;
    }

    if ((kStatus_Success == status) && (recvPayload->payloadLen > 0U))
    {
        memcpy((void*)recvPayload->payload,
                (void*)(spiRcvBuf + SF_ACK_PAYLOAD_OF),
                (size_t)recvPayload->payloadLen);
    }

    return status;
}

static status_t SF_SendAndCheck(sf_drv_data_t *drvData,
        uint8_t sendLen, uint8_t recvLen,
        uint8_t *sendBuffer, uint8_t *recvBuffer, uint32_t tmoutUs)
{
    status_t status = SF_SendCommandInt(drvData, sendLen,
            recvLen,
            sendBuffer,
            recvBuffer,
            tmoutUs);

    /* Check the response. */
    if (kStatus_Success == status)
    {
        if (recvBuffer[SF_ACK_LENGTH_OF] != recvLen)
        {   /* An acknowledgement does not have expected length. */
            status = kStatus_SF_SpiAckLength;
        }
    }

    return status;
}

static status_t SF_SetFrequency(sf_drv_data_t *drvData, sf_spi_cmd_t cmd,
        uint32_t freqHz)
{
    uint8_t  spiSndBuf[SF_SET_FREQ_FRM_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_SET_FREQ_FRM_B]; /* SPI receive buffer. */
    uint32_t fcint = 0U;        /* Value of the integer divider parameter FCINT. */
    uint32_t fcfrac = 0U;       /* Value of the fractional divider parameter FCFRAC. */
    uint32_t mdes = 0U;         /* Value of AFC_MDES. */

    /* Preconditions. */
    AML_ASSERT(drvData != NULL);
    AML_ASSERT((freqHz >= SF_FREQ_MIN_HZ) && (freqHz <= SF_FREQ_MAX_HZ));
    AML_ASSERT((sfSpiCmdSetUlFreq == cmd) || (sfSpiCmdSetDlFreq == cmd));

    /* Calculate value of the FREQCON0/1 registers. */
    SF_CalcFreqRegs(freqHz, &fcint, &fcfrac, &mdes);

    /* Store the registers into a SPI frame. */
    SF_PackFrame(cmd, SF_SET_FREQ_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    SF_StoreRegs(fcint, fcfrac, mdes, (uint8_t*)spiSndBuf);

#if defined(DEBUG_FREQ_CALC) && (SDK_VERSION == SDK_KSDK)
    PRINTF("Freq   = %u Hz\r\n", freqHz);
    PRINTF("FCFRAC = %u\r\n", fcfrac);
    PRINTF("FCINT  = %u\r\n", fcint);
    PRINTF("MDES   = %u\r\n", mdes);
    PRINTF("Frame  = 0x%02x %02x %02x %02x %02x %02x %02x %02x\r\n", spiSndBuf[0], spiSndBuf[1],
            spiSndBuf[2], spiSndBuf[3], spiSndBuf[4], spiSndBuf[5], spiSndBuf[6], spiSndBuf[7]);
#endif

    return SF_SendAndCheck(drvData, SF_SET_FREQ_INF_B, SF_SET_FREQ_ACK_B,
            spiSndBuf, spiRcvBuf, SF_ACK_NOTRANS_TIMEOUT_US);
}

static status_t SF_GetFrequency(sf_drv_data_t *drvData, sf_spi_cmd_t cmd,
        uint32_t *freqHz)
{
    status_t status = kStatus_Fail;
    uint8_t  spiSndBuf[SF_GET_FREQ_FRM_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_GET_FREQ_FRM_B]; /* SPI receive buffer. */
    uint32_t fcint = 0U;        /* Value of the integer divider parameter FCINT. */
    uint32_t fcfrac = 0U;       /* Value of the fractional divider parameter FCFRAC. */

    /* Preconditions. */
    AML_ASSERT(drvData != NULL);
    AML_ASSERT(freqHz != NULL);

    SF_PackFrame(cmd, SF_GET_FREQ_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    status = SF_SendAndCheck(drvData, SF_GET_FREQ_INF_B, SF_GET_FREQ_ACK_B,
            spiSndBuf, spiRcvBuf, SF_ACK_NOTRANS_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        SF_RestoreRegs(spiRcvBuf, &fcint, &fcfrac);
        *freqHz = SF_CalcFreq(fcint, fcfrac);
    }

#if defined(DEBUG_FREQ_CALC) && (SDK_VERSION == SDK_KSDK)
    PRINTF("Freq   = %u Hz\r\n", *freqHz);
    PRINTF("FCFRAC = %u\r\n", fcfrac);
    PRINTF("FCINT  = %u\r\n", fcint);
    PRINTF("FRAME  = 0x%02x %02x %02x %02x %02x %02x %02x\r\n", spiRcvBuf[0], spiRcvBuf[1],
            spiRcvBuf[2], spiRcvBuf[0 + 3], spiRcvBuf[1 + 3], spiRcvBuf[2 + 3],
            spiRcvBuf[3 + 3]);
#endif

    return status;
}

/*******************************************************************************
 * Code - general functions
 ******************************************************************************/

status_t SF_Init(sf_drv_data_t *drvData, const sf_user_config_t *userConfig)
{
    status_t status = kStatus_Success;
    bool     spiTestSuccess = false;           /* Result of SPI test. */

    /* Preconditions. */
    AML_ASSERT(drvData != NULL);
    AML_ASSERT(userConfig != NULL);

    drvData->devState = sfStateInit;
    drvData->errorCode = sfErrNone;

    /* Wait until ACK pin is high (Sigfox device is ready). */
    status = SF_WaitPinLevel(&(drvData->gpioConfig.ackPin), 1U,
            SF_ACK_INIT_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        status = SF_WakeUp(drvData);
    }

    /* Test SPI communication. */
    if (kStatus_Success == status)
    {
        spiTestSuccess = true;
        status = SF_TestSpiCon(drvData, &spiTestSuccess);
    }
    if ((kStatus_Success == status) && (spiTestSuccess == false))
    {
        status = kStatus_SF_SpiTestFail;
    }

    /* Initialize registers of the Sigfox device. */
    if (kStatus_Success == status)
    {
        status = SF_ChangeNetworkStandard(drvData, userConfig->netStandard, userConfig->txAttSteps,
                        userConfig->xtal, userConfig->txRepeat, userConfig->paType);
    }

    /* Set default PA curve. */
    if (kStatus_Success == status)
    {
        status = SF_SetDefaultPATable(drvData, userConfig->netStandard);
    }

    return status;
}

void SF_GetDefaultConfig(sf_user_config_t *userConfig)
{
    AML_ASSERT(userConfig != NULL);

    userConfig->netStandard = sfNetStandardETSI;
    userConfig->txAttSteps = 0;
    userConfig->xtal = sfXtal552;
    userConfig->txRepeat = sfTxRepeat3;
    userConfig->paType = sfPa14;
}

status_t SF_SendCommand(sf_drv_data_t *drvData, sf_spi_cmd_t cmd,
        const sf_msg_payload_t *sendPayload, sf_msg_payload_t *recvPayload,
        uint8_t recvBufferSize)
{
    status_t status = kStatus_Success;
    uint8_t  spiSndBuf[SF_SPI_MSG_MAX_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_SPI_MSG_MAX_B]; /* SPI receive buffer. */
    uint8_t  recvSize = 0U;               /* Expected size of an ACK frame. */

    /* Preconditions. */
    AML_ASSERT((cmd >= SF_CMD_FIRST_ID) && (cmd <= SF_CMD_LAST_ID));
    AML_ASSERT(sendPayload != NULL);
    AML_ASSERT(sendPayload->payloadLen <= SF_INF_PAYLOAD_MAX_B);
    /* Payload data variable must be non-null when the command contains paylaod. */
    AML_ASSERT((SF_HAS_CMD_PLD(cmd) && (sendPayload->payload != NULL)) ||
            (SF_HAS_CMD_PLD(cmd) == false));

    /* When a command has an acknowledgement, the actual ACK frame size is
     * defined by the first byte of the ACK and updated later. */
    recvSize = (SF_HAS_CMD_ACK(cmd)) ? SF_SPI_MSG_MAX_B : 0U;

    SF_PackFrame(cmd, sendPayload->payloadLen, sendPayload->payload, (uint8_t*)spiSndBuf);
    status = SF_SendCommandInt(drvData, sendPayload->payloadLen + SF_INF_HEADER_B,
            recvSize, spiSndBuf, spiRcvBuf, SF_ACK_TRANS_RECV_TIMEOUT_US);

    if ((recvPayload != NULL) && (kStatus_Success == status))
    {   /* Store the payload and its length. */
        status = SF_StorePayload(spiRcvBuf, recvPayload, recvBufferSize);
    }

    return status;
}

status_t SF_WakeUp(sf_drv_data_t *drvData)
{
    status_t status = kStatus_Fail;
    uint8_t  spiSndBuf[SF_SEND_WAKEUP_INF_B]; /* SPI send buffer. */

    SF_PackFrame(sfSpiCmdWakeup, SF_SEND_WAKEUP_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    status = SF_SendCommandInt(drvData, SF_SEND_WAKEUP_INF_B, SF_SEND_WAKEUP_ACK_B,
            spiSndBuf, NULL, SF_ACK_NOTRANS_TIMEOUT_US);

    /* Delay is needed after wake up. */
    WAIT_AML_WaitMs(SF_WAKEUP_DLY_MS);

    return status;
}

status_t SF_Sleep(sf_drv_data_t *drvData)
{
    uint8_t spiSndBuf[SF_SEND_TO_SLEEP_INF_B]; /* SPI send buffer. */

    SF_PackFrame(sfSpiCmdSleep, SF_SEND_TO_SLEEP_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    return SF_SendCommandInt(drvData, SF_SEND_TO_SLEEP_INF_B, SF_SEND_TO_SLEEP_ACK_B,
            spiSndBuf, NULL, SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_SetUlFrequency(sf_drv_data_t *drvData, uint32_t freqHz)
{
    return SF_SetFrequency(drvData, sfSpiCmdSetUlFreq, freqHz);
}

status_t SF_GetUlFrequency(sf_drv_data_t *drvData, uint32_t *freqHz)
{
    return SF_GetFrequency(drvData, sfSpiCmdGetUlFreq, freqHz);
}

status_t SF_SendPayload(sf_drv_data_t *drvData, const sf_msg_payload_t *sendPayload)
{
    uint8_t  spiSndBuf[SF_SEND_PAYLOAD_FRM_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_SEND_PAYLOAD_FRM_B]; /* SPI receive buffer. */

    /* Preconditions. */
    AML_ASSERT(sendPayload != NULL);
    AML_ASSERT((sendPayload->payloadLen > 0U) && (sendPayload->payload != NULL));

    SF_PackFrame(sfSpiCmdSendPayload, sendPayload->payloadLen, sendPayload->payload,
           (uint8_t*)spiSndBuf);
    return SF_SendAndCheck(drvData, SF_INF_HEADER_B + sendPayload->payloadLen,
            SF_SEND_PAYLOAD_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_TRANS_TIMEOUT_US);
}

status_t SF_SendBit(sf_drv_data_t *drvData, uint8_t bitVal)
{
    uint8_t  spiSndBuf[SF_SEND_BIT_FRM_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_SEND_BIT_FRM_B]; /* SPI receive buffer. */

    AML_ASSERT((0U == bitVal) || (1U == bitVal));

    SF_PackFrame(sfSpiCmdSendBit, SF_SEND_BIT_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    spiSndBuf[SF_SEND_BIT_VAL_OF] = bitVal;
    /* Initiate download flag is fixed (no download). */
    spiSndBuf[SF_SEND_BIT_INIT_DOWNLOAD_OF] = 0U;

    return SF_SendAndCheck(drvData, SF_SEND_BIT_INF_B, SF_SEND_BIT_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_TRANS_TIMEOUT_US);
}

status_t SF_SendOutOfBand(sf_drv_data_t *drvData)
{
    uint8_t  spiSndBuf[SF_SEND_OUT_BAND_FRM_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_SEND_OUT_BAND_FRM_B]; /* SPI receive buffer. */

    SF_PackFrame(sfSpiCmdOutOfBand, SF_SEND_OUT_BAND_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    return SF_SendAndCheck(drvData, SF_SEND_OUT_BAND_INF_B, SF_SEND_OUT_BAND_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_TRANS_TIMEOUT_US);
}

#if (SF_MODEL == SF_MODEL_OL2385)
status_t SF_ReceiveMessage(sf_drv_data_t *drvData, const sf_msg_payload_t *sendPayload,
        sf_msg_payload_t *recvPayload)
{
    status_t status = kStatus_Success;
    uint8_t  spiSndBuf[SF_RECEIVE_FR_FRM_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_RECEIVE_FR_FRM_B]; /* SPI receive buffer. */

    /* Preconditions. */
    AML_ASSERT(recvPayload != NULL);
    AML_ASSERT(sendPayload != NULL);
    AML_ASSERT((sendPayload->payloadLen > 0U) && (sendPayload->payload != NULL));

    SF_PackFrame(sfSpiCmdReceive, sendPayload->payloadLen, sendPayload->payload,
            (uint8_t*)spiSndBuf);
    status = SF_SendAndCheck(drvData, SF_INF_HEADER_B + sendPayload->payloadLen,
            SF_RECEIVE_FR_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_TRANS_RECV_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        recvPayload->payloadLen = spiRcvBuf[SF_ACK_LENGTH_OF] - SF_ACK_HEADER_B;
        if (recvPayload->payloadLen > 0U)
        {
            memcpy((void*)recvPayload->payload,
                    (void*)(spiRcvBuf + SF_ACK_PAYLOAD_OF),
                    (size_t)(recvPayload->payloadLen));
        }
    }

    return status;
}
#endif

status_t SF_TriggerWatchdog(sf_drv_data_t *drvData)
{
    uint8_t spiSndBuf[SF_TRIGGER_WD_INF_B]; /* SPI send buffer. */

    SF_PackFrame(sfSpiCmdTriggerWd, SF_TRIGGER_WD_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    return SF_SendCommandInt(drvData, SF_TRIGGER_WD_INF_B, SF_TRIGGER_WD_ACK_B,
            spiSndBuf, NULL, SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_GetDeviceInfo(sf_drv_data_t *drvData, sf_device_info_t *devInfo)
{
    status_t status = kStatus_Success;
    uint8_t  spiSndBuf[SF_GET_INFO_FRM_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_GET_INFO_FRM_B]; /* SPI receive buffer. */

    /* Preconditions. */
    AML_ASSERT(devInfo != NULL);
    AML_ASSERT(devInfo->libVersion != NULL);

    /* Send the Get Info command. */
    SF_PackFrame(sfSpiCmdGetInfo, SF_GET_INFO_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    status = SF_SendAndCheck(drvData, SF_GET_INFO_INF_B, SF_GET_INFO_ACK_B,
            spiSndBuf, spiRcvBuf, SF_ACK_NOTRANS_TIMEOUT_US);

    /* Process the response. */
    if (kStatus_Success == status)
    {
        /* Device ID bytes are in reverse order. */
        devInfo->devId = SF_GET_DEV_ID(spiRcvBuf +
                (SF_DEV_ID_OF + SF_ACK_PAYLOAD_OF));
        memcpy(devInfo->devPac, spiRcvBuf + (SF_DEV_PAC_OF + SF_ACK_PAYLOAD_OF),
                SF_DEV_PAC_B);
        memcpy(devInfo->libVersion, spiRcvBuf +
                (SF_LIB_VER_OF + SF_ACK_PAYLOAD_OF), SF_LIB_VER_B);
    }

    /* Send the Get Device version command. */
    if (kStatus_Success == status)
    {
        SF_PackFrame(sfSpiCmdGetDevVer, SF_GET_DEV_VER_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
        status = SF_SendAndCheck(drvData, SF_GET_DEV_VER_INF_B, SF_GET_DEV_VER_ACK_B,
                spiSndBuf, spiRcvBuf, SF_ACK_NOTRANS_TIMEOUT_US);
    }

    /* Process the response. */
    if (kStatus_Success == status)
    {
        memcpy(devInfo->devVersion, spiRcvBuf + (SF_ACK_PAYLOAD_OF + SF_DEV_VER_OF),
                SF_DEV_VER_B);
    }

    return status;
}

sf_intern_error_t SF_GetErrorCode(sf_drv_data_t *drvData)
{
    sf_intern_error_t tempErrorCode = drvData->errorCode;
    drvData->errorCode = sfErrNone;
    return tempErrorCode;
}

status_t SF_TestSpiCon(sf_drv_data_t *drvData, bool *success)
{
    status_t      status = kStatus_Success;
    uint8_t       spiSndBuf[SF_SEND_ECHO_FRM_B]; /* SPI send buffer. */
    uint8_t       spiRcvBuf[SF_SEND_ECHO_FRM_B]; /* SPI receive buffer. */
    const uint8_t payload[] = SF_ECHO_PLD_ARRAY; /* I-frame payload. */
    /* Size of the payload in bytes. */
    const uint8_t payloadLen = (uint8_t)sizeof(payload) / (uint8_t)sizeof(payload[0]);
    uint8_t       i = 0U;

    /* Preconditions. */
    AML_ASSERT(success != NULL);
    AML_ASSERT((payloadLen > 0U) && (payloadLen <= SF_SEND_ECHO_INF_PLD_B));

    *success = false;

    /* Send the Echo command. */
    SF_PackFrame(sfSpiCmdSendEcho, payloadLen, (uint8_t*)payload, (uint8_t*)spiSndBuf);
    status = SF_SendAndCheck(drvData, SF_SEND_ECHO_INF_B, payloadLen + SF_ACK_HEADER_B,
            spiSndBuf, spiRcvBuf, SF_ACK_NOTRANS_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        /* Check an acknowledgement which should contain an inverted payload
         * of the I-frame sent. */
        *success = true;
        for (i = 0U; i < payloadLen; i++)
        {
            if (((uint8_t)(~spiRcvBuf[SF_ACK_PAYLOAD_OF + i])) != payload[i])
            {
                *success = false;
                break;
            }
        }
    }
    return status;
}

status_t SF_TestDevice(sf_drv_data_t *drvData, sf_test_mode_t testMode,
        uint8_t modeConfig)
{
    uint8_t spiSndBuf[SF_SEND_TEST_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_SEND_TEST_FRM_B]; /* SPI receive buffer. */

    SF_PackFrame(sfSpiCmdSendTestMode, SF_SEND_TEST_INF_PLD_B, NULL,
            (uint8_t*)spiSndBuf);
    spiSndBuf[SF_TEST_MODE_OF] = testMode;
    spiSndBuf[SF_TEST_CFG_OF] = modeConfig;

    return SF_SendAndCheck(drvData, SF_SEND_TEST_INF_B, SF_SEND_TEST_ACK_B,
            spiSndBuf, spiRcvBuf, SF_ACK_TRANS_TIMEOUT_US);
}

status_t SF_GenerateContWave(sf_drv_data_t *drvData, bool enable)
{
    uint8_t spiSndBuf[SF_CONT_WAVE_FRM_B]; /* SPI send buffer. */

    SF_PackFrame(sfSpiCmdContWave, SF_CONT_WAVE_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    spiSndBuf[SF_CONT_WAVE_SWITCH_OF] = (enable) ? SF_CONT_WAVE_EN : SF_CONT_WAVE_DIS;

    return SF_SendCommandInt(drvData, SF_CONT_WAVE_INF_B, SF_CONT_WAVE_ACK_B,
            spiSndBuf, NULL, SF_ACK_TRANS_TIMEOUT_US);
}

status_t SF_ChangeNetworkStandard(sf_drv_data_t *drvData, sf_net_standard_t standardSel,
        uint8_t txAttSteps, sf_xtal_type_t xtal, sf_tx_repeat_t txRepeat, sf_pa_type_t paType)
{
    uint8_t spiSndBuf[SF_CHANGE_TO_RCZX_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_CHANGE_TO_RCZX_FRM_B]; /* SPI receive buffer. */
    sf_spi_cmd_t cmd = sfSpiCmdChangeToRCZ1;    /* SPI command ID. */

    /* Preconditions. */
    AML_ASSERT(sfNetStandardFCC_SouthAmerica >= standardSel);
    AML_ASSERT(sfXtal276 >= xtal);
    AML_ASSERT(sfTxRepeat3 >= txRepeat);
    AML_ASSERT(sfPa0 >= paType);

    /* Get command ID. */
    cmd = (sf_spi_cmd_t)(SF_CMD_CHANGE_TO_RCZ1_ID + (uint8_t)standardSel);

    /* Send the command. */
    SF_PackFrame(cmd, SF_CHANGE_TO_RCZX_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);

    spiSndBuf[SF_CHANGE_TO_RCZX_TX_STEPS_INF_OF] = txAttSteps;
    spiSndBuf[SF_CHANGE_TO_RCZX_XTAL_INF_OF] = xtal;
    spiSndBuf[SF_CHANGE_TO_RCZX_TX_REPEAT_INF_OF] = txRepeat;
    spiSndBuf[SF_CHANGE_TO_RCZX_PA_TYPE_INF_OF] = paType;

    return SF_SendCommandInt(drvData, SF_CHANGE_TO_RCZX_INF_B, SF_CHANGE_TO_RCZX_ACK_B,
            spiSndBuf, spiRcvBuf, SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_SwitchToPrivateKey(sf_drv_data_t *drvData)
{
    uint8_t spiSndBuf[SF_SWITCH_PRIVATE_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_SWITCH_PRIVATE_FRM_B]; /* SPI receive buffer. */

    /* Send the command. */
    SF_PackFrame(sfSpiCmdSwitchToPrivateKey, SF_SWITCH_PRIVATE_INF_PLD_B,
            NULL,
            (uint8_t*)spiSndBuf);
    return SF_SendAndCheck(drvData, SF_SWITCH_PRIVATE_INF_B,
            SF_SWITCH_PRIVATE_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_SwitchToPublicKey(sf_drv_data_t *drvData)
{
    uint8_t spiSndBuf[SF_SWITCH_PUBLIC_FRM_B]; /* SPI send buffer. */

    /* Send the command. */
    SF_PackFrame(sfSpiCmdSwitchToPublicKey, SF_SWITCH_PUBLIC_INF_PLD_B,
            NULL,
            (uint8_t*)spiSndBuf);
    return SF_SendCommandInt(drvData, SF_SWITCH_PUBLIC_INF_B, SF_SWITCH_PUBLIC_ACK_B,
            spiSndBuf, NULL, SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_TransmitZeroSeq(sf_drv_data_t *drvData, int16_t repetitions)
{
    uint8_t spiSndBuf[SF_PSK_ASK_ZERO_FRM_B]; /* SPI send buffer. */

    /* Preconditions. */
    AML_ASSERT(repetitions <= (int16_t)SF_ZERO_MAX_REPETITIONS);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdPskAskZeroSeq, SF_PSK_ASK_ZERO_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    spiSndBuf[SF_ZERO_INFIN_LOOP_OF] = (repetitions >= 0) ?
            SF_ZERO_INFIN_LOOP_DIS : SF_ZERO_INFIN_LOOP_EN;
    spiSndBuf[SF_ZERO_REPETITIONS_OF] = (repetitions >= 0) ? repetitions : 0U;

    return SF_SendCommandInt(drvData, SF_PSK_ASK_ZERO_INF_B, SF_PSK_ASK_ZERO_ACK_B,
            spiSndBuf, NULL, SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_TransmitAAAASeq(sf_drv_data_t *drvData, int16_t repetitions)
{
    uint8_t spiSndBuf[SF_PSK_ASK_AAAA_FRM_B]; /* SPI send buffer. */

    /* Preconditions. */
    AML_ASSERT(repetitions <= (int16_t)SF_AAAA_MAX_REPETITIONS);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdPskAskAAAASeq, SF_PSK_ASK_AAAA_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    spiSndBuf[SF_AAAA_INFIN_LOOP_OF] = (repetitions >= 0) ?
            SF_AAAA_INFIN_LOOP_DIS : SF_AAAA_INFIN_LOOP_EN;
    spiSndBuf[SF_AAAA_REPETITIONS_OF] = (repetitions >= 0) ? repetitions : 0U;

    return SF_SendCommandInt(drvData, SF_PSK_ASK_AAAA_INF_B, SF_PSK_ASK_AAAA_ACK_B,
            spiSndBuf, NULL, SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_SetDlFrequency(sf_drv_data_t *drvData, uint32_t freqHz)
{
    return SF_SetFrequency(drvData, sfSpiCmdSetDlFreq, freqHz);
}

status_t SF_GetDlFrequency(sf_drv_data_t *drvData, uint32_t *freqHz)
{
    return SF_GetFrequency(drvData, sfSpiCmdGetDlFreq, freqHz);
}

status_t SF_SetFCCMacroChannel(sf_drv_data_t *drvData, const uint32_t fhMask[],
        uint8_t defaultChNo)
{
    uint8_t spiSndBuf[SF_SET_FCC_CH_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_SET_FCC_CH_FRM_B]; /* SPI receive buffer. */
    uint8_t i = 0U;

    AML_ASSERT(NULL != fhMask);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdSetFCCMacroCh, SF_SET_FCC_CH_INF_PLD_B, NULL, (uint8_t*)spiSndBuf);
    for (i = 0U; i < (SF_MACRO_CH_CFG_B / 4); i++) {
        spiSndBuf[SF_INF_PAYLOAD_OF + 4*i] = (uint8_t)(fhMask[i] >> 24U);
        spiSndBuf[SF_INF_PAYLOAD_OF + 4*i + 1U] = (uint8_t)(fhMask[i] >> 16U);
        spiSndBuf[SF_INF_PAYLOAD_OF + 4*i + 2U] = (uint8_t)(fhMask[i] >> 8U);
        spiSndBuf[SF_INF_PAYLOAD_OF + 4*i + 3U] = (uint8_t)(fhMask[i]);
    }
    spiSndBuf[SF_DEFAULT_CHANNEL_OF] = defaultChNo;

    return SF_SendAndCheck(drvData, SF_SET_FCC_CH_INF_B, SF_SET_FCC_CH_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_GetFCCMacroChUsage(sf_drv_data_t *drvData, uint8_t *channelUsage)
{
    status_t status = kStatus_Fail;
    uint8_t spiSndBuf[SF_GET_FCC_CH_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_GET_FCC_CH_FRM_B]; /* SPI receive buffer. */

    AML_ASSERT(NULL != channelUsage);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdGetFCCMacroChUsage, SF_GET_FCC_CH_INF_PLD_B, NULL,
            (uint8_t*)spiSndBuf);
    status = SF_SendAndCheck(drvData, SF_GET_FCC_CH_INF_B, SF_GET_FCC_CH_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        *channelUsage = spiRcvBuf[SF_FCC_CH_OF];
    }

    return status;
}

status_t SF_ResetFCCMacroChannel(sf_drv_data_t *drvData)
{
    uint8_t spiSndBuf[SF_RESET_FCC_CH_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_RESET_FCC_CH_FRM_B]; /* SPI receive buffer. */

    /* Send the command. */
    SF_PackFrame(sfSpiCmdResetFCCMacroCh, SF_RESET_FCC_CH_INF_PLD_B, NULL,
            (uint8_t*)spiSndBuf);
    return SF_SendAndCheck(drvData, SF_RESET_FCC_CH_INF_B, SF_RESET_FCC_CH_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_CalibrateFreqStatic(sf_drv_data_t *drvData, bool signPlus,
        uint32_t driftHz)
{
    uint8_t spiSndBuf[SF_STATIC_FREQ_CAL_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_STATIC_FREQ_CAL_FRM_B]; /* SPI receive buffer. */

    AML_ASSERT(SF_FREQ_MAX_HZ >= driftHz);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdStaticFreqCal, SF_STATIC_FREQ_CAL_INF_PLD_B, NULL,
            (uint8_t*)spiSndBuf);
    spiSndBuf[SF_STATIC_FREQ_CAL_SIGN_OF] = (!signPlus) ?
            SF_DRIFT_NEGATIVE : SF_DRIFT_POSITIVE;
    spiSndBuf[SF_STATIC_FREQ_CAL_DRIFT_OF] = (uint8_t)(driftHz >> 24U);
    spiSndBuf[SF_STATIC_FREQ_CAL_DRIFT_OF + 1] = (uint8_t)(driftHz >> 16U);
    spiSndBuf[SF_STATIC_FREQ_CAL_DRIFT_OF + 2] = (uint8_t)(driftHz >> 8U);
    spiSndBuf[SF_STATIC_FREQ_CAL_DRIFT_OF + 3] = (uint8_t)driftHz;

    return SF_SendAndCheck(drvData, SF_STATIC_FREQ_CAL_INF_B, SF_STATIC_FREQ_CAL_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_UpdateTempFreqCalTable(sf_drv_data_t *drvData, const uint8_t driftTable[])
{
    uint8_t spiSndBuf[SF_TEMP_FREQ_CAL_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_TEMP_FREQ_CAL_FRM_B]; /* SPI receive buffer. */
    uint8_t i = 0U;

    AML_ASSERT(NULL != driftTable);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdTempFreqCal, SF_TEMP_FREQ_CAL_INF_PLD_B, NULL,
            (uint8_t*)spiSndBuf);

    spiSndBuf[SF_TEMP_FREQ_CAL_SUBCMD_INF_OF] = SF_SUBCMD_UPDATE_TBL;
    for (i = 0U; i < SF_TEMP_FREQ_CAL_TBL_SIZE; i++)
    {
        spiSndBuf[SF_TEMP_FREQ_CAL_TEMP_INF_OF + i] = driftTable[i];
    }

    return SF_SendAndCheck(drvData,
            SF_TEMP_FREQ_CAL_INF_B,
            SF_ACK_HEADER_B, /* Update sub-cmd does not return any payload data. */
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_ReadTempFreqCalTable(sf_drv_data_t *drvData, uint8_t driftTableRes[])
{
    uint8_t spiSndBuf[SF_TEMP_FREQ_CAL_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_TEMP_FREQ_CAL_FRM_B]; /* SPI receive buffer. */
    status_t status = kStatus_Fail;
    uint8_t i = 0U;

    AML_ASSERT(NULL != driftTableRes);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdTempFreqCal, 1U /* Sub-cmd parameter only. */,
            NULL,
            (uint8_t*)spiSndBuf);

    spiSndBuf[SF_TEMP_FREQ_CAL_SUBCMD_INF_OF] = SF_SUBCMD_READ_TBL;

    status = SF_SendAndCheck(drvData,
            1U + SF_INF_HEADER_B, /* Read sub-cmd does not have any payload data. */
            SF_TEMP_FREQ_CAL_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        for (i = 0U; i < SF_TEMP_FREQ_CAL_TBL_SIZE; i++)
        {
            driftTableRes[i] = spiRcvBuf[SF_TEMP_FREQ_CAL_TEMP_ACK_OF + i];
        }
    }

    return status;
}

status_t SF_SetDefaultTempFreqCalTable(sf_drv_data_t *drvData)
{
    uint8_t spiSndBuf[SF_TEMP_FREQ_CAL_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_TEMP_FREQ_CAL_FRM_B]; /* SPI receive buffer. */

    /* Send the command. */
    SF_PackFrame(sfSpiCmdTempFreqCal, 1U /* Sub-cmd parameter only. */,
            NULL,
            (uint8_t*)spiSndBuf);

    spiSndBuf[SF_TEMP_FREQ_CAL_SUBCMD_INF_OF] = SF_SUBCMD_SET_DEFAULT_TBL;

    return SF_SendAndCheck(drvData,
            1U + SF_INF_HEADER_B,   /* Sub-cmd parameter only. */
            SF_ACK_HEADER_B,        /* This sub-cmd does not return any payload. */
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_GetLastSentRSSI(sf_drv_data_t *drvData, uint8_t *rssiRes)
{
    status_t status = kStatus_Fail;
    uint8_t spiSndBuf[SF_GET_LAST_SENT_RSSI_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_GET_LAST_SENT_RSSI_FRM_B]; /* SPI receive buffer. */

    AML_ASSERT(drvData != NULL);
    AML_ASSERT(rssiRes != NULL);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdGetLastSentRSSI, 0U, NULL,
            (uint8_t*)spiSndBuf);

    spiSndBuf[SF_GET_LAST_SENT_RSSI_SUBCMD_INF_OF] = SF_SUBCMD_GET_LAST_RSSI;

    status = SF_SendAndCheck(drvData,
            2U,
            1U + SF_ACK_HEADER_B, /* Get Last Sent RSSI sub-cmd returns just
                                     RSSI info byte in the payload. */
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        *rssiRes = spiRcvBuf[SF_SENT_RSSI_DATA_OF];
    }

    return status;
}

status_t SF_GetLast30SentRSSI(sf_drv_data_t *drvData, uint8_t *rssiRes)
{
    status_t status = kStatus_Fail;
    uint8_t spiSndBuf[SF_GET_LAST_SENT_RSSI_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_GET_LAST_SENT_RSSI_FRM_B]; /* SPI receive buffer. */
    uint8_t i = 0U;

    AML_ASSERT(drvData != NULL);
    AML_ASSERT(rssiRes != NULL);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdGetLastSentRSSI, SF_GET_LAST_SENT_RSSI_INF_PLD_B, NULL,
            (uint8_t*)spiSndBuf);

    spiSndBuf[SF_GET_LAST_SENT_RSSI_SUBCMD_INF_OF] = SF_SUBCMD_GET_LAST_30_RSSI;

    status = SF_SendAndCheck(drvData,
            SF_GET_LAST_SENT_RSSI_INF_B,
            SF_GET_LAST_SENT_RSSI_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        for (i = 0; i < SF_SENT_RSSI_TBL_SIZE; i++)
        {
            rssiRes[i] = spiRcvBuf[SF_SENT_RSSI_DATA_OF + i];
        }
    }

    return status;
}

status_t SF_ResetLastSentRSSIBuffer(sf_drv_data_t *drvData)
{
    uint8_t spiSndBuf[SF_GET_LAST_SENT_RSSI_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_GET_LAST_SENT_RSSI_FRM_B]; /* SPI receive buffer. */

    AML_ASSERT(drvData != NULL);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdGetLastSentRSSI, SF_GET_LAST_SENT_RSSI_INF_PLD_B, NULL,
            (uint8_t*)spiSndBuf);

    spiSndBuf[SF_GET_LAST_SENT_RSSI_SUBCMD_INF_OF] = SF_SUBCMD_RESET_RSSI_BUFFER;

    return SF_SendAndCheck(drvData,
            SF_GET_LAST_SENT_RSSI_INF_B,
            SF_ACK_HEADER_B, /* Reset RSSI buffer sub-cmd returns no payload. */
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_ReadCurrentPATable(sf_drv_data_t *drvData, sf_net_standard_t netStd, uint8_t *paTable)
{
    status_t status = kStatus_Fail;
    uint8_t spiSndBuf[SF_UPDATE_PA_CURVE_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_UPDATE_PA_CURVE_FRM_B]; /* SPI receive buffer. */
    uint8_t i = 0U;

    AML_ASSERT(drvData != NULL);
    AML_ASSERT(paTable != NULL);
    AML_ASSERT((netStd == sfNetStandardETSI) || (netStd == sfNetStandardFCC_SouthAmerica) ||
            (netStd == sfNetStandardFCC_USA) || (netStd == sfNetStandardARIB));

    /* Send the command. */
    SF_PackFrame(sfSpiCmdUpdatePACurve, 1U /* Sub-cmd parameter only. */, NULL,
            (uint8_t*)spiSndBuf);

    if ((netStd == sfNetStandardETSI) || (netStd == sfNetStandardARIB))
    {
        spiSndBuf[SF_UPDATE_PA_CURVE_SUBCMD_INF_OF] = SF_SUBCMD_READ_ETSI_PA_TABLE;
    }
    else
    {
        spiSndBuf[SF_UPDATE_PA_CURVE_SUBCMD_INF_OF] = SF_SUBCMD_READ_FCC_PA_TABLE;
    }

    status = SF_SendAndCheck(drvData,
            1U + SF_INF_HEADER_B, /* Sub-cmd parameter in the payload only. */
            SF_UPDATE_PA_CURVE_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);

    if (kStatus_Success == status)
    {
        for (i = 0U; i < SF_PA_TBL_SIZE; i++)
        {
            paTable[i] = spiRcvBuf[SF_SENT_PA_CURVE_OF + i];
        }
    }

    return status;
}

status_t SF_UpdatePATable(sf_drv_data_t *drvData, sf_net_standard_t netStd, uint8_t *paTable)
{
    uint8_t spiSndBuf[SF_UPDATE_PA_CURVE_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_UPDATE_PA_CURVE_FRM_B]; /* SPI receive buffer. */
    uint8_t i = 0U;

    AML_ASSERT(drvData != NULL);
    AML_ASSERT(paTable != NULL);
    AML_ASSERT((netStd == sfNetStandardETSI) || (netStd == sfNetStandardFCC_SouthAmerica) ||
            (netStd == sfNetStandardFCC_USA) || (netStd == sfNetStandardARIB));

    /* Send the command. */
    SF_PackFrame(sfSpiCmdUpdatePACurve, SF_UPDATE_PA_CURVE_INF_PLD_B, NULL,
            (uint8_t*)spiSndBuf);

    if ((netStd == sfNetStandardETSI) || (netStd == sfNetStandardARIB))
    {
        spiSndBuf[SF_UPDATE_PA_CURVE_SUBCMD_INF_OF] = SF_SUBCMD_UPDATE_ETSI_PA_TABLE;
    }
    else
    {
        spiSndBuf[SF_UPDATE_PA_CURVE_SUBCMD_INF_OF] = SF_SUBCMD_UPDATE_FCC_PA_TABLE;
    }

    for (i = 0U; i < SF_PA_TBL_SIZE; i++)
    {
        spiSndBuf[SF_UPDATE_PA_CURVE_CURVE_INF_OF + i] = paTable[i];
    }

    return SF_SendAndCheck(drvData,
            SF_UPDATE_PA_CURVE_INF_B,
            SF_ACK_HEADER_B, /* Update PA table sub-cmd has no payload. */
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_SetDefaultPATable(sf_drv_data_t *drvData, sf_net_standard_t netStd)
{
    uint8_t spiSndBuf[SF_UPDATE_PA_CURVE_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_UPDATE_PA_CURVE_FRM_B]; /* SPI receive buffer. */

    AML_ASSERT(drvData != NULL);
    AML_ASSERT((netStd == sfNetStandardETSI) || (netStd == sfNetStandardFCC_SouthAmerica) ||
            (netStd == sfNetStandardFCC_USA) || (netStd == sfNetStandardARIB));

    /* Send the command. */
    SF_PackFrame(sfSpiCmdUpdatePACurve, 1U /* Sub-cmd parameter only. */, NULL,
            (uint8_t*)spiSndBuf);

    if ((netStd == sfNetStandardETSI) || (netStd == sfNetStandardARIB))
    {
        spiSndBuf[SF_UPDATE_PA_CURVE_SUBCMD_INF_OF] = SF_SUBCMD_DEFAULT_ETSI_PA_TABLE;
    }
    else
    {
        spiSndBuf[SF_UPDATE_PA_CURVE_SUBCMD_INF_OF] = SF_SUBCMD_DEFAULT_FCC_PA_TABLE;
    }

    return SF_SendAndCheck(drvData,
            1U + SF_INF_HEADER_B, /* Sub-cmd parameter in the payload only. */
            SF_ACK_HEADER_B, /* Set default PA table sub-cmd has no payload. */
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_CalibrateInitialRSSI(sf_drv_data_t *drvData, bool signPlus, uint16_t offset)
{
    uint8_t spiSndBuf[SF_INIT_RSSI_CALIB_FRM_B]; /* SPI send buffer. */
    uint8_t spiRcvBuf[SF_INIT_RSSI_CALIB_FRM_B]; /* SPI receive buffer. */

    AML_ASSERT(drvData != NULL);

    /* Send the command. */
    SF_PackFrame(sfSpiCmdInitRSSICal, SF_INIT_RSSI_CALIB_INF_PLD_B, NULL,
            (uint8_t*)spiSndBuf);

    spiSndBuf[SF_INIT_RSSI_CALIB_SIGN_INF_OF] = (!signPlus) ?
            SF_INIT_RSSI_NEGATIVE : SF_INIT_RSSI_POSITIVE;

    spiSndBuf[SF_INIT_RSSI_CALIB_RSSI_MSB_INF_OF] = (uint8_t)((offset >> 8) & 0xFF);
    spiSndBuf[SF_INIT_RSSI_CALIB_RSSI_LSB_INF_OF] = (uint8_t)(offset & 0xFF);

    return SF_SendAndCheck(drvData,
            SF_INIT_RSSI_CALIB_INF_B,
            SF_INIT_RSSI_CALIB_ACK_B,
            spiSndBuf,
            spiRcvBuf,
            SF_ACK_NOTRANS_TIMEOUT_US);
}

status_t SF_SendCommandNonBlock(sf_drv_data_t *drvData, sf_spi_cmd_t cmd,
        const sf_msg_payload_t *sendPayload)
{
    uint8_t  spiSndBuf[SF_INF_SPI_MSG_MAX_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_INF_SPI_MSG_MAX_B]; /* SPI receive buffer. */
    spi_aml_transfer_t amlSpiData = {0};      /* SPI transmission structure. */

    /* Preconditions. */
    AML_ASSERT(drvData != NULL);
    AML_ASSERT((cmd >= SF_CMD_FIRST_ID) && (cmd <= SF_CMD_LAST_ID));
    AML_ASSERT(sendPayload != NULL);
    AML_ASSERT(sendPayload->payloadLen <= SF_INF_PAYLOAD_MAX_B);

    amlSpiData.txBuffer = spiSndBuf;
    amlSpiData.rxBuffer = spiRcvBuf;
    amlSpiData.dataSize = sendPayload->payloadLen + SF_INF_HEADER_B;

    SF_PackFrame(cmd, sendPayload->payloadLen, sendPayload->payload, (uint8_t*)spiSndBuf);
    return SF_SendSpiData(drvData, &amlSpiData);
}

bool SF_IsAckFrameReady(sf_drv_data_t *drvData)
{
    AML_ASSERT(drvData != NULL);

    return (GPIO_AML_ReadInput(drvData->gpioConfig.ackPin.gpioInstance,
            drvData->gpioConfig.ackPin.gpioPinNumber) == 0U);
}

status_t SF_ReadAckFrameNonBlock(sf_drv_data_t *drvData,
        sf_msg_payload_t *recvPayload, uint8_t recvBufferSize)
{
    status_t status = kStatus_Success;
    uint8_t  spiSndBuf[SF_ACK_SPI_MSG_MAX_B]; /* SPI send buffer. */
    uint8_t  spiRcvBuf[SF_ACK_SPI_MSG_MAX_B]; /* SPI receive buffer. */
    spi_aml_transfer_t amlSpiData =           /* SPI transmission structure. */
    {
            (uint8_t*)spiSndBuf,
            (uint8_t*)spiRcvBuf,
            SF_ACK_SPI_MSG_MAX_B,
            0x00U
    };

    /* Preconditions. */
    AML_ASSERT(drvData != NULL);
    AML_ASSERT(recvPayload != NULL);
    AML_ASSERT(recvPayload->payload != NULL);

    /* Receive and process an ACK frame. */
    status = SF_ReceiveSpiData(drvData, &amlSpiData, SF_ACK_TRANS_RECV_TIMEOUT_US);

    /* Store items from the ACK frame. */
    if (kStatus_Success == status)
    {
        status = SF_StoreDevState(drvData, amlSpiData.rxBuffer);
    }
    if (kStatus_Success == status)
    {
        status = SF_StorePayload(amlSpiData.rxBuffer, recvPayload, recvBufferSize);
    }

    return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
