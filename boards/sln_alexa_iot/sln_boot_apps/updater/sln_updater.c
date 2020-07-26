/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "sln_updater.h"

/*! @brief Symbol defined in ld script, it will point to image end address */
extern uint32_t _image_end;

/*! @brief This array will track what module types the bundle contains */
static uint8_t s_bundle_mod_types[UPD_MOD_TYPE_TYPES] = {0};

/*! @brief Used for retaining ca root certificate start address */
static uint8_t *s_ca_root = NULL;

/*! @brief Local copy of FICA table */
static fica_t s_fica_table = {0};

/*! @brief Buffer for storing verification certificates */
static uint8_t s_temp_cert[MAX_CERT_LEN] = {0};

/*! @brief Flags if we're running in encrypted XIP mode */
static bool s_is_enc_xip = false;

/*! @brief Will be used for writing updates in flash chunk by chunk */
static uint8_t s_flash_page[EXT_FLASH_PROGRAM_PAGE] = {0};

status_t upd_bundle_meta_get(bundle_meta_t *bundle_meta)
{
    DEFINE_UPDATER_METHOD_NAME("upd_bundle_meta_get");

    status_t ret = kStatus_Success;

    if (NULL == bundle_meta)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Is this EXIP? */
    if (kStatus_Success == ret)
    {
        s_is_enc_xip = bl_nor_encrypt_has_encrypted_region();
    }

    if (kStatus_Success == ret)
    {
        uint32_t end_addr = (uint32_t)&_image_end;
        /* Fetch bundle metadata from flash */
        ret = SLN_Read_Flash_At_Address(end_addr & 0x0FFFFFFF, (uint8_t *)bundle_meta, sizeof(bundle_meta_t));

        configPRINTF(
            ("[%s]:\r\n"
             "Updater code size:      %d\r\n"
             "Updater bundle size:    %d\r\n"
             "Updater modules number: %d\r\n",
             UPDATER_METHOD_NAME, bundle_meta->upd_code_size, bundle_meta->upd_bundle_size, bundle_meta->upd_mod_num));
    }

    return ret;
}

status_t upd_bundle_meta_validate(bundle_meta_t *bundle_meta)
{
    DEFINE_UPDATER_METHOD_NAME("upd_bundle_meta_validate");

    status_t ret = kStatus_Success;

    configPRINTF(("[%s] Validating bundle metadata ...\r\n", UPDATER_METHOD_NAME));

    if (NULL == bundle_meta)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* verify validity of modules number */
    if (kStatus_Success == ret)
    {
        if ((bundle_meta->upd_mod_num > UPD_MOD_TYPE_TYPES) || (bundle_meta->upd_mod_num == 0))
        {
            configPRINTF(
                ("[%s] Invalid number of modules received: %d\r\n", UPDATER_METHOD_NAME, bundle_meta->upd_mod_num));

            ret = kStatus_InvalidArgument;
        }
    }

    /* verify validity of bundle size */
    if (kStatus_Success == ret)
    {
        /* bundle size must be at least bigger than updater firmware size plus all modules metadata sizes.
         * this is, of course, just a first sanity check; later on it will be verified if the bundle size
         * is big enough to fit all the reported module sizes */
        uint32_t bundle_min_size = bundle_meta->upd_code_size + bundle_meta->upd_mod_num * sizeof(mod_meta_t);

        if (bundle_min_size >= bundle_meta->upd_bundle_size)
        {
            configPRINTF(
                ("[%s] Invalid updater bundle size received, it does not fit all modules\r\n", UPDATER_METHOD_NAME));

            ret = kStatus_InvalidArgument;
        }
    }

    return ret;
}

status_t upd_mod_meta_validate_all(bundle_meta_t *bundle_meta)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_meta_validate_all");

    status_t ret               = kStatus_Success;
    mod_meta_t mod_meta        = {0};
    uint32_t mod_meta_addr     = 0;
    uint32_t mod_meta_end_addr = 0;
    uint32_t bundle_end_addr   = 0;

    if (NULL == bundle_meta)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }
    else
    {
        /* Initialize needed variables here */
        bundle_end_addr = bundle_meta->upd_start_addr + bundle_meta->upd_bundle_size;
        mod_meta_addr   = bundle_meta->upd_start_addr + bundle_meta->upd_code_size + sizeof(bundle_meta_t);
    }

    if (kStatus_Success == ret)
    {
        /* Iterate through each module's metadata, validate each entry */
        for (uint8_t mod_meta_num = 0; mod_meta_num < bundle_meta->upd_mod_num; mod_meta_num++)
        {
            mod_meta_end_addr = mod_meta_addr + sizeof(mod_meta_t);

            configPRINTF(("[%s] Validating metadata #%d ... \r\n", UPDATER_METHOD_NAME, mod_meta_num + 1));

            /* Check if current metadata overflows the bundle size */
            if (mod_meta_end_addr > bundle_end_addr)
            {
                configPRINTF(
                    ("[%s] Metadata #%d overflows bundle end!\r\n"
                     "Metadata end address: %x\r\n"
                     "Updater bundle end address: %x\r\n",
                     UPDATER_METHOD_NAME, mod_meta_num + 1, mod_meta_end_addr, bundle_end_addr));

                ret = kStatus_Fail;
            }

            /* Fetch module metadata from flash */
            if (kStatus_Success == ret)
            {
                ret = SLN_Read_Flash_At_Address(mod_meta_addr & 0x0FFFFFFF, (uint8_t *)&mod_meta, sizeof(mod_meta_t));
            }

            /* Validate module type */
            if (kStatus_Success == ret)
            {
                if ((mod_meta.upd_mod_type < UPD_MOD_TYPE_BOOTSTRAP) || (mod_meta.upd_mod_type >= UPD_MOD_TYPE_TYPES))
                {
                    configPRINTF(("[%s] Invalid module type: %d\r\n", UPDATER_METHOD_NAME, mod_meta.upd_mod_type));

                    ret = kStatus_Fail;
                }
            }

            /* Check that this module type wasn't previously seen in the bundle */
            if (kStatus_Success == ret)
            {
                if (s_bundle_mod_types[mod_meta.upd_mod_type])
                {
                    configPRINTF(("[%s] Metadata type %d encountered more than once in the bundle!\r\n",
                                  UPDATER_METHOD_NAME, mod_meta.upd_mod_type));

                    ret = kStatus_Fail;
                }
                else
                {
                    s_bundle_mod_types[mod_meta.upd_mod_type] = 1;
                }
            }

            /* Validate module length, in terms of bundle fitting */
            if (kStatus_Success == ret)
            {
                uint32_t mod_end_addr = mod_meta_end_addr + mod_meta.upd_mod_length;

                if (mod_end_addr > bundle_end_addr)
                {
                    configPRINTF(
                        ("[%s] Module #%d overflows bundle end!\r\n"
                         "Module end address: %x\r\n"
                         "Updater bundle end address: %x\r\n",
                         UPDATER_METHOD_NAME, mod_meta_num + 1, mod_end_addr, bundle_end_addr));

                    ret = kStatus_Fail;
                }
            }

            /* Validate module length against max module size */
            if (kStatus_Success == ret)
            {
                uint32_t max_mod_size = 0;

                switch (mod_meta.upd_mod_type)
                {
                    case UPD_MOD_TYPE_BOOTSTRAP:
                        max_mod_size = FICA_IMG_BOOTSTRAP_SIZE;
                        break;
                    case UPD_MOD_TYPE_BOOTLOADER:
                        max_mod_size = FICA_IMG_BOOTLOADER_SIZE;
                        break;
                    case UPD_MOD_TYPE_APP_A:
                        max_mod_size = FICA_IMG_APP_A_SIZE;
                        break;
                    case UPD_MOD_TYPE_APP_B:
                        max_mod_size = FICA_IMG_APP_B_SIZE;
                        break;
                    case UPD_MOD_TYPE_CERT_CRED:
                    case UPD_MOD_TYPE_CERT_APP_A:
                    case UPD_MOD_TYPE_CERT_APP_B:
                    case UPD_MOD_TYPE_CERT_CA_ROOT:
                        max_mod_size = MAX_CERT_LEN;
                        break;
                    default:
                        break;
                }

                if (mod_meta.upd_mod_length > max_mod_size)
                {
                    configPRINTF(
                        ("[%s] Module #%d length higher than the expected max size!\r\n"
                         "Module max size: %x\r\n"
                         "Module size: %x\r\n",
                         UPDATER_METHOD_NAME, mod_meta_num + 1, max_mod_size, mod_meta.upd_mod_length));

                    ret = kStatus_Fail;
                }
            }

            /* Remember to use the new CA root certificate for modules validation, if present */
            if (kStatus_Success == ret)
            {
                if (UPD_MOD_TYPE_CERT_CA_ROOT == mod_meta.upd_mod_type)
                {
                    s_ca_root = (uint8_t *)mod_meta_end_addr;
                }
            }

            /* All good with this metadata? Get next metadata address, then */
            if (kStatus_Success == ret)
            {
                configPRINTF(("[%s] Metadata validated for module type %d ... \r\n", UPDATER_METHOD_NAME,
                              mod_meta.upd_mod_type));

                mod_meta_addr += sizeof(mod_meta_t) + mod_meta.upd_mod_length;
            }
            else
            {
                break;
            }
        }
    }

    return ret;
}

status_t upd_mod_dep_check(bundle_meta_t *bundle_meta)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_dep_check");

    status_t ret                     = kStatus_Success;
    upd_mod_type_t updater_bank      = UPD_MOD_TYPE_INVALID;
    upd_mod_type_t updater_bank_cert = UPD_MOD_TYPE_INVALID;

    configPRINTF(("[%s] Checking modules dependencies ...\r\n", UPDATER_METHOD_NAME));

    if (bundle_meta->upd_start_addr == FlexSPI_AMBA_BASE + FICA_IMG_APP_A_ADDR)
    {
        updater_bank      = UPD_MOD_TYPE_APP_A;
        updater_bank_cert = UPD_MOD_TYPE_CERT_APP_A;
    }
    else if (bundle_meta->upd_start_addr == FlexSPI_AMBA_BASE + FICA_IMG_APP_B_ADDR)
    {
        updater_bank      = UPD_MOD_TYPE_APP_B;
        updater_bank_cert = UPD_MOD_TYPE_CERT_APP_B;
    }

    if (UPD_MOD_TYPE_INVALID == updater_bank)
    {
        configPRINTF(("[%s] Invalid start address for updater bundle: %x\r\n", UPDATER_METHOD_NAME,
                      bundle_meta->upd_start_addr));
        ret = kStatus_Fail;
    }

    /* Overwriting the bank in which the updater bundle is written is not allowed */
    if (kStatus_Success == ret)
    {
        if (s_bundle_mod_types[updater_bank] == 1)
        {
            configPRINTF(
                ("[%s] It is not supported to overwrite the updater bundle region!\r\n"
                 "Bundle start address: %x\r\n"
                 "Module type: %d",
                 UPDATER_METHOD_NAME, bundle_meta->upd_start_addr, updater_bank));

            ret = kStatus_Fail;
        }
    }

    /* Updating the certificate corresponding to the bank from which the updater runs doesn't make sense */
    if (kStatus_Success == ret)
    {
        if (s_bundle_mod_types[updater_bank_cert] == 1)
        {
            configPRINTF(
                ("[%s] It is not supported to update the updater bundle certificate!\r\n"
                 "Bundle start address: %x\r\n"
                 "Module type: %d",
                 UPDATER_METHOD_NAME, bundle_meta->upd_start_addr, updater_bank_cert));

            ret = kStatus_Fail;
        }
    }

    /* If new CA ROOT certificate received, check that all dependencies are met */
    if (kStatus_Success == ret)
    {
        /* One of these two must happen (and only one):
         * 1. new certificates for all binaries except the one for the
         *    current running bank were received (cred_cert, app_a / app_b cert)
         * 2. new modules for all binaries which have a certificate associated were received,
         *    except for the current running bank (bootloader, app_a / app_b)  */
        if (s_bundle_mod_types[UPD_MOD_TYPE_CERT_CA_ROOT] == 1)
        {
            upd_mod_type_t other_bank_module_cert =
                (updater_bank == UPD_MOD_TYPE_APP_A) ? UPD_MOD_TYPE_CERT_APP_B : UPD_MOD_TYPE_CERT_APP_A;

            upd_mod_type_t other_bank_module_bin =
                (updater_bank == UPD_MOD_TYPE_APP_A) ? UPD_MOD_TYPE_APP_B : UPD_MOD_TYPE_APP_A;

            bool all_certs_rcvd =
                ((s_bundle_mod_types[UPD_MOD_TYPE_CERT_CRED] || s_bundle_mod_types[UPD_MOD_TYPE_BOOTLOADER]) &&
                 (s_bundle_mod_types[other_bank_module_cert] || s_bundle_mod_types[other_bank_module_bin]));

            if (!all_certs_rcvd)
            {
                configPRINTF(("[%s] CA root certificate dependencies not met\r\n", UPDATER_METHOD_NAME));
                ret = kStatus_Fail;
            }
        }
    }

    /* Verify that there is no module binary (bootloader, main demo) -
     * module certificate (certificate for bootloader, main demo) redundancy.
     * If someone wants to update some firmware, it will have the certificate appended (needed for validation)
     * and that appended certificate will be saved as well, hence a certificate update.
     * If someone wants to update only the certificate, using a certificate module,
     * then why sending the corresponding firmware as well? */
    if (kStatus_Success == ret)
    {
        if ((s_bundle_mod_types[UPD_MOD_TYPE_BOOTLOADER] && s_bundle_mod_types[UPD_MOD_TYPE_CERT_CRED]) ||
            (s_bundle_mod_types[UPD_MOD_TYPE_APP_A] && s_bundle_mod_types[UPD_MOD_TYPE_CERT_APP_A]) ||
            (s_bundle_mod_types[UPD_MOD_TYPE_APP_B] && s_bundle_mod_types[UPD_MOD_TYPE_CERT_APP_B]))
        {
            configPRINTF(("[%s] Firmware - certificate redundancy encountered\r\n", UPDATER_METHOD_NAME));
            ret = kStatus_Fail;
        }
    }

    return ret;
}

/*!
 * @brief   Called from upd_mod_validate_all, after upd_mod_meta_validate_all was performed.
 *          If a root ca cert module was seen in the bundle, then that cert must be used to validate
 *          all the other modules. Otherwise, the root ca cert kept in the file system shalle be used.
 * @returns kStatus_Success in case of success or an error value in case of error
 */
static status_t upd_mod_root_ca_set(void)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_root_ca_set");

    status_t ret                           = kStatus_Success;
    sln_flash_mgmt_status_t flash_mgmt_ret = SLN_FLASH_MGMT_OK;
    uint32_t ca_root_len                   = 0;

    /* Check if a new CA root certificate was discovered during upd_mod_meta_validate_all phase  */
    if (NULL == s_ca_root)
    {
        flash_mgmt_ret = SLN_FLASH_MGMT_Read(ROOT_CA_CERT, NULL, &ca_root_len);

        if (SLN_FLASH_MGMT_OK == flash_mgmt_ret)
        {
            s_ca_root = (uint8_t *)pvPortMalloc(ca_root_len);

            if (NULL == s_ca_root)
            {
                configPRINTF(("[%s] Failed to allocate memory for ROOT CA cert\r\n", UPDATER_METHOD_NAME));
                flash_mgmt_ret = SLN_FLASH_MGMT_ENOMEM;
            }
        }

        if (SLN_FLASH_MGMT_OK == flash_mgmt_ret)
        {
            flash_mgmt_ret = SLN_FLASH_MGMT_Read(ROOT_CA_CERT, s_ca_root, &ca_root_len);
        }

        if (SLN_FLASH_MGMT_OK != flash_mgmt_ret)
        {
            configPRINTF(("[%s] Failed to read ROOT CA cert, error: %d\r\n", UPDATER_METHOD_NAME, flash_mgmt_ret));

            ret = kStatus_Fail;
        }
        else
        {
            configPRINTF(("[%s] Using ROOT CA cert from the file system\r\n", UPDATER_METHOD_NAME));
        }
    }
    else
    {
        /* s_ca_root already set */
        configPRINTF(("[%s] Using ROOT CA cert from the updater bundle\r\n", UPDATER_METHOD_NAME));
    }

    return ret;
}

/*!
 * @brief   Verifies if a valid entry point exists for app modules.
 * @param   mod_meta   Pointer to a mod_meta_t structure
 * @param   mod_addr   Start address of the app module
 * @returns kStatus_Success for valid entry point, kStatus_Fail otherwise
 */
static status_t upd_module_verify_entry_point(mod_meta_t *mod_meta, void *mod_addr)
{
    DEFINE_UPDATER_METHOD_NAME("upd_module_verify_entry_point");

    status_t ret                     = kStatus_Success;
    uint32_t app_vector_table        = 0;
    uint32_t app_entry_point_addr    = 0;
    uint32_t target_app_vector_table = 0;

    /* Check if its a valid image type */
    if (IS_APP_MODULE(mod_meta))
    {
        /* Get flash read address (0x6xxx_xxxx address) */
        switch (mod_meta->upd_mod_type)
        {
            case UPD_MOD_TYPE_BOOTSTRAP:
                app_vector_table = (uint32_t)mod_addr + FICA_IMG_BOOTSTRAP_IVT_ADDR;
                target_app_vector_table =
                    SLN_Flash_Get_Read_Address(FICA_IMG_BOOTSTRAP_ADDR + FICA_IMG_BOOTSTRAP_IVT_ADDR);
                break;
            case UPD_MOD_TYPE_BOOTLOADER:
                app_vector_table        = (uint32_t)mod_addr;
                target_app_vector_table = SLN_Flash_Get_Read_Address(FICA_IMG_BOOTLOADER_ADDR);
                break;
            case UPD_MOD_TYPE_APP_A:
                app_vector_table        = (uint32_t)mod_addr;
                target_app_vector_table = SLN_Flash_Get_Read_Address(FICA_IMG_APP_A_ADDR);
                break;
            case UPD_MOD_TYPE_APP_B:
                app_vector_table        = (uint32_t)mod_addr;
                target_app_vector_table = SLN_Flash_Get_Read_Address(FICA_IMG_APP_B_ADDR);
                break;
            default:
                /* nothing */
                break;
        }

        app_entry_point_addr = (*(uint32_t *)(app_vector_table + 4));

        if ((0xFFFFFFFF == app_entry_point_addr) ||                     /* invalid address */
            (0x00000000 == app_entry_point_addr) ||                     /* invalid address */
            (app_entry_point_addr <= target_app_vector_table) ||        /* must be greater than ivt address */
            (app_entry_point_addr - target_app_vector_table > 0x10000)) /* entry point address should not be too far */
        {
            configPRINTF(
                ("[%s] Invalid image entry point address!\r\n"
                 "Image entry point: %x\r\n"
                 "App vector table:  %x\r\n"
                 "Target app vector table: %x\r\n",
                 UPDATER_METHOD_NAME, app_entry_point_addr, app_vector_table, target_app_vector_table));

            ret = kStatus_Fail;
        }
        else
        {
            configPRINTF(("[%s] Image entry point validated\r\n", UPDATER_METHOD_NAME));
        }
    }

    return ret;
}

/*!
 * @brief   Verifies the validity of a certificate. Works for both
 *          app and certificate modules. For the former case, it verifies
 *          the app padding certificate.
 * @param   mod_meta   Pointer to a mod_meta_t structure
 * @param   mod_addr   Start address of the module
 * @returns kStatus_Success for valid certificate, an error otherwise
 */
static status_t upd_module_verify_cert(mod_meta_t *mod_meta, void *mod_addr)
{
    DEFINE_UPDATER_METHOD_NAME("upd_module_verify_cert");

    status_t ret                   = kStatus_Success;
    sln_auth_status_t sln_auth_ret = SLN_AUTH_OK;
    uint8_t *vf_pem                = NULL;

    if (NULL == mod_meta || NULL == mod_addr)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Certificate validation */
    if (kStatus_Success == ret)
    {
        /* Is this a CA root cert module? Then just validate
         * it's a valid certificate and bail out */
        if (UPD_MOD_TYPE_CERT_CA_ROOT == mod_meta->upd_mod_type)
        {
            sln_auth_ret = SLN_AUTH_Parse_Cert(mod_addr);
            if (SLN_AUTH_OK != sln_auth_ret)
            {
                configPRINTF(
                    ("[%s] Root CA certificate verification failed, error: %d\r\n", UPDATER_METHOD_NAME, sln_auth_ret));
            }
        }
        else
        {
            /* Determine where the signing certificate starts */
            if (IS_APP_MODULE(mod_meta))
            {
                /* If current module is a binary type,
                 * certificate should occupy latest MAX_CERT_LEN bytes */
                vf_pem = (uint8_t *)(mod_addr + mod_meta->upd_mod_length - MAX_CERT_LEN);
            }
            else
            {
                /* If current module is a certificate type */
                vf_pem = mod_addr;
            }

            /* Certificate validation */
            sln_auth_ret = SLN_AUTH_Verify_Cert((uint8_t *)s_ca_root, (uint8_t *)vf_pem);

            if (SLN_AUTH_OK != sln_auth_ret)
            {
                configPRINTF(("[%s] Certificate verification for module type %d failed, error: %d\r\n",
                              UPDATER_METHOD_NAME, mod_meta->upd_mod_type, sln_auth_ret));
            }
        }

        if (SLN_AUTH_OK != sln_auth_ret)
        {
            ret = kStatus_Fail;
        }
    }

    return ret;
}

/*!
 * @brief   Verifies the signature of an application. Works for both
 *          app and certificate modules. For the former case, it verifies
 *          the signature of the app module itself. For the latter case,
 *          the signature is verified against the app corresponding
            to the certificate module.
 * @param   mod_meta   Pointer to a mod_meta_t structure
 * @param   mod_addr   Start address of the module
 * @returns kStatus_Success for valid signature, an error otherwise
 */
static status_t upd_module_verify_signature(mod_meta_t *mod_meta, void *mod_addr)
{
    DEFINE_UPDATER_METHOD_NAME("upd_module_verify_signature");

    status_t ret                   = kStatus_Success;
    sln_auth_status_t sln_auth_ret = SLN_AUTH_OK;
    uint8_t *vf_pem                = NULL;
    uint32_t image_len             = 0;
    uint32_t image_start           = 0;
    fica_img_type_t fica_img_type  = FICA_IMG_TYPE_NONE;

    if (NULL == mod_meta || NULL == mod_addr)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Signature validation */
    if (kStatus_Success == ret)
    {
        /* Is this a CA root cert module? No signature to validate, then */
        if (UPD_MOD_TYPE_CERT_CA_ROOT == mod_meta->upd_mod_type)
        {
            /* Just smile and wave */
        }
        else
        {
            if (IS_APP_MODULE(mod_meta))
            {
                /* If current module is a binary type,
                 * certificate should occupy latest MAX_CERT_LEN bytes */
                vf_pem = (uint8_t *)(mod_addr + mod_meta->upd_mod_length - MAX_CERT_LEN);

                /* Calculate the image length without certificate */
                image_len = mod_meta->upd_mod_length - MAX_CERT_LEN;

                /* Image start is module start */
                image_start = (uint32_t)mod_addr;
            }
            else
            {
                /* If current module is a certificate type */
                vf_pem = mod_addr;

                /* Determine FICA table entry */
                switch (mod_meta->upd_mod_type)
                {
                    case UPD_MOD_TYPE_CERT_CRED:
                        fica_img_type = FICA_IMG_TYPE_BOOTLOADER;
                        break;
                    case UPD_MOD_TYPE_CERT_APP_A:
                        fica_img_type = FICA_IMG_TYPE_APP_A;
                        break;
                    case UPD_MOD_TYPE_CERT_APP_B:
                        fica_img_type = FICA_IMG_TYPE_APP_B;
                        break;
                    default:
                        fica_img_type = FICA_IMG_TYPE_NONE;
                }

                /* Image length and image address must be taken from FICA table,
                 * a padding certificate must also be taken into consideration */
                image_start = SLN_Flash_Get_Read_Address(s_fica_table.records[fica_img_type].imgAddr);
                image_len   = s_fica_table.records[fica_img_type].imgLen - MAX_CERT_LEN;

                if (strncmp((const char *)(image_start + image_len), "-----BEGIN CERTIFICATE-----",
                            strlen("-----BEGIN CERTIFICATE-----")))
                {
                    /* No certificate at end, we can use entire image length (factory application load) */
                    image_len = s_fica_table.records[fica_img_type].imgLen;
                }
            }

            /* Signature validation */
            if (s_is_enc_xip && ((UPD_MOD_TYPE_CERT_APP_A == mod_meta->upd_mod_type) ||
                                 (UPD_MOD_TYPE_CERT_APP_B == mod_meta->upd_mod_type)))
            {
                /* Special handling of APP certificate update for EXIP */
                sln_auth_ret = SLN_AUTH_Verify_Signature_Encrypted(vf_pem, (uint8_t *)(image_start & 0x0FFFFFFF),
                                                                   image_len, mod_meta->modPkiSig);
            }
            else
            {
                sln_auth_ret =
                    SLN_AUTH_Verify_Signature(vf_pem, (uint8_t *)image_start, image_len, mod_meta->modPkiSig);
            }

            if (SLN_AUTH_OK != sln_auth_ret)
            {
                configPRINTF(("[%s] Signature verification for module type %d failed, error: %d\r\n",
                              UPDATER_METHOD_NAME, mod_meta->upd_mod_type, sln_auth_ret));

                ret = kStatus_Fail;
            }
        }
    }

    return ret;
}

/*!
 * @brief   Validates a single module. Called from upd_mod_validate_all.
 *          The function checks the validity of: signing certificate, signature
 *          and app entry point (the last one applies for app modules only).
 * @param   mod_meta   Pointer to a mod_meta_t structure
 * @param   mod_addr   Start address of the module
 * @returns kStatus_Success for valid module, an error otherwise
 */
static status_t upd_mod_validate_single(mod_meta_t *mod_meta, void *mod_addr)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_validate_single");

    status_t ret = kStatus_Success;

    if (NULL == mod_meta || NULL == mod_addr)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Check the certificate */
    if (kStatus_Success == ret)
    {
        ret = upd_module_verify_cert(mod_meta, mod_addr);
    }

    /* Check the signature */
    if (kStatus_Success == ret)
    {
        ret = upd_module_verify_signature(mod_meta, mod_addr);
    }

    /* Check the entry point for APPs */
    if ((kStatus_Success == ret) && IS_APP_MODULE(mod_meta))
    {
        ret = upd_module_verify_entry_point(mod_meta, mod_addr);
    }

    return ret;
}

status_t upd_mod_validate_all(bundle_meta_t *bundle_meta)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_validate_all");

    status_t ret            = kStatus_Success;
    mod_meta_t mod_meta     = {0};
    uint32_t mod_meta_addr  = 0;
    uint32_t mod_start_addr = 0;

    if (NULL == bundle_meta)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Determine what root CA certificate will be used */
    if (kStatus_Success == ret)
    {
        ret = upd_mod_root_ca_set();
    }

    /* Read FICA table */
    if (kStatus_Success == ret)
    {
        ret = SLN_Read_Flash_At_Address(FICA_START_ADDR, (uint8_t *)&s_fica_table, sizeof(fica_t));
    }

    if (kStatus_Success == ret)
    {
        /* Compute address of first module metadata */
        mod_meta_addr = bundle_meta->upd_start_addr + bundle_meta->upd_code_size + sizeof(bundle_meta_t);

        /* Iterate through each module's metadata, validate each entry */
        for (uint8_t mod_meta_num = 0; mod_meta_num < bundle_meta->upd_mod_num; mod_meta_num++)
        {
            mod_start_addr = mod_meta_addr + sizeof(mod_meta_t);

            configPRINTF(("[%s] Validating module #%d ... \r\n", UPDATER_METHOD_NAME, mod_meta_num + 1));

            /* Fetch module metadata from flash */
            ret = SLN_Read_Flash_At_Address(mod_meta_addr & 0x0FFFFFFF, (uint8_t *)&mod_meta, sizeof(mod_meta_t));

            /* Validate the current module */
            if (kStatus_Success == ret)
            {
                ret = upd_mod_validate_single(&mod_meta, (void *)mod_start_addr);
            }

            /* All good with this module? Get next metadata address, then */
            if (kStatus_Success == ret)
            {
                configPRINTF(("[%s] Module type %d validated\r\n", UPDATER_METHOD_NAME, mod_meta.upd_mod_type));

                mod_meta_addr += sizeof(mod_meta_t) + mod_meta.upd_mod_length;
            }
            else
            {
                configPRINTF(
                    ("[%s] Validation failed for module type %d\r\n", UPDATER_METHOD_NAME, mod_meta.upd_mod_type));
                break;
            }
        }
    }

    return ret;
}

/*!
 * @brief   Updates a certificate of upd_mod_type type. Also verifies the
 *          integrity of the certificate after it was written to flash.
 *          If pki_sig is given, the fica table signature will be updated as well.
 * @param   upd_mod_type   Type of certificate
 * @param   mod_addr       Start address of the module
 * @param   pki_sig        Signature associated with current certificate.
 *                         If NULL, only the certificate will be updated
 * @returns kStatus_Success for successful update, an error otherwise
 */
static status_t upd_mod_update_cert(upd_mod_type_t upd_mod_type, void *mod_addr, void *pki_sig)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_update_cert");

    status_t ret                      = kStatus_Success;
    sln_flash_mgmt_status_t flash_ret = SLN_FLASH_MGMT_OK;
    char *cert_name                   = NULL;
    fica_img_type_t fica_type         = FICA_IMG_TYPE_NONE;
    uint32_t cert_len                 = sizeof(s_temp_cert);

    if (NULL == mod_addr)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Get file name and fica type */
    if (kStatus_Success == ret)
    {
        switch (upd_mod_type)
        {
            case UPD_MOD_TYPE_CERT_CRED:
                fica_type = FICA_IMG_TYPE_BOOTLOADER;
                cert_name = CRED_SIGNING_CERT;
                break;
            case UPD_MOD_TYPE_CERT_APP_A:
                fica_type = FICA_IMG_TYPE_APP_A;
                cert_name = APP_A_SIGNING_CERT;
                break;
            case UPD_MOD_TYPE_CERT_APP_B:
                fica_type = FICA_IMG_TYPE_APP_B;
                cert_name = APP_B_SIGNING_CERT;
                break;
            case UPD_MOD_TYPE_CERT_CA_ROOT:
                cert_name = ROOT_CA_CERT;
                break;
            default:
                configPRINTF(("[%s] Invalid certificate type received\r\n", UPDATER_METHOD_NAME));
                ret = kStatus_InvalidArgument;
                break;
        }
    }

    /* Determine if the image signature must be updater in FICA.
     * This does not apply for root CA certificate */
    if ((kStatus_Success == ret) && (NULL != pki_sig) && (UPD_MOD_TYPE_CERT_CA_ROOT != upd_mod_type))
    {
        /* Read FICA here to have latest records */
        ret = SLN_Read_Flash_At_Address(FICA_START_ADDR, (uint8_t *)&s_fica_table, sizeof(fica_t));

        /* Update only the pkiSignature for this entry */
        if (kStatus_Success == ret)
        {
            flash_ret = FICA_write_image_record(fica_type, s_fica_table.records[fica_type].imgAddr,
                                                s_fica_table.records[fica_type].imgLen, pki_sig);
        }
    }

    if (kStatus_Success == ret)
    {
        flash_ret = SLN_FLASH_MGMT_Save(cert_name, mod_addr, safe_strlen((char *)mod_addr, MAX_CERT_LEN) + 1);

        if ((SLN_FLASH_MGMT_EOVERFLOW == flash_ret) || (SLN_FLASH_MGMT_EOVERFLOW2 == flash_ret))
        {
            /* Handle file header overflow */
            flash_ret = SLN_FLASH_MGMT_Erase(cert_name);

            /* Save Certificate again */
            if (SLN_FLASH_NO_ERROR == flash_ret)
            {
                flash_ret = SLN_FLASH_MGMT_Save(cert_name, mod_addr, safe_strlen((char *)mod_addr, MAX_CERT_LEN) + 1);
            }
        }

        if (SLN_FLASH_MGMT_OK != flash_ret)
        {
            configPRINTF(("[%s] Failed to save %s, error: %d\r\n", UPDATER_METHOD_NAME, cert_name, flash_ret));

            ret = kStatus_Fail;
        }
    }

    /* Verify what was written  */

    if (kStatus_Success == ret)
    {
        memset(s_temp_cert, 0, sizeof(s_temp_cert));
        flash_ret = SLN_FLASH_MGMT_Read(cert_name, s_temp_cert, &cert_len);

        if (SLN_FLASH_MGMT_OK != flash_ret)
        {
            configPRINTF(("[%s] Failed to read %s, error: %d\r\n", UPDATER_METHOD_NAME, cert_name, flash_ret));

            ret = kStatus_Fail;
        }
    }

    if ((kStatus_Success == ret) && (SLN_FLASH_MGMT_OK == flash_ret))
    {
        if (memcmp(s_temp_cert, mod_addr, safe_strlen(mod_addr, MAX_CERT_LEN)))
        {
            configPRINTF(("[%s] Verification failed for %s\r\n", UPDATER_METHOD_NAME, cert_name));

            ret = kStatus_Fail;
        }
        else
        {
            configPRINTF(("[%s] Verification passed for %s\r\n", UPDATER_METHOD_NAME, cert_name));
        }
    }

    return ret;
}

/*!
 * @brief   Called during an app module update for encrypted XIP running case.
 *          It creates a new crypto context for the given index. Also starts an md
 *          computation.
 * @param   crypto_idx   Crypto index for which a new context should be created
 * @param   md_ctx       Pointer to a message-digest structure
 * @returns kStatus_Success on success, an error otherwise
 */
static status_t upd_mod_crypto_settings(hab_config_idx_t crypto_idx, mbedtls_md_context_t *md_ctx)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_crypto_settings");

    status_t ret               = kStatus_Success;
    mbedtls_md_type_t md_type  = MBEDTLS_MD_SHA256;
    mbedtls_md_info_t *md_info = NULL;

    if ((NULL == md_ctx) || (kHabMaxConfigCount <= crypto_idx) || (kHabBootloader > crypto_idx))
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Generate new crypto context */
    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Generating a new crypto ctx for index %d\r\n", UPDATER_METHOD_NAME, crypto_idx));
        ret = bl_nor_encrypt_make_new_config(crypto_idx);

        if (kStatus_Success != ret)
        {
            configPRINTF(("[%s] Failed to generate new crypto context, err: %d\r\n", UPDATER_METHOD_NAME, ret));
        }
    }

    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Setting up message digest info structure\r\n", UPDATER_METHOD_NAME));
        md_info = (mbedtls_md_info_t *)mbedtls_md_info_from_type(md_type);

        mbedtls_md_init(md_ctx);
        ret = mbedtls_md_setup(md_ctx, md_info, 1);

        if (kStatus_Success != ret)
        {
            configPRINTF(("[%s] mbedtls_md_setup failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
        }
    }

    /* Start a message-digest computation */
    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Starting message digest computation\r\n", UPDATER_METHOD_NAME));
        ret = mbedtls_md_starts(md_ctx);

        if (kStatus_Success != ret)
        {
            configPRINTF(("[%s] mbedtls_md_starts failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
        }
    }

    return ret;
}

/*!
 * @brief   Backs-up the crypto-context for index crypto_idx. Then writes the newly-generated-
 *          but-kept-so-far-in-RAM context.
 * @param   crypto_idx   Crypto index for which the new context should be applied
 * @returns kStatus_Success on success, an error otherwise
 */
static status_t upd_mod_crypto_ctx_apply(hab_config_idx_t crypto_idx)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_crypto_ctx_apply");

    status_t ret         = kStatus_Success;
    uint32_t crypto_len  = 0;
    uint32_t *crypto_ctx = NULL;

    if ((kHabMaxConfigCount <= crypto_idx) || (kHabBootloader > crypto_idx))
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Get the RAM-kept crypto context */
    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Getting encrypted context block from RAM\r\n", UPDATER_METHOD_NAME));
        bl_nor_encrypt_get_config_block(crypto_idx, &crypto_ctx, &crypto_len);

        if (NULL == crypto_ctx)
        {
            configPRINTF(("[%s] Unable to get encrypted context block!", UPDATER_METHOD_NAME));
            ret = kStatus_Fail;
        }
    }

    /* Back-up old context */
    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Backing up encrypted context block from FLASH\r\n", UPDATER_METHOD_NAME));
        ret = SLN_AUTH_backup_context(crypto_idx);

        if (kStatus_Success != ret)
        {
            configPRINTF(("[%s] Failed to back-up crypto context, err: %d", UPDATER_METHOD_NAME, ret));
        }
    }

    /* Save new crypto context */
    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Saving new encrypted context block in FLASH\r\n", UPDATER_METHOD_NAME));
        ret = SLN_AUTH_save_context(crypto_idx, crypto_ctx);

        if (kStatus_Success != ret)
        {
            configPRINTF(("[%s] Failed to save new crypto context, err: %d", UPDATER_METHOD_NAME, ret));
        }
    }

    return ret;
}

/*!
 * @brief   Re-encrypts sln toolbox when EXIP is used. This must be called after a bootloader update
 *          under EXIP running scenario. This is needed because the bootloader and the toolbox are
 *          placed in the same encryption area. Since a new crypto ctx is generated when a bootloder
 *          update is performed, the toolbox needs to be re-encrypted with this new ctx, otherwise
 *          from ais_demo one shall read gibberish.
 * @returns kStatus_Success on success, an error otherwise
 */
static status_t upd_reencrypt_toolbox(void)
{
    DEFINE_UPDATER_METHOD_NAME("upd_reencrypt_toolbox");

    status_t ret                               = kStatus_Success;
    sln_auth_status_t auth_ret                 = SLN_AUTH_OK;
    uint8_t dummy_page[EXT_FLASH_PROGRAM_PAGE] = {0xFF};

    /* Image handling variables */
    uint32_t start_addr = FICA_IMG_RESERVED_ADDR;
    uint32_t processed  = 0;

    /* SHA256 hash of data */
    mbedtls_md_context_t md_ctx;
    mbedtls_md_type_t md_type  = MBEDTLS_MD_SHA256;
    mbedtls_md_info_t *md_info = (mbedtls_md_info_t *)mbedtls_md_info_from_type(md_type);
    unsigned char *hash        = NULL;

    mbedtls_md_init(&md_ctx);
    mbedtls_md_setup(&md_ctx, md_info, 1);

    /* Allocate hash memory */
    hash = (unsigned char *)pvPortMalloc(md_ctx.md_info->size);

    if (NULL == hash)
    {
        configPRINTF(("[%s] ERROR: Could not allocate memory for hash.\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_Fail;
    }

    /* Start message digest */
    if (kStatus_Success == ret)
    {
        ret = mbedtls_md_starts(&md_ctx);

        if (kStatus_Success != ret)
        {
            configPRINTF(("[%s] mbedtls_md_starts failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
        }
    }

    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Erasing upper half of toolbox bank\r\n", UPDATER_METHOD_NAME));

        /* Delete half of the toolbox bank */
        ret = FICA_Erase_Bank(FICA_IMG_RESERVED_ADDR + FICA_IMG_RESERVED_SIZE / 2, FICA_IMG_RESERVED_SIZE / 2);
    }

    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Starting toolbox re-encryption into temporary destination\r\n", UPDATER_METHOD_NAME));

        /* Assumption made that toolbox is smaller than half of MB for this approach to work.
         * Otherwise, this code will need to be re-written. The condition is satisfied atm */
        while (processed + EXT_FLASH_PROGRAM_PAGE < FICA_IMG_RESERVED_SIZE / 2)
        {
            memset(s_flash_page, 0, sizeof(s_flash_page));

            /* Fetch chunk from flash */
            ret =
                SLN_Read_Flash_At_Address((start_addr + processed) & 0x0FFFFFFF, s_flash_page, EXT_FLASH_PROGRAM_PAGE);

            /* Have we reached tool-box end? */
            if (!memcmp(s_flash_page, dummy_page, sizeof(dummy_page)))
            {
                configPRINTF(("[%s] Toolbox end encountered, size %x\r\n", UPDATER_METHOD_NAME, processed));
                break;
            }

            /* Encrypt current chunk, using the newly generated bootloader crypto context */
            if (kStatus_Success == ret)
            {
                ret = bl_nor_encrypt_data(FlexSPI_AMBA_BASE + start_addr + processed, EXT_FLASH_PROGRAM_PAGE,
                                          (uint32_t *)s_flash_page);

                if (kStatus_Success != ret)
                {
                    configPRINTF(("[%s] bl_nor_encrypt_data failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
                }
            }

            /* Update hash */
            if (kStatus_Success == ret)
            {
                ret = mbedtls_md_update(&md_ctx, (const unsigned char *)s_flash_page, EXT_FLASH_PROGRAM_PAGE);

                if (kStatus_Success != ret)
                {
                    configPRINTF(("[%s] mbedtls_md_update failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
                }
            }

            /* Write page in flash, in the upper half of the toolbox bank */
            if (kStatus_Success == ret)
            {
                ret = SLN_Write_Flash_At_Address(start_addr + processed + FICA_IMG_RESERVED_SIZE / 2,
                                                 (uint8_t *)s_flash_page);

                if (kStatus_Success != ret)
                {
                    configPRINTF(("[%s] Writing flash page failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
                }
            }

            if (kStatus_Success == ret)
            {
                processed += EXT_FLASH_PROGRAM_PAGE;
            }
            else
            {
                break;
            }
        }

        if (processed + EXT_FLASH_PROGRAM_PAGE > FICA_IMG_RESERVED_SIZE / 2)
        {
            configPRINTF(("[%s] Something went wrong, tool-box end not encountered\r\n", UPDATER_METHOD_NAME));
            ret = kStatus_Fail;
        }
    }

    /* Finish hash */
    if (kStatus_Success == ret)
    {
        ret = mbedtls_md_finish(&md_ctx, hash);

        if (kStatus_Success != ret)
        {
            configPRINTF(("[%s] mbedtls_md_finish failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
        }
    }

    /* Verify what was written in the upper half of the bank */
    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Verifying re-encrypted toolbox, temporary written into upper toolbox bank\r\n",
                      UPDATER_METHOD_NAME));

        auth_ret = SLN_AUTH_Verify_Hash((uint8_t *)start_addr + FICA_IMG_RESERVED_SIZE / 2, processed, (uint8_t *)hash);

        if (SLN_AUTH_OK != auth_ret)
        {
            configPRINTF(("[%s] Verification of the temporary re-encrypted tool-box failed, err: %d\r\n",
                          UPDATER_METHOD_NAME, auth_ret));
            ret = kStatus_Fail;
        }
        else
        {
            configPRINTF(("[%s] Verification of the temporary re-encrypted tool-box passed\r\n", UPDATER_METHOD_NAME));
        }
    }

    /* Delete the old toolbox */
    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Erasing lower half of toolbox bank\r\n", UPDATER_METHOD_NAME));

        /* Delete half of the toolbox bank */
        ret = FICA_Erase_Bank(FICA_IMG_RESERVED_ADDR, FICA_IMG_RESERVED_SIZE / 2);

        if (kStatus_Success != ret)
        {
            configPRINTF(("[%s] FICA_Erase_Bank failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
        }
    }

    /* Now write the re-encrypted toolbox at the right address */
    if (kStatus_Success == ret)
    {
        uint32_t to_write = processed;
        processed         = 0;

        while (to_write)
        {
            memset(s_flash_page, 0, sizeof(s_flash_page));

            /* Fetch chunk from flash */
            ret = SLN_Read_Flash_At_Address((start_addr + processed + FICA_IMG_RESERVED_SIZE / 2) & 0x0FFFFFFF,
                                            s_flash_page, EXT_FLASH_PROGRAM_PAGE);

            /* Encrypt current chunk, using the in-place context, to cancel BEE's own decryption */
            if (kStatus_Success == ret)
            {
                ret = bl_nor_encrypt_data_prev_ctx(
                    FlexSPI_AMBA_BASE + start_addr + processed + FICA_IMG_RESERVED_SIZE / 2, EXT_FLASH_PROGRAM_PAGE,
                    (uint32_t *)s_flash_page);

                if (kStatus_Success != ret)
                {
                    configPRINTF(("[%s] bl_nor_encrypt_data_prev_ctx failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
                }
            }

            /* Write page in flash, in the first half of the toolbox bank */
            if (kStatus_Success == ret)
            {
                ret = SLN_Write_Flash_At_Address(start_addr + processed, (uint8_t *)s_flash_page);

                if (kStatus_Success != ret)
                {
                    configPRINTF(("[%s] Writing flash page failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
                }
            }

            if (kStatus_Success == ret)
            {
                processed += EXT_FLASH_PROGRAM_PAGE;
                to_write -= EXT_FLASH_PROGRAM_PAGE;
            }
            else
            {
                break;
            }
        }
    }

    /* Verify what was written in the lower half of the bank */
    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Verifying re-encrypted toolbox\r\n", UPDATER_METHOD_NAME));

        auth_ret = SLN_AUTH_Verify_Hash((uint8_t *)start_addr, processed, (uint8_t *)hash);

        if (SLN_AUTH_OK != auth_ret)
        {
            configPRINTF(
                ("[%s] Verification of the re-encrypted tool-box failed, err: %d\r\n", UPDATER_METHOD_NAME, auth_ret));
            ret = kStatus_Fail;
        }
        else
        {
            configPRINTF(("[%s] Verification of the re-encrypted tool-box passed\r\n", UPDATER_METHOD_NAME));
        }
    }

    /* Cleanup */
    if (NULL != hash)
    {
        for (uint32_t idx = 0; idx < md_ctx.md_info->size; idx++)
        {
            hash[idx] = 0x00;
        }
    }

    vPortFree(hash);
    hash = NULL;

    mbedtls_md_free(&md_ctx);

    return ret;
}

/*!
 * @brief   Updates an app described by mod_meta structure. Also verifies the
 *          integrity of the app after it was written to flash.
 *          When encrypted XIP is used, it generates a new crypto context as well.
 * @param   mod_meta   Pointer to a mod_meta_t structure
 * @param   mod_addr   Start address of the module
 * @returns kStatus_Success for successful update, an error otherwise
 */
static status_t upd_mod_update_app(mod_meta_t *mod_meta, void *mod_addr)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_update_app");

    status_t ret                         = kStatus_Success;
    sln_auth_status_t auth_ret           = SLN_AUTH_OK;
    sln_flash_mgmt_status_t flash_status = 0;

    fica_img_type_t fica_type    = FICA_IMG_TYPE_NONE;
    upd_mod_type_t app_cert_type = UPD_MOD_TYPE_INVALID;
    hab_config_idx_t crypto_idx  = 0;
    bool do_encrypt              = false;
    uint8_t *vf_pem              = NULL;

    /* Needed for obtaining SHA256 hash of data */
    mbedtls_md_context_t md_ctx;
    unsigned char *hash = NULL;

    /* Image writing variables */
    uint32_t bank_size     = 0;
    uint32_t start_addr    = 0;
    uint32_t to_write      = 0;
    uint32_t written       = 0;
    uint32_t current_chunk = 0;

    /* Needed when updating bootstrap when EXIP is activated */
    uint8_t boot_header[0x1000] = {0};

    if (NULL == mod_meta || NULL == mod_addr)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Specific APP variables setting phase */
    /*--------------------------------------*/
    if (kStatus_Success == ret)
    {
        vf_pem   = (uint8_t *)(mod_addr + mod_meta->upd_mod_length - MAX_CERT_LEN);
        to_write = mod_meta->upd_mod_length;

        /* Determine if we run EXIP, set crypto index to change, init other variables
         * (assume above protections give us valid data) */
        switch (mod_meta->upd_mod_type)
        {
            case UPD_MOD_TYPE_BOOTSTRAP:
                /* Nothing to encrypt for bootstrap */
                do_encrypt = false;
                start_addr = FICA_IMG_BOOTSTRAP_ADDR;
                bank_size  = FICA_IMG_BOOTSTRAP_SIZE;
                break;
            case UPD_MOD_TYPE_BOOTLOADER:
                crypto_idx    = kHabBootloader;
                do_encrypt    = s_is_enc_xip ? true : false;
                start_addr    = FICA_IMG_BOOTLOADER_ADDR;
                bank_size     = FICA_IMG_BOOTLOADER_SIZE;
                fica_type     = FICA_IMG_TYPE_BOOTLOADER;
                app_cert_type = UPD_MOD_TYPE_CERT_CRED;
                break;
            case UPD_MOD_TYPE_APP_A:
                crypto_idx    = kHabApplication;
                do_encrypt    = s_is_enc_xip ? true : false;
                start_addr    = FICA_IMG_APP_A_ADDR;
                bank_size     = FICA_IMG_APP_A_SIZE;
                fica_type     = FICA_IMG_TYPE_APP_A;
                app_cert_type = UPD_MOD_TYPE_CERT_APP_A;
                break;
            case UPD_MOD_TYPE_APP_B:
                crypto_idx    = kHabApplication;
                do_encrypt    = s_is_enc_xip ? true : false;
                start_addr    = FICA_IMG_APP_B_ADDR;
                bank_size     = FICA_IMG_APP_B_SIZE;
                fica_type     = FICA_IMG_TYPE_APP_B;
                app_cert_type = UPD_MOD_TYPE_CERT_APP_B;
                break;
            default:
                configPRINTF(
                    ("[%s] Invalid module type received: %d\r\n", UPDATER_METHOD_NAME, mod_meta->upd_mod_type));
                ret = kStatus_InvalidArgument;
        }
    }

    /* Initialize crypto settings */
    if ((kStatus_Success == ret) && do_encrypt)
    {
        ret = upd_mod_crypto_settings(crypto_idx, &md_ctx);
    }

    /* Destination bank erase phase */
    /*------------------------------*/

    /* In case of bootstrap update when running EXIP:
     * Backing up first 0x1000 bytes from flash is a must.
     * That area contains the flash configuration settings
     * (which are not a part of the received bootstrap image!)
     * and the crypto contexts. Losing any of these would brick the device! */
    if ((kStatus_Success == ret) && s_is_enc_xip && ((UPD_MOD_TYPE_BOOTSTRAP == mod_meta->upd_mod_type)))
    {
        ret = SLN_Read_Flash_At_Address(0x0, boot_header, 0x1000);
    }

    if (kStatus_Success == ret)
    {
        configPRINTF(
            ("[%s] Erasing app bank, start_addr: %x, bank_size: %x\r\n", UPDATER_METHOD_NAME, start_addr, bank_size));
        ret = FICA_Erase_Bank(start_addr, bank_size);

        if (kStatus_Success != ret)
        {
            configPRINTF(
                ("[%s] Failed to erase bank, err: %d\r\n"
                 "Start address: %x"
                 "Bank size: %x",
                 UPDATER_METHOD_NAME, ret, start_addr, bank_size));
        }
    }

    /* Restoring first 0x1000 bytes in case of
     * bootstrap update when running EXIP: */
    if ((kStatus_Success == ret) && s_is_enc_xip && (UPD_MOD_TYPE_BOOTSTRAP == mod_meta->upd_mod_type))
    {
        uint32_t boot_header_written  = 0;
        uint32_t boot_header_to_write = 0x1000;

        while (boot_header_to_write)
        {
            /* Write page in flash */
            if (kStatus_Success == ret)
            {
                ret = SLN_Write_Flash_At_Address(start_addr + boot_header_written,
                                                 (uint8_t *)(boot_header + boot_header_written));

                if (kStatus_Success != ret)
                {
                    configPRINTF(
                        ("[%s] Restoring boot header failed at page writing, err: %d\r\n", UPDATER_METHOD_NAME, ret));
                }
            }

            if (kStatus_Success == ret)
            {
                boot_header_written += EXT_FLASH_PROGRAM_PAGE;
                boot_header_to_write -= EXT_FLASH_PROGRAM_PAGE;
            }
            else
            {
                break;
            }
        }

        if (kStatus_Success == ret)
        {
            /* Update start_addr */
            start_addr = 0x1000;
        }
    }

    /* Actual image writing phase */
    /*----------------------------*/

    if (kStatus_Success == ret)
    {
        configPRINTF(
            ("[%s] Starting writing image in the destination bank, img size: %x\r\n", UPDATER_METHOD_NAME, to_write));

        while (to_write)
        {
            memset(s_flash_page, 0, sizeof(s_flash_page));
            current_chunk = to_write > EXT_FLASH_PROGRAM_PAGE ? EXT_FLASH_PROGRAM_PAGE : to_write;

            /* Fetch chunk from flash */
            ret = SLN_Read_Flash_At_Address(((uint32_t)mod_addr + written) & 0x0FFFFFFF, s_flash_page, current_chunk);

            /* Encrypt current chunk */
            if ((kStatus_Success == ret) && do_encrypt)
            {
                ret = bl_nor_encrypt_data(FlexSPI_AMBA_BASE + start_addr + written, current_chunk,
                                          (uint32_t *)s_flash_page);

                if (kStatus_Success != ret)
                {
                    configPRINTF(("[%s] bl_nor_encrypt_data failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
                }
            }

            /* Update hash */
            if ((kStatus_Success == ret) && do_encrypt)
            {
                ret = mbedtls_md_update(&md_ctx, (const unsigned char *)s_flash_page, EXT_FLASH_PROGRAM_PAGE);

                if (kStatus_Success != ret)
                {
                    configPRINTF(("[%s] mbedtls_md_update failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
                }
            }

            /* Write page in flash */
            if (kStatus_Success == ret)
            {
                ret = SLN_Write_Flash_At_Address(start_addr + written, (uint8_t *)s_flash_page);

                if (kStatus_Success != ret)
                {
                    configPRINTF(("[%s] Writing flash page failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
                }
            }

            if (kStatus_Success == ret)
            {
                written += current_chunk;
                to_write -= current_chunk;
            }
            else
            {
                break;
            }
        }
    }

    /* Finish hash computation for EXIP scenario */
    /*-------------------------------------------*/
    if ((kStatus_Success == ret) && do_encrypt)
    {
        /* Allocate hash memory */
        hash = (unsigned char *)pvPortMalloc(md_ctx.md_info->size);

        if (NULL == hash)
        {
            configPRINTF(("[%s] Memory allocation failed\r\n", UPDATER_METHOD_NAME));
            ret = kStatus_Fail;
        }
    }

    /* Finish hash */
    if ((kStatus_Success == ret) && do_encrypt)
    {
        configPRINTF(("[%s] Finishing message digest computation\r\n", UPDATER_METHOD_NAME));
        ret = mbedtls_md_finish(&md_ctx, hash);

        if (kStatus_Success != ret)
        {
            configPRINTF(("[%s] mbedtls_md_finish failed, err: %d\r\n", UPDATER_METHOD_NAME, ret));
        }

        mbedtls_md_free(&md_ctx);
    }

    /* Written image verification phase */
    /*----------------------------------*/
    if (kStatus_Success == ret)
    {
        if (do_encrypt)
        {
            configPRINTF(("[%s] Verifying encrypted image hash\r\n", UPDATER_METHOD_NAME));
            auth_ret =
                SLN_AUTH_Verify_Hash((uint8_t *)start_addr,
                                     mod_meta->upd_mod_length +
                                         (EXT_FLASH_PROGRAM_PAGE - (mod_meta->upd_mod_length % EXT_FLASH_PROGRAM_PAGE)),
                                     (uint8_t *)hash);
            vPortFree(hash);
        }
        else
        {
            configPRINTF(("[%s] Verifying written image signature\r\n", UPDATER_METHOD_NAME));
            auth_ret = SLN_AUTH_Verify_Signature(vf_pem, (uint8_t *)SLN_Flash_Get_Read_Address(start_addr),
                                                 mod_meta->upd_mod_length - MAX_CERT_LEN, mod_meta->modPkiSig);
        }

        if (SLN_AUTH_OK != auth_ret)
        {
            configPRINTF(("[%s] Verification of the written image failed, err: %d\r\n", UPDATER_METHOD_NAME, auth_ret));
            ret = kStatus_Fail;
        }
        else
        {
            configPRINTF(("[%s] Verification of the written image passed\r\n", UPDATER_METHOD_NAME));
        }
    }

    /* Save new crypto context */
    /*-------------------------*/
    if ((kStatus_Success == ret) && do_encrypt)
    {
        configPRINTF(("[%s] Applying the new crypto context\r\n", UPDATER_METHOD_NAME));
        ret = upd_mod_crypto_ctx_apply(crypto_idx);
    }

    /* Signature, certificate, FICA entry update phase, unless it's a bootstrap.
     * bootstrap does not have a FICA entry and it is validated by HAB. */
    /*------------------------------------------------------------------*/
    if ((kStatus_Success == ret) && (mod_meta->upd_mod_type != UPD_MOD_TYPE_BOOTSTRAP))
    {
        configPRINTF(("[%s] Writing FICA entry for the new image\r\n", UPDATER_METHOD_NAME));
        flash_status = FICA_write_image_record(fica_type, start_addr, mod_meta->upd_mod_length, mod_meta->modPkiSig);

        if (SLN_FLASH_MGMT_OK != flash_status)
        {
            configPRINTF(("[%s] Failed to save signature, err: %d\r\n", UPDATER_METHOD_NAME, flash_status));
            ret = kStatus_Fail;
        }
    }

    if ((kStatus_Success == ret) && (mod_meta->upd_mod_type != UPD_MOD_TYPE_BOOTSTRAP))
    {
        configPRINTF(
            ("[%s] Image written and verified, saving the appended certificate as well\r\n", UPDATER_METHOD_NAME));
        ret = upd_mod_update_cert(app_cert_type, vf_pem, NULL);
    }

    /* Re-encrypt intelligence tool-box if bootloader was just updated under EXIP */
    /*----------------------------------------------------------------------------*/
    if ((kStatus_Success == ret) && (mod_meta->upd_mod_type == UPD_MOD_TYPE_BOOTLOADER) && do_encrypt)
    {
        configPRINTF(("[%s] Starting sln tool-box re-encryption\r\n", UPDATER_METHOD_NAME));
        ret = upd_reencrypt_toolbox();
    }

    return ret;
}

/*!
 * @brief   Updates a single module, calling the designated cert / app update functions.
 * @param   mod_meta   Pointer to a mod_meta_t structure
 * @param   mod_addr   Start address of the module
 * @returns kStatus_Success for successful update, an error otherwise
 */
static status_t upd_mod_update_single(mod_meta_t *mod_meta, void *mod_addr)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_update_single");

    status_t ret = kStatus_Success;

    if (NULL == mod_meta || NULL == mod_addr)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    /* Module update */
    if (kStatus_Success == ret)
    {
        if (IS_APP_MODULE(mod_meta))
        {
            ret = upd_mod_update_app(mod_meta, mod_addr);
        }
        else
        {
            ret = upd_mod_update_cert(mod_meta->upd_mod_type, mod_addr, mod_meta->modPkiSig);
        }
    }

    return ret;
}

status_t upd_mod_update_all(bundle_meta_t *bundle_meta)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_update_all");

    status_t ret                         = kStatus_Success;
    mod_meta_t mod_meta                  = {0};
    uint32_t mod_meta_addr               = 0;
    uint32_t mod_start_addr              = 0;
    sln_flash_mgmt_status_t flash_status = 0;
    fica_img_type_t updater_bank         = FICA_IMG_TYPE_NONE;

    if (NULL == bundle_meta)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    if ((kStatus_Success == ret) && (s_bundle_mod_types[UPD_MOD_TYPE_BOOTLOADER]))
    {
        /* If bootloader module in the bundle, update FICA boot entry to point
         * to this updater app. In this way, if power goes down during
         * the update, we won't be affected by a bricked bootloader */

        if (bundle_meta->upd_start_addr == FlexSPI_AMBA_BASE + FICA_IMG_APP_A_ADDR)
        {
            updater_bank = FICA_IMG_TYPE_APP_A;
        }
        else if (bundle_meta->upd_start_addr == FlexSPI_AMBA_BASE + FICA_IMG_APP_B_ADDR)
        {
            updater_bank = FICA_IMG_TYPE_APP_B;
        }

        flash_status = FICA_SetCurBootType(updater_bank);

        if (SLN_FLASH_MGMT_OK != flash_status)
        {
            configPRINTF(("[%s] Failed to set current boot type, err: %d\r\n", UPDATER_METHOD_NAME, flash_status));
            ret = kStatus_Fail;
        }
    }

    if (kStatus_Success == ret)
    {
        /* If bootstrap / root CA module in the bundle, perform IVT update.
         * In this way, if power goes down during
         * the update, we won't be affected by a bricked bootstrap */
        /* TODO */
    }

    if (kStatus_Success == ret)
    {
        /* Compute address of first module metadata */
        mod_meta_addr = bundle_meta->upd_start_addr + bundle_meta->upd_code_size + sizeof(bundle_meta_t);

        /* Iterate through each module, perform update */
        for (uint8_t mod_meta_num = 0; mod_meta_num < bundle_meta->upd_mod_num; mod_meta_num++)
        {
            mod_start_addr = mod_meta_addr + sizeof(mod_meta_t);

            configPRINTF(("[%s] Updating module #%d ... \r\n", UPDATER_METHOD_NAME, mod_meta_num + 1));

            /* Fetch module metadata from flash */
            if (kStatus_Success == ret)
            {
                ret = SLN_Read_Flash_At_Address(mod_meta_addr & 0x0FFFFFFF, (uint8_t *)&mod_meta, sizeof(mod_meta_t));
            }

            /* Write the current module */
            if (kStatus_Success == ret)
            {
                ret = upd_mod_update_single(&mod_meta, (void *)mod_start_addr);
            }

            /* All good with this module? Get next metadata address */
            if (kStatus_Success == ret)
            {
                configPRINTF(("[%s] Module type %d updated\r\n", UPDATER_METHOD_NAME, mod_meta.upd_mod_type));

                mod_meta_addr += sizeof(mod_meta_t) + mod_meta.upd_mod_length;
            }
            else
            {
                configPRINTF(("[%s] Failed to update module type %d\r\n", UPDATER_METHOD_NAME, mod_meta.upd_mod_type));
                break;
            }
        }
    }

    /* Restore boot type */
    if ((kStatus_Success == ret) && (s_bundle_mod_types[UPD_MOD_TYPE_BOOTLOADER]))
    {
        flash_status = FICA_SetCurBootType(FICA_IMG_TYPE_BOOTLOADER);

        if (SLN_FLASH_MGMT_OK != flash_status)
        {
            configPRINTF(("[%s] Failed to restore current boot type, err: %d\r\n", UPDATER_METHOD_NAME, flash_status));
            ret = kStatus_Fail;
        }
    }

    return ret;
}

status_t upd_mod_restore_main_app_settings(bundle_meta_t *bundle_meta, status_t update_status)
{
    DEFINE_UPDATER_METHOD_NAME("upd_mod_restore_main_app_settings");
    fica_img_type_t other_bank           = FICA_IMG_TYPE_NONE;
    sln_flash_mgmt_status_t flash_status = 0;
    status_t ret                         = kStatus_Success;

    if (NULL == bundle_meta)
    {
        configPRINTF(("[%s] Invalid argument received\r\n", UPDATER_METHOD_NAME));
        ret = kStatus_InvalidArgument;
    }

    if (kStatus_Success == ret)
    {
        if (bundle_meta->upd_start_addr == FlexSPI_AMBA_BASE + FICA_IMG_APP_A_ADDR)
        {
            other_bank = FICA_IMG_TYPE_APP_B;
        }
        else if (bundle_meta->upd_start_addr == FlexSPI_AMBA_BASE + FICA_IMG_APP_B_ADDR)
        {
            other_bank = FICA_IMG_TYPE_APP_A;
        }
        else
        {
            configPRINTF(("[%s] Invalid updater start address received: %x\r\n", UPDATER_METHOD_NAME,
                          bundle_meta->upd_start_addr));
            ret = kStatus_InvalidArgument;
        }
    }

    if ((kStatus_Success == ret) && s_is_enc_xip)
    {
        /* If all update operations succeeded, restore the crypto context only when
         * main app was not one of the updated modules; when that happens,
         * a new crypto context was generated and saved before reaching here;
         * restoring the running bank is all that's needed for running that app.
         *
         * If main app was not updated or if the update failed, crypto context needs
         * to be restored in order to run the app from the other bank */
        if (((kStatus_Success == update_status) && !s_bundle_mod_types[UPD_MOD_TYPE_APP_A] &&
             !s_bundle_mod_types[UPD_MOD_TYPE_APP_B]) ||
            (kStatus_Fail == update_status))
        {
            configPRINTF(("[%s] Restoring main app crypto context\r\n", UPDATER_METHOD_NAME));

            if (SLN_AUTH_restore_context(kHabApplication))
            {
                configPRINTF(("[%s] Failed to restore main app crypto context\r\n", UPDATER_METHOD_NAME));
                ret = kStatus_Fail;
            }
        }
    }

    if (kStatus_Success == ret)
    {
        configPRINTF(("[%s] Restoring main app running bank\r\n", UPDATER_METHOD_NAME));

        /* Restore the previous running bank, no matter what the update_status is */
        flash_status = FICA_SetCurAppStartType(other_bank);

        if (SLN_FLASH_MGMT_OK != flash_status)
        {
            configPRINTF(("[%s] Failed to restore main app bank, err: %d\r\n", UPDATER_METHOD_NAME, flash_status));
            ret = kStatus_Fail;
        }
    }

    return ret;
}
