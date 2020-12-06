/*
 * Copyright 2019-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* Standard library includes. */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* Board includes */
#include "board.h"
#include "pin_mux.h"

/* Driver includes */
#include "fsl_iomuxc.h"

#include "app_aws_shadow.h"
#include "cJSON.h"
#include "aws_shadow.h"
#include "ux_attention_system.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define shadowDemoTIMEOUT         pdMS_TO_TICKS(30000UL)
#define SHADOW_UPDATE_INTERVAL_MS (3000)
#define SHADOW_DEMO_LEDS          (1)

/*******************************************************************************
 * Declarations
 ******************************************************************************/

ShadowOperationParams_t xGetParams;
ShadowClientHandle_t xClientHandle;
char *shadowThingName = NULL;

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

BaseType_t APP_AWS_ShadowUpdate(cJSON *payload)
{
    cJSON *json = NULL, *state = NULL;
    char *jsonString = NULL;
    ShadowOperationParams_t xShadowUpdateParams = {0};
    BaseType_t status = eShadowSuccess;

    json  = cJSON_CreateObject();
    state = cJSON_CreateObject();

    if ((json == NULL) || (state == NULL))
    {
        status = eShadowFailure;
    }

    if (eShadowSuccess == status)
    {
        cJSON_AddItemToObject(state, "reported", payload);
        cJSON_AddItemToObject(json, "state", state);
    }

    jsonString = cJSON_PrintUnformatted(json);

    /* Shadow Update parameters */
    xShadowUpdateParams.pcThingName         = shadowThingName;
    xShadowUpdateParams.xQoS                = eMQTTQoS0;
    xShadowUpdateParams.pcData              = jsonString;
    xShadowUpdateParams.ucKeepSubscriptions = pdTRUE;

    if (eShadowSuccess == status)
    {
        status = SHADOW_Update(xClientHandle, &xShadowUpdateParams, pdMS_TO_TICKS(SHADOW_UPDATE_INTERVAL_MS));
    }

    if (status == eShadowSuccess)
    {
        configPRINTF(("Shadow Update Succeeded\r\n"));
    }
    else
    {
        configPRINTF(("Shadow Update Failed: %d.\r\n", status));
    }

    vPortFree(jsonString);
    cJSON_Delete(json);

    return status;
}

BaseType_t ProcessUpdatedShadowDocument(const char *const pcThingName,
                                        const char *const pcUpdateDocument,
                                        uint32_t ulDocumentLength)
{
    cJSON *monitor_json = NULL, *brightness = NULL, *desired = NULL, *current = NULL, *state = NULL;
    int32_t success = pdTRUE;

    configPRINTF(("%s", pcUpdateDocument));
    configPRINTF(("Update as been made on SHADOW doc!\r\n"));
    configPRINTF(("Add Shadow Update Code Logic Here\r\n"));

#ifdef SHADOW_DEMO_LEDS
    /* The following section is an example of how a shadow update could be received */
    /* Parse the JSON string received */
    monitor_json = cJSON_Parse(pcUpdateDocument);

    if (NULL == monitor_json)
    {
        /* The JSON string is not valid, ensure parsing doesn't continue */
        configPRINTF(("Invalid JSON Message\r\n"));
        success = pdFAIL;
    }

    if (pdTRUE == success)
    {
        /* If the JSON string is valid, get the JSON sub objects */

        current    = cJSON_GetObjectItem(monitor_json, "current");
        state      = cJSON_GetObjectItem(current, "state");
        desired    = cJSON_GetObjectItem(state, "desired");
        brightness = cJSON_GetObjectItemCaseSensitive(desired, "brightness");

        /* Ensure all objects are parsed correctly */
        if ((NULL == state) || (NULL == desired) || (NULL == brightness))
        {
            configPRINTF(
                ("Missing JSON parameter, state %x, desired %x, brightness %x\r\n", state, desired, brightness));
            success = pdFAIL;
        }
    }

    if (pdTRUE == success)
    {
        /* If the JSON string is valid and the objects have been parsed, continue to execute action */
        configPRINTF(("LED brightness for %s: \"%s\" \r\n", pcThingName, brightness->valuestring));

        // Set LEDs on GPIO
        if (!strcmp(brightness->valuestring, "HIGH"))
        {
            ux_attention_set_state(uxHighBrightness);
        }
        else if (!strcmp(brightness->valuestring, "MEDIUM"))
        {
            ux_attention_set_state(uxMediumBrightness);
        }
        else if (!strcmp(brightness->valuestring, "LOW"))
        {
            ux_attention_set_state(uxLowBrightness);
        }
    }
#endif

    cJSON_Delete(monitor_json);

    /* Return 0 to ensure the memory is freed by calling function */
    return 0;
}
