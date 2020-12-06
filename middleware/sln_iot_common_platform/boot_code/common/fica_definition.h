/*
 * Copyright 2018-2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _FICA_DEFINITION_
#define _FICA_DEFINITION_

/*******************************************************************************
 * FICA (Flash Image Configuration Area) Definitions
 ******************************************************************************/
#define FICA_VER 2

// FICA Descriptor, used to determine if the FICA area has been initialized or not
#define FICA_ICA_DESC 0xA5A5A5A5

// Start address of FICA area in flash
#define FICA_START_ADDR 0x01FC0000 // with 256KB Erase Block
#define FICA_TABLE_SIZE 0x00040000 // with 256KB Erase Block

// FICA field size
#define FICA_FIELD_SIZE 4

/*! @brief FICA Header Structure */
typedef struct __attribute__((packed)) _fica_header
{
    uint32_t descriptor;    /*!< FICA Descriptor field */
    uint32_t version;       /*!< FICA Version number */
    uint32_t communication; /*!< Inter-App communication field */
    uint32_t currType;      /*!< New App Status */
    uint32_t newType;       /*!< New App Type */
    uint32_t currBootType;  /*!< Current boot type, typically FICA_IMG_TYPE_BOOTLOADER, but may vary */
} fica_header_t;

#define CHECK_APP_SIGN 1
#define APP_SIGN_SIZE 2048

/*******************************************************************************
 * Image Field Definitions
 * Defaults and choices for each of the FICA Record fields
 * Each offset is a word (4 bytes)
 ******************************************************************************/

// Image Record Start Descriptor
#define FICA_REC_START_ID 0x5A5A5A5A

// Image Record Field Default values and choices
// Offset 0 - Descriptor ID (start id)
#define FICA_IMG_DESC_ID 0x5A5A5A5A

/*! @brief FICA Image Types enumeration */
typedef enum _fica_img_type
{
    FICA_IMG_TYPE_NONE = -1,  /*!< Default */
    FICA_IMG_TYPE_BOOTLOADER, /*!< Bootloader Image */
    FICA_IMG_TYPE_APP_A,      /*!< Application A Image */
    FICA_IMG_TYPE_APP_B,      /*!< Application B Image */
    FICA_NUM_IMG_TYPES,       /*!< Total Number of Images Defined in this Version */
} fica_img_type_t;

#define FICA_COMM_BOOT_NONE 0x00000000
#define FICA_COMM_AIS_OTA_BIT 0x00000010
#define FICA_COMM_FWUPDATE_BIT 0x00000020
#define FICA_COMM_AIS_NAV_BIT 0x00000100
#define FICA_COMM_AIS_NAP_BIT 0x00000200
#define FICA_COMM_AIS_NAI_BIT 0x00000400

#if (defined(ENABLE_UNSIGNED_USB_MSD) || ENABLE_UNSIGNED_USB_MSD == 1)
// NOTE: To facilitate ease of use in SLN-ALEXA-IOT kits
#define FICA_COMM_AIS_USB_BIT 0x00000800
#endif

// Image Flash Start Addresses for this flash (IS26KL256S)
#define FICA_IMG_ICA_DEF_ADDR FICA_START_ADDR + FICA_ICA_DEFINITION_SIZE

#define FICA_IMG_BOOTSTRAP_ADDR 0x00000000
#define FICA_IMG_BOOTSTRAP_IVT_ADDR 0x00001000
#define FICA_IMG_BOOTLOADER_ADDR 0x00040000
#define FICA_IMG_RESERVED_ADDR 0x00200000
#define FICA_IMG_APP_A_ADDR 0x00300000
#define FICA_IMG_APP_B_ADDR 0x00D00000

// The ResetISR address is placed on the index 1 of the vector table
#define APPLICATION_RESET_ISR_ADDRESS (SCB->VTOR + 0x4)

#define FICA_IMG_BOOTSTRAP_SIZE 0x00040000  // 0.25 MB
#define FICA_IMG_BOOTLOADER_SIZE 0x001C0000 // 1.75 MB
#define FICA_IMG_RESERVED_SIZE 0x00100000   // 1 MB
#define FICA_IMG_APP_A_SIZE 0x00A00000      // 10 MB
#define FICA_IMG_APP_B_SIZE 0x00A00000      // 10 MB

// Image Size
#define FICA_IMG_SIZE_ZERO 0 // default

/*! @brief FICA Image Format Types enumeration */
typedef enum _fica_img_fmt
{
    FICA_IMG_FMT_NONE = 0, /*!< default */
    FICA_IMG_FMT_BIN,      /*!< Binary */
    FICA_IMG_FMT_SREC,     /*!< S-Record */
    FICA_IMG_FMT_AES_128,  /*!< Secure Binary */
} fica_img_fmt_t;

// Image CRC
#define FICA_IMG_CRC_NONE 0 // default

// Image Key
#define FICA_IMG_HASH_NONE 0 // default

// Offset 7 - Reserved
#define FICA_IMG_RSV 0 // default

/*! @brief FICA Record Structure */
typedef struct __attribute__((packed)) _fica_record
{
    uint32_t descriptor;    /*!< Record descriptor */
    uint32_t imgType;       /*!< Image type (see fica_img_type_t) */
    uint32_t imgAddr;       /*!< Image start address */
    uint32_t imgLen;        /*!< Image length */
    uint32_t imgFmt;        /*!< Image format (see fica_img_fmt_t) */
    uint32_t imgHashType;   /*!< Image hash type */
    uint32_t imgHashLoc;    /*!<  */
    uint32_t imgEncType;    /*!<  */
    uint32_t imgPkiType;    /*!<  */
    uint32_t imgPkiLoc;     /*!<  */
    uint8_t imgPkiSig[256]; /*!<  */
    uint8_t res[12];        /*!<  */
} fica_record_t;

/*! @brief FICA Record Structure */
typedef struct __attribute__((packed)) _fica
{
    fica_header_t header;
    fica_record_t records[FICA_NUM_IMG_TYPES];
} fica_t;

#endif // _FICA_DEFINITION_
