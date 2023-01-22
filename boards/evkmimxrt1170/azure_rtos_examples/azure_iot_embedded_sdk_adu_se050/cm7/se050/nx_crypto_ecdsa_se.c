/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** NetX Crypto Component                                                 */
/**                                                                       */
/**   Elliptic Curve Digital Signature Algorithm (ECDSA)                  */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#include "se_api.h"
#include "nx_crypto_ecdsa_se.h"

#include "fsl_debug_console.h"

/* Declare the ECDSA crypto method */
NX_CRYPTO_METHOD crypto_method_ecdsa_se05x =
{
    NX_CRYPTO_DIGITAL_SIGNATURE_ECDSA,            /* ECDSA crypto algorithm                 */
    0,                                            /* Key size in bits                       */
    0,                                            /* IV size in bits                        */
    0,                                            /* ICV size in bits, not used             */
    0,                                            /* Block size in bytes                    */
    sizeof(NX_CRYPTO_ECDSA_SE05X),                /* Metadata size in bytes                 */
    _nx_crypto_method_ecdsa_se05x_init,           /* ECDSA initialization routine           */
    _nx_crypto_method_ecdsa_se05x_cleanup,        /* ECDSA cleanup routine                  */
    _nx_crypto_method_ecdsa_se05x_operation,      /* ECDSA operation                        */

};

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_ecdsa_sign_se05x                        PORTABLE C       */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Zhen Kong, Microsoft Corporation                                    */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function computes a signature of the hash data using the       */
/*    private key.                                                        */
/*                                                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ecdsa_ptr                             ECDSA context                 */
/*    hash                                  Hash data to be signed        */
/*    hash_length                           Length of hash data           */
/*    private_key                           Pointer to EC private key     */
/*    signature                             Pointer to signature output   */
/*    signature_length                      Signature length              */
/*    actual_signature_length               Pointer to actual sign length */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx        Zhen Kong             Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
static UINT _nx_crypto_ecdsa_sign_se05x(NX_CRYPTO_ECDSA_SE05X *ecdsa_ptr,
                                        UCHAR *hash, UINT hash_length,
                                        UCHAR *private_key, UINT private_key_length,
                                        UCHAR *signature, UINT signature_length,
                                        UINT *actual_signature_length)
{

    NX_CRYPTO_EC *curve = ecdsa_ptr->nx_crypto_ecdsa_curve;
    UINT status;

	if (curve->nx_crypto_ec_id != NX_CRYPTO_EC_SECP256R1)
	{
		PRINTF("ERR: Unsupported ecc curve 0x%x\n", curve->nx_crypto_ec_id);
		return(NX_CRYTPO_MISSING_ECC_CURVE);
	}

	*actual_signature_length = signature_length;

    status = se_device_ecdsa_sign(hash, hash_length,
                                  signature, actual_signature_length);
	if (status != NX_CRYPTO_SUCCESS) {
		PRINTF("ERR: se_device_ecdsa_sign: status = 0x%X, actual_signature_length = %u, signature_length = %u\n",
               status, *actual_signature_length, signature_length);
        return(NX_NOT_SUCCESSFUL);
	}

    return(NX_CRYPTO_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_ecdsa_verify_se05x                       PORTABLE C      */
/*    Zhen Kong, Microsoft Corporation                                    */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function verifies the signature of the hash data using the     */
/*    public key.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ecdsa_ptr                             ECDSA context                 */
/*    hash                                  Hash data to be verified      */
/*    hash_length                           Length of hash data           */
/*    public_key                            Pointer to EC public key      */
/*    signature                             Signature to be verified      */
/*    signature_length                      Signature length              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx        Zhen Kong             Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
static UINT _nx_crypto_ecdsa_verify_se05x(NX_CRYPTO_ECDSA_SE05X *ecdsa_ptr,
                                          UCHAR *hash, UINT hash_length,
                                          UCHAR *public_key, UINT public_key_length,
                                          UCHAR *signature, UINT signature_length)
{
    NX_CRYPTO_EC *curve = ecdsa_ptr->nx_crypto_ecdsa_curve;
    UINT status;

	if (curve->nx_crypto_ec_id != NX_CRYPTO_EC_SECP256R1)
	{
		PRINTF("ERR: Unsupported ecc curve 0x%x\n", curve->nx_crypto_ec_id);
		return(NX_CRYTPO_MISSING_ECC_CURVE);
	}

    status = se_device_ecdsa_verify(hash, hash_length,
                                    signature, signature_length);
	if (status != NX_CRYPTO_SUCCESS) {
		PRINTF("ERR: sss_asymmetric_verify_digest: status = 0x%X\n", status);
        return(NX_NOT_SUCCESSFUL);
	}

    return(NX_CRYPTO_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_ecdsa_se05x_init                  PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Zhen Kong, Microsoft Corporation                                    */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the common crypto method init callback for         */
/*    Express Logic supported ECDSA cryptograhic algorithm.               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    method                                Pointer to crypto method      */
/*    key                                   Pointer to key                */
/*    key_size_in_bits                      Length of key size in bits    */
/*    handler                               Returned crypto handler       */
/*    crypto_metadata                       Metadata area                 */
/*    crypto_metadata_size                  Size of the metadata area     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx        Zhen Kong             Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT _nx_crypto_method_ecdsa_se05x_init(struct NX_CRYPTO_METHOD_STRUCT *method,
                                                       UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                                       VOID  **handle,
                                                       VOID  *crypto_metadata,
                                                       ULONG crypto_metadata_size)
{
    UINT status;

    NX_PARAMETER_NOT_USED(key);
    NX_PARAMETER_NOT_USED(key_size_in_bits);
    NX_PARAMETER_NOT_USED(handle);

    NX_CRYPTO_STATE_CHECK

    if ((method == NX_NULL) || (crypto_metadata == NX_NULL))
    {
        return(NX_PTR_ERROR);
    }

    /* Verify the metadata addrsss is 4-byte aligned. */
    if((((ULONG)crypto_metadata) & 0x3) != 0)
    {
        return(NX_PTR_ERROR);
    }

    if(crypto_metadata_size < sizeof(NX_CRYPTO_ECDSA_SE05X))
    {
        return(NX_PTR_ERROR);
    }

    status = se_open();
    if (status != NX_CRYPTO_SUCCESS)
    {
        return status;
    }

    return(NX_CRYPTO_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_ecdsa_se05x_cleanup         PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function cleans up the crypto metadata.                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    crypto_metadata                       Crypto metadata               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    NX_CRYPTO_MEMSET                      Set the memory                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT _nx_crypto_method_ecdsa_se05x_cleanup(VOID *crypto_metadata)
{
    NX_CRYPTO_STATE_CHECK

#ifdef NX_SECURE_KEY_CLEAR
    if (!crypto_metadata)
        return (NX_CRYPTO_SUCCESS);

    /* Clean up the crypto metadata.  */
    NX_CRYPTO_MEMSET(crypto_metadata, 0, sizeof(NX_CRYPTO_ECDSA_SE05X));
#else
    NX_PARAMETER_NOT_USED(crypto_metadata);
#endif/* NX_SECURE_KEY_CLEAR  */

    se_close();

    return(NX_CRYPTO_SUCCESS);
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_crypto_method_ecdsa_se05x_operation       PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function performs an ECDSA operation.                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    op                                    ECDSA operation               */
/*    handle                                Crypto handle                 */
/*    method                                Cryption Method Object        */
/*    key                                   Encryption Key                */
/*    key_size_in_bits                      Key size in bits              */
/*    input                                 Input data                    */
/*    input_length_in_byte                  Input data size               */
/*    iv_ptr                                Initial vector                */
/*    output                                Output buffer                 */
/*    output_length_in_byte                 Output buffer size            */
/*    crypto_metadata                       Metadata area                 */
/*    crypto_metadata_size                  Metadata area size            */
/*    packet_ptr                            Pointer to packet             */
/*    nx_crypto_hw_process_callback         Callback function pointer     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _nx_crypto_ecdsa_sign_se05x     Sign using ECDSA              */
/*    _nx_crypto_ecdsa_verify_se05x   Verify ECDSA signature        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Timothy Stapko           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
NX_CRYPTO_KEEP UINT _nx_crypto_method_ecdsa_se05x_operation(UINT op,
                                                            VOID *handle,
                                                            struct NX_CRYPTO_METHOD_STRUCT *method,
                                                            UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                                            UCHAR *input, ULONG input_length_in_byte,
                                                            UCHAR *iv_ptr,
                                                            UCHAR *output, ULONG output_length_in_byte,
                                                            VOID *crypto_metadata, ULONG crypto_metadata_size,
                                                            VOID *packet_ptr,
                                                            VOID (*nx_crypto_hw_process_callback)(VOID *, UINT))
{
    NX_CRYPTO_ECDSA_SE05X *ecdsa;
    NX_CRYPTO_EXTENDED_OUTPUT *extended_output;
    UINT status = NX_CRYPTO_SUCCESS;

    NX_PARAMETER_NOT_USED(handle);
    NX_PARAMETER_NOT_USED(iv_ptr);
    NX_PARAMETER_NOT_USED(packet_ptr);
    NX_PARAMETER_NOT_USED(nx_crypto_hw_process_callback);
    NX_PARAMETER_NOT_USED(key);

    NX_CRYPTO_STATE_CHECK

    /* Verify the metadata addrsss is 4-byte aligned. */
    if((method == NX_NULL) || (crypto_metadata == NX_NULL) || ((((ULONG)crypto_metadata) & 0x3) != 0))
    {
        return(NX_PTR_ERROR);
    }

    if(crypto_metadata_size < sizeof(NX_CRYPTO_ECDSA_SE05X))
    {
        return(NX_PTR_ERROR);
    }

    ecdsa = (NX_CRYPTO_ECDSA_SE05X *)crypto_metadata;


    if (op == NX_CRYPTO_EC_CURVE_SET)
    {
        /* Set curve to be used in the ECDSA. */
        status = ((NX_CRYPTO_METHOD *)input) -> nx_crypto_operation(NX_CRYPTO_EC_CURVE_GET,
                                                                    NX_NULL,
                                                                    (NX_CRYPTO_METHOD *)input,
                                                                    NX_NULL, 0,
                                                                    NX_NULL, 0,
                                                                    NX_NULL,
                                                                    (UCHAR *)&ecdsa -> nx_crypto_ecdsa_curve,
                                                                    sizeof(NX_CRYPTO_EC *),
                                                                    NX_NULL, 0,
                                                                    NX_NULL, NX_NULL);
    }
    else if (op == NX_CRYPTO_AUTHENTICATE)
    {

        extended_output = (NX_CRYPTO_EXTENDED_OUTPUT *)output;

        status = _nx_crypto_ecdsa_sign_se05x(ecdsa,
                                           input,
                                           input_length_in_byte,
                                           key,
                                           key_size_in_bits >> 3,
                                           extended_output -> nx_crypto_extended_output_data,
                                           extended_output -> nx_crypto_extended_output_length_in_byte,
                                           (UINT *)&extended_output -> nx_crypto_extended_output_actual_size);
    }
    else if (op == NX_CRYPTO_VERIFY)
    {

        status = _nx_crypto_ecdsa_verify_se05x(ecdsa,
                                             input,
                                             input_length_in_byte,
                                             key,
                                             key_size_in_bits >> 3,
                                             output, output_length_in_byte);
    }
    else
    {
        status = NX_NOT_SUCCESSFUL;
    }

    return(status);
}
