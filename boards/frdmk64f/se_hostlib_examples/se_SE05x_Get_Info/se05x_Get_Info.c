/* Copyright 2019,2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxLog_App.h>
#include <se05x_APDU.h>
#include <se05x_const.h>
#include <se05x_ecc_curves.h>
#include <se05x_ecc_curves_values.h>
#include <se05x_tlv.h>
#include <string.h>

#include "ex_sss_auth.h"
#include "global_platf.h"
#include "smCom.h"

static ex_sss_boot_ctx_t gex_sss_get_info_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_get_info_ctx)
#define EX_SSS_BOOT_DO_ERASE 0
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0

#include <ex_sss_main_inc.h>

static sss_status_t JCOP4_GetDataIdentify(void *conn_ctx);
static sss_status_t JCOP4_GetCPLCData(void *conn_ctx);
static sss_status_t Applet_Identify(sss_se05x_session_t *pSession);

sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx)
{
    sss_status_t status           = kStatus_SSS_Fail;
    sss_se05x_session_t *pSession = (sss_se05x_session_t *)&pCtx->session;

    status = Applet_Identify(pSession);
    if (status != kStatus_SSS_Success)
        goto cleanup;

    status = JCOP4_GetDataIdentify(pSession->s_ctx.conn_ctx);
    if (status != kStatus_SSS_Success)
        goto cleanup;

    status = JCOP4_GetCPLCData(pSession->s_ctx.conn_ctx);
    if (status != kStatus_SSS_Success)
        goto cleanup;

    status = kStatus_SSS_Success;
cleanup:
    return status;
}

#define CHECK_FEATURE_PRESENT(AppletConfig, ITEM)                                            \
    if (((kSE05x_AppletConfig_##ITEM) == ((AppletConfig) & (kSE05x_AppletConfig_##ITEM)))) { \
        LOG_I("With    " #ITEM);                                                             \
    }                                                                                        \
    else {                                                                                   \
        LOG_I("WithOut " #ITEM);                                                             \
    }

static sss_status_t Applet_Identify(sss_se05x_session_t *pSession)
{
    sss_status_t status = kStatus_SSS_Fail;
    smStatus_t sw_status;
    SE05x_Result_t result = kSE05x_Result_NA;
    uint8_t uid[SE050_MODULE_UNIQUE_ID_LEN];
    size_t uidLen = sizeof(uid);
    uint8_t applet_version[7];
    size_t applet_versionLen = sizeof(applet_version);

    sw_status = Se05x_API_CheckObjectExists(&pSession->s_ctx, kSE05x_AppletResID_UNIQUE_ID, &result);
    if (SM_OK != sw_status) {
        LOG_E("Failed Se05x_API_CheckObjectExists");
        goto cleanup;
    }

    sw_status = Se05x_API_ReadObject(&pSession->s_ctx, kSE05x_AppletResID_UNIQUE_ID, 0, (uint16_t)uidLen, uid, &uidLen);
    if (SM_OK != sw_status) {
        LOG_E("Failed Se05x_API_CheckObjectExists");
        goto cleanup;
    }
    LOG_W("#####################################################");
    LOG_AU8_I(uid, uidLen);

    // VersionInfo is a 7 - byte value consisting of :
    // - 1 - byte Major applet version
    // - 1 - byte Minor applet version
    // - 1 - byte patch applet version
    // - 2 - byte AppletConfig, indicating the supported applet features
    // - 2-byte Secure Box version: major version (MSB) concatenated with minor version (LSB).

    sw_status = Se05x_API_GetVersion(&pSession->s_ctx, applet_version, &applet_versionLen);
    if (SM_OK != sw_status) {
        LOG_E("Failed Se05x_API_GetVersion");
        goto cleanup;
    }
    LOG_W("#####################################################");
    LOG_I("Applet Major = %d", applet_version[0]);
    LOG_I("Applet Minor = %d", applet_version[1]);
    LOG_I("Applet patch = %d", applet_version[2]);
    LOG_I("AppletConfig = %02X%02X", applet_version[3], applet_version[4]);
    {
        U16 AppletConfig = applet_version[3] << 8 | applet_version[4];
        CHECK_FEATURE_PRESENT(AppletConfig, ECDAA);
        CHECK_FEATURE_PRESENT(AppletConfig, ECDSA_ECDH_ECDHE);
        CHECK_FEATURE_PRESENT(AppletConfig, EDDSA);
        CHECK_FEATURE_PRESENT(AppletConfig, DH_MONT);
        CHECK_FEATURE_PRESENT(AppletConfig, HMAC);
        CHECK_FEATURE_PRESENT(AppletConfig, RSA_PLAIN);
        CHECK_FEATURE_PRESENT(AppletConfig, RSA_CRT);
        CHECK_FEATURE_PRESENT(AppletConfig, AES);
        CHECK_FEATURE_PRESENT(AppletConfig, DES);
        CHECK_FEATURE_PRESENT(AppletConfig, PBKDF);
        CHECK_FEATURE_PRESENT(AppletConfig, TLS);
        CHECK_FEATURE_PRESENT(AppletConfig, MIFARE);
        CHECK_FEATURE_PRESENT(AppletConfig, I2CM);
    }
    LOG_I("Internal = %02X%02X", applet_version[5], applet_version[6]);

    status = kStatus_SSS_Success;
cleanup:
    return status;
}

/* See Secion 5.1.1.1 GET DATA IDENTIFY in JCOP 4 SE050 User Manual
 *
 * @warn This must be the last call.  After this call, ISD is selected
 *       and hence Applet Commands won't work.
 */
static sss_status_t JCOP4_GetDataIdentify(void *conn_ctx)
{
    sss_status_t status = kStatus_SSS_Fail;
    smStatus_t rxStatus;
    char jcop_platform_id[17] = {0};
    /* Must be packed */
    typedef struct
    {
        //0xFE Tag value - proprietary data Only present if class byte is 0x80
        uint8_t vTag_value_proprietary_data;
        //0x49 / 0x45 Length of following data Only present if class byte is 0x80
        uint8_t vLength_of_following_data;
        //0xDF28 Tag card identification data Only present if class byte is 0x80
        uint8_t vTag_card_identification_data[0x02];
        //0x46 Length of card identification data Only present if class byte is 0x80
        uint8_t vLength_of_card_identification_data;
        //0x01 Tag configuration ID Identifies the configuration content
        uint8_t vTag_configuration_ID;
        uint8_t vLength_configuration_ID; //0x0C Length configuration ID
        uint8_t vConfiguration_ID[0x0C];  //var Configuration ID
        uint8_t vTag_patch_ID;            //0x02 Tag patch ID Identifies the patch level
        uint8_t vLength_patch_ID;         //0x08 Length patch ID
        uint8_t vPatch_ID[0x08];          //var Patch ID
        //0x03 Tag platform build ID1 Identifies the JCOP platform
        uint8_t vTag_platform_build_ID1;
        uint8_t vLength_platform_build_ID; //0x18 Length platform build ID
        uint8_t vPlatform_build_ID[0x18];  //var Platform build ID
        uint8_t vTag_FIPS_mode;            //0x052 Tag FIPS mode FIPS mode active
        uint8_t vLength_FIPS_mode;         //0x01 Length FIPS mode
            //var FIPS mode 0x00 - FIPS mode not active, 0x01 - FIPS mode active
        uint8_t vFIPS_mode;

        //uint8_t vTag_modules_enabled; //0x06 Tag modules enabled Lists enabled and disabled modules
        //uint8_t vLength_modules_enabled; //0x02 Length modules enabled Big endian format
        //uint8_t vBit_mask_of_enabled_modules[0x02]; //var Bit mask of enabled modules See Table 5.3

        //0x07 Tag pre-perso state Lists pre-perso state
        uint8_t vTag_pre_perso_state;

        //0x01 Length pre-perso state
        uint8_t vLength_pre_perso_state;

        //var Bit mask of pre-perso state bit0 = 1 = config module available,
        //  bit1 = 1 = transport state is active.
        //  Unused bits are set to 0x0.
        uint8_t vBit_mask_of_pre_perso_state;
        uint8_t vTag_ROM_ID;            //'08' Tag ROM ID Indentifies the ROM content
        uint8_t vLength_ROM_ID;         //'08' Length ROM ID Normal ending
        uint8_t vROM_ID[0x08];          //var ROM ID
        uint8_t vStatus_Word_SW_[0x02]; //9000h Status Word (SW)
    } identifyRsp_t;

    const uint8_t cmd[] = {
        0x80, // CLA '80' / '00' GlobalPlatform / ISO / IEC
        0xCA, // INS 'CA' GET DATA(IDENTIFY)
        0x00, // P1 '00' High order tag value
        0xFE, // P2 'FE' Low order tag value - proprietary data
        0x02, // Lc '02' Length of data field
        0xDF,
        0x28, // Data 'DF28' Card identification data
        0x00  // Le '00' Length of response data
    };

    identifyRsp_t identifyRsp = {0};
    U32 prspLen               = sizeof(identifyRsp_t);

    U16 dummyResponse16 = sizeof(identifyRsp_t);
    /* Select card manager / ISD
     * (ReUsing same dummy buffers) */
    rxStatus = GP_Select(conn_ctx, (uint8_t *)&identifyRsp /* dummy */, 0, (uint8_t *)&identifyRsp, &dummyResponse16);
    if (rxStatus != SM_OK) {
        LOG_E("Could not select ISD.");
        goto cleanup;
    }

    rxStatus = smCom_TransceiveRaw(conn_ctx, (uint8_t *)cmd, sizeof(cmd), (uint8_t *)&identifyRsp, &prspLen);
    if (rxStatus == SM_OK && prspLen == sizeof(identifyRsp)) {
        LOG_W("#####################################################");
        LOG_I("%s = 0x%02X", "Tag value - proprietary data 0xFE", identifyRsp.vTag_value_proprietary_data);
        LOG_I("%s = 0x%02X", "Length of following data 0x45", identifyRsp.vLength_of_following_data);
        LOG_MAU8_I("Tag card identification data",
            identifyRsp.vTag_card_identification_data,
            sizeof(identifyRsp.vTag_card_identification_data));
        LOG_I("%s = 0x%02X",
            "Length of card identification data", // 0x46
            identifyRsp.vLength_of_card_identification_data);
        LOG_I("%s = 0x%02X", "Tag configuration ID (Must be 0x01)", identifyRsp.vTag_configuration_ID);
        LOG_D("%s = 0x%02X", "Length configuration ID 0x0C", identifyRsp.vLength_configuration_ID);
        LOG_MAU8_I("Configuration ID", identifyRsp.vConfiguration_ID, sizeof(identifyRsp.vConfiguration_ID));

        //Third and fourth Byte of vConfiguration_ID is the OEF ID
        LOG_MAU8_I("OEF ID", &identifyRsp.vConfiguration_ID[2], 2);
        LOG_I("%s = 0x%02X", "Tag patch ID (Must be 0x02)", identifyRsp.vTag_patch_ID);
        LOG_D("%s = 0x%02X", "Length patch ID 0x08", identifyRsp.vLength_patch_ID);
        LOG_MAU8_I("Patch ID", identifyRsp.vPatch_ID, sizeof(identifyRsp.vPatch_ID));
        LOG_I("%s = 0x%02X", "Tag platform build ID1 (Must be 0x03)", identifyRsp.vTag_platform_build_ID1);
        LOG_D("%s = 0x%02X", "Length platform build ID 0x18", identifyRsp.vLength_platform_build_ID);
        LOG_MAU8_I("Platform build ID", identifyRsp.vPlatform_build_ID, sizeof(identifyRsp.vPlatform_build_ID));
        memcpy(jcop_platform_id, identifyRsp.vPlatform_build_ID, 16);

        LOG_I("%s = %s", "JCOP Platform ID", jcop_platform_id);
        LOG_I("%s = 0x%02X", "Tag FIPS mode (Must be 0x05)", identifyRsp.vTag_FIPS_mode);
        LOG_D("%s = 0x%02X", "Length FIPS mode 0x01", identifyRsp.vLength_FIPS_mode);
        LOG_I("%s = 0x%02X", "FIPS mode var", identifyRsp.vFIPS_mode);
        //LOG_I("%s = 0x%02X", "Tag modules enabled 0x06", identifyRsp.vTag_modules_enabled);
        //LOG_I("%s = 0x%02X", "Length modules enabled 0x02", identifyRsp.vLength_modules_enabled);
        //LOG_MAU8_I("Bit mask of enabled modules", identifyRsp.vBit_mask_of_enabled_modules, sizeof(identifyRsp.vBit_mask_of_enabled_modules));
        LOG_I("%s = 0x%02X", "Tag pre-perso state (Must be 0x07)", identifyRsp.vTag_pre_perso_state);
        LOG_D("%s = 0x%02X", "Length pre-perso state 0x01", identifyRsp.vLength_pre_perso_state);
        LOG_I("%s = 0x%02X", "Bit mask of pre-perso state var", identifyRsp.vBit_mask_of_pre_perso_state);

        LOG_I("%s = 0x%02X", "Tag ROM ID (Must be 0x08)", identifyRsp.vTag_ROM_ID);
        LOG_D("%s = 0x%02X", "Length ROM ID 0x08", identifyRsp.vLength_ROM_ID);
        LOG_MAU8_I("ROM ID", identifyRsp.vROM_ID, sizeof(identifyRsp.vROM_ID));
        LOG_MAU8_I("Status Word (SW)", identifyRsp.vStatus_Word_SW_, sizeof(identifyRsp.vStatus_Word_SW_));
    }
    else {
        LOG_E("Error in retreiving the JCOP Identifier. Response is as follows");
        LOG_AU8_E((uint8_t *)&identifyRsp, sizeof(identifyRsp));
        goto cleanup;
    }
    status = kStatus_SSS_Success;

cleanup:
    return status;
}

/* See 5.1.1.2 Get CPLC data
 *   This command returns the Card Production Life Cycle (CPLC) data. The APDU can be sent without prior authentication.
 *
 *
 * The Card Production Life Cycle data as defined in VISA GlobalPlatform Card Specification 2.1.1 are coded in 42
 * bytes. The CPLC format and the default values are shown below
 *
 */

static sss_status_t JCOP4_GetCPLCData(void *conn_ctx)
{
    sss_status_t status = kStatus_SSS_Fail;
    smStatus_t rxStatus;
    /* Must be packed */
    typedef struct
    {
        uint8_t p9F7F[2];
        uint8_t pLen[1];                            // = 2A
        uint8_t IC_fabricator[2];                   // 2 '4790' NXP
        uint8_t IC_type1[2];                        // 2 'D321' NXP
        uint8_t Operating_system_identifier[2];     //  2 '47' SS NXP
        uint8_t Operating_system_release_date[2];   //  2 SS SS NXP
        uint8_t Operating_system_release_level[2];  //  2 <Mask ID> <Patch ID> NXP
        uint8_t IC_fabrication_date[2];             //  tt NXP
        uint8_t IC_Serial_number[4];                //  4 nnnb NXP
        uint8_t IC_Batch_identifier[2];             //  2 bb NXP
        uint8_t IC_module_fabricator[2];            //  2 '00' '00' Customer
        uint8_t IC_module_packaging_date[2];        //  2 '00' '00' Customer
        uint8_t ICC_manufacturer[2];                // 2 '00' '00' Customer
        uint8_t IC_embedding_date[2];               //  2 '00' '00' Customer
        uint8_t IC_OS_initializer[2];               //  2 WX Customer
        uint8_t IC_OS_initialization_date[2];       // 2 YN Customer
        uint8_t IC_OS_initialization_equipment[4];  // ID 4 NNNN Customer
        uint8_t IC_personalizer[2];                 // 2 '00' '00' Customer
        uint8_t IC_personalization_date[2];         //  2 '00' '00' Customer
        uint8_t IC_personalization_equipment_ID[4]; //  4 '00'. . . '00' Customer
        uint8_t SW[2];
    } cplcRsp_t;

    const uint8_t cmd[] = {
        0x80, // CLA '80' / '00' GlobalPlatform / ISO / IEC
        0xCA, // INS
        0x9F, // P1
        0x7F, // P2
        0x00, // Lc
    };

    cplcRsp_t cplc_data = {{0}};
    U32 cplc_data_len   = sizeof(cplcRsp_t);

    SSS_ASSERT(sizeof(cplcRsp_t) == 47);

    rxStatus = smCom_TransceiveRaw(conn_ctx, (uint8_t *)cmd, sizeof(cmd), (uint8_t *)&cplc_data, &cplc_data_len);
    if (rxStatus == SM_OK && cplc_data_len == sizeof(cplc_data)) {
        LOG_W("#####################################################");
        LOG_AU8_I(cplc_data.IC_fabricator, sizeof(cplc_data.IC_fabricator));
        LOG_AU8_I(cplc_data.IC_type1, sizeof(cplc_data.IC_type1));
        LOG_AU8_I(cplc_data.Operating_system_identifier, sizeof(cplc_data.Operating_system_identifier));
        LOG_AU8_I(cplc_data.Operating_system_release_date, sizeof(cplc_data.Operating_system_release_date));
        LOG_AU8_I(cplc_data.Operating_system_release_level, sizeof(cplc_data.Operating_system_release_level));
        LOG_AU8_I(cplc_data.IC_fabrication_date, sizeof(cplc_data.IC_fabrication_date));
        LOG_AU8_I(cplc_data.IC_Serial_number, sizeof(cplc_data.IC_Serial_number));
        LOG_AU8_I(cplc_data.IC_Batch_identifier, sizeof(cplc_data.IC_Batch_identifier));
        LOG_AU8_I(cplc_data.IC_module_fabricator, sizeof(cplc_data.IC_module_fabricator));
        LOG_AU8_I(cplc_data.IC_module_packaging_date, sizeof(cplc_data.IC_module_packaging_date));
        LOG_AU8_I(cplc_data.ICC_manufacturer, sizeof(cplc_data.ICC_manufacturer));
        LOG_AU8_I(cplc_data.IC_embedding_date, sizeof(cplc_data.IC_embedding_date));
        LOG_AU8_I(cplc_data.IC_OS_initializer, sizeof(cplc_data.IC_OS_initializer));
        LOG_AU8_I(cplc_data.IC_OS_initialization_date, sizeof(cplc_data.IC_OS_initialization_date));
        LOG_AU8_I(cplc_data.IC_OS_initialization_equipment, sizeof(cplc_data.IC_OS_initialization_equipment));
        LOG_AU8_I(cplc_data.IC_personalizer, sizeof(cplc_data.IC_personalizer));
        LOG_AU8_I(cplc_data.IC_personalization_date, sizeof(cplc_data.IC_personalization_date));
        LOG_AU8_I(cplc_data.IC_personalization_equipment_ID, sizeof(cplc_data.IC_personalization_equipment_ID));
        LOG_AU8_I(cplc_data.SW, sizeof(cplc_data.SW));
    }
    else {
        LOG_E("Error in retreiving the CPLC Data. Response is as follows");
        LOG_AU8_E((uint8_t *)&cplc_data, sizeof(cplc_data));
        goto cleanup;
    }
    status = kStatus_SSS_Success;

cleanup:
    return status;
}
