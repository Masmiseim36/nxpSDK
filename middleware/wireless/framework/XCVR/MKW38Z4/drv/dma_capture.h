/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DMA_CAPTURE_H
/* clang-format off */
#define DMA_CAPTURE_H
/* clang-format on */

#include "fsl_common.h"
#include "fsl_device_registers.h"
#include "nxp2p4_xcvr.h"

/*!
 * @addtogroup dma_capture DMA Sample Capture Routines
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Define DSB FIFO size */
#define DSB_FIFO_SIZE   (16U)   /*!< Maximum FIFO size  */
#define DMA_START_DELAY_MAX (2048U) /*!< Maximum start delay to be used  */
   
/* Define callback function for DMA capture. */
/*! *********************************************************************************
* \brief  This callback is a pointer to a user routine to be called in case of DMA interrtupt.
*
*  This function is the user callback for DSB interrupts.
*
* \param[in] userData - pointer to user data to be passed to the callback function
* \param[in] transferDone - boolean indicating if a transfer was complete (if false then an error occurred).
* \param[in] count - the count of bytes transferred (transferDone == true) or the error interrupt bits (transferDone == false).
*
* \note This callback is called for any DSB interrupt, whether transfer is done or an error was asserted. The transferDone
*  parameter can be used to distinguish these cases.
***********************************************************************************/
typedef void (*dma_capture_callback)(void *userData, bool transferDone, uint16_t count);

/*! @brief DMA Status return values. */
typedef enum
{
    DMA_SUCCESS = 0,                  /*!< The operation or request was successful.  */
    DMA_FAIL_SAMPLE_NUM_LIMIT = 1,    /*!< The request exceeded the sample number limit or had zero buffer size. */
    DMA_FAIL_PAGE_ERROR = 2,          /*!< The request used an invalid DMA page number.  */
    DMA_FAIL_NULL_POINTER = 3,        /*!< The request passed in a null pointer. */
    DMA_INVALID_TRIG_SETTING = 4,     /*!< The request provided an invalid trigger setting. */
    DMA_FAIL_NOT_ENOUGH_SAMPLES = 5,  /*!< The request failed due to insufficient samples captured. */
    DMA_FAIL_INVALID_CHANNEL = 6,     /*!< The request used an invalid DMA channel number. */
    DMA_CAPTURE_NOT_COMPLETE = 7,     /*!< Not an error response, but an indication that capture isn't complete for status polling. */
    DMA_CAPTURE_TIMEOUT = 8,          /*!< The DMA operation timed out.  */
    DMA_INVALID_DECIMATE_SETTING = 9, /*!< The request provided an invalid decimation setting. */
    DMA_INVALID_EDGE_SETTING = 10,    /*!< The request provided an invalid start edge setting. */
    DMA_INVALID_START_DELAY = 11,     /*!< The request provided an invalid start delay setting. */
    DMA_INVALID_WTRMRK_VALUE = 12,    /*!< The request provided an invalid watermark value for the FIFO. */
    DMA_INVALID_OUTPUT_SETTING = 13,  /*!< The request provided an invalid output selection. */
} dmaStatus_t; 

/*! @brief DMA Start Trigger selections. */
typedef enum
{
    NO_DMA_START_TRIG = 0,                  /*!< Immediate Start Trigger, sample capture starts as soon as DMA Page is non-zero.  */
    START_DMA_ON_FSK_PREAMBLE_FOUND = 1,    /*!< Preamble Found Start Trigger.  */
    START_DMA_ON_FSK_AA_MATCH = 2,          /*!< Access Address Match Start Trigger.  */
    START_DMA_ON_RESERVED1 = 3,             /*!< Reserved.  */
    START_DMA_ON_RESERVED2 = 4,             /*!< Reserved. */
    START_DMA_ON_AGC_DCOC_GAIN_CHG = 5,     /*!< AGC DCOC gain change. */
    START_DMA_ON_TSM_RX_DIG_EN = 6,         /*!< RX Digital Enable Start Trigger (TSM signal when RX warmup starts).  */
    START_DMA_ON_TSM_SPARE2_EN = 7,         /*!< TSM SPARE2 Start Trigger (based on custom TSM timing signal for TSM_IRQ0). */
    START_DMA_ON_RBME_CRC_PASS = 8,         /*!< CRC Passing Start Trigger (asserted when CRC result is passing). */
    START_DMA_ON_RBME_CRC_CMPLT = 9,        /*!< CRC Processing Complete Start Trigger (asserted when CRC processing completes, no matter what the result). */
    START_DMA_ON_LCL_PATT_MATCH = 10,       /*!< Localization Pattern Match Start Trigger. */
    START_DMA_ON_GENLL_CTE_PRESENT = 11,    /*!< CTE Tone Present Start Trigger.  */
    INVALID_DMA_START_TRIG = 12             /*!< Upper limit of DMA start trigger setting, used for error checking. */
} dmaStartTriggerType_t;

/*! @brief DMA Page selections. */
typedef enum
{
    DMA_PAGE_IDLE = 0,           /*!< IDLE page, used to stop DMA capture. */
    DMA_PAGE_RXDIGIQ = 1,        /*!< RXDIG I and Q sample capture. Data is MSB aligned in 16bit portions of the 32bit word. */
    DMA_PAGE_RXDIGIQ_ALT = 2,    /*!< RXDIG I and Q sample capture plus additional signals in unused bits. Data is MSB aligned in 16bit portions of the 32bit word. */
    DMA_PAGE_RAWADCIQ = 3,       /*!< Raw ADC capture of I and Q channel. Newest bit is in the MS bit of each byte, and newest I/Q samples are in the upper half-word.*/
    DMA_PAGE_DCESTIQ = 4,        /*!< I and Q DCOC tracking estimate. Data is MSB aligned in each 16bit half-word.. */
    DMA_PAGE_PHASE = 5,          /*!< Wide-band phase, narrowband phase, or high-precision phase (as selected in RXDIG). Newest sample in most significant byte. Each byte contains a MSB-aligned phase sample.*/
    DMA_PAGE_RSSI_PHASE = 6,     /*!< 8bit RSSI with 8bit high-resolution phase. Newest sample in most significant half-word.*/
    DMA_PAGE_MAG_PHASE = 7,      /*!< RSSI magnitude with 8bit high-resolution phase.*/
    DMA_PAGE_PHY = 8,            /*!< As selected via programming of the PHY, various signals can be output on this 32bit bus for DMA capture. */
    DMA_PAGE_DETERMINISTIC = 9,  /*!< Mimics the RXDIGIQ page, but uses deterministic data for software debug purposes. The Q up-counter start from 0x800, the I up-counter starts from 0x000. */
    DMA_PAGE_MAX = 10,           /*!< Upper limit of DMA page setting, used for error checking. */
} dmaPageType_t;

/*! @brief DMA decimation rate selections. */
typedef enum
{
    DMA_NO_DECIMATE = 0,         /*!< Capture data on every data valid strobe. */
    DMA_DECIMATE_BY_2 = 1,       /*!< Capture data on every 2nd data valid strobe. */
    DMA_DECIMATE_BY_4 = 2,       /*!< Capture data on every 4th data valid strobe. */
    DMA_DECIMATE_BY_8 = 3,       /*!< Capture data on every 8th data valid strobe. */
    DMA_DECIMATE_MAX = 4,        /*!< Upper limit of DMA decimate setting, used for error checking. */
} dmaDecimationType_t;

/*! @brief DMA capture edge selections. */
typedef enum
{
    DMA_START_RISING = 0,         /*!< Capture trigger fires on the rising edge fo the selected trigger source. */
    DMA_START_FALLING = 1,        /*!< Capture trigger fires on the falling edge fo the selected trigger source. */
    DMA_CAPTURE_EDGE_MAX = 2,     /*!< Upper limit of DMA capture edge setting, used for error checking. */
} dmaCaptureEdgeType_t;

#if defined(RADIO_IS_GEN_3P5)
/*! @brief DMA capture output selections. */
typedef enum
{
    DMA_OUT_DEC_FILTER_SEL = 0,         /*!< Decimation filter I/Q output */
    DMA_OUT_IQMC_SEL = 1,               /*!< IQMC I/Q output */
    DMA_OUT_DCOC_SEL = 2,               /*!< DCOC I/Q output */
    DMA_OUT_CH_FILTER_SEL = 3,          /*!< Channel filter I/Q output */
    DMA_OUT_SRC_SEL = 4,                /*!< SRC I/Q output */
    DMA_OUT_DC_RES_SEL = 5,             /*!< DC Residual I/Q output */
    DMA_OUT_SAMPLE_BUFF_SEL = 6,        /*!< Sample Buffer I/Q output */
    DMA_OUT_CFO_MIXER_SEL = 7,          /*!< CFO Mixer I/Q output */
    DMA_OUT_DEMOD_CH_FILTER_SEL = 8,    /*!< Demod Channel filter I/Q output */
    DMA_OUT_FRAC_CORR_SEL = 9,          /*!< Frac Corr I/Q output */
    DMA_OUT_INVALID_SEL = 10,           /*!< Invalid output */
} dmaOutputSel_t;

#elif defined(RADIO_IS_GEN_4P0)
/*! @brief DMA capture output selections. */
typedef enum
{
    DMA_OUT_DISABLED_SEL = 0,           /*!< Disabled output */
    DMA_OUT_IF_MIXER_SEL = 1,           /*!< IF mixer I/Q output */
    DMA_OUT_CIC_SEL = 2,                /*!< CIC output */
    DMA_OUT_CH_FILTER_SEL = 3,          /*!< Channel filter I/Q output */
    DMA_OUT_SRC_SEL = 4,                /*!< SRC I/Q output */
    DMA_OUT_CFO_MIXER_SEL = 5,          /*!< CFO Mixer I/Q output */
    DMA_OUT_DEMOD_CH_FILTER_SEL = 6,    /*!< Demod Channel filter I/Q output */
    DMA_OUT_INVALID_SEL = 7,            /*!< Invalid output*/
} dmaOutputSel_t;
#endif

/* DMA sample capture settings strtucture*/
typedef struct
{    
    dmaPageType_t dma_page;             /*!< DMA page to be used. Refer to dmaPageType_t for more information */
    dmaOutputSel_t out_sel;             /*!< Select capturing output */
    dmaStartTriggerType_t start_trig;   /*!< Start DMA trigger to be used. Refer to dmaStartTriggerType_t for more information */
    uint16_t start_delay;               /*!< Start delay to be applied to the capture */
    dmaDecimationType_t decimation;     /*!< Decimate the output data */
    dmaCaptureEdgeType_t capture_edge;  /*!< Select capturing edge */
} dma_capture_config_t;

/* DSB settings structure*/
typedef struct
{
    void * result_buffer;               /*!< Output buffer pointer to store the data */
    uint32_t buffer_sz;                 /*!< Output buffer size */
    uint8_t watermark;                  /*!< FIFO Watermark value to perform DMA transfers */
    dma_capture_callback user_callback; /*!< User defined callback to be called when transfer is done */
    void *userData;                     /*!< User data available from callback */
} dsb_config_t;

/* DSB handler structure*/
typedef struct
{
    dsb_config_t dsb_config;            /*!< DSB configuration struct */
    volatile bool g_Transfer_Done;      /*!< Internal variable to track when transfers are done */
} dsb_handler_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! *********************************************************************************
* \brief  This function prepares for sample capture to system RAM using DSB DMA.
*
* \param[in] dsb_configuration The pointer to the complete DSB DMA configuration structure.
*
* \return Status of the request, DMA_SUCCESS if initialization is complete.
* \note Calling this function while a DMA capture operation is already in progress may corrupt that 
* capture operation.
***********************************************************************************/
dmaStatus_t dma_init(const dsb_config_t * dsb_configuration);

/*! *********************************************************************************
* \brief  This function refreshes DSB before repeated sample capture to system RAM using DSB DMA.
*
*  This function performs a subset of dma_init() that is required before a repeated call to dma_start_capture().
*
* \param[in] dsb_configuration The pointer to the complete DSB DMA configuration structure.
*
* \return Status of the request, DMA_SUCCESS if initialization is complete.
* \note Calling this function while a DMA capture operation is already in progress may corrupt that 
* capture operation.
***********************************************************************************/
dmaStatus_t dma_refresh(const dsb_config_t * dsb_configuration);

/*! *********************************************************************************
* \brief  This function performs a blocking wait for completion of the capture of transceiver data to the system RAM.
*
* \return Status of the request, DMA_SUCCESS if capture is complete.
*
* \details 
* This function performs a wait loop for capture completion and may take an indeterminate amount of time for 
* some capture trigger types.
* Blocking wait for capture completion, no matter what trigger type. Waits only for IRQ indicating transfer done.
***********************************************************************************/
dmaStatus_t dma_wait_for_complete(void);

/*! *********************************************************************************
* \brief  This function polls the state of the capture of transceiver data to the system RAM.
*
* \return Status of the request, DMA_SUCCESS if capture is complete, DMA_CAPTURE_NOT_COMPLETE if not complete.
*
* \details
* Non-blocking capture completion check, just reads the current status of the IRQ for capture. 
***********************************************************************************/
dmaStatus_t dma_poll_capture_status(void);

/*! *********************************************************************************
* \brief  This function performs a blocking wait for completion of the capture of transceiver data to the system RAM.
*
* \param[in] mask The OR'd mask to use for polling the bits in the DSB->INT register.
* \return Status of the request, DMA_SUCCESS if capture is complete. DMA_FAIL_NULL_POINTER if mask was == 0U
*
* \details 
* This function performs a wait loop for capture completion and may take an indeterminate amount of time for 
* some capture trigger types.
* Blocking wait for capture completion, no matter what trigger type. Waits for any of the bits in the mask
* to be asserted and returns.
***********************************************************************************/
dmaStatus_t dma_wait_for_done_bits(uint8_t mask);

/*! *********************************************************************************
* \brief  This function sets the DMA IRQ to enabled or masked.
*
* \details
* Enables/disables at both the NVIC and DSB block. 
***********************************************************************************/
void dma_set_irq_control(bool irq_enable);

/*! *********************************************************************************
* \brief  This function aborts an in-progress DMA capture to system RAM.
* 
* \details
* User must still call dma_release() to completely release the DMA setup and restore any settings if no
* additional DMA transactions are needed. If additonal DMA transactions are needed then dma_init() 
* must be called to re-initialize the DMA and DSB blocks.
***********************************************************************************/
void dma_abort(void);

/*! *********************************************************************************
* \brief  This function performs any state restoration at the completion of DMA capture to system RAM.
* 
* \details
* Because DMA capture only works at specific OSR values the DMA capture routines must force the OSR to the
* supported value when capturing data. This forced OSR value must be restored after DMA operations are complete.
***********************************************************************************/
void dma_release(void);


/*! *********************************************************************************
* \brief  This function initiates the capture of transceiver data to the system RAM.
*
* \param[in] dma_configuration The complete DMA configuration structure.
*
* \return Status of the request.
*
* \details 
* This function starts the process of capturing data to the system RAM. Depending upon the start  trigger
* settings, the actual capture process can take an indeterminate amount of time. Other APIs are provided to 
* perform a blocking wait for completion or allow polling for completion of the capture.
* After any capture has completed, data will be left in system RAM. Subsequent calls to this function will 
* place data in the same buffer.
* \warning
* If the transceiver is warmed down or stopped for other reasons before the the full number of samples is
* captured through the DSB then any application which waits on transfer done may hang.
***********************************************************************************/
dmaStatus_t dma_start_capture(const dma_capture_config_t * dma_configuration);


/*! *********************************************************************************
* \brief  This function performs conversion from IQ samples to floating point phase.
*
* \param[in] iq_buffer The pointer to the buffer containing IQ samples to be processed. I first, then Q.
* \param[in] phase_buffer The pointer to the buffer where floating point phase should be stored.
* \param[in] num_points The number of sample pairs to be processed.
*
* \return None.
*
* \pre 
* The incoming IQ samples must be stored in pairs with I first, then Q. They must be
* sign extended already to 16 bit integers. 
***********************************************************************************/
void iq_to_phase_float(const int16_t * iq_buffer, float32_t * phase_buffer, uint16_t num_points);

/*! *********************************************************************************
* \brief  This function performs conversion from IQ samples to fixed point S(3,12) phase.
*
* \param[in] iq_buffer The pointer to the buffer containing IQ samples to be processed. I first, then Q.
* \param[in] phase_buffer The pointer to the buffer where fixed point phase should be stored.
* \param[in] num_points The number of sample pairs to be processed.
*
* \return None.
*
* \pre 
* The incoming IQ samples must be stored in pairs with I first, then Q. They must be
* sign extended already to 16 bit integers.  
* \post
* The output phase is stored as a fixed point with 1 sign bit, 3 bits
* integer portion, and 12 bits  fractional portion. Output phase values range from -pi to +pi.
***********************************************************************************/
void iq_to_phase_fixedpt(const int16_t * iq_buffer, int16_t * phase_buffer, uint16_t num_points);

#if defined(GCOV_DO_COVERAGE)
/*! *********************************************************************************
* \brief  This function allows configuring the values of static state variables during unit testing.
*
* \param[in] addr The destination address value.
* \param[in] size The buffer size value.
*
* \return None.
*
* \note
* This routine is only visible when GCOV_DO_COVERAGE flag is true.
***********************************************************************************/
void dma_test_config_static_state(uint32_t * addr, uint32_t size);
#endif
/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* DMA_CAPTURE_H */


