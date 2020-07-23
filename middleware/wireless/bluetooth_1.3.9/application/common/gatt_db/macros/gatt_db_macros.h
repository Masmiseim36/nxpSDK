/*! *********************************************************************************
 * \addtogroup GATT_DB
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef GATT_DB_MACROS_H
#define GATT_DB_MACROS_H

/*
*
* Useful macros
*
*/

/* Need two layers of indirection to avoid the expansion of __LINE__ together with the token
   paste macro which causes the __LINE__ to be taken literally. */
#define TOKEN_PASTE_LAYER_1(a,b) a##b
#define TOKEN_PASTE_LAYER_2(a,b) TOKEN_PASTE_LAYER_1(a,b)

#define HANDLE (__LINE__)
#define NEXT_HANDLE (__LINE__ + 1)

#define LSB2(two_byte_value) ((uint8_t) ((two_byte_value) & 0xFF))
#define MSB2(two_byte_value) ((uint8_t) (((two_byte_value) >> 8) & 0xFF))

#define LSBF(multi_byte_value, byte_index_lsb_first) \
    ((uint8_t) (((multi_byte_value) >> (8 * (byte_index_lsb_first))) & 0xFF))

/*
*
* Attribute name allocation macros
* These allocate the name for the Attribute Value in the database
*
*/

/*
* Service Declaration Attribute Value contains:
*  - service UUID (2/4/16 bytes)
*/

#define PRIMARY_SERVICE_ALLOC(name, uuid)\
    static uint8_t name##_valueArray[2] = { LSB2(uuid), MSB2(uuid) };

#define PRIMARY_SERVICE_UUID32_ALLOC(name, ...)\
    static uint8_t name##_valueArray[4] = { __VA_ARGS__ };

#define PRIMARY_SERVICE_UUID128_ALLOC(name, uuid128)\
    static uint8_t name##_valueArray[16];

#define PRIMARY_SERVICE_UUID128_INIT(name, uuid128) \
    for (uint8_t j = 0; j < gcBleLongUuidSize_c; j++) { (name##_valueArray)[j] = (uuid128)[j]; }

#define SECONDARY_SERVICE_ALLOC                PRIMARY_SERVICE_ALLOC
#define SECONDARY_SERVICE_UUID32_ALLOC        PRIMARY_SERVICE_UUID32_ALLOC
#define SECONDARY_SERVICE_UUID128_ALLOC        PRIMARY_SERVICE_UUID128_ALLOC
#define SECONDARY_SERVICE_UUID128_INIT        PRIMARY_SERVICE_UUID128_INIT


/*
* Include Declaration Attribute Value contains:
*  - service start handle (2 bytes)
*  - end group handle (2 bytes)
*  - service UUID (2 bytes) -> exists only for 16-bit service UUIDs
*/

#define INCLUDE_ALLOC(service_attribute_handle)\
    static uint8_t TOKEN_PASTE_LAYER_2(include_##service_attribute_handle##_valueArray,__LINE__)[6] = { LSB2(service_attribute_handle), MSB2(service_attribute_handle), 0, 0, 0, 0 };

#define INCLUDE_CUSTOM_ALLOC(service_attribute_handle)\
    static uint8_t TOKEN_PASTE_LAYER_2(include_##service_attribute_handle##_valueArray,__LINE__)[4] = { LSB2(service_attribute_handle), MSB2(service_attribute_handle), 0, 0 };

/*
* Characteristic Declaration Attribute Value contains:
*  - characteristic properties (1 byte)
*  - characteristic value Attribute Handle (2 bytes)
*  - characteristic UUID (2/4/16 bytes)
*/

#define CHARACTERISTIC_ALLOC(name, uuid, properties)\
    static uint8_t name##_valueArray[5] = { (uint8_t)(properties), LSB2(NEXT_HANDLE), MSB2(NEXT_HANDLE), LSB2(uuid), MSB2(uuid) };

#define CHARACTERISTIC_UUID32_ALLOC(name, uuid32, properties)\
    static uint8_t name##_valueArray[7] = { (uint8_t)(properties), LSB2(NEXT_HANDLE), MSB2(NEXT_HANDLE), \
        LSBF(uuid32, 0), LSBF(uuid32, 1), LSBF(uuid32, 2), LSBF(uuid32, 3) };

#define CHARACTERISTIC_UUID128_ALLOC(name, properties) \
    static uint8_t name##_valueArray[19] = { (uint8_t)(properties), LSB2(NEXT_HANDLE), MSB2(NEXT_HANDLE), \
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; \

#define CHARACTERISTIC_UUID128_INIT(name, uuid128) \
    for (uint8_t j = 0; j < gcBleLongUuidSize_c; j++) { (name##_valueArray)[3 + j] = (uuid128)[j]; }

/*
*
* Characteristic Value Attribute Value
*  - max 512 bytes
*/

#define VALUE_ALLOC(name, size, ...)\
    static uint8_t name##_valueArray[size] = { __VA_ARGS__ };

#define VALUE_VARLEN_ALLOC(name, maxSize, ...)\
    static uint8_t name##_valueArray[maxSize] = { __VA_ARGS__ };

/*
* Client Characteristic Configuration Descriptor Attribute Value contains:
*  - characteristic configuration bits (2 bytes)
*/

#define CCCD_ALLOC(name)\
    static uint8_t name##_valueArray[2] = { LSB2(gCccdEmpty_c), MSB2(gCccdEmpty_c) };

/*
* General Descriptor Attribute Value
*  - max 512 bytes
*/

#define DESCRIPTOR_ALLOC  VALUE_ALLOC

/*
*
* Database initialization macros
* These initialize the database Attribute Array
* Each Attribute in the database has:
*    - Attribute Handle (2 bytes)
*    - Attribute Permissions (1 bytes)
*    - Attribute UUID (2/4/16 bytes)
*    - Attribute Value (variable length name)
*        - name - allocated by _ALLOC macros
*        - size  - depending on Attribute UUID
*
*/

/*
* Service declaration
*  - UUID = gBleSig_PrimaryService_d for Primary Service
*        gBleSig_SecondaryService_d for Secondary Service
*  - permissions = Read Only
*/

#define PRIMARY_SERVICE_DECL(name)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_PrimaryService_d,\
       name##_valueArray,\
       2, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define PRIMARY_SERVICE_UUID32_DECL(name)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_PrimaryService_d,\
       name##_valueArray,\
       4, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define PRIMARY_SERVICE_UUID128_DECL(name)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_PrimaryService_d,\
       name##_valueArray,\
       16, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define SECONDARY_SERVICE_DECL(name)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_SecondaryService_d,\
       name##_valueArray,\
       2, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define SECONDARY_SERVICE_UUID32_DECL(name)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_SecondaryService_d,\
       name##_valueArray,\
       4, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define SECONDARY_SERVICE_UUID128_DECL(name)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_SecondaryService_d,\
       name##_valueArray,\
       16, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define PRIMARY_SERVICE_SECURITY(security)
#define SECONDARY_SERVICE_SECURITY(security)

/*
* Include declaration
*  - UUID = gBleSig_Include_d for Primary Service
*  - permissions = Read Only
*/

#define INCLUDE_DECL(service_attribute_handle)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_Include_d,\
       TOKEN_PASTE_LAYER_2(include_##service_attribute_handle##_valueArray,__LINE__),\
       6, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define INCLUDE_CUSTOM_DECL(service_attribute_handle)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_Include_d,\
       TOKEN_PASTE_LAYER_2(include_##service_attribute_handle##_valueArray,__LINE__),\
       4, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define INCLUDE_INIT(service_attribute_handle)\
    for (uint16_t ji = 0, done = 0; ji < gGattDbAttributeCount_c && done == 0; ji++) { \
        if (gattDatabase[ji].handle == HANDLE) {\
            /* ji is the index in the database where this include is declared */\
            uint32_t service_start_handle = gattDatabase[ji].pValue[0] | (gattDatabase[ji].pValue[1] << 8);\
            for (uint16_t j = 0; j < gGattDbAttributeCount_c; j++) {\
                if (gattDatabase[j].handle == service_start_handle) {\
                    /* j is the index in the database where the service is declared */\
                    if (gattDatabase[j].valueLength == 2) {\
                        /* 16-bit Service UUID */\
                        gattDatabase[ji].pValue[4] = gattDatabase[j].pValue[0];\
                        gattDatabase[ji].pValue[5] = gattDatabase[j].pValue[1];\
                    }\
                    uint16_t k = j + 1;\
                    /* move k to the next service declaration or the end of the database*/\
                    while (k < gGattDbAttributeCount_c && (gattDatabase[k].uuidType != gBleUuidType16_c ||\
                        (gattDatabase[k].uuid != gBleSig_PrimaryService_d && gattDatabase[k].uuid != gBleSig_SecondaryService_d)))\
                    {\
                        k++;\
                    }\
                    /* previous entry in the database from where k is must contain the end group handle */\
                    gattDatabase[ji].pValue[2] = LSB2(gattDatabase[k-1].handle);\
                    gattDatabase[ji].pValue[3] = MSB2(gattDatabase[k-1].handle);\
                    done = 1;\
                    break;\
                }\
            }\
        }\
    }

#define INCLUDE_CUSTOM_INIT(service_attribute_handle)\
    for (uint16_t ji = 0, done = 0; ji < gGattDbAttributeCount_c && done == 0; ji++) { \
        if (gattDatabase[ji].handle == HANDLE) {\
            /* ji is the index in the database where this include is declared */\
            uint32_t service_start_handle = gattDatabase[ji].pValue[0] | (gattDatabase[ji].pValue[1] << 8);\
            for (uint16_t j = 0; j < gGattDbAttributeCount_c; j++) {\
                if (gattDatabase[j].handle == service_start_handle) {\
                    /* j is the index in the database where the service is declared */\
                    uint16_t k = j + 1;\
                    /* move k to the next service declaration or the end of the database*/\
                    while (k < gGattDbAttributeCount_c && (gattDatabase[k].uuidType != gBleUuidType16_c ||\
                        (gattDatabase[k].uuid != gBleSig_PrimaryService_d && gattDatabase[k].uuid != gBleSig_SecondaryService_d)))\
                    {\
                        k++;\
                    }\
                    /* previous entry in the database from where k is must contain the end group handle */\
                    gattDatabase[ji].pValue[2] = LSB2(gattDatabase[k-1].handle);\
                    gattDatabase[ji].pValue[3] = MSB2(gattDatabase[k-1].handle);\
                    done = 1;\
                    break;\
                }\
            }\
        }\
    }

/*
* Characteristic declaration
*  - UUID = gBleSig_Characteristic_d
*  - permissions = Read Only
*/

#define CHARACTERISTIC_DECL(name)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_Characteristic_d,\
       name##_valueArray,\
       5, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define CHARACTERISTIC_UUID32_DECL(name)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_Characteristic_d,\
       name##_valueArray,\
       7, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

#define CHARACTERISTIC_UUID128_DECL(name)\
    {\
       HANDLE,\
       (uint16_t)gPermissionFlagReadable_c,\
       gBleSig_Characteristic_d,\
       name##_valueArray,\
       19, \
       (uint16_t)gBleUuidType16_c, \
       0, \
    },

/*
* Characteristic Value declaration
*  - UUID = Characteristic UUID
*  - permissions = custom
*
* ! This must be placed immediately after the Characteristic declaration
*/

#define VALUE_DECL(name, size, uuid, permissions)\
    {\
        HANDLE,\
        (uint16_t)permissions,\
        uuid,\
        name##_valueArray,\
        (uint16_t)size, \
        (uint16_t)gBleUuidType16_c, \
        0, \
    },

#define VALUE_UUID32_DECL(name, size, uuid32, permissions)\
    {\
        HANDLE,\
        (uint16_t)permissions,\
        uuid,\
        name##_valueArray,\
        (uint16_t)size, \
        (uint16_t)gBleUuidType32_c, \
        0, \
    },

#define VALUE_UUID128_DECL(name, size, uuid128, permissions)\
    {\
        HANDLE,\
        (uint16_t)permissions,\
        (uint32_t) uuid128,\
        name##_valueArray,\
        (uint16_t)size, \
        (uint16_t)gBleUuidType128_c, \
        0, \
    },

#define VALUE_VARLEN_DECL(name, maxSize, initSize, uuid, permissions)\
    {\
        HANDLE,\
        (uint16_t)permissions,\
        uuid,\
        name##_valueArray,\
        (uint16_t)initSize, \
        (uint16_t)gBleUuidType16_c, \
        (uint16_t)maxSize, \
    },

#define VALUE_UUID32_VARLEN_DECL(name, maxSize, initSize, uuid32, permissions)\
    {\
        HANDLE,\
        (uint16_t)permissions,\
        uuid,\
        name##_valueArray,\
        (uint16_t)initSize, \
        (uint16_t)gBleUuidType32_c, \
        (uint16_t)maxSize, \
    },

#define VALUE_UUID128_VARLEN_DECL(name, maxSize, initSize, uuid128, permissions)\
    {\
        HANDLE,\
        (uint16_t)permissions,\
        (uint32_t) uuid128,\
        name##_valueArray,\
        (uint16_t)initSize, \
        (uint16_t)gBleUuidType128_c, \
        (uint16_t)maxSize, \
    },

/*
* Client Characteristic Configuration Descriptor declaration
*  - UUID = 0x2902
*  - permissions = custom
*/

#define CCCD_DECL(name)\
    {\
        HANDLE,\
        (uint16_t)((uint16_t)gPermissionFlagReadable_c | (uint16_t)gPermissionFlagWritable_c),\
        gBleSig_CCCD_d,\
        name##_valueArray,\
        2, \
        (uint16_t)gBleUuidType16_c, \
        0, \
    },

/*
* Characteristic Descriptor declaration
*  - UUID = Descriptor UUID
*  - permissions = custom
*
*/

#define DESCRIPTOR_DECL         VALUE_DECL
#define DESCRIPTOR_UUID32_DECL  VALUE_UUID32_DECL
#define DESCRIPTOR_UUID128_DECL VALUE_UUID128_DECL


/*
* Macro for counting the attributes in the database
*/

#define UNIVERSAL_MACRO_SIZE(name)  uint32_t name##_long;

#define INCLUDE_MACRO_SIZE(name)  uint32_t TOKEN_PASTE_LAYER_2(name##_long,__LINE__);


/*
* Macros for enumeration
*/

#define UNIVERSAL_MACRO_ENUM(name) name = HANDLE,

#define INCLUDE_MACRO_ENUM(name) TOKEN_PASTE_LAYER_2(name,__LINE__) = HANDLE,

#endif /* GATT_DB_MACROS_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
