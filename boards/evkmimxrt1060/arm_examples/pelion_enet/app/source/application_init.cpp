// ----------------------------------------------------------------------------
// Copyright 2016-2020 ARM Ltd.
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

#ifdef MBED_CLOUD_CLIENT_USER_CONFIG_FILE
#include MBED_CLOUD_CLIENT_USER_CONFIG_FILE
#endif

#include "mbed-trace/mbed_trace.h"
#include "mbed-trace-helper.h"
#include "factory_configurator_client.h"
#include "app_platform_setup.h"
#include "mcc_common_setup.h"
#include "mcc_common_button_and_led.h"
#if defined (MBED_HEAP_STATS_ENABLED) || (MBED_STACK_STATS_ENABLED)
#include "memory_tests.h"
#endif
#include "application_init.h"
#include "mbed-client-randlib/mbed-client-randlib/randLIB.h"
#ifdef MBED_CONF_MBED_CLOUD_CLIENT_SECURE_ELEMENT_SUPPORT
#include "mcc_se_init.h"
#endif 

void print_fcc_status(int fcc_status)
{
#ifndef MCC_MINIMAL
#ifndef DISABLE_ERROR_DESCRIPTION
    const char *error;
    switch(fcc_status) {
        case FCC_STATUS_SUCCESS:
            return;
        case FCC_STATUS_ERROR :
            error = "Operation ended with an unspecified error.";
            break;
        case FCC_STATUS_MEMORY_OUT:
            error = "An out-of-memory condition occurred.";
            break;
        case FCC_STATUS_INVALID_PARAMETER:
            error = "A parameter provided to the function was invalid.";
            break;
        case FCC_STATUS_STORE_ERROR:
            error = "Storage internal error.";
            break;
        case FCC_STATUS_INTERNAL_ITEM_ALREADY_EXIST:
            error = "Current item already exists in storage.";
            break;
        case FCC_STATUS_CA_ERROR:
            error = "CA Certificate already exist in storage (currently only bootstrap CA)";
            break;
        case FCC_STATUS_ROT_ERROR:
            error = "ROT already exist in storage";
            break;
        case FCC_STATUS_ENTROPY_ERROR:
            error = "Entropy already exist in storage";
            break;
        case FCC_STATUS_FACTORY_DISABLED_ERROR:
            error = "FCC flow was disabled - denial of service error.";
            break;
        case FCC_STATUS_INVALID_CERTIFICATE:
            error = "Invalid certificate found.";
            break;
        case FCC_STATUS_INVALID_CERT_ATTRIBUTE:
            error = "Operation failed to get an attribute.";
            break;
        case FCC_STATUS_INVALID_CA_CERT_SIGNATURE:
            error = "Invalid ca signature.";
            break;
        case FCC_STATUS_EXPIRED_CERTIFICATE:
            error = "LWM2M or Update certificate is expired.";
            break;
        case FCC_STATUS_INVALID_LWM2M_CN_ATTR:
            error = "Invalid CN field of certificate.";
            break;
        case FCC_STATUS_KCM_ERROR:
            error = "KCM basic functionality failed.";
            break;
        case FCC_STATUS_KCM_STORAGE_ERROR:
            error = "KCM failed to read, write or get size of item from/to storage.";
            break;
        case FCC_STATUS_KCM_FILE_EXIST_ERROR:
            error = "KCM tried to create existing storage item.";
            break;
        case FCC_STATUS_KCM_CRYPTO_ERROR:
            error = "KCM returned error upon cryptographic check of an certificate or key.";
            break;
        case FCC_STATUS_NOT_INITIALIZED:
            error = "FCC failed or did not initialized.";
            break;
        case FCC_STATUS_BUNDLE_ERROR:
            error = "Protocol layer general error.";
            break;
        case FCC_STATUS_BUNDLE_RESPONSE_ERROR:
            error = "Protocol layer failed to create response buffer.";
            break;
        case FCC_STATUS_BUNDLE_UNSUPPORTED_GROUP:
            error = "Protocol layer detected unsupported group was found in a message.";
            break;
        case FCC_STATUS_BUNDLE_INVALID_GROUP:
            error = "Protocol layer detected invalid group in a message.";
            break;
        case FCC_STATUS_BUNDLE_INVALID_SCHEME:
            error = "The scheme version of a message in the protocol layer is wrong.";
            break;
        case FCC_STATUS_ITEM_NOT_EXIST:
            error = "Current item wasn't found in the storage";
            break;
        case FCC_STATUS_EMPTY_ITEM:
            error = "Current item's size is 0";
            break;
        case FCC_STATUS_WRONG_ITEM_DATA_SIZE:
            error = "Current item's size is different then expected";
            break;
        case FCC_STATUS_URI_WRONG_FORMAT:
            error = "Current URI is different than expected.";
            break;
        case FCC_STATUS_FIRST_TO_CLAIM_NOT_ALLOWED:
            error = "Can't use first to claim without bootstrap or with account ID";
            break;
        case FCC_STATUS_BOOTSTRAP_MODE_ERROR:
            error = "Wrong value of bootstrapUse mode.";
            break;
        case FCC_STATUS_OUTPUT_INFO_ERROR:
            error = "The process failed in output info creation.";
            break;
        case FCC_STATUS_WARNING_CREATE_ERROR:
            error = "The process failed in output info creation.";
            break;
        case FCC_STATUS_UTC_OFFSET_WRONG_FORMAT:
            error = "Current UTC is wrong.";
            break;
        case FCC_STATUS_CERTIFICATE_PUBLIC_KEY_CORRELATION_ERROR:
            error = "Certificate's public key failed do not matches to corresponding private key";
            break;
        case FCC_STATUS_BUNDLE_INVALID_KEEP_ALIVE_SESSION_STATUS:
            error = "The message status is invalid.";
            break;
        default:
            error = "UNKNOWN";
    }
    printf("\nFactory Configurator Client [ERROR]: %s\r\n", error);
#endif
#endif
}

#if defined(__SXOS__) || defined(__RTX)

extern "C"
void trace_printer(const char* str)
{
    printf("%s\r\n", str);
}

#endif

bool application_init_mbed_trace(void)
{
#ifndef MCC_MINIMAL
    // Create mutex for tracing to avoid broken lines in logs
    if(!mbed_trace_helper_create_mutex()) {
        printf("ERROR - Mutex creation for mbed_trace failed!\n");
        return 1;
    }
#endif

    // Initialize mbed trace
    (void) mbed_trace_init();
    // If you want, you can filter out the trace set.
    // For example  mbed_trace_include_filters_set("COAP");
    // would enable only the COAP level traces.
    // Or           mbed_trace_exclude_filters_set("COAP");
    // would leave them out.
    // More details on mbed_trace.h file.

#ifndef MCC_MINIMAL
    mbed_trace_mutex_wait_function_set(mbed_trace_helper_mutex_wait);
    mbed_trace_mutex_release_function_set(mbed_trace_helper_mutex_release);
#endif

#if defined(__SXOS__) || defined(__RTX)
    mbed_trace_print_function_set(trace_printer);
#endif

    return 0;
}

static bool application_init_verify_cloud_configuration()
{
    int status;
    bool result = 0;

#if MBED_CONF_APP_DEVELOPER_MODE == 1
    printf("Starting developer flow\r\n");
    status = fcc_developer_flow();
    if (status == FCC_STATUS_KCM_FILE_EXIST_ERROR) {
        printf("Developer credentials already exist, continuing..\r\n");
        result = 0;
    } else if (status != FCC_STATUS_SUCCESS) {
        printf("Failed to load developer credentials\r\n");
        result = 1;
    }
#endif
#ifndef MCC_MINIMAL
#if MBED_CONF_APP_DEVELOPER_MODE == 1
    status = fcc_verify_device_configured_4mbed_cloud();
    print_fcc_status(status);
    if (status != FCC_STATUS_SUCCESS && status != FCC_STATUS_EXPIRED_CERTIFICATE) {
        result = 1;
    }
#endif
#endif
    return result;
}

static bool application_init_fcc(void)
{
#ifdef MBED_STACK_STATS_ENABLED
    print_stack_statistics();
#endif
    int status;
    status = mcc_platform_fcc_init();
    if(status != FCC_STATUS_SUCCESS) {
        printf("application_init_fcc fcc_init failed with status %d! - exit\r\n", status);
        return 1;
    }
#if RESET_STORAGE
    status = mcc_platform_reset_storage();
    if(status != FCC_STATUS_SUCCESS) {
        printf("application_init_fcc reset_storage failed with status %d! - exit\r\n", status);
        return 1;
    }
    // Reinitialize SOTP
    status = mcc_platform_sotp_init();
    if (status != FCC_STATUS_SUCCESS) {
        printf("application_init_fcc sotp_init failed with status %d! - exit\r\n", status);
        return 1;
    }
#endif

#ifdef MBED_CONF_MBED_CLOUD_CLIENT_SECURE_ELEMENT_SUPPORT
    //Initialize secure element
    status = mcc_se_init();
    if (status != 0) {
        printf("Failed to initialize secure element\n");
        return 1;
    }
#endif
    status = application_init_verify_cloud_configuration();
    if (status != 0) {
#ifndef MCC_MINIMAL
    // This is designed to simplify user-experience by auto-formatting the
    // primary storage if no valid certificates exist.
    // This should never be used for any kind of production devices.
#ifndef MBED_CONF_APP_MCC_NO_AUTO_FORMAT
        status = mcc_platform_reset_storage();
        if (status != FCC_STATUS_SUCCESS) {
            return 1;
        }
        status = mcc_platform_sotp_init();
        if (status != FCC_STATUS_SUCCESS) {
            return 1;
        }
        status = application_init_verify_cloud_configuration();
        if (status != 0) {
            return 1;
        }
#else
        return 1;
#endif
#endif
    }
    return 0;
}

bool application_init(void)
{
    // The function always returns 0.
    (void) mcc_platform_init_button_and_led();

    // Print some statistics of current heap memory consumption, useful for finding
    // out where the memory goes.
#ifdef MBED_HEAP_STATS_ENABLED
    print_heap_stats();
#endif

#ifdef MBED_STACK_STATS_ENABLED
    print_stack_statistics();
#endif
    printf("Start Device Management Client\r\n");

    if (application_init_fcc() != 0) {
        printf("Failed initializing FCC\r\n" );
        return false;
    }

    return true;
}

void wait_application_startup_delay()
{
#if (STARTUP_MIN_RANDOM_DELAY > STARTUP_MAX_RANDOM_DELAY)
#error "STARTUP_MAX_RANDOM_DELAY must be larger than STARTUP_MIN_RANDOM_DELAY"
#endif
    randLIB_seed_random();
    uint16_t delay = randLIB_get_random_in_range(STARTUP_MIN_RANDOM_DELAY, STARTUP_MAX_RANDOM_DELAY);
    printf("Delaying registration by %d seconds\r\n", delay);
    mcc_platform_do_wait(delay * 1000);
}
