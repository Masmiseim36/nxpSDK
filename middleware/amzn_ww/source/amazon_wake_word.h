/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _AMAZON_WAKE_WORD_H_
#define _AMAZON_WAKE_WORD_H_

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define AMZ_WW_MODEL_FILE_NAME "amz_ww_model.dat"
#define AMZ_WW_MODEL_LENGTH 6

#define LIGHTEN_MODEL_de_DE
#define LIGHTEN_MODEL_en_AU
//#define LIGHTEN_MODEL_en_GB
#define LIGHTEN_MODEL_en_IN
//#define LIGHTEN_MODEL_en_US
#define LIGHTEN_MODEL_es_ES
#define LIGHTEN_MODEL_fr_CA
//#define LIGHTEN_MODEL_fr_FR
#define LIGHTEN_MODEL_it_IT
#define LIGHTEN_MODEL_ja_JP

#define MAX_SIZE_CMD 5

#define AMZ_WW_SELFWAKE_DELAY_MS (500)

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern const char prlBinaryModelData_deDE[];
extern const char prlBinaryModelData_deDE_50KB[];
extern const char prlBinaryModelData_enAU[];
extern const char prlBinaryModelData_enAU_50KB[];
extern const char prlBinaryModelData_enGB[];
extern const char prlBinaryModelData_enGB_50KB[];
extern const char prlBinaryModelData_enIN[];
extern const char prlBinaryModelData_enIN_50KB[];
extern const char prlBinaryModelData_enUS[];
extern const char prlBinaryModelData_enUS_50KB[];
extern const char prlBinaryModelData_esES[];
extern const char prlBinaryModelData_esES_50KB[];
extern const char prlBinaryModelData_frCA[];
extern const char prlBinaryModelData_frCA_50KB[];
extern const char prlBinaryModelData_frFR[];
extern const char prlBinaryModelData_frFR_50KB[];
extern const char prlBinaryModelData_itIT[];
extern const char prlBinaryModelData_itIT_50KB[];
extern const char prlBinaryModelData_jaJP[];
extern const char prlBinaryModelData_jaJP_50KB[];

extern int prlBinaryModeldeDELen;
extern int prlBinaryModeldeDE50KBLen;
extern int prlBinaryModelenAULen;
extern int prlBinaryModelenAU50KBLen;
extern int prlBinaryModelenGBLen;
extern int prlBinaryModelenGB50KBLen;
extern int prlBinaryModelenINLen;
extern int prlBinaryModelenIN50KBLen;
extern int prlBinaryModelenUSLen;
extern int prlBinaryModelenUS50KBLen;
extern int prlBinaryModelesESLen;
extern int prlBinaryModelesES50KBLen;
extern int prlBinaryModelfrFRLen;
extern int prlBinaryModelfrFR50KBLen;
extern int prlBinaryModelitITLen;
extern int prlBinaryModelitIT50KBLen;
extern int prlBinaryModeljaJPLen;
extern int prlBinaryModeljaJP50KBLen;

struct ww_model_cmd
{
    char cmd[MAX_SIZE_CMD];
    char lang_code[AMZ_WW_MODEL_LENGTH];
};

enum
{
    WW_MODEL_SUCCESS       = 0,
    WW_MODEL_NULL          = 1,
    WW_MODEL_NOT_SUPPORTED = 2,
    WW_MODEL_FAILED        = 3,
};
typedef struct ww_model_cmd ww_model_cmd_t;
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Passes audio data to the wake word to be processed
 *
 * @param *pi16AudioBuff Audio sample to be fed to the wake word
 * @param *u16BufferSize The size of the sample of audio
 *
 * @returns Status of whether the model string is supported
 *
 */
uint32_t SLN_AMAZON_WAKE_ProcessWakeWord(int16_t *pi16AudioBuff, uint16_t u16BufferSize);

/*!
 * @brief Passes audio data to the self wake word to be processed
 *
 * @param *pi16AudioBuff Internal audio sample to be fed to the self wake word
 * @param *u16BufferSize The size of the sample of audio
 *
 * @returns Status of whether the model string is supported
 *
 */
uint32_t SLN_AMAZON_WAKE_ProcessWakeWordSelfWake(int16_t *pi16AudioBuff, uint16_t u16BufferSize);

/*!
 * @brief Initializes the Amazon Wake word
 *
 * @returns Status of whether the wake word was initialized
 *
 */
uint32_t SLN_AMAZON_WAKE_Initialize();

/*!
 * @brief Configures the pointers to be modified upon wake and length update
 *
 * @param *pu8Wake Pointer to the wake word trigger variable
 * @param *pu16WWLen Pointer to the wake word offset start
 *
 * @returns Status of whether the model string is supported
 *
 */
void SLN_AMAZON_WAKE_SetWakeupDetectedParams(uint8_t *pu8Wake, uint16_t *pu16WWLen);

/*!
 * @brief Checks to see if the string local is supported
 *
 * @param *lang_code Checks to see if the locale string is supported
 *
 * @returns Status of whether the model string is supported
 *
 */
uint32_t SLN_AMAZON_WAKE_IsWakeWordSupported(char *lang_code);

/*!
 * @brief Sets the Amazon Model Locale string to be used upon reset
 *
 * @param *modelLocale String of the wake word locale to be saved
 *
 * @returns Status of model locale string flash save
 *
 */
uint32_t SLN_AMAZON_WAKE_SetModelLocale(char *modelLocale);

/*!
 * @brief Gets the Amazon Model Locale string from Flash
 *
 * @param *modelLocale String of the wake word locale to be returned
 *
 */
void SLN_AMAZON_WAKE_GetModelLocale(uint8_t *modelLocal);
/*!
 * @brief Retrieves the Amazon Model Locale size
 *
 * @returns Size of the Amazon Wake Word Locale String
 *
 */
uint32_t SLN_AMAZON_WAKE_GetModelLocaleSize();

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _AMAZON_WAKE_WORD_H_ */

/*******************************************************************************
 * API
 ******************************************************************************/
