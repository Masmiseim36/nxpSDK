/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Generated by erpcgen 1.7.4 on Thu Apr 16 10:59:25 2020.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#include "erpc_remote_control_app_0_server.h"
#include <new>
#include "erpc_port.h"
#include "erpc_manually_constructed.h"

#if 10704 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif

using namespace erpc;
using namespace std;

#if ERPC_NESTED_CALLS_DETECTION
extern bool nestingDetection;
#endif

static ManuallyConstructed<remote_control_app_0_service> s_remote_control_app_0_service;


//! @brief Function to write struct BoardConfig
static void write_BoardConfig_struct(erpc::Codec * codec, const BoardConfig * data);

//! @brief Function to write struct AdcConfig
static void write_AdcConfig_struct(erpc::Codec * codec, const AdcConfig * data);

//! @brief Function to write struct Vector
static void write_Vector_struct(erpc::Codec * codec, const Vector * data);


// Write struct BoardConfig function implementation
static void write_BoardConfig_struct(erpc::Codec * codec, const BoardConfig * data)
{
    codec->write(data->DAC);

    codec->write(data->MAG);

    for (uint32_t arrayCount0 = 0; arrayCount0 < 3; ++arrayCount0)
    {
        codec->write(data->RGB[arrayCount0]);
    }
}

// Write struct AdcConfig function implementation
static void write_AdcConfig_struct(erpc::Codec * codec, const AdcConfig * data)
{
    codec->write(data->vref);

    codec->write(data->atomicSteps);
}

// Write struct Vector function implementation
static void write_Vector_struct(erpc::Codec * codec, const Vector * data)
{
    codec->write(data->A_x);

    codec->write(data->A_y);

    codec->write(data->A_z);

    codec->write(data->M_x);

    codec->write(data->M_y);

    codec->write(data->M_z);
}



// Call the correct server shim based on method unique ID.
erpc_status_t remote_control_app_0_service::handleInvocation(uint32_t methodId, uint32_t sequence, Codec * codec, MessageBufferFactory *messageFactory)
{
    switch (methodId)
    {
        case kremote_control_app_0_get_board_config_id:
            return get_board_config_shim(codec, messageFactory, sequence);

        case kremote_control_app_0_get_adc_config_id:
            return get_adc_config_shim(codec, messageFactory, sequence);

        case kremote_control_app_0_convert_dac_adc_id:
            return convert_dac_adc_shim(codec, messageFactory, sequence);

        case kremote_control_app_0_read_mag_accel_id:
            return read_mag_accel_shim(codec, messageFactory, sequence);

        case kremote_control_app_0_set_led_id:
            return set_led_shim(codec, messageFactory, sequence);

        default:
            return kErpcStatus_InvalidArgument;
    }
}

// Server shim for get_board_config of remote_control_app_0 interface.
erpc_status_t remote_control_app_0_service::get_board_config_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    BoardConfig *config = NULL;

    // startReadMessage() was already called before this shim was invoked.

    config = (BoardConfig *) erpc_malloc(sizeof(BoardConfig));
    if (config == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        get_board_config(config);
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = false;
#endif

        // preparing MessageBuffer for serializing data
        err = messageFactory->prepareServerBufferForSend(codec->getBuffer());
    }

    if (!err)
    {
        // preparing codec for serializing data
        codec->reset();

        // Build response message.
        codec->startWriteMessage(kReplyMessage, kremote_control_app_0_service_id, kremote_control_app_0_get_board_config_id, sequence);

        write_BoardConfig_struct(codec, config);

        err = codec->getStatus();
    }

    if (config)
    {
        erpc_free(config);
    }

    return err;
}

// Server shim for get_adc_config of remote_control_app_0 interface.
erpc_status_t remote_control_app_0_service::get_adc_config_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    AdcConfig *config = NULL;

    // startReadMessage() was already called before this shim was invoked.

    config = (AdcConfig *) erpc_malloc(sizeof(AdcConfig));
    if (config == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        get_adc_config(config);
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = false;
#endif

        // preparing MessageBuffer for serializing data
        err = messageFactory->prepareServerBufferForSend(codec->getBuffer());
    }

    if (!err)
    {
        // preparing codec for serializing data
        codec->reset();

        // Build response message.
        codec->startWriteMessage(kReplyMessage, kremote_control_app_0_service_id, kremote_control_app_0_get_adc_config_id, sequence);

        write_AdcConfig_struct(codec, config);

        err = codec->getStatus();
    }

    if (config)
    {
        erpc_free(config);
    }

    return err;
}

// Server shim for convert_dac_adc of remote_control_app_0 interface.
erpc_status_t remote_control_app_0_service::convert_dac_adc_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    uint32_t numberToConvert;
    uint32_t result;

    // startReadMessage() was already called before this shim was invoked.

    codec->read(&numberToConvert);

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        convert_dac_adc(numberToConvert, &result);
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = false;
#endif

        // preparing MessageBuffer for serializing data
        err = messageFactory->prepareServerBufferForSend(codec->getBuffer());
    }

    if (!err)
    {
        // preparing codec for serializing data
        codec->reset();

        // Build response message.
        codec->startWriteMessage(kReplyMessage, kremote_control_app_0_service_id, kremote_control_app_0_convert_dac_adc_id, sequence);

        codec->write(result);

        err = codec->getStatus();
    }

    return err;
}

// Server shim for read_mag_accel of remote_control_app_0 interface.
erpc_status_t remote_control_app_0_service::read_mag_accel_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    Vector *results = NULL;
    bool status;

    // startReadMessage() was already called before this shim was invoked.

    results = (Vector *) erpc_malloc(sizeof(Vector));
    if (results == NULL)
    {
        codec->updateStatus(kErpcStatus_MemoryError);
    }

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        read_mag_accel(results, &status);
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = false;
#endif

        // preparing MessageBuffer for serializing data
        err = messageFactory->prepareServerBufferForSend(codec->getBuffer());
    }

    if (!err)
    {
        // preparing codec for serializing data
        codec->reset();

        // Build response message.
        codec->startWriteMessage(kReplyMessage, kremote_control_app_0_service_id, kremote_control_app_0_read_mag_accel_id, sequence);

        write_Vector_struct(codec, results);

        codec->write(status);

        err = codec->getStatus();
    }

    if (results)
    {
        erpc_free(results);
    }

    return err;
}

// Server shim for set_led of remote_control_app_0 interface.
erpc_status_t remote_control_app_0_service::set_led_shim(Codec * codec, MessageBufferFactory *messageFactory, uint32_t sequence)
{
    erpc_status_t err = kErpcStatus_Success;

    uint8_t whichLed;

    // startReadMessage() was already called before this shim was invoked.

    codec->read(&whichLed);

    err = codec->getStatus();
    if (!err)
    {
        // Invoke the actual served function.
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = true;
#endif
        set_led(whichLed);
#if ERPC_NESTED_CALLS_DETECTION
        nestingDetection = false;
#endif
    }

    return err;
}

erpc_service_t create_remote_control_app_0_service()
{
    s_remote_control_app_0_service.construct();
    return s_remote_control_app_0_service.get();
}

void destroy_remote_control_app_0_service()
{
    s_remote_control_app_0_service.destroy();
}
