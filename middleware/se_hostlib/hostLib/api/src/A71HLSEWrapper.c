/**
 * @file A71HLSEWrapper.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2017
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
 * @par Description
 * Host Lib wrapper API implementation over the A71CH host library
 *
 * @par HISTORY
 *
 */

#include <stdlib.h>
#include "HLSEAPI.h"
#include "ax_api.h"
#include "a71_debug.h"
#include "hcAsn.h"

/// Key Operations
typedef struct {
    U8 keyVersion;          //!< key Version
    U8 keyEnc[16];          //!< Enc Key
    U8 keyMac[16];          //!< Mac Key
    U8 keyDek[16];          //!< Dek Key
    U8* currentKeyDek;      //!< Current Dek Key
    U16 currentKeyDekLen;   //!< Current Dek Key length
} HLSE_A71_SM_KEYS_DATA;

#define CREDENTIAL_UNLOCKED     0x05    //!< Credential Unlocked
#define CREDENTIAL_LOCKED       0x0F    //!< Credential locked
#define CREDENTIAL_ABSENT       0xA0    //!< Credential Absent
#define CREDENTIAL_INITIALIZED  0x50    //!< Credential initialized

// GetCredentialnfo returns Credential states values as below (see 6.2.2.2 CredentialInfo) :
#define CREDENTIAL_UNLOCKED_ABSENT       CREDENTIAL_UNLOCKED | CREDENTIAL_ABSENT            //!< Credential unlocked and absent
#define CREDENTIAL_UNLOCKED_INITIALIZED  CREDENTIAL_UNLOCKED | CREDENTIAL_INITIALIZED       //!< Credential unlocked and initialized
#define CREDENTIAL_LOCKED_ABSENT         CREDENTIAL_LOCKED   | CREDENTIAL_ABSENT            //!< Credential locked and absent
#define CREDENTIAL_LOCKED_INITIALIZED    CREDENTIAL_LOCKED   | CREDENTIAL_INITIALIZED       //!< Credential locked and initialized

/// offsets in CredentialinfoTable (CIT) returned from GetCredentialInfo
/// as of applet 1.3 (in ROM only)
#define CIT_SCP_OFFSET          0
/// 4 key pairs
#define CIT_KEYPAIR_OFFSET      1
/// 3 pub keys
#define CIT_PUBKEY_OFFSET       5
/// 3 config keys
#define CIT_CONFIGKEY_OFFSET    8
/// 8 symmetric keys
#define CIT_SYMMETRICKEY_OFFSET 11
/// 2 counters
#define CIT_COUNTER_OFFSET      19
/// GP Storage
#define CIT_GP_OFFSET           21



//*******************************************************************
// Logical objects (e.g. Certificates) in GP Data
//*******************************************************************

/// Set this to 1 if more than one process acesses the SE and may update the mapping table
//#define HLSE_MULTI_PROCESS              1

// currently supporting 1 chunk =32 bytes of Gp Data table
// this is a preparaion if more is required to hold more than 5 logical objects
#define HLSE_GP_DATA_CHUNKS_NUM         1       //!< Num of chunks in GP Table

#define HLSE_GP_DATA_CHUNK              32      //!< GP Data chunk size byted

#define HLSE_FREE_CHUNK                 0       //!< Free chunk indication
#define HLSE_OCCUPIED_CHUNK             1       //!< Occupied chunk indication
#define HLSE_LOCKED_CHUNK               2       //!< Locked Chunk indication

#define HLSE_INDIRECT_LENGTH            0x8000  //!< indirect length indication - obtain from TLV

/// Alligned Size
#define HLSE_ALIGN_SIZE(size)           ((size + HLSE_GP_DATA_CHUNK - 1) / HLSE_GP_DATA_CHUNK)

/// To hold GP Size
static U16 gGPSize = 0;

/// GP Table Data Entry
typedef struct {
    U8      klass;
    U8      index;
    U16     length;
    U16     offset;
} HLSE_GP_DATA_TABLE_ENTRY;

#define HLSE_GP_DATA_TABLE_ENTRY_SIZE   6   //!< GP Table Entry size in bytes

/// Max number of objects that can be in GP Data (5 in case of 1 chunk is used)
#define HLSE_MAX_OBJECTS_IN_TABLE       (((HLSE_GP_DATA_CHUNK * HLSE_GP_DATA_CHUNKS_NUM) - 2 )/ 6)

/// GP Data Table
typedef struct {
    U8                          numOfEntries;                       //!< num of entries
    U8                          updateCounter;                      //!< update counter
    HLSE_GP_DATA_TABLE_ENTRY    entries[HLSE_MAX_OBJECTS_IN_TABLE]; //!< entries array
} HLSE_GP_DATA_TABLE;

/// cached Mapping table
static HLSE_GP_DATA_TABLE gMappingTable;
/// indication whether cahced mapping table already read
static U8                 gMappingTableRead = 0;


/**
 * Get The General Purpose memory size in bytes
 *
 * for internal use
 *
 * \param[out] gpSize OUT: General Purpose Data size in bytes
 *
 * \retval ::HLSE_SW_OK Successfull execution
 * \retval ::HLSE_ERR_API_ERROR Invalid function arguments
 */
static HLSE_RET_CODE GetGPDataSize(U16* gpSize)
{
    HLSE_RET_CODE lReturn = HLSE_SW_OK;

    // Get the Module's handle
    HLSE_OBJECT_HANDLE modHandle = 0;
    //U16 modHandleNum = 1;

    if (gGPSize != 0) {
        *gpSize = gGPSize;
        return HLSE_SW_OK;
    }

    modHandle = HLSE_CREATE_HANDLE(1, HLSE_MODULE, 0);

    {
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_MODULE_TOTAL_GP_SIZE;
        attr.value = gpSize;
        attr.valueLen = sizeof(U16);

        lReturn = HLSE_GetObjectAttribute(modHandle, &attr);
    }

    gGPSize = *gpSize;

    return lReturn;
}

/**
 * Sort GP mapping Table
 *
 * \param[in,out] table IN: pointer to gp mapping table, OUT: sorted table
 */
static void SortTable(HLSE_GP_DATA_TABLE* table)
{
    U8 i,j;
    if (table->numOfEntries >= 1) {
        for (i = 0; i < (HLSE_MAX_OBJECTS_IN_TABLE - 1); ++i) {
            for (j = i + 1; j < HLSE_MAX_OBJECTS_IN_TABLE; ++j) {
                if (table->entries[i].offset > table->entries[j].offset) {
                    // exchange entry positions
                    HLSE_GP_DATA_TABLE_ENTRY temp;
                    memcpy(&temp, &table->entries[i], sizeof(temp));
                    memcpy(&table->entries[i], &table->entries[j], sizeof(temp));
                    memcpy(&table->entries[j], &temp, sizeof(temp));
                }
            }
        }
    }
}

/**
 * Read the General Purpose mapping Data Table
 *
 * for internal use
 *
 * \param [out] table OUT: General Purpose Data table read
 * \param [in] forceReadFromGPData 1 to force read the table from GP memory instead of from cache, 0 read it from cached varaiable
 *
 * \retval ::HLSE_SW_OK Successfull execution
 */
static HLSE_RET_CODE ReadGPDataTable(HLSE_GP_DATA_TABLE* table, U8 forceReadFromGPData)
{
    // NOTE: only one chunk is read. To be updated if a table with more than 5 objects is used

    U16 gpSize;
    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    U8 dataRead[HLSE_GP_DATA_CHUNK * HLSE_GP_DATA_CHUNKS_NUM];
    U8 entryNum;

    U8 dataReadByteSize = sizeof(dataRead);
    int nObj;
    U8 bValidEntry;
    U8 nMaxObj;
    U8 tmpClass, tmpIndex;

#ifndef HLSE_MULTI_PROCESS
    if (forceReadFromGPData == 0) {
        if (gMappingTableRead) {
            memcpy(table, &gMappingTable, sizeof(gMappingTable));
            return HLSE_SW_OK;
        }
    }
#endif

    lReturn = GetGPDataSize(&gpSize);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    memset(table, 0xFF, sizeof(HLSE_GP_DATA_TABLE));
    table->numOfEntries = 0;
    table->updateCounter = 0;

    // Read the entire table
    lReturn = A71_GetGpData(gpSize - dataReadByteSize, dataRead, dataReadByteSize);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    table->numOfEntries = dataRead[dataReadByteSize - 1];
    if (table->numOfEntries == 0xFF) {
        // the table doesnt exist or invalid - use a dummy one
        memcpy(&gMappingTable, table, sizeof(gMappingTable));
        gMappingTableRead = 1;
        return HLSE_SW_OK;
    }

    table->updateCounter = dataRead[dataReadByteSize - 2];

    // Read only Valid entries up to numOfEntries starting from end of GP storage ( high address to low address )
    nObj = 1; // first object to check if valid in GP Table
    for (entryNum = 0; entryNum < table->numOfEntries; ++entryNum) {
        //  Notes:
        //      X + 1 is the address of the last byte of the GP Storage.
        //      N is the object number from 1 to N

        // read from object 1 to numOfEntries (skipping invalid =(deleted) entries)

        /*
        Address     Value
        -------     ----------------------
        X-N*6+0     N'th Object Class       - 1 byte
        X-N*6+1     N'th Object Index       - 1 byte
        X-N*6+2     N'th Object Length MSB  - 1 byte
        X-N*6+3     N'th Object Length LSB  - 1 byte
        X-N*6+4     N'th Object Offset MSB  - 1 byte
        X-N*6+5     N'th Object Offset LSB  - 1 byte
        */

        // Note : Since the table is kept compacted with invalid entries moved to its end whenever an entry is deleted -
        //        the following code to skip invalid entries is here just to be on the safe side in case provisioning was done incorrectly somehow,
        //        with the table created initially not with consecutive valid entries
        // {
        bValidEntry = 0;
        // max objects that could be held in the gp data table
        nMaxObj = HLSE_MAX_OBJECTS_IN_TABLE; //((HLSE_GP_DATA_CHUNK * HLSE_GP_DATA_CHUNKS_NUM) - 2) / 6;

        while (!bValidEntry && nObj <= nMaxObj) {
            // check if entry is valid
            tmpClass = dataRead[dataReadByteSize - 2 - nObj * 6 + 0];
            tmpIndex = dataRead[dataReadByteSize - 2 - nObj * 6 + 1];

            // skip to next valid entry
            if (tmpClass != 0xFF && tmpIndex != 0xFF) {
                // this is a valid entry
                bValidEntry = 1;
                break;
            }
            nObj++;     // skip to next object
        }

        if (!bValidEntry) {
            // error - no more valid entries in table , although num of entries in gp table says there are more !
            break;
        }
        // }

        // fill this valid entry in our table
        table->entries[entryNum].klass  = dataRead[dataReadByteSize - 2 - nObj * 6 + 0];
        table->entries[entryNum].index  = dataRead[dataReadByteSize - 2 - nObj * 6 + 1];
        table->entries[entryNum].length = dataRead[dataReadByteSize - 2 - nObj * 6 + 2] * 256 | dataRead[dataReadByteSize - 2 - nObj * 6 + 3];
        table->entries[entryNum].offset = dataRead[dataReadByteSize - 2 - nObj * 6 + 4] * 256 | dataRead[dataReadByteSize - 2 - nObj * 6 + 5];

        nObj++;
    }

    // sort the entries in ascending order of the offset
    SortTable(table);

    memcpy(&gMappingTable, table, sizeof(gMappingTable));
    gMappingTableRead = 1;

    return lReturn;
}

/**
 * Write the General Purpose mapping Data Table to Gp Memory and to cached variable
 *
 * for internal use
 *
 * \param [in] table IN: General Purpose Data table to write
 *
 * \retval ::HLSE_SW_OK Successfull execution
 */
static HLSE_RET_CODE WriteGPDataTable(HLSE_GP_DATA_TABLE* table)
{
    U16 gpSize;
    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    U8 dataToBeWritten[HLSE_GP_DATA_CHUNK * HLSE_GP_DATA_CHUNKS_NUM];
    U8 entryNum;

    U8 dataToBeWrittenSize = sizeof(dataToBeWritten);

    lReturn = GetGPDataSize(&gpSize);
    if (lReturn != HLSE_SW_OK) {
        return lReturn;
    }

    memset(dataToBeWritten, 0xFF, dataToBeWrittenSize);

    dataToBeWritten[dataToBeWrittenSize - 1] = table->numOfEntries;
    table->updateCounter++;
    dataToBeWritten[dataToBeWrittenSize - 2] = table->updateCounter;

    // fill in the table with objects up to num of entries , in gp storage from high memory address to low memory address
    for (entryNum = 0; entryNum < table->numOfEntries; ++entryNum) {
        dataToBeWritten[dataToBeWrittenSize - 2 - (entryNum + 1) * 6 + 0] = table->entries[entryNum].klass;
        dataToBeWritten[dataToBeWrittenSize - 2 - (entryNum + 1) * 6 + 1] = table->entries[entryNum].index;
        dataToBeWritten[dataToBeWrittenSize - 2 - (entryNum + 1) * 6 + 2] = (table->entries[entryNum].length >> 8) & 0xFF;
        dataToBeWritten[dataToBeWrittenSize - 2 - (entryNum + 1) * 6 + 3] = table->entries[entryNum].length & 0xFF;
        dataToBeWritten[dataToBeWrittenSize - 2 - (entryNum + 1) * 6 + 4] = (table->entries[entryNum].offset >> 8) & 0xFF;
        dataToBeWritten[dataToBeWrittenSize - 2 - (entryNum + 1) * 6 + 5] = table->entries[entryNum].offset & 0xFF;
    }

    lReturn = A71_SetGpData(gpSize - dataToBeWrittenSize, dataToBeWritten, dataToBeWrittenSize);
    if (lReturn != HLSE_SW_OK) {
        return lReturn;
    }

    memcpy(&gMappingTable, table, sizeof(gMappingTable));
    gMappingTableRead = 1;

    return lReturn;
}

/**
 * Update an entry in  the General Purpose mapping Data Table in Gp Memory and to cached variable
 *
 * for internal use
 *
 * \param [in] entry IN: General Purpose Data table entry to update
 *
 * \retval ::HLSE_SW_OK Successfull execution
 */
static HLSE_RET_CODE UpdateGPDataTableEntry(HLSE_GP_DATA_TABLE_ENTRY* entry)
{
    HLSE_GP_DATA_TABLE table;
    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    U8 entryNum;

    lReturn = ReadGPDataTable(&table, 0);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    // find the entry according to the index
    for (entryNum = 0; entryNum < table.numOfEntries; ++entryNum) {
        if (table.entries[entryNum].index == entry->index && table.entries[entryNum].klass == entry->klass) {
            memcpy(&table.entries[entryNum], entry, sizeof(HLSE_GP_DATA_TABLE_ENTRY));
            break;
        }
    }

    lReturn = WriteGPDataTable(&table);

    return lReturn;
}

/**
 * Delete an entry in  the General Purpose mapping Data Table in Gp Memory and to cached variable
 *
 * for internal use
 *
 * \param [in] index index idntifier of object entry to delete
 * \param [in] objClass class identifier of object entry to delete
 *
 * \retval ::HLSE_SW_OK Successfull execution
 */
static HLSE_RET_CODE DeleteGPDataTableEntry(U8 index, U8 objClass)
{
    HLSE_GP_DATA_TABLE table;
    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    U8 entryNum;

    lReturn = ReadGPDataTable(&table, 0);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    // find the entry according to the index
    for (entryNum = 0; entryNum < table.numOfEntries; ++entryNum) {
        if (table.entries[entryNum].index == index && table.entries[entryNum].klass == objClass) {
            HLSE_GP_DATA_TABLE_ENTRY entry;
            memset(&entry, 0xFF, sizeof(entry));
            memcpy(&table.entries[entryNum], &entry, sizeof(HLSE_GP_DATA_TABLE_ENTRY));

            table.numOfEntries--;
            break;
        }
    }

    // sort the entries in ascending order of the offset
    SortTable(&table);

    lReturn = WriteGPDataTable(&table);

    return lReturn;
}

/// Get File Size base on TLV in data - for internal use
static U16 GetFileSize(U8* data)
{
    U16 dataLen = 6;

    // calculate the Tag's length
    U8 TagLen = 1;
    if ((data[1] & 0x1F) == 0x1F) {
        TagLen = 2;
        if (data[2] & 0x80) {
            TagLen = 3;
        }
    }

    if (data[TagLen] & 0x80){
        if (data[TagLen] == 0x81)
            dataLen = data[TagLen + 1] + 2 + TagLen;
        else {
            dataLen = data[TagLen + 1] * 256 + data[TagLen + 2] + 3 + TagLen;
        }
    }
    else
        dataLen = data[TagLen] + 1 + TagLen;

    return dataLen;
}

/**
* Get an Object's Offset and Length
*
* for internal use
*
* \param [in] index index idntifier of object entry
* \param [in] objClass class identifier of object entry
* \param [out] offset object's offset in bytes
* \param [out] length object's length in bytes - in case of indirect length in the table, read it from actual object
* \param [out] availLength - total available length
*
* \retval ::HLSE_SW_OK Successfull execution
*/
static HLSE_RET_CODE GetObjectOffsetAndLength(U8 index, U8 objClass, U16* offset, U16* length, U16* availLength)
{
    HLSE_GP_DATA_TABLE table;
    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    U8 entryNum;

    lReturn = ReadGPDataTable(&table, 0);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    // find the entry according to the index and klass
    for (entryNum = 0; entryNum < table.numOfEntries; ++entryNum) {
        if (table.entries[entryNum].index == index && table.entries[entryNum].klass == objClass) {
            *offset = table.entries[entryNum].offset;

            // length is known
            if (!(table.entries[entryNum].length & HLSE_INDIRECT_LENGTH)) {
                *length = table.entries[entryNum].length;
            }
            // INDIRECT length
            else if (table.entries[entryNum].length == HLSE_INDIRECT_LENGTH) {
                // get the length from the TLV data
                U8 header[6];

                lReturn = A71_GetGpData(table.entries[entryNum].offset, header, 6);
                if (lReturn != HLSE_SW_OK)
                    return lReturn;

                *length = GetFileSize(header);
                gMappingTable.entries[entryNum].length = HLSE_INDIRECT_LENGTH | *length;
            }
            else {
                *length = table.entries[entryNum].length & 0x7FFF;
            }

            // obtain total available length
            // 2 cases:
            // 1) in case of Certificate type - get the length up to the next object in the table
            // 2) in case of data object - get the length of the data object

            if (table.entries[entryNum].klass == HLSE_GET_LOGICAL_OBJECT_CLASS(HLSE_CERTIFICATE))
            {
                // Get the length up to the next object in the table
                if (entryNum < (table.numOfEntries - 1))
                {
                    *availLength = table.entries[entryNum + 1].offset - table.entries[entryNum].offset;
                }
                else // this is the last entry so we need to find the max length till beginning of GP table data
                {
                    *availLength = gGPSize - (HLSE_GP_DATA_TABLE_ENTRY_SIZE * HLSE_MAX_OBJECTS_IN_TABLE - 2) - table.entries[entryNum].offset;
                }
            }
            else // assume a data object
            {
                // length for Data object is always well known
                *availLength = table.entries[entryNum].length;
            }

            break;
        }
    }

    return lReturn;
}

/// for internal use - Get Object data for object identified by 'index' and 'objClass'
static HLSE_RET_CODE GetObjectData(U8 index, U8 objClass, U8* objData, U16* objDataLen)
{
    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    U16 length, offset;
    U16 availLength; // total available length

    lReturn = GetObjectOffsetAndLength(index, objClass, &offset, &length, &availLength);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    if (objData == NULL) {
        *objDataLen = length;
        return HLSE_SW_OK;
    }
    else if (*objDataLen < length) {
        *objDataLen = length;
        return HLSE_ERR_BUF_TOO_SMALL;
    }

    // if this is a certificate , get its actual length from the TLV header and return only the certificate's data, without trailing padding if any
    if (objClass == HLSE_GET_LOGICAL_OBJECT_CLASS(HLSE_CERTIFICATE))
    {
        // get the length from the TLV data
        U8 header[6];
        U16 certLength = 0;

        lReturn = A71_GetGpData(offset, header, 6);
        if (lReturn != HLSE_SW_OK) {
            return lReturn;
        }

        certLength = GetFileSize(header);
        if (certLength < length) {
            length = certLength;        // actual cert length without trailing padding
        }
    }

    *objDataLen = length;

    return A71_GetGpData(offset, objData, length);
}

/// for internal use - Get Direct Access Object data for object identified by 'index' and 'objClass'
static HLSE_RET_CODE DirectAccessGetObjectData(U8 index, U8 objClass, U16 internalOffset, U16 bytes, U8* objData, U16* objDataLen)
{
    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    U16 length, offset;
    U16 availLength; // total available length

    lReturn = GetObjectOffsetAndLength(index, objClass, &offset, &length, &availLength);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    // check that we can read as many bytes as requested
    if (internalOffset > length || (internalOffset + bytes) > length)
        return HLSE_ERR_MEMORY;

    if (objData == NULL) {
        *objDataLen = bytes;
        return HLSE_SW_OK;
    }
    else if (*objDataLen < bytes) {
        *objDataLen = bytes;
        return HLSE_ERR_BUF_TOO_SMALL;
    }

    *objDataLen = bytes;

    return A71_GetGpData(offset + internalOffset, objData, bytes);
}


/// for internal use - Update Object data for object identified by 'index' and 'objClass'
static HLSE_RET_CODE UpdateObjectData(U8 index, U8 objClass, U8* newData, U16 newDataLen)
{
    HLSE_GP_DATA_TABLE_ENTRY entry;

    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    U16 length, offset;
    U16 availLength; // total available length

    lReturn = GetObjectOffsetAndLength(index, objClass, &offset, &length, &availLength);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    // update is only permitted if enough memory is available to grow
    if (HLSE_ALIGN_SIZE(newDataLen) > HLSE_ALIGN_SIZE(availLength)) {
        return HLSE_ERR_MEMORY;
    }

    entry.index = index;
    entry.klass = objClass;
    entry.offset = offset;
    // leave the max length used so far in the table entry
    entry.length = (length > newDataLen ? length : newDataLen);

    lReturn = A71_SetGpDataWithLockCheck(offset, newData, newDataLen);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    return UpdateGPDataTableEntry(&entry);
}

/// for internal use - Update Object data for object identified by 'index' and 'objClass'
static HLSE_RET_CODE DirectAccessUpdateObjectData(U8 index, U8 objClass, U16 internalOffset, U8* newData, U16 newDataLen)
{
    HLSE_GP_DATA_TABLE_ENTRY entry;

    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    U16 length, offset;
    U16 availLength; // total available length

    lReturn = GetObjectOffsetAndLength(index, objClass, &offset, &length, &availLength);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    // update is only permitted within the same chunk number - if larger, delete the object and re-create it
    if (HLSE_ALIGN_SIZE(internalOffset + newDataLen) > HLSE_ALIGN_SIZE(length))
        return HLSE_ERR_MEMORY;

    entry.index = index;
    entry.klass = objClass;
    entry.offset = offset;
    entry.length = (internalOffset + newDataLen) > length ? (internalOffset + newDataLen) : length;

    lReturn = A71_SetGpData(offset + internalOffset, newData, newDataLen);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    return UpdateGPDataTableEntry(&entry);
}


/**
 * for internal use - Get the Offset For a New Object and add its entry to GP table
 * NOTE: allocation is done in chunks - one chunk can not be shared between objects!!!
 */
static HLSE_RET_CODE GetOffsetForNewObjectAndAddNewEntry(U16 newObjectSize, HLSE_GP_DATA_TABLE_ENTRY* entry)
{
    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    HLSE_GP_DATA_TABLE table;
    U16 gpSize;
    U16 newObjectSizeInChunks;
    U8 found = 0;
    U8 entryNum;
    U8 bEntryAlreadyExists = 0;

    lReturn = ReadGPDataTable(&table, 0);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    // Check if data object already exists - if so return an error
    for (entryNum = 0; entryNum < table.numOfEntries; ++entryNum) {
        if (table.entries[entryNum].index == entry->index && table.entries[entryNum].klass == entry->klass) {
            bEntryAlreadyExists = 1;
            break;
        }
    }
    if (bEntryAlreadyExists) {
        return HLSE_OBJ_ALREADY_EXISTS; // entry already exists
    }

    // resolve INDIRECT lengths
    for (entryNum = 0; entryNum < table.numOfEntries; ++entryNum) {
        if (table.entries[entryNum].index != 0xFF) {
            // INDIRECT length and no real length found yet
            if (table.entries[entryNum].length == HLSE_INDIRECT_LENGTH) {
                // get the length from the TLV data
                U8 header[6];
                U16 actualLength = 0;

                lReturn = A71_GetGpData(table.entries[entryNum].offset, header, 6);
                if (lReturn != HLSE_SW_OK)
                    return lReturn;

                actualLength = GetFileSize(header);
                table.entries[entryNum].length = HLSE_INDIRECT_LENGTH | actualLength;
            }
        }
    }

    lReturn = GetGPDataSize(&gpSize);
    if (lReturn != HLSE_SW_OK)
        return lReturn;

    // now find a unoccupied large enough offset for the new object, assuming the table is sorted in ascending order of offsets
    newObjectSizeInChunks = HLSE_ALIGN_SIZE(newObjectSize);

    entry->offset = 0;

    // invalid entries will have length = 0xFFFF so they will be at the end

    // check if it possible to insert the object at the begining of the GP Storage
    if (table.numOfEntries == 0 || (table.entries[0].offset / HLSE_GP_DATA_CHUNK) >= newObjectSizeInChunks) {
        entry->offset = 0;
        found = 1;
    }
    else {
        U8 leftEntry, rightEntry;
        for (leftEntry = 0; leftEntry < table.numOfEntries && !found; ++leftEntry) {
            if (table.entries[leftEntry].index != 0xFF && table.entries[leftEntry].klass != 0xFF) {
                // leftEntry is a valid entry
                rightEntry = leftEntry + 1;
                if (rightEntry < table.numOfEntries && table.entries[rightEntry].index != 0xFF && table.entries[rightEntry].klass != 0xFF) {
                    // rightEntry is a valid entry
                    U16 leftEnd = HLSE_ALIGN_SIZE(table.entries[leftEntry].offset + (table.entries[leftEntry].length & 0x7FFF));
                    if (((table.entries[rightEntry].offset / HLSE_GP_DATA_CHUNK) - leftEnd) >= newObjectSizeInChunks) {
                        entry->offset = leftEnd;
                        found = 1;
                    }
                }
             }
        }

        if (!found) {
            // check if there is space in the end

            // find the last valid entry
            S8 lastValidEntry = table.numOfEntries - 1;
            for (; lastValidEntry >= 0; --lastValidEntry) {
                if (table.entries[lastValidEntry].index != 0xFF)
                    break;
            }

            // no valid entries - check that the gpData is big enough (exclude last chunk for table)
            if (lastValidEntry == 0 && table.entries[lastValidEntry].index == 0xFF && (HLSE_ALIGN_SIZE(gpSize) - 1) >= newObjectSizeInChunks) {
                entry->offset = 0;
                found = 1;
            }
            else {
                U16 prevObjectEnd = HLSE_ALIGN_SIZE(table.entries[lastValidEntry].offset + (table.entries[lastValidEntry].length & 0x7FFF));
                if (((HLSE_ALIGN_SIZE(gpSize) - 1) - prevObjectEnd) >= newObjectSizeInChunks) {
                    entry->offset = prevObjectEnd;
                    found = 1;
                }
            }
        }
    }

    if (found) {
        entry->offset *= HLSE_GP_DATA_CHUNK;
    }
    else {
        entry->offset = 0;
        return HLSE_ERR_MEMORY;
    }

    // check if there is an empty entry
    found = 0;
    for (entryNum = 0; entryNum < table.numOfEntries; ++entryNum) {
        if (table.entries[entryNum].index == 0xFF) {
            memcpy(&table.entries[entryNum], entry, sizeof(HLSE_GP_DATA_TABLE_ENTRY));
            found = 1;
            break;
        }
    }

    if (!found) {
        table.numOfEntries++;
        if (table.numOfEntries > HLSE_MAX_OBJECTS_IN_TABLE) {
            return HLSE_ERR_IDENT_IDX_RANGE;
        }
        memcpy(&table.entries[table.numOfEntries - 1], entry, sizeof(HLSE_GP_DATA_TABLE_ENTRY));
    }

    // sort the entries in ascending order of the offset
    SortTable(&table);

    lReturn = WriteGPDataTable(&table);

    return lReturn;
}

//*******************************************************************
// Object Operations - defined in HLSEObjects.h
//*******************************************************************

HLSE_RET_CODE   HLSE_EnumerateObjects(HLSE_OBJECT_TYPE objectType, HLSE_OBJECT_HANDLE* objectHandles, U16* objectHandlesLen)
{
    HLSE_GP_DATA_TABLE gpDataTable;
    U8 credentials[1000] = {0};
    U16 credentialsSize = sizeof(credentials);
    U16 objCount = 0; // the Module always exists
    U8 fReadOnly = 0;

    U16 lReturn;
#ifndef HLSE_IGNORE_PARAM_CHECK
    //objectHandles is NULL, then all that the function does is return (in \p *objectHandlesLen) a number of HLSE_OBJECT_HANDLE which would suffice
    // to hold the returned list.  HLSE_SW_OK is returned by the function.

    //If objectHandles is not NULL, then objectHandlesLen must contain the number of handles in the buffer objectHandles
    if (objectHandles != NULL && objectHandlesLen == NULL) {
        return HLSE_ERR_API_ERROR;
    }

    if (objectHandlesLen == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif



    lReturn = A71_GetCredentialInfo(credentials, &credentialsSize);
    if (lReturn != HLSE_SW_OK) {
        return lReturn;
    }

    if (objectType == HLSE_MODULE) {
        objCount++;
    }
    else if (objectType != HLSE_CERTIFICATE && objectType != HLSE_DATA) {
        U16 i;

        if (objectType == HLSE_MODULE || objectType == HLSE_ANY_TYPE) {
            objCount++;
        }
        for (i = 1; i < credentialsSize; ++i) {
            if (credentials[i] & CREDENTIAL_INITIALIZED) {
                if (i >= CIT_KEYPAIR_OFFSET && i <= (CIT_PUBKEY_OFFSET -1) && (objectType == HLSE_KEY_PAIR || objectType == HLSE_ANY_TYPE))
                    objCount++;
                else if (i >= CIT_PUBKEY_OFFSET && i <= (CIT_CONFIGKEY_OFFSET-1) && (objectType == HLSE_PUBLIC_KEY || objectType == HLSE_ANY_TYPE))
                    objCount++;
                else if (i >= CIT_CONFIGKEY_OFFSET && i <= (CIT_SYMMETRICKEY_OFFSET-1) && (objectType == HLSE_CONFIG_KEY || objectType == HLSE_ANY_TYPE))
                    objCount++;
                else if (i >= CIT_SYMMETRICKEY_OFFSET && i <= (CIT_COUNTER_OFFSET-1) && (objectType == HLSE_SYMMETRIC_KEY || objectType == HLSE_ANY_TYPE))
                    objCount++;
                else if (i >= CIT_COUNTER_OFFSET && i <= (CIT_GP_OFFSET-1) && (objectType == HLSE_COUNTER || objectType == HLSE_ANY_TYPE))
                    objCount++;
            }
        }
    }

    // read Certificate/Data objects
    if (objectType == HLSE_CERTIFICATE || objectType == HLSE_DATA || objectType == HLSE_ANY_TYPE) {
        lReturn = ReadGPDataTable(&gpDataTable, 0);
        if (lReturn == HLSE_SW_OK) {
            U8 entryNum;
            for (entryNum = 0; entryNum < gpDataTable.numOfEntries; ++entryNum) {
                if (gpDataTable.entries[entryNum].index != 0xFF) {
                    if (objectType == HLSE_GET_LOGICAL_OBJECT_TYPE(gpDataTable.entries[entryNum].klass) || objectType == HLSE_ANY_TYPE) {
                        objCount++;
                    }
                }
            }
        }
    }

    if (objectHandles == NULL) {
        *objectHandlesLen = objCount;
        return HLSE_SW_OK;
    }
    if (objectHandles != NULL && objectHandlesLen != NULL && *objectHandlesLen < objCount) {
        *objectHandlesLen = objCount;
        return HLSE_ERR_BUF_TOO_SMALL;
    }

    *objectHandlesLen = 0;

    if (objectType == HLSE_MODULE || objectType == HLSE_ANY_TYPE) {
        objectHandles[(*objectHandlesLen)++] = HLSE_CREATE_HANDLE(1, HLSE_MODULE, 0);
    }

    if (objectType != HLSE_MODULE && objectType != HLSE_CERTIFICATE && objectType != HLSE_DATA) {
        U16 i;
        for (i = 1; i < credentialsSize; ++i) {
            if (credentials[i] & CREDENTIAL_INITIALIZED) {
                if (i >= CIT_KEYPAIR_OFFSET && i <= (CIT_PUBKEY_OFFSET - 1) && (objectType == HLSE_KEY_PAIR || objectType == HLSE_ANY_TYPE))
                    objectHandles[(*objectHandlesLen)++] = HLSE_CREATE_HANDLE(credentials[i] & CREDENTIAL_LOCKED, HLSE_KEY_PAIR, (i - CIT_KEYPAIR_OFFSET));
                else if (i >= CIT_PUBKEY_OFFSET && i <= (CIT_CONFIGKEY_OFFSET - 1) && (objectType == HLSE_PUBLIC_KEY || objectType == HLSE_ANY_TYPE))
                    objectHandles[(*objectHandlesLen)++] = HLSE_CREATE_HANDLE(credentials[i] & CREDENTIAL_LOCKED, HLSE_PUBLIC_KEY, (i - CIT_PUBKEY_OFFSET));
                else if (i >= CIT_CONFIGKEY_OFFSET && i <= (CIT_SYMMETRICKEY_OFFSET - 1) && (objectType == HLSE_CONFIG_KEY || objectType == HLSE_ANY_TYPE))
                    objectHandles[(*objectHandlesLen)++] = HLSE_CREATE_HANDLE(credentials[i] & CREDENTIAL_LOCKED, HLSE_CONFIG_KEY, (i - CIT_CONFIGKEY_OFFSET));
                else if (i >= CIT_SYMMETRICKEY_OFFSET && i <= (CIT_COUNTER_OFFSET - 1) && (objectType == HLSE_SYMMETRIC_KEY || objectType == HLSE_ANY_TYPE))
                    objectHandles[(*objectHandlesLen)++] = HLSE_CREATE_HANDLE(credentials[i] & CREDENTIAL_LOCKED, HLSE_SYMMETRIC_KEY, (i - CIT_SYMMETRICKEY_OFFSET));
                else if (i >= CIT_COUNTER_OFFSET && i <= (CIT_GP_OFFSET - 1) && (objectType == HLSE_COUNTER || objectType == HLSE_ANY_TYPE))
                    objectHandles[(*objectHandlesLen)++] = HLSE_CREATE_HANDLE(credentials[i] & CREDENTIAL_LOCKED, HLSE_COUNTER, (i - CIT_COUNTER_OFFSET));
            }
        }
    }

    // add Certificate objects
    if (objectType == HLSE_CERTIFICATE || objectType == HLSE_DATA || objectType == HLSE_ANY_TYPE) {
        U8 entryNum;
        for (entryNum = 0; entryNum < gpDataTable.numOfEntries; ++entryNum) {
            if (gpDataTable.entries[entryNum].index != 0xFF) {
                if (objectType == HLSE_GET_LOGICAL_OBJECT_TYPE(gpDataTable.entries[entryNum].klass) || objectType == HLSE_ANY_TYPE) {
                    fReadOnly = ((credentials[(gpDataTable.entries[entryNum].offset / HLSE_GP_DATA_CHUNK) + CIT_GP_OFFSET] & 0x0F) == CREDENTIAL_LOCKED ) ? 0x01 : 0x00;
                    objectHandles[(*objectHandlesLen)++] = HLSE_CREATE_HANDLE(fReadOnly,
                                                                              HLSE_GET_LOGICAL_OBJECT_TYPE(gpDataTable.entries[entryNum].klass),
                                                                              gpDataTable.entries[entryNum].index);
                }
            }
        }
    }

    return HLSE_SW_OK;
}

HLSE_RET_CODE   HLSE_SetObjectAttribute(HLSE_OBJECT_HANDLE hObject, HLSE_ATTRIBUTE* attribute)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if ((attribute == NULL) || (hObject == HLSE_ANY_TYPE)) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    //***************************
    // HLSE_KEY_PAIR
    //***************************
    if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_KEY_PAIR) {
        if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
            if (attribute->value == NULL) // generate
                return A71_GenerateEccKeyPair(HLSE_GET_OBJECT_INDEX(hObject));

            if (attribute->valueLen != 97)
                return HLSE_ERR_API_ERROR;
            return A71_SetEccKeyPair(HLSE_GET_OBJECT_INDEX(hObject), &((U8*)(attribute->value))[32], 65, (U8*)(attribute->value), 32);
        }
        else if (attribute->type == HLSE_ATTR_WRAPPED_OBJECT_VALUE) {
            if (attribute->valueLen != 105)
                return HLSE_ERR_API_ERROR;
            return A71_SetEccKeyPair(HLSE_GET_OBJECT_INDEX(hObject), &((U8*)(attribute->value))[40], 65, (U8*)(attribute->value), 40);
        }
        else
            return HLSE_ERR_API_ERROR;
    }
    //***************************
    // HLSE_PUBLIC_KEY
    //***************************
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_PUBLIC_KEY) {
        if (attribute->value == NULL || attribute->valueLen == 0)
            return HLSE_ERR_API_ERROR;

        if (attribute->type == HLSE_ATTR_OBJECT_VALUE)
            return A71_SetEccPublicKey(HLSE_GET_OBJECT_INDEX(hObject), (U8*)(attribute->value), attribute->valueLen);
        else
            return HLSE_ERR_API_ERROR;
    }
    //***************************
    // HLSE_SYMMETRIC_KEY
    //***************************
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_SYMMETRIC_KEY) {
        if (attribute->value == NULL)
            return HLSE_ERR_API_ERROR;

        if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
            if (attribute->valueLen == 0)
                return HLSE_ERR_API_ERROR;
            return A71_SetSymKey(HLSE_GET_OBJECT_INDEX(hObject), (U8*)(attribute->value), attribute->valueLen);
        }
        else if (attribute->type == HLSE_ATTR_WRAPPED_OBJECT_VALUE) {
            if (attribute->valueLen == 0)
                return HLSE_ERR_API_ERROR;
            return A71_SetRfc3394WrappedAesKey(HLSE_GET_OBJECT_INDEX(hObject), (U8*)(attribute->value), attribute->valueLen);
        }
        else
            return HLSE_ERR_API_ERROR;
    }
    //***************************
    // HLSE_CONFIG_KEY
    //***************************
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_CONFIG_KEY) {
        if (attribute->value == NULL)
            return HLSE_ERR_API_ERROR;

        if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
            if (attribute->valueLen == 0)
                return HLSE_ERR_API_ERROR;
            return A71_SetConfigKey(HLSE_GET_OBJECT_INDEX(hObject), (U8*)(attribute->value), attribute->valueLen);
        }
        else if (attribute->type == HLSE_ATTR_WRAPPED_OBJECT_VALUE) {
            if (attribute->valueLen == 0)
                return HLSE_ERR_API_ERROR;
            return A71_SetRfc3394WrappedConfigKey(HLSE_GET_OBJECT_INDEX(hObject), (U8*)(attribute->value), attribute->valueLen);
        }
        else
            return HLSE_ERR_API_ERROR;
    }
    //***************************
    // HLSE_COUNTER
    //***************************
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_COUNTER) {
        if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
            if (attribute->valueLen == 0 && attribute->value == NULL)
                return A71_IncrementCounter(HLSE_GET_OBJECT_INDEX(hObject));
            else {
                unsigned long val ;
                if (attribute->valueLen == 0 || attribute->value == NULL)
                    return HLSE_ERR_API_ERROR;
                val = *(U8*)(attribute->value);
                return A71_SetCounter(HLSE_GET_OBJECT_INDEX(hObject), val);
            }
        }
        else
            return HLSE_ERR_API_ERROR;
    }
    //***************************
    // HLSE_SM_KEY
    //***************************
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_SM_KEYS) {
        if (attribute->value == NULL)
            return HLSE_ERR_API_ERROR;

        if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
            HLSE_A71_SM_KEYS_DATA* data;
            if (attribute->valueLen != sizeof(HLSE_A71_SM_KEYS_DATA) || attribute->value == NULL)
                return HLSE_ERR_API_ERROR;

            data = (HLSE_A71_SM_KEYS_DATA*)(attribute->value);
            return SCP_GP_PutKeys(data->keyVersion, data->keyEnc, data->keyMac, data->keyDek, data->currentKeyDek, 16);
        }
        else
            return HLSE_ERR_API_ERROR;
    }
    //***************************
    // HLSE_CERTIFICATE/DATA
    //***************************
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_CERTIFICATE || HLSE_GET_OBJECT_TYPE(hObject) == HLSE_DATA) {
        if (attribute->value == NULL)
            return HLSE_ERR_API_ERROR;

        if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
            if (attribute->valueLen == 0 || attribute->value == NULL)
                return HLSE_ERR_API_ERROR;

            return UpdateObjectData(HLSE_GET_OBJECT_INDEX(hObject), HLSE_GET_LOGICAL_OBJECT_CLASS(hObject), (U8*)(attribute->value), attribute->valueLen);
        }
        else if (attribute->type == HLSE_ATTR_DIRECT_ACCESS_OBJECT_VALUE && HLSE_GET_OBJECT_TYPE(hObject) == HLSE_DATA) {
            HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE* params;
            if (attribute->value == NULL || attribute->valueLen != sizeof(HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE))
                return HLSE_ERR_API_ERROR;

            params = (HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE*)attribute->value;

            return DirectAccessUpdateObjectData(HLSE_GET_OBJECT_INDEX(hObject), HLSE_GET_LOGICAL_OBJECT_CLASS(hObject), params->offset, params->buffer, params->bufferLen);
        }
        else
            return HLSE_ERR_API_ERROR;
    }
    //***************************
    // HLSE_MODULE
    //***************************
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_MODULE) {
        if (attribute->value == NULL)
            return HLSE_ERR_API_ERROR;

        if (attribute->type == HLSE_ATTR_MODULE_TRANSPORT_LOCK_STATE) {
            HLSE_LIFE_CYCLE_STATE lockMode;
            if (attribute->value == NULL)
                return HLSE_ERR_API_ERROR;

            lockMode = *((HLSE_LIFE_CYCLE_STATE*)attribute->value);
            if (lockMode == HLSE_MODULE_LOCKED) {
                return A71_LockModule();
            }
            else if (lockMode == HLSE_MODULE_UNLOCKED) {
                HLSE_A71_MODULE_UNLOCK_PARAMS* params = ((HLSE_A71_MODULE_UNLOCK_PARAMS*)attribute->value);
                return A71_UnlockModule(params->response, sizeof(params->response));
            }
        }
        else if (attribute->type == HLSE_ATTR_MODULE_INJECTION_LOCK_STATE) {
            HLSE_LIFE_CYCLE_STATE lockMode ;
            if (attribute->valueLen != sizeof(HLSE_LIFE_CYCLE_STATE) || attribute->value == NULL)
                return HLSE_ERR_API_ERROR;

            lockMode = *((HLSE_LIFE_CYCLE_STATE*)attribute->value);
            if (lockMode == HLSE_INJECT_LOCKED) {
                return A71_InjectLock();
            }
        }
        else
            return HLSE_ERR_API_ERROR;
    }

    return HLSE_ERR_API_ERROR;
}

HLSE_RET_CODE   HLSE_GetObjectAttribute(HLSE_OBJECT_HANDLE hObject, HLSE_ATTRIBUTE* attribute)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    // allow only specific type
    if (hObject == HLSE_ANY_TYPE) {
        return HLSE_ERR_API_ERROR;
    }

    if (attribute == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    // { Check if user requets to obtain the length only
    if (attribute->value == NULL) {
        // return in attribute->valueLen a number of bytes which would suffice to hold the value to be returned
        if (attribute->type == HLSE_ATTR_OBJECT_TYPE) {
            attribute->valueLen = sizeof(HLSE_OBJECT_TYPE);
            return HLSE_SW_OK;
        }
        else if (attribute->type == HLSE_ATTR_OBJECT_INDEX) {
            attribute->valueLen = sizeof(HLSE_OBJECT_INDEX);
            return HLSE_SW_OK;
        }
        else if (attribute->type == HLSE_ATTR_READ_ONLY) {
            attribute->valueLen = 1;
            return HLSE_SW_OK;
        }
        else if ((attribute->type == HLSE_ATTR_MODULE_TOTAL_GP_SIZE) || (attribute->type == HLSE_ATTR_MODULE_APPLET_INFO)) {
            attribute->valueLen = sizeof(U16);
            return HLSE_SW_OK;
        }
        else if ((attribute->type == HLSE_ATTR_MODULE_TRANSPORT_LOCK_STATE) ||
                 (attribute->type == HLSE_ATTR_MODULE_SCP_LOCK_STATE) ||
                 (attribute->type == HLSE_ATTR_MODULE_INJECTION_LOCK_STATE) ||
                 (attribute->type == HLSE_ATTR_MODULE_RESTRICTED_KEYPAIR_INDEX))
        {
            attribute->valueLen = sizeof(U8);
            return HLSE_SW_OK;
        }
        else if (attribute->type == HLSE_ATTR_MODULE_UNIQUE_ID) {
            attribute->valueLen = A71CH_MODULE_UNIQUE_ID_LEN;
            return HLSE_SW_OK;
        }
        else if (attribute->type == HLSE_ATTR_MODULE_UNLOCK_CHALLENGE) {
            attribute->valueLen = A71CH_MODULE_UNLOCK_CHALLENGE_LEN;
            return HLSE_SW_OK;
        }
        else if ((attribute->type == HLSE_ATTR_MODULE_FREE_PERSISTENT_MEM) || (attribute->type == HLSE_ATTR_MODULE_FREE_TRANSIENT_MEM)) {
            attribute->valueLen = sizeof(short);
            return HLSE_SW_OK;
        }
        else if (attribute->type == HLSE_ATTR_MODULE_APPLET_NAME) {
            attribute->valueLen = 5;
            return HLSE_SW_OK;
        }
        else if (((HLSE_GET_OBJECT_TYPE(hObject) == HLSE_KEY_PAIR) || (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_PUBLIC_KEY)) && (attribute->type == HLSE_ATTR_OBJECT_VALUE))   {
            attribute->valueLen = A71CH_PUB_KEY_LEN; // at least 65 bytes
            return HLSE_SW_OK;
        }
        else if ((HLSE_GET_OBJECT_TYPE(hObject) == HLSE_COUNTER) && (attribute->type == HLSE_ATTR_OBJECT_VALUE)) {
            attribute->valueLen = sizeof(U32);
            return HLSE_SW_OK;
        }
        else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_CERTIFICATE || HLSE_GET_OBJECT_TYPE(hObject) == HLSE_DATA) {
            // length will be retrieved later on GetObjectData()
        }
        else {
            // type requested not found
            return HLSE_ERR_API_ERROR;
        }
    }
    // } end section obtaining only the length

    if (attribute->type == HLSE_ATTR_OBJECT_TYPE) {
        if (attribute->valueLen >= sizeof(HLSE_OBJECT_TYPE)) {
            HLSE_OBJECT_TYPE type = HLSE_GET_OBJECT_TYPE(hObject);
            memcpy(attribute->value, &type, sizeof(HLSE_OBJECT_TYPE));
            attribute->valueLen = sizeof(HLSE_OBJECT_TYPE);
            return HLSE_SW_OK;
        }
        else {
            return HLSE_ERR_BUF_TOO_SMALL;
        }
    }
    else if (attribute->type == HLSE_ATTR_OBJECT_INDEX) {
        if (attribute->valueLen >= sizeof(HLSE_OBJECT_INDEX)) {
            HLSE_OBJECT_INDEX index = HLSE_GET_OBJECT_FULL_INDEX(hObject);
            memcpy(attribute->value, &index, sizeof(HLSE_OBJECT_INDEX));
            attribute->valueLen = sizeof(HLSE_OBJECT_INDEX);
            return HLSE_SW_OK;
        }
        else {
            return HLSE_ERR_BUF_TOO_SMALL;
        }
    }
    else if (attribute->type == HLSE_ATTR_READ_ONLY) {
        if (attribute->valueLen >= sizeof(U8)) {
            U8 objState = ((hObject & 0x80000000) ? 1 : 0);
            memcpy(attribute->value, &objState, 1);
            attribute->valueLen = 1;
            return HLSE_SW_OK;
        }
        else {
            return HLSE_ERR_BUF_TOO_SMALL;
        }
    }
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_MODULE) {
        if ((attribute->type == HLSE_ATTR_MODULE_TOTAL_GP_SIZE && attribute->valueLen < sizeof(U16)) ||
            (attribute->type == HLSE_ATTR_MODULE_TRANSPORT_LOCK_STATE && attribute->valueLen < sizeof(U8)) ||
            (attribute->type == HLSE_ATTR_MODULE_SCP_LOCK_STATE && attribute->valueLen < sizeof(U8)) ||
            (attribute->type == HLSE_ATTR_MODULE_INJECTION_LOCK_STATE && attribute->valueLen < sizeof(U8)) ||
            (attribute->type == HLSE_ATTR_MODULE_APPLET_INFO && attribute->valueLen < sizeof(U16)))
        {
            return HLSE_ERR_BUF_TOO_SMALL;
        }

        if (attribute->type == HLSE_ATTR_MODULE_TOTAL_GP_SIZE  || attribute->type == HLSE_ATTR_MODULE_TRANSPORT_LOCK_STATE ||
            attribute->type == HLSE_ATTR_MODULE_SCP_LOCK_STATE || attribute->type == HLSE_ATTR_MODULE_INJECTION_LOCK_STATE ||
            attribute->type == HLSE_ATTR_MODULE_APPLET_INFO    || attribute->type == HLSE_ATTR_MODULE_RESTRICTED_KEYPAIR_INDEX )
        {
            U16 selectResponse = 0;
            U8 debugOn = 0;
            U8 restrictedKpIdx = 0;
            U8 transportLockState = 0;
            U8 scpState = 0;
            U8 injectLockState = 0;
            U16 gpStorageSize = 0;
            U16 lReturn ;
            attribute->valueLen = sizeof(S16);
            lReturn = A71_GetModuleInfo(&selectResponse, &debugOn, &restrictedKpIdx, &transportLockState, &scpState, &injectLockState, &gpStorageSize);

            if (lReturn != SW_OK)
                return lReturn;

            if (attribute->type == HLSE_ATTR_MODULE_TOTAL_GP_SIZE) {
                attribute->valueLen = sizeof(S16);
                memcpy(attribute->value, &gpStorageSize, sizeof(short));
                return HLSE_SW_OK;
            }
            else if (attribute->type == HLSE_ATTR_MODULE_TRANSPORT_LOCK_STATE) {
                attribute->valueLen = sizeof(U8);
                memcpy(attribute->value, &transportLockState, sizeof(U8));
                return HLSE_SW_OK;
            }
            else if (attribute->type == HLSE_ATTR_MODULE_SCP_LOCK_STATE) {
                attribute->valueLen = sizeof(U8);
                memcpy(attribute->value, &scpState, sizeof(U8));
                return HLSE_SW_OK;
            }
            else if (attribute->type == HLSE_ATTR_MODULE_INJECTION_LOCK_STATE) {
                attribute->valueLen = sizeof(U8);
                memcpy(attribute->value, &injectLockState, sizeof(U8));
                return HLSE_SW_OK;
            }
            else if (attribute->type == HLSE_ATTR_MODULE_APPLET_INFO) {
                attribute->valueLen = sizeof(U16);
                memcpy(attribute->value, &selectResponse, sizeof(U16));
                return HLSE_SW_OK;
            }
            else if (attribute->type == HLSE_ATTR_MODULE_RESTRICTED_KEYPAIR_INDEX) {
                attribute->valueLen = sizeof(U8);
                memcpy(attribute->value, &restrictedKpIdx, sizeof(U8));
                return HLSE_SW_OK;
            }
        }
        else
        {
            if (attribute->type == HLSE_ATTR_MODULE_UNIQUE_ID) {
                return A71_GetUniqueID(attribute->value, &(attribute->valueLen));
            }
            else if (attribute->type == HLSE_ATTR_MODULE_UNLOCK_CHALLENGE) {
                return A71_GetUnlockChallenge(attribute->value, &(attribute->valueLen));
            }
            else if (attribute->type == HLSE_ATTR_MODULE_RANDOM) {
                return A71_GetRandom(attribute->value, (U8)attribute->valueLen);
            }
            else if (attribute->type == HLSE_ATTR_MODULE_CREDENTIAL_INFO) {
                return A71_GetCredentialInfo(attribute->value, &attribute->valueLen);
            }
            else if (attribute->type == HLSE_ATTR_MODULE_FREE_PERSISTENT_MEM) {
                if (attribute->valueLen >= sizeof(short)) {
                    attribute->valueLen = sizeof(short);
                    return A71_DbgGetFreePersistentMemory((short*)attribute->value);
                }
                else {
                    return HLSE_ERR_BUF_TOO_SMALL;
                }
            }
            else if (attribute->type == HLSE_ATTR_MODULE_FREE_TRANSIENT_MEM) {
                if (attribute->valueLen >= sizeof(S16)) {
                    attribute->valueLen = sizeof(S16);
                    return A71_DbgGetFreeTransientMemory((S16*)attribute->value);
                }
                else {
                    return HLSE_ERR_BUF_TOO_SMALL;
                }
            }
            else if (attribute->type == HLSE_ATTR_MODULE_APPLET_NAME && attribute->valueLen >= 5) {
                U8 appletAID[] = { 0x61, 0x37, 0x31, 0x63, 0x68 };

                memcpy(attribute->value, appletAID, sizeof(appletAID));
                attribute->valueLen = 5;

                return HLSE_SW_OK;
            }
        }
        return HLSE_ERR_API_ERROR;
    }
    else {
        if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_KEY_PAIR) {
            if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
                return A71_GetPublicKeyEccKeyPair(HLSE_GET_OBJECT_INDEX(hObject), attribute->value, &(attribute->valueLen));
            }
        }
        else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_PUBLIC_KEY) {
            if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
                return A71_GetEccPublicKey(HLSE_GET_OBJECT_INDEX(hObject), attribute->value, &(attribute->valueLen));
            }
        }
        else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_COUNTER) {
            if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
                U32 counter = 0;
                A71_GetCounter(HLSE_GET_OBJECT_INDEX(hObject), &counter);
                memcpy(attribute->value, &counter, sizeof(U32));
                attribute->valueLen = sizeof(U32);
                return HLSE_SW_OK;
            }
        }
        else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_CERTIFICATE || HLSE_GET_OBJECT_TYPE(hObject) == HLSE_DATA) {
            if (attribute->type == HLSE_ATTR_OBJECT_VALUE) {
                return GetObjectData(HLSE_GET_OBJECT_INDEX(hObject), HLSE_GET_LOGICAL_OBJECT_CLASS(hObject), attribute->value, &attribute->valueLen);
            }
            else if (attribute->type == HLSE_ATTR_DIRECT_ACCESS_OBJECT_VALUE && HLSE_GET_OBJECT_TYPE(hObject) == HLSE_DATA) {
                HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE* params;
                if (attribute->value == NULL || attribute->valueLen != sizeof(HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE))
                    return HLSE_ERR_API_ERROR;

                params = (HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE*)attribute->value;

                return DirectAccessGetObjectData(HLSE_GET_OBJECT_INDEX(hObject), HLSE_GET_LOGICAL_OBJECT_CLASS(hObject), params->offset, params->bytes, params->buffer, &params->bufferLen);
            }
        }
    }

    return HLSE_ERR_API_ERROR;
}

HLSE_RET_CODE   HLSE_CreateObject(HLSE_ATTRIBUTE* attributes, U16 attributesNum, HLSE_OBJECT_HANDLE* hObject)
{
    // Get the object type and index and create the handle
    HLSE_OBJECT_TYPE objType = HLSE_ANY_TYPE;
    HLSE_OBJECT_INDEX objIndex = 0;
    U16 valAttrIndex = 0;
    U8 readOnly = 0;
    HLSE_RET_CODE lReturn = HLSE_SW_OK;
    HLSE_GP_DATA_TABLE_ENTRY entry;
    U16 i;
#ifndef HLSE_IGNORE_PARAM_CHECK
    // HLSE_ATTR_OBJECT_TYPE and HLSE_ATTR_OBJECT_INDEX must appear
    if (attributes == NULL || hObject == NULL || attributesNum < 2)
        return HLSE_ERR_API_ERROR;
#endif

    for (i = 0; i < attributesNum; ++i) {
        if (attributes[i].type == HLSE_ATTR_OBJECT_TYPE) {
            if (attributes[i].value == NULL || attributes[i].valueLen < 4)
                return HLSE_ERR_API_ERROR;

            objType = *(HLSE_OBJECT_TYPE*)(attributes[i].value);
        }
        else if (attributes[i].type == HLSE_ATTR_OBJECT_INDEX) {
            if (attributes[i].value == NULL || attributes[i].valueLen < 4)
                return HLSE_ERR_API_ERROR;

            objIndex = *(HLSE_OBJECT_INDEX*)(attributes[i].value);
        }
        else if (attributes[i].type == HLSE_ATTR_READ_ONLY) {
            if (attributes[i].value == NULL || attributes[i].valueLen < 1)
                return HLSE_ERR_API_ERROR;

            readOnly = *(U8*)(attributes[i].value);
        }
        else if (attributes[i].type == HLSE_ATTR_OBJECT_VALUE || attributes[i].type == HLSE_ATTR_WRAPPED_OBJECT_VALUE) {
            valAttrIndex = i;
        }
    }

    *hObject = HLSE_CREATE_HANDLE(readOnly, objType, objIndex);

    // Create the table entry and the object handle , and set the object value in GP storage
    if (objType == HLSE_CERTIFICATE || objType == HLSE_DATA) {
        entry.index = (U8)objIndex;
        entry.klass = HLSE_GET_LOGICAL_OBJECT_CLASS(objType);
        entry.length = attributes[valAttrIndex].valueLen;

        lReturn = GetOffsetForNewObjectAndAddNewEntry(attributes[valAttrIndex].valueLen, &entry);
        if (lReturn != HLSE_SW_OK) {
            return lReturn;
        }
    }

    lReturn = HLSE_SetObjectAttribute(*hObject, &attributes[valAttrIndex]);
    if (lReturn != HLSE_SW_OK) {
        return lReturn;
     }

    // if read only, freeze the data
    if (readOnly) {
        if (objType == HLSE_KEY_PAIR) {
            return A71_FreezeEccKeyPair((U8)objIndex);
        }
        else if (objType == HLSE_PUBLIC_KEY) {
            return A71_FreezeEccPublicKey((U8)objIndex);
        }
        else if (objType == HLSE_SYMMETRIC_KEY) {
            return A71_FreezeSymKey((U8)objIndex);
        }
        else if (objType == HLSE_CERTIFICATE || objType == HLSE_DATA) {
            U16 alignedLength = HLSE_ALIGN_SIZE(attributes[valAttrIndex].valueLen) * HLSE_GP_DATA_CHUNK;
            return A71_FreezeGpData(entry.offset, alignedLength);
        }
    }

    return lReturn;
}

HLSE_RET_CODE   HLSE_EraseObject(HLSE_OBJECT_HANDLE hObject)
{
    if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_KEY_PAIR) {
        return A71_EraseEccKeyPair(HLSE_GET_OBJECT_INDEX(hObject));
    }
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_PUBLIC_KEY) {
        return A71_EraseEccPublicKey(HLSE_GET_OBJECT_INDEX(hObject));
    }
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_SYMMETRIC_KEY) {
        return A71_EraseSymKey(HLSE_GET_OBJECT_INDEX(hObject));
    }
    else if ((HLSE_GET_OBJECT_TYPE(hObject) == HLSE_CERTIFICATE) || (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_DATA)) {
        if ((hObject & 0x80000000)) {
            // HLSE_ATTR_READ_ONLY - means it is frozen - cannot be deleted
            return HLSE_ERR_API_ERROR;
        }
        else {
            return DeleteGPDataTableEntry(HLSE_GET_OBJECT_INDEX(hObject), HLSE_GET_LOGICAL_OBJECT_CLASS(hObject));
        }
    }

    return HLSE_ERR_API_ERROR;
}

//*******************************************************************
// Cryptographic Operations - defined in HLSECrypto.h
//*******************************************************************

HLSE_RET_CODE   HLSE_GetSupportedMechanisms(HLSE_MECHANISM_TYPE* mechanism, U16* mechanismLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (mechanismLen == NULL)
        return HLSE_ERR_API_ERROR;
#endif

    if (mechanism == NULL) {
        *mechanismLen = 15;
        return HLSE_SW_OK;
    }
    if (mechanism != NULL && mechanismLen != NULL && *mechanismLen < 15) {
        *mechanismLen = 15;
        return HLSE_ERR_BUF_TOO_SMALL;
    }

    *mechanismLen = 15;

    *mechanism++ = HLSE_SHA1;
    *mechanism++ = HLSE_SHA256;
    *mechanism++ = HLSE_AES_CMAC;
    *mechanism++ = HLSE_TLS_1_2;
    *mechanism++ = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
    *mechanism++ = HLSE_HKDF_HMAC_SHA256_SKIP_EXTRACT;
    *mechanism++ = HLSE_HMAC_AES_SHA256;
    *mechanism++ = HLSE_RFC3394_AES_KEY_WRAPPING;
    *mechanism++ = HLSE_TLS_PSK_MASTER_KEY_DERIVE;
    *mechanism++ = HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC;
    *mechanism++ = HLSE_ECDH;
    *mechanism++ = HLSE_ECDSA_SIGN;
    *mechanism++ = HLSE_ECDSA_VERIFY;
    *mechanism++ = HLSE_TRNG;
    *mechanism++ = HLSE_SCP03_ALG;

    return HLSE_SW_OK;
}

HLSE_RET_CODE   HLSE_GetSupportedMechanismsForObject(HLSE_OBJECT_HANDLE hObject, HLSE_MECHANISM_TYPE* mechanism, U16* mechanismLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (mechanismLen == NULL)
        return HLSE_ERR_API_ERROR;
#endif

    if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_KEY_PAIR) {
        if (mechanism == NULL) {
            *mechanismLen = 4;
            return HLSE_SW_OK;
        }
        if (mechanism != NULL && mechanismLen != NULL && *mechanismLen < 4) {
            *mechanismLen = 4;
            return HLSE_ERR_BUF_TOO_SMALL;
        }

        *mechanismLen = 4;

        *mechanism++ = HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC;
        *mechanism++ = HLSE_ECDH;
        *mechanism++ = HLSE_ECDSA_SIGN;
        *mechanism++ = HLSE_ECDSA_VERIFY;
    }
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_PUBLIC_KEY) {
        if (mechanism == NULL) {
            *mechanismLen = 1;
            return HLSE_SW_OK;
        }
        if (mechanism != NULL && mechanismLen != NULL && *mechanismLen < 1) {
            *mechanismLen = 1;
            return HLSE_ERR_BUF_TOO_SMALL;
        }

        *mechanismLen = 1;

        *mechanism++ = HLSE_ECDSA_VERIFY;
    }
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_SYMMETRIC_KEY) {
        if (mechanism == NULL) {
            *mechanismLen = 4;
            return HLSE_SW_OK;
        }
        if (mechanism != NULL && mechanismLen != NULL && *mechanismLen < 4) {
            *mechanismLen = 4;
            return HLSE_ERR_BUF_TOO_SMALL;
        }

        *mechanismLen = 4;

        *mechanism++ = HLSE_AES_CMAC;
        *mechanism++ = HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND;
        *mechanism++ = HLSE_HKDF_HMAC_SHA256_SKIP_EXTRACT;
        *mechanism++ = HLSE_HMAC_AES_SHA256;
    }
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_CONFIG_KEY) {
        if (mechanism == NULL) {
            *mechanismLen = 1;
            return HLSE_SW_OK;
        }
        if (mechanism != NULL && mechanismLen != NULL && *mechanismLen < 1) {
            *mechanismLen = 1;
            return HLSE_ERR_BUF_TOO_SMALL;
        }

        *mechanismLen = 1;

        *mechanism++ = HLSE_RFC3394_AES_KEY_WRAPPING;
    }
    else if (HLSE_GET_OBJECT_TYPE(hObject) == HLSE_SM_KEYS) {
        if (mechanism == NULL) {
            *mechanismLen = 1;
            return HLSE_SW_OK;
        }
        if (mechanism != NULL && mechanismLen != NULL && *mechanismLen < 1) {
            *mechanismLen = 1;
            return HLSE_ERR_BUF_TOO_SMALL;
        }

        *mechanismLen = 1;

        *mechanism++ = HLSE_SCP03;
    }

    return HLSE_ERR_API_ERROR;
}

//HLSE_CONTEXT_HANDLE
HLSE_RET_CODE   HLSE_Digest(HLSE_MECHANISM_INFO* pMechanismType,
                            U8* inData, U16 inDataLen,
                            U8* outDigest, U16* outDigestLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if ((pMechanismType == NULL) || inData == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    // Check for a request for digest Len
    if (outDigest == NULL) {
        if (pMechanismType->mechanism == HLSE_SHA256) {
            *outDigestLen = 32;
            return HLSE_SW_OK;
        }
        else {
            return HLSE_ERR_API_ERROR;
        }
    }

    if (pMechanismType->mechanism == HLSE_SHA256) {
        return A71_GetSha256(inData, (U16)inDataLen, outDigest, outDigestLen);
    }

    return HLSE_ERR_API_ERROR;
}

//HLSE_RET_CODE   HLSE_DigestInit(HLSE_MECHANISM_INFO* pMechanismType, HLSE_CONTEXT_HANDLE* hContext)
//{
//
//}
//
//HLSE_RET_CODE   HLSE_DigestUpdate(HLSE_CONTEXT_HANDLE hContext, U8* inDataPart, U16 inDataPartLen)
//{
//
//}
//
//HLSE_RET_CODE   HLSE_DigestFinal(HLSE_CONTEXT_HANDLE hContext, U8* outDigest, U16* outDigestLen)
//{
//
//}

HLSE_RET_CODE   HLSE_Sign(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                          U8* inData, U16 inDataLen,
                          U8* outSignature, U16* outSignatureLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if ((pMechanismType == NULL) || (inData == NULL)) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    // Check for a request to get the required signature len
    if (outSignature == NULL) {
        if (pMechanismType->mechanism == HLSE_HMAC_AES_SHA256) {
            *outSignatureLen = 32;
            return HLSE_SW_OK;
        }
        else if (pMechanismType->mechanism == HLSE_ECDSA_SIGN) {
            *outSignatureLen = 128;
            return HLSE_SW_OK;
        }
        else if (pMechanismType->mechanism == HLSE_ECDSA_NORMALIZE_ASN_SIGN) {
            *outSignatureLen = 128;
            return HLSE_SW_OK;
        }
        else {
            return HLSE_ERR_API_ERROR;
        }
    }

    if (pMechanismType->mechanism == HLSE_HMAC_AES_SHA256)
    {
        HLSE_HKDF_PARAMS* params = (HLSE_HKDF_PARAMS*)pMechanismType->pParameter;
        return A71_GetHmacSha256(HLSE_GET_OBJECT_INDEX(hObject),
                                 params->nBlock,
                                 inData, (U16)inDataLen,
                                 outSignature, outSignatureLen);
    }
    else if (pMechanismType->mechanism == HLSE_ECDSA_SIGN)
    {
        return A71_EccSign(HLSE_GET_OBJECT_INDEX(hObject), inData, (U16)inDataLen, outSignature, outSignatureLen);
    }
    else if (pMechanismType->mechanism == HLSE_ECDSA_NORMALIZE_ASN_SIGN)
    {
        return A71_EccNormalizedAsnSign(HLSE_GET_OBJECT_INDEX(hObject), inData, (U16)inDataLen, outSignature, outSignatureLen);
    }

    return HLSE_ERR_API_ERROR;
}

HLSE_RET_CODE   HLSE_VerifySignature(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                                     U8* inData, U16 inDataLen,
                                     U8* inSignature, U16 inSignatureLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if ((pMechanismType == NULL) || (inData == NULL) || (inSignature == NULL)) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    if (pMechanismType->mechanism == HLSE_ECDSA_VERIFY) {
        U8 result = 0;
        HLSE_RET_CODE lReturn = 0;
        lReturn = A71_EccVerify(HLSE_GET_OBJECT_INDEX(hObject), inData, (U16)inDataLen, inSignature, inSignatureLen, &result);

        if (lReturn != HLSE_SW_OK)
            return lReturn;
        if (result == 0) // fail
            return HLSE_ERR_GENERAL_ERROR;

        return HLSE_SW_OK;
    }

    return HLSE_ERR_API_ERROR;
}

HLSE_RET_CODE   HLSE_VerifySignatureWithExternalKey(HLSE_MECHANISM_INFO* pMechanismType,
                                                    U8* inExtKey, U16 inExtKeyLen,
                                                    U8* inData, U16 inDataLen,
                                                    U8* inSignature, U16 inSignatureLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if ((pMechanismType == NULL) || (inData == NULL) || (inSignature == NULL)) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    if (pMechanismType->mechanism == HLSE_ECDSA_VERIFY) {
        U8 result = 0;
        HLSE_RET_CODE lReturn = 0;
        lReturn = A71_EccVerifyWithKey(inExtKey, (U16)inExtKeyLen, inData, (U16)inDataLen, inSignature, (U16)inSignatureLen, &result);

        if (lReturn != HLSE_SW_OK)
            return lReturn;
        if (result == 0) // fail
            return HLSE_ERR_GENERAL_ERROR;

        return HLSE_SW_OK;
    }

    return HLSE_ERR_API_ERROR;
}

// NOTE: all the data required for the key derivation (except for the key itself that is specified by hObject) is conveyed in the mechanism
HLSE_RET_CODE   HLSE_DeriveKey(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                               U8* outDerivedKey, U16* outDerivedKeyLen)
{
    // TODO/verify ulParameterLen is correct!!!

#ifndef HLSE_IGNORE_PARAM_CHECK
    if ((pMechanismType == NULL) || (outDerivedKeyLen == NULL)) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    // Check for a request to get outDerivedKeyLen
    if (outDerivedKey == NULL) {
        if (pMechanismType->mechanism == HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND) {
            *outDerivedKeyLen = 32;
            return HLSE_SW_OK;
        }
        else if (pMechanismType->mechanism == HLSE_HKDF_HMAC_SHA256_SKIP_EXTRACT) {
            *outDerivedKeyLen = DERIVE_KEYDATA_FROM_SHARED_SECRET_MAX_DERIVED_DATA; // 255
            return HLSE_SW_OK;
        }
        else if (pMechanismType->mechanism == HLSE_TLS_PSK_MASTER_KEY_DERIVE) {
            *outDerivedKeyLen = 48;
            return HLSE_SW_OK;
        }
        else if (pMechanismType->mechanism == HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC) {
            *outDerivedKeyLen = 48;
            return HLSE_SW_OK;
        }
        else if (pMechanismType->mechanism == HLSE_ECDH) {
            *outDerivedKeyLen = 256;
            return HLSE_SW_OK;
        }
        else {
            return HLSE_ERR_API_ERROR;
        }
    }



    if (pMechanismType->mechanism == HLSE_HKDF_HMAC_SHA256_EXTRACT_AND_EXPAND)
    {
        HLSE_HKDF_PARAMS* params = (HLSE_HKDF_PARAMS*)pMechanismType->pParameter;
        return A71_HkdfExpandSymKey(HLSE_GET_OBJECT_INDEX(hObject),
                                    params->nBlock,
                                    params->pInfo, (U16)params->ulInfoLen,
                                    outDerivedKey, *outDerivedKeyLen);
    }
    else if (pMechanismType->mechanism == HLSE_HKDF_HMAC_SHA256_SKIP_EXTRACT)
    {
        HLSE_HKDF_PARAMS* params = (HLSE_HKDF_PARAMS*)pMechanismType->pParameter;
        return A71_HkdfSymKey(HLSE_GET_OBJECT_INDEX(hObject),
                              params->nBlock,
                              params->pSalt, (U16)params->ulSaltLen,
                              params->pInfo, (U16)params->ulInfoLen,
                              outDerivedKey, *outDerivedKeyLen);
    }
    else if (pMechanismType->mechanism == HLSE_TLS_PSK_MASTER_KEY_DERIVE)
    {
        HLSE_TLS_PSK_MASTER_KEY_DERIVE_PARAMS* params = (HLSE_TLS_PSK_MASTER_KEY_DERIVE_PARAMS*)pMechanismType->pParameter;
        return A71_PskDeriveMasterSecret(HLSE_GET_OBJECT_INDEX(hObject),
                                         params->nBlock,
                                         params->pSeed, (U16)params->ulSeedLen, outDerivedKey);
    }
    else if (pMechanismType->mechanism == HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC)
    {
        HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC_PARAMS* params = (HLSE_TLS_PSK_MASTER_KEY_DERIVE_DH_ECC_PARAMS*)pMechanismType->pParameter;
        return A71_EcdhPskDeriveMasterSecret(HLSE_GET_OBJECT_INDEX(hObject),
                                             params->pPublicKey, (U16)params->ulPublicKeyLen,
                                             HLSE_GET_OBJECT_INDEX(params->symKeyHandle), params->numBlocks, params->pSeed, (U16)params->ulSeedLen, outDerivedKey);
    }
    else if (pMechanismType->mechanism == HLSE_ECDH) {
        HLSE_ECDH_PARAMS* params = (HLSE_ECDH_PARAMS*)pMechanismType->pParameter;
        return A71_EcdhGetSharedSecret(HLSE_GET_OBJECT_INDEX(hObject), params->pPublicKey, (U16)params->ulPublicKeyLen, outDerivedKey, outDerivedKeyLen);
    }

    return HLSE_ERR_API_ERROR;
}

HLSE_RET_CODE   HLSE_Encrypt(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                             U8* inData, U16 inDataLen,
                             U8* outData, U16* outDataLen)
{
    return HLSE_ERR_NOT_SUPPORTED;
}

HLSE_RET_CODE   HLSE_Decrypt(HLSE_MECHANISM_INFO* pMechanismType, HLSE_OBJECT_HANDLE hObject,
                             U8* inData, U16 inDataLen,
                             U8* outData, U16* outDataLen)
{
    return HLSE_ERR_NOT_SUPPORTED;
}

//CK_ECDH1_DERIVE_PARAMS params;
//CK_MECHANISM    mechanism = { CKM_ECDH1_DERIVE, &params, sizeof(params) };

//*******************************************************************
// Module Operations - defined in HLSEMisc.h
//*******************************************************************

// Debug functions
HLSE_RET_CODE   HLSE_DbgDisableDebug()
{
    return A71_DbgDisableDebug();
}

HLSE_RET_CODE   HLSE_DbgReflect(U8* inData, U16 inDataLen, U8* outData, U16* outDataLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if ((inData == NULL) || (outDataLen == NULL)) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    return A71_DbgReflect(inData, (U16)inDataLen, outData, outDataLen);
}

HLSE_RET_CODE   HLSE_DbgReset()
{
    // the cached gp table no lnger reflects the actual data in the GP storage and has to be re-read
    gMappingTableRead = 0;

    return A71_DbgReset();
}

//*******************************************************************
// Communication and Secure Channel - defined in HLSEComm.h
//*******************************************************************

HLSE_RET_CODE HLSE_CloseConnection(HLSE_CLOSE_CONNECTION_MODE mode)
{
    if ((mode != HLSE_CLOSE_CONNECTION_RESET) && (mode != HLSE_CLOSE_CONNECTION_NO_RESET)) {
        return HLSE_ERR_API_ERROR;
    }

    return SM_Close((U8)mode);
}

HLSE_RET_CODE HLSE_Connect(HLSE_CONNECTION_PARAMS* params, HLSE_COMMUNICATION_STATE *commState)
{
#if defined(RJCT_SOCKET)
        SmCommState_t a71SmCommState;
        U16 lReturn;
#ifndef HLSE_IGNORE_PARAM_CHECK
        if (params == NULL || commState == NULL || params->pParameter == NULL) {
            return HLSE_ERR_API_ERROR;
        }
#endif

        lReturn = SM_RjctConnect((const char *)params->pParameter, &a71SmCommState, commState->atr, &(commState->atrLen));
        if (lReturn != SW_OK) {
            return lReturn;
        }

        memcpy(commState, &a71SmCommState, sizeof(a71SmCommState));
        return HLSE_SW_OK;
#else
        SmCommState_t a71SmCommState;
        U16 lReturn;

#ifndef HLSE_IGNORE_PARAM_CHECK
        if (params == NULL || commState == NULL ) {
            return HLSE_ERR_API_ERROR;
        }
#endif

        lReturn = SM_Connect(&a71SmCommState, commState->atr, &(commState->atrLen));

        if (lReturn != SW_OK) {
            return lReturn;
        }

        memcpy(commState, &a71SmCommState, sizeof(a71SmCommState));
        return HLSE_SW_OK;
#endif
}

HLSE_RET_CODE HLSE_ResumeConnection(HLSE_COMMUNICATION_STATE *commState, HLSE_SECURE_CHANNEL_SESSION_STATE *smState)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (commState == NULL || smState == NULL ) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    if (smState->type != HLSE_SCP03 || smState->ulParameterLen != sizeof(HLSE_SCP03_SESSION_STATE))
        return HLSE_ERR_MEMORY;

    return SM_ResumeConnection((SmCommState_t *)commState, (Scp03SessionState_t*)(smState->pParameter));
}

HLSE_RET_CODE HLSE_SendAPDU(U8 *cmd, U16 cmdLen, U8 *resp, U16 *respLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (cmd == NULL || resp == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    return SM_SendAPDU(cmd, cmdLen, resp, respLen);
}

HLSE_RET_CODE HLSE_SCP_Subscribe(HLSE_SCP_SignalFunction callback, void *context)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (callback == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    return SCP_Subscribe((SCP_SignalFunction)callback, context);
}

HLSE_RET_CODE HLSE_SMChannelAuthenticate(HLSE_SECURE_CHANNEL_ESTABLISH_PARAMS* params, HLSE_SECURE_CHANNEL_STATE* channelState)
{
    HLSE_SECURE_CHANNEL_SCP03_ESTABLISH_PARAMS* scp03EstablishParams;
    HLSE_SCP03_CHANNEL_STATE* scp03ChannelState;
    U16 counterLen = 16;
    U16 lReturn;

#ifndef HLSE_IGNORE_PARAM_CHECK
    if (params == NULL || channelState == NULL) {
        return HLSE_ERR_API_ERROR;
    }

    if (params->type != HLSE_SCP03 || params->pParameter == NULL || params->ulParameterLen != sizeof(HLSE_SECURE_CHANNEL_SCP03_ESTABLISH_PARAMS))
        return HLSE_ERR_MEMORY;
    if (channelState->type != HLSE_SCP03 || channelState->pParameter == NULL || channelState->ulParameterLen != sizeof(HLSE_SCP03_CHANNEL_STATE))
        return HLSE_ERR_MEMORY;
#endif

    scp03EstablishParams = (HLSE_SECURE_CHANNEL_SCP03_ESTABLISH_PARAMS*)(params->pParameter);
    scp03ChannelState = (HLSE_SCP03_CHANNEL_STATE*)(channelState->pParameter);
    lReturn = SCP_Authenticate(scp03EstablishParams->keyEnc, scp03EstablishParams->keyMac, scp03EstablishParams->keyDek, 16, scp03ChannelState->cCounter, &counterLen);

    if (lReturn != SW_OK)
        return lReturn;

    channelState->ulParameterLen = counterLen;

    return HLSE_SW_OK;
}

HLSE_RET_CODE HLSE_SMChannelGetScpSessionState(HLSE_SECURE_CHANNEL_SESSION_STATE *channelState)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (channelState == NULL) {
        return HLSE_ERR_API_ERROR;
    }

    if (channelState->type != HLSE_SCP03 || channelState->pParameter == NULL || channelState->ulParameterLen != sizeof(HLSE_SCP03_SESSION_STATE))
        return HLSE_ERR_MEMORY;
#endif

    return SCP_GetScpSessionState((Scp03SessionState_t *)(channelState->pParameter));
}

HLSE_RET_CODE HLSE_SMChannelSetScpSessionState(HLSE_SECURE_CHANNEL_SESSION_STATE *channelState)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (channelState == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    if (channelState->type != HLSE_SCP03 || channelState->pParameter == NULL || channelState->ulParameterLen != sizeof(HLSE_SCP03_SESSION_STATE))
        return HLSE_ERR_MEMORY;

    SCP_SetScpSessionState((Scp03SessionState_t *)(channelState->pParameter));

    return HLSE_SW_OK;
}

//*******************************************************************
// Helper functions
//*******************************************************************

//HLSE_RET_CODE HLSE_WrapKey(inKeyData, wrappingKeyIndex, outData);

HLSE_RET_CODE   HLSE_NormalizeECCSignature(U8 *signature, U16 signatureLen, U8 *normalizedSignature, U16 *normalizedSignatureLen)
{
#ifndef HLSE_IGNORE_PARAM_CHECK
    if (signature == NULL || normalizedSignature == NULL || normalizedSignatureLen == NULL) {
        return HLSE_ERR_API_ERROR;
    }
#endif

    // prepare in/out parameters
    memcpy(normalizedSignature, signature, signatureLen);
    *normalizedSignatureLen = signatureLen;

    return hcNormalizeAsnSignatureEcc(normalizedSignature, normalizedSignatureLen);
}


//*******************************************************************
// For testing or Debug Use only
//*******************************************************************
HLSE_RET_CODE Debug_ForceReadGPDataTable()
{
    HLSE_GP_DATA_TABLE table;
    return ReadGPDataTable(&table, 1);
}
