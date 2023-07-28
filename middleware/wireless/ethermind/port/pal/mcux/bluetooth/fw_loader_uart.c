#ifdef DOWNLOAD_BT_FW

/*===================== Include Files ============================================*/
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "fw_loader_types.h"

#include "fsl_adapter_uart.h"
#include "fsl_lpuart.h"

#include "controller.h"
#include "eBT_os.h"

typedef struct
{
	uint32 iBaudRate;
	uint32 iUartDivisor;
	uint32 iClkDivisor;
}UART_BAUDRATE;

/*===================== Macros ===================================================*/
#define VERSION	           "M322"
#define MAX_LENGTH         0xFFFF       //Maximum 2 byte value 
#define END_SIG_TIMEOUT    2500
#define MAX_CTS_TIMEOUT    500         //500ms   
#define TIMEOUT_VAL_MILLISEC  20000       // Timeout for getting 0xa5 or 0xaa or 0xa7, 2 times of helper timeout
#define STRING_SIZE        6
#define HDR_LEN            16
#define CMD4               0x4
#define CMD6               0x6
#define CMD7               0x7

#define V1_HEADER_DATA_REQ      0xa5
#define V1_REQUEST_ACK          0x5a
#define V1_START_INDICATION     0xaa

#define V3_START_INDICATION     0xab
#define V3_HEADER_DATA_REQ      0xa7
#define V3_REQUEST_ACK          0x7a
#define V3_TIMEOUT_ACK          0x7b
#define V3_CRC_ERROR            0x7c

#define PRINT(...)         printf(__VA_ARGS__)

#define DOWNLOAD_SUCCESS                 0x0
#define OPEN_SERIAL_PORT_OR_FILE_ERROR   0x1
#define FEEK_SEEK_ERROR                  0x2
#define FILESIZE_IS_ZERO                 0x3
#define HEADER_SIGNATURE_TIMEOUT         0x4
#define READ_FILE_FAIL                   0x5
#define CHANGE_BAUDRATE_FAIL             0x6
#define CHANGE_TIMEOUT_VALUE_FAIL        0x7
#define OPEN_FILE_FAIL                   0x8
#define FILE_MODE_CANNOT_CHANGE          0X9
#define UNEXPECTED_BEHAVIOUR_IN_SETJMP   0xA
#define MALLOC_RETURNED_NULL             0xB

#define REQ_HEADER_LEN          1
#define A6REQ_PAYLOAD_LEN       8
#define AbREQ_PAYLOAD_LEN       3

#define END_SIG       0x005043

#define GP            0x107   /* x^8 + x^2 + x + 1 */
#define DI            0x07

#define CRC_ERR_BIT            1 << 0
#define NAK_REC_BIT            1 << 1
#define TIMEOUT_REC_ACK_BIT    1 << 2
#define TIMEOUT_REC_HEAD_BIT   1 << 3
#define TIMEOUT_REC_DATA_BIT   1 << 4
#define INVALID_CMD_REC_BIT    1 << 5
#define WIFI_MIC_FAIL_BIT      1 << 6
#define BT_MIC_FAIL_BIT        1 << 7

#define SWAPL(x) ((((x) >> 24) & 0xff) \
		| (((x) >> 8) & 0xff00) \
		| (((x) << 8) & 0xff0000L) \
		| (((x) << 24) & 0xff000000L))

#define POLYNOMIAL 0x04c11db7L

#define CLKDIVAddr       0x7f00008f
#define UARTDIVAddr      0x7f000090
#define UARTMCRAddr      0x7f000091
#define UARTREINITAddr   0x7f000092
#define UARTICRAddr      0x7f000093
#define UARTFCRAddr      0x7f000094

#define MCR   0x00000022
#define INIT  0x00000001
#define ICR   0x000000c7
#define FCR   0x000000c7

static unsigned char crc8_table[256];     /* 8-bit table */
static int made_table=0;

static unsigned long crc_table[256];
static uint8 cmd7_Req = FALSE;
static uint8 EntryPoint_Req = FALSE;
static uint32 change_baudrata_buffer_len = 0;
static uint32 cmd7_change_timeout_len = 0;

//CMD5 Header to change bootload baud rate 
static int8 m_Buffer_CMD5_Header[16] = {0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2c,0x00,0x00,0x00,0x77,0xdb,0xfd,0xe0};
static int8 m_Buffer_CMD7_ChangeTimeoutValue[16] = {0x07,0x00,0x00,0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5b,0x88,0xf8,0xba};
static const UART_BAUDRATE UartCfgTbl[]={
	{115200,   16, 0x0075F6FD},
	{3000000,  1, 0x00C00000},
};

#define DEBUG_PRINT
#define TEST_CODE

/*==================== Typedefs =================================================*/

/*===================== Global Vars ==============================================*/
// Maximum Length that could be asked by the Helper = 2 bytes 
static uint8 ucByteBuffer[MAX_LENGTH];

// Size of the File to be downloaded
static long  ulTotalFileSize = 0;

// Current size of the Download
static uint32  ulCurrFileSize = 0;
static uint32  ulLastOffsetToSend = 0xFFFF;
static uint8 uiErrCase = FALSE;
static uint8 uiReDownload = FALSE;
// Received Header
static uint8   ucRcvdHeader = 0xFF;

static uint8   ucString[STRING_SIZE];

static uint8 b16BytesData = FALSE;

static  uint16 uiNewLen;
static  uint32 ulNewOffset;
static  uint16 uiNewError;
static  uint8  uiNewCrc;

static  uint8   uiProVer;
static  uint8 bVerChecked = FALSE;
static  uint8   ucCalCrc[10];

typedef enum
{
	Ver1,
	Ver2,
	Ver3,
}Version;

#ifdef DEBUG_PRINT
static uint8 uiErrCnt[16] = {0};
#endif

lpuart_config_t config;
static UART_HANDLE_DEFINE(s_controllerHciUartHandle);

/*==================== Function Prototypes ======================================*/

status_t  (*fw_upload_ComReadChar)(LPUART_Type *base, uint8_t *data, size_t length);
status_t (*fw_upload_ComWriteChar)(LPUART_Type *base, const uint8_t *data, size_t length);
status_t  (*fw_upload_ComReadChars)(LPUART_Type *base, uint8_t *data, size_t length);
status_t (*fw_upload_ComWriteChars)(LPUART_Type *base, const uint8_t *data, size_t length);
status_t (*fw_upload_init_uart)(LPUART_Type *base, const lpuart_config_t *config, uint32_t srcClock_Hz);
size_t (*fw_upload_GetBufferSize)(LPUART_Type *base, lpuart_handle_t *handle);
void (*fw_upload_CloseUart)(LPUART_Type *base);
INT32 (*fw_upload_GetTime)(void);
void (*fw_upload_DelayInMs)(uint32_t millisec);

static void fw_upload_gen_crc_table(void);
static unsigned long fw_upload_update_crc(unsigned long crc_accum, char *data_blk_ptr, int data_blk_size);
static void fw_upload_io_func_init(void);
static uint8 fw_upload_Check_ReqCrc(uint8 * uiStr, uint8 uiReq);
static uint8 fw_upload_lenValid(uint16 * uiLenToSend, uint8 * ucArray);
static void init_crc8(void);

/*==================== Coded Procedures =========================================*/
#ifdef TEST_CODE

static uint32 ucTestCase = 0;
static uint32 ucSleepTimeMs = 0;
static uint8  ucTestDone = 0;
static uint16 uiCurrLenToSend = 0;
uint8 myCrcCorrByte, myChangeCrc = 0;
static uint8 uiBaudRateDone = FALSE;

#endif

/******************************************************************************

 *
 * Name: gen_crc_table
 *
 * Description:
 *   Genrate crc table    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
void fw_upload_gen_crc_table()
{ 
	int i, j;
	unsigned long crc_accum;

	for (i=0; i < 256; i++)
	{
		crc_accum = ((unsigned long) i << 24);
		for (j=0;  j < 8;  j++)
		{
			if (crc_accum & 0x80000000L)
			{
				crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
			}
			else
			{
				crc_accum = (crc_accum << 1);
			}
		}
		crc_table[i] = crc_accum;
	}

	return;
}

/******************************************************************************

 *
 * Name: update_crc
 *
 * Description:
 *   update the CRC on the data block one byte at a time    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   ata_blk_ptr:   the buffer pointer for updating crc.
 *   data_blk_size: the size of buffer
 *
 * Return Value:
 *   CRC value.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
unsigned long fw_upload_update_crc(unsigned long crc_accum, char *data_blk_ptr, int data_blk_size)
{
	uint i, j;

	for(j = 0; j < data_blk_size; j++)
	{
		i = ((uint) (crc_accum >> 24) ^ *data_blk_ptr++) & 0xff;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}
	return crc_accum;
}

/******************************************************************************
 *
 * Name: fw_upload_io_func_init
 *
 * Description:
 *   This function initializes the IO function pointers.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
void fw_upload_io_func_init()
{
	fw_upload_DelayInMs = OSA_TimeDelay;
	fw_upload_GetTime = EM_get_time_ms;
	fw_upload_ComReadChar    = LPUART_ReadBlocking;
	fw_upload_ComWriteChar   = LPUART_WriteBlocking;
	fw_upload_ComWriteChars  = LPUART_WriteBlocking;
	fw_upload_ComReadChars   = LPUART_ReadBlocking;
	fw_upload_init_uart      = LPUART_Init;
	fw_upload_GetBufferSize  = LPUART_TransferGetRxRingBufferLength;
	fw_upload_CloseUart      = LPUART_Deinit;
}

/******************************************************************************
 *
 * Name: init_crc8
 *
 * Description:
 *   This function init crc.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
static void init_crc8()
{
	int i,j;
	int crc;
	if (!made_table)
	{
		for (i = 0; i < 256; i++)
		{
			crc = i;
			for (j = 0; j < 8; j++)
				crc = (crc << 1) ^ ((crc & 0x80) ? DI : 0);
			crc8_table[i] = (unsigned char) ((unsigned char)crc & (unsigned char)0xFF);
		}
		made_table = 1;
	}
}
/******************************************************************************
 *
 * Name: crc8
 *
 * Description:
 *   This function calculate crc.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   array: array to be calculated.
 *   len :  len of array.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
static unsigned char crc8 (unsigned char *array, unsigned char len)  
{  
	unsigned char CRC = 0xff;
	for(;len >0 ; len--)
	{
		CRC = crc8_table[CRC ^ *array];
		array++;
	}
	return CRC;
} 

/******************************************************************************
 *
 * Name: fw_upload_WaitForHeaderSignature(uint32 uiMs)
 *
 * Description:
 *   This function basically waits for reception
 *   of character 0xa5 on UART Rx. If no 0xa5 is 
 *   received, it will kind of busy wait checking for
 *   0xa5.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   uiMs:   the expired time.
 *
 * Return Value:
 *   TRUE:   0xa5 or 0xab is received.
 *   FALSE:  0xa5 or 0xab is not received.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
static uint8 fw_upload_WaitForHeaderSignature(uint32 uiMs)
{
	uint8 ucDone = 0; // signature not Received Yet.
	uint64 startTime = 0;
	uint64 currTime = 0;
	uint8 bResult = TRUE;
	ucRcvdHeader = 0xFF;
	startTime = fw_upload_GetTime();
	while (!ucDone)
	{
		fw_upload_ComReadChar(LPUART7, &ucRcvdHeader, 1);

		if ((ucRcvdHeader == V1_HEADER_DATA_REQ) || (ucRcvdHeader == V1_START_INDICATION) || \
				(ucRcvdHeader == V3_START_INDICATION) || (ucRcvdHeader == V3_HEADER_DATA_REQ))
		{
			ucDone = 1;
#ifdef DEBUG_PRINT
			PRINT("\nReceived 0x%x ", ucRcvdHeader);
#endif
			if(!bVerChecked)
			{
				if((ucRcvdHeader == V1_HEADER_DATA_REQ) || (ucRcvdHeader == V1_START_INDICATION))
				{
					uiProVer = (uint8)Ver1;
				}
				else
				{
					uiProVer = (uint8)Ver3;
				}
				bVerChecked = TRUE;
			}
		}
		else
		{

			if(uiMs)
			{
				currTime = fw_upload_GetTime();
				if(currTime - startTime > uiMs)
				{
					bResult = FALSE;
#ifdef DEBUG_PRINT
					PRINT("\ndidnt Received 0x%x , currTime %d startTime %d", ucRcvdHeader, currTime, startTime);
#endif
					break;
				}
			}
			fw_upload_DelayInMs(1);
		}
	}
	return bResult;
}

/******************************************************************************
 *
 * Name: fw_upload_WaitFor_Len
 *
 * Description:
 *   This function waits to receive the 4 Byte length.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pFile: The handler of file
 *
 * Return Value:
 *   2 Byte Length to send back to the Helper.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
static uint16 fw_upload_WaitFor_Len(uint8 flag)
{
	// Length Variables
	uint16 uiLen=0x0;
	uint16 uiLenComp=0x0;
	//uint8  uiVersion;
	// uiLen and uiLenComp are 1's complement of each other.
	// In such cases, the XOR of uiLen and uiLenComp will be all 1's
	// i.e 0xffff.
	uint16 uiXorOfLen = 0xFFFF;
	uint8 tmp;

	// Read the Lengths.
	fw_upload_ComReadChars(LPUART7, (char*)&uiLen, 2);
	fw_upload_ComReadChars(LPUART7, (char*)&uiLenComp, 2);

	// Check if the length is valid.
	if ((uiLen ^ uiLenComp) == uiXorOfLen) // All 1's
	{
#ifdef DEBUG_PRINT
		PRINT("\n       bootloader asks for %d bytes \n ", uiLen);
#endif
		// Successful. Send back the ack.
		if ((ucRcvdHeader == V1_HEADER_DATA_REQ) || (ucRcvdHeader == V1_START_INDICATION))
		{
			tmp = V1_REQUEST_ACK;
			fw_upload_ComWriteChar(LPUART7, &tmp, 1);
			if (ucRcvdHeader == V1_START_INDICATION)
			{
				/* Eliminated longjmp(resync, 1); returning restart status */
				return (uint16)V1_START_INDICATION;
			}
		}
	}
	else
	{
#ifdef DEBUG_PRINT
		PRINT("\n    NAK case: bootloader LEN = %x bytes \n ", uiLen);
		PRINT("\n    NAK case: bootloader LENComp = %x bytes \n ", uiLenComp);
#endif
		// Failure due to mismatch.
		tmp = 0xbf;
		fw_upload_ComWriteChar(LPUART7, &tmp, 1);
		// Start all over again.
		if(flag)
		{
			/* Eliminated longjmp(resync, 1); returning restart status */
			return (uint16)V1_START_INDICATION;
		}
		else
		{
			uiLen = 0;
		}
	}
	return uiLen;
}

/******************************************************************************
 *
 * Name: fw_upload_StoreBytes
 *
 * Description:
 *   This function stores mul-bytes variable to array.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   ulVal: variable to be stored.
 *   uiSize: size of bytes of this variable.
 *   uiStored: array to store variable.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
static void fw_upload_StoreBytes(uint32 ulVal, uint8 uiSize, uint8 *uiStored)
{
	uint8 i;
	for(i = 0; i < uiSize; i ++)
	{
		uiStored[i] = (uint8)(ulVal >> (i * (uint8)8)) & (uint8)0xFF;
	}
}

/******************************************************************************
 *
 * Name: fw_upload_Send_Ack
 *
 * Description:
 *   This function sends ack to per req.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   uiAck: the ack type.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
static void fw_upload_Send_Ack(uint8 uiAck)
{
	uint8 uiAckCrc;
	if((uiAck == V3_REQUEST_ACK) || (uiAck == V3_CRC_ERROR))
	{
#ifdef TEST_CODE
		if (ucRcvdHeader == V3_START_INDICATION)
		{
			// prepare crc for 0x7A or 0x7C
			ucCalCrc[0] = uiAck;
			uiAckCrc = crc8(ucCalCrc, 1);

			if(ucTestCase == 301 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep %dms, NOT send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte", ucTestCase, ucSleepTimeMs, ucRcvdHeader);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 302 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep %dms, NOT send V3_REQUEST_ACK for Header Signature %02X, send CRC byte", ucTestCase, ucSleepTimeMs, ucRcvdHeader);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 303 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte", ucTestCase, ucSleepTimeMs, ucRcvdHeader);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 304 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, send CRC byte", ucTestCase, ucSleepTimeMs, ucRcvdHeader);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 305 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 306 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, NOT send CRC byte", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 307 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 308 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 309 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte, sleep %dms", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 310 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else
			{
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
			}
		}
		else if (ucRcvdHeader == V3_HEADER_DATA_REQ)
		{
			// prepare crc for 0x7A or 0x7C
			ucCalCrc[0] = uiAck;
			uiAckCrc = crc8(ucCalCrc, 1);

			if(ucTestCase == 311 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep %dms, NOT send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte", ucTestCase, ucSleepTimeMs, ucRcvdHeader);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 312 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep %dms, NOT send V3_REQUEST_ACK for Header Signature %02X, send CRC byte", ucTestCase, ucSleepTimeMs, ucRcvdHeader);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 313 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte", ucTestCase, ucSleepTimeMs, ucRcvdHeader);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 314 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep %dms, send V3_REQUEST_ACK for Header Signature %02X, send CRC byte", ucTestCase, ucSleepTimeMs, ucRcvdHeader);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 315 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 316 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, NOT send CRC byte", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 317 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, sleep %dms, send CRC byte", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				ucTestDone = 1;
			}
			else if (ucTestCase == 318 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  NOT send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar((hal_uart_handle_t)s_controllerHciUartHandle, &uiAckCrc, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 319 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, NOT send CRC byte, sleep %dms", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 320 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send V3_REQUEST_ACK for Header Signature %02X, send CRC byte, sleep %dms", ucTestCase, ucRcvdHeader, ucSleepTimeMs);
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else
			{
				fw_upload_ComWriteChar(LPUART7, &uiAck, 1);
				fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
			}
		}
#else

		fw_upload_ComWriteChar(LPUART7, &uiAck, 1);

		// prepare crc for 0x7A or 0x7C
		ucCalCrc[0] = uiAck;
		uiAckCrc = crc8(ucCalCrc, 1);
		fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);

#endif
	}
	else if(uiAck == V3_TIMEOUT_ACK)
	{
		fw_upload_ComWriteChar(LPUART7, &uiAck, 1);

		//prepare crc for 0x7B
		ucCalCrc[0] = uiAck;
		fw_upload_StoreBytes(ulNewOffset, sizeof(ulNewOffset), &ucCalCrc[1]);
		fw_upload_ComWriteChars(LPUART7, (uint8*)&ulNewOffset, 4);
		uiAckCrc = crc8(ucCalCrc, 5);
		fw_upload_ComWriteChar(LPUART7, &uiAckCrc, 1);
	}
	else
	{
		PRINT("\nNon-empty else statement\n");
	}

#ifdef DEBUG_PRINT
	PRINT("\n ===> ACK = %x, CRC = %x \n", uiAck, uiAckCrc);
#endif
}
/******************************************************************************
 *
 * Name: fw_upload_Check_ReqCrc
 *
 * Description:
 *   This function check the request crc.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   uiStr: array to put req header + payload.
 *   uiReq: the request type.
 *
 * Return Value:
 *   result of crc check.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
uint8 fw_upload_Check_ReqCrc(uint8 *uiStr, uint8 uiReq)
{
	uint8 uiCalCrc;

	if(uiReq == V3_HEADER_DATA_REQ)
	{
		uiCalCrc = crc8(uiStr,A6REQ_PAYLOAD_LEN + REQ_HEADER_LEN);
		if(uiCalCrc != uiStr[A6REQ_PAYLOAD_LEN + REQ_HEADER_LEN])
		{
			return FALSE;
		}
	}
	else if(uiReq == V3_START_INDICATION)
	{
		uiCalCrc = crc8(uiStr,AbREQ_PAYLOAD_LEN + REQ_HEADER_LEN);
		if(uiCalCrc != uiStr[AbREQ_PAYLOAD_LEN + REQ_HEADER_LEN])
		{
			return FALSE;
		}
	}
	else
	{
		PRINT("\nNon-empty else statement\n");
	}

	return TRUE;
}

/******************************************************************************
 *
 * Name: fw_upload_WaitFor_Req
 *
 * Description:
 *   This function waits for req from bootcode or helper.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
static uint8 fw_upload_WaitFor_Req(int32 iSecondBaudRate)
{
	uint16  uiChipId=0;
	uint8 uiVersion=0, uiReqCrc=0;
	uint8 uiTmp[20]={0};
	uint8 bCrcMatch = FALSE;
	uint8 status = TRUE;

	if(ucRcvdHeader == V3_HEADER_DATA_REQ)
	{
		//0xA7 <LEN><Offset><ERR><CRC8>
		fw_upload_ComReadChars(LPUART7, (char*)&uiNewLen, 2);
		fw_upload_ComReadChars(LPUART7, (char*)&ulNewOffset, 4);
		fw_upload_ComReadChars(LPUART7, (char*)&uiNewError, 2);
		fw_upload_ComReadChars(LPUART7, (char*)&uiNewCrc, 1);
#ifdef DEBUG_PRINT
		PRINT("\n <=== REQ = 0xA7, Len = %x,Off = %x,Err = %x,CRC = %x\n ",uiNewLen,ulNewOffset,uiNewError,uiNewCrc);
#endif
		//check crc
		uiTmp[0] = V3_HEADER_DATA_REQ;
		fw_upload_StoreBytes((uint32)uiNewLen, sizeof(uiNewLen), &uiTmp[1]);
		fw_upload_StoreBytes(ulNewOffset, sizeof(ulNewOffset), &uiTmp[3]);
		fw_upload_StoreBytes(uiNewError, sizeof(uiNewError), &uiTmp[7]);
		uiTmp[9] = uiNewCrc;
		bCrcMatch = fw_upload_Check_ReqCrc(uiTmp, V3_HEADER_DATA_REQ);

#ifdef TEST_CODE

		if (ucTestCase == 331 && !ucTestDone)
		{
			PRINT("\n         TC-%d:  Simulate Device CRC error on Header Signature 0x%X", ucTestCase, ucRcvdHeader);
			bCrcMatch = 0;
			ucTestDone = 1;
		}

#endif

		if(!bCrcMatch)
		{
#ifdef DEBUG_PRINT
			PRINT("\n === REQ = 0xA7, CRC Mismatched === ");
#endif
			fw_upload_Send_Ack(V3_CRC_ERROR);
			status = FALSE;

		}
	}
	else if(ucRcvdHeader == V3_START_INDICATION)
	{
		//0xAB <CHIP ID> <SW loader REV 1 byte> <CRC8>
		fw_upload_ComReadChars(LPUART7, (char*)&uiChipId, 2);
		fw_upload_ComReadChar(LPUART7, &uiVersion , 1);
		fw_upload_ComReadChar(LPUART7, &uiReqCrc, 1);
		PRINT("\nChipID is : %x, Version is : %x\n",uiChipId, uiVersion);

		//check crc
		uiTmp[0] = V3_START_INDICATION;
		fw_upload_StoreBytes((uint32)uiChipId, sizeof(uiChipId), &uiTmp[1]);
		uiTmp[3] = uiVersion;
		uiTmp[4] = uiReqCrc;
		bCrcMatch = fw_upload_Check_ReqCrc(uiTmp, V3_START_INDICATION);

#ifdef TEST_CODE

		if (ucTestCase == 330 && !ucTestDone)
		{
			PRINT("\n         TC-%d:  Simulate Device CRC error on Header Signature 0x%X", ucTestCase, ucRcvdHeader);
			bCrcMatch = 0;
			ucTestDone = 1;
		}

#endif

		if(bCrcMatch)
		{
#ifdef DEBUG_PRINT
			PRINT("\n === REQ = 0xAB, CRC Matched === ");
#endif
			fw_upload_Send_Ack(V3_REQUEST_ACK);
			if(iSecondBaudRate == 0)
			{
				/* longjmp(resync, 1); eliminated and added return false to start again */
				return FALSE;
			}
		}
		else
		{
#ifdef DEBUG_PRINT
			PRINT("\n === REQ = 0xAB, CRC Mismatched === ");
#endif
			fw_upload_Send_Ack(V3_CRC_ERROR);
			status = FALSE;
			if(iSecondBaudRate == 0)
			{
				/* longjmp(resync, 1); eliminated and added return false to start again */
				return FALSE;
			}
		}
	}
	else
	{
		PRINT("\nNon-empty else statement\n");
	}
	return status;
}

/******************************************************************************
 *
 * Name: fw_upload_GetCmd
 *
 * Description:
 *   This function gets CMD value in the header.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   *buf: buffer that stores header and following data.
 *
 * Return Value:
 *   CMD value part in the buffer.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
static uint32 fw_upload_GetCmd(uint8 *buf)
{
	return (buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[32] << 24));
}

/******************************************************************************
 *
 * Name: fw_upload_GetDataLen
 *
 * Description:
 *   This function gets buf data length.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   *buf: buffer that stores header and following data.
 *
 * Return Value:
 *   length of data part in the buffer.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
static uint16 fw_upload_GetDataLen(uint8 *buf)
{
	return (buf[8] | (buf[9] << 8));
}

/******************************************************************************
 *
 * Name: fw_upload_lenValid
 *
 * Description:
 *   This function validates the length from 5 bytes request.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   ucArray: store the 5 bytes request.
 *
 * Return Value:
 *   uiLenToSend: if the length is valid, get value from ucArray.
 *
 * Notes:
 *   None.
 *

 *****************************************************************************/
uint8 fw_upload_lenValid(uint16 *uiLenToSend, uint8 *ucArray)
{
	uint16 uiLen, uiLenComp;
	uint16 uiXorOfLen = 0xFFFF;
	uiLen = (uint16) ((ucArray[1] & 0xFF) | ((ucArray[2] << 8) & 0xFF00));
	uiLenComp = (uint16) ((ucArray[3] & 0xFF) | ((ucArray[4] << 8) & 0xFF00));
	// LEN valid if len & complement match
	if ((uiLen ^ uiLenComp) == uiXorOfLen) // All 1's
	{
		*uiLenToSend = uiLen;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/******************************************************************************
 *
 * Name: fw_upload_GetHeaderStartBytes
 *
 * Description:
 *   This function gets 0xa5 and it's following 4 bytes length.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *

 *****************************************************************************/
static void fw_upload_GetHeaderStartBytes(uint8 *ucStr)
{
	uint8 ucDone = FALSE, ucStringCnt = 0, i;
	while (!ucDone)
	{
		fw_upload_ComReadChar(LPUART7, &ucRcvdHeader, 1);

		if(ucRcvdHeader == V1_HEADER_DATA_REQ)
		{
			ucStr[ucStringCnt++] = ucRcvdHeader;
			ucDone = TRUE;
#ifdef DEBUG_PRINT
			PRINT("\nReceived 0x%x\n ", ucRcvdHeader);
#endif
		}
		else
		{
			fw_upload_DelayInMs(1);
		}
	}
	//while(!fw_upload_GetBufferSize((hal_uart_handle_t)s_controllerHciUartHandle, &RcvCount));
	for(i = 0; i < 4; i ++)
	{
		fw_upload_ComReadChar(LPUART7, &ucRcvdHeader, 1);
		ucStr[ucStringCnt++] = ucRcvdHeader;
	}
}
/******************************************************************************
 *
 * Name: fw_upload_GetLast5Bytes
 *
 * Description:
 *   This function gets last valid request.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   *buf: buffer that stores header and following data.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *

 *****************************************************************************/
static void fw_upload_GetLast5Bytes(uint8 *buf)
{
	uint8  a5cnt, i;
	uint8  ucTemp[STRING_SIZE];
	uint16 uiTempLen = 0;
	uint16 len = 0;
	int32  fifosize;
	uint8 alla5times = FALSE;

	// initialise
	memset(ucString, 0x00, STRING_SIZE);

	//fw_upload_GetBufferSize((hal_uart_handle_t)s_controllerHciUartHandle, &fifosize);

	fw_upload_GetHeaderStartBytes(ucString);

	if(fw_upload_lenValid(&uiTempLen, ucString) == TRUE)
	{
		//Valid length recieved
#ifdef DEBUG_PRINT
		PRINT("\n Valid length = %d \n", uiTempLen);
#endif
	}

	len = fw_upload_GetDataLen(buf);
	if((fifosize < 6) && ((uiTempLen == HDR_LEN) || (uiTempLen == len)))
	{
#ifdef DEBUG_PRINT
		PRINT("=========>success case\n");
#endif
		uiErrCase = FALSE;
	}
	else // start to get last valid 5 bytes
	{
#ifdef DEBUG_PRINT
		PRINT("=========>fail case\n");
#endif
		while (fw_upload_lenValid(&uiTempLen, ucString) == FALSE)
		{
			fw_upload_GetHeaderStartBytes(ucString);
			fifosize -= 5;
		}
#ifdef DEBUG_PRINT
		PRINT("Error cases 1, 2, 3, 4, 5...\n");
#endif
		if(fifosize > 5)
		{
			fifosize -= 5;
			do
			{
				do
				{
					a5cnt = 0;
					do
					{
						fw_upload_GetHeaderStartBytes(ucTemp);
						fifosize -= 5;
					} while ((fw_upload_lenValid(&uiTempLen, ucTemp) == TRUE) && (!
								alla5times) && (fifosize > 5));
					//if 5bytes are all 0xa5, continue to clear 0xa5
					for (i = 0; i < 5; i ++)
					{
						if (ucTemp[i] == V1_HEADER_DATA_REQ)
						{
							a5cnt ++;
						}
					}
					alla5times = TRUE;
				} while (a5cnt == 5);
#ifdef DEBUG_PRINT
				PRINT("a5 count in last 5 bytes: %d\n", a5cnt);
#endif
				if (fw_upload_lenValid(&uiTempLen, ucTemp) == FALSE)
				{
					for (i = 0; i < (5 - a5cnt); i ++)
					{
						fw_upload_ComReadChar(LPUART7, &ucTemp[i + a5cnt], 1);
					}
					if(a5cnt > 0){
						memcpy(ucString, &ucTemp[a5cnt - 1], (5 - a5cnt)*sizeof(uint8));
					}
				}
				else
				{
					memcpy(ucString, ucTemp, 5*sizeof(uint8));
				}
			} while (fw_upload_lenValid(&uiTempLen, ucTemp) == FALSE);
		}
		uiErrCase = TRUE;
	}
}

/******************************************************************************
 *
 * Name: fw_upload_SendBuffer
 *
 * Description:
 *   This function sends buffer with header and following data.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *      uiLenToSend: len of header request.
 *            ucBuf: the buf to be sent.
 *   uiHighBaudrate: send the buffer for high baud rate change.
 * Return Value:
 *   Returns the len of next header request.
 *
 * Notes:
 *   None.
 *

 *****************************************************************************/
static uint16 fw_upload_SendBuffer(uint16 uiLenToSend, uint8 *ucBuf, uint8 uiHighBaudrate)
{
	uint16 uiBytesToSend = HDR_LEN, uiFirstChunkSent = 0;
	uint16 uiDataLen = 0;
	uint8 ucSentDone = 0;
	uint8 uiValidLen = FALSE;

	uint8 tmp;

	// Get data len
	uiDataLen = fw_upload_GetDataLen(ucBuf);
	// Send buffer
	while (!ucSentDone)
	{
		if (uiBytesToSend == uiLenToSend)
		{
			// All good
			if ((uiBytesToSend == HDR_LEN) && (!b16BytesData))
			{
				if ((uiFirstChunkSent == 0) || ((uiFirstChunkSent == 1) && (uiErrCase == TRUE)))
				{
					// Write first 16 bytes of buffer
#ifdef DEBUG_PRINT
					PRINT("\n====>  Sending first chunk...\n");
					PRINT("\n====>  Sending %d bytes...\n", uiBytesToSend);
#endif
					fw_upload_ComWriteChars(LPUART7, ucBuf, uiBytesToSend);
					if(cmd7_Req == TRUE || EntryPoint_Req == TRUE)
					{
						uiBytesToSend = HDR_LEN;
						uiFirstChunkSent = 1;
					}
					else
					{
						uiBytesToSend = uiDataLen;
						uiFirstChunkSent = 0;
						if(uiBytesToSend == HDR_LEN)
						{
							b16BytesData = TRUE;
						}
					}
				}
				else
				{
					// Done with buffer
					ucSentDone = 1;
					break;
				}
			}
			else
			{
				// Write remaining bytes
#ifdef DEBUG_PRINT
				PRINT("\n====>  Sending %d bytes...\n", uiBytesToSend);
#endif
				if(uiBytesToSend != 0)
				{
					fw_upload_ComWriteChars(LPUART7, &ucBuf[HDR_LEN], uiBytesToSend);
					uiFirstChunkSent = 1;
					// We should expect 16, then next block will start
					uiBytesToSend = HDR_LEN;
					b16BytesData = FALSE;
					if(uiHighBaudrate)
					{
						return 0;
					}
				}
				else  //end of bin download
				{
#ifdef DEBUG_PRINT
					PRINT("\n ========== Download Complete =========\n\n");
#endif
					return 0;
				}
			}
		}
		else
		{
			// Something not good
			if ((uiLenToSend & 0x01) == 0x01)
			{
				// some kind of error
				if (uiLenToSend == (HDR_LEN + 1))
				{
					// Send first chunk again
#ifdef DEBUG_PRINT
					PRINT("\n1. Resending first chunk...\n");
#endif
					fw_upload_ComWriteChars(LPUART7, ucBuf, (uiLenToSend - 1));
					uiBytesToSend = uiDataLen;
					uiFirstChunkSent = 0;
				}
				else if (uiLenToSend == (uiDataLen + 1))
				{
					// Send second chunk again
#ifdef DEBUG_PRINT
					PRINT("\n2. Resending second chunk...\n");
#endif
					fw_upload_ComWriteChars(LPUART7, &ucBuf[HDR_LEN], (uiLenToSend - 1));
					uiBytesToSend = HDR_LEN;
					uiFirstChunkSent = 1;
				}
				else
				{
					PRINT("\nNon-empty terminating else statement\n");
				}
			}
			else if (uiLenToSend == HDR_LEN)
			{
				// Out of sync. Restart sending buffer
#ifdef DEBUG_PRINT
				PRINT("\n3.  Restart sending the buffer...\n");
#endif
				fw_upload_ComWriteChars(LPUART7, ucBuf, uiLenToSend);
				uiBytesToSend = uiDataLen;
				uiFirstChunkSent = 0;
			}
			else
			{
				PRINT("\nNon-empty else statement\n");
			}
		}
		// Get last 5 bytes now
		fw_upload_GetLast5Bytes(ucBuf);
		// Get next length
		uiValidLen = FALSE;
		do
		{
			if (fw_upload_lenValid(&uiLenToSend, ucString) == TRUE)
			{
				// Valid length received
				uiValidLen = TRUE;
#ifdef DEBUG_PRINT
				PRINT("\n Valid length = %d \n", uiLenToSend);
#endif
				// ACK the bootloader
				tmp = V1_REQUEST_ACK;
				fw_upload_ComWriteChar(LPUART7, &tmp, 1);
#ifdef DEBUG_PRINT
				PRINT("\n  BOOT_HEADER_ACK 0x5a sent \n");
#endif
			}
		} while (!uiValidLen);
	}
#ifdef DEBUG_PRINT
	PRINT("\n ========== Buffer is successfully sent =========\n\n");
#endif
	return uiLenToSend;
}

/******************************************************************************
 *
 * Name: fw_upload_V1SendLenBytes
 *
 * Description:
 *   This function sends Len bytes(header+data) to the boot code.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pFileBuffer: bin file buffer being sent.
 *   uiLenTosend: the length will be sent.
 *
 * Return Value:
 *   the 'len' of next header request.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
static uint16 fw_upload_V1SendLenBytes(const uint8* pFileBuffer, uint16 uiLenToSend)
{
	uint16 ucDataLen, uiLen;
	uint32 ulCmd;
#ifdef DEBUG_PRINT
	uint16 i;
#endif
	memset (ucByteBuffer, 0, sizeof(ucByteBuffer));

	cmd7_Req = FALSE;
	EntryPoint_Req = FALSE;

	if(ulCurrFileSize + uiLenToSend > ulTotalFileSize)
		uiLenToSend = (uint16)(ulTotalFileSize - ulCurrFileSize);

	memcpy(&ucByteBuffer[uiLenToSend]-uiLenToSend,pFileBuffer+ulCurrFileSize,uiLenToSend);
	ulCurrFileSize += uiLenToSend;
	ulCmd = fw_upload_GetCmd(ucByteBuffer);
	if(ulCmd == CMD7)
	{
		cmd7_Req = TRUE;
		ucDataLen = 0;
	}
	else
	{
		ucDataLen = fw_upload_GetDataLen(ucByteBuffer);
		memcpy(&ucByteBuffer[uiLenToSend],pFileBuffer+ulCurrFileSize,ucDataLen);
		ulCurrFileSize += ucDataLen;
		if( (ulCurrFileSize < ulTotalFileSize) && (ulCmd == CMD6 || ulCmd == CMD4) )
		{
			EntryPoint_Req = TRUE;
		}
	}

#ifdef DEBUG_PRINT
	PRINT("The buffer is to be sent: %d", uiLenToSend + ucDataLen);
	for(i = 0; i < (uiLenToSend + ucDataLen); i ++)
	{
		if(i % 16 == 0)
		{
			PRINT("\n");
		}
		PRINT(" %02x ", ucByteBuffer[i]);
	}
#endif
	//start to send Temp buffer
	uiLen = fw_upload_SendBuffer(uiLenToSend, ucByteBuffer,FALSE);
	PRINT("File downloaded: %8u:%8ld\r", ulCurrFileSize, ulTotalFileSize);

	return uiLen;
}

/******************************************************************************
 *
 * Name: fw_upload_V3SendLenBytes
 *
 * Description:
 *   This function sends Len bytes to the Helper.    
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pFileBuffer: bin file buffer being sent.
 *   uiLenTosend: the length will be sent.
 *   ulOffset: the offset of current sending.
 *
 * Return Value:
 *   None.
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
static void fw_upload_V3SendLenBytes(const uint8* pFileBuffer, uint16 uiLenToSend, uint32 ulOffset)
{
	// Retransmittion of previous block
	if (ulOffset == ulLastOffsetToSend)
	{
#ifdef DEBUG_PRINT
		PRINT("\nResend offset %d...\n", ulOffset);
#endif
		fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
	}
	else
	{
		// The length requested by the Helper is equal to the Block
		// sizes used while creating the FW.bin. The usual
		// block sizes are 128, 256, 512.
		// uiLenToSend % 16 == 0. This means the previous packet
		// was error free (CRC ok) or this is the first packet received.
		//  We can clear the ucByteBuffer and populate fresh data.
		memset (ucByteBuffer, 0, MAX_LENGTH* sizeof(uint8));
		memcpy(ucByteBuffer,pFileBuffer+ulOffset - change_baudrata_buffer_len - cmd7_change_timeout_len,uiLenToSend);
		ulCurrFileSize = ulOffset - change_baudrata_buffer_len - cmd7_change_timeout_len + uiLenToSend;
#ifdef TEST_CODE

		if (uiLenToSend == HDR_LEN)
		{
			if (ucTestCase == 321 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleeping for %dms before sending %d bytes HEADER", ucTestCase, ucSleepTimeMs, uiLenToSend);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
				ucTestDone = 1;
			}
			else if (ucTestCase == 322 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send only 8 bytes of 16-byte HEADER, then sleep for %dms", ucTestCase, ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, 8);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 323 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send 8 bytes of 16-byte HEADER, sleep for %dms, then send remaining 8 bytes HEADER", ucTestCase, ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, 8);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, &ucByteBuffer[8], 8);
				ucTestDone = 1;
			}
			else if (ucTestCase == 324 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send 8 bytes of 16-byte HEADER, sleep for %dms, then send full 16 bytes HEADER", ucTestCase, ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, 8);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
				ucTestDone = 1;
			}
			else if (ucTestCase == 325 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep for %dms, and NOT sending 16-bytes HEADER, but send DATA", ucTestCase, ucSleepTimeMs);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 326 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send 16-byte HEADER with last byte changed to 7C", ucTestCase);
				myCrcCorrByte = ucByteBuffer[uiLenToSend - 1];
				ucByteBuffer[uiLenToSend - 1] = 0x7c;
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
				ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
				ucTestDone = 1;
			}
			else if (ucTestCase == 327 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send 16-byte HEADER with last byte changed to 7C, then sleep for %dms", ucTestCase, ucSleepTimeMs);
				myCrcCorrByte = ucByteBuffer[uiLenToSend - 1];
				ucByteBuffer[uiLenToSend - 1] = 0x7c;
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
				ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 328 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep for %dms, and NOT sending 16-bytes HEADER, and NOT sending DATA", ucTestCase, ucSleepTimeMs);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else
			{
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
			}
		}
		else
		{
			if (ucTestCase == 301 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleeping for %dms before sending %d bytes DATA", ucTestCase, ucSleepTimeMs, uiLenToSend);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
				ucTestDone = 1;
			}
			else if (ucTestCase == 302 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send only first 8 bytes of %d bytes of DATA, then sleep for %dms", ucTestCase, uiLenToSend, ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, 8);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 303 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send first 8 bytes of %d bytes DATA, sleep for %dms, then send remaining %d DATA", ucTestCase, uiLenToSend, ucSleepTimeMs, uiLenToSend-8);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, 8);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, &ucByteBuffer[8], uiLenToSend-8);
				ucTestDone = 1;
			}
			else if (ucTestCase == 304 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send first 8 bytes of %d bytes DATA, sleep for %dms, then send full %d bytes DATA", ucTestCase, uiLenToSend, ucSleepTimeMs, uiLenToSend);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, 8);
				fw_upload_DelayInMs(ucSleepTimeMs);
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
				ucTestDone = 1;
			}
			else if (ucTestCase == 305 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Sleep for %dms, and NOT sending %d bytes DATA", ucTestCase, ucSleepTimeMs, uiLenToSend);
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else if (ucTestCase == 306 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send %d bytes DATA with last byte changed to 7C", ucTestCase, uiLenToSend);
				myCrcCorrByte = ucByteBuffer[uiLenToSend - 1];
				ucByteBuffer[uiLenToSend - 1] = 0x7c;
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
				ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
				ucTestDone = 1;
			}
			else if (ucTestCase == 307 && !ucTestDone)
			{
				PRINT("\n         TC-%d:  Send %d bytes DATA with last byte changed to 7C, then sleep for %dms", ucTestCase, uiLenToSend, ucSleepTimeMs);
				myCrcCorrByte = ucByteBuffer[uiLenToSend - 1];
				ucByteBuffer[uiLenToSend - 1] = 0x7c;
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
				ucByteBuffer[uiLenToSend - 1] = myCrcCorrByte;
				fw_upload_DelayInMs(ucSleepTimeMs);
				ucTestDone = 1;
			}
			else
			{
				fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);
			}
		}

#else

		fw_upload_ComWriteChars(LPUART7, ucByteBuffer, uiLenToSend);

#endif
		ulLastOffsetToSend = ulOffset;
	}
}

/******************************************************************************
 *
 * Name: fw_Change_Baudrate
 *
 * Description:
 *   This function changes the baud rate of bootrom.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pPortName:        Serial port value.
 *   iFirstBaudRate:   The default baud rate of boot rom.
 *   iSecondBaudRate:  The chaned baud rate.
 *
 * Return Value:
 *   TRUE:            Change baud rate successfully
 *   FALSE:           Change baud rate unsuccessfully
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
static int32 fw_Change_Baudrate(int32 iFirstBaudRate,int32 iSecondBaudRate, uint8 bFirstWaitHeaderSignature)
{
	uint8 uartConfig[60];
	uint8 ucBuffer[80];
	uint32 j;
	uint32 uartClk = 0x00C00000;
	uint32 uartDiv = 0x1;
	uint16 uiLenToSend = 0;
	uint32 uiCrc = 0;
	uint32 uiLen = 0;
	uint8 bRetVal = FALSE;
	int32  ucResult = -1;
	uint8 ucLoadPayload = 0;
	uint32 waitHeaderSigTime = 0;
	uint8 uiReUsedInitBaudrate = FALSE;
	uint32 headLen = 0;
	uint8 flag = FALSE;

	uint8 ucParity = 0;
	uint8 ucStopBits = 1;
	uint8 ucByteSizeinBits = sizeof(uint8) * 8;

	uint32 mcr = MCR;
	uint32 init = INIT;
	uint32 icr = ICR;
	uint32 fcr = FCR;
	uint32 brAddr = CLKDIVAddr;
	uint32 divAddr = UARTDIVAddr;
	uint32 mcrAddr = UARTMCRAddr;
	uint32 reInitAddr = UARTREINITAddr;
	uint32 icrAddr = UARTICRAddr;
	uint32 fcrAddr = UARTFCRAddr;

	for(j= 0; j < sizeof(UartCfgTbl)/sizeof(UART_BAUDRATE); j++)
	{
		if(iSecondBaudRate == UartCfgTbl[j].iBaudRate)
		{
			uartDiv = UartCfgTbl[j].iUartDivisor;
			uartClk = UartCfgTbl[j].iClkDivisor;
			ucResult = 0;
			break;
		}
	}

	if(ucResult != 0)
	{
		return ucResult;
	}

	// Generate CRC value for CMD5 payload
	memcpy(uartConfig+uiLen,&brAddr,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&uartClk,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&divAddr,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&uartDiv,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&mcrAddr,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&mcr,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&reInitAddr,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&init,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&icrAddr,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&icr,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&fcrAddr,4);
	uiLen += 4;
	memcpy(uartConfig+uiLen,&fcr,4);
	uiLen += 4;
	headLen = uiLen+4;

	fw_upload_gen_crc_table();
	memcpy(m_Buffer_CMD5_Header + 8, &headLen,4);
	uiCrc = (uint32) fw_upload_update_crc((unsigned long)0, (char*)m_Buffer_CMD5_Header, (int)12);
	uiCrc = (uint32)SWAPL(uiCrc);
	memcpy(m_Buffer_CMD5_Header+12,&uiCrc ,4);
	uiCrc = (uint32)fw_upload_update_crc((unsigned long)0, (char*)uartConfig,( int)uiLen);
	uiCrc = (uint32)SWAPL(uiCrc);
	memcpy(uartConfig+uiLen,&uiCrc ,4);
	uiLen += 4;

	while (!bRetVal)
	{
		if(ucLoadPayload != 0 || uiReUsedInitBaudrate)
		{
			waitHeaderSigTime = TIMEOUT_VAL_MILLISEC;
		}
		else
		{
			waitHeaderSigTime = 0;
		}
		// Wait to Receive 0xa5, 0xaa, 0xab, 0xa7
		//If the second baudrate is used, wait for 2s to check 0xa5
		flag = fw_upload_WaitForHeaderSignature(waitHeaderSigTime);
		if(bFirstWaitHeaderSignature && flag)
		{
			if(ucLoadPayload)
			{
				if(uiProVer == Ver3)
				{
					change_baudrata_buffer_len = (uint32)HDR_LEN + (uint32)uiNewLen;
				}
				break;
			}
		}
		else
		{
			if(uiReUsedInitBaudrate)
			{
				ucResult = -2;
				return ucResult;
			}
			if(ucLoadPayload)
			{
				// If 0xa5 or 0xa7 is not received by using the second baudrate, change baud rate to the first baudrate.
				fw_upload_CloseUart(LPUART7);

				lpuart_config_t config;
				LPUART_GetDefaultConfig(&config);
				config.baudRate_Bps = 115200;
				config.enableTx     = true;
				config.enableRx     = true;

				fw_upload_init_uart(LPUART7, &config, CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart7));

				ucLoadPayload = 0;
				uiReUsedInitBaudrate = TRUE;
				continue;
			}
		}
		if(uiProVer == Ver1)
		{
			uiLenToSend = fw_upload_WaitFor_Len(0);
			if (uiLenToSend == V1_START_INDICATION)  {
				return (int32)V1_START_INDICATION;
			}
			if(uiLenToSend == 0)
			{
				continue;
			}
			else if(uiLenToSend == HDR_LEN)
			{
				// Download CMD5 header and Payload packet.
				memcpy(ucBuffer,m_Buffer_CMD5_Header,HDR_LEN);
				memcpy(ucBuffer + HDR_LEN,uartConfig,uiLen);
				fw_upload_SendBuffer(uiLenToSend,ucBuffer,TRUE);
				fw_upload_CloseUart(LPUART7);

				lpuart_config_t config;
				LPUART_GetDefaultConfig(&config);
				config.baudRate_Bps = 115200;
				config.enableTx     = true;
				config.enableRx     = true;

				fw_upload_init_uart(LPUART7, &config, CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart7));

				ucLoadPayload = 1;
			}
			else
			{
				// Download CMD5 header and Payload packet
				fw_upload_ComWriteChars(LPUART7, uartConfig, uiLen);
				fw_upload_CloseUart(LPUART7);

				lpuart_config_t config;
				LPUART_GetDefaultConfig(&config);
				config.baudRate_Bps = 115200;
				config.enableTx     = true;
				config.enableRx     = true;

				fw_upload_init_uart(LPUART7, &config, CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart7));
				ucLoadPayload = 1;
			}
		}
		else if(uiProVer == Ver3)
		{
			flag = fw_upload_WaitFor_Req(iSecondBaudRate);
			if(!bFirstWaitHeaderSignature || flag)
			{
				if(uiNewLen != 0 && ucRcvdHeader == V3_HEADER_DATA_REQ)
				{
					if(uiNewError == 0)
					{
						fw_upload_Send_Ack(V3_REQUEST_ACK);
						bFirstWaitHeaderSignature = TRUE;

						if(uiNewLen == HDR_LEN)
						{
							fw_upload_ComWriteChars(LPUART7, m_Buffer_CMD5_Header, uiNewLen);
						}
						else
						{
							fw_upload_ComWriteChars(LPUART7, uartConfig, uiNewLen);
							//Reopen Uart by using the second baudrate after downloading the payload.
							fw_upload_CloseUart(LPUART7);

							lpuart_config_t config;
							LPUART_GetDefaultConfig(&config);
							config.baudRate_Bps = 115200;
							config.enableTx     = true;
							config.enableRx     = true;

							fw_upload_init_uart(LPUART7, &config, CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart7));
							ucLoadPayload = 1;
						}
					}
					else //NAK,TIMEOUT,INVALID COMMAND...
					{
						fw_upload_Send_Ack(V3_TIMEOUT_ACK);
					}
				}
			}
		}
		else
		{
			PRINT("\nNon-empty terminating else statement\n");
		}
	}
	return ucResult;
}

/******************************************************************************
 *
 * Name: fw_Change_Timeout
 *
 * Description:
 *   This function changes timeout value of boot loader
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pPortName:       Com port number.

 * Return Value:
 *   the status  of changing timeout value
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
static int32 fw_Change_Timeout()
{

	int32 Status = -1;
	uint8 bFirst = TRUE;
	uint8 bRetVal = FALSE;
	uint8 reTryNumber  = 0;
	fw_upload_gen_crc_table();

	while (!bRetVal)
	{
		if(fw_upload_WaitForHeaderSignature(TIMEOUT_VAL_MILLISEC))
		{
			if(uiProVer == Ver3)
			{
				if(fw_upload_WaitFor_Req(1))
				{
					if(uiNewLen != 0)
					{
						if(uiNewError == 0)
						{
#ifdef DEBUG_PRINT
							PRINT("\n === Succ: REQ = 0xA7, Errcode = 0 ");
#endif
							if(bFirst || ulLastOffsetToSend == ulNewOffset)
							{
								fw_upload_Send_Ack(V3_REQUEST_ACK);
								fw_upload_ComWriteChars(LPUART7, m_Buffer_CMD7_ChangeTimeoutValue, uiNewLen);
								ulLastOffsetToSend = ulNewOffset;
								bFirst = FALSE;
							}
							else
							{
								bRetVal = TRUE;
								Status = 0;

							}
						}
						else
						{
							if(reTryNumber < 6)
							{
								fw_upload_Send_Ack(V3_TIMEOUT_ACK);
								reTryNumber++;
							}
							else
							{
								bRetVal = TRUE;
							}

						}
					}

				}

			}
			if(uiProVer == Ver1)
			{
				Status = 1;
				break;
			}

		}
	}
	return Status;
}


/******************************************************************************
 *
 * Name: fw_upload_FW
 *
 * Description:
 *   This function performs the task of FW load over UART.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   pPortName:       Com port number.
 *   iBaudRate:       the initial baud rate.
 *   pFileName:       the file name for downloading.
 *   iSecondBaudRate: the second baud rate.
 *
 * Return Value:
 *   the error code of downloading
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/ 
static uint32 fw_upload_FW(int32 iBaudRate, int32 iSecondBaudRate, const unsigned char *wlan_fw_bin, unsigned int wlan_fw_bin_len)
{ 
	uint16 uiLenToSend = 0;

	uint8 bRetVal = 0;
	uint8 flag = FALSE;
	int32  result = 0;

	uint32 ulReadLen = 0;

	uint8 bFirstWaitHeaderSignature = TRUE;
	// UART specific variables
	uint8 ucParity = 0;
	uint8 ucStopBits = 1;
	uint8 ucByteSizeinBits = sizeof(uint8) * 8;

	if (uiReDownload == FALSE)
	{
		lpuart_config_t config;
		LPUART_GetDefaultConfig(&config);
		config.baudRate_Bps = 115200;
		config.enableTx     = true;
		config.enableRx     = true;

		fw_upload_init_uart(LPUART7, &config, CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart7));
	}

	result = fw_Change_Timeout();
	if(result == 0)
	{
		cmd7_change_timeout_len = HDR_LEN;
		bFirstWaitHeaderSignature = FALSE;
	}

	if(iSecondBaudRate != 0)
	{
		uint32 j = 0;
		result = fw_Change_Baudrate(iBaudRate,iSecondBaudRate,bFirstWaitHeaderSignature);
		switch(result)
		{
			case -1:
				PRINT("\nSecond baud rate %d is not support\n",iSecondBaudRate);
				PRINT("\nFw loader only supports the baud rate as");
				for(j = 0; j < sizeof(UartCfgTbl)/sizeof(UART_BAUDRATE); j++)
				{
					PRINT(" %d ",UartCfgTbl[j].iBaudRate);
				}
				PRINT("\n");
				break;
			case -2:
				PRINT("\n0xa5 or 0xaa is not received after changing baud rate in 2s.\n");
				break;
			default:
				break;
		}
		if(result != 0)
		{
			return CHANGE_BAUDRATE_FAIL;
		}
	}

	ulTotalFileSize = (long)wlan_fw_bin_len;
	if (ulTotalFileSize <= 0)
	{
		PRINT("\nError:Download Size is 0\n");
		return FILESIZE_IS_ZERO;
	}

	ulCurrFileSize = 0;

	while (!bRetVal)
	{
		// Wait to Receive 0xa5, 0xaa, 0xab, 0xa7
		flag = fw_upload_WaitForHeaderSignature(TIMEOUT_VAL_MILLISEC);
		if(!iSecondBaudRate && !flag)
		{
			PRINT("\n0xa5,0xaa,0xab or 0xa7 is not received in %d ms\n", TIMEOUT_VAL_MILLISEC);
			return HEADER_SIGNATURE_TIMEOUT;
		}
		iSecondBaudRate = 0;

		if(uiProVer == Ver1)
		{
			// Read the 'Length' bytes requested by Helper
			uiLenToSend = fw_upload_WaitFor_Len(1);
			if (uiLenToSend == V1_START_INDICATION) {
				//  restart again
				continue;
			}
			do
			{
				uiLenToSend = fw_upload_V1SendLenBytes(wlan_fw_bin, uiLenToSend);
			} while(uiLenToSend != 0);
			// If the Length requested is 0, download is complete.
			if (uiLenToSend == 0)
			{
				bRetVal = TRUE;
				break;
			}
		}
		else if(uiProVer == Ver3)
		{
			// check if restart required
			if(ucRcvdHeader == V3_START_INDICATION)
			{
				fw_upload_WaitFor_Req(0);
				continue;
			}
			else if(fw_upload_WaitFor_Req(0))
			{
				if(uiNewLen != 0)
				{
					if(uiNewError == 0)
					{
#ifdef DEBUG_PRINT
						PRINT("\n === Succ: REQ = 0xA7, Errcode = 0 ");
#endif
						fw_upload_Send_Ack(V3_REQUEST_ACK);
						fw_upload_V3SendLenBytes(wlan_fw_bin, uiNewLen, ulNewOffset);

#ifdef DEBUG_PRINT
						PRINT("\n sent %d bytes..\n", uiNewLen);
#endif
					}
					else //NAK,TIMEOUT,INVALID COMMAND...
					{
#ifdef DEBUG_PRINT
						uint8 i;
						PRINT("\n === Fail: REQ = 0xA7, Errcode != 0 ");
						for(i = 0; i < 7; i ++)
						{
							uiErrCnt[i] += (uiNewError >> i) & 0x1;
						}
#endif
						fw_upload_Send_Ack(V3_TIMEOUT_ACK);
						if(uiNewError & BT_MIC_FAIL_BIT)
						{
							change_baudrata_buffer_len = 0;
							ulCurrFileSize = 0;
							ulLastOffsetToSend = 0xFFFF;
						}
					}
				}
				else
				{
					/* check if download complete */
					if(uiNewError == 0)
					{
						fw_upload_Send_Ack(V3_REQUEST_ACK);
						bRetVal = TRUE;
						break;
					}
					else if(uiNewError & BT_MIC_FAIL_BIT)
					{
#ifdef DEBUG_PRINT
						uiErrCnt[7] += 1;
#endif
						fw_upload_Send_Ack(V3_REQUEST_ACK);
						change_baudrata_buffer_len = 0;
						ulCurrFileSize = 0;
						ulLastOffsetToSend = 0xFFFF;
					}
					else if (uiNewError & TIMEOUT_REC_ACK_BIT) {
						// Send ACK when Timeout & Len=0 .
						fw_upload_Send_Ack(V3_TIMEOUT_ACK);
					}
					else
					{
						PRINT("\nNon-empty terminating else statement\n");
					}
				}
			}
#ifdef TEST_CODE
			PRINT("\n");
#endif
			PRINT("File downloaded: %8u:%8ld\r", ulCurrFileSize, ulTotalFileSize);
		}
		else
		{
			PRINT("\nNot downloaded\n");
		}
	}

	return DOWNLOAD_SUCCESS;
}


/******************************************************************************
 *
 * Name: main
 *
 * Description:
 *   Main Entry point of the application.
 *
 * Conditions For Use:
 *   None.
 *
 * Arguments:
 *   None.
 *
 * Return Value:
 *   None 
 *
 * Notes:
 *   None.
 *
 *****************************************************************************/
int uart_fw_download(const unsigned char *wlan_fw_bin, unsigned int wlan_fw_bin_len)
{
	int32 iBaudrate = 115200;

	const char *pVersionName = VERSION;
	uint64 endTime;
	uint64 start;
	uint64 cost;
	uint32 ulResult;
	int32 iSecondBaudrate = 0;

	// Initialize the function pointers.
	fw_upload_io_func_init();
	init_crc8();
	start = fw_upload_GetTime();

	//iSecondBaudrate = 3000000;

	do
	{
		ulResult = fw_upload_FW(iBaudrate, iSecondBaudrate, wlan_fw_bin, wlan_fw_bin_len);

		if(ulResult == 0)
		{
			PRINT("\nDownload Complete\n");
			cost = fw_upload_GetTime() - start;
			PRINT("time:%llu\n",cost);
			if(uiProVer == Ver3 && iSecondBaudrate != 0)
			{
				fw_upload_DelayInMs(100);
				endTime = fw_upload_GetTime() + 2* MAX_CTS_TIMEOUT;
			}
			else
			{
				fw_upload_DelayInMs(500);
				endTime = fw_upload_GetTime() + MAX_CTS_TIMEOUT;
			}

			do
			{
			} while (endTime > fw_upload_GetTime());
			PRINT("Error code is %d\n",ulResult);

			uiReDownload =FALSE;
		}
		else
		{
			PRINT("\nDownload Error\n");
			PRINT("Error code is %d\n",ulResult);
			uiReDownload = FALSE;
		}
	} while (uiReDownload);

	if(uiReDownload==FALSE)
	{
		fw_upload_CloseUart(LPUART7);
		//fw_upload_DelayInMs(100);
	}
	return 0;
}

#endif
