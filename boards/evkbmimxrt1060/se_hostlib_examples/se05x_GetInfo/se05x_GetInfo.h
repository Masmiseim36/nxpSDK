/* Copyright 2023 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SE05X_GETINFO_H__
#define __SE05X_GETINFO_H__

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
    uint8_t vTag_ROM_ID;             //'08' Tag ROM ID Indentifies the ROM content
    uint8_t vLength_ROM_ID;          //'08' Length ROM ID Normal ending
    uint8_t vROM_ID[0x08];           //var ROM ID
    uint8_t vTag_JCOP_OS_Core;       // '0A' Tag for JCOP OS Core ID
    uint8_t vLength_JCOP_OS_Core_ID; // '08' Length for JCOP OS Core ID
    uint8_t vJCOP_OS_Core_ID[8];     // JCOP OS Core ID
    uint8_t vStatus_Word_SW_[0x02];  //9000h Status Word (SW)
} identifyRsp_v2_t;

#endif // __SE05X_GETINFO_H__
