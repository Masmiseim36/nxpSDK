/*
 * Copyright 2018-2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef _NXP_IOT_AGENT_CONFIG_H_
#define _NXP_IOT_AGENT_CONFIG_H_

#include <nxp_iot_agent_config_certificates.h>
#include <nxp_iot_agent_config_credentials.h>

#define NXP_IOT_AGENT_MAX_NUM_KEYSTORES       (1U)
#define NXP_IOT_AGENT_MAX_NUM_DATASTORES      (1U)

#define NXP_IOT_AGENT_MAX_NUM_ENDPOINTS     \
    ((size_t)(1U /* agent itself */         \
	+ 1U /* edgelock2go datastore */        \
	+ NXP_IOT_AGENT_MAX_NUM_KEYSTORES       \
	+ NXP_IOT_AGENT_MAX_NUM_DATASTORES))

/**
 * The size of the buffer for handling request messages.
 * Note: Typically, the request message handling is done 
 * via endpoint specific callbacks. This means normally
 * data can be streamed from the source (network connection) to 
 * the sink without having to store everything in buffers.
 * This means this limit is rather artificial. Since the 
 * streaming approach was not always used, if not set, the 
 * EdgeLock 2GO cloud service uses a default of 1024 bytes.
 * If endpoints do have limits on the data they can handle 
 * in their callbacks, they should negotiate with 
 * the respective endpoint driver.
 */
#define NXP_IOT_AGENT_REQUEST_BUFFER_SIZE     (INT32_MAX)


/**
 * The size of the buffer for assembling response messages.
 * Note: This affects the buffer used for assembling 
 * protobuf messages, network layers might on top reserve 
 * space for TLS record assembly.
 */
#define NXP_IOT_AGENT_RESPONSE_BUFFER_SIZE    (4*1024)

/** 
 * The maximum expected size of a certificate chain. This 
 * memory is reserved on the stack when writing the 
 * certificate chain to a file. 
 */
#define NXP_IOT_AGENT_CERTIFICATE_BUFFER_SIZE (4*1024)


/**
 * If this define evaluates to true, the server certificate of 
 * the EdgeLock 2GO cloud service is verified against a list 
 * of trusted certificates as configured 
 * with NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES.
 * For security reasons performing this verification 
 * is recommended.
 */
#define NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE 1


/**
 * If this define evaluates to true, a certificate revocation 
 * list (CRL) will be requested from the EdgeLock 2GO cloud 
 * service after establishing the TLS connection. The server 
 * certificate chain is then verified using a trusted root
 * and the information in the received CRL.
 */
#define NXP_IOT_AGENT_REQUEST_CRL_FROM_EDGELOCK_2GO (1 && NXP_IOT_AGENT_VERIFY_EDGELOCK_2GO_SERVER_CERTIFICATE)


#ifndef EDGELOCK2GO_HOSTNAME
/**
 * The hostname to use for the connection to the EdgeLock 2GO cloud service.
 *
 * This is specific to your customer account, the needed value can be obtaine in the GUI of the 
 * EdgeLock 2GO cloud service in the company settings of the account.
 */
#define EDGELOCK2GO_HOSTNAME             "CHANGE THIS! - company specific hostname"
#endif

#ifndef EDGELOCK2GO_PORT
/**
 * The port to use for the communication to the EdgeLock 2GO cloud service.
 */
#define EDGELOCK2GO_PORT                 443U
#endif


/** 
 * These are the trusted root ca certificates that are used to verify the certificate chain that
 * the EdgeLock 2GO cloud service presents to the EdgeLock 2GO agent.
 *
 * Several certificates for different EdgeLock 2GO cloud service installations are predefined in
 * nxp_iot_agent_config_certificates.h, a selection must be taken based on the environment EdgeLock
 * 2GO agent is about to connect to and based on the ciphersuite that is intended to be used. 
 *
 * EdgeLock 2GO agent will prefer authentication with an ECC client certificate, however, not all
 * secure elements offer ECC functionality. In those cases the authentication will be done with an
 * RSA client certificate.
 *
 * The EdgeLock 2GO cloud service will select the ciphersuite it is using based on the
 * ciphersuites/signature algorithms that the client supports (this is a configuration option to the
 * underlying crypto library (openssl/mbedtls) and communicated to the server as part of the
 * ClientHello). If ciphersuites for ECDSA authentication is choosen, the EdgeLock 2GO cloud service
 * uses an ECC certificate, in case a RSA authentication is chosen, it uses an RSA certificate.
 * Therefore, if ECDSA authentication is in the supported ciphersuites/signature algorithms list of
 * the client, an ECC root CA certificate has to be trusted, in case RSA authentication is in the
 * list, an RSA root CA certificate has to be trusted. It is possible to give more than one trusted
 * certificates by concatenating them when defining NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES.
 *
 * Available certificates:
 *   - NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_ECC_PROD
 *   - NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_RSA_PROD
 */

#if defined(NXP_IOT_AGENT_ENABLE_SANDBOX_TEST) && (NXP_IOT_AGENT_ENABLE_SANDBOX_TEST == 1)
 /* Test / Sandbox certificates */

#define NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES { \
    NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_RSA_SANDBOX,    \
    NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_ECC_SANDBOX     \
}

#define NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES_SIZE \
    NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_RSA_SANDBOX_SIZE + \
    NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_ECC_SANDBOX_SIZE

#define NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY        NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY_SANDBOX
#define NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY_SIZE   NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY_SANDBOX_SIZE

#else
#define NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES { \
    NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_RSA_PROD,    \
    NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_ECC_PROD     \
}

#define NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY        NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY_PROD
#define NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY_SIZE   NXP_IOT_AGENT_CLAIMCODE_KEY_AGREEMENT_PUBLIC_KEY_PROD_SIZE


 /**
  * Total size of the trusted root ca certificates. This must match the certificates configured in
  * NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES.
  */
#define NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERTIFICATES_SIZE \
    NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_RSA_PROD_SIZE + \
    NXP_IOT_AGENT_TRUSTED_ROOT_CA_CERT_ECC_PROD_SIZE
#endif

/**
 * Flag to enable or disable measurement performance timings of EdgeLock 2GO Agent.
 */
#ifndef IOT_AGENT_TIME_MEASUREMENT_ENABLE
#define IOT_AGENT_TIME_MEASUREMENT_ENABLE     1
#endif
 
/**
 * Set the log-level that is used for output of messages of the openssl engine. The level is a 
 * combination of flags for different message types:
 *   - 0x04 : Error message
 *   - 0x02 : Debug messages
 *   - 0x01 : Flow messages
 */
#define NXP_IOT_AGENT_OPENSSL_ENGINE_LOG_LEVEL 0x04

/**
 * Set the maximum possible size of key data that can be received from EdgeLock 2GO. 
 */
#define NXP_IOT_AGENT_MAX_KEY_DATA_SIZE (10*1024)

#ifndef ENABLE_IOT_AGENT_TRACE
#define ENABLE_IOT_AGENT_TRACE     0
#endif

#ifndef ENABLE_IOT_AGENT_DEBUG
#define ENABLE_IOT_AGENT_DEBUG     1
#endif

#ifndef ENABLE_IOT_AGENT_INFO
#define ENABLE_IOT_AGENT_INFO      1
#endif

#ifndef ENABLE_IOT_AGENT_WARN
#define ENABLE_IOT_AGENT_WARN      1
#endif

#ifndef ENABLE_IOT_AGENT_ERROR
#define ENABLE_IOT_AGENT_ERROR     1
#endif

#endif // #ifndef _NXP_IOT_AGENT_CONFIG_H_
