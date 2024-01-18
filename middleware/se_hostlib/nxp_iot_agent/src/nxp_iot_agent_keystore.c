/* 
 * Copyright 2018-2021 NXP
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 */

#include <nxp_iot_agent_keystore.h>

iot_agent_status_t iot_agent_keystore_free(iot_agent_keystore_t* keystore)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	if (keystore != NULL)
	{
        if (keystore->iface.destroy != NULL)
        {
            agent_status = keystore->iface.destroy(keystore->context);
            if (agent_status != IOT_AGENT_SUCCESS) {
#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
            	IOT_AGENT_ERROR("Failed to destroy keystore [type: %lu, id: 0x%08lx]: 0x%08x",
                    keystore->type, keystore->identifier, agent_status);
#else
    			IOT_AGENT_ERROR("Failed to destroy keystore [type: %d, id: 0x%08x]: 0x%08x",
                        keystore->type, keystore->identifier, agent_status);
#endif
            }
        }
        free(keystore->context);
		keystore->context = NULL;
	}
	return agent_status;
}


iot_agent_status_t iot_agent_keystore_open_session(iot_agent_keystore_t* keystore)
{
	return keystore->iface.open_session(keystore->context);
}


void iot_agent_keystore_close_session(iot_agent_keystore_t* keystore)
{
    if (keystore != NULL && keystore->iface.close_session != NULL)
    {
        keystore->iface.close_session(keystore->context);
    }
}

