/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_SSCP_CONFIG_SNT_H_
#define _FSL_SSCP_CONFIG_SNT_H_

#include "sab.h"

#define SSCP_COMMAND_ENUM_ALT
typedef enum _sscp_command
{
    kSSCP_CMD_SSS_OpenSession                  = SAB_OPEN_SESSION_REQ,
    kSSCP_CMD_SSS_CloseSession                 = SAB_CLOSE_SESSION_REQ,
    kSSCP_CMD_SSS_Ping                         = SAB_PING_REQ,
    kSSCP_CMD_SSS_ContextFree                  = SAB_CONTEXT_FREE_REQ,
    kSSCP_CMD_SSS_SymmetricContextInit         = SAB_SYMMETRIC_CONTEXT_INIT_REQ,
    kSSCP_CMD_SSS_SymmetricCipherOneGo         = SAB_CIPHER_ONE_GO_REQ,
    kSSCP_CMD_SSS_SymmetricCipherInit          = SAB_CIPHER_INIT_REQ,
    kSSCP_CMD_SSS_SymmetricCipherUpdate        = SAB_CIPHER_UPDATE_REQ,
    kSSCP_CMD_SSS_SymmetricCipherFinish        = SAB_CIPHER_FINISH_REQ,
    kSSCP_CMD_SSS_SymmetricCryptCtr            = SAB_CIPHER_CRYPT_CTR_REQ,
    kSSCP_CMD_SSS_AeadContextInit              = SAB_AEAD_CONTEXT_INIT_REQ,
    kSSCP_CMD_SSS_AeadOneGo                    = SAB_AEAD_ONE_GO_REQ,
    kSSCP_CMD_SSS_AeadInit                     = SAB_AEAD_INIT_REQ,
    kSSCP_CMD_SSS_AeadUpdateAead               = SAB_AEAD_UPDATE_AAD_REQ,
    kSSCP_CMD_SSS_AeadUpdate                   = SAB_AEAD_UPDATE_REQ,
    kSSCP_CMD_SSS_AeadFinish                   = SAB_AEAD_FINISH_REQ,
    kSSCP_CMD_SSS_DigestContextInit            = SAB_DIGEST_CONTEXT_INIT_REQ,
    kSSCP_CMD_SSS_DigestOneGo                  = SAB_DIGEST_ONE_GO_REQ,
    kSSCP_CMD_SSS_DigestInit                   = SAB_DIGEST_INIT_REQ,
    kSSCP_CMD_SSS_DigestUpdate                 = SAB_DIGEST_UPDATE_REQ,
    kSSCP_CMD_SSS_DigestFinish                 = SAB_DIGEST_FINISH_REQ,
    kSSCP_CMD_SSS_MacContextInit               = SAB_MAC_CONTEXT_INIT_REQ,
    kSSCP_CMD_SSS_MacOneGo                     = SAB_MAC_ONE_GO_REQ,
    kSSCP_CMD_SSS_MacInit                      = SAB_MAC_INIT_REQ,
    kSSCP_CMD_SSS_MacUpdate                    = SAB_MAC_UPDATE_REQ,
    kSSCP_CMD_SSS_MacFinish                    = SAB_MAC_FINISH_REQ,
    kSSCP_CMD_SSS_AsymetricContextInit         = SAB_ASYMMETRIC_CONTEXT_INIT_REQ,
    kSSCP_CMD_SSS_AsymmetricEncrypt            = SAB_ASYMMETRIC_ENCRYPT_REQ,
    kSSCP_CMD_SSS_AsymmetricDecrypt            = SAB_ASYMMETRIC_DECRYPT_REQ,
    kSSCP_CMD_SSS_AsymmetricSignDigest         = SAB_ASYMMETRIC_SIGN_DIGEST_REQ,
    kSSCP_CMD_SSS_AsymmetricVerifyDigest       = SAB_ASYMMETRIC_VERIFY_DIGEST_REQ,
    kSSCP_CMD_SSS_TunnelContextInit            = SAB_TUNNEL_CONTEXT_INIT_REQ,
    kSSCP_CMD_SSS_Tunnel                       = SAB_TUNNEL_REQ,
    kSSCP_CMD_SSS_DeriveKeyContextInit         = SAB_DERIVE_KEY_CONTEXT_INIT_REQ,
    kSSCP_CMD_SSS_DeriveKey                    = SAB_DERIVE_KEY_REQ,
    kSSCP_CMD_SSS_AsymmetricDeriveKey          = SAB_ASYMMETRIC_DH_DERIVE_KEY_REQ,
    kSSCP_CMD_SSS_KeyObjectContextInit         = SAB_KEY_OBJECT_INIT_REQ,
    kSSCP_CMD_SSS_KeyObjectAllocateHandle      = SAB_KEY_OBJECT_ALLOCATE_HANDLE_REQ,
    kSSCP_CMD_SSS_KeyObjectGetHandle           = SAB_KEY_OBJECT_GET_HANDLE_REQ,
    kSSCP_CMD_SSS_KeyObjectContextFree         = SAB_KEY_OBJECT_FREE_REQ,
    kSSCP_CMD_SSS_KeyStoreContextInit          = SAB_KEY_STORE_CONTEXT_INIT_REQ,
    kSSCP_CMD_SSS_KeyStoreAllocate             = SAB_KEY_STORE_ALLOCATE_REQ,
    kSSCP_CMD_SSS_KeyStoreSave                 = SAB_KEY_STORE_SAVE_REQ,
    kSSCP_CMD_SSS_KeyStoreLoad                 = SAB_KEY_STORE_LOAD_REQ,
    kSSCP_CMD_SSS_KeyStoreSetKey               = SAB_KEY_STORE_SET_KEY_REQ,
    kSSCP_CMD_SSS_KeyStoreGenerateKey          = SAB_KEY_STORE_GENERATE_KEY_REQ,
    kSSCP_CMD_SSS_KeyStoreGetKey               = SAB_KEY_STORE_GET_KEY_REQ,
    kSSCP_CMD_SSS_KeyStoreOpenKey              = SAB_KEY_STORE_OPEN_KEY_REQ,
    kSSCP_CMD_SSS_KeyStoreFreezeKey            = SAB_KEY_STORE_FREEZE_KEY_REQ,
    kSSCP_CMD_SSS_KeyStoreEraseKey             = SAB_KEY_STORE_ERASE_KEY_REQ,
    kSSCP_CMD_SSS_KeyStoreEraseAll             = SAB_MGMT_CLEAR_ALL_KEYS_REQ,
    KSSCP_CMD_SSS_RngGet                       = SAB_MGMT_GET_RANDOM_REQ,
    kSSCP_CMD_SSS_MGMT_ContextInit             = SAB_MGMT_CONTEXT_INIT_REQ,
    kSSCP_CMD_SSS_MGMT_FuseRead                = SAB_MGMT_FUSE_READ_REQ,
    kSSCP_CMD_SSS_MGMT_FuseShadowRegisterRead  = SAB_MGMT_FUSE_SHADOW_READ_REQ,
    kSSCP_CMD_SSS_MGMT_FuseProgram             = SAB_MGMT_FUSE_PROGRAM_REQ,
    kSSCP_CMD_SSS_MGMT_PropertyGet             = SAB_MGMT_GET_PROPERTY_REQ,
    kSSCP_CMD_SSS_MGMT_LifeCycleGet            = SAB_MGMT_GET_LIFECYCLE_REQ,
    kSSCP_CMD_SSS_MGMT_PropertySet             = SAB_MGMT_SET_PROPERTY_REQ,
    kSSCP_CMD_SSS_MGMT_AdvanceLifecycle        = SAB_MGMT_ADVANCE_LIFECYCLE_REQ,
    kSSCP_CMD_SSS_MGMT_SecretImport            = SAB_MGMT_IMPORT_SECRET_REQ,
    kSSCP_CMD_SSS_MGMT_SecretExport            = SAB_MGMT_EXPORT_SECRET_REQ,
    kSSCP_CMD_SSS_MGMT_Attest                  = SAB_MGMT_ATTEST_REQ,
    kSSCP_CMD_SSS_MGMT_SecretBlobLoad          = SAB_MGMT_BLOB_LOAD_SECRET_REQ,
    kSSCP_CMD_SSS_MGMT_SecretBlobExport        = SAB_MGMT_BLOB_EXPORT_SECRET_REQ,
    kSSCP_CMD_SSS_MGMT_SecretBlobImport        = SAB_MGMT_BLOB_IMPORT_SECRET_REQ,
    kSSCP_CMD_SSS_MGMT_SoftwareVersionGet      = SAB_MGMT_GET_SOFTWARE_VERSION_REQ,
    kSSCP_CMD_SSS_MGMT_ReturnFaSet             = SAB_MGMT_SET_RETURN_FA_REQ,
    kSSCP_CMD_SSS_MGMT_HostAccessPermissionSet = SAB_MGMT_SET_HOST_ACCESS_PERMISSION_REQ,
    kSSCP_CMD_SSS_MGMT_IntegrityCheckEnable    = SAB_MGMT_INTEGRITY_CHECK_ENABLE_REQ,
} sscp_command_t;

#endif /* _FSL_SSCP_CONFIG_SNT_H_ */
