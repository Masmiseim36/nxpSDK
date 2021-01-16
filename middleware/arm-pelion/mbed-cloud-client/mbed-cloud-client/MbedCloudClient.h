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


#ifndef __MBED_CLOUD_CLIENT_H__
#define __MBED_CLOUD_CLIENT_H__

#include "include/ServiceClient.h"
#ifndef MBED_CLIENT_DISABLE_EST_FEATURE
#include "est_defs.h"
#endif // !MBED_CLIENT_DISABLE_EST_FEATURE
#include "mbed-cloud-client/MbedCloudClientConfig.h"

#ifndef MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT
// This overly long path is needed to have build compatibility with previous
// version. A #include can't just point to a file which is not in application's
// include path and we should not force application to add every internal directory
// of MCC to their paths.
// On next phase, the cmake is used to publish the API paths via
// target_include_directories(), but that requires a bit more cleanups.
#include "certificate-enrollment-client/certificate-enrollment-client/ce_defs.h"
#endif // MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT

#ifdef MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS
#include "di_status.h"
#endif // MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS

#if MBED_CLOUD_CLIENT_STL_API
#include <map>
#include <string>
#include <vector>
#endif

#if MBED_CLOUD_CLIENT_STD_NAMESPACE_POLLUTION
// We should not really pollute application's namespace with std:: by having this in
// a public header file.
// But as as removal of the next line may break existing applications, which build due to this
// leakage, we need to maintain the old behavior for a while and just allow one to remove it.
using namespace std;
#endif

class SimpleM2MResourceBase;

/**
 * \brief MbedCloudClientCallback
 * A callback class for informing updated Object and Resource value from the
 * LwM2M server to the user of the `MbedCloudClient` class. The user must instantiate the
 * class derived out of this and pass the Object to `MbedCloudClient::set_update_callback()`.
 */
class MbedCloudClientCallback {

public:

    /**
    * \brief A callback indicating that the value of the Resource Object is updated
    *  by the LwM2M Device Management server.
    * \param base The Object whose value is updated.
    * \param type The type of the Object.
    */
    virtual void value_updated(M2MBase *base, M2MBase::BaseType type) = 0;
};



/*! \file MbedCloudClient.h
 *  \brief MbedCloudClient.
 *  This class provides an interface for handling all the client interface operations
 *  including device provisioning, identity setup, device resource management defined in the OMA
 *  LwM2M specifications, and update firmware.
 *  Device Resource management includes bootstrapping, client registration, device management &
 *  service enablement and information reporting.
 */

class MbedCloudClient : public ServiceClientCallback {

public:

    /**
     * \brief An enum defining different kinds of errors
     * that can occur during various client operations.
     */
    typedef enum {
        // Range 0x30 - 0x3FF reserved for Connector error.
        ConnectErrorNone                        = M2MInterface::ErrorNone,

        // Not used.
        ConnectAlreadyExists                    = M2MInterface::AlreadyExists,

        // Bootstrap failed.
        // Client recovers automatically.
        ConnectBootstrapFailed                  = M2MInterface::BootstrapFailed,

        // Security object is not valid or server rejects the registration.
        // No internal recovery mechanism. Actions needed on the application side.
        ConnectInvalidParameters                = M2MInterface::InvalidParameters,

        // Cannot unregister as client is not registered.
        // No internal recovery mechanism. Actions needed on the application side.
        ConnectNotRegistered                    = M2MInterface::NotRegistered,

        // Registration has timed out.
        // Client recovers automatically.
        ConnectTimeout                          = M2MInterface::Timeout,

        // Socket level operation error.
        // Client recovers automatically.
        ConnectNetworkError                     = M2MInterface::NetworkError,

        // Failed to parse an incoming CoAP message.
        // Client will continue working, no actions needed.
        ConnectResponseParseFailed              = M2MInterface::ResponseParseFailed,

        // Unknown CoAP level error.
        // Client recovers automatically.
        ConnectUnknownError                     = M2MInterface::UnknownError,

        // Memory allocation has failed.
        // No internal recovery mechanism. Actions needed on the application side.
        ConnectMemoryConnectFail                = M2MInterface::MemoryFail,

        // API call is not allowed for now.
        // Application should try again later.
        ConnectNotAllowed                       = M2MInterface::NotAllowed,

        // Failed to initialize secure connection or DTLS/TLS handshake failed.
        // Client recovers automatically.
        ConnectSecureConnectionFailed           = M2MInterface::SecureConnectionFailed,

        // DNS resolving has failed.
        // Client recovers automatically.
        ConnectDnsResolvingFailed               = M2MInterface::DnsResolvingFailed,

        // Not used.
        ConnectorFailedToStoreCredentials       = M2MInterface::FailedToStoreCredentials,

        // Failed to read credentials from storage.
        // No internal recovery mechanism. Actions needed on the application side.
        ConnectorFailedToReadCredentials        = M2MInterface::FailedToReadCredentials,

        // Not used.
        ConnectorInvalidCredentials,
#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
        UpdateWarningNoActionRequired           = UpdateClient::WarningBase, // Range reserved for Update Error from 0x0400 - 0x04FF
        UpdateWarningCertificateNotFound        = UpdateClient::WarningCertificateNotFound,
        UpdateWarningIdentityNotFound           = UpdateClient::WarningIdentityNotFound,
        UpdateWarningVendorMismatch             = UpdateClient::WarningVendorMismatch,
        UpdateWarningClassMismatch              = UpdateClient::WarningClassMismatch,
        UpdateWarningDeviceMismatch             = UpdateClient::WarningDeviceMismatch,
        UpdateWarningCertificateInvalid         = UpdateClient::WarningCertificateInvalid,
        UpdateWarningSignatureInvalid           = UpdateClient::WarningSignatureInvalid,
        UpdateWarningBadKeytable                = UpdateClient::WarningBadKeytable,
        UpdateWarningURINotFound                = UpdateClient::WarningURINotFound,
        UpdateWarningRollbackProtection         = UpdateClient::WarningRollbackProtection,
        UpdateWarningAuthorizationRejected      = UpdateClient::WarningAuthorizationRejected,
        UpdateWarningAuthorizationUnavailable   = UpdateClient::WarningAuthorizationUnavailable,
        UpdateWarningUnknown                    = UpdateClient::WarningUnknown,
        UpdateCertificateInsertion              = UpdateClient::WarningCertificateInsertion,
        UpdateErrorUserActionRequired           = UpdateClient::ErrorBase,
        UpdateErrorWriteToStorage               = UpdateClient::ErrorWriteToStorage,
        UpdateErrorInvalidHash                  = UpdateClient::ErrorInvalidHash,
        UpdateErrorConnection                   = UpdateClient::ErrorConnection,
        UpdateFatalRebootRequired,
#endif
#ifndef MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT
        // Certificate Enrollment error 0x0500 - 0x05ff. Defined in ce_status.h
        EnrollmentErrorBase = CE_STATUS_RANGE_BASE,
        EnrollmentErrorEnd = CE_STATUS_RANGE_END,
#endif // MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT
#ifdef MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS
        // Device Insights error 0x0600 - 0x06ff. Defined in di_status.h
        DeviceInsightsErrorBase = DI_STATUS_RANGE_BASE,
        DeviceInsightsErrorEnd = DI_STATUS_RANGE_END
#endif // MBED_CONF_MBED_CLOUD_CLIENT_ENABLE_DEVICE_INSIGHTS

    }Error;

#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
    /**
     * \brief Enum defining authorization requests from Update Client.
     */
    enum {
        UpdateRequestInvalid                    = UpdateClient::RequestInvalid,
        UpdateRequestDownload                   = UpdateClient::RequestDownload,
        UpdateRequestInstall                    = UpdateClient::RequestInstall
    };
#endif

    /**
     * \brief Constructor
     */
    MbedCloudClient();

    /**
     * \brief Constructor with callback registration.
     * \param on_registered_cb Callback function that Device Management Client calls when the client has registered
     * successfully to Device Management.
     * \param on_unregistered_cb Callback function that Device Management Client calls when the client has unregistered
     * successfully from Device Management.
     * \param on_error_cb Callback function that Device Management Client calls when there is an error occuring in the
     * client functionality.
     * \param on_update_authorize_cb Callback function that Update Client calls to authorize firmware download or
     * an firmware update.
     * \param on_update_progress_cb Callback function that Update Client calls to report download progress.
     */
    MbedCloudClient(void(*on_registered_cb)(void),
                    void(*on_unregistered_cb)(void),
                    void(*on_error_cb)(int)
#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
                    ,void(*update_authorize_cb)(int32_t request) = NULL,
                    void(*update_progress_cb)(uint32_t progress, uint32_t total) = NULL
#endif
                   );

    /**
     * \brief Destructor
     */
    virtual ~MbedCloudClient();

    /**
     * \brief Add a list of Objects that the application wants to register to the
     * LwM2M server. This function must be called before calling the `setup()`
     * API. Otherwise, the application gets error `ConnectInvalidParameters`, when
     * calling `setup()`.
     * \param object_list Objects that contain information about Device Management Client
     * attempting to register to the LwM2M server.
     */
    void add_objects(const M2MObjectList& object_list);

    /**
     * \brief Add a list of M2MBase interface implementing objects that the application wants
     * to register to the LwM2M server. This function must be called before calling the `setup()`
     * API. Otherwise, the application gets error `ConnectInvalidParameters`, when
     * calling `setup()`.
     * \param base_list Object implementing the M2MBase interface that contain information about Device Management
     * Client attempting to register to the LwM2M server.
     */
    void add_objects(const M2MBaseList& base_list);

    void remove_object(M2MBase *object);

    /**
     * \brief Set the callback function that is called when there is
     * a new update on any Object/ObjectInstance/Resource from the LwM2M server,
     * typically on receiving `PUT` commands on the registered Objects.
     * \param callback Passes the class Object that implements the callback
     * function to handle the incoming `PUT` request on a given Object.
     */
    void set_update_callback(MbedCloudClientCallback *callback);

    /**
     * \brief Initialize the Device Management Client library.
     *
     * If you have not called `init()` API separately, `setup()` will call it internally.
     *
     * You can use this API to ensure two-phased memory allocation
     * for initialization of Device Management Client.
     * This is important in a constrained environment
     * where the network stack or some other component may consume run-time
     * memory before the application calls the `setup()` API.
     * \return True on success or false in case of failure.
     * False means your application is running low on memory and all APIs,
     * except `setup()`, will fail with undefined behaviour.
     */
    bool init();

    /**
     * \brief Initiate bootstrapping (first time usage) and register the Device Management
     * Client application to the service.
     *
     * If you have not called `init()` API separately, `setup()` will call it internally
     *
     * \param iface A handler to the network interface.
     * \return True on success or false in case of failure.
     * False means your application is running low on memory and all APIs will fail with undefined behaviour.
     * An application using Device Management Client must be able to recover from the failure and retry the initialization of
     * Device Management Client by calling this API or `init()` at later stage.
     */
    bool setup(void* iface);

    /**
     * \brief Set the callback function that is called when Device Management Client is registered
     * successfully to Device Management. This is used for a statically defined function.
     * \param fn Function pointer to the function that is called when Device Management Client
     * is registered.
     */
    void on_registered(void(*fn)(void));

    /**
    * \brief Set the callback function that is called when Device Management Client is registered
    * successfully to Device Management. This is an overloaded function for a class function.
    * \param object Function pointer to the function that is called when Device Management Client
    * is registered.
    */
    template<typename T>
    void on_registered(T *object, void (T::*member)(void));

    /**
     * \brief Set the callback function that is called when there is any error
     * occuring in the client functionality. The error code can be mapped from the
     * `MbedCloudClient::Error` enum. This is used for a statically defined function.
     * \param fn Function pointer to the function that is called when there
     * is an error in Device Management Client.
     */
    void on_error(void(*fn)(int));

    /**
     * \brief Set the callback function that is called when there is an error
     * occuring in the client functionality. The error code can be mapped from
     * `MbedCloudClient::Error` enum. This is an overloaded function for a class function.
     * \param object Function pointer to the function that is called when there
     * is an error in Device Management Client.
     */
    template<typename T>
    void on_error(T *object, void (T::*member)(int));

    /**
     * \brief Set the callback function that is called when Device Management Client is unregistered
     * successfully from Device Management. This is used for a statically defined function.
     * \param fn Function pointer to the function that is called when Device Management Client
     * is unregistered.
     */
    void on_unregistered(void(*fn)(void));

    /**
    * \brief Set the callback function that is called when Device Management Client is unregistered
    * successfully from Device Management. This is an overloaded function for a class function.
    * \param object Function pointer to the function that is called when Device Management Client
    * is unregistered.
    */
    template<typename T>
    void on_unregistered(T *object, void (T::*member)(void));

    /**
     * \brief Set the callback function that is called when Device Management Client registration
     * is updated successfully to Device Management. This is used for a statically defined function.
     * \param fn Function pointer to the function that is called when Device Management Client
     * registration is updated.
     */
    void on_registration_updated(void(*fn)(void));

    /**
     * \brief Set the callback function that is called when Device Management Client registration
     * is updated successfully to Device Management. This is an overloaded function for a class
     * function.
     * \param object Function pointer to the function that is called when Device Management Client
     * registration is updated.
     */
    template<typename T>
        void on_registration_updated(T *object, void (T::*member)(void));

    /**
    * \brief Send a registration update message to Device Management when Device Management Client is registered
    * successfully and there is no internal connection error.
    * If Device Management Client is not connected and there is some other internal network
    * transaction ongoing, this function triggers an error `MbedCloudClient::ConnectNotAllowed`.
    * \deprecated
    */
    void keep_alive() m2m_deprecated;

    /**
    * \brief Send a registration update message to Device Management when Device Management Client is registered
    * successfully and there is no internal connection error.
    * If Device Management Client is not connected and there is some other internal network
    * transaction ongoing, this function triggers an error `MbedCloudClient::ConnectNotAllowed`.
    */
    void register_update();

    /**
    * \brief Close the connection towards Device Management and unregister Device Management Client.
    * This function triggers the `on_unregistered()` callback if set by the application.
    */
    void close();

    /**
     * \brief Return pointer to the `ConnectorClientEndpointInfo` object.
     * \return `ConnectorClientEndpointInfo` pointer.
     */
    const ConnectorClientEndpointInfo *endpoint_info() const;

    /**
     * \brief Set the function that is called for indicating that Device Management Client
     * is going to sleep when the binding mode is selected with queue mode.
     * \param callback Function pointer that is called when Device Management Client
     * goes to sleep.
     */
    void set_queue_sleep_handler(callback_handler handler);

    /**
     * \brief Set the function callback that is called by Device Management Client to
     * fetch a random number from an application to ensure strong entropy.
     * \param random_callback Function pointer that is called by Device Management Client
     * while performing a secure handshake.
     * The function signature should be `uint32_t (*random_number_callback)(void);`.
     */
    void set_random_number_callback(random_number_cb callback);

    /**
     * \brief Set the function callback that is called by Device Management Client to
     * provide an entropy source from an application to ensure strong entropy.
     * \param entropy_callback Function pointer that is called by Device Management Client
     * while performing a secure handshake.
     * Function signature, if using `mbed-client-mbedtls`, should be
     * `int (*mbedtls_entropy_f_source_ptr)(void *data, unsigned char *output,
     *                                     size_t len, size_t *olen);`.
     */
    void set_entropy_callback(entropy_cb callback);

#if MBED_CLOUD_CLIENT_STL_API
    /**
     * \brief Set resource value in the Device Object.
     *
     * \note This API and functionality using STL is being phased out, as it is wasting resources by
     * duplicating data and harming portability by requiring an STL implementation.
     *
     * \param resource Device enum to have value set.
     * \param value String object.
     * \return True if successful, false otherwise.
     */
    bool set_device_resource_value(M2MDevice::DeviceResource resource,
                                   const std::string &value) m2m_deprecated;
#endif

#ifdef MBED_CLOUD_CLIENT_SUPPORT_UPDATE
    /**
     * \brief Register a callback function for authorizing firmware downloads and reboots.
     * \param handler Callback function.
     */
    void set_update_authorize_handler(void (*handler)(int32_t request)) __attribute__((deprecated("Use set_update_authorize_priority_handler instead")));

    /**
     * \brief Register a callback function for authorizing update requests with priority.
     * \param handler Callback function.
     */
    void set_update_authorize_priority_handler(void (*handler)(int32_t request, uint64_t priority));

    /**
     * \brief Authorize request passed to authorization handler.
     * \param request Request being authorized.
     */
    void update_authorize(int32_t request);

    /**
     * \brief Reject request passed to authorization handler.
     * \param request Request being rejected.
     * \param reason Reason for rejecting the request.
     */
    void update_reject(int32_t request, int32_t reason);

    /**
     * \brief Register a callback function for monitoring download progress.
     * \param handler Callback function.
     */
    void set_update_progress_handler(void (*handler)(uint32_t progress, uint32_t total));
#endif

    /**
     * @brief Return error description for the latest error code.
     * @return Error description string.
     */
    const char *error_description() const;

    /**
     * @brief Send the CoAP `GET` request to the server.
     * The API must be called again with the updated offset to complete the whole transfer.
     * @type Download type.
     * @uri URI path to the data.
     * @offset Data offset.
     * @get_data_cb Callback triggered once there is data available.
     * @get_data_error_cb Callback trigged in case of an error.
    */
    void send_get_request(DownloadType type,
                          const char *uri,
                          const size_t offset,
                          get_data_cb data_cb,
                          get_data_error_cb error_cb,
                          void *context);

#ifndef MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT
    /**
    * \brief Initiate a renewal for a specific certificate.
    * The process will generate new keys in order to create a CSR. The CSR is then sent to the EST service to retrieve the renewed certificate.
    * The new certificate is then safely stored in the device, along with its corresponding private key.
    * \note The certificate to be renewed must already exist in the device.
    * \param cert_name A null terminated C string indicating the name of the certificate to be renewed.
    * \return CE_STATUS_SUCCESS if the asynchronous operation has started successfully. In this case, user callback will be executed at the end of the operation, indicating completion status.
    *         If any other `ce_status_e::` status is returned - operation encountered an error before the start of the asynchronous stage and user callback will not be executed.
    */
    ce_status_e certificate_renew(const char *cert_name);

    /**
    * \brief Set the callback function that is called when the certificate renewal process has completed.
    * Must be called before any certificate renewal operation.
    * \param user_cb Function pointer to the user callback. If `user_cb` is NULL - no callback is called when the process has completed.
    */
    void on_certificate_renewal(cert_renewal_cb_f user_cb);
#endif // MBED_CONF_MBED_CLOUD_CLIENT_DISABLE_CERTIFICATE_ENROLLMENT

#ifdef MBED_CLOUD_CLIENT_EDGE_EXTENSION
    /**
     * @brief Return the pointer to the inner object list.
     * You are not allowed to modify the list. It is owned by the Device Management Client instance.
     * @return The inner object list pointer.
     */
    const M2MBaseList *get_object_list() const;
#endif // MBED_CLOUD_CLIENT_EDGE_EXTENSION

    /**
     * \brief Pause Device Management Client's timed functionality and close network connection
     * to Device Management. After a successful call, you can continue the operation
     * by calling `resume()`.
     *
     * \note This operation does not unregister Device Management Client from Device Management.
     * It closes the socket and removes the interface from the interface list.
     */
    void pause();

    /**
     * \brief Resume Device Management Client's timed functionality and network connection
     * to Device Management. Updates registration. Can be only called after
     * a successful call to `pause()`.
     *
     * \param iface A handler to the network interface.
     */
    void resume(void *iface);

#ifndef MBED_CLIENT_DISABLE_EST_FEATURE
    /**
     * \brief Perform enrollment over secure transport for a certificate signing request.
     *
     * \param cert_name Name of certificate to enroll.
     * \param cert_name_length Length of certificate name.
     * \param csr Buffer containing the certificate signing request.
     * \param csr_length Length of CSR buffer.
     * \param result_cb Callback function that will be called when the enrollment has completed.
     * \param context Optional pointer to a user context.
     */
    est_status_e est_request_enrollment(const char *cert_name,
                                        const size_t cert_name_length,
                                        uint8_t *csr,
                                        const size_t csr_length,
                                        est_enrollment_result_cb result_cb,
                                        void *context) const;

    /**
     * \brief Free a certificate chain context structure passed to `est_enrollment_result_cb`
     * callback function.
     *
     * \param context Certificate chain context to free.
     */
    void est_free_cert_chain_context(cert_chain_context_s *context) const;
#endif // !MBED_CLIENT_DISABLE_EST_FEATURE

protected: // from ServiceClientCallback

    /**
    * \brief Indicate that the setup or close operation is complete
    * with success or failure.
    * \param status Indicates success or failure in terms of status code.
    */
    virtual void complete(ServiceClientCallbackStatus status);

    /**
    * \brief Indicates an error condition from Device Management Client.
    * \param error Indicates an error code translated to `MbedCloudClient::Error`.
    * \param reason Indicates human readable text for error description.
    */
    virtual void error(int error, const char *reason);

    /**
    * \brief A callback indicating that the value of the Resource Object is updated
    *  by the LwM2M Device Management server.
    * \param base The Object whose value is updated.
    * \param type The type of the Object.
    */
    virtual void value_updated(M2MBase *base, M2MBase::BaseType type);

private:

    /**
    * \brief Register the update callback functions for `SimpleM2MResourceBase`
    * objects.
    * \param route The URI path of the registered Resource such as `/Test/0/res/`.
    * \param resource Object of the `SimpleM2MResourceBase`.
    */
#if MBED_CLOUD_CLIENT_STL_API
    void register_update_callback(string route, SimpleM2MResourceBase* resource) m2m_deprecated;
#endif

private:

    ServiceClient                                   _client;
    MbedCloudClientCallback                         *_value_callback;
    M2MBaseList                                     _object_list;
    FP0<void>                                       _on_registered;
    FP0<void>                                       _on_unregistered;
    FP0<void>                                       _on_registration_updated;
    FP1<void,int>                                   _on_error;
    const char                                      *_error_description;
    bool                                            _init_done;

#if MBED_CLOUD_CLIENT_STL_API
    // This API and functionality is being phased out, as it is wasting resources by
    // duplicating data and harming portability by requiring an STL implementation.
    map<string, M2MObject*>                         _objects;
    map<string, M2MResource*>                       _resources;
    map<string, SimpleM2MResourceBase*>             _update_values;

friend class SimpleM2MResourceBase;

#endif // MBED_CLOUD_CLIENT_STL_API
};

template<typename T>
void MbedCloudClient::on_registered(T *object, void (T::*member)(void))
{
    FP0<void> fp(object, member);
    _on_registered = fp;
}

template<typename T>
void MbedCloudClient::on_error(T *object, void (T::*member)(int))
{
    FP1<void, int> fp(object, member);
    _on_error = fp;
}

template<typename T>
void MbedCloudClient::on_unregistered(T *object, void (T::*member)(void))
{
    FP0<void> fp(object, member);
    _on_unregistered = fp;
}

template<typename T>
void MbedCloudClient::on_registration_updated(T *object, void (T::*member)(void))
{
    FP0<void> fp(object, member);
    _on_registration_updated = fp;
}
#endif // __MBED_CLOUD_CLIENT_H__
