// ----------------------------------------------------------------------------
// Copyright 2016-2019 ARM Ltd.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------

// Note: this macro is needed on armcc to get the the PRI*32 macros
// from inttypes.h in a C++ code.
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#include "include/ServiceClient.h"
#include "include/CloudClientStorage.h"
#include "include/UpdateClientResources.h"
#include "factory_configurator_client.h"
#include "mbed-client/m2mconstants.h"
#include "mbed-trace/mbed_trace.h"
#include "pal.h"
#ifndef MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT
#include "CertificateEnrollmentClient.h"
#endif // MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT

#ifdef MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS
#include "DeviceInsightsClient.h"
#endif // MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS


#if MBED_CLOUD_CLIENT_STL_API
#include <string>
#endif

#include <assert.h>

#define TRACE_GROUP "mClt"

#define CONNECT 0
#define ERROR_UPDATE "Update has failed, check MbedCloudClient::Error"

/* lookup table for printing hexadecimal values */
const uint8_t ServiceClient::hex_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};

ServiceClient::ServiceClient(ServiceClientCallback& callback)
: _service_callback(callback),
  _service_uri(NULL),
  _stack(NULL),
  _client_objs(NULL),
  _current_state(State_Init),
  _event_generated(false),
  _state_engine_running(false),
#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
  _setup_update_client(false),
#endif
  _connector_client(this)
{
}

ServiceClient::~ServiceClient()
{
#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
    ARM_UC_HUB_Uninitialize();
#endif
#ifndef MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT
    CertificateEnrollmentClient::finalize();
#endif // MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT
#ifdef MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS
    DeviceInsightsClient::finalize();
#endif // MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS
}

void ServiceClient::initialize_and_register(M2MBaseList& reg_objs)
{
    tr_debug("ServiceClient::initialize_and_register");
    if(_current_state == State_Init ||
       _current_state == State_Unregister ||
       _current_state == State_Failure) {
        _client_objs = &reg_objs;

#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
        tr_debug("ServiceClient::initialize_and_register: update client supported");

        if(!_setup_update_client) {
            _setup_update_client = true;

#ifdef MBED_CLOUD_DEV_UPDATE_ID
            /* Overwrite values stored in KCM. This is for development only
               since these IDs should be provisioned in the factory.
            */
            tr_debug("ServiceClient::initialize_and_register: update IDs defined");

            /* Delete VendorId */
            ccs_delete_item("mbed.VendorId", CCS_CONFIG_ITEM);
            /* Store Vendor Id to mbed.VendorId. No conversion is performed. */
            set_device_resource_value(M2MDevice::Manufacturer,
                                      (const char*) arm_uc_vendor_id,
                                      arm_uc_vendor_id_size);

            /* Delete ClassId */
            ccs_delete_item("mbed.ClassId", CCS_CONFIG_ITEM);
            /* Store Class Id to mbed.ClassId. No conversion is performed. */
            set_device_resource_value(M2MDevice::ModelNumber,
                                      (const char*) arm_uc_class_id,
                                      arm_uc_class_id_size);
#endif /* MBED_CLOUD_DEV_UPDATE_ID */

#ifdef ARM_UPDATE_CLIENT_VERSION
            /* Inject Update Client version number if no other software
               version is present in the KCM.
            */
            tr_debug("ServiceClient::initialize_and_register: update version defined");

            const size_t buffer_size = 16;
            uint8_t buffer[buffer_size];
            size_t size = 0;

            /* check if software version is already set */
            ccs_status_e status = ccs_get_item(KEY_DEVICE_SOFTWAREVERSION,
                                               buffer, buffer_size, &size, CCS_CONFIG_ITEM);

            if (status == CCS_STATUS_KEY_DOESNT_EXIST) {
                tr_debug("ServiceClient::initialize_and_register: insert update version");

                /* insert value from Update Client Common */
                ccs_set_item(KEY_DEVICE_SOFTWAREVERSION,
                             (const uint8_t*) ARM_UPDATE_CLIENT_VERSION,
                             sizeof(ARM_UPDATE_CLIENT_VERSION),
                             CCS_CONFIG_ITEM);
            }
#endif /* ARM_UPDATE_CLIENT_VERSION */

            /* Update Client adds the OMA LWM2M Firmware Update object */
            UpdateClient::populate_object_list(*_client_objs);

            /* Initialize Update Client */
            FP1<void, int32_t> callback(this, &ServiceClient::update_error_callback);
            UpdateClient::UpdateClient(callback, _connector_client.m2m_interface(), this);
        }
        // else branch is required for re-initialization.
        else {
            finish_initialization();
        }
#else /* MBED_CLOUD_CLIENT_SUPPORT_UPDATE */
        finish_initialization();
#endif /* MBED_CLOUD_CLIENT_SUPPORT_UPDATE */
    } else if (_current_state == State_Success) {
        state_success();
    }
}

void ServiceClient::finish_initialization(void)
{
    /* Device Object is mandatory.
       Get instance and add it to object list
    */
    M2MDevice *device_object = device_object_from_storage();

#ifndef MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT
        // Initialize the certificate enrollment resources and module
        if (CertificateEnrollmentClient::init(*_client_objs, &_connector_client.est_client()) != CE_STATUS_SUCCESS) {
            _service_callback.error((int)CE_STATUS_INIT_FAILED, "Certificate Enrollment initialization failed");
        }
#endif /* !MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT */

#ifdef MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS
    // Initialize the device insights feature
    if (DeviceInsightsClient::init(*_client_objs) != DI_STATUS_SUCCESS) { 
         _service_callback.error((int)DI_STATUS_INIT_FAILED, "Device Insights initialization failed"); 
    }
#endif /* MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS */

    if (device_object) {
        /* Publish device object resource to mds */
        M2MResourceList list = device_object->object_instance()->resources();
        if(!list.empty()) {
            M2MResourceList::const_iterator it;
            it = list.begin();
            for ( ; it != list.end(); it++ ) {
                (*it)->set_register_uri(true);
            }
        }

        /* Add Device Object to object list. */
        _client_objs->push_back(device_object);
    }
#ifndef MBED_CONF_MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    internal_event(State_Bootstrap);
#else
    internal_event(State_Register);
#endif
}

ConnectorClient &ServiceClient::connector_client()
{
    return _connector_client;
}

const ConnectorClient &ServiceClient::connector_client() const
{
    return _connector_client;
}

// generates an internal event. called from within a state
// function to transition to a new state
void ServiceClient::internal_event(StartupMainState new_state)
{
    tr_debug("ServiceClient::internal_event: state: %d -> %d", _current_state, new_state);

    _event_generated = true;
    _current_state = new_state;

    if (!_state_engine_running) {
        state_engine();
    }
}

// the state engine executes the state machine states
void ServiceClient::state_engine(void)
{
    tr_debug("ServiceClient::state_engine");

    // this simple flagging gets rid of recursive calls to this method
    _state_engine_running = true;

    // while events are being generated keep executing states
    while (_event_generated) {
        _event_generated = false;  // event used up, reset flag

        state_function(_current_state);
    }

    _state_engine_running = false;
}

void ServiceClient::state_function(StartupMainState current_state)
{
    switch (current_state) {
        case State_Init:        // -> Goes to bootstrap state
#ifndef MBED_CONF_MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
        case State_Bootstrap:    // -> State_Register OR State_Failure
            state_bootstrap();
            break;
#endif
        case State_Register:     // -> State_Succes OR State_Failure
            state_register();
            break;
        case State_Success:      // return success to user
            state_success();
            break;
        case State_Failure:      // return error to user
            state_failure();
            break;
        case State_Unregister:   // return error to user
            state_unregister();
            break;
    }
}

#ifndef MBED_CONF_MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
void ServiceClient::state_bootstrap()
{
    tr_info("ServiceClient::state_bootstrap()");
    bool credentials_ready = _connector_client.connector_credentials_available();
    bool bootstrap = _connector_client.use_bootstrap();
    tr_info("ServiceClient::state_bootstrap() - lwm2m credentials available: %d", credentials_ready);
    tr_info("ServiceClient::state_bootstrap() - use bootstrap: %d", bootstrap);

    bool get_time = false;
#if defined (PAL_USE_SECURE_TIME) && (PAL_USE_SECURE_TIME == 1)
    // Strong time is mandatory in bootstrap mode
    get_time = pal_osGetTime() == 0 ? true : false;
#endif
    // Fallback to rebootstrap if time fetch fails in PAL_USE_SECURE_TIME case
    if (credentials_ready && bootstrap && get_time) {
        _connector_client.bootstrap_again();
    } else if (credentials_ready || !bootstrap) {
        internal_event(State_Register);
    } else {
        _connector_client.start_bootstrap();
    }
}
#endif //MBED_CONF_MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
void ServiceClient::state_register()
{
    tr_info("ServiceClient::state_register()");
    _connector_client.start_registration(_client_objs);
}

void ServiceClient::registration_process_result(ConnectorClient::StartupSubStateRegistration status)
{
    tr_debug("ServiceClient::registration_process_result(): status: %d", status);
    if (status == ConnectorClient::State_Registration_Success) {
        internal_event(State_Success);
    } else if(status == ConnectorClient::State_Registration_Failure ||
              status == ConnectorClient::State_Bootstrap_Failure){
        internal_event(State_Failure); // XXX: the status should be saved to eg. event object
    }
    if(status == ConnectorClient::State_Bootstrap_Success) {
        internal_event(State_Register);
    }
    if(status == ConnectorClient::State_Unregistered) {
        internal_event(State_Unregister);
    }
    if (status == ConnectorClient::State_Registration_Updated) {
        _service_callback.complete(ServiceClientCallback::Service_Client_Status_Register_Updated);
    }
}

void ServiceClient::connector_error(M2MInterface::Error error, const char *reason)
{
    tr_error("ServiceClient::connector_error() error %d", (int)error);
    if (_current_state == State_Register) {
        registration_process_result(ConnectorClient::State_Registration_Failure);
    }
#ifndef MBED_CONF_MBED_CLIENT_DISABLE_BOOTSTRAP_FEATURE
    else if (_current_state == State_Bootstrap) {
        registration_process_result(ConnectorClient::State_Bootstrap_Failure);
    }
#endif
    _service_callback.error(int(error),reason);
    internal_event(State_Failure);
}

void ServiceClient::value_updated(M2MBase *base, M2MBase::BaseType type)
{
    tr_debug("ServiceClient::value_updated()");
    _service_callback.value_updated(base, type);
}

void ServiceClient::state_success()
{
    tr_info("ServiceClient::state_success()");
    // this is verified already at client API level, but this might still catch some logic failures
    _service_callback.complete(ServiceClientCallback::Service_Client_Status_Registered);
}

void ServiceClient::state_failure()
{
    tr_error("ServiceClient::state_failure()");
    _service_callback.complete(ServiceClientCallback::Service_Client_Status_Failure);
}

void ServiceClient::state_unregister()
{
    tr_debug("ServiceClient::state_unregister()");
    _service_callback.complete(ServiceClientCallback::Service_Client_Status_Unregistered);
}

M2MDevice* ServiceClient::device_object_from_storage()
{
    M2MDevice *device_object = M2MInterfaceFactory::create_device();
    if (device_object == NULL) {
        return NULL;
    }
    M2MObjectInstance* instance = device_object->object_instance(0);
    if(instance == NULL) {
        return NULL;
    }

    const size_t buffer_size = 128;
    uint8_t buffer[buffer_size];
    size_t size = 0;
    M2MResource *res = NULL;

    // Read values to device object
    // create_resource() function returns NULL if resource already exists
    ccs_status_e status = ccs_get_item(g_fcc_manufacturer_parameter_name, buffer, buffer_size, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        const String data((char*)buffer, size);
        res = device_object->create_resource(M2MDevice::Manufacturer, data);
        if (res == NULL) {
            res = instance->resource(DEVICE_MANUFACTURER);
            device_object->set_resource_value(M2MDevice::Manufacturer, data);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
    }
    status = ccs_get_item(g_fcc_model_number_parameter_name, buffer, buffer_size, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        const String data((char*)buffer, size);
        res = device_object->create_resource(M2MDevice::ModelNumber, data);
        if (res == NULL) {
            res = instance->resource(DEVICE_MODEL_NUMBER);
            device_object->set_resource_value(M2MDevice::ModelNumber, data);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
    }
    status = ccs_get_item(g_fcc_device_serial_number_parameter_name, buffer, buffer_size, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        const String data((char*)buffer, size);
        res = device_object->create_resource(M2MDevice::SerialNumber, data);
        if (res == NULL) {
            res = instance->resource(DEVICE_SERIAL_NUMBER);
            device_object->set_resource_value(M2MDevice::SerialNumber, data);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
    }

    status = ccs_get_item(g_fcc_device_type_parameter_name, buffer, buffer_size, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        const String data((char*)buffer, size);
        res = device_object->create_resource(M2MDevice::DeviceType, data);
        if (res == NULL) {
            res = instance->resource(DEVICE_DEVICE_TYPE);
            device_object->set_resource_value(M2MDevice::DeviceType, data);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
    }

    status = ccs_get_item(g_fcc_hardware_version_parameter_name, buffer, buffer_size, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        const String data((char*)buffer, size);
        res = device_object->create_resource(M2MDevice::HardwareVersion, data);
        if (res == NULL) {
            res = instance->resource(DEVICE_HARDWARE_VERSION);
            device_object->set_resource_value(M2MDevice::HardwareVersion, data);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
    }

    status = ccs_get_item(KEY_DEVICE_SOFTWAREVERSION, buffer, buffer_size, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        const String data((char*)buffer, size);
        res = device_object->create_resource(M2MDevice::SoftwareVersion, data);
        if (res == NULL) {
            res = instance->resource(DEVICE_SOFTWARE_VERSION);
            device_object->set_resource_value(M2MDevice::SoftwareVersion, data);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
    }

    uint8_t data[4] = {0};
    uint32_t value;
    status = ccs_get_item(g_fcc_memory_size_parameter_name, data, 4, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        memcpy(&value, data, 4);
        res = device_object->create_resource(M2MDevice::MemoryTotal, value);
        if (res == NULL) {
            res = instance->resource(DEVICE_MEMORY_TOTAL);
            device_object->set_resource_value(M2MDevice::MemoryTotal, value);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
        tr_debug("ServiceClient::device_object_from_storage() - setting memory total value %" PRIu32 " (%s)", value, tr_array(data, 4));
    }

    status = ccs_get_item(g_fcc_current_time_parameter_name, data, 4, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        memcpy(&value, data, 4);
        res = device_object->create_resource(M2MDevice::CurrentTime, value);
        if (res == NULL) {
            res = instance->resource(DEVICE_CURRENT_TIME);
            device_object->set_resource_value(M2MDevice::CurrentTime, value);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
        tr_debug("ServiceClient::device_object_from_storage() - setting current time value %" PRIu32 " (%s)", value, tr_array(data, 4));
    }

    status = ccs_get_item(g_fcc_device_time_zone_parameter_name, buffer, buffer_size, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        const String data((char*)buffer, size);
        res = device_object->create_resource(M2MDevice::Timezone, data);
        if ( res == NULL) {
            res = instance->resource(DEVICE_TIMEZONE);
            device_object->set_resource_value(M2MDevice::Timezone, data);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
    }

    status = ccs_get_item(g_fcc_offset_from_utc_parameter_name, buffer, buffer_size, &size, CCS_CONFIG_ITEM);
    if (status == CCS_STATUS_SUCCESS) {
        const String data((char*)buffer, size);
        res = device_object->create_resource(M2MDevice::UTCOffset, data);
        if (res == NULL) {
            res = instance->resource(DEVICE_UTC_OFFSET);
            device_object->set_resource_value(M2MDevice::UTCOffset, data);
        }
        if(res) {
            res->publish_value_in_registration_msg(true);
            res->set_auto_observable(true);
        }
    }
    String binding_mode;
    M2MInterface::BindingMode mode = _connector_client.transport_mode();
    if (mode == M2MInterface::UDP || mode == M2MInterface::TCP) {
        binding_mode = (char*)BINDING_MODE_UDP;
    } else if (mode == M2MInterface::UDP_QUEUE || mode == M2MInterface::TCP_QUEUE) {
        binding_mode = (char*)BINDING_MODE_UDP_QUEUE;
    }
    device_object->set_resource_value(M2MDevice::SupportedBindingMode, binding_mode);

    // Add handler for reboot
    res = instance->resource(DEVICE_REBOOT);
    if (res) {
        res->set_execute_function(execute_callback(this, &ServiceClient::reboot_execute_handler));
        res->set_delayed_response(true);
        res->set_message_delivery_status_cb(post_response_status_handler, this);
    }

    return device_object;
}

/**
 * \brief Set resource value in the Device Object
 *
 * \param resource Device enum to have value set.
 * \param value String object.
 * \return True if successful, false otherwise.
 */
#if MBED_CLOUD_CLIENT_STL_API
bool ServiceClient::set_device_resource_value(M2MDevice::DeviceResource resource,
                                              const std::string& value)
{
    return set_device_resource_value(resource,
                                     value.c_str(),
                                     value.size());
}
#endif

/**
 * \brief Set resource value in the Device Object
 *
 * \param resource Device enum to have value set.
 * \param value Byte buffer.
 * \param length Buffer length.
 * \return True if successful, false otherwise.
 */
bool ServiceClient::set_device_resource_value(M2MDevice::DeviceResource resource,
                                              const char* value,
                                              uint32_t length)
{
    bool retval = false;

    /* sanity check */
    if (value && (length < 256) && (length > 0))
    {
#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
        /* Pass resource value to Update Client.
           Used for validating the manifest.
        */
        switch (resource) {
            case M2MDevice::Manufacturer:
                ARM_UC_SetVendorId((const uint8_t*) value, length);
                break;
            case M2MDevice::ModelNumber:
                ARM_UC_SetClassId((const uint8_t*) value, length);
                break;
            default:
                break;
        }
#endif
    }

    return retval;
}

void ServiceClient::post_response_status_handler(const M2MBase& base,
                                                 const M2MBase::MessageDeliveryStatus status,
                                                 const M2MBase::MessageType type,
                                                 void* me)
{
    switch(status) {
        case M2MBase::MESSAGE_STATUS_DELIVERED: // intentional fall-through
        case M2MBase::MESSAGE_STATUS_SEND_FAILED: {
            M2MDevice* dev = M2MInterfaceFactory::create_device();
            if (&base == dev->object_instance(0)->resource(DEVICE_REBOOT)) {
                ((ServiceClient*)me)->m2mdevice_reboot_execute();
            }
            break;
        }
        default:
            break;
    }
}

void ServiceClient::reboot_execute_handler(void*)
{
    // Don't perform reboot yet, as server will not get response. Instead, send response and wait
    // for acknowledgement before rebooting.
    M2MInterfaceFactory::create_device()->object_instance(0)->resource(DEVICE_REBOOT)->send_delayed_post_response();
}

void ServiceClient::m2mdevice_reboot_execute()
{
    pal_osReboot();
}

#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
void ServiceClient::set_update_authorize_handler(void (*handler)(int32_t request))
{
    UpdateClient::set_update_authorize_handler(handler);
}

void ServiceClient::set_update_authorize_priority_handler(void (*handler)(int32_t request, uint64_t priority))
{
    UpdateClient::set_update_authorize_priority_handler(handler);
}

void ServiceClient::update_authorize(int32_t request)
{
    UpdateClient::update_authorize(request);
}

void ServiceClient::update_reject(int32_t request, int32_t reason)
{
    UpdateClient::update_reject(request, reason);
}

void ServiceClient::set_update_progress_handler(void (*handler)(uint32_t progress, uint32_t total))
{
    UpdateClient::set_update_progress_handler(handler);
}

void ServiceClient::update_error_callback(int32_t error)
{
    _service_callback.error(error, ERROR_UPDATE);
}
#endif
