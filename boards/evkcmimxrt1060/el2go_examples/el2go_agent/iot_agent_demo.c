/*
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <iot_agent_demo_config.h>
#include <iot_agent_osal.h>
#include <nxp_iot_agent.h>
#include <nxp_iot_agent_common.h>
#include <nxp_iot_agent_keystore.h>
#include <nxp_iot_agent_datastore.h>
#include <nxp_iot_agent_utils.h>
#include <nxp_iot_agent_macros.h>
#include <nxp_iot_agent_time.h>

#if NXP_IOT_AGENT_HAVE_SSS
#include <se05x_APDU.h>
#include <ex_sss_boot.h>
#include <nxp_iot_agent_keystore_sss_se05x.h>
#endif

#if IOT_AGENT_CLAIMCODE_INJECT_ENABLE

#if NXP_IOT_AGENT_HAVE_SSS
#include <iot_agent_claimcode_inject.h>

#elif NXP_IOT_AGENT_HAVE_PSA
#if NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM
#include <iot_agent_claimcode_import.h>
#else

#include <iot_agent_claimcode_encrypt.h>
const uint8_t iot_agent_claimcode_el2go_pub_key[1 + NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY_SIZE] = {
    0x04,
    NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY
};
const size_t iot_agent_claimcode_el2go_pub_key_size = sizeof(iot_agent_claimcode_el2go_pub_key);
#endif // NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM
#endif // NXP_IOT_AGENT_HAVE_PSA
#endif // IOT_AGENT_CLAIMCODE_INJECT_ENABLE

#if NXP_IOT_AGENT_HAVE_HOSTCRYPTO_MBEDTLS
#include <mbedtls/version.h>
#endif

#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_mbedtls_apis.h>
#endif

#if NXP_IOT_AGENT_HAVE_PSA && USE_RTOS == 1
#include <psa/crypto.h>
#include <iot_agent_psa_sign_test.h>
#endif

#if NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL
#include <fsl_sss_openssl_apis.h>
#include <openssl/opensslv.h>
#include <iot_agent_mqtt_paho.h>
#endif

#if SSS_HAVE_SSCP
#include <fsl_sss_sscp.h>
#include <sm_types.h>
#endif

#if NXP_IOT_AGENT_HAVE_SSS
#include <fsl_sss_se05x_apis.h>
#endif

#if AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1
#ifdef __ZEPHYR__
#include <iot_agent_mqtt_zephyr.h>
#else
#include <iot_agent_mqtt_freertos.h>
#endif

#include <iot_agent_network.h>

#if NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM
#ifdef NXP_IOT_AGENT_ENABLE_LITE
extern void config_mbedtls_threading_alt(void);
#else
extern uint32_t tfm_ns_interface_init(void);
#endif
#endif
#endif

#include <iot_agent_demo_config.h>

#define MAX_UID_DECIMAL_STRING_SIZE 44U
#define DEVICEID_LENGTH             16U

const char * gszEdgeLock2GoDatastoreFilename = "edgelock2go_datastore.bin";
const char * gszDatastoreFilename = "datastore.bin";
const uint32_t gKeystoreId = 0x0000BEEFU;

const char * gszKeystoreFilename = "keystore.bin";

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
extern iot_agent_time_t iot_agent_time;
#endif

void iot_agent_print_uid_integer(char* hexString, size_t len);
#if NXP_IOT_AGENT_HAVE_SSS
iot_agent_status_t iot_agent_print_uid (sss_se05x_session_t* pSession);
#else
iot_agent_status_t iot_agent_print_uid();
#endif

#if	((AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1) || (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL)) && (IOT_AGENT_MQTT_ENABLE == 1)

#define COMMON_NAME_MAX_SIZE 256
const PB_BYTES_ARRAY_T(AWS_ROOT_SERVER_CERT_SIZE) aws_root_server_cert =
{ AWS_ROOT_SERVER_CERT_SIZE, AWS_ROOT_SERVER_CERT };
const PB_BYTES_ARRAY_T(AZURE_ROOT_SERVER_CERT_SIZE) azure_root_server_cert =
{ AZURE_ROOT_SERVER_CERT_SIZE, AZURE_ROOT_SERVER_CERT };
#endif

iot_agent_status_t agent_start(int argc, const char* argv[]);

int main(int argc, const char *argv[]) {

	return iot_agent_osal_start_task(agent_start, argc, argv);
}

void iot_agent_print_uid_integer(char* hexString, size_t len) {

	char decimalString[MAX_UID_DECIMAL_STRING_SIZE + 1];
	uint32_t weigthValueArray[MAX_UID_DECIMAL_STRING_SIZE] = { 0U };
	uint32_t decimalDigitWeightedSum[MAX_UID_DECIMAL_STRING_SIZE] = { 0U };

	// initialize variables used in the calculation
	weigthValueArray[0] = 1U;
	memset(decimalString, '0', MAX_UID_DECIMAL_STRING_SIZE);
	decimalString[MAX_UID_DECIMAL_STRING_SIZE] = '\0';

	// for every loop iteration:
	// - pick the hex char in the string starting from last
	// - compute the the contribution of the actual char to all the weighted sum
	//   for single digits in decimal string
	// - compute the weight values for next calculation
	for (size_t i = 0U; i < len; i++) {
		for (uint8_t j = 0U; j < MAX_UID_DECIMAL_STRING_SIZE; j++) {
			// pick char
			uint32_t charWeightedContrib = 0U;
			unsigned char pickChar = *(hexString + len - 1U - i);

			if ((pickChar >= '0') && (pickChar <= '9')) {
				charWeightedContrib = (((unsigned char)pickChar) - '0');
			}
			else if ((pickChar >= 'A') && (pickChar <= 'F')) {
				charWeightedContrib = (((unsigned char)pickChar) - 'A') + 10U;
			}
			else if ((pickChar >= 'a') && (pickChar <= 'f')) {
				charWeightedContrib = (uint32_t)(pickChar - 'a') + 10U;
			}
			charWeightedContrib *= weigthValueArray[j];
			decimalDigitWeightedSum[j] += charWeightedContrib;
		}
		uint32_t rem = 0U;
		for (uint8_t k = 0U; k < MAX_UID_DECIMAL_STRING_SIZE; k++) {
			weigthValueArray[k] = rem + (weigthValueArray[k] << 4);
			rem = weigthValueArray[k] / 10U;
			weigthValueArray[k] %= 10U;
		}
	}

	// from the decimal digit weighted sum compute the final decimal string
	uint32_t rem = 0U;
	for (uint8_t i = 0U; i < MAX_UID_DECIMAL_STRING_SIZE; i++) {
		uint8_t index = MAX_UID_DECIMAL_STRING_SIZE - 1U - i;
		decimalString[index] += ((rem + decimalDigitWeightedSum[i]) % 10U);
		rem = (rem + decimalDigitWeightedSum[i]) / 10U;
	}

	// print without leasign 0s
	char* printedString;
	printedString = decimalString;
	while (*printedString && *printedString == '0') {
		printedString++;
	}
	IOT_AGENT_INFO("UID in decimal format: %s", printedString);

}

#if NXP_IOT_AGENT_HAVE_SSS
iot_agent_status_t iot_agent_print_uid (sss_se05x_session_t* pSession) {
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
	uint8_t uid[SE050_MODULE_UNIQUE_ID_LEN];
	size_t uidLen = sizeof(uid);
	smStatus_t sm_status;
	SE05x_Result_t result = kSE05x_Result_NA;

	sm_status = Se05x_API_CheckObjectExists(&pSession->s_ctx, (uint32_t)kSE05x_AppletResID_UNIQUE_ID, &result);
	if (SM_OK != sm_status) {
		EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "UID not present on the secure element");
	}

	sm_status =
		Se05x_API_ReadObject(&pSession->s_ctx, (uint32_t)kSE05x_AppletResID_UNIQUE_ID, 0U, (uint16_t)uidLen, uid, &uidLen);
	if (SM_OK != sm_status) {
		EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Error in reading UID from the device");
	}
	char uidHexString[(SE050_MODULE_UNIQUE_ID_LEN * 2U) + 1U];
	for (uint8_t i = 0U; i < SE050_MODULE_UNIQUE_ID_LEN; i++) {
		ASSERT_OR_EXIT_MSG(sprintf((uidHexString + (i * 2U)), "%02X", uid[i]) >= 0, "Error in printing UUID");
	}
	IOT_AGENT_INFO("UID in hex format: %s", uidHexString);
	iot_agent_print_uid_integer(uidHexString, (((size_t)SE050_MODULE_UNIQUE_ID_LEN) * 2U));
exit:
	return agent_status;
}
#else
iot_agent_status_t iot_agent_print_uid(void) {
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	uint8_t uid[DEVICEID_LENGTH];
	char uidHexString[(DEVICEID_LENGTH * 2U) + 1U];

	size_t uidLen = sizeof(uid);
	agent_status = iot_agent_utils_get_device_id(uid, &uidLen);
	ASSERT_OR_EXIT_MSG(agent_status == IOT_AGENT_SUCCESS, "iot_agent_utils_get_device_id failed with 0x%08x", agent_status);

	for (uint8_t i = 0U; i < uidLen; i++) {
		ASSERT_OR_EXIT_MSG(sprintf((uidHexString + (i * 2)), "%02X", uid[i]) >= 0, "Error in printing UUID");
	}

	IOT_AGENT_INFO("UID in hex format: %s", uidHexString);
	iot_agent_print_uid_integer(uidHexString, (uidLen * 2U));
exit:
	return agent_status;
}
#endif


#if	((AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1) || (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL)) && (IOT_AGENT_MQTT_ENABLE == 1)
void iot_agent_fill_service_char_array(char** dest, char *src, size_t len)
{
	char* ptr = malloc(len);
	memcpy(ptr, src, len);
	*dest = ptr;
}

void iot_agent_free_mqtt_service_descriptor(nxp_iot_ServiceDescriptor* service_descriptor)
{
	if (service_descriptor->client_id != NULL)
	{
		free(service_descriptor->client_id);
	}
	if (service_descriptor->hostname != NULL)
	{
		free(service_descriptor->hostname);
	}
	if (service_descriptor->azure_id_scope != NULL)
	{
		free(service_descriptor->azure_id_scope);
	}
	if (service_descriptor->azure_registration_id != NULL)
	{
		free(service_descriptor->azure_registration_id);
	}
	if (service_descriptor->azure_global_device_endpoint != NULL)
	{
		free(service_descriptor->azure_global_device_endpoint);
	}
}

// doc: configure service descriptor - start
iot_agent_status_t iot_agent_get_mqtt_service_descriptor_for_aws(iot_agent_context_t* iot_agent_context,
	nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "Service descriptor is null");

	// AWS Service type
	service_descriptor->identifier = AWS_SERVICE_ID;
	service_descriptor->has_service_type = true;
	service_descriptor->service_type = nxp_iot_ServiceType_AWSSERVICE;

	// service key pair
	service_descriptor->has_client_key_sss_ref = true;
	service_descriptor->client_key_sss_ref.object_id = AWS_SERVICE_KEY_PAIR_ID;
	service_descriptor->client_key_sss_ref.has_type = false;
	service_descriptor->client_key_sss_ref.type = nxp_iot_EndpointType_INVALID;
	service_descriptor->client_key_sss_ref.has_endpoint_id = true;
	service_descriptor->client_key_sss_ref.endpoint_id = EDGELOCK2GO_KEYSTORE_ID;
	service_descriptor->client_key_sss_ref.has_object_id = true;

	// client certificate
	service_descriptor->has_client_certificate_sss_ref = true;
	service_descriptor->client_certificate_sss_ref.object_id = AWS_SERVICE_DEVICE_CERT_ID;
	service_descriptor->client_certificate_sss_ref.has_type = false;
	service_descriptor->client_certificate_sss_ref.type = nxp_iot_EndpointType_INVALID;
	service_descriptor->client_certificate_sss_ref.has_endpoint_id = true;
	service_descriptor->client_certificate_sss_ref.endpoint_id = EDGELOCK2GO_KEYSTORE_ID;
	service_descriptor->client_certificate_sss_ref.has_object_id = true;

	// AWS server certificate
	service_descriptor->server_certificate = (pb_bytes_array_t*)&aws_root_server_cert;
	service_descriptor->has_server_certificate_sss_ref = false;

	// AWS MQTT connection parameters
	char hostname[] = AWS_HOSTNAME;
	iot_agent_fill_service_char_array(&service_descriptor->hostname, hostname, sizeof(hostname));

	service_descriptor->has_port = true;
	service_descriptor->port = 8883;
	service_descriptor->has_timeout_ms = true;
	service_descriptor->timeout_ms = 0x4E20;
	service_descriptor->has_protocol = true;
	service_descriptor->protocol = nxp_iot_ServiceProtocolType_MQTTS;

#ifdef AWS_CLIENT_ID
	char client_id[] = AWS_CLIENT_ID;
	iot_agent_fill_service_char_array(&service_descriptor->client_id, client_id, sizeof(client_id));
#else
	service_descriptor->client_id = malloc(COMMON_NAME_MAX_SIZE);
	memset(service_descriptor->client_id, 0, COMMON_NAME_MAX_SIZE);
	agent_status = iot_agent_utils_get_certificate_common_name(iot_agent_context, service_descriptor, service_descriptor->client_id, COMMON_NAME_MAX_SIZE);
	AGENT_SUCCESS_OR_EXIT();
#endif

	service_descriptor->username = NULL;
	service_descriptor->password = NULL;

	// fill metadata if any
	service_descriptor->customer_metadata_json = NULL;

exit:
	return agent_status;
}

iot_agent_status_t iot_agent_get_service_descriptor_for_azure(iot_agent_context_t* iot_agent_context,
	nxp_iot_ServiceDescriptor* service_descriptor)
{
	iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;

	ASSERT_OR_EXIT_MSG(service_descriptor != NULL, "Service descriptor is null");

	// Azure Service type
	service_descriptor->identifier = AZURE_SERVICE_ID;
	service_descriptor->has_service_type = true;
	service_descriptor->service_type = nxp_iot_ServiceType_AZURESERVICE;

	// service key pair
	service_descriptor->has_client_key_sss_ref = true;
	service_descriptor->client_key_sss_ref.object_id = AZURE_SERVICE_KEY_PAIR_ID;
	service_descriptor->client_key_sss_ref.has_type = false;
	service_descriptor->client_key_sss_ref.type = nxp_iot_EndpointType_INVALID;
	service_descriptor->client_key_sss_ref.has_endpoint_id = true;
	service_descriptor->client_key_sss_ref.endpoint_id = EDGELOCK2GO_KEYSTORE_ID;
	service_descriptor->client_key_sss_ref.has_object_id = true;

	// service client certificate
	service_descriptor->has_client_certificate_sss_ref = true;
	service_descriptor->client_certificate_sss_ref.object_id = AZURE_SERVICE_DEVICE_CERT_ID;
	service_descriptor->client_certificate_sss_ref.has_type = false;
	service_descriptor->client_certificate_sss_ref.type = nxp_iot_EndpointType_INVALID;
	service_descriptor->client_certificate_sss_ref.has_endpoint_id = true;
	service_descriptor->client_certificate_sss_ref.endpoint_id = EDGELOCK2GO_KEYSTORE_ID;
	service_descriptor->client_certificate_sss_ref.has_object_id = true;

	// Azure server certificate
	service_descriptor->server_certificate = (pb_bytes_array_t*)&azure_root_server_cert;
	service_descriptor->has_server_certificate_sss_ref = false;

	// Azure MQTT connection parameters
	char azure_id_scope[] = AZURE_ID_SCOPE;
	iot_agent_fill_service_char_array(&service_descriptor->azure_id_scope, azure_id_scope, sizeof(azure_id_scope));

	// the registration ID will be by defualt set dynamically to the Common Name of the device leaf certficate;
	// it is possible to hardocode it, by uncommenting the AZURE_REGISTRATION_ID in the configuration file
#ifdef AZURE_REGISTRATION_ID
	char azure_registration_id[] = AZURE_REGISTRATION_ID;
	iot_agent_fill_service_char_array(&service_descriptor->azure_registration_id, azure_registration_id, sizeof(azure_registration_id));
#else
	service_descriptor->azure_registration_id = malloc(COMMON_NAME_MAX_SIZE);
	memset(service_descriptor->azure_registration_id, 0, COMMON_NAME_MAX_SIZE);
	agent_status = iot_agent_utils_get_certificate_common_name(iot_agent_context, service_descriptor, service_descriptor->azure_registration_id, COMMON_NAME_MAX_SIZE);
	AGENT_SUCCESS_OR_EXIT();
#endif

	char azure_global_device_endpoint[] = AZURE_GLOBAL_DEVICE_ENDPOINT;
	iot_agent_fill_service_char_array(&service_descriptor->azure_global_device_endpoint, azure_global_device_endpoint, sizeof(azure_global_device_endpoint));

	service_descriptor->has_port = false;
	service_descriptor->port = 0;
	service_descriptor->has_timeout_ms = true;
	service_descriptor->timeout_ms = 0x4E20;
	service_descriptor->has_protocol = false;
	service_descriptor->username = NULL;
	service_descriptor->password = NULL;

	// fill metadata if any
	service_descriptor->customer_metadata_json = NULL;

exit:
	return agent_status;
}
// doc: configure service descriptor - end
#endif

iot_agent_status_t agent_start(int argc, const char* argv[])
{
#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
    iot_agent_time_context_t iot_agent_demo_time = {0};
    iot_agent_time_init_measurement(&iot_agent_demo_time);
#endif
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    iot_agent_context_t iot_agent_context = { 0 };

	// Context including platform specific information
    iot_agent_platform_context_t iot_agent_platform_context = { 0 };

	// The datastore holding data to connect to EdgeLock 2GO cloud service.
	// This right now is left empty. This implies that the agent will fall
	// back to the configuration defined at compile time
	// in nxp_iot_agent_demo_config.h.
	iot_agent_datastore_t edgelock2go_datastore = { 0 };

	// The datastore that is to be filled with service descriptors
	// for customer cloud services.
	iot_agent_datastore_t datastore = { 0 };

	// The keystore (it holds credentials for connecting to EdgeLock 2GO
	// cloud service as well as for customer cloud services).
	iot_agent_keystore_t keystore = { 0 };

    //Intializations of local variables
    size_t number_of_services = 0U;
	nxp_iot_UpdateStatusReport status_report = nxp_iot_UpdateStatusReport_init_default;
#if	((AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1) || (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL)) && (IOT_AGENT_MQTT_ENABLE == 1)
	nxp_iot_ServiceDescriptor aws_service_descriptor = nxp_iot_ServiceDescriptor_init_default;
	nxp_iot_ServiceDescriptor azure_service_descriptor = nxp_iot_ServiceDescriptor_init_default;
#endif

    IOT_AGENT_INFO("Start");

    agent_status = iot_agent_platform_init(argc, argv, &iot_agent_platform_context);
    AGENT_SUCCESS_OR_EXIT();

#if IOT_AGENT_CLAIMCODE_INJECT_ENABLE
#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
	long claimcode_inject_time = 0;
    iot_agent_time_context_t iot_agent_claimcode_inject_time = { 0 };
    iot_agent_time_init_measurement(&iot_agent_claimcode_inject_time);
#endif
#if NXP_IOT_AGENT_HAVE_SSS
    agent_status = iot_agent_claimcode_inject((ex_sss_boot_ctx_t *)iot_agent_platform_context.ctx, IOT_AGENT_CLAIMCODE_STRING, strlen(IOT_AGENT_CLAIMCODE_STRING));
    AGENT_SUCCESS_OR_EXIT_MSG("Injecting claimcode failed");
#elif NXP_IOT_AGENT_HAVE_PSA
#if NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM
    agent_status = iot_agent_claimcode_import();
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_claimcode_import failed: 0x%08x", agent_status);
#else
    agent_status = iot_agent_claimcode_encrypt_and_import(IOT_AGENT_CLAIMCODE_STRING,
            iot_agent_claimcode_el2go_pub_key, iot_agent_claimcode_el2go_pub_key_size);
	AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_claimcode_encrypt failed: 0x%08x", agent_status);
#endif // NXP_IOT_AGENT_HAVE_PSA_IMPL_TFM
#endif
#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
    iot_agent_time_conclude_measurement(&iot_agent_claimcode_inject_time);
    claimcode_inject_time = iot_agent_time_get_measurement(&iot_agent_claimcode_inject_time);
    iot_agent_time_free_measurement_ctx(&iot_agent_claimcode_inject_time);
    IOT_AGENT_INFO("Performance timing: CLAIMCODE_INJECT_TIME : %ldms", claimcode_inject_time);
#endif  //IOT_AGENT_TIME_MEASUREMENT_ENABLE
#endif  //IOT_AGENT_CLAIMCODE_INJECT_ENABLE

	// print the uid of the device
#if NXP_IOT_AGENT_HAVE_SSS
	agent_status = iot_agent_print_uid((sss_se05x_session_t*)&((ex_sss_boot_ctx_t *)(iot_agent_platform_context.ctx))->session);
#else
	agent_status = iot_agent_print_uid();
#endif
	AGENT_SUCCESS_OR_EXIT();

    // doc: initialization of contexts - start
    agent_status = iot_agent_init(&iot_agent_context);
    AGENT_SUCCESS_OR_EXIT();

    /************* Register keystore*************/

    agent_status = iot_agent_keystore_init(&keystore, EDGELOCK2GO_KEYSTORE_ID, &iot_agent_platform_context);
    AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_register_keystore(&iot_agent_context, &keystore);
	AGENT_SUCCESS_OR_EXIT();

    /************* Register datastore*************/

	// This is the datastore that holds connection information on how to connect to
	// the EdgeLock 2GO cloud service itself. Typically this would be a persistent
	// datastore which supports transactions to allow for atomic updates of
	// the URL/port/etc. If a filesystem is available, here, we use a file-based
	// datastore.
	// Note, that the ID of the datastore is a given.
	agent_status = iot_agent_datastore_init(&edgelock2go_datastore,
		nxp_iot_DatastoreIdentifiers_DATASTORE_EDGELOCK2GO_ID, gszEdgeLock2GoDatastoreFilename,
		&iot_agent_service_is_configuration_data_valid);
	AGENT_SUCCESS_OR_EXIT();

	// Configure the datastore using the EdgeLock 2GO hostname and port from one of the inputs:
	// - Command line arguments
	// - Environment variables
	// - Existing valid datastore file
	// - Macro defines
	agent_status = iot_agent_utils_configure_edgelock2go_datastore(&keystore, &edgelock2go_datastore, argc, argv);
	AGENT_SUCCESS_OR_EXIT();

	// For connecting to the EdgeLock 2GO cloud service, we also need to register the
	// datastore that contains the information on how to connect there.
	agent_status = iot_agent_set_edgelock2go_datastore(&iot_agent_context, &edgelock2go_datastore);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_datastore_init(&datastore, 0U, gszDatastoreFilename,
		&iot_agent_service_is_configuration_data_valid);
	AGENT_SUCCESS_OR_EXIT();

	agent_status = iot_agent_register_datastore(&iot_agent_context, &datastore);
    AGENT_SUCCESS_OR_EXIT();
    // doc: initialization of contexts - end

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
    iot_agent_time_conclude_measurement(&iot_agent_demo_time);
    iot_agent_time.init_time = iot_agent_time_get_measurement(&iot_agent_demo_time);
#if IOT_AGENT_CLAIMCODE_INJECT_ENABLE
    iot_agent_time.init_time -= claimcode_inject_time;
#endif
#endif

    // doc: update device configuration - start
	agent_status = iot_agent_update_device_configuration(&iot_agent_context, &status_report);

	// We have a status report which can have some details on the operations that happened.
	// This gives the opportunity to programmatically react on errors/failures. For demonstration
	// purpose, print that information to the console here.
	if ((agent_status == IOT_AGENT_SUCCESS || agent_status == IOT_AGENT_UPDATE_FAILED) && status_report.has_status) {
		iot_agent_print_status_report(&status_report);
	}

	AGENT_SUCCESS_OR_EXIT();
    // doc: update device configuration - end

    // doc: iterating over services - start
    if (!iot_agent_is_service_configuration_data_valid(&iot_agent_context))
    {
        EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Not all configuration data is valid");
    }

    // get total number of services
    number_of_services = iot_agent_get_number_of_services(&iot_agent_context);
    IOT_AGENT_INFO("Found configuration data for %d services.", (int) number_of_services);
    // doc: iterating over services - end

#if IOT_AGENT_TIME_MEASUREMENT_ENABLE
    iot_agent_time_conclude_measurement(&iot_agent_demo_time);
    iot_agent_time.total_time = iot_agent_time_get_measurement(&iot_agent_demo_time);
    iot_agent_time_free_measurement_ctx(&iot_agent_demo_time);
    iot_agent_log_performance_timing();
    memset(&iot_agent_time, 0, sizeof(iot_agent_time));
#endif

#if NXP_IOT_AGENT_HAVE_SSS
    // doc: trigger MQTT connection - start
#if (AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1) || (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL)
    agent_status = iot_agent_verify_mqtt_connection(&iot_agent_context);
    AGENT_SUCCESS_OR_EXIT();
#endif
#endif //NXP_IOT_AGENT_HAVE_SSS
	// doc: trigger MQTT connection - end
    // doc: trigger MQTT connection RTP - start
#if	((AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1) || (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL)) && (IOT_AGENT_MQTT_ENABLE == 1)
	// example code for MQTT conneciton when provisioning services as RTP objects
	iot_agent_cleanup_mqtt_config_files_cos_over_rtp();
	AGENT_SUCCESS_OR_EXIT();
#if AWS_ENABLE
	// modify the connection parameters to allow connection to your AWS account
	agent_status = iot_agent_get_mqtt_service_descriptor_for_aws(&iot_agent_context, &aws_service_descriptor);
	AGENT_SUCCESS_OR_EXIT();
	// in case of AWS device auto-registration the MQTT connection will fail on the first connection; in this
	// use case add additional call to the funcion iot_agent_verify_mqtt_connection_cos_over_rtp
	agent_status = iot_agent_verify_mqtt_connection_cos_over_rtp(&iot_agent_context, &aws_service_descriptor);
	AGENT_SUCCESS_OR_EXIT();
#endif
#if AZURE_ENABLE
	agent_status = iot_agent_get_service_descriptor_for_azure(&iot_agent_context, &azure_service_descriptor);
	AGENT_SUCCESS_OR_EXIT();
	agent_status = iot_agent_verify_mqtt_connection_cos_over_rtp(&iot_agent_context, &azure_service_descriptor);
	AGENT_SUCCESS_OR_EXIT();
#endif
#endif
	// doc: trigger MQTT connection RTP - end

#if NXP_IOT_AGENT_HAVE_PSA && (AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1) && (IOT_AGENT_NONSE_TESTS_ENABLE == 1)
        IOT_AGENT_INFO("Verification of the psa imported object in progress...\r\n");
        agent_status = iot_agent_verify_psa_import(); // use provisioned psa key for sign operation
        AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_verify_psa_import_by_using_blob failed: 0x%08x", agent_status);
        IOT_AGENT_INFO("Verification of the psa imported object succesfull, keyId %2x", IMPORT_OBJ_ID);

        agent_status = iot_agent_verify_psa_export();
        AGENT_SUCCESS_OR_EXIT_MSG("iot_agent_verify_psa_export failed: 0x%08x", agent_status);
#endif // NXP_IOT_AGENT_HAVE_PSA && (AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1)
exit:
#if	((AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1) || (NXP_IOT_AGENT_HAVE_HOSTCRYPTO_OPENSSL)) && (IOT_AGENT_MQTT_ENABLE == 1)
	iot_agent_free_mqtt_service_descriptor(&aws_service_descriptor);
	iot_agent_free_mqtt_service_descriptor(&azure_service_descriptor);
#endif
	//close SE session
	iot_agent_keystore_close_session(&keystore);
	iot_agent_free_update_status_report(&status_report);
    iot_agent_datastore_free(&edgelock2go_datastore);
	iot_agent_datastore_free(&datastore);
	iot_agent_keystore_free(&keystore);
	return agent_status;
}
