/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef SLN_UPDATER_H_
#define SLN_UPDATER_H_

#include "FreeRTOS.h"

#include "fsl_common.h"
#include "fica_definition.h"
#include "sln_flash.h"
#include "sln_auth.h"
#include "sln_flash_mgmt.h"
#include "flash_ica_driver.h"
#include "nor_encrypt_bee.h"
#include "mbedtls/md.h"
#include "mbedtls/md_internal.h"

#define DEFINE_UPDATER_METHOD_NAME(name) static const char UPDATER_METHOD_NAME[] = name;
#define IS_APP_MODULE(mod_meta)                                                                               \
    (mod_meta->upd_mod_type == UPD_MOD_TYPE_BOOTSTRAP || mod_meta->upd_mod_type == UPD_MOD_TYPE_BOOTLOADER || \
     mod_meta->upd_mod_type == UPD_MOD_TYPE_APP_A || mod_meta->upd_mod_type == UPD_MOD_TYPE_APP_B)

/*
 * Updater bundle sections:
 * |--upd-code--|--upd-bundle-meta--|--upd-mod1-meta--|--upd-mod1--| ...
 * |--upd-modN-meta--|--upd-modN--|--upd-mod-size--|--upd-bundle-certificate--|
 *
 * Module metadata and module itself sections, signed binary case:
 * |--binary-type--|--binary-size--|--binary-signature--|--binary--|--signing-certificate--|
 *
 * Module metadata and module itself sections, certificate case:
 * |--cert-type--|--cert-size--|--binary-signature--|--certificate--|
 *
 * NOTE: the binary signature above is for the binary corresponding to the certificate.
 * That binary is not part of the bundle when a certificate update is tried.
 * Certificate modules are supposed to be used when someone wants to update the certificates only and
 * knows exactly the firmware version that runs on the board. In this way, only the certificates and
 * the new modPkiSig signatures of the corresponding binaries can be bundled in an updater package.
 */

/*! @brief Updater module types enumeration */
typedef enum _upd_mod_type
{
    UPD_MOD_TYPE_INVALID = -1, /*!< Invalid type */
    UPD_MOD_TYPE_BOOTSTRAP,    /*!< Bootstrap module */
    UPD_MOD_TYPE_BOOTLOADER,   /*!< Bootloader module */
    UPD_MOD_TYPE_APP_A,        /*!< Application A module */
    UPD_MOD_TYPE_APP_B,        /*!< Application B module */
    UPD_MOD_TYPE_CERT_CRED,    /*!< Bootloader certificate module */
    UPD_MOD_TYPE_CERT_APP_A,   /*!< App A certificate module */
    UPD_MOD_TYPE_CERT_APP_B,   /*!< App B certificate module */
    UPD_MOD_TYPE_CERT_CA_ROOT, /*!< CA Root certificate module */
    UPD_MOD_TYPE_TYPES         /*!< Total number of modules defined */
} upd_mod_type_t;

/*! @brief Updater module metadata structure */
typedef struct _module_metadata
{
    upd_mod_type_t upd_mod_type; /*!< Module type */
    uint32_t upd_mod_length;     /*!< Module length */
    uint8_t modPkiSig[256];      /*!< Module signature */
} mod_meta_t;

/*! @brief Updater bundle metadata structure */
typedef struct _bundle_metadata
{
    uint32_t upd_start_addr;  /*!< Start address of the updater firmware  */
    uint32_t upd_code_size;   /*!< Size of the updater firmware  */
    uint32_t upd_bundle_size; /*!< Size of the updater bundle */
    uint32_t upd_mod_num;     /*!< Number of modules encapsulated into the bundle */
} bundle_meta_t;

/*!
 * @brief   Reads the updater bundle metadata
 * @param   bundle_meta   Pointer to a pre-allocated bundle_meta_t structure
 * @returns kStatus_Success in case of success or an error value in case of error
 */
status_t upd_bundle_meta_get(bundle_meta_t *bundle_meta);

/*!
 * @brief   Validates the updater bundle metadata
 * @param   bundle_meta   Pointer to a bundle_meta_t structure
 * @returns kStatus_Success in case of success or an error value in case of error
 */
status_t upd_bundle_meta_validate(bundle_meta_t *bundle_meta);

/*!
 * @brief   Validates all modules metadata
 * @param   bundle_meta   Pointer to a bundle_meta_t structure
 * @returns kStatus_Success in case of success or an error value in case of error
 */
status_t upd_mod_meta_validate_all(bundle_meta_t *bundle_meta);

/*!
 * @brief   Checks if inter-module dependencies are met
 * @param   bundle_meta   Pointer to a bundle_meta_t structure
 * @returns kStatus_Success in case of success or an error value in case of error
 */
status_t upd_mod_dep_check(bundle_meta_t *bundle_meta);

/*!
 * @brief   Validates all modules (certificates, signatures)
 * @param   bundle_meta   Pointer to a bundle_meta_t structure
 * @returns kStatus_Success in case of success or an error value in case of error
 */
status_t upd_mod_validate_all(bundle_meta_t *bundle_meta);

/*!
 * @brief   Updates all modules, verifying the integrity afterwards.
 *          Also takes care of new crypto context generation if encrypted XIP is used.
 * @param   bundle_meta   Pointer to a bundle_meta_t structure
 * @returns kStatus_Success in case of success or an error value in case of error
 */
status_t upd_mod_update_all(bundle_meta_t *bundle_meta);

/*!
 * @brief   Applies needed settings from running from the other bank after the next reset
 * @param   bundle_meta   Pointer to a bundle_meta_t structure
 * @param   update_status Tells if the update succeeded or not
 * @returns kStatus_Success in case of success or an error value in case of error
 */
status_t upd_mod_restore_main_app_settings(bundle_meta_t *bundle_meta, status_t update_status);

#endif /* SLN_UPDATER_H_ */
