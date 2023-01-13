/*
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_CLI_Console.h"
#include "FreeRTOS_CLI.h"
#include "kvstore.h"
#include "core_pkcs11.h"
#include "mbedtls/base64.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/pem.h"

/**
 * @brief Maximum length of the buffer used to send a command output to console.
 * The implementation uses this buffer to return success or failure code along with
 * any reason to the output console.
 */
#define MAX_COMMAND_BUFFER_LENGTH    ( 512 )

/**
 * @brief Maximum length of a PKI object retrieved through CLI.
 * PKI objects can be Certificate or a public key which are DER/PEM encoded.
 */
#define MAX_PKI_OBJECT_LENGTH        ( 2048 )

#define MODULUS_LENGTH               pkcs11RSA_2048_MODULUS_BITS / 8

/**
 * @brief This static buffer is used to read a PKCS11 object from secure element
 * or write the object read from console into secure element.
 */
static uint8_t pkcs11Object[ MAX_PKI_OBJECT_LENGTH ];

static BaseType_t prvConfigCommandHandler( char * pcWriteBuffer,
                                           size_t xWriteBufferLen,
                                           const char * pcCommandString );

static BaseType_t prvPKICommandHandler( char * pcWriteBuffer,
                                        size_t xWriteBufferLen,
                                        const char * pcCommandString );

static CLI_Command_Definition_t xCommandConfig =
{
    .pcCommand                   = "conf",
    .pcHelpString                = "\r\n"
                                   "conf:\r\n"
                                   "    Command to change or retrieve configuration for the device.\r\n"
                                   "    Usage: conf [get|set] key [value]\r\n",
    .pxCommandInterpreter        = prvConfigCommandHandler,
    .cExpectedNumberOfParameters = -1
};

static CLI_Command_Definition_t xPKICommandConfig =
{
    .pcCommand                   = "pki",
    .pcHelpString                = "\r\n"
                                   "pki:\r\n"
                                   "   Perform Public Key Infrastructure operations for device.\r\n"
                                   "   Usage: pki [get|set] [cert|pub_key] label\r\n",
    .pxCommandInterpreter        = prvPKICommandHandler,
    .cExpectedNumberOfParameters = -1
};

extern xConsoleIO_t uartConsoleIO;

static char commandBuffer[ MAX_COMMAND_BUFFER_LENGTH + 1 ];

static BaseType_t prvConfigCommandHandler( char * pcWriteBuffer,
                                           size_t xWriteBufferLen,
                                           const char * pcCommandString )
{
    BaseType_t result = pdFAIL;
    const char * pRequest = NULL, * pKey = NULL, * pValue = NULL;
    BaseType_t requestLength = 0, keyLength = 0, valueLength = 0;
    KVStoreKey_t kvStoreKey;


    pRequest = FreeRTOS_CLIGetParameter( pcCommandString, 1U, &requestLength );

    if( pRequest != NULL )
    {
        if( strncmp( pRequest, "get", requestLength ) == 0 )
        {
            pKey = FreeRTOS_CLIGetParameter( pcCommandString, 2U, &keyLength );

            if( pKey != NULL )
            {
                result = KVStore_getKey( pKey, keyLength, &pKey, &kvStoreKey );

                if( result == pdPASS )
                {
                    valueLength = KVStore_getString( kvStoreKey, pcWriteBuffer, xWriteBufferLen );

                    if( valueLength == 0 )
                    {
                        result = pdFAIL;
                    }
                }
            }
        }
        else if( strncmp( pRequest, "set", requestLength ) == 0 )
        {
            pKey = FreeRTOS_CLIGetParameter( pcCommandString, 2U, &keyLength );
            pValue = FreeRTOS_CLIGetParameter( pcCommandString, 3U, &valueLength );

            if( ( pKey != NULL ) && ( pValue != NULL ) )
            {
                result = KVStore_getKey( pKey, keyLength, &pKey, &kvStoreKey );

                if( result == pdPASS )
                {
                    result = KVStore_setString( kvStoreKey, valueLength, pValue );

                    if( result == pdPASS )
                    {
                        strncpy( pcWriteBuffer, "OK", xWriteBufferLen );
                    }
                }
            }
        }
    }

    if( result != pdPASS )
    {
        strncpy( pcWriteBuffer, "ERR", xWriteBufferLen );
    }

    return pdFALSE;
}


static CK_RV prvOpenPKCS11Session( CK_FUNCTION_LIST_PTR pxFunctionList,
                                   CK_SESSION_HANDLE_PTR pSession )
{
    /* Find the certificate */
    CK_RV xResult;
    CK_SLOT_ID xSlotId;
    CK_ULONG xCount = 1;


    xResult = pxFunctionList->C_Initialize( NULL );

    if( ( CKR_OK == xResult ) || ( CKR_CRYPTOKI_ALREADY_INITIALIZED == xResult ) )
    {
        xResult = pxFunctionList->C_GetSlotList( CK_TRUE, &xSlotId, &xCount );
    }

    if( CKR_OK == xResult )
    {
        xResult = pxFunctionList->C_OpenSession( xSlotId, CKF_SERIAL_SESSION, NULL, NULL, pSession );
    }

    return xResult;
}

static CK_RV prvGetPKCS11Object( const char * pcLabelName,
                                 size_t labelLength,
                                 uint8_t * pObject,
                                 uint32_t * pObjlength )
{
    CK_ATTRIBUTE xTemplate;
    CK_RV xResult = CKR_OK;
    CK_ULONG ulCount = 0;
    CK_BBOOL xFindInit = CK_FALSE;
    CK_FUNCTION_LIST_PTR xFunctionList;
    CK_SESSION_HANDLE xSession = CKR_SESSION_HANDLE_INVALID;
    CK_OBJECT_HANDLE xObjHandle = CKR_OBJECT_HANDLE_INVALID;

    xResult = C_GetFunctionList( &xFunctionList );

    if( xResult == CKR_OK )
    {
        xResult = prvOpenPKCS11Session( xFunctionList, &xSession );
    }

    /* Get the certificate handle. */
    if( CKR_OK == xResult )
    {
        xTemplate.type = CKA_LABEL;
        xTemplate.ulValueLen = labelLength + 1;
        xTemplate.pValue = ( char * ) pcLabelName;
        xResult = xFunctionList->C_FindObjectsInit( xSession, &xTemplate, 1 );
    }

    if( CKR_OK == xResult )
    {
        xFindInit = CK_TRUE;
        xResult = xFunctionList->C_FindObjects( xSession,
                                                ( CK_OBJECT_HANDLE_PTR ) &xObjHandle,
                                                1,
                                                &ulCount );
    }

    if( CK_TRUE == xFindInit )
    {
        ( void ) xFunctionList->C_FindObjectsFinal( xSession );
    }

    if( ( CKR_OK == xResult ) && ( xObjHandle != CKR_OBJECT_HANDLE_INVALID ) )
    {
        /* Get the length of the object */
        xTemplate.type = CKA_VALUE;
        xTemplate.pValue = NULL;
        xResult = xFunctionList->C_GetAttributeValue( xSession, xObjHandle, &xTemplate, ulCount );

        if( xResult == CKR_OK )
        {
            if( xTemplate.ulValueLen <= *pObjlength )
            {
                xTemplate.pValue = pObject;
                xResult = xFunctionList->C_GetAttributeValue( xSession, xObjHandle, &xTemplate, ulCount );
            }
            else
            {
                xResult = CKR_HOST_MEMORY;
            }

            if( xResult == CKR_OK )
            {
                *pObjlength = xTemplate.ulValueLen;
            }
        }
    }
    else
    {
        xResult = CKR_FUNCTION_FAILED;
    }

    if( xSession != CKR_SESSION_HANDLE_INVALID )
    {
        ( void ) xFunctionList->C_CloseSession( xSession );
    }

    return xResult;
}

static int prvWriteCertPemToConsole( const unsigned char * int_ca_cert_der,
                                     int len_der )
{
    int ret;
    const char start[] = "-----BEGIN CERTIFICATE-----\r\n";
    const char end[] = "-----END CERTIFICATE-----\r\n";

#define CERT_PER_LINE    64
    static char cCertPem[ 4096 ] = { 0 };
    size_t offset = 0U, certLength = 0U;

    ret = mbedtls_pem_write_buffer( start,
                                    end,
                                    int_ca_cert_der,
                                    len_der,
                                    cCertPem,
                                    4096,
                                    &certLength );

    if( ret == 0 )
    {
        for( offset = 0; offset < certLength; offset++ )
        {
            if( ( cCertPem[ offset ] == '\n' ) &&
                ( ( offset == 0 ) || ( cCertPem[ offset - 1 ] != '\r' ) ) )
            {
                uartConsoleIO.write( "\r\n", 2U );
            }
            else
            {
                uartConsoleIO.write( &cCertPem[ offset ], 1U );
            }
        }
    }

    return ret;
}

static CK_RV prvWriteCertificate( const char * pcLabelName,
                                  size_t labelLength )
{
    CK_RV pkcs11ret = CKR_OK;
    int ret;
    uint32_t objectLength = MAX_PKI_OBJECT_LENGTH;
    mbedtls_x509_crt certificate = { 0 };

    memset( pkcs11Object, 0x00, sizeof( pkcs11Object ) );

    pkcs11ret = prvGetPKCS11Object( pcLabelName,
                                    labelLength,
                                    pkcs11Object,
                                    &objectLength );

    if( pkcs11ret == CKR_OK )
    {
        mbedtls_x509_crt_init( &certificate );

        ret = mbedtls_x509_crt_parse( &certificate, ( const unsigned char * ) pkcs11Object, objectLength );

        if( ret == 0 )
        {
            /* Write the certificate raw format (DER) as PEM encoded bytes to console. */
            ret = prvWriteCertPemToConsole( certificate.raw.p, certificate.raw.len );
        }

        if( ret != 0 )
        {
            pkcs11ret = CKR_FUNCTION_FAILED;
        }

        mbedtls_x509_crt_free( &certificate );
    }

    return pkcs11ret;
}

/* Import the specified public key into storage. */
CK_RV prvProvisionPublicKey( uint8_t * pucKey,
                             size_t xKeyLength,
                             CK_KEY_TYPE xPublicKeyType,
                             uint8_t * pucPublicKeyLabel,
                             size_t xPublicKeyLabeLength )
{
    CK_RV xResult;
    CK_BBOOL xTrue = CK_TRUE;
    CK_FUNCTION_LIST_PTR pxFunctionList;
    CK_OBJECT_CLASS xClass = CKO_PUBLIC_KEY;
    int lMbedResult = 0;
    mbedtls_pk_context xMbedPkContext = { 0 };
    CK_SESSION_HANDLE xSession = CKR_SESSION_HANDLE_INVALID;
    CK_OBJECT_HANDLE xPublicKeyHandle = CKR_OBJECT_HANDLE_INVALID;

    xResult = C_GetFunctionList( &pxFunctionList );

    if( xResult == CKR_OK )
    {
        xResult = prvOpenPKCS11Session( pxFunctionList, &xSession );
    }

    if( xResult == CKR_OK )
    {
        mbedtls_pk_init( &xMbedPkContext );

        /* Try parsing the private key using mbedtls_pk_parse_key. */
        lMbedResult = mbedtls_pk_parse_key( &xMbedPkContext, pucKey, xKeyLength, NULL, 0 );

        /* If mbedtls_pk_parse_key didn't work, maybe the private key is not included in the input passed in.
         * Try to parse just the public key. */
        if( lMbedResult != 0 )
        {
            lMbedResult = mbedtls_pk_parse_public_key( &xMbedPkContext, pucKey, xKeyLength );
        }

        if( lMbedResult != 0 )
        {
            configPRINTF( ( "Failed to parse the public key. \r\n" ) );
            xResult = CKR_ARGUMENTS_BAD;
        }
    }

    if( ( xResult == CKR_OK ) && ( xPublicKeyType == CKK_RSA ) )
    {
        CK_BYTE xPublicExponent[] = { 0x01, 0x00, 0x01 };
        CK_BYTE xModulus[ MODULUS_LENGTH + 1 ] = { 0 };

        lMbedResult = mbedtls_rsa_export_raw( ( mbedtls_rsa_context * ) xMbedPkContext.pk_ctx,
                                              ( unsigned char * ) &xModulus, MODULUS_LENGTH + 1,
                                              NULL, 0,
                                              NULL, 0,
                                              NULL, 0,
                                              NULL, 0 );
        CK_ATTRIBUTE xPublicKeyTemplate[] =
        {
            { CKA_CLASS,           NULL /* &xClass */,         sizeof( CK_OBJECT_CLASS ) },
            { CKA_KEY_TYPE,        NULL /* &xPublicKeyType */, sizeof( CK_KEY_TYPE )     },
            { CKA_TOKEN,           NULL /* &xTrue */,          sizeof( xTrue )           },
            { CKA_MODULUS,         NULL /* &xModulus + 1 */,   MODULUS_LENGTH            },                          /* Extra byte allocated at beginning for 0 padding. */
            { CKA_VERIFY,          NULL /* &xTrue */,          sizeof( xTrue )           },
            { CKA_PUBLIC_EXPONENT, NULL /* xPublicExponent */, sizeof( xPublicExponent ) },
            { CKA_LABEL,           pucPublicKeyLabel,          xPublicKeyLabeLength      }
        };

        /* Aggregate initializers must not use the address of an automatic variable. */
        /* See MSVC Compiler Warning C4221 */
        xPublicKeyTemplate[ 0 ].pValue = &xClass;
        xPublicKeyTemplate[ 1 ].pValue = &xPublicKeyType;
        xPublicKeyTemplate[ 2 ].pValue = &xTrue;
        xPublicKeyTemplate[ 3 ].pValue = &xModulus + 1;
        xPublicKeyTemplate[ 4 ].pValue = &xTrue;
        xPublicKeyTemplate[ 5 ].pValue = xPublicExponent;

        xResult = pxFunctionList->C_CreateObject( xSession,
                                                  ( CK_ATTRIBUTE_PTR ) xPublicKeyTemplate,
                                                  sizeof( xPublicKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                  &xPublicKeyHandle );
    }
    else if( ( xResult == CKR_OK ) && ( xPublicKeyType == CKK_EC ) )
    {
        CK_BYTE xEcParams[] = pkcs11DER_ENCODED_OID_P256;
        size_t xLength;
        CK_BYTE xEcPoint[ 256 ] = { 0 };

        mbedtls_ecdsa_context * pxEcdsaContext = ( mbedtls_ecdsa_context * ) xMbedPkContext.pk_ctx;

        /* DER encoded EC point. Leave 2 bytes for the tag and length. */
        lMbedResult = mbedtls_ecp_point_write_binary( &pxEcdsaContext->grp,
                                                      &pxEcdsaContext->Q,
                                                      MBEDTLS_ECP_PF_UNCOMPRESSED,
                                                      &xLength,
                                                      xEcPoint + 2,
                                                      sizeof( xEcPoint ) - 2 );
        xEcPoint[ 0 ] = 0x04; /* Octet string. */
        xEcPoint[ 1 ] = ( CK_BYTE ) xLength;

        CK_ATTRIBUTE xPublicKeyTemplate[] =
        {
            { CKA_CLASS,     NULL /* &xClass */,         sizeof( xClass )         },
            { CKA_KEY_TYPE,  NULL /* &xPublicKeyType */, sizeof( xPublicKeyType ) },
            { CKA_TOKEN,     NULL /* &xTrue */,          sizeof( xTrue )          },
            { CKA_VERIFY,    NULL /* &xTrue */,          sizeof( xTrue )          },
            { CKA_EC_PARAMS, NULL /* xEcParams */,       sizeof( xEcParams )      },
            { CKA_EC_POINT,  NULL /* xEcPoint */,        xLength + 2              },
            { CKA_LABEL,     pucPublicKeyLabel,          xPublicKeyLabeLength     }
        };

        /* Aggregate initializers must not use the address of an automatic variable. */
        /* See MSVC Compiler Warning C4221 */
        xPublicKeyTemplate[ 0 ].pValue = &xClass;
        xPublicKeyTemplate[ 1 ].pValue = &xPublicKeyType;
        xPublicKeyTemplate[ 2 ].pValue = &xTrue;
        xPublicKeyTemplate[ 3 ].pValue = &xTrue;
        xPublicKeyTemplate[ 4 ].pValue = xEcParams;
        xPublicKeyTemplate[ 5 ].pValue = xEcPoint;

        xResult = pxFunctionList->C_CreateObject( xSession,
                                                  ( CK_ATTRIBUTE_PTR ) xPublicKeyTemplate,
                                                  sizeof( xPublicKeyTemplate ) / sizeof( CK_ATTRIBUTE ),
                                                  &xPublicKeyHandle );
    }
    else
    {
        xResult = CKR_ATTRIBUTE_VALUE_INVALID;
        configPRINTF( ( "Invalid key type. Supported options are CKK_RSA and CKK_EC.\r\n" ) );
    }

    mbedtls_pk_free( &xMbedPkContext );

    if( xSession != CKR_SESSION_HANDLE_INVALID )
    {
        ( void ) pxFunctionList->C_CloseSession( xSession );
    }

    return xResult;
}

CK_RV prvReadAndProvisionPublicKey( uint8_t * pucPublicKeyLabel,
                                    size_t xPublicKeyLabeLength )
{
    int32_t byteRead;
    uint32_t ulReadOffset = 0, ulLineStart = 0, ulLineLength;
    BaseType_t xReadComplete = pdFALSE;
    CK_RV result = CKR_FUNCTION_FAILED;

    memset( pkcs11Object, 0x00, MAX_PKI_OBJECT_LENGTH );

    /* The object needs to be null terminated for successful parsing. */
    while( ulReadOffset < MAX_PKI_OBJECT_LENGTH - 1U )
    {
        byteRead = uartConsoleIO.getChar();

        if( ( byteRead == '\r' ) || ( byteRead == '\n' ) )
        {
            /* End of a line detected. Process the line now. */
            if( ( ulReadOffset > 0 ) &&
                ( pkcs11Object[ ulReadOffset - 1U ] != '\n' ) )
            {
                ulLineLength = ulReadOffset - ulLineStart;
                pkcs11Object[ ulReadOffset++ ] = '\n';

                if( ( ulLineLength > 0 ) &&
                    ( strncmp( ( char * ) ( pkcs11Object + ulLineStart ),
                               "-----END PUBLIC KEY-----",
                               ulLineLength ) == 0 ) )
                {
                    pkcs11Object[ ulReadOffset++ ] = '\0';
                    xReadComplete = pdTRUE;
                    break;
                }

                ulLineStart = ulReadOffset;
            }
        }
        else if( ( byteRead >= 0 ) && ( byteRead <= 255 ) )
        {
            pkcs11Object[ ulReadOffset++ ] = ( char ) ( byteRead );
        }
        else
        {
            /* There was an error in reading a character. Break out of loop. */
            break;
        }
    }

    if( xReadComplete == pdTRUE )
    {
        result = prvProvisionPublicKey( pkcs11Object,
                                        ulReadOffset,
                                        CKK_EC,
                                        pucPublicKeyLabel,
                                        xPublicKeyLabeLength );
    }

    return result;
}

static BaseType_t prvPKICommandHandler( char * pcWriteBuffer,
                                        size_t xWriteBufferLen,
                                        const char * pcCommandString )
{
    const char * pRequest = NULL, * pObjectType = NULL, * pObjectLabel = NULL;
    BaseType_t requestLength, labelLength, objectTypeLength;
    CK_RV pkcs11Status;

    pRequest = FreeRTOS_CLIGetParameter( pcCommandString, 1U, &requestLength );

    if( pRequest != NULL )
    {
        if( strncmp( pRequest, "get", requestLength ) == 0 )
        {
            pObjectType = FreeRTOS_CLIGetParameter( pcCommandString, 2U, &objectTypeLength );
            pObjectLabel = FreeRTOS_CLIGetParameter( pcCommandString, 3U, &labelLength );

            if( ( pObjectLabel != NULL ) && ( pObjectType != NULL ) )
            {
                if( strncmp( pObjectType, "cert", objectTypeLength ) == 0 )
                {
                    pkcs11Status = prvWriteCertificate( pObjectLabel, labelLength );

                    if( pkcs11Status != CKR_OK )
                    {
                        snprintf( pcWriteBuffer, xWriteBufferLen, "PKCS11 ERR %x", pkcs11Status );
                    }
                    else
                    {
                        snprintf( pcWriteBuffer, xWriteBufferLen, "OK\r\n" );
                    }
                }
                else
                {
                    snprintf( pcWriteBuffer, xWriteBufferLen, "ERR\r\n" );
                }
            }
            else
            {
                snprintf( pcWriteBuffer, xWriteBufferLen, "ERR\r\n" );
            }
        }
        else if( strncmp( pRequest, "set", requestLength ) == 0 )
        {
            pObjectType = FreeRTOS_CLIGetParameter( pcCommandString, 2U, &objectTypeLength );
            pObjectLabel = FreeRTOS_CLIGetParameter( pcCommandString, 3U, &labelLength );

            if( ( pObjectLabel != NULL ) && ( pObjectType != NULL ) )
            {
                if( strncmp( pObjectType, "pub_key", objectTypeLength ) == 0 )
                {
                    pkcs11Status = prvReadAndProvisionPublicKey( (uint8_t * ) pObjectLabel, labelLength );

                    if( pkcs11Status != CKR_OK )
                    {
                        snprintf( pcWriteBuffer, xWriteBufferLen, "PKCS11 ERR %x", pkcs11Status );
                    }
                    else
                    {
                        snprintf( pcWriteBuffer, xWriteBufferLen, "\r\nOK\r\n" );
                    }
                }
                else
                {
                    snprintf( pcWriteBuffer, xWriteBufferLen, "ERR\r\n" );
                }
            }
            else
            {
                snprintf( pcWriteBuffer, xWriteBufferLen, "ERR\r\n" );
            }
        }
        else
        {
            snprintf( pcWriteBuffer, xWriteBufferLen, "ERR\r\n" );
        }
    }
    else
    {
        snprintf( pcWriteBuffer, xWriteBufferLen, "ERR\r\n" );
    }

    return pdFALSE;
}

void vCLITask( void * pvParam )
{
    BaseType_t xResult;

    xResult = FreeRTOS_CLIRegisterCommand( &xCommandConfig );
    configASSERT( xResult == pdTRUE );

    xResult = FreeRTOS_CLIRegisterCommand( &xPKICommandConfig );
    configASSERT( xResult == pdTRUE );

    FreeRTOS_CLIEnterConsoleLoop( uartConsoleIO,
                                  commandBuffer,
                                  MAX_COMMAND_BUFFER_LENGTH,
                                  FreeRTOS_CLIGetOutputBuffer(),
                                  configCOMMAND_INT_MAX_OUTPUT_SIZE );


    vTaskDelete( NULL );
}
