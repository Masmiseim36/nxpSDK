/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef DMA_CAPTURE_H
/* clang-format off */
#define DMA_CAPTURE_H
/* clang-format on */

#include "fsl_common.h"
#include "fsl_device_registers.h"
#include "fsl_edma.h"

/*!
 * @addtogroup xcvr
 * @{
 */

/*! @file*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Page definitions */
#define DMA_PAGE_IDLE           (0x00)
#define DMA_PAGE_RXDIGIQ        (0x01)
#define DMA_PAGE_RXDIGI         (0x02)
#define DMA_PAGE_RXDIGQ         (0x03)
#define DMA_PAGE_RAWADCIQ       (0x04)
#define DMA_PAGE_RAWADCI        (0x05)
#define DMA_PAGE_RAWADCQ        (0x06)
#define DMA_PAGE_DCESTIQ        (0x07)
#define DMA_PAGE_DCESTI         (0x08)
#define DMA_PAGE_DCESTQ         (0x09)
#define DMA_PAGE_RXINPH         (0x0A)
#define DMA_PAGE_DEMOD_HARD     (0x0B)
#define DMA_PAGE_DEMOD_SOFT     (0x0C)
#define DMA_PAGE_DEMOD_DATA     (0x0D)
#define DMA_PAGE_DEMOD_CFO_PH   (0x0E)

#define DMA_PAGE_MAX   (0x0F)

/* Define callback function for DMA capture. */
typedef void (*dma_capture_callback)(struct _edma_handle *handle,
                                     void *userData, bool transferDone,
                                     uint32_t tcds);

typedef struct
{  
    /* EDMA config and handler */
    edma_handle_t g_EDMA_Handle;
    edma_config_t dmaConfig;

    /* capture temp variables */
    uint8_t osr_temp;
    volatile bool g_Transfer_Done;

    /* User defined callback function */
    dma_capture_callback user_callback; 
} dma_capture_config_t;

typedef struct
{
    uint8_t dma_page;
    uint8_t osr;
} dma_capture_lut_t;

typedef enum _dmaStatus
{
    DMA_SUCCESS = 0,
    DMA_FAIL_SAMPLE_NUM_LIMIT = 1,
    DMA_FAIL_PAGE_ERROR = 2,
    DMA_FAIL_NULL_POINTER = 3,
    DMA_INVALID_TRIG_SETTING = 4,
    DMA_FAIL_NOT_ENOUGH_SAMPLES = 5,
    DMA_FAIL_INVALID_CHANNEL = 6,
    DMA_CAPTURE_NOT_COMPLETE = 7,     /* Not an error response, but an indication that capture isn't complete for status polling. */
    DMA_CAPTURE_TIMEOUT = 8,
} dmaStatus_t; 

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
typedef enum _dmaStartTriggerType
{
    NO_DMA_START_TRIG = 0,
    START_DMA_ON_FSK_PREAMBLE_FOUND = 1,
    START_DMA_ON_FSK_AA_MATCH = 2,
    START_DMA_ON_ZBDEMOD_PREAMBLE_FOUND = 3,
    START_DMA_ON_ZBDEMOD_SFD_MATCH = 4,
    START_DMA_ON_AGC_DCOC_GAIN_CHG = 5,
    START_DMA_ON_TSM_RX_DIG_EN = 6,
    START_DMA_ON_TSM_SPARE2_EN = 7,
    INVALID_DMA_START_TRIG = 8
} dmaStartTriggerType;

#endif /* (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1) */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! *********************************************************************************
* \brief  This function prepares for sample capture to system RAM using DMA.
*
* \param[in] dma_channel_num - The channel number to use for the DMA transfer.
* \param[in] dma_callback - User callback to be called after transfer completed.
* \param[in] user_data - User data to be passed to the callback
*
* \return Status of the request, DMA_SUCCESS if initialization is complete.
*
* \details 
* This routine assumes that some other functions in the calling routine both set
* the channel and force RX warmup before calling the appropriate capture routine.
* This routine may corrupt the OSR value of the system but restores it in the ::dma_release() routine.
***********************************************************************************/
dmaStatus_t dma_init(uint8_t dma_channel_num, dma_capture_callback dma_callback, void *user_data);

/*! *********************************************************************************
* \brief  This function prepares for sample capture to system RAM using DMA and uses the currently set OSR.
*
* \param[in] dma_channel_num - The channel number to use for the DMA transfer.
* \param[in] dma_callback - User callback to be called after transfer completed.
* \param[in] user_data - User data to be passed to the callback
*
* \return Status of the request, DMA_SUCCESS if initialization is complete.
*
* \details 
* This routine assumes that some other functions in the calling routine both set
* the channel and force RX warmup before calling the appropriate capture routine.
* This routine prevents and corruption the OSR value of the system and assumes that the user intends to capture
* DMA samples at the OSR specified by the current radio mode. This may result in corrupted DMA samples (decimated).
***********************************************************************************/
dmaStatus_t dma_init_lock_osr(uint8_t dma_channel_num, dma_capture_callback dma_callback, void *user_data);

/*! *********************************************************************************
* \brief  This function performs a blocking wait for completion of the capture of transceiver data to the system RAM.
*
* \return Status of the request, DMA_SUCCESS if capture is complete.
*
* \details 
* This function performs a wait loop for capture completion and may take an indeterminate amount of time for 
* some capture trigger types.
* Blocking wait for capture completion, no matter what trigger type.
***********************************************************************************/
dmaStatus_t dma_wait_for_complete(void);

/*! *********************************************************************************
* \brief  This function polls the state of the capture of transceiver data to the transceiver packet RAM.
*
* \return Status of the request, DMA_SUCCESS if capture is complete, DMA_CAPTURE_NOT_COMPLETE if not complete.
*
* \details
* Non-blocking completion check, just reads the current status of the capure.
***********************************************************************************/
dmaStatus_t dma_poll_capture_status(void);

/*! *********************************************************************************
* \brief  This function aborts an in-progress DMA capture to system RAM.
* 
* \details
* User must still call dma_release() to completely release the DMA setup and restore any settings.
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

#if (RADIO_IS_GEN_3P0 || RADIO_IS_GEN_2P1)
/*! *********************************************************************************
* \brief  This function initiates the capture of transceiver data to the system RAM.
*
* \param[in] dbg_page - The page selector (DBG_PAGE).
* \param[in] buffer_sz_bytes - The size of the output buffer (in bytes)
* \param[in] result_buffer - The pointer to the output buffer of a size large enough for the samples.
* \param[in] dbg_start_trigger - The trigger to start acquisition.
*
* \return Status of the request.
*
* \details 
* This function starts the process of capturing data to the system RAM. Depending upon the start  trigger
* settings, the actual capture process can take an indeterminate amount of time. Other APIs are provided to 
* perform a blocking wait for completion or allow polling for completion of the capture.
* After any capture has completed, data will be left in system RAM.
* \warning
* This function modifies the OSR due to limitations on DMA speed (2MHz capture rate is the maximum) for IQ 
* sample capture. 
* This function may enable the RADIO_GASKET_BYPASS_OVRD bit in order to support the 2MHz capture rate for
* IQ samples. This capture rate can only be supported if the SOC clock frequency is 48MHz during the time that
* the capture is being performed. During the time that RADIO_GASKET_BYPASS_OVRD is set, XCVR register writes
* may produce a glitch to the logic that they control so these writes should be minimized or avoided.
* The ::dma_release() function must be called at the completion of capture in order to restore the prior OSR
* setting and release the RADIO_GASKET_BYPASS_OVRD.
***********************************************************************************/
dmaStatus_t dma_start_capture(uint8_t dma_page, uint32_t buffer_sz_bytes, void * result_buffer, dmaStartTriggerType start_trig);

#else /* Gen 2.0 version */
/*! *********************************************************************************
* \brief  This function captures transceiver data to the transceiver packet RAM.
*
* \param[in] dma_page - The page selector (DBG_PAGE).
* \param[in] buffer_sz_bytes - The size of the output buffer (in bytes)
* \param[in] result_buffer - The pointer to the output buffer of a size large enough for the samples.
*
* \return None.
*
* \details 
* The capture to system RAM . The samples will be 
* copied to the buffer pointed to by result_buffer parameter until buffer_sz_bytes worth of data have
* been copied. Data will be copied 
* NOTE: This routine has a slight hazard of getting stuck waiting for the specified number of bytes when RX has
* not been enabled or RX ends before the specified byte count is achieved (such as when capturing packet data ). It is 
* intended to be used with manually triggered RX where RX data will continue as long as needed.
***********************************************************************************/
dmaStatus_t dma_capture(uint8_t dma_page, uint16_t buffer_sz_bytes, uint32_t * result_buffer);
#endif /* RADIO_IS_GEN_3P0 */

/*! *********************************************************************************
* \brief  This function performs sign extension of 12-bit signed values to 16-bit signed values.
*
* \param[in] buffer_ptr - The pointer to the buffer containing samples to be processed.
* \param[in] count - The number of samples to be processed.
*
* \return None.
*
* \details 
* This routine performs sign extension in place.
***********************************************************************************/
void sign_extend_12_16(uint16_t * buffer_ptr, uint32_t count);

/*! *********************************************************************************
* \brief  This function performs sign extension of 5-bit signed values to 8-bit signed values.
*
* \param[in] buffer_ptr - The pointer to the buffer containing samples to be processed.
* \param[in] count - The number of samples to be processed.
*
* \return None.
*
* \details 
* This routine performs sign extension in place.
***********************************************************************************/
void sign_extend_5_8(uint8_t * buffer_ptr, uint32_t count);

/*! *********************************************************************************
* \brief  This function performs conversion from IQ samples to 4 byte float phase.
*
* \param[in] iq_buffer - The pointer to the buffer containing IQ samples to be processed. I first, then Q.
* \param[in] phase_buffer - The pointer to the buffer where floating point phase should be stored.
* \param[in] num_points - The number of sample pairs to be processed.
*
* \return None.
*
* \details 
* The incoming IQ samples must be stored in pairs with I first, then Q. They must be
* sign extended already to 16 bit integers. 
***********************************************************************************/
void iq_to_phase_float(uint16_t * iq_buffer, float * phase_buffer, uint16_t num_points);

/*! *********************************************************************************
* \brief  This function performs conversion from IQ samples to fixed point S(3,12) phase.
*
* \param[in] iq_buffer - The pointer to the buffer containing IQ samples to be processed. I first, then Q.
* \param[in] phase_buffer - The pointer to the buffer where fixed point phase should be stored.
* \param[in] num_points - The number of sample pairs to be processed.
*
* \return None.
*
* \details 
* The incoming IQ samples must be stored in pairs with I first, then Q. They must be
* sign extended already to 16 bit integers.  The output phase is stored as a fixed point with 1 sign bit, 3 bits
* integer portion, and 12 bits  fractional portion. Output phase values range from -pi to +pi.
***********************************************************************************/
void iq_to_phase_fixedpt(uint16_t * iq_buffer, uint16_t * phase_buffer, uint16_t num_points);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* DMA_CAPTURE_H */

