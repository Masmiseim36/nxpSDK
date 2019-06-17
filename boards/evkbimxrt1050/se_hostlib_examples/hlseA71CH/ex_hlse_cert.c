/**
 * @file ex_hlse_cert.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2018 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * Example using Certificate objects
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "sm_printf.h"
#include "HLSEAPI.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "tst_sm_util.h"
#include "a71ch_util.h"

#include "HLSEAPI.h"
#include "tst_a71ch_util.h"
#include "tst_hlse_a71ch_util.h"

static U8 exDataObjUsage(U8 initMode);
static U8 exMixedCertsAndDataObj(U8 initMode, U16 nBaseSize, U16 nObj);

static U8 exCertUsageBasic(U8 initMode);
static U8 exCertUsageEnlarge(U8 initMode);
static U8 exCertUsageGpTableLengthUnknown(U8 initMode);

static U8 exCertUsageReadOnly(U8 initMode);
static U8 exCertEnumerate(void);
static U8 exCertGetAttr(void);
static U8 exCertSetAttr(void);

// internal utilities
static HLSE_RET_CODE patchCertificateInitialTL(U8 *clientCertDer, U16 clientCertDerLen);
static HLSE_RET_CODE getObject(HLSE_OBJECT_HANDLE handle, U8 *data, U16 *len);
static HLSE_RET_CODE setObject(HLSE_OBJECT_HANDLE handle, U8 *data, U16 len);

/**
 * Demonstrate usage of certificate objects
 */
U8 exHlseCert()
{
    U8 result = 1;
    PRINTF("\r\n-----------\r\nStart exHlseCert()\r\n------------\r\n");

    DEV_ClearChannelState();

    // Example of Data object usage
    result &= exDataObjUsage(INIT_MODE_RESET);

#if AX_EMBEDDED
    result &= exMixedCertsAndDataObj(INIT_MODE_RESET, 40 /* size */, 8 /* num of objects */);
#else
    result &= exMixedCertsAndDataObj(INIT_MODE_RESET, 300 /* size */, 8 /* num of objects */);
#endif

    // Example certificate usage with enhanced validations
    result &= exCertUsageBasic(INIT_MODE_RESET);

    // Example certificate usage enlarging and updating a certificate
    result &= exCertUsageEnlarge(INIT_MODE_RESET);

    // Example of Certificate creation / enumeration / Get / Set / Delete - when Read Only attribute used
    result &= exCertUsageReadOnly(INIT_MODE_RESET);

    // Example certificate usage when map contains indirect length (to be obtained from TLV of actual object)
    result &= exCertUsageGpTableLengthUnknown(INIT_MODE_RESET);

    // overall result
    PRINTF("\r\n-----------\r\nEnd exHlseCert(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}


/**
 * Demonstrate usage of certificate object:
 *
 * Simulate the following scenario and aim to verify it works correctly:
 *
 * - Create Certificate
 * - Read certificate data and verify correctness
 *      - Index ok
 *      - Value was correctly set
 * - Update Certificate contents
 * - Verify Certificate updated correctly
 * - Delete certificate
 * - Verify deletion
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exCertUsageBasic(U8 initMode)
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    HLSE_OBJECT_HANDLE certHandles[5];
    U16 certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);

    HLSE_OBJECT_INDEX index = 0;
    HLSE_OBJECT_TYPE objType = HLSE_CERTIFICATE;

    // Cert for this test is 50 bytes which occupies 2 chunks of 32 bytes on the GP Storage
    U8 certData[50];

    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;

    U8 data[50];        // to hold data to update certificate

    memset(certHandles, 0x00, sizeof(certHandles));
    memset(certData, 0xAA, sizeof(certData));

    PRINTF("\r\n-----------\r\nStart exCertUsageBasic(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);
    assert(result);

    // start with clean GP table to avoid cached data from previous test
    hlseRc = Debug_ForceReadGPDataTable();
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &index;
    attr[1].valueLen = sizeof(index);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = certData;
    attr[2].valueLen = sizeof(certData);

    // Create certificate object index = 0
    PRINTF("\r\nHLSE_CreateObject() - Create certificate object...\r\n------------\r\n");
    hlseRc = HLSE_CreateObject(attr, templateSize, &certHandles[0]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // assumption : correct slot in GP Table data was occupied
    // TODO: consider additional low level test for it

    // Now read certificate data and verify correctness
    // Find certificate where index=0

    // enumerate objects - we should have one certificate by now with index 0
    PRINTF("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 1, "err");

    // Get the attributes of the single certificate we have created
    // - Object Index
    // - Certificate data
    PRINTF("\r\nHLSE_GetObjectAttribute()...\r\n");
    {
        U32 certIndex = 0xFFFFFFFF;
        HLSE_ATTRIBUTE attrL;
        attrL.type = HLSE_ATTR_OBJECT_INDEX;
        attrL.value = &certIndex;
        attrL.valueLen = sizeof(certIndex);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        PRINTF("\tcert index = 0x%X\r\n", certIndex);
        result &= AX_CHECK_U16((U16)certIndex, 0x00, "err");
    }

    PRINTF("Verify Certificate contents...\r\n");
    {
        U8 readCertData[50];

        HLSE_ATTRIBUTE attrL;
        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readCertData;
        attrL.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("certOnA71CH", readCertData, sizeof(certData),
            "expected cert data", certData, sizeof(certData), AX_COLON_32);
    }

    PRINTF("Update Certificate contents...\r\n");
    {
        HLSE_ATTRIBUTE attrL;
        memset(data, 0xAB, sizeof(data));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = data;
        attrL.valueLen = sizeof(data);

        hlseRc = HLSE_SetObjectAttribute(certHandles[certHandlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }


    PRINTF("Verify Certificate Updated correctly...\r\n");
    {
        U8 readCertData[50];
        HLSE_ATTRIBUTE attrL;

        memset(readCertData, 0x00, sizeof(readCertData));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readCertData;
        attrL.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(certData),
            "expected cert data", data, sizeof(data), AX_COLON_32);
    }

    // Delete the certificate
    PRINTF("\r\nHLSE_EraseObject()...\r\n");
    hlseRc = HLSE_EraseObject(certHandles[certHandlesNum - 1]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // Verify it was actually deleted
    // enumerate objects - we should have no certificates by now
    PRINTF("\r\nVerify object was erased...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 0, "err");


    PRINTF("\r\n-----------\r\nEnd exCertUsageBasic(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}


/**
* Demonstrate usage of a data object:
*
* Simulate the following scenario and aim to verify it works correctly:
*
* - Create Data Object
* - Shows how to obtain the object GP offset
* - Read Data object and verify correctness
*      - Index ok
*      - Value was correctly set
* - Update Data Object contents
* - Verify Data Object updated correctly
*
* - Show direct update of partial block in Data Object
* - Show direct read of partial block in Data Object
* - Show direct Read / Write with offset that exceeds object boundaries is not allowed
* - Show direct Read / Write with length that exceeds object boundaries is not allowed
*
* - Delete Data Object
* - Verify deletion
*
* @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
*/
static U8 exDataObjUsage(U8 initMode)
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    HLSE_OBJECT_HANDLE handles[5];
    U16 handlesNum = sizeof(handles) / sizeof(HLSE_OBJECT_HANDLE);

    HLSE_OBJECT_INDEX index = 0;
    HLSE_OBJECT_TYPE objType = HLSE_DATA;

    // data for this test is 100 bytes which occupies 2 chunks of 32 bytes on the GP Storage
    U8 data[50];

    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;

    //U8 updateData[50];        // to hold data to update data object

    memset(handles, 0x00, sizeof(handles));
    memset(data, 0xAA, sizeof(data));

    PRINTF("\r\n-----------\r\nStart exDataObjUsage(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);
    assert(result);

    // start with clean GP table to avoid cached data from previous test
    hlseRc = Debug_ForceReadGPDataTable();
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &index;
    attr[1].valueLen = sizeof(index);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = data;
    attr[2].valueLen = sizeof(data);

    // Create data object index = 0
    PRINTF("\r\nHLSE_CreateObject() - Create Data object...\r\n------------\r\n");
    hlseRc = HLSE_CreateObject(attr, templateSize, &handles[0]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // assumption : correct slot in GP Table data was occupied
    // TODO: consider additional low level test for it

    // Now read data object and verify correctness
    // Find data object where index=0

    // enumerate objects - we should have one certificate by now with index 0
    PRINTF("\r\nHLSE_EnumerateObjects()...\r\n");
    handlesNum = sizeof(handles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_DATA, handles, &handlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(handlesNum, 1, "err");

    // show how to get the object's GP offset
    PRINTF("\r\nGet object GP offset...\r\n");
    {
        U16 offset;
        HLSE_ATTRIBUTE attrL;
        attrL.type = HLSE_ATTR_OBJECT_OFFSET;
        attrL.value = &offset;
        attrL.valueLen = sizeof(offset);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        PRINTF("\tdata object offset = 0x%u\r\n", offset);
    }

    // make sure our data object is the one we previously created
    // Get the attributes of the single data object we have
    PRINTF("\r\nHLSE_GetObjectAttribute()...\r\n");
    {
        U32 dataIndex = 0xFFFFFFFF;
        HLSE_ATTRIBUTE attrL;
        attrL.type = HLSE_ATTR_OBJECT_INDEX;
        attrL.value = &dataIndex;
        attrL.valueLen = sizeof(dataIndex);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        PRINTF("\tdata object index = 0x%X\r\n", dataIndex);
        result &= AX_CHECK_U16((U16)dataIndex, 0x00, "err");
    }

    PRINTF("Verify Data object contents...\r\n");
    {
        U8 readData[50];

        HLSE_ATTRIBUTE attrL;
        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readData;
        attrL.valueLen = sizeof(readData);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("dataOnA71CH", readData, sizeof(data),
            "expected data", data, sizeof(data), AX_COLON_32);
    }

    PRINTF("Update Data object contents...\r\n");
    {
        HLSE_ATTRIBUTE attrL;
        memset(data, 0xAB, sizeof(data));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = data;
        attrL.valueLen = sizeof(data);

        hlseRc = HLSE_SetObjectAttribute(handles[handlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    PRINTF("Verify Data object Updated correctly...\r\n");
    {
        U8 readData[50];
        HLSE_ATTRIBUTE attrL;

        memset(readData, 0x00, sizeof(readData));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readData;
        attrL.valueLen = sizeof(readData);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readData, sizeof(readData),
            "expected data", data, sizeof(data), AX_COLON_32);
    }

    PRINTF("Show usage of direct read / Update access to Data Object...\r\n");
    {
        HLSE_ATTRIBUTE attrL;
        HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE theValue;
        U8 buffer[50]; // a buffer to hold the read data
        U8 refBuffer2[50];
        memset(buffer, 0x00, sizeof(buffer));

        attrL.type = HLSE_ATTR_DIRECT_ACCESS_OBJECT_VALUE;

        PRINTF("    Verify Direct Read of Data Object...\r\n");
        theValue.offset = 0;
        theValue.bytes = 32;
        theValue.buffer = buffer;
        theValue.bufferLen = sizeof(buffer);

        attrL.value = &theValue;
        attrL.valueLen = sizeof(theValue);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("dataOnA71CH", buffer, theValue.bytes,
            "expected data", data, theValue.bytes, AX_COLON_32);

        // Now show usage of direct update to data object - update 10 bytes at offset 0 to 0x11
        memset(buffer, 0x11, 10);
        theValue.bytes = 10;
        theValue.buffer = buffer;

        hlseRc = HLSE_SetObjectAttribute(handles[handlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        PRINTF("    Verify Direct Update of Data Object...\r\n");
        memcpy(refBuffer2, buffer, 50);
        memset(buffer, 0x00, sizeof(buffer));

        theValue.bytes = 32;
        theValue.buffer = buffer;
        theValue.bufferLen = sizeof(buffer);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("dataOnA71CH", buffer, theValue.bytes,
            "expected data", refBuffer2, theValue.bytes, AX_COLON_32);

        PRINTF("    Test Direct Read / Write with offset that exceeds object boundaries...\r\n");
        {
            // Note : data object 0 is at offset 0 , size 50 bytes

            memset(buffer, 0x00, sizeof(buffer));

            theValue.offset = 100;
            theValue.bytes = 50;
            theValue.buffer = buffer;
            theValue.bufferLen = sizeof(buffer);

            // Read with offset exceeding object boundary - not allowed
            hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attrL);
            result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");

            // Write with offset exceeding object boundary - not allowed
            hlseRc = HLSE_SetObjectAttribute(handles[handlesNum - 1], &attrL);
            result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");
        }

        PRINTF("    Test Direct Read / Write with length that exceeds object boundaries...\r\n");
        {
            // Note : data object 0 is at offset 0 , size 50 bytes

            memset(buffer, 0x00, sizeof(buffer));

            theValue.offset = 30;
            theValue.bytes = 50;
            theValue.buffer = buffer;
            theValue.bufferLen = sizeof(buffer);

            // Read exceeding object boundary - not allowed
            hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attrL);
            result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");

            // Write exceeding object boundary - not allowed
            hlseRc = HLSE_SetObjectAttribute(handles[handlesNum - 1], &attrL);
            result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");
        }
    }


    // Delete the Data object
    PRINTF("\r\nHLSE_EraseObject()...\r\n");
    hlseRc = HLSE_EraseObject(handles[handlesNum - 1]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // Verify it was actually deleted
    // enumerate objects - we should have no certificates by now
    PRINTF("\r\nVerify object was erased...\r\n");
    handlesNum = sizeof(handles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_DATA, handles, &handlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(handlesNum, 0, "err");

    PRINTF("\r\n-----------\r\nEnd exDataObjUsage(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}


#define N_ADV_CERT         255
#define MAX_ADV_CERT_SIZE  (4096 - 32)

#define LEN_SHORT_FORM_MAX 127
#define LEN_LONG_FORM_ONE_BYTE_MAX 255
#define LEN_LONG_FORM_TWO_BYTE_MAX 65536
#define ERR_GP_NO_CERT 0x2000

/**
* An Example to show usage of mixed certificates and data objects
*
* Scenario:
*   - Create certificates and data objects
*   - Read back the certificate and data objects
*       - verify objects exists
*       - verify object's length matches
*       - verify objects's data matches
*   - Delete a random number of objects
*   - Read back the remaining certificate and data objects
*       - verify objects exists
*       - verify object's length matches
*       - verify objects's data matches
*   - Update an objects with new data
*   - Read back the remaining certificate and data objects
*       - verify objects exists
*       - verify object's length matches
*       - verify objects's data matches
*   - Delete all objects
*
* @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
* @param[in] nBaseSize base size of Cert/Data object to create
* @param[in] nObj number of total objects to create
*/
static U8 exMixedCertsAndDataObj(U8 initMode, U16 nBaseSize, U16 nObj)
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    HLSE_OBJECT_HANDLE mixedHandles[N_ADV_CERT];        // to hold handles of objects created

    HLSE_OBJECT_INDEX index = 0;
    HLSE_OBJECT_TYPE objType = HLSE_CERTIFICATE;

    U8 *refData[N_ADV_CERT];
    U16 effectiveSize[N_ADV_CERT];

    U8 *refDataU[N_ADV_CERT];
    U16 effectiveSizeU[N_ADV_CERT];

    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;

    U8 data[MAX_ADV_CERT_SIZE];        // to hold data to read certificate

    U8 nObjCreated = 0;  // certs and data objects
    U8 nObjToDelete = 0;
    U8 handleIndDeleted[N_ADV_CERT];   // to hold array ind of handle deleted
    U8 handleAlreadySelected = 0;

    int i, j;

    PRINTF("\r\n-----------\r\nStart exMixedCertsAndDataObjUsage(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);
    assert(result);

    // start with clean GP table to avoid cached data from previous test
    hlseRc = Debug_ForceReadGPDataTable();
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    memset(mixedHandles, 0x00, sizeof(mixedHandles));
    memset(handleIndDeleted, 0x00, sizeof(handleIndDeleted));

    // Create reference arrays containing random values which could serve as certificates or data objects
    srand(0);
    for (i = 0; i < nObj; i++)
    {
        effectiveSize[i] = (U16)(nBaseSize - (i % 24));
        refData[i] = (U8 *)malloc(effectiveSize[i]);
        for (j = 0; j < effectiveSize[i]; j++)
        {
            refData[i][j] = (U8)rand();
        }
    }

    // Create reference arrays containing random values which could serve as certificates or data objects, for updating the objects later
    for (i = 0; i < nObj; i++)
    {
        effectiveSizeU[i] = (U16)(nBaseSize - (i % 24));
        refDataU[i] = (U8 *)malloc(effectiveSizeU[i]);
        for (j = 0; j < effectiveSizeU[i]; j++)
        {
            refDataU[i][j] = (U8)rand();
        }
    }

    // Since the reference data arrays can be used as certificate we have to patch it to make sure it begins with a TLV of the certificate size,
    // as this determines the certificate value returned when we retrieve it
    for (i = 0; i < nObj; i++)
    {
        hlseRc = patchCertificateInitialTL(refData[i], effectiveSize[i]);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "Failed to patch initial TL of certificate");

        hlseRc = patchCertificateInitialTL(refDataU[i], effectiveSizeU[i]);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "Failed to patch initial TL of certificate");
    }

    PRINTF("\r\n-----------\r\nStart intMixedCertsAndDataObjUsage()\r\n------------\r\n");

    for (i = 0; i < nObj; i++)
    {
        index = (HLSE_OBJECT_INDEX)i;

        // create mixed data objects and certficiates objects
        objType = (i % 2 == 0) ? HLSE_CERTIFICATE : HLSE_DATA;

        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = refData[i];
        attr[2].valueLen = effectiveSize[i];

        // Create certificate object @ index
        PRINTF("\r\nHLSE_CreateObject(index=%lu, size=%d) - Create %s object...\r\n------------\r\n",
            index, attr[2].valueLen, (objType == HLSE_CERTIFICATE ? "Certificate" : "Data"));
        hlseRc = HLSE_CreateObject(attr, templateSize, &mixedHandles[i]);

        if ((hlseRc == HLSE_ERR_MEMORY) && (index > 4)) {
            // this is acceptable since we managed to create at least 5 certificates
            PRINTF("\r\nGp Memory filled up");
            break;
        }
        else if (hlseRc == HLSE_SW_OK)
        {
            nObjCreated++;
        }

        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }
    PRINTF("\r\nCreated %02d total objects ( mixed data and certificates objects )...", nObjCreated);

    PRINTF("\r\nNow read back certificates and data objects...");
    for (i = 0; i < nObjCreated; i++)
    {
        U16 dataLen = MAX_ADV_CERT_SIZE;
        U8 localScore = 1;

        hlseRc = getObject(mixedHandles[i], data, &dataLen);
        localScore &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Does the length match?
        localScore &= AX_CHECK_U16(dataLen, effectiveSize[i], "dataLen");

        // Does the data match?
        localScore &= AX_COMPARE_BYTE_ARRAY("data", data, dataLen, "refData", refData[i], effectiveSize[i], AX_COLON_16);

        if (localScore != 1) { PRINTF("\r\nFailed to retrieve certificate at index %d (expected size=%d)\r\n", i, effectiveSize[i]); }

        result &= localScore;
    }

    // Delete a random number of objects
    do {
        U8 nObjCreated_by2 = nObjCreated / 2;
        if ( nObjCreated_by2 != 0 ) {
            nObjToDelete = (U8)(rand() % (nObjCreated_by2));
        }
    } while (nObjToDelete == 0);

    PRINTF("\r\nAbout to delete %d objects...", nObjToDelete);
    for (i = 0; i < nObjToDelete; i++)
    {
        handleAlreadySelected = 0;
        // find an index to delete
        do {
            // index in mixedHandles array of handle to delete
            index = (U8)(rand() % nObjCreated);
            // verify this index was not already deleted
            if (handleIndDeleted[index] == 1) {
                // need to select another handle
                handleAlreadySelected = 1;
            }
            else {
                handleAlreadySelected = 0;
            }
        } while (handleAlreadySelected == 1);

        // set this index as a handle which was deleted
        handleIndDeleted[index] = 1;

        // Delete the object
        PRINTF("\r\nHLSE_EraseObject(),  handle=0x%X...\r\n", mixedHandles[index]);
        hlseRc = HLSE_EraseObject(mixedHandles[index]);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }


    // Verify the remaining objects are still ok
    PRINTF("\r\nNow read back remaining certificates and data objects...\r\n");
    for (i = 0; i < nObjCreated; i++)
    {
        U16 dataLen = MAX_ADV_CERT_SIZE;
        U8 localScore = 1;

        // ignore this handle if was deleted
        if (handleIndDeleted[i] != 0)
        {
            // skip this deleted object;
            continue;
        }

        hlseRc = getObject(mixedHandles[i], data, &dataLen);
        localScore &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Does the length match?
        localScore &= AX_CHECK_U16(dataLen, effectiveSize[i], "dataLen");

        // Does the certificate match?
        localScore &= AX_COMPARE_BYTE_ARRAY("data", data, dataLen, "refData", refData[i], effectiveSize[i], AX_COLON_16);

        if (localScore != 1) { PRINTF("\r\nFailed to retrieve certificate at index %d (expected size=%d)\r\n", i, effectiveSize[i]); }

        result &= localScore;
    }

    // Update remaining objects with the alternate reference data, and verify updated data succesfull
    PRINTF("\r\nNow update remaining certificates and data objects...\r\n");
    for (i = 0; i < nObjCreated; i++)
    {
        U16 dataLen;
        U8 localScore = 1;
        // ignore this handle if was deleted
        if (handleIndDeleted[i] != 0)
        {
            // skip this deleted object;
            continue;
        }


        // Set object with new data
        memcpy(data, refDataU[i], effectiveSizeU[i]);
        dataLen = effectiveSizeU[i];
        hlseRc = setObject(mixedHandles[i], data, dataLen);
        localScore &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Get updated data
        memset(data, 0, sizeof(data));
        dataLen = MAX_ADV_CERT_SIZE;
        hlseRc = getObject(mixedHandles[i], data, &dataLen);
        localScore &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Does the length match?
        localScore &= AX_CHECK_U16(dataLen, effectiveSizeU[i], "dataLen");

        // Does the data match?
        localScore &= AX_COMPARE_BYTE_ARRAY("data", data, dataLen, "refData", refDataU[i], effectiveSizeU[i], AX_COLON_16);


        if (localScore != 1) { PRINTF("\r\nFailed to retrieve certificate at index %d (expected size=%d)\r\n", i, effectiveSize[i]); }

        result &= localScore;
    }


    /*
    * Delete all objects
    */
    PRINTF("\r\nNow Delete all objects...\r\n");
    for (i = 0; i < nObjCreated; i++)
    {
        // ignore this handle if was deleted
        if (handleIndDeleted[i] != 0)
        {
            // skip this deleted object;
            continue;
        }

        // set this index as a handle which was deleted
        handleIndDeleted[i] = 1;

        // Delete the object
        PRINTF("\r\nHLSE_EraseObject(), handle=0x%X...\r\n", mixedHandles[i]);
        hlseRc = HLSE_EraseObject(mixedHandles[i]);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }


    for (i = 0; i < nObjCreated; i++)
    {
        free(refData[i]);
        free(refDataU[i]);
    }

    PRINTF("\r\n-----------\r\nEnd exMixedCertsAndDataObjUsage(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}



/**
 * Demonstrate usage of certificate objects:
 *
 * Simulate the following scenario and aim to verify it works correctly:
 *
 * - Create certificates
 * - Verify cannot create additional certificate
 * - Verify allows to enlarge a certificate with size within allocated boundary
 * - Verify cannot enlarge beyond the allocated boundary
 * - Delete the certificate
 * - Recreate it with bigger size
 * - Verify last certificate created still exists
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exCertUsageEnlarge(U8 initMode)
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    // num of certificates to try to create ( actually created depends on GP memory availability )
    U8 nCertsToCreate = 50;

    // Note at max of 254 objects could be stored in the Gp table
    // we allocate here space for one more in case we want to check that more that that is not possible
    HLSE_OBJECT_HANDLE certHandles[255 + 1];

    U8 nCertsCreated = 0;

    // to hold actual handles created
    //HLSE_OBJECT_HANDLE certHandle0;
    //HLSE_OBJECT_HANDLE certHandle1;
    HLSE_OBJECT_HANDLE certHandle2;
    HLSE_OBJECT_HANDLE certHandle3;
    //HLSE_OBJECT_HANDLE certHandle4;
    HLSE_OBJECT_HANDLE lastCertHandle;

    HLSE_OBJECT_INDEX index = 0;
    HLSE_OBJECT_TYPE objType = HLSE_CERTIFICATE;
    U8 indexCert;
    U16 certHandlesNum;

    // Cert for this test is 32 bytes which occupies 2 chunks of 32 bytes on the GP Storage
    U8 certData[50];

    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;

    U8 dataIn2Chunks[60];       // to hold data to update certificate - within 2 chunks boundary

    U8 dataIn3Chunks[90];       // to hold data to update certificate - within 3 chunks boundary

    memset(certHandles, 0x00, sizeof(certHandles));
    memset(certData, 0xAA, sizeof(certData));

    PRINTF("\r\n-----------\r\nStart exCertUsageEnlarge(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);
    assert(result);

    // start with clean GP table to avoid cached data from previous test
    hlseRc = Debug_ForceReadGPDataTable();
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &index;
    attr[1].valueLen = sizeof(index);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = certData;
    attr[2].valueLen = sizeof(certData);

    PRINTF("\r\nAbout to create up to  %02d certificates...", nCertsToCreate);
    for (indexCert = 0; indexCert < nCertsToCreate; indexCert++)
    {
        index = indexCert;
        memset(certData, (0xAA+index), sizeof(certData));

        PRINTF("\r\nHLSE_CreateObject() - Create certificate object(0x%02x)", index);
        hlseRc = HLSE_CreateObject(attr, templateSize, &certHandles[indexCert]);

        if ((hlseRc != HLSE_SW_OK) && (indexCert > 4)) {
            // this is acceptable since we managed to create 5 certificates
            break;
        }
        else if (hlseRc == HLSE_SW_OK )
        {
            nCertsCreated++;
        }
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }
    PRINTF("\r\nCreated %02d certificates...", nCertsCreated);

    // save the handles as the values in the array might be changed . not reflecting the actual handles , after deletion
    //certHandle0 = certHandles[0];
    //certHandle1 = certHandles[1];
    certHandle2 = certHandles[2];
    certHandle3 = certHandles[3];
    //certHandle4 = certHandles[4];
    lastCertHandle = certHandles[nCertsCreated - 1];

    // Enlarge certificate at index=2 within boundary
    PRINTF("Try to Enlarge Certificate contents within 2 chunks...\r\n");
    {
        HLSE_ATTRIBUTE attrL;
        memset(dataIn2Chunks, 0xBB, sizeof(dataIn2Chunks));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = dataIn2Chunks;
        attrL.valueLen = sizeof(dataIn2Chunks);

        hlseRc = HLSE_SetObjectAttribute(certHandle2, &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    PRINTF("Verify Certificate Updated correctly...\r\n");
    {
        U8 readCertData[60];
        HLSE_ATTRIBUTE attrL;

        memset(readCertData, 0x00, sizeof(readCertData));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readCertData;
        attrL.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandle2, &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(readCertData),
            "expected cert data", dataIn2Chunks, sizeof(dataIn2Chunks), AX_COLON_32);
    }

    // Enlarge certificate at index=2, over the boundary - not allowed - need to delete it and recreate it
    PRINTF("Try to Enlarge Certificate with 3 chunks data over the allocated size - not allowed...\r\n");
    {
        HLSE_ATTRIBUTE attrL;
        memset(dataIn3Chunks, 0xCC, sizeof(dataIn3Chunks));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = dataIn3Chunks;
        attrL.valueLen = sizeof(dataIn3Chunks);

        hlseRc = HLSE_SetObjectAttribute(certHandle2, &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");
    }

    PRINTF("Verify Certificate contents has not changed...\r\n");
    {
        U8 readCertData[60];
        HLSE_ATTRIBUTE attrL;

        memset(readCertData, 0x00, sizeof(readCertData));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readCertData;
        attrL.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandle2, &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(readCertData),
            "expected cert data", dataIn2Chunks, sizeof(dataIn2Chunks), AX_COLON_32);
    }

    // enumerate objects - we should have nCertsCreated objects
    memset(certHandles, 0, sizeof(certHandles));
    PRINTF("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, nCertsCreated, "err");

    // Need to delete the object and recreate it
    PRINTF("\r\nHLSE_EraseObject()...\r\n");
    hlseRc = HLSE_EraseObject(certHandle2);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    nCertsCreated--;

    // enumerate objects - we should have nCertsCreated objects
    memset(certHandles, 0, sizeof(certHandles));
    PRINTF("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, nCertsCreated, "err");
    //

    // recreate it - should now succeed
    index = 2;
    // set the reference data that in cert 3 that is expected not to change
    memset(dataIn2Chunks, (0xBB + index), sizeof(certData));
    {
        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = dataIn2Chunks;
        attr[2].valueLen = sizeof(dataIn2Chunks);

        PRINTF("\r\nHLSE_CreateObject() - Create certificate object(0x%02x)\r\n", index);
        hlseRc = HLSE_CreateObject(attr, templateSize, &certHandle2);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
        nCertsCreated++;
    }

    PRINTF("Verify Certificate Updated correctly...\r\n");
    {
        U8 readCertData[60];
        HLSE_ATTRIBUTE attrL;

        memset(readCertData, 0x00, sizeof(readCertData));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readCertData;
        attrL.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandle2, &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(readCertData),
            "expected cert data", dataIn2Chunks, sizeof(dataIn2Chunks), AX_COLON_32);
    }


    PRINTF("Verify Cert 4 still exists\r\n");
    {
        U8 readCertData[90];
        HLSE_ATTRIBUTE attrL;
        index = 3;

        memset(readCertData, 0x00, sizeof(readCertData));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readCertData;
        attrL.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandle3 /*certHandles[index]*/, &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // set the reference data that in cert 3 that is expected not to change
        memset(certData, (0xAA + index), sizeof(certData));

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(certData),
            "expected cert data", certData, sizeof(certData), AX_COLON_32);
    }
    //

    // note : last certificate depends on max certifcate which are allowed at Gp table
    PRINTF("Verify last Certificate created in table still exists\r\n");
    {
        U8 readCertData[90];
        HLSE_ATTRIBUTE attrL;
        index = nCertsCreated - 1;

        memset(readCertData, 0x00, sizeof(readCertData));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readCertData;
        attrL.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(lastCertHandle , &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // set the reference data that in lasr cert that is expected not to change
        memset(certData, (0xAA + index), sizeof(certData));

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(certData),
            "expected cert data", certData, sizeof(certData), AX_COLON_32);
    }

    // enumerate objects - we should have certHandlesNum objects
    memset(certHandles, 0, sizeof(certHandles));
    PRINTF("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, nCertsCreated, "err");

    // delete all certs
    for (indexCert = 0; indexCert < certHandlesNum; indexCert++)
    {
        index = indexCert;

        PRINTF("\r\nHLSE_EraseObject() - index 0x%02x", index);
        hlseRc = HLSE_EraseObject(certHandles[index]);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    PRINTF("\r\n-----------\r\nEnd exCertUsageEnlarge(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}


/**
 * Demonstrate usage of certificate objects:
 *
 * this example Verifies correct operation in case when length of certificate object was Unknown at the
 * time the mapping table was created
 *
 * Detailed explanation :
 *
 * - The abstraction for various objects that reside in the GP Storage area is achieved by maintaining a
 *   lookup table (mapping) at the end of the GP Storage area to holds information about the logical objects that exist
 *   in the GP Storage
 *
 * - The gp table is rewritten(whole) each time an object is inserted or deleted
 * - Each object is aligned to occupy at least the amount of memory on the boundary of the GP storage, that is 32 bytes.
 *   E.g. an object of 17 bytes will occupy 32 bytes
 *
 * In cases where the length of an object is not known at the time the table entry is set, the MSBit (0x8000) will be set
 * in the length as an indicator for the host library to know that the data is in TLV format and that the actual length
 * can be obtained by reading the first bytes of the object's data.
 *
 * Example pre setup required:
 *
 * - We want to have a single certificate at offset 0 in the GP storage
 * - we want to have a mapping table with one entry in which the length of the certificate object is unknown.
 *
 * Simulate the following scenario to verify it works correctly:
 *
 * - Create A Mapping table with one entry with length unknown as follows:
 *
 * \verbatim
 *
 *      Notes:
 *          X+1 is the address of the last byte of the GP Storage.
 *          N is the object number from 1 to N
 *
 *      Address     Value
 *      -------     ----------------------
 *      X-1*6+0     First Object Class      - 1 byte    value : 0
 *      X-1*6+1     First Object Index      - 1 byte    value : 0
 *      X-1*6+2     First Object Length MSB - 1 byte    value : 0x80    ( 0x8000 the MSBit Indicates Indirect length)
 *      X-1*6+3     First Object Length LSB - 1 byte    value : 0x00
 *      X-1*6+4     First Object Offset MSB - 1 byte    value : 0
 *      X-1*6+5     First Object Offset LSB - 1 byte    value : 0
 *      X           Update Counter          - 1 byte    value : 1
 *      X+1         Number of table entries - 1 byte    value : 1
 *      End of GP Storage
 *
 * \endverbatim
 *
 * - Write simulated certificate object at offset 0 of gp storage
 *      use TLV length encoded length:
 *          - 2 bytes 0x81, 0x..
 *          - 3 bytes 0x82, 0x00, 0x..
 *
 * - Read certificate data and verify correctness
 *      - index ok
 *      - value was correctly set
 * - Update Certificate contents
 * - Verify Certificate Updated correctly
 * - Delete certificate
 * - Verify deletion
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exCertUsageGpTableLengthUnknown(U8 initMode)
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    HLSE_OBJECT_HANDLE certHandles[5];

    U16 certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);

    HLSE_OBJECT_INDEX index = 0;
    HLSE_OBJECT_TYPE objType = HLSE_CERTIFICATE;

    // Cert for this test is 50 bytes which occupies 2 chunks of 32 bytes on the GP Storage
    U8 certData[50];

    HLSE_ATTRIBUTE attr[3];
    unsigned short templateSize = 3;

    U8 data[50];        // to hold data to update certificate

    memset(certHandles, 0x00, sizeof(certHandles));
    memset(certData, 0xAA, sizeof(certData));

    // Set header data - to be able to simulate length is unknown in the GP table at time Gp table was created -
    // so it will be retrieved from this header
    certData[0] = 0x30;     // tag
    certData[1] = 0x81;     // length in next byte
    certData[2] = 0x2F;     // length is 47 bytes

    PRINTF("\r\n-----------\r\nStart exCertUsageGpTableLengthUnknown(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);
    assert(result);

    // start with clean GP table to avoid cached data from previous test
    hlseRc = Debug_ForceReadGPDataTable();
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &index;
    attr[1].valueLen = sizeof(index);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = certData;
    attr[2].valueLen = sizeof(certData);

    // Create certificate object index = 0
    PRINTF("\r\nHLSE_CreateObject() - Create certificate object, use 2 bytes encoded length in header...\r\n------------\r\n");
    hlseRc = HLSE_CreateObject(attr, templateSize, &certHandles[0]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // We now have a mapping table, starting at the beginning of the last chunk of the GP storage (order is backward) which consists of 32 bytes composed of:
    //
    // Start of last chunk of GP storage :
    // Address Offset                 Value
    // -------  ------------------    ----------------------
    // ...  (note invalid entries are filled with 0xff)
    // ...      ...                   ...
    // 4088 [0x00000018]          0x09 class               certificate 1
    // 4089 [0x00000019]          0x00 index
    // 4090 [0x0000001a]          0x00 Length MSB Byte
    // 4091 [0x0000001b]          0x32 Length LSB Byte     means size is 50 bytes = sizeof(certData)
    // 4092 [0x0000001c]          0x00 Offset MSB Byte     offset 0
    // 4093 [0x0000001d]          0x00 Offset LSB Byte
    // 4094 [0x0000001e]          0x01 update counter
    // 4095 [0x0000001f]          0x01 number of entries    (Note it is in the last byte of the GP storage)
    // End of Gp Storage

    // We'll now overrun this map to contain 0x8000 in the length to indicate indirect length
    PRINTF("\r\noverwrite gp table map to have indirect length indication...\r\n");
    {
        U16 gpSize;
        //HLSE_RET_CODE lReturn = HLSE_SW_OK;
        U8 dataL[32];
        U8 dataSize = sizeof(dataL);

        hlseRc = hlse_GetGPDataSize(&gpSize);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Read the entire table - consists of one chunk 5 entries
        hlseRc = A71_GetGpData(gpSize - 32, dataL, 32);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Now overwrite Gp Table with indirect length
        dataL[dataSize - 2 - 1*6 + 2] = 0x80;
        dataL[dataSize - 2 - 1*6 + 3] = 0x00;

//      dataL[2] = 0x80;
//      dataL[3] = 0x00;

#if 0   // just for debug !!
        //  data format to check
        U8 altData[32] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,            // invalid entry
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,            // invalid entry
            0x09, 0x02, 0x80, 0x00, 0x06, 0x00,         // third certificate
            0x09, 0x01, 0x80, 0x00, 0x02, 0x00,         // second certificate
            0x09, 0x00, 0x80, 0x00, 0x00, 0x00,         // first certificate
            0x00, 0x03                                                       // update counter + num of entries
        };
        memcpy(dataL, altData, 32);
#endif

        hlseRc = A71_SetGpData(gpSize - 32, dataL, 32);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        hlseRc = Debug_ForceReadGPDataTable();
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }


    // enumerate objects - we should have one certificate by now with index 0
    PRINTF("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 1, "err");

    // make sure our certificate is the one we previously created
    // Get the attributes of the single certificate we have
    PRINTF("\r\nHLSE_GetObjectAttribute()...\r\n");
    {
        U32 certIndex = 0xFFFFFFFF;
        HLSE_ATTRIBUTE attrL;
        attrL.type = HLSE_ATTR_OBJECT_INDEX;
        attrL.value = &certIndex;
        attrL.valueLen = sizeof(certIndex);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        PRINTF("\tcert index = 0x%X\r\n", certIndex);
        result &= AX_CHECK_U16((U16)certIndex, 0x00, "err");
    }

    PRINTF("Verify Certificate contents...\r\n");
    {
        U8 readCertData[50];

        HLSE_ATTRIBUTE attrL;
        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readCertData;
        attrL.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("certOnA71CH", readCertData, sizeof(certData),
            "expected cert data", certData, sizeof(certData), AX_COLON_32);
    }

    PRINTF("Update Certificate contents , use 3 bytes encoded length in header...\r\n");
    {
        HLSE_ATTRIBUTE attrL;
        memset(data, 0xAB, sizeof(data));

        // Now use 3 bytes indirect length

        // Set header data - to be able to simulate length is unknown in the GP table at time Gp table was created -
        // so it will be retrieved from this header
        data[0] = 0x06;     // tag
        data[1] = 0x82;     // length in next 2 bytes
        data[2] = 0x00;     // MSB 0
        data[3] = 0x2E;     // length is 46 bytes

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = data;
        attrL.valueLen = sizeof(data);

        hlseRc = HLSE_SetObjectAttribute(certHandles[certHandlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    // We'll now overrun this map to contain 0x8000 in the length to indicate indirect length
    {
        U16 gpSize;
        //HLSE_RET_CODE lReturn = HLSE_SW_OK;
        U8 dataL[32];

        hlseRc = hlse_GetGPDataSize(&gpSize);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Read the entire table - consists of one chunk 5 entries
        hlseRc = A71_GetGpData(gpSize - 32, dataL, 32);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Now overrite with indirect length
        dataL[2] = 0x80;
        dataL[3] = 0x00;

        hlseRc = A71_SetGpData(gpSize - 32, dataL, 32);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        hlseRc = Debug_ForceReadGPDataTable();
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    PRINTF("Verify Certificate Updated correctly...\r\n");
    {
        U8 readCertData[50];
        HLSE_ATTRIBUTE attrL;

        memset(readCertData, 0x00, sizeof(readCertData));

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = readCertData;
        attrL.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attrL);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(certData),
            "expected cert data", data, sizeof(data), AX_COLON_32);
    }


    // Delete the certificate
    PRINTF("\r\nHLSE_EraseObject()...\r\n");
    hlseRc = HLSE_EraseObject(certHandles[certHandlesNum - 1]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // Verify it was actually deleted
    // enumerate objects - we should have no certificates by now
    PRINTF("\r\nVerify object was erased...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 0, "err");


    PRINTF("\r\n-----------\r\nEnd exCertUsageGpTableLengthUnknown(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}


/**
 * Demonstrate
 *
 *  - Creating 3 read only certificates
 *  - Verify erasure of existing cert is disallowed since it's Read Only
 *  - Verify recreation of existing cert is disallowed since it's Read Only
 *  - Certificate Enumeration
 *  - Get Certificate value
 *  - Verify unable to update certificate value since it's Read Only
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exCertUsageReadOnly(U8 initMode)
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    HLSE_OBJECT_HANDLE objHandle, objHandle2, objHandle3, objHandle4;

    HLSE_OBJECT_INDEX index = 1;
    HLSE_OBJECT_TYPE objType = HLSE_CERTIFICATE;

    U8 readOnly = 1;

    U8 certData[50];
    HLSE_ATTRIBUTE attr[4];
    unsigned short templateSize = 4;

    U8 largeCertData[300];

    memset(certData, 0xAA, sizeof(certData));
    memset(largeCertData, 0xAC, sizeof(largeCertData));

    PRINTF("\r\n-----------\r\nStart exCertUsageReadOnly(%s)\r\n------------\r\n", getInitModeAsString(initMode));

    // Initialize the A71CH (Debug mode restrictions may apply)
    result &= hlse_a71chInitModule(initMode);
    assert(result);

    // start with clean GP table to avoid cached data from previous test
    hlseRc = Debug_ForceReadGPDataTable();
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    attr[0].type = HLSE_ATTR_OBJECT_TYPE;
    attr[0].value = &objType;
    attr[0].valueLen = sizeof(objType);
    attr[1].type = HLSE_ATTR_OBJECT_INDEX;
    attr[1].value = &index;
    attr[1].valueLen = sizeof(index);
    attr[2].type = HLSE_ATTR_OBJECT_VALUE;
    attr[2].value = certData;
    attr[2].valueLen = sizeof(certData);
    attr[3].type = HLSE_ATTR_READ_ONLY;     // meaning further modification of the GP storage area is disallowed
    attr[3].value = &readOnly;
    attr[3].valueLen = sizeof(readOnly);


    // Create certificate objects index 1 to 3 - with HLSE_ATTR_READ_ONLY

    hlseRc = HLSE_CreateObject(attr, templateSize, &objHandle);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    index = 2;
    hlseRc = HLSE_CreateObject(attr, templateSize, &objHandle2);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // Create large certificate - READ ONLY
    index = 3;
    attr[2].value = largeCertData;
    attr[2].valueLen = sizeof(largeCertData);
    hlseRc = HLSE_CreateObject(attr, templateSize, &objHandle3);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // Create large certificate
    index = 4;
    hlseRc = HLSE_CreateObject(attr, 3 /* without READ ONLY */, &objHandle4);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // Try to erase certificate object at index 2 - should be disallowed as we created it with HLSE_ATTR_READ_ONLY attribute
    hlseRc = HLSE_EraseObject(objHandle2);
    result &= AX_CHECK_SW(hlseRc, HLSE_ERR_API_ERROR, "err");

    // Certificate Enumeration
    result &= exCertEnumerate();

    // Get certificate Attributes
    result &= exCertGetAttr();

    // Set Certificate Attributes
    result &= exCertSetAttr();

    // Certificate Enumeration
    result &= exCertEnumerate();

    PRINTF("\r\n-----------\r\nEnd exCertUsageReadOnly(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}

/**
 * Internal function , called from exCertUsageReadOnly()
 *
 * Demonstrate
 * - using HLSE_EnumerateObjects() for certificate enumeration
 * - using HLSE_GetObjectAttribute() to get certificate tag
 *
 */
static U8 exCertEnumerate()
{
    HLSE_OBJECT_HANDLE handles[10] = { 0 };
    U16 handleNum = 10;
    HLSE_RET_CODE hlseRc;
    U16 i;

    U8 result = 1;
    PRINTF("\r\n-----------\r\nStart exCertEnumerate()\r\n------------\r\n");

    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, handles, &handleNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    for (i = 0 ; i < handleNum ; ++i)  {
        U32 tag = 0;
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_INDEX;
        attr.value = &tag;
        attr.valueLen = sizeof(tag);

        hlseRc = HLSE_GetObjectAttribute(handles[i], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        PRINTF("exCertEnumerate - tag = 0x%X\r\n", tag);
    }

    PRINTF("\r\n-----------\r\nEnd exCertEnumerate(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}

/**
 * Internal function , called from exCertUsageReadOnly()
 *
 * Demonstrate
 * - using HLSE_EnumerateObjects() for certificate enumeration
 * - using HLSE_GetObjectAttribute() with HLSE_ATTR_OBJECT_VALUE to get object's value
 *
 */
static U8 exCertGetAttr()
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    HLSE_OBJECT_HANDLE handles[10] = { 0 };
    U16 handleNum = 10;
    U16 i;

    PRINTF("\r\n-----------\r\nStart exCertGetAttr()\r\n------------\r\n");

    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, handles, &handleNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    for (i = 0; i < handleNum; ++i)  {
        U8 data[300];

        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = data;
        attr.valueLen = sizeof(data);

        hlseRc = HLSE_GetObjectAttribute(handles[i], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    PRINTF("\r\n-----------\r\nEnd exCertGetAttr(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}

/**
 * Demonstrate
 * - using HLSE_EnumerateObjects() for certificate enumeration
 * - shows using HLSE_SetObjectAttribute() to change an object's value fails when credential is frozen
 *
 */
static U8 exCertSetAttr()
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    HLSE_OBJECT_HANDLE handles[10];
    U16 handleNum = 10;

    U8 certData[40];

    U8 largeCertData[300];

    U16 i ;
    memset(certData, 0xBB, sizeof(certData));
    memset(largeCertData, 0xBD, sizeof(largeCertData));

    PRINTF("\r\n-----------\r\nStart exCertSetAttr()\r\n------------\r\n");

    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, handles, &handleNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    for (i = 0; i < handleNum-2; ++i)  {
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = certData;
        attr.valueLen = sizeof(certData);

        hlseRc = HLSE_SetObjectAttribute(handles[i], &attr);
        result &= AX_CHECK_SW(hlseRc, SW_COMMAND_NOT_ALLOWED, "Expected to fail, frozen credential cannot be changed");
    }

    // Try to update large cert data which is locked in GP storage
    i = handleNum - 2;
    {
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = largeCertData;
        attr.valueLen = sizeof(largeCertData);

        hlseRc = HLSE_SetObjectAttribute(handles[i], &attr);
        result &= AX_CHECK_SW(hlseRc, SW_COMMAND_NOT_ALLOWED, "Expected to fail, frozen credential cannot be changed");
        // Last one is not frozen and should allow update
        i = handleNum - 1;
        hlseRc = HLSE_SetObjectAttribute(handles[i], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    PRINTF("\r\n-----------\r\nEnd exCertSetAttr(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}


/**
* Internal utility to patch the TLV header of a data buffer so it contains the
* certificate header TLV correct
*
* A certificate header consists of the 0x30 tag followes by a length which could be encoded as
* 1) one byte: the length byte in case of length <= 127
* 2) 2 bytes : 0x81 followed byte the length byte in case 127 < length <= 255
* 3) 3 bytes : 0x82 followed by 2 bytes length otherwise
*/
static HLSE_RET_CODE patchCertificateInitialTL(U8 *clientCertDer, U16 clientCertDerLen)
{
    if (clientCertDerLen < 3)
    {
        // This is too short for a certificate
        return ERR_GP_NO_CERT;
    }
    else if (clientCertDerLen <= LEN_SHORT_FORM_MAX)
    {
        clientCertDer[0] = 0x30;
        clientCertDer[1] = (clientCertDerLen-2) & 0x00FF;
    }
    else if (clientCertDerLen <= LEN_LONG_FORM_ONE_BYTE_MAX)
    {
        clientCertDer[0] = 0x30;
        clientCertDer[1] = 0x81;
        clientCertDer[2] = (clientCertDerLen-3) & 0x00FF;
    }
    else
    {
        clientCertDer[0] = 0x30;
        clientCertDer[1] = 0x82;
        clientCertDer[2] = ((clientCertDerLen-4) >> 8) & 0x00FF;
        clientCertDer[3] = (clientCertDerLen-4) & 0x00FF;
    }
    return HLSE_SW_OK;
}


/**
* Internal - called from exMixedCertsAndDataObjUsage
*  Get an object's value and length based on based on object handle provided
*
* @param[in]     handle     object handle
* @param[out]    data       buffer to store object's value retrieved
* @param[in,out] len        in - length of data buffer, out - actual object's length
*/
static HLSE_RET_CODE getObject(HLSE_OBJECT_HANDLE handle, U8 *data, U16 *len)
{
    HLSE_RET_CODE hlseRc;
    HLSE_ATTRIBUTE attr;
    HLSE_OBJECT_TYPE type;
    U16 offset;

    // We are looking for the object on index i in handles array
    type = HLSE_GET_OBJECT_TYPE(handle);
    if (type == HLSE_CERTIFICATE) {
        PRINTF("get Certificate Object, handle=0x%X\n", handle);
    }
    else {
        PRINTF("get Data Object, handle=0x%X\n", handle);
    }

    // Read
    attr.type = HLSE_ATTR_OBJECT_VALUE;
    attr.value = data;
    attr.valueLen = *len;
    hlseRc = HLSE_GetObjectAttribute(handle, &attr);
    if (hlseRc != HLSE_SW_OK) {
        return hlseRc;
    }

    *len = attr.valueLen;

    // obtain object's GP offset
    {
        HLSE_ATTRIBUTE attrL;
        attrL.type = HLSE_ATTR_OBJECT_OFFSET;
        attrL.value = &offset;
        attrL.valueLen = sizeof(offset);

        hlseRc = HLSE_GetObjectAttribute(handle, &attrL);
        if (hlseRc != HLSE_SW_OK) {
            return hlseRc;
        }
    }

    PRINTF("Object retrieved: Offset=%d, Size=%d\n", offset, *len);

    return HLSE_SW_OK;
}


/**
* Internal - called from exMixedCertsAndDataObjUsage
*  Set a Certificate or Data object's value based on on object handle provided
*
* @param[in] handle     object handle
* @param[in] data       buffer of data to set obj value
* @param[in] len        in - length of data buffer
*/
static HLSE_RET_CODE setObject(HLSE_OBJECT_HANDLE handle, U8 *data, U16 len)
{
    HLSE_RET_CODE hlseRc;
    HLSE_OBJECT_TYPE type;

    // We are looking for the object on index i in handles array
    type = HLSE_GET_OBJECT_TYPE(handle);
    if (type == HLSE_CERTIFICATE) {
        PRINTF("Set Certificate Object, handle=0x%X\n", handle);
    }
    else {
        PRINTF("Set Data Object, handle=0x%X\n", handle);
    }

    // Update
    {
        HLSE_ATTRIBUTE attrL;

        attrL.type = HLSE_ATTR_OBJECT_VALUE;
        attrL.value = data;
        attrL.valueLen = len;

        hlseRc = HLSE_SetObjectAttribute(handle, &attrL);
        if (hlseRc != HLSE_SW_OK) {
            return hlseRc;
        }
    }

    return HLSE_SW_OK;
}
