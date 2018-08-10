/**
 * @file ex_hlse_cert.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2018
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
 *
 * @par Description
 * Example using Certificate objects
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "HLSEAPI.h"
#include "sm_types.h"
#include "sm_apdu.h"
#include "tst_sm_util.h"

#include "HLSEAPI.h"
#include "tst_a71ch_util.h"
#include "tst_hlse_a71ch_util.h"

static U8 exDataObjUsage(U8 initMode);

static U8 exCertUsageBasic(U8 initMode);
static U8 exCertUsageEnlarge(U8 initMode);
static U8 exCertUsageGpTableLengthUnknown(U8 initMode);

static U8 exCertUsageReadOnly(U8 initMode);
static U8 exCertEnumerate(void);
static U8 exCertGetAttr(void);
static U8 exCertSetAttr(void);
#if 0  // currently not used
static U8 exCertDelete(void);
#endif

/**
 * Demonstrate usage of certificate objects
 */
U8 exHlseCert()
{
    U8 result = 1;
    printf("\r\n-----------\r\nStart exHlseCert()\r\n------------\r\n");

    // Example of Data object usage
    result &= exDataObjUsage(INIT_MODE_RESET);

    // Example certificate usage with enhanced validations
    result &= exCertUsageBasic(INIT_MODE_RESET);

    // Example certificate usage enlarging and updating a certificate
    result &= exCertUsageEnlarge(INIT_MODE_RESET);

    // Example of Certificate creation / enumeration / Get / Set / Delete - when Read Only attribute used
    result &= exCertUsageReadOnly(INIT_MODE_RESET);

    // Example certificate usage when map contains indirect length (to be obtained from TLV of actual object)
    result &= exCertUsageGpTableLengthUnknown(INIT_MODE_RESET);

    // overall result
    printf("\r\n-----------\r\nEnd exHlseCert(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}


/**
 * Demonstrate usage of certificate objects:
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

    printf("\r\n-----------\r\nStart exCertUsageBasic(%s)\r\n------------\r\n", getInitModeAsString(initMode));

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
    printf("\r\nHLSE_CreateObject() - Create certificate object...\r\n------------\r\n");
    hlseRc = HLSE_CreateObject(attr, templateSize, &certHandles[0]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // assumption : correct slot in GP Table data was occupied
    // TODO: consider additional low level test for it

    // Now read certificate data and verify correctness
    // Find certificate where index=0

    // enumerate objects - we should have one certificate by now with index 0
    printf("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 1, "err");

    // make sure our certificate is the one we previously created
    // Get the attributes of the single certificate we have
    printf("\r\nHLSE_GetObjectAttribute()...\r\n");
    {
        U32 certIndex = 0xFFFFFFFF;
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_INDEX;
        attr.value = &certIndex;
        attr.valueLen = sizeof(certIndex);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        printf("\tcert index = 0x%lx\r\n", certIndex);
        result &= AX_CHECK_U16((U16)certIndex, 0x00, "err");
    }

    printf("Verify Certificate contents...\r\n");
    {
        U8 readCertData[50];

        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readCertData;
        attr.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("certOnA71CH", readCertData, sizeof(certData),
            "expected cert data", certData, sizeof(certData), AX_COLON_32);
    }

    printf("Update Certificate contents...\r\n");
    {
        HLSE_ATTRIBUTE attr;
        memset(data, 0xAB, sizeof(data));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = data;
        attr.valueLen = sizeof(data);

        hlseRc = HLSE_SetObjectAttribute(certHandles[certHandlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }


    printf("Verify Certificate Updated correctly...\r\n");
    {
        U8 readCertData[50];
        HLSE_ATTRIBUTE attr;

        memset(readCertData, 0x00, sizeof(readCertData));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readCertData;
        attr.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(certData),
            "expected cert data", data, sizeof(data), AX_COLON_32);
    }

    // Delete the certificate
    printf("\r\nHLSE_EraseObject()...\r\n");
    hlseRc = HLSE_EraseObject(certHandles[certHandlesNum - 1]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // Verify it was actually deleted
    // enumerate objects - we should have no certificates by now
    printf("\r\nVerify object was erased...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 0, "err");


    printf("\r\n-----------\r\nEnd exCertUsageBasic(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}


/**
* Demonstrate usage of data objects:
*
* Simulate the following scenario and aim to verify it works correctly:
*
* - Create Data Object
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

    printf("\r\n-----------\r\nStart exDataObjUsage(%s)\r\n------------\r\n", getInitModeAsString(initMode));

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
    printf("\r\nHLSE_CreateObject() - Create Data object...\r\n------------\r\n");
    hlseRc = HLSE_CreateObject(attr, templateSize, &handles[0]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // assumption : correct slot in GP Table data was occupied
    // TODO: consider additional low level test for it

    // Now read data object and verify correctness
    // Find data object where index=0

    // enumerate objects - we should have one certificate by now with index 0
    printf("\r\nHLSE_EnumerateObjects()...\r\n");
    handlesNum = sizeof(handles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_DATA, handles, &handlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(handlesNum, 1, "err");

    // make sure our data object is the one we previously created
    // Get the attributes of the single data object we have
    printf("\r\nHLSE_GetObjectAttribute()...\r\n");
    {
        U32 dataIndex = 0xFFFFFFFF;
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_INDEX;
        attr.value = &dataIndex;
        attr.valueLen = sizeof(dataIndex);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        printf("\tdata object index = 0x%lx\r\n", dataIndex);
        result &= AX_CHECK_U16((U16)dataIndex, 0x00, "err");
    }

    printf("Verify Data object contents...\r\n");
    {
        U8 readData[50];

        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readData;
        attr.valueLen = sizeof(readData);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("dataOnA71CH", readData, sizeof(data),
            "expected data", data, sizeof(data), AX_COLON_32);
    }

    printf("Update Data object contents...\r\n");
    {
        HLSE_ATTRIBUTE attr;
        memset(data, 0xAB, sizeof(data));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = data;
        attr.valueLen = sizeof(data);

        hlseRc = HLSE_SetObjectAttribute(handles[handlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    printf("Verify Data object Updated correctly...\r\n");
    {
        U8 readData[50];
        HLSE_ATTRIBUTE attr;

        memset(readData, 0x00, sizeof(readData));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readData;
        attr.valueLen = sizeof(readData);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readData, sizeof(readData),
            "expected data", data, sizeof(data), AX_COLON_32);
    }

    printf("Show usage of direct read / Update access to Data Object...\r\n");
    {
        HLSE_ATTRIBUTE attr;
        HLSE_DIRECT_ACCESS_ATTRIBUTE_VALUE theValue;
        U8 buffer[50]; // a buffer to hold the read data
        U8 refBuffer2[50];
        memset(buffer, 0x00, sizeof(buffer));

        attr.type = HLSE_ATTR_DIRECT_ACCESS_OBJECT_VALUE;

        printf("    Verify Direct Read of Data Object...\r\n");
        theValue.offset = 0;
        theValue.bytes = 32;
        theValue.buffer = buffer;
        theValue.bufferLen = sizeof(buffer);

        attr.value = &theValue;
        attr.valueLen = sizeof(theValue);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("dataOnA71CH", buffer, theValue.bytes,
            "expected data", data, theValue.bytes, AX_COLON_32);

        // Now show usage of direct update to data object - update 10 bytes at offset 0 to 0x11
        memset(buffer, 0x11, 10);
        theValue.bytes = 10;
        theValue.buffer = buffer;

        hlseRc = HLSE_SetObjectAttribute(handles[handlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        printf("    Verify Direct Update of Data Object...\r\n");
        memcpy(refBuffer2, buffer, 50);
        memset(buffer, 0x00, sizeof(buffer));

        theValue.bytes = 32;
        theValue.buffer = buffer;
        theValue.bufferLen = sizeof(buffer);

        hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("dataOnA71CH", buffer, theValue.bytes,
            "expected data", refBuffer2, theValue.bytes, AX_COLON_32);

        printf("    Test Direct Read / Write with offset that exceeds object boundaries...\r\n");
        {
            // Note : data object 0 is at offset 0 , size 50 bytes

            memset(buffer, 0x00, sizeof(buffer));

            theValue.offset = 100;
            theValue.bytes = 50;
            theValue.buffer = buffer;
            theValue.bufferLen = sizeof(buffer);

            // Read with offset exceeding object boundary - not allowed
            hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attr);
            result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");

            // Write with offset exceeding object boundary - not allowed
            hlseRc = HLSE_SetObjectAttribute(handles[handlesNum - 1], &attr);
            result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");
        }

        printf("    Test Direct Read / Write with length that exceeds object boundaries...\r\n");
        {
            // Note : data object 0 is at offset 0 , size 50 bytes

            memset(buffer, 0x00, sizeof(buffer));

            theValue.offset = 30;
            theValue.bytes = 50;
            theValue.buffer = buffer;
            theValue.bufferLen = sizeof(buffer);

            // Read exceeding object boundary - not allowed
            hlseRc = HLSE_GetObjectAttribute(handles[handlesNum - 1], &attr);
            result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");

            // Write exceeding object boundary - not allowed
            hlseRc = HLSE_SetObjectAttribute(handles[handlesNum - 1], &attr);
            result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");
        }
    }


    // Delete the Data object
    printf("\r\nHLSE_EraseObject()...\r\n");
    hlseRc = HLSE_EraseObject(handles[handlesNum - 1]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // Verify it was actually deleted
    // enumerate objects - we should have no certificates by now
    printf("\r\nVerify object was erased...\r\n");
    handlesNum = sizeof(handles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_DATA, handles, &handlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(handlesNum, 0, "err");

    printf("\r\n-----------\r\nEnd exDataObjUsage(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}

/**
 * Demonstrate usage of certificate objects:
 *
 * Simulate the following scenario and aim to verify it works correctly:
 *
 * - Create 5 certificates
 * - Verify cannot create additional certificate
 * - Verify allows to enlarge a certificate with size within allocated boundary
 * - Verify cannot enlarge beyond the allocated boundary
 * - Delete the certificate
 * - Create it with new size over the boundary allocated before deletion
 *
 * @param[in] initMode Visit the documentation of ::a71chInitModule for more information on this parameter
 */
static U8 exCertUsageEnlarge(U8 initMode)
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    HLSE_OBJECT_HANDLE certHandles[6];

    // to hold actual handles created
    //HLSE_OBJECT_HANDLE certHandle0;
    //HLSE_OBJECT_HANDLE certHandle1;
    HLSE_OBJECT_HANDLE certHandle2;
    HLSE_OBJECT_HANDLE certHandle3;
    //HLSE_OBJECT_HANDLE certHandle4;

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

    printf("\r\n-----------\r\nStart exCertUsageEnlarge(%s)\r\n------------\r\n", getInitModeAsString(initMode));

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

    printf("\r\nAbout to create 5 certificates...");
    for (indexCert = 0; indexCert < 5; indexCert++)
    {
        index = indexCert;
        memset(certData, (0xAA+index), sizeof(certData));

        printf("\r\nHLSE_CreateObject() - Create certificate object(0x%02lx)", index);
        hlseRc = HLSE_CreateObject(attr, templateSize, &certHandles[indexCert]);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    // save the handles as the values in the array might be changed . not reflecting the actual handles , after deletion
    //certHandle0 = certHandles[0];
    //certHandle1 = certHandles[1];
    certHandle2 = certHandles[2];
    certHandle3 = certHandles[3];
    //certHandle4 = certHandles[4];

    printf("\r\nTrying to create an additional certificate - should fail...");
    index = 5;
    memset(certData, (0xAA + index), sizeof(certData));
    printf("\r\nHLSE_CreateObject() - Create certificate object(0x%02lx)\r\n", index);
    hlseRc = HLSE_CreateObject(attr, templateSize, &certHandles[index]);
    result &= AX_CHECK_SW(hlseRc, HLSE_ERR_IDENT_IDX_RANGE, "err");


    // enlarge certificate index=2 within boundary
//    index = 2;
    printf("Try to Enlarge Certificate contents within 2 chunks...\r\n");
    {
        HLSE_ATTRIBUTE attr;
        memset(dataIn2Chunks, 0xBB, sizeof(dataIn2Chunks));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = dataIn2Chunks;
        attr.valueLen = sizeof(dataIn2Chunks);

        hlseRc = HLSE_SetObjectAttribute(certHandle2 /*certHandles[index]*/, &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    printf("Verify Certificate Updated correctly...\r\n");
    {
        U8 readCertData[60];
        HLSE_ATTRIBUTE attr;

        memset(readCertData, 0x00, sizeof(readCertData));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readCertData;
        attr.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandle2 /*certHandles[index]*/, &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(readCertData),
            "expected cert data", dataIn2Chunks, sizeof(dataIn2Chunks), AX_COLON_32);
    }

    // enlarge certificate index=2 , over the boundary - not allowed - need to delete it and recreate it
//    index = 2;
    printf("Try to Enlarge Certificate with 3 chunks data over the allocated size - not allowed...\r\n");
    {
        HLSE_ATTRIBUTE attr;
        memset(dataIn3Chunks, 0xCC, sizeof(dataIn3Chunks));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = dataIn3Chunks;
        attr.valueLen = sizeof(dataIn3Chunks);

        hlseRc = HLSE_SetObjectAttribute(certHandle2 /*certHandles[index]*/, &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_ERR_MEMORY, "err");
    }

    printf("Verify Certificate contents has not changed...\r\n");
    {
        U8 readCertData[60];
        HLSE_ATTRIBUTE attr;

        memset(readCertData, 0x00, sizeof(readCertData));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readCertData;
        attr.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandle2 /*certHandles[index]*/, &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(readCertData),
            "expected cert data", dataIn2Chunks, sizeof(dataIn2Chunks), AX_COLON_32);
    }

    // enumerate objects - we should have 5 objects
    memset(certHandles, 0, sizeof(certHandles));
    printf("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 5, "err");
    //

    // Need to delete the object and recreate it
    printf("\r\nHLSE_EraseObject()...\r\n");
    hlseRc = HLSE_EraseObject(certHandle2 /*certHandles[index]*/);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // enumerate objects - we should have 4 objects
    memset(certHandles, 0, sizeof(certHandles));
    printf("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 4, "err");
    //

    // recreate it - should now succeed
    index = 2;
    // set the reference data that in cert 3 that is expected not to change
    memset(dataIn3Chunks, (0xAA + index), sizeof(certData));
    {
        attr[0].type = HLSE_ATTR_OBJECT_TYPE;
        attr[0].value = &objType;
        attr[0].valueLen = sizeof(objType);
        attr[1].type = HLSE_ATTR_OBJECT_INDEX;
        attr[1].value = &index;
        attr[1].valueLen = sizeof(index);
        attr[2].type = HLSE_ATTR_OBJECT_VALUE;
        attr[2].value = dataIn3Chunks;
        attr[2].valueLen = sizeof(dataIn3Chunks);

        printf("\r\nHLSE_CreateObject() - Create certificate object(0x%02lx)\r\n", index);
        hlseRc = HLSE_CreateObject(attr, templateSize, &certHandle2 /*&certHandles[index]*/);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    printf("Verify Certificate Updated correctly...\r\n");
    {
        U8 readCertData[90];
        HLSE_ATTRIBUTE attr;

        memset(readCertData, 0x00, sizeof(readCertData));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readCertData;
        attr.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandle2 /*certHandles[index]*/, &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(readCertData),
            "expected cert data", dataIn3Chunks, sizeof(dataIn3Chunks), AX_COLON_32);
    }

    //
    printf("Verify Cert 3 still exists\r\n");
    {
        U8 readCertData[90];
        HLSE_ATTRIBUTE attr;
        index = 3;

        memset(readCertData, 0x00, sizeof(readCertData));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readCertData;
        attr.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandle3 /*certHandles[index]*/, &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // set the reference data that in cert 3 that is expected not to change
        memset(certData, (0xAA + index), sizeof(certData));

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(certData),
            "expected cert data", certData, sizeof(certData), AX_COLON_32);
    }
    //

    // enumerate objects - we should have 5 objects
    memset(certHandles, 0, sizeof(certHandles));
    printf("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 5, "err");

    // delete all certs
    for (indexCert = 0; indexCert < 5; indexCert++)
    {
        index = indexCert;

        printf("\r\nHLSE_EraseObject() - index 0x%02lx", index);
        hlseRc = HLSE_EraseObject(certHandles[index]);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    printf("\r\n-----------\r\nEnd exCertUsageEnlarge(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

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
 * - Each object is aligned to occupy at least the amount of memory on the boundary of the GP storage, that is 32 bytes.Eg an object of 17 bytes will occupy 32 bytes
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
    certData[0] = 0x06;     // tag
    certData[1] = 0x81;     // length in next byte
    certData[2] = 0x2F;     // length is 47 bytes

    printf("\r\n-----------\r\nStart exCertUsageGpTableLengthUnknown(%s)\r\n------------\r\n", getInitModeAsString(initMode));

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
    printf("\r\nHLSE_CreateObject() - Create certificate object, use 2 bytes encoded length in header...\r\n------------\r\n");
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
    printf("\r\noverwrite gp table map to have indirect length indication...\r\n");
    {
        U16 gpSize;
        //HLSE_RET_CODE lReturn = HLSE_SW_OK;
        U8 data[32];
        U8 dataSize = sizeof(data);

        hlseRc = hlse_GetGPDataSize(&gpSize);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Read the entire table - consists of one chunk 5 entries
        hlseRc = A71_GetGpData(gpSize - 32, data, 32);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Now overwrite Gp Table with indirect length
        data[dataSize - 2 - 1*6 + 2] = 0x80;
        data[dataSize - 2 - 1*6 + 3] = 0x00;

//      data[2] = 0x80;
//      data[3] = 0x00;

#if 0   // just for debug !!
        //  data format to check
        U8 altData[32] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,            // invalid entry
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,            // invalid entry
            0x09, 0x02, 0x80, 0x00, 0x06, 0x00,         // third certificate
            0x09, 0x01, 0x80, 0x00, 0x02, 0x00,         // second certificate
            0x09, 0x00, 0x80, 0x00, 0x00, 0x00,         // first certificate
            0x00, 0x03                                                       // update counter + num of entries
        };
        memcpy(data, altData, 32);
#endif

        hlseRc = A71_SetGpData(gpSize - 32, data, 32);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        hlseRc = Debug_ForceReadGPDataTable();
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }


    // enumerate objects - we should have one certificate by now with index 0
    printf("\r\nHLSE_EnumerateObjects()...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 1, "err");

    // make sure our certificate is the one we previously created
    // Get the attributes of the single certificate we have
    printf("\r\nHLSE_GetObjectAttribute()...\r\n");
    {
        U32 certIndex = 0xFFFFFFFF;
        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_INDEX;
        attr.value = &certIndex;
        attr.valueLen = sizeof(certIndex);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        printf("\tcert index = 0x%lx\r\n", certIndex);
        result &= AX_CHECK_U16((U16)certIndex, 0x00, "err");
    }

    printf("Verify Certificate contents...\r\n");
    {
        U8 readCertData[50];

        HLSE_ATTRIBUTE attr;
        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readCertData;
        attr.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("certOnA71CH", readCertData, sizeof(certData),
            "expected cert data", certData, sizeof(certData), AX_COLON_32);
    }

    printf("Update Certificate contents , use 3 bytes encoded length in header...\r\n");
    {
        HLSE_ATTRIBUTE attr;
        memset(data, 0xAB, sizeof(data));

        // Now use 3 bytes indirect length

        // Set header data - to be able to simulate length is unknown in the GP table at time Gp table was created -
        // so it will be retrieved from this header
        data[0] = 0x06;     // tag
        data[1] = 0x82;     // length in next 2 bytes
        data[2] = 0x00;     // MSB 0
        data[3] = 0x2E;     // length is 46 bytes

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = data;
        attr.valueLen = sizeof(data);

        hlseRc = HLSE_SetObjectAttribute(certHandles[certHandlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    // We'll now overrun this map to contain 0x8000 in the length to indicate indirect length
    {
        U16 gpSize;
        //HLSE_RET_CODE lReturn = HLSE_SW_OK;
        U8 data[32];

        hlseRc = hlse_GetGPDataSize(&gpSize);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Read the entire table - consists of one chunk 5 entries
        hlseRc = A71_GetGpData(gpSize - 32, data, 32);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        // Now overrite with indirect length
        data[2] = 0x80;
        data[3] = 0x00;

        hlseRc = A71_SetGpData(gpSize - 32, data, 32);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        hlseRc = Debug_ForceReadGPDataTable();
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    printf("Verify Certificate Updated correctly...\r\n");
    {
        U8 readCertData[50];
        HLSE_ATTRIBUTE attr;

        memset(readCertData, 0x00, sizeof(readCertData));

        attr.type = HLSE_ATTR_OBJECT_VALUE;
        attr.value = readCertData;
        attr.valueLen = sizeof(readCertData);

        hlseRc = HLSE_GetObjectAttribute(certHandles[certHandlesNum - 1], &attr);
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

        result &= AX_COMPARE_BYTE_ARRAY("read data", readCertData, sizeof(certData),
            "expected cert data", data, sizeof(data), AX_COLON_32);
    }


    // Delete the certificate
    printf("\r\nHLSE_EraseObject()...\r\n");
    hlseRc = HLSE_EraseObject(certHandles[certHandlesNum - 1]);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    // Verify it was actually deleted
    // enumerate objects - we should have no certificates by now
    printf("\r\nVerify object was erased...\r\n");
    certHandlesNum = sizeof(certHandles) / sizeof(HLSE_OBJECT_HANDLE);
    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, certHandles, &certHandlesNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    result &= AX_CHECK_U16(certHandlesNum, 0, "err");


    printf("\r\n-----------\r\nEnd exCertUsageGpTableLengthUnknown(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

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

    printf("\r\n-----------\r\nStart exCertUsageReadOnly(%s)\r\n------------\r\n", getInitModeAsString(initMode));

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

    printf("\r\n-----------\r\nEnd exCertUsageReadOnly(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

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
    HLSE_OBJECT_HANDLE handles[10];
    U16 handleNum = 10;
    HLSE_RET_CODE hlseRc;
    U16 i;

    U8 result = 1;
    printf("\r\n-----------\r\nStart exCertEnumerate()\r\n------------\r\n");

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

        printf("exCertEnumerate - tag = 0x%lx\r\n", tag);
    }

    printf("\r\n-----------\r\nEnd exCertEnumerate(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

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

    HLSE_OBJECT_HANDLE handles[10];
    U16 handleNum = 10;
    U16 i;

    printf("\r\n-----------\r\nStart exCertGetAttr()\r\n------------\r\n");

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

    printf("\r\n-----------\r\nEnd exCertGetAttr(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

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

    printf("\r\n-----------\r\nStart exCertSetAttr()\r\n------------\r\n");

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

    printf("\r\n-----------\r\nEnd exCertSetAttr(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}

#if 0
static U8 exCertDelete()
{
    U8 result = 1;
    HLSE_RET_CODE hlseRc;

    HLSE_OBJECT_HANDLE handles[10];
    U16 handleNum = 10;

    U8 certData[40];
    U16 i;
    memset(certData, 0xBB, sizeof(certData));

    printf("\r\n-----------\r\nStart exCertDelete()\r\n------------\r\n");

    hlseRc = HLSE_EnumerateObjects(HLSE_CERTIFICATE, handles, &handleNum);
    result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");

    for (i = 0; i < handleNum; ++i)  {
        hlseRc = HLSE_EraseObject(handles[i]);
        // TODO : Verify Behaviour -  here returns 0x9000 and just markes as erased in the GP Table with index = 0xFF-
        //  - It has no knowlgdge whether the actual object in the GP data is frozen
        result &= AX_CHECK_SW(hlseRc, HLSE_SW_OK, "err");
    }

    printf("\r\n-----------\r\nEnd exCertDelete(), result = %s\r\n------------\r\n", ((result == 1) ? "OK" : "FAILED"));

    return result;
}
#endif
