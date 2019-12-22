/*
 * Copyright 2018,2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_SSCP_COMMANDS_H_
#define _FSL_SSCP_COMMANDS_H_

/*!
@defgroup sscp_cmd SSCP commands
@details This section describes the SSCP commands and parameters.
@ingroup sscp
*/

/*!
 * @addtogroup sscp_cmd
 * @{
 */

/*! @brief SSCP common commands */
typedef enum _sscp_command
{
    kSSCP_CMD_SSS_OpenSession,
    kSSCP_CMD_SSS_CloseSession,
    kSSCP_CMD_SSS_SymmetricCipherOneGo,
    kSSCP_CMD_SSS_SymmetricCipherInit,
    kSSCP_CMD_SSS_SymmetricCipherUpdate,
    kSSCP_CMD_SSS_SymmetricCipherFinish,
    kSSCP_CMD_SSS_SymmetricCryptCtr,
    kSSCP_CMD_SSS_AeadOneGo,
    kSSCP_CMD_SSS_AeadOneInit,
    kSSCP_CMD_SSS_AeadOneUpdateAad,
    kSSCP_CMD_SSS_AeadOneUpdate,
    kSSCP_CMD_SSS_AeadOneFinish,
    kSSCP_CMD_SSS_DigestOneGo,
    kSSCP_CMD_SSS_DigestInit,
    kSSCP_CMD_SSS_DigestUpdate,
    kSSCP_CMD_SSS_DigestFinish,
    kSSCP_CMD_SSS_MacOneGo,
    kSSCP_CMD_SSS_MacInit,
    kSSCP_CMD_SSS_MacUpdate,
    kSSCP_CMD_SSS_MacFinish,
    kSSCP_CMD_SSS_AsymmetricEncrypt,
    kSSCP_CMD_SSS_AsymmetricDecrypt,
    kSSCP_CMD_SSS_AsymmetricSignDigest,
    kSSCP_CMD_SSS_AsymmetricVerifyDigest,
    kSSCP_CMD_SSS_Tunnel,
    kSSCP_CMD_SSS_DeriveKey,
    kSSCP_CMD_SSS_AsymmetricDeriveKey,
    kSSCP_CMD_SSS_KeyObjectAllocateHandle,
    kSSCP_CMD_SSS_KeyObjectGetHandle,
    kSSCP_CMD_SSS_KeyStoreAllocate,
    kSSCP_CMD_SSS_KeyStoreSave,
    kSSCP_CMD_SSS_KeyStoreLoad,
    kSSCP_CMD_SSS_KeyStoreSetKey,
    kSSCP_CMD_SSS_KeyStoreGenerateKey,
    kSSCP_CMD_SSS_KeyStoreGetKey,
    kSSCP_CMD_SSS_KeyStoreOpenKey,
    kSSCP_CMD_SSS_KeyStoreFreezeKey,
    kSSCP_CMD_SSS_KeyStoreEraseKey,
    kSSCP_CMD_SSS_KeyStoreEraseAll,

    kSSCP_CMD_NONE,
    kSSCP_CMD_GENERATE_KEY,
    kSSCP_CMD_ERASE_KEY,
    kSSCP_CMD_GET_KEY,
    kSSCP_CMD_FREEZE_KEY,
    kSSCP_CMD_ALLOCATE_KEYSTORE,
    kSSCP_CMD_SAVE_KEYSTORE,
    kSSCP_CMD_LOAD_KEYSTORE,
    kSSCP_CMD_SET_KEY,
    kSSCP_KEYOBJ_CMD_ALLOCATE_HANDLE,
    kSSCP_KEYOBJ_CMD_GET_HANDLE,
    kSSCP_KEYOBJ_CMD_SET_USER,
    kSSCP_KEYOBJ_CMD_SET_PURPOSE,
    kSSCP_KEYOBJ_CMD_SET_ACCESS,
    kSSCP_KEYOBJ_CMD_GET_USER,
    kSSCP_KEYOBJ_CMD_GET_PURPOSE,
    kSSCP_KEYOBJ_CMD_GET_ACCESS,
    kSSCP_ASYMMETRIC_CTX_INIT,
    kSSCP_ASYMMETRIC_CMD_ENCRYPT,
    kSSCP_ASYMMETRIC_CMD_DECRYPT,
    kSSCP_ASYMMETRIC_CMD_SIGN_DIGEST,
    kSSCP_ASYMMETRIC_CMD_VERIFY_DIGEST,
    kSSCP_SYMM_CIPHER_CTX_INIT,
    kSSCP_SYMM_CIPHER_ONE_GO,
    kSSCP_SYMM_CIPHER_INIT,
    kSSCP_SYMM_CIPHER_UPDATE,
    kSSCP_SYMM_CIPHER_FINISH,
    kSSCP_SYMM_CIPHER_CRYPT_CTR,
    kSSCP_DERIVE_CTX_INIT,
    kSSCP_DERIVE_KEY,
    kSSCP_ASYMM_DH_DERIVE_KEY,
    kSSCP_GEN_RANDOM_NUM,
    kSSCP_CMD_LAST
} sscp_command_t;

enum _sscp_context_type
{
    kSSCP_ParamContextType_SSS_Symmetric = 1u,
    kSSCP_ParamContextType_SSS_Aead,
    kSSCP_ParamContextType_SSS_Digest,
    kSSCP_ParamContextType_SSS_Mac,
    kSSCP_ParamContextType_SSS_Asymmetric,
    kSSCP_ParamContextType_SSS_Tunnel,
    kSSCP_ParamContextType_SSS_DeriveKey,
    kSSCP_ParamContextType_SSS_Object,
    kSSCP_ParamContextType_SSS_KeyStore,
    kSSCP_ParamContextType_SSS_RandomGen,
};

/*!
 *@}
 */ /* end of sscp */

#endif /* _FSL_SSCP_COMMANDS_H_ */
