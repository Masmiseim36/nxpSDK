/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* FreeRTOS kernel includes. */
#include "board.h"
/* Freescale includes. */
#include "fsl_debug_console.h"
#include "pdm_pcm_definitions.h"
#include "pryon_lite.h"
#include "audio_samples.h"
#include "amazon_wake_word.h"
#include "FreeRTOSConfig.h"
#include "sln_flash_mgmt.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PRL_MODEL_ALIGN __attribute__((aligned(16)))
#define AMZ_WW_SELFWAKE_DELAY_CYCLES (AMZ_WW_SELFWAKE_DELAY_MS / 10)

/*******************************************************************************
 * Global Vars
 ******************************************************************************/

PryonLiteDecoderConfig config = PryonLiteDecoderConfig_Default;
PryonLiteModelAttributes modelAttributes;
static PryonLiteDecoderHandle sDecoder = NULL;

PryonLiteDecoderConfig configSelfWake = PryonLiteDecoderConfig_Default;
PryonLiteModelAttributes modelAttributesSelfWake;
static PryonLiteDecoderHandle sDecoderSelfWake = NULL;

uint8_t *pu8WakeupDetected = NULL;
uint16_t *pu8WakeupSize    = NULL;

uint8_t u8WakeupDetectedSelfWake = 0;
uint16_t u8WakeupSizeSelfWake    = 0;
uint8_t u8SelfWakeCount          = 0;

char *decoderBuffer         = NULL; // should be an array large enough to hold the largest decoder
char *decoderBufferSelfWake = NULL; // should be an array large enough to hold the largest decoder

// initialize decoder
PryonLiteSessionInfo sessionInfo;
PryonLiteSessionInfo sessionInfoSelfWake;

/* Structure containing Wake Word Information */
typedef struct
{
    uint32_t ww_model_len;
    uint8_t ww_model[AMZ_WW_MODEL_LENGTH];
} tsWakeWordAttributes;

typedef enum
{
    E_AMAZON_WAKE_WORD_MODEL_50KB,
    E_AMAZON_WAKE_WORD_MODEL_150KB,
    E_AMAZON_WAKE_WORD_MODEL_250KB,
} teAmazonWakeWordModelType;

/* Wake word model information */
tsWakeWordAttributes sWakeWordAttr = {0};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t SLN_AMAZON_WAKE_GetModelLocaleSize()
{
    return sWakeWordAttr.ww_model_len;
}

uint32_t SLN_AMAZON_WAKE_SetModelLocale(char *modelLocale)
{
    int32_t status      = WW_MODEL_SUCCESS;
    int32_t flashStatus = SLN_FLASH_MGMT_OK;

    sWakeWordAttr.ww_model_len = AMZ_WW_MODEL_LENGTH;
    strncpy((char *)sWakeWordAttr.ww_model, modelLocale, AMZ_WW_MODEL_LENGTH);

    flashStatus = SLN_FLASH_MGMT_Save(AMZ_WW_MODEL_FILE_NAME, sWakeWordAttr.ww_model, sWakeWordAttr.ww_model_len);

    if (flashStatus != SLN_FLASH_MGMT_OK)
    {
        if (flashStatus == SLN_FLASH_MGMT_EOVERFLOW || flashStatus == SLN_FLASH_MGMT_EOVERFLOW2)
        {
            flashStatus = SLN_FLASH_MGMT_Erase(AMZ_WW_MODEL_FILE_NAME);

            if (SLN_FLASH_MGMT_OK != flashStatus)
            {
                configPRINTF(("SLN_FLASH_MGMT_Erase AMZ_WW_MODEL_FILE_NAME Failed due to Overflow!\r\n"));
            }

            flashStatus =
                SLN_FLASH_MGMT_Save(AMZ_WW_MODEL_FILE_NAME, sWakeWordAttr.ww_model, sWakeWordAttr.ww_model_len);

            if (SLN_FLASH_MGMT_OK != flashStatus)
            {
                configPRINTF(("SLN_FLASH_MGMT_Save AMZ_WW_MODEL_FILE_NAME Failed!\r\n"));
            }
        }
        else
        {
            configPRINTF(("SLN_FLASH_MGMT_Save failed and returned: %X\r\n", status));
        }
    }

    if (flashStatus != SLN_FLASH_MGMT_OK)
    {
        status = WW_MODEL_FAILED;
    }

    return status;
}

void SLN_AMAZON_WAKE_GetModelLocale(uint8_t *modelLocal)
{
    uint32_t status = SLN_FLASH_MGMT_OK;

    if (sWakeWordAttr.ww_model_len == 0)
    {
        memset(&sWakeWordAttr, 0, sizeof(sWakeWordAttr));

        /* Check if the ww model is set */
        status = SLN_FLASH_MGMT_Read(AMZ_WW_MODEL_FILE_NAME, sWakeWordAttr.ww_model, &sWakeWordAttr.ww_model_len);

        /* If no model is set, write default value in flash */
        if (status == SLN_FLASH_MGMT_ENOENTRY || status == SLN_FLASH_MGMT_ENOENTRY2)
        {
            strcpy((char *)sWakeWordAttr.ww_model, "en-US");
            sWakeWordAttr.ww_model_len = AMZ_WW_MODEL_LENGTH;

            status = SLN_FLASH_MGMT_Save(AMZ_WW_MODEL_FILE_NAME, sWakeWordAttr.ww_model, sWakeWordAttr.ww_model_len);
            if (status != SLN_FLASH_MGMT_OK)
            {
                if (status == SLN_FLASH_MGMT_EOVERFLOW || status == SLN_FLASH_MGMT_EOVERFLOW2)
                {
                    status = SLN_FLASH_MGMT_Erase(AMZ_WW_MODEL_FILE_NAME);
                    if (SLN_FLASH_MGMT_OK != status)
                    {
                        configPRINTF(
                            ("SLN_FLASH_MGMT_Erase AMZ_WW_MODEL_FILE_NAME Failed and returned : %X!\r\n", status));
                    }
                    status =
                        SLN_FLASH_MGMT_Save(AMZ_WW_MODEL_FILE_NAME, sWakeWordAttr.ww_model, sWakeWordAttr.ww_model_len);
                    if (SLN_FLASH_MGMT_OK != status)
                    {
                        configPRINTF(
                            ("SLN_FLASH_MGMT_Save AMZ_WW_MODEL_FILE_NAME Failed and returned : %X!\r\n", status));
                    }
                }
                else
                {
                    configPRINTF(("SLN_FLASH_MGMT_Save failed and returned: %X\r\n", status));
                }
            }
        }
        else if (status != SLN_FLASH_MGMT_OK)
        {
            configPRINTF(("SLN_FLASH_MGMT_Read failed and returned: %X\r\n", status));
        }
    }

    if (modelLocal != NULL)
    {
        strcpy((char *)modelLocal, (char *)sWakeWordAttr.ww_model);
    }
}

void SLN_AMAZON_WAKE_SetWakeupDetectedParams(uint8_t *pu8Wake, uint16_t *pu16WWLen)
{
    pu8WakeupDetected = pu8Wake;
    pu8WakeupSize     = pu16WWLen;
}
uint32_t SLN_AMAZON_WAKE_IsWakeWordSupported(char *lang_code)
{
    if (lang_code == NULL)
    {
        return WW_MODEL_NULL;
    }
#ifndef LIGHTEN_MODEL_de_DE
    if (0 == strcmp(lang_code, "de-DE"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif
#ifndef LIGHTEN_MODEL_en_AU
    if (0 == strcmp(lang_code, "en-AU"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif
#ifndef LIGHTEN_MODEL_en_GB
    if (0 == strcmp(lang_code, "en-GB"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif
#ifndef LIGHTEN_MODEL_en_IN
    if (0 == strcmp(lang_code, "en-IN"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif
#ifndef LIGHTEN_MODEL_en_US
    if (0 == strcmp(lang_code, "en-US"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif
#ifndef LIGHTEN_MODEL_es_ES
    if (0 == strcmp(lang_code, "es-ES"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif
#ifndef LIGHTEN_MODEL_fr_CA
    if (0 == strcmp(lang_code, "fr-CA"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif
#ifndef LIGHTEN_MODEL_fr_FR
    if (0 == strcmp(lang_code, "fr-FR"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif
#ifndef LIGHTEN_MODEL_it_IT
    if (0 == strcmp(lang_code, "it-IT"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif
#ifndef LIGHTEN_MODEL_ja_JP
    if (0 == strcmp(lang_code, "ja-JP"))
    {
        return WW_MODEL_SUCCESS;
    }
#endif

    /* Reaching here means model us not supported */
    return WW_MODEL_NOT_SUPPORTED;
}
static void loadModel(PryonLiteDecoderConfig *config, teAmazonWakeWordModelType modelType)
{
    // In order to detect keywords, the decoder uses a model which defines the parameters,
    // neural network weights, classifiers, etc that are used at runtime to process the audio
    // and give detection results.

    // Each model is packaged in two formats:
    // 1. A .bin file that can be loaded from disk (via fopen, fread, etc)
    // 2. A .c file that can be hard-coded at compile time

    /* This is the first time so Get the current Locale */
    if (sWakeWordAttr.ww_model_len == 0)
    {
        SLN_AMAZON_WAKE_GetModelLocale(NULL);
    }
    if (0 == strcmp((char *)sWakeWordAttr.ww_model, "de-DE"))
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModeldeDE50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_deDE_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModeldeDELen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_deDE; // pointer to model in memory
        }
    }
    else if (0 == strcmp((char *)sWakeWordAttr.ww_model, "en-AU"))
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModelenAU50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enAU_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModelenAULen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enAU; // pointer to model in memory
        }
    }
    else if (0 == strcmp((char *)sWakeWordAttr.ww_model, "en-GB"))
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModelenGB50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enGB_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModelenGBLen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enGB; // pointer to model in memory
        }
    }
    else if (0 == strcmp((char *)sWakeWordAttr.ww_model, "en-IN"))
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModelenIN50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enIN_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModelenINLen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enIN; // pointer to model in memory
        }
    }
    else if (0 == strcmp((char *)sWakeWordAttr.ww_model, "en-US"))
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModelenUS50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enUS_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModelenUSLen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enUS; // pointer to model in memory
        }
    }
    else if (0 == strcmp((char *)sWakeWordAttr.ww_model, "es-ES"))
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModelesES50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_esES_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModelesESLen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_esES; // pointer to model in memory
        }
    }
    else if (0 == strcmp((char *)sWakeWordAttr.ww_model, "fr-FR"))
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModelfrFR50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_frFR_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModelfrFRLen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_frFR; // pointer to model in memory
        }
    }
    else if (0 == strcmp((char *)sWakeWordAttr.ww_model, "it-IT"))
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModelitIT50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_itIT_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModelitITLen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_itIT; // pointer to model in memory
        }
    }
    else if (0 == strcmp((char *)sWakeWordAttr.ww_model, "ja-JP"))
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModeljaJP50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_jaJP_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModeljaJPLen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_jaJP; // pointer to model in memory
        }
    }
    else
    {
        if (modelType == E_AMAZON_WAKE_WORD_MODEL_50KB)
        {
            config->sizeofModel =
                prlBinaryModelenUS50KBLen; // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enUS_50KB; // pointer to model in memory
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_150KB)
        {
            /* Not implemented yet */
        }
        else if (modelType == E_AMAZON_WAKE_WORD_MODEL_250KB)
        {
            config->sizeofModel =
                prlBinaryModelenUSLen;                // example value, will be the size of the binary model byte array
            config->model = &prlBinaryModelData_enUS; // pointer to model in memory
        }
        configPRINTF(("\r\n WW model not recognized, loading default one: en-US\r\n"));
    }
}

// keyword detection callback
static void detectionCallback(PryonLiteDecoderHandle handle, const PryonLiteResult *result)
{
    configPRINTF(("Woke Up\r\n"));
    if (!u8WakeupDetectedSelfWake)
    {
        *pu8WakeupDetected = (0 == strcmp(result->keyword, "ALEXA") ? 1 : 0);
        *pu8WakeupSize     = ((result->endSampleIndex - result->beginSampleIndex) / 160);
    }
}

// VAD event callback
static void vadCallback(PryonLiteDecoderHandle handle, const PryonLiteVadEvent *vadEvent)
{
    // printf("VAD state %d\n", (int) vadEvent->vadState);
}

// keyword detection callback
static void detectionSelfWakeCallback(PryonLiteDecoderHandle handle, const PryonLiteResult *result)
{
    configPRINTF(("Self Wake Woke Up\r\n"));
    u8WakeupDetectedSelfWake = (0 == strcmp(result->keyword, "ALEXA") ? 1 : 0);
}

uint32_t SLN_AMAZON_WAKE_SelfWakeInitialize()
{
    loadModel(&configSelfWake, E_AMAZON_WAKE_WORD_MODEL_50KB);

    // Query for the size of instance memory required by the decoder
    volatile PryonLiteError err =
        PryonLite_GetModelAttributes(configSelfWake.model, configSelfWake.sizeofModel, &modelAttributesSelfWake);

    configSelfWake.decoderMem = (char *)pvPortMalloc(modelAttributesSelfWake.requiredDecoderMem);

    if (configSelfWake.decoderMem == NULL)
    {
        while (1)
            ;
    }

    configSelfWake.sizeofDecoderMem = modelAttributesSelfWake.requiredDecoderMem;

    configSelfWake.detectThreshold = 100;                       // default threshold
    configSelfWake.resultCallback  = detectionSelfWakeCallback; // register detection handler
    // configSelfWake.vadCallback = vadSelfWakeCallback; // register VAD handler
    // configSelfWake.useVad = 1;  // enable voice activity detector
    configSelfWake.lowLatency = 1; // Make it quicker

    sessionInfoSelfWake.samplesPerFrame = 160; // enable voice activity detector

    err = PryonLiteDecoder_Initialize(&configSelfWake, &sessionInfoSelfWake, &sDecoderSelfWake);

    if (err != PRYON_LITE_ERROR_OK)
    {
        // Set detection threshold for all keywords (this function can be called any time after decoder initialization)
        int detectionThreshold = 100;
        err                    = PryonLiteDecoder_SetDetectionThreshold(sDecoderSelfWake, NULL, detectionThreshold);
    }

    return err;
}

uint32_t SLN_AMAZON_WAKE_Initialize()
{
    loadModel(&config, E_AMAZON_WAKE_WORD_MODEL_250KB);

    // Query for the size of instance memory required by the decoder
    PryonLiteError err = PryonLite_GetModelAttributes(config.model, config.sizeofModel, &modelAttributes);

    config.decoderMem = (char *)pvPortMalloc(modelAttributes.requiredDecoderMem);

    if (config.decoderMem == NULL)
    {
        while (1)
            ;
    }

    config.sizeofDecoderMem = modelAttributes.requiredDecoderMem;

    config.detectThreshold = 500;               // default threshold
    config.resultCallback  = detectionCallback; // register detection handler
    config.vadCallback     = vadCallback;       // register VAD handler
    config.useVad          = 1;                 // enable voice activity detector

    sessionInfo.samplesPerFrame = 160; // enable voice activity detector

    err = PryonLiteDecoder_Initialize(&config, &sessionInfo, &sDecoder);

    if (err != PRYON_LITE_ERROR_OK)
    {
        // Set detection threshold for all keywords (this function can be called any time after decoder initialization)
        int detectionThreshold = 500;
        err                    = PryonLiteDecoder_SetDetectionThreshold(sDecoder, NULL, detectionThreshold);
    }

    return err;
}

uint32_t SLN_AMAZON_WAKE_ProcessWakeWordSelfWake(int16_t *pi16AudioBuff, uint16_t u16BufferSize)
{
    PryonLiteError err;

    /* Push the audio data to detect if a self wake is happening */
    err = PryonLiteDecoder_PushAudioSamples(sDecoderSelfWake, pi16AudioBuff, sessionInfoSelfWake.samplesPerFrame);

    /* if the self wake has already been detected, then increment the delay */
    if (u8WakeupDetectedSelfWake)
    {
        u8SelfWakeCount++;
        if (u8SelfWakeCount > AMZ_WW_SELFWAKE_DELAY_CYCLES)
        {
            u8SelfWakeCount          = 0;
            u8WakeupDetectedSelfWake = 0;
        }
    }

    return err;
}

uint32_t SLN_AMAZON_WAKE_ProcessWakeWord(int16_t *pi16AudioBuff, uint16_t u16BufferSize)
{
    PryonLiteError err;

    err = PryonLiteDecoder_PushAudioSamples(sDecoder, pi16AudioBuff, sessionInfo.samplesPerFrame);

    return err;
}
