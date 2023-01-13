/*
 * Copyright 2015 Freescale Semiconductor, Inc.
 * Copyright 2015-2021 NXP.
 *
 * License: LA_OPT_NXP_Software_License
 *
 * This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms. If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * @version 4.2
 *
 * @brief TSI test common routines - IEC60730 Class B.
 */

#ifndef IEC60730B_TSI_H_
#define IEC60730B_TSI_H_

/*
 * List of devices and supported functions can be found in library user guide,
 * section Core self test library - source code version.
 */

/******************************************************************************
 * Macros
 ******************************************************************************/
/* Return values */
#define FS_FAIL_TSI             (FS_TSI_CODE + 0x1U)
#define FS_TSI_PROGRESS_NONSTIM (FS_TSI_CODE + 0x2U)
#define FS_TSI_PASS_NONSTIM     (FS_TSI_CODE + 0x3U)
#define FS_TSI_PROGRESS_STIM    (FS_TSI_CODE + 0x4U)
#define FS_TSI_PASS_STIM        (FS_TSI_CODE + 0x5U)
#define FS_TSI_INPROGRESS       (FS_TSI_CODE + 0x6U)
#define FS_TSI_INIT             (FS_TSI_CODE + 0x7U)
#define FS_TSI_INCORRECT_CALL   (FS_TSI_CODE + 0x8U)

#define SAFETY_SELFCAP_MODE 0xFFU

/* Assembler skips following section */
#ifndef __ASM__

/******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
    uint32_t low;
    uint32_t high;
    int32_t delta;
    int32_t deltaDev;
    int32_t deltaPer;
} fs_tsi_limits_t;

typedef struct
{
    uint16_t tsicnt;
    uint16_t tsicntStim;
    int32_t tsicntDelta;
} fs_tsi_data_t;

typedef struct
{
    uint8_t rxCH;
    uint8_t txCH; /* If this valua is "-1"  => chanel is in SELF CAP MODE */
} fs_tsi_input_t;

typedef struct
{
    uint32_t rxGpio;
    uint32_t rxPcr;
    uint16_t rxPinNum;
    uint32_t txGpio;
    uint16_t txPinNum;
} fs_tsi_gpio_input_t;

/* Used to backup original DIO port/pin configuration */
typedef struct
{
    uint32_t pcr;
    uint32_t pddr;
    uint32_t pdor;
} fs_tsi_backup_t;

typedef struct
{
    uint32_t counterValue;
    uint32_t counterLimit;
} fs_tsi_counter_t;

typedef struct
{
    fs_tsi_limits_t limits;
    fs_tsi_input_t input;
    fs_tsi_gpio_input_t gpioInput;
    fs_tsi_data_t data;
    fs_tsi_backup_t inputRegBackup;
    fs_tsi_counter_t tsiCounter;
    FS_RESULT state;
    uint8_t stimPolarity;
    uint32_t pPcrBase;
} fs_tsi_t;

/******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 *
 * @brief    The function initializes respective items in the defined
 *           fs_tsi_tstructure and sets state to FS_TSI_INIT.
 *           It should be called prior to non-stimulated input test.
 *
 * @param    *pObj - input argument is pointer to tsi test instance.
 *
 * @return   void
 *
 ******************************************************************************/
void FS_TSI_InputInit(fs_tsi_t *pObj);

/*******************************************************************************
 *
 * @brief    The function stimulates the appropriate TSI pin by pull-resistor
 *           on the current TSI channel.
 *           The pull-up/down polarity is given by parameter "stim_polarity"
 *           in structure fs_tsi.
 *
 * @param    *pObj - input argument is pointer to tsi test instance.
 *
 * @return   FS_PASS
 *           FS_FAIL_TSI
 *
 ******************************************************************************/
FS_RESULT FS_TSI_InputStimulate(fs_tsi_t *pObj);

/*******************************************************************************
 *
 * @brief    The function disables the pull-resistor on the appropriate TSI channel.
 *
 * @param    *pObj - input argument is pointer to tsi test instance.
 *
 * @return   FS_PASS
 *           FS_FAIL_TSI
 *
 ******************************************************************************/
FS_RESULT FS_TSI_InputRelease(fs_tsi_t *pObj);

/*******************************************************************************
 *
 * @brief    Function performs the TSI test with non-stimulated input,
 *           check if the TSI counter value stays in the predefined tolerance range,
 *           gets the reference TSI counter data for the non-stimulated test.
 *
 * @param    *pObj - input argument is pointer to tsi test instance.
 * @param    pTsi - input argument is adress of tsi module.
 *
 * @return   FS_TSI_PASS_NONSTIM
 *           FS_TSI_INCORRECT_CALL
 *           FS_FAIL_TSI
 *
 ******************************************************************************/
FS_RESULT FS_TSI_InputCheckNONStimulated(fs_tsi_t *pObj, uint32_t pTsi);

/*******************************************************************************
 *
 * @brief    Function performs the TSI test with stimulated input,
 *           checks if the TSI input stimulated counter delta is in the expected range.
 *           The test function can be called only after the non-stimulated test is passed.
 *           Otherwise FS_TSI_INCORRECT_CALL will be returned.
 *
 * @param    *pObj - input argument is pointer to tsi test instance.
 * @param    pTsi - input argument is adress of tsi module.
 *
 * @return   FS_TSI_PASS_STIM
 *           FS_TSI_INCORRECT_CALL
 *           FS_FAIL_TSI
 *
 ******************************************************************************/
FS_RESULT FS_TSI_InputCheckStimulated(fs_tsi_t *pObj, uint32_t pTsi);

#endif /* __ASM__ */
#endif /* IEC60730B_TSI_H_ */
