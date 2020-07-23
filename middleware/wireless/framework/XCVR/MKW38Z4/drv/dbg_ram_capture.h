/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef DBG_RAM_CAPTURE_H
/* clang-format off */
#define DBG_RAM_CAPTURE_H
/* clang-format on */

#include "fsl_common.h"
#include "nxp2p4_xcvr.h"


/*!
 * @addtogroup pkt_ram_capture Packet RAM Sample Capture Routines
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DBG_RAM_START_DELAY_MAX (2048U) /*!< Maximum start delay to be used  */

/* Define callback function for DBG RAM  capture. */
typedef void (*dbg_ram_capture_callback)(void *userData, bool transferDone, uint32_t count);

/*! @brief DBG RAM Page selections. */
typedef enum
{
    DBG_RAM_PAGE_IDLE = 0,           /*!< IDLE page, used to stop DMA capture. */
    DBG_RAM_PAGE_RXDIGIQ = 1,        /*!< RXDIG I and Q sample capture. Data is MSB aligned in 16bit portions of the 32bit word. */
    DBG_RAM_PAGE_RXDIGIQ_ALT = 2,    /*!< RXDIG I and Q sample capture plus additional signals in unused bits. Data is MSB aligned in 16bit portions of the 32bit word. */
    DBG_RAM_PAGE_RSVD1 = 3,          /*!< Reserved. */
    DBG_RAM_PAGE_RSVD2 = 4,          /*!< Reserved. */
    DBG_RAM_PAGE_PHASE = 5,          /*!< Wide-band phase, narrowband phase, or high-precision phase (as selected in RXDIG). Newest sample in most significant byte. Each byte contains a MSB-aligned phase sample.*/
    DBG_RAM_PAGE_RSSI_PHASE = 6,     /*!< 8bit RSSI with 8bit high-resolution phase. Newest sample in most significant half-word.*/
    DBG_RAM_PAGE_MAG_PHASE = 7,      /*!< RSSI magnitude with 8bit high-resolution phase.*/
    DBG_RAM_PAGE_RSVD3 = 8,          /*!< Reserved. */
    DBG_RAM_PAGE_RSVD4 = 9,          /*!< Reserved. */
    DBG_RAM_PAGE_MAX = 10,           /*!< Upper limit of DMA page setting, used for error checking. */
} dbgRamPageType_t;


/*! @brief Status return values for DBG RAM routines. */
typedef enum
{
    DBG_RAM_SUCCESS = 0,                /*!< The operation or request was successful.  */
    DBG_RAM_FAIL_SAMPLE_NUM_LIMIT = 1,  /*!< The request exceeded the sample number limit. */
    DBG_RAM_FAIL_PAGE_ERROR = 2,        /*!< The request used an invalid DBG RAM page number.  */
    DBG_RAM_FAIL_NULL_POINTER = 3,      /*!< The request passed in a null pointer. */
    DBG_RAM_INVALID_TRIG_SETTING = 4,   /*!< The request provided an invalid trigger setting. */
    DBG_RAM_FAIL_NOT_ENOUGH_SAMPLES = 5, /*!< The request failed due to insufficient samples captured. */
    DBG_RAM_CAPTURE_NOT_COMPLETE = 6,   /* not an error response, but an indication that capture isn't complete for status polling */
    DBG_RAM_INVALID_DECIMATE_SETTING = 7, /*!< The request provided an invalid decimation setting. */
    DBG_RAM_INVALID_EDGE_SETTING = 8,   /*!< The request provided an invalid start edge setting. */
    DBG_RAM_INVALID_START_DELAY = 9,    /*!< The request provided an invalid start delay setting. */
    DBG_RAM_INVALID_OUT_SEL = 10,       /*!< The request provided an invalid output selection. */
    DBG_RAM_INVALID_ADDRESS = 11,       /*!< The request provided an invalid address for the start or stop of the capture. */
} dbgRamStatus_t; 

/*! @brief Start Trigger selections. */
typedef enum
{
    NO_START_TRIG = 0,                  /*!< Immediate Start Trigger, sample capture starts as soon as DMA Page is non-zero.  */
    START_ON_FSK_PREAMBLE_FOUND = 1,    /*!< Preamble Found Start Trigger.  */
    START_ON_FSK_AA_MATCH = 2,          /*!< Access Address Match Start Trigger.  */
    START_ON_RESERVED1 = 3,             /*!< Reserved.  */
    START_ON_RESERVED2 = 4,             /*!< Reserved.  */
    START_ON_AGC_DCOC_GAIN_CHG = 5,     /*!< AGC DCOC gain change. */
    START_ON_TSM_RX_DIG_EN = 6,         /*!< RX Digital Enable Start Trigger (TSM signal when RX warmup starts).  */
    START_ON_TSM_SPARE2_EN = 7,         /*!< TSM SPARE2 Start Trigger (based on custom TSM timing signal for TSM_IRQ0). */
    START_ON_RBME_CRC_PASS = 8,         /*!< CRC Passing Start Trigger (asserted when CRC result is passing). */
    START_ON_RBME_CRC_CMPLT = 9,        /*!< CRC Processing Complete Start Trigger (asserted when CRC processing completes, no matter what the result). */
    START_ON_LCL_DMA_TRIGD = 10,        /*!< Localization control Start Trigger: DMA capture triggered */
    START_ON_CTE_PRESENT = 11,          /*!< CTE Tone Present Start Trigger.  */
    INVALID_START_TRIG = 12             /*!< Upper limit of DBG RAM Start trigger setting, used for error checking. */
} dbgRamStartTriggerType_t;

/*! @brief Stop Trigger selections. */
typedef enum
{
    NO_STOP_TRIG = 0,                   /*!< No Stop trigger defined */
    STOP_ON_FSK_PREAMBLE_FOUND = 1,     /*!< Preamble Found Stop Trigger.  */        
    STOP_ON_FSK_AA_MATCH = 2,           /*!< Access Address Match Stop Trigger. */
    STOP_ON_RESERVED1 = 3,              /*!< Reserved.  */
    STOP_ON_RESERVED2 = 4,              /*!< Reserved.  */
    STOP_ON_AGC_DCOC_GAIN_CHG = 5,      /*!< AGC DCOC gain change Stop Trigger. */
    STOP_ON_TSM_RX_DIG_EN = 6,          /*!< RX Digital Enable Stop Trigger (TSM signal when RX warmup starts).  */
    STOP_ON_TSM_SPARE3_EN = 7,          /*!< TSM SPARE3 Stop Trigger (based on custom TSM timing signal for TSM_IRQ0). */
    STOP_ON_RBME_CRC_FAIL = 8,          /*!< CRC Fail Stop Trigger (asserted when CRC result is failing). */
    STOP_ON_RBME_CRC_VLD = 9,           /*!< CRC Passing Stop Trigger (asserted when CRC result is valid). */
    STOP_ON_RBME_ERROR = 10,            /*!< RBME error Stop Trigger (asserted when RBME engine fails). */
    STOP_ON_GENFSK_HEADER_FAIL = 11,    /*!< Generic FSK header fail Stop Trigger */
    STOP_ON_PLL_UNLOCK = 12,            /*!< PLL unlock Stop Trigger */
    STOP_ON_BLE_CRC_ERR_INCR = 13,      /*!< BLE CRC error incremented Stop Trigger */
    INVALID_STOP_TRIG = 14              /*!< Upper limit of DBG RAM Stop trigger setting, used for error checking. */
} dbgRamStopTriggerType_t;

/*! @brief DBG RAM decimation rate selections. */
typedef enum
{
    DBG_RAM_NO_DECIMATE = 0,         /*!< Capture data on every data valid strobe. */
    DBG_RAM_DECIMATE_BY_2 = 1,       /*!< Capture data on every 2nd data valid strobe. */
    DBG_RAM_DECIMATE_BY_4 = 2,       /*!< Capture data on every 4th data valid strobe. */
    DBG_RAM_DECIMATE_BY_8 = 3,       /*!< Capture data on every 8th data valid strobe. */
    DBG_RAM_DECIMATE_MAX = 4,        /*!< Upper limit of DMA decimate setting, used for error checking. */
} dbgRamDecimationType_t;

/*! @brief DBG RAM capture edge selections. */
typedef enum
{
    DBG_RAM_EDGE_RISING = 0,         /*!< Capture trigger fires on the rising edge fo the selected trigger source. */
    DBG_RAM_EDGE_FALLIN = 1,         /*!< Capture trigger fires on the falling edge fo the selected trigger source. */
    DBG_RAM_CAPTURE_EDGE_MAX = 2,    /*!< Upper limit of DMA capture edge setting, used for error checking. */
} dbgRamCaptureEdgeType_t;

#if defined(RADIO_IS_GEN_3P5)
/*! @brief DBG RAM capture output selections. */
typedef enum
{
    DBG_RAM_OUT_DEC_FILTER_SEL = 0,         /*!< Decimation filter I/Q output */
    DBG_RAM_OUT_IQMC_SEL = 1,               /*!< IQMC I/Q output */
    DBG_RAM_OUT_DCOC_SEL = 2,               /*!< DCOC I/Q output */
    DBG_RAM_OUT_CH_FILTER_SEL = 3,          /*!< Channel filter I/Q output */
    DBG_RAM_OUT_SRC_SEL = 4,                /*!< SRC I/Q output */
    DBG_RAM_OUT_DC_RES_SEL = 5,             /*!< DC Residual I/Q output */
    DBG_RAM_OUT_SAMPLE_BUFF_SEL = 6,        /*!< Sample Buffer I/Q output */
    DBG_RAM_OUT_CFO_MIXER_SEL = 7,          /*!< CFO Mixer I/Q output */
    DBG_RAM_OUT_DEMOD_CH_FILTER_SEL = 8,    /*!< Demod Channel filter I/Q output */
    DBG_RAM_OUT_FRAC_CORR_SEL = 9,          /*!< Frac Corr I/Q output */
    DBG_RAM_OUT_INVALID_SEL = 10,           /*!< Invalid output */
} dbgRamOutputSel_t;

#elif defined(RADIO_IS_GEN_4P0)
/*! @brief DBG RAM capture output selections. */
typedef enum
{
    DBG_RAM_OUT_DISABLED_SEL = 0,           /*!< Disabled output */
    DBG_RAM_OUT_IF_MIXER_SEL = 1,           /*!< IF mixer I/Q output */
    DBG_RAM_OUT_CIC_SEL = 2,                /*!< CIC output */
    DBG_RAM_OUT_CH_FILTER_SEL = 3,          /*!< Channel filter I/Q output */
    DBG_RAM_OUT_SRC_SEL = 4,                /*!< SRC I/Q output */
    DBG_RAM_OUT_CFO_MIXER_SEL = 5,          /*!< CFO Mixer I/Q output */
    DBG_RAM_OUT_DEMOD_CH_FILTER_SEL = 6,    /*!< Demod Channel filter I/Q output */
    DBG_RAM_OUT_INVALID_SEL = 7,            /*!< Invalid output*/
} dbgRamOutputSel_t;
#endif
/*! @brief DBG RAM capture settings structure. */
typedef struct
{  
    uint16_t buffer_sz_bytes;         /*!< The number of bytes to capture. */
    void * result_buffer;             /*!< The location in system RAM where the results should be copied. */
    int16_t * dbg_ram_start_addr;       /*!< The start location in DBG RAM where radio samples are captured. */
} dbg_ram_capture_config_t;


/*! @brief DBG RAM trigger settings structure. */
typedef struct
{  
    dbgRamOutputSel_t out_sel;                  /*!< The selector for data acqusition. */
    dbgRamStartTriggerType_t start_trig;        /*!< The start trigger to cause samples to be captured. */
    dbgRamStopTriggerType_t stop_trig;          /*!< The stop trigger to cause samples capture to end. */
    dbgRamCaptureEdgeType_t start_capture_edge; /*!< The edge on which start trigger should be applied (rising/falling). */
    dbgRamCaptureEdgeType_t stop_capture_edge;  /*!< The edge on which stop trigger should be applied (rising/falling). */
    dbgRamDecimationType_t decimation;          /*!< The decimation factor to be applied during sample capture. */
    uint16_t start_delay;                       /*!< The delay time to be inserted after a start trigger, before samples are captured. */
} dbg_ram_trigger_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*! *********************************************************************************
* \brief  This function prepares for sample capture to packet RAM.
*
* \param[in] dbg_ram_configuration The configuration structure controlling the setup of the DBG RAM capture.
*
* \return None.
*
* \details 
* This routine assumes that some other functions in the calling routine both set
 * the RF channel and force RX warmup before calling ::dbg_ram_start_capture().
***********************************************************************************/
dbgRamStatus_t dbg_ram_init(const dbg_ram_capture_config_t * dbg_ram_configuration);

/*! *********************************************************************************
* \brief  This function performs any state restoration at the completion of PKT RAM capture.
* 
* \details
* Any clocks enabled to the packet RAM capture circuitry are disabled.
***********************************************************************************/
void dbg_ram_release(void);

/*! *********************************************************************************
* \brief  This function initiates the capture of transceiver data to the transceiver packet RAM.
*
* \param[in] dbg_ram_page DBG RAM Page to be captured 
* \param[in] dbg_ram_trigger_config The configuration structure with the setup of the DBG RAM capture trigger.
*
* \return Status of the request.
*
* \details 
* This function starts the process of capturing data to the packet RAM. Depending upon the start and stop trigger
* settings, the actual capture process can take an indeterminate amount of time. Other APIs are provided to 
* perform a blocking wait for completion or allow polling for completion of the capture.
* After any capture has completed, a separate routine must be called to postprocess the capture and copy all
* data out of the packet RAM into a normal RAM buffer.
***********************************************************************************/
dbgRamStatus_t dbg_ram_start_capture(dbgRamPageType_t dbg_ram_page, const dbg_ram_trigger_config_t * dbg_ram_trigger_config);

/*! *********************************************************************************
* \brief  This function performs a blocking wait for completion of the capture of transceiver data to the transceiver packet RAM.
*
* \details 
* This function performs a wait loop for capture completion and may take an indeterminate amount of time for 
* some capture trigger types.
***********************************************************************************/
void dbg_ram_wait_for_complete(void);

/*! *********************************************************************************
* \brief  This function polls the state of the capture of transceiver data to the transceiver packet RAM.
*
* \return Status of the request, DBG_RAM_SUCCESS if capture is complete, DBG_RAM_CAPTURE_NOT_COMPLETE if not complete.
* \details 
* This function polls the status of the capture and returns without blocking.
***********************************************************************************/
dbgRamStatus_t dbg_ram_poll_capture_status(void);

/*! *********************************************************************************
* \brief  This function processes the captured data into a usable order and copies from packet RAM to normal RAM.
*
* \param[in] dbg_ram_configuration The configuration structure controlling the setup of the DBG RAM capture.
*
* \return None.
*
* \details 
* Data is copied from packet RAM in bytes to ensure no access problems. Data is unpacked from packet RAM 
* (either sequentially captured or simultaneously captured) into a linear RAM buffer in system RAM. 
* If a start trigger is enabled then the first buffer_sz_bytes that are captured are copied out.
***********************************************************************************/
dbgRamStatus_t dbg_ram_postproc_capture(const dbg_ram_capture_config_t * dbg_ram_configuration);

#ifdef GCOV_DO_COVERAGE /* routine is local except when testing code coverage */
dbgRamStatus_t unpack_sequential_data(int16_t * pkt_ram_location, uint16_t num_bytes, uint8_t * result_buffer);
dbgRamStatus_t dbg_ram_set_trigger(const dbg_ram_trigger_config_t * dbg_ram_trigger_config);
bool IN_TX_RAM_RANGE(uint32_t start, uint32_t stop);
bool IN_RX_RAM_RANGE(uint32_t start, uint32_t stop);
#endif /* defined(GCOV_DO_COVERAGE) */


/* @} */

#if defined(__cplusplus)
}
#endif


#endif /* DBG_RAM_CAPTURE_H */


