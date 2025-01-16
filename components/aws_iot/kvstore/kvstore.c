/*
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/**
 * @file kvstore.c
 * @brief File contains the implementation of a configuration store (key-value pair store)
 * persisted in non-volatile memory. APIs are provided to insert or retrieve values of
 * different data types such as unsigned/signed integer types, base types, string or a byte blob.
 * Values are stored and retrieved using a key string. APIs are thread safe and can be called
 * concurrently from different tasks.
 */

#include "FreeRTOS.h"
#include "semphr.h"
#include "kvstore.h"
#include "mflash_file.h"

/* Magic numbers to check for presence of Key Value Store. */
#define KVSTORE_MAGIC_NUMBER    ( 0x11223344U )

typedef struct KeyValueStoreHeader
{
    uint32_t magicNumber;
    uint32_t numEntries;
} KVStoreHeader_t;

typedef struct KVStoreEntry
{
    char key[ KVSTORE_KEY_MAX_LEN + 1 ];
    KVStoreValueType_t valueType;
    uint32_t valueLength;
    union
    {
        uint32_t u32;
        int32_t i32;
        BaseType_t bt;
        UBaseType_t ubt;
        uint8_t value[ KVSTORE_VAL_MAX_LEN ];
    };
} KVStoreEntry_t;

typedef struct KeyValueStore
{
    KVStoreHeader_t header;
    KVStoreEntry_t table[ KVS_NUM_KEYS ];
} KeyValueStore_t;

static const char * keys[ KVS_NUM_KEYS ] = KVSTORE_KEYS;
static const KVStoreDefaultEntry_t gDefaultValues[ KVS_NUM_KEYS ] = KV_STORE_DEFAULTS;
static KeyValueStore_t gKeyValueStore = { 0 };
static BaseType_t keyValueStoreInitialized = pdFALSE;
static SemaphoreHandle_t keyValueStoreMutex;

static void prvCopyDefaultValue( KVStoreEntry_t * pEntry,
                                 const KVStoreDefaultEntry_t * pDefaultEntry )
{
    pEntry->valueType = pDefaultEntry->type;
    pEntry->valueLength = pDefaultEntry->length;

    switch( pDefaultEntry->type )
    {
        case KV_TYPE_BASE_T:
            pEntry->bt = pDefaultEntry->bt;
            break;

        case KV_TYPE_UBASE_T:
            pEntry->ubt = pDefaultEntry->ubt;
            break;

        case KV_TYPE_INT32:
            pEntry->i32 = pDefaultEntry->i32;
            break;

        case KV_TYPE_UINT32:
            pEntry->u32 = pDefaultEntry->u32;
            break;

        case KV_TYPE_STRING:
            configASSERT( pEntry->valueLength < KVSTORE_VAL_MAX_LEN );
            memcpy( pEntry->value, pDefaultEntry->str, pEntry->valueLength );
            pEntry->value[ pEntry->valueLength ] = '\0';
            break;

        case KV_TYPE_BLOB:
            configASSERT( pEntry->valueLength <= KVSTORE_VAL_MAX_LEN );
            memcpy( pEntry->value, pDefaultEntry->blob, pEntry->valueLength );
            break;

        default:
            break;
    }
}

static BaseType_t prvLoadConfigStore( void )
{
    status_t flashStatus;
    KeyValueStore_t * keyStorePtr = NULL;
    size_t index, keyIndex;
    uint32_t keyStoreSize = 0;
    BaseType_t result = pdFAIL, valueFound = pdFALSE;


    flashStatus = mflash_file_mmap( KVSTORE_FILE_PATH, ( uint8_t ** ) &keyStorePtr, &keyStoreSize );

    if( flashStatus == kStatus_Success )
    {
        if( keyStorePtr->header.magicNumber == ( uint32_t ) KVSTORE_MAGIC_NUMBER )
        {
            /*
             * If a key value store entry exists in the flash then load the persisted entry
             * into memory else copy the default values.
             */
            gKeyValueStore.header.magicNumber = KVSTORE_MAGIC_NUMBER;
            gKeyValueStore.header.numEntries = KVS_NUM_KEYS;

            for( keyIndex = 0; keyIndex < KVS_NUM_KEYS; keyIndex++ )
            {
                strcpy( gKeyValueStore.table[ keyIndex ].key, keys[ keyIndex ] );
                valueFound = pdFALSE;

                for( index = 0; index < keyStorePtr->header.numEntries; index++ )
                {
                    if( strcmp( keys[ keyIndex ], keyStorePtr->table[ index ].key ) == 0 )
                    {
                        gKeyValueStore.table[ keyIndex ].valueType = keyStorePtr->table[ index ].valueType;
                        gKeyValueStore.table[ keyIndex ].valueLength = keyStorePtr->table[ index ].valueLength;
                        memcpy( gKeyValueStore.table[ keyIndex ].value, keyStorePtr->table[ index ].value, keyStorePtr->table[ index ].valueLength );
                        valueFound = pdTRUE;
                        break;
                    }
                }

                if( valueFound == pdFALSE )
                {
                    /* Copy default value. */
                    prvCopyDefaultValue( &gKeyValueStore.table[ keyIndex ], &gDefaultValues[ keyIndex ] );
                }
            }

            result = pdPASS;
        }
        else
        {
            result = pdFAIL;
        }
    }
    else
    {
        /* Could not find a persisted key value store. Write a new store with the default values. */


        gKeyValueStore.header.magicNumber = KVSTORE_MAGIC_NUMBER;
        gKeyValueStore.header.numEntries = KVS_NUM_KEYS;

        for( keyIndex = 0; keyIndex < KVS_NUM_KEYS; keyIndex++ )
        {
            strcpy( gKeyValueStore.table[ keyIndex ].key, keys[ keyIndex ] );
            prvCopyDefaultValue( &gKeyValueStore.table[ keyIndex ], &gDefaultValues[ keyIndex ] );
        }

        result = pdPASS;
    }

    return result;
}

BaseType_t KVStore_init( void )
{
    BaseType_t result = pdFAIL;

    if( mflash_is_initialized() == true )
    {
        result = prvLoadConfigStore();

        if( result == pdPASS )
        {
            keyValueStoreMutex = xSemaphoreCreateMutex();

            if( keyValueStoreMutex != NULL )
            {
                keyValueStoreInitialized = pdTRUE;
            }
            else
            {
                result = pdFAIL;
            }
        }
    }

    return result;
}

BaseType_t KVStore_setBlob( KVStoreKey_t key,
                            size_t xLength,
                            const void * pvNewValue )
{
    BaseType_t result = pdFAIL;
    status_t flashStatus;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            if( ( pvNewValue != NULL ) &&
                ( xLength > 0 ) &&
                ( xLength <= KVSTORE_VAL_MAX_LEN ) )
            {
                gKeyValueStore.table[ key ].valueLength = xLength;
                memcpy( gKeyValueStore.table[ key ].value, pvNewValue, xLength );
                flashStatus = mflash_file_save( KVSTORE_FILE_PATH, ( uint8_t * ) &gKeyValueStore, sizeof( gKeyValueStore ) );

                if( flashStatus == kStatus_Success )
                {
                    result = pdPASS;
                }
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return result;
}

size_t KVStore_getBlob( KVStoreKey_t key,
                        void * pvBuffer,
                        size_t xMaxLength )
{
    size_t lengthCopied = 0;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            if( ( pvBuffer != NULL ) &&
                ( gKeyValueStore.table[ key ].valueLength > 0 ) &&
                ( xMaxLength >= gKeyValueStore.table[ key ].valueLength ) )
            {
                memcpy( pvBuffer, gKeyValueStore.table[ key ].value, gKeyValueStore.table[ key ].valueLength );
                lengthCopied = gKeyValueStore.table[ key ].valueLength;
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return lengthCopied;
}

BaseType_t KVStore_setString( KVStoreKey_t key,
                              size_t xLength,
                              const char * pcNewValue )
{
    BaseType_t result = pdFAIL;
    status_t flashStatus;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            if( ( pcNewValue != NULL ) &&
                ( xLength > 0 ) &&
                ( xLength < KVSTORE_VAL_MAX_LEN ) )
            {
                gKeyValueStore.table[ key ].valueLength = xLength;
                strncpy( ( char * ) gKeyValueStore.table[ key ].value, pcNewValue, xLength );
                flashStatus = mflash_file_save( KVSTORE_FILE_PATH, ( uint8_t * ) &gKeyValueStore, sizeof( gKeyValueStore ) );

                if( flashStatus == kStatus_Success )
                {
                    result = pdPASS;
                }
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return result;
}

size_t KVStore_getString( KVStoreKey_t key,
                          char * pvBuffer,
                          size_t xMaxLength )
{
    size_t lengthCopied = 0;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            if( ( pvBuffer != NULL ) &&
                ( gKeyValueStore.table[ key ].valueLength > 0 ) &&
                ( xMaxLength > gKeyValueStore.table[ key ].valueLength ) )
            {
                memcpy( pvBuffer, gKeyValueStore.table[ key ].value, gKeyValueStore.table[ key ].valueLength );
                lengthCopied = gKeyValueStore.table[ key ].valueLength;
                pvBuffer[ lengthCopied ] = '\0';
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return lengthCopied;
}

uint32_t KVStore_getUInt32( KVStoreKey_t key,
                            BaseType_t * pxSuccess )
{
    uint32_t value = 0U;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            if( ( gKeyValueStore.table[ key ].valueType == KV_TYPE_UINT32 ) &&
                ( gKeyValueStore.table[ key ].valueLength > 0 ) )
            {
                value = gKeyValueStore.table[ key ].u32;
                ( *pxSuccess ) = pdTRUE;
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return value;
}

BaseType_t KVStore_setUInt32( KVStoreKey_t key,
                              uint32_t ulNewVal )
{
    BaseType_t result = pdFAIL;
    status_t flashStatus;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            gKeyValueStore.table[ key ].u32 = ulNewVal;
            gKeyValueStore.table[ key ].valueType = KV_TYPE_UINT32;
            gKeyValueStore.table[ key ].valueLength = sizeof( ulNewVal );
            flashStatus = mflash_file_save( KVSTORE_FILE_PATH, ( uint8_t * ) &gKeyValueStore, sizeof( gKeyValueStore ) );

            if( flashStatus == kStatus_Success )
            {
                result = pdPASS;
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return result;
}

int32_t KVStore_geInt32( KVStoreKey_t key,
                         BaseType_t * pxSuccess )
{
    int32_t value = 0L;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            if( ( gKeyValueStore.table[ key ].valueType == KV_TYPE_INT32 ) &&
                ( gKeyValueStore.table[ key ].valueLength > 0 ) )
            {
                value = gKeyValueStore.table[ key ].i32;
                ( *pxSuccess ) = pdTRUE;
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return value;
}

BaseType_t KVStore_setInt32( KVStoreKey_t key,
                             int32_t lNewVal )
{
    BaseType_t result = pdFAIL;
    status_t flashStatus;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            gKeyValueStore.table[ key ].i32 = lNewVal;
            gKeyValueStore.table[ key ].valueType = KV_TYPE_INT32;
            gKeyValueStore.table[ key ].valueLength = sizeof( lNewVal );
            flashStatus = mflash_file_save( KVSTORE_FILE_PATH, ( uint8_t * ) &gKeyValueStore, sizeof( gKeyValueStore ) );

            if( flashStatus == kStatus_Success )
            {
                result = pdPASS;
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return result;
}


UBaseType_t KVStore_getUBase( KVStoreKey_t key,
                              BaseType_t * pxSuccess )
{
    UBaseType_t value = 0L;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            if( ( gKeyValueStore.table[ key ].valueType == KV_TYPE_UBASE_T ) &&
                ( gKeyValueStore.table[ key ].valueLength > 0 ) )
            {
                value = gKeyValueStore.table[ key ].ubt;
                ( *pxSuccess ) = pdTRUE;
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return value;
}

BaseType_t KVStore_setUBase( KVStoreKey_t key,
                             UBaseType_t uxNewVal )
{
    BaseType_t result = pdFAIL;
    status_t flashStatus;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            gKeyValueStore.table[ key ].ubt = uxNewVal;
            gKeyValueStore.table[ key ].valueType = KV_TYPE_UBASE_T;
            gKeyValueStore.table[ key ].valueLength = sizeof( uxNewVal );
            flashStatus = mflash_file_save( KVSTORE_FILE_PATH, ( uint8_t * ) &gKeyValueStore, sizeof( gKeyValueStore ) );

            if( flashStatus == kStatus_Success )
            {
                result = pdPASS;
            }
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return result;
}


size_t KVStore_getValueLength( KVStoreKey_t key )
{
    size_t lengthRet = 0;

    if( keyValueStoreInitialized == pdTRUE )
    {
        ( void ) xSemaphoreTake( keyValueStoreMutex, portMAX_DELAY );
        {
            lengthRet = gKeyValueStore.table[ key ].valueLength;
        }
        ( void ) xSemaphoreGive( keyValueStoreMutex );
    }

    return lengthRet;
}

BaseType_t KVStore_getKey( const char * pInput,
                           size_t length,
                           const char ** pKey,
                           KVStoreKey_t * pKeyType )
{
    BaseType_t result = pdFAIL;
    size_t index;

    for( index = 0; index < KVS_NUM_KEYS; index++ )
    {
        if( ( length >= strlen( keys[ index ] ) ) &&
            ( strncmp( pInput, keys[ index ], strlen( keys[ index ] ) ) == 0 ) )
        {
            ( *pKey ) = keys[ index ];
            ( *pKeyType ) = ( KVStoreKey_t ) ( index );
            result = pdPASS;
            break;
        }
    }

    return result;
}
