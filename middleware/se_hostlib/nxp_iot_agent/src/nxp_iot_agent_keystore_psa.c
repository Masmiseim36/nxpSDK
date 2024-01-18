/*
 * Copyright 2021-2023 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#include "nxp_iot_agent_keystore_psa.h"

#if SSS_HAVE_MBEDTLS_ALT_PSA

#include "nxp_iot_agent_session.h"
#include "nxp_iot_agent_macros.h"
#include "nxp_iot_agent_dispatcher.h"
#include "nxp_iot_agent_time.h"

#include "psa/crypto.h"
#include "psa_crypto_its.h"

#include "nxp_iot_agent_utils_protobuf.h"

#include "../protobuf/Dispatcher.pb.h"
#include "../protobuf/Agent.pb.h"
#include "../protobuf/PSA.pb.h"

#define IOT_AGENT_KEYSTORE_PSA_VERSION_MAJOR (1U)
#define IOT_AGENT_KEYSTORE_PSA_VERSION_MINOR (0U)
#define IOT_AGENT_KEYSTORE_PSA_VERSION_PATCH (0U)
#define IOT_AGENT_KEYSTORE_PSA_VERSION (((IOT_AGENT_KEYSTORE_PSA_VERSION_MAJOR * 256U) \
		+ IOT_AGENT_KEYSTORE_PSA_VERSION_MINOR) * 256U + IOT_AGENT_KEYSTORE_PSA_VERSION_PATCH)

#if defined(NXP_IOT_AGENT_ENABLE_LITE)
#include <fsl_romapi_iap.h>
#include <task.h>

#define SYSTEM_IS_XIP_FLEXSPI()                                                                               \
    ((((uint32_t)SystemCoreClockUpdate >= 0x08000000U) && ((uint32_t)SystemCoreClockUpdate < 0x10000000U)) || \
     (((uint32_t)SystemCoreClockUpdate >= 0x18000000U) && ((uint32_t)SystemCoreClockUpdate < 0x20000000U)))

#define FCB_ADDRESS             (0x08000400U)
#define FLEXSPI_INSTANCE        (0U)
#define FLASH_OPTION_QSPI_SDR   (0xc0000004U)

#define BLOB_BASE_ADDRESS (0x080A0000U)
#define BLOB_AREA_SIZE    (0x2000U)

struct BlobBuffer {
    uint8_t data[BLOB_AREA_SIZE];
    size_t offset;
};
#endif // NXP_IOT_AGENT_ENABLE_LITE

const iot_agent_keystore_interface_t iot_agent_keystore_psa_interface =
{
	&iot_agent_keystore_psa_destroy,
	&iot_agent_keystore_psa_open_session,
	&iot_agent_keystore_psa_close_session,
	{
		&iot_agent_keystore_psa_get_endpoint_info,
		&iot_agent_keystore_psa_handle_request,
	}
};

#if defined(NXP_IOT_AGENT_ENABLE_LITE)
static iot_agent_status_t write_to_flash(struct BlobBuffer *blob_buffer)
{
    if (0 == blob_buffer->offset)
    {
        // Skip writing to flash when there are no object changes
        return IOT_AGENT_SUCCESS;
    }

    api_core_context_t context  = {0};
    uint8_t iap_api_arena[4096] = {0};
    kp_api_init_param_t params  = {0};
    params.allocStart           = (uint32_t)&iap_api_arena;
    params.allocSize            = sizeof(iap_api_arena);

    status_t status = iap_api_init(&context, &params);
    if (kStatus_Success != status)
    {
        return IOT_AGENT_FAILURE;
    }

    flexspi_nor_config_t flashConfig = {0};
    if (!SYSTEM_IS_XIP_FLEXSPI())
    {
        serial_nor_config_option_t flashConfigOption = {.option0 = {.U = FLASH_OPTION_QSPI_SDR}};
        status = flexspi_nor_get_config(FLEXSPI_INSTANCE, &flashConfig, &flashConfigOption);
        if (kStatus_Success != status)
        {
            return IOT_AGENT_FAILURE;
        }
    }
    else
    {
        flashConfig = *((flexspi_nor_config_t *)FCB_ADDRESS);
    }

    status = iap_mem_config(&context, (uint32_t *)&flashConfig, kMemoryID_FlexspiNor);
    if (kStatus_Success != status)
    {
        return IOT_AGENT_FAILURE;
    }

    taskENTER_CRITICAL();
    status = iap_mem_erase(&context, BLOB_BASE_ADDRESS, BLOB_AREA_SIZE, kMemoryID_FlexspiNor);
    taskEXIT_CRITICAL();
    if (kStatus_Success != status)
    {
        return IOT_AGENT_FAILURE;
    }

    taskENTER_CRITICAL();
    status = iap_mem_write(&context, BLOB_BASE_ADDRESS, blob_buffer->offset, blob_buffer->data, kMemoryID_FlexspiNor);
    taskEXIT_CRITICAL();
    if (kStatus_Success != status)
    {
        return IOT_AGENT_FAILURE;
    }

    status = iap_mem_flush(&context);
    if (kStatus_Success != status)
    {
        return IOT_AGENT_FAILURE;
    }

    if(0 != memcmp((uint8_t *)BLOB_BASE_ADDRESS, blob_buffer->data, blob_buffer->offset))
    {
        return IOT_AGENT_FAILURE;
    }

    return IOT_AGENT_SUCCESS;
}
#endif // NXP_IOT_AGENT_ENABLE_LITE

iot_agent_status_t iot_agent_keystore_psa_init(iot_agent_keystore_t* keystore,
	uint32_t identifier)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	keystore->iface = iot_agent_keystore_psa_interface;
	keystore->type = IOT_AGENT_KS_PSA;
	keystore->identifier = identifier;
#if defined(NXP_IOT_AGENT_ENABLE_LITE)
    keystore->context = calloc(1, sizeof(struct BlobBuffer));
    if (keystore->context == NULL) {
        return IOT_AGENT_FAILURE;
    }
#endif // NXP_IOT_AGENT_ENABLE_LITE

    // We call psa_init here to make sure the psa stack is initialized. It may have been done by the
    // caller already, in which case this obsolete but harmless.
    psa_status_t psa_status = psa_crypto_init();
    PSA_SUCCESS_OR_EXIT_MSG("psa_crypto_init failed: 0x%08x", psa_status);
exit:
	return agent_status;
}


iot_agent_status_t iot_agent_keystore_psa_destroy(void *context)
{
#if defined(NXP_IOT_AGENT_ENABLE_LITE)
    free(context);
    context = NULL;
#else
    IOT_AGENT_UNUSED(context);
#endif // NXP_IOT_AGENT_ENABLE_LITE
    return IOT_AGENT_SUCCESS;
}


iot_agent_status_t iot_agent_keystore_psa_open_session(void *context)
{
#if defined(NXP_IOT_AGENT_ENABLE_LITE)
    return write_to_flash((struct BlobBuffer *)context);
#else
    IOT_AGENT_UNUSED(context);
    return IOT_AGENT_SUCCESS;
#endif // NXP_IOT_AGENT_ENABLE_LITE
}


void iot_agent_keystore_psa_close_session(void *context)
{
    IOT_AGENT_UNUSED(context);
}


bool iot_agent_keystore_psa_get_endpoint_info(
	void *context, void* endpoint_information)
{
    IOT_AGENT_UNUSED(context);
	nxp_iot_EndpointInformation* info = (nxp_iot_EndpointInformation*)endpoint_information;
	info->has_version = true;
	info->version = IOT_AGENT_KEYSTORE_PSA_VERSION;
	return true;
}

#define HAS_FIELD_OR_EXIT(MESSAGE_HAS_FIELD)                  \
    if (! MESSAGE_HAS_FIELD) {                                \
        IOT_AGENT_ERROR("missing field " #MESSAGE_HAS_FIELD); \
        result = false;                                       \
        goto exit;                                            \
    }


bool iot_agent_keystore_psa_handle_request(pb_istream_t *istream,
	pb_ostream_t *ostream, const pb_field_t* message_type, void *context)
{
    IOT_AGENT_UNUSED(context);
    bool result = IOT_AGENT_SUCCESS;
    psa_status_t psa_status = PSA_SUCCESS;
    nxp_iot_PsaRequest request = nxp_iot_PsaRequest_init_default;
    nxp_iot_ResponsePayload response = nxp_iot_ResponsePayload_init_default;
    PB_BYTES_ARRAY_T(1) empty_data = { 0 };

    result = message_type != NULL;
    RESULT_TRUE_OR_EXIT_MSG("message_type is NULL");

    result = message_type == nxp_iot_PsaRequest_fields;
    RESULT_TRUE_OR_EXIT_MSG("unsupported message type: tag: %u", message_type->tag);

    result = pb_decode_delimited(istream, nxp_iot_PsaRequest_fields, &request);
	RESULT_TRUE_OR_EXIT_MSG("decoding of server message failed: %s\n", PB_GET_ERROR(istream));

    switch(request.which_command) {

        case nxp_iot_PsaRequest_get_version_tag: 
            response.which_message = nxp_iot_ResponsePayload_psa_tag;
            response.message.psa.which_response = nxp_iot_PsaResponse_get_version_tag;
            nxp_iot_GetVersionResponse* gvr = &response.message.psa.response.get_version;
            gvr->has_crypto_api_version = true;
            gvr->crypto_api_version = (PSA_CRYPTO_API_VERSION_MAJOR << 16) | (PSA_CRYPTO_API_VERSION_MINOR << 8);
            gvr->supported_key_data_formats_count = 1;
            gvr->supported_key_data_formats[0] = nxp_iot_KeyDataFormat_S50;
            gvr->has_max_raw_key_size = true;
            gvr->max_raw_key_size = NXP_IOT_AGENT_MAX_KEY_DATA_SIZE;
            break;

        case nxp_iot_PsaRequest_destroy_key_tag:
            HAS_FIELD_OR_EXIT(request.command.destroy_key.has_identifier);
            psa_key_id_t key_id = request.command.destroy_key.identifier;
            psa_status = psa_destroy_key(key_id);
        if (psa_status == PSA_ERROR_INVALID_HANDLE) // TODO investigate why psa in psa_crypto_slot_management.c line 372 sets PSA_ERROR_INVALID_HANDLE and how to properly handle this behavior
            psa_status = PSA_ERROR_DOES_NOT_EXIST;
            response.which_message = nxp_iot_ResponsePayload_psa_tag;
            response.message.psa.which_response = nxp_iot_PsaResponse_destroy_key_tag;
            response.message.psa.response.destroy_key.has_status = true;
            response.message.psa.response.destroy_key.status = (nxp_iot_StatusCode)psa_status;
            break;

        case nxp_iot_PsaRequest_import_key_tag:
            HAS_FIELD_OR_EXIT(request.command.import_key.has_identifier);
            HAS_FIELD_OR_EXIT(request.command.import_key.has_type);
            HAS_FIELD_OR_EXIT(request.command.import_key.has_usage);
            HAS_FIELD_OR_EXIT(request.command.import_key.has_algorithm);
            HAS_FIELD_OR_EXIT(request.command.import_key.has_key_data_format);
            HAS_FIELD_OR_EXIT(request.command.import_key.has_lifetime);
            result = request.command.import_key.data->size > 0;
            RESULT_TRUE_OR_EXIT_MSG("key data size is <= 0");
            result = request.command.import_key.key_data_format == nxp_iot_KeyDataFormat_S50;
            RESULT_TRUE_OR_EXIT_MSG("unsupported key data format: 0x%08x", request.command.import_key.key_data_format);

#if defined(NXP_IOT_AGENT_ENABLE_LITE)
            struct BlobBuffer *blob_buffer = (struct BlobBuffer *)context;
            memcpy(blob_buffer->data + blob_buffer->offset,
                   request.command.import_key.data->bytes, request.command.import_key.data->size);
            blob_buffer->offset += request.command.import_key.data->size;
#else
            psa_key_attributes_t attributes = PSA_KEY_ATTRIBUTES_INIT;

            /* Set key attributes */
            psa_set_key_usage_flags(&attributes, request.command.import_key.usage);
            psa_set_key_algorithm(&attributes, request.command.import_key.algorithm);
            psa_set_key_type(&attributes, request.command.import_key.type);
            psa_set_key_id(&attributes, request.command.import_key.identifier);
            psa_set_key_lifetime(&attributes, request.command.import_key.lifetime);
            psa_set_key_bits(&attributes, request.command.import_key.key_bits);

            /* Import the key */
            psa_key_id_t imported_id;
            psa_status = psa_import_key(&attributes, 
                    request.command.import_key.data->bytes, 
                    request.command.import_key.data->size, &imported_id);
#endif // NXP_IOT_AGENT_ENABLE_LITE

            response.which_message = nxp_iot_ResponsePayload_psa_tag;
            response.message.psa.which_response = nxp_iot_PsaResponse_import_key_tag;
            response.message.psa.response.import_key.has_status = true;
            response.message.psa.response.import_key.status = (nxp_iot_StatusCode)psa_status;
            break;

        case nxp_iot_PsaRequest_read_object_tag:
            response.which_message = nxp_iot_ResponsePayload_psa_tag;
            response.message.psa.which_response = nxp_iot_PsaResponse_read_object_tag;

            HAS_FIELD_OR_EXIT(request.command.read_object.has_identifier);
#if defined(MBEDTLS_PSA_CRYPTO_STORAGE_C)
            psa_storage_uid_t uid = request.command.read_object.identifier;
            struct psa_storage_info_t storage_info = { 0 };
            psa_status = psa_its_get_info(uid, &storage_info);
            if (psa_status != PSA_SUCCESS) {
                response.message.psa.response.read_object.has_status = true;
                response.message.psa.response.read_object.status = (nxp_iot_StatusCode) psa_status;
                response.message.psa.response.read_object.data = (pb_bytes_array_t*) & empty_data;
                break;
            }

            size_t chunk_size = storage_info.size; 
            if (request.command.read_object.has_length) {
                uint32_t remaining = storage_info.size - request.command.read_object.offset;
                chunk_size = remaining < request.command.read_object.length 
                    ? remaining 
                    : request.command.read_object.length;
            }

            response.message.psa.response.read_object.data = malloc(PB_BYTES_ARRAY_T_ALLOCSIZE(chunk_size));
            response.message.psa.response.read_object.data->size = chunk_size;

            size_t read_length = 0;
            psa_status = psa_its_get(uid, request.command.read_object.offset, 
                chunk_size,
                response.message.psa.response.read_object.data->bytes, 
                &read_length);
            response.message.psa.response.read_object.data->size = read_length;

            response.message.psa.response.read_object.has_status = true;
            response.message.psa.response.read_object.status = (nxp_iot_StatusCode) psa_status;
#else
            response.message.psa.response.read_object.has_status = true;
            response.message.psa.response.read_object.status = (nxp_iot_StatusCode) PSA_ERROR_DOES_NOT_EXIST;
            response.message.psa.response.read_object.data = (pb_bytes_array_t*)& empty_data;
#endif
            break;

        case nxp_iot_PsaRequest_remove_object_tag:
            HAS_FIELD_OR_EXIT(request.command.read_object.has_identifier);
#if defined(MBEDTLS_PSA_CRYPTO_STORAGE_C)
            psa_status = psa_its_remove(request.command.read_object.identifier); // will be enabled later point in time
            psa_status = PSA_ERROR_NOT_SUPPORTED;
#endif
            response.which_message = nxp_iot_ResponsePayload_psa_tag;
            response.message.psa.which_response = nxp_iot_PsaResponse_remove_object_tag;
            response.message.psa.response.remove_object.has_status = true;
            response.message.psa.response.remove_object.status = (nxp_iot_StatusCode)psa_status;
            break;

        default:
            IOT_AGENT_ERROR("unsupported command: 0x%08x", request.which_command);
            break;
    }


    // And encode the actual payload including a responses tag and a length in front.
    result = encode_responses_from_payload(ostream, &response);
    RESULT_TRUE_OR_EXIT_MSG("encode_responses_from_payload failed");

exit:

    pb_release(nxp_iot_PsaRequest_fields, &request);
    pb_release(nxp_iot_PsaResponse_fields, &response);
	return result;
}

#endif // SSS_HAVE_MBEDTLS_ALT_PSA
