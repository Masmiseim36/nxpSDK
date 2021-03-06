/*
 * Generated by erpcgen 1.4.0 on Fri Jan 20 08:45:14 2017.
 *
 * AUTOGENERATED - DO NOT EDIT
 */

#include "erpc_remote_control_app_1.h"
#include "client_manager.h"
#include "erpc_port.h"

using namespace erpc;
#if !(__embedded_cplusplus)
using namespace std;
#endif

extern ClientManager *g_client;

// remote_control_app_1 interface button_pressed function client shim.
void button_pressed(uint32_t which)
{
    erpc_status_t err = kErpcStatus_Success;

    // Get a new request.
    RequestContext request = g_client->createRequest(true);

    // Encode the request.
    Codec * codec = request.getCodec();
    if (codec == NULL)
    {
        err = kErpcStatus_MemoryError;
    }

    if (!err)
    {
        err = codec->startWriteMessage(kOnewayMessage, kremote_control_app_1_service_id, kremote_control_app_1_button_pressed_id, request.getSequence());
    }
    if (!err)
    {
        err = codec->write(which);
    }

    if (!err)
    {
        err = codec->endWriteMessage();
    }

    // Send message to server
    if (!err)
    {
        err = g_client->performRequest(request);
    }

    // Dispose of the request.
    g_client->releaseRequest(request);

    if (err)
    {
        client_error_handler_t errorHandler = g_client->getErrorHandler();
        if (errorHandler != NULL)
        {
            (*errorHandler)(err);
        }
        
    }

    return;
}
