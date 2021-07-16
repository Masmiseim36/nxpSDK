/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016, 2018, 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_host_config.h"
#include "usb_host.h"
#include "usb_host_msd.h"
#include "host_msd_fatfs.h"
#include "ff.h"
#include "diskio.h"
#include "stdio.h"
#include "fsl_device_registers.h"

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_INFO
#endif

#define LIBRARY_LOG_NAME         ( "MSD_FATFS" )
#include "iot_logging_setup.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if MSD_FATFS_THROUGHPUT_TEST_ENABLE
#include "fsl_device_registers.h"
#define THROUGHPUT_BUFFER_SIZE (64 * 1024) /* throughput test buffer */
#define MCU_CORE_CLOCK (120000000)         /* mcu core clock, user need to configure it. */
#endif                                     /* MSD_FATFS_THROUGHPUT_TEST_ENABLE */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void USB_HostClockInit(void);

extern void USB_HostIsrEnable(void);

/*!
 * @brief host msd control transfer callback.
 *
 * This function is used as callback function for control transfer .
 *
 * @param param      the host msd fatfs instance pointer.
 * @param data       data buffer pointer.
 * @param dataLength data length.
 * @status           transfer result status.
 */
void USB_HostMsdControlCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status);

#if ((defined MSD_FATFS_THROUGHPUT_TEST_ENABLE) && (MSD_FATFS_THROUGHPUT_TEST_ENABLE))
/*!
 * @brief msd fatfs test code execute done.
 */
static void USB_HostMsdFatfsTestDone(void);
#endif

#if ((defined MSD_FATFS_THROUGHPUT_TEST_ENABLE) && (MSD_FATFS_THROUGHPUT_TEST_ENABLE))
/*!
 * @brief host msd fatfs throughput test.
 *
 * @param msdFatfsInstance   the host fatfs instance pointer.
 */
static void USB_HostMsdFatfsThroughputTest(usb_host_msd_fatfs_instance_t *msdFatfsInstance);

#else

/*!
 * @brief display file information.
 */
static void USB_HostMsdFatfsDisplayFileInfo(FILINFO *fileInfo);

/*!
 * @brief list files and sub-directory in one directory, the function don't check all sub-directories recursively.
 */
static FRESULT USB_HostMsdFatfsListDirectory(const TCHAR *path);

/*!
 * @brief forward function pointer for fatfs f_forward function.
 *
 * @param data_ptr   forward data pointer.
 * @param dataLength data length.
 */
#if _USE_FORWARD && _FS_TINY
static uint32_t USB_HostMsdFatfsForward(const uint8_t *data_ptr, uint32_t dataLength);
#endif

/*!
 * @brief host msd fatfs test.
 *
 * This function implements msd fatfs test.
 *
 * @param msdFatfsInstance   the host fatfs instance pointer.
 */
static void USB_HostMsdFatfsMount(usb_host_msd_fatfs_instance_t *msdFatfsInstance);

#endif /* MSD_FATFS_THROUGHPUT_TEST_ENABLE */

#if ((defined USB_HOST_CONFIG_COMPLIANCE_TEST) && (USB_HOST_CONFIG_COMPLIANCE_TEST))
extern usb_status_t USB_HostTestModeInit(usb_device_handle deviceHandle);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

usb_host_handle g_HostHandle;

/*! @brief msd class handle array for fatfs */
extern usb_host_class_handle g_UsbFatfsClassHandle;

usb_host_msd_fatfs_instance_t g_MsdFatfsInstance; /* global msd fatfs instance */
static FATFS fatfs;
/* control transfer on-going state. It should set to 1 when start control transfer, it is set to 0 in the callback */
volatile uint8_t controlIng;
/* control transfer callback status */
volatile usb_status_t controlStatus;

#if MSD_FATFS_THROUGHPUT_TEST_ENABLE
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint32_t testThroughputBuffer[THROUGHPUT_BUFFER_SIZE / 4]; /* the buffer for throughput test */
uint32_t testSizeArray[] = {20 * 1024, 20 * 1024};                /* test time and test size (uint: K)*/
#else
#if FF_USE_MKFS
/*fix build warning: declared but never used.*/
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t testBuffer[(FF_MAX_SS > 256) ? FF_MAX_SS : 256]; /* normal test buffer */
#endif
#endif /* MSD_FATFS_THROUGHPUT_TEST_ENABLE */

/*******************************************************************************
 * Code
 ******************************************************************************/

static usb_status_t USB_HostEvent(usb_device_handle deviceHandle,
                                  usb_host_configuration_handle configurationHandle,
                                  uint32_t eventCode)
{
#if ((defined USB_HOST_CONFIG_COMPLIANCE_TEST) && (USB_HOST_CONFIG_COMPLIANCE_TEST))
    usb_host_configuration_t *configuration;
    usb_status_t status1;
    usb_status_t status2;
    uint8_t interfaceIndex = 0;
#endif
    usb_status_t status = kStatus_USB_Success;
    switch (eventCode & 0x0000FFFFU)
    {
        case kUSB_HostEventAttach:
#if ((defined USB_HOST_CONFIG_COMPLIANCE_TEST) && (USB_HOST_CONFIG_COMPLIANCE_TEST))
            status1 = USB_HostTestEvent(deviceHandle, configurationHandle, eventCode);
            status2 = USB_HostMsdEvent(deviceHandle, configurationHandle, eventCode);
            if ((status1 == kStatus_USB_NotSupported) && (status2 == kStatus_USB_NotSupported))
            {
                status = kStatus_USB_NotSupported;
            }
#else
            status = USB_HostMsdEvent(deviceHandle, configurationHandle, eventCode);
#endif
            break;

        case kUSB_HostEventNotSupported:
#if ((defined USB_HOST_CONFIG_COMPLIANCE_TEST) && (USB_HOST_CONFIG_COMPLIANCE_TEST))
            configuration = (usb_host_configuration_t *)configurationHandle;
            for (interfaceIndex = 0; interfaceIndex < configuration->interfaceCount; ++interfaceIndex)
            {
                if (((usb_descriptor_interface_t *)configuration->interfaceList[interfaceIndex].interfaceDesc)
                        ->bInterfaceClass == 9U) /* 9U is hub class code */
                {
                    break;
                }
            }

            if (interfaceIndex < configuration->interfaceCount)
            {
                IotLogError("unsupported hub");
            }
            else
            {
                IotLogError("Unsupported Device");
            }
#else
            IotLogError("Unsupported Device");
#endif
            break;

        case kUSB_HostEventEnumerationDone:
        	IotLogDebug("New USB MSD disk arrived");
#if ((defined USB_HOST_CONFIG_COMPLIANCE_TEST) && (USB_HOST_CONFIG_COMPLIANCE_TEST))
            status1 = USB_HostTestEvent(deviceHandle, configurationHandle, eventCode);
            status2 = USB_HostMsdEvent(deviceHandle, configurationHandle, eventCode);
            if ((status1 != kStatus_USB_Success) && (status2 != kStatus_USB_Success))
            {
                status = kStatus_USB_Error;
            }
#else
            status = USB_HostMsdEvent(deviceHandle, configurationHandle, eventCode);
#endif
            break;

        case kUSB_HostEventDetach:
#if ((defined USB_HOST_CONFIG_COMPLIANCE_TEST) && (USB_HOST_CONFIG_COMPLIANCE_TEST))
            status1 = USB_HostTestEvent(deviceHandle, configurationHandle, eventCode);
            status2 = USB_HostMsdEvent(deviceHandle, configurationHandle, eventCode);
            if ((status1 != kStatus_USB_Success) && (status2 != kStatus_USB_Success))
            {
                status = kStatus_USB_Error;
            }
#else
            status = USB_HostMsdEvent(deviceHandle, configurationHandle, eventCode);
#endif
            break;

        case kUSB_HostEventEnumerationFail:
            IotLogError("Enumeration failed");
            break;

        default:
            break;
    }
    return status;
}

void USB_HostMsdControlCallback(void *param, uint8_t *data, uint32_t dataLength, usb_status_t status)
{
    usb_host_msd_fatfs_instance_t *msdFatfsInstance = (usb_host_msd_fatfs_instance_t *)param;

    if (msdFatfsInstance->runWaitState == kUSB_HostMsdRunWaitSetInterface) /* set interface finish */
    {
        msdFatfsInstance->runWaitState = kUSB_HostMsdRunIdle;
        msdFatfsInstance->runState     = kUSB_HostMsdRunMassStorageMount;
    }
    controlIng    = 0;
    controlStatus = status;
}

#if ((defined MSD_FATFS_THROUGHPUT_TEST_ENABLE) && (MSD_FATFS_THROUGHPUT_TEST_ENABLE))

static void USB_HostMsdFatfsTestDone(void)
{
    IotLogDebug("............................test done......................");
}

static void USB_HostMsdFatfsThroughputTest(usb_host_msd_fatfs_instance_t *msdFatfsInstance)
{
    uint64_t totalTime;
    FRESULT fatfsCode;
    FIL file;
    uint32_t resultSize;
    uint32_t testSize;
    uint8_t testIndex;
    char test_file_name[30];

    /* time delay (~100ms) */
    for (resultSize = 0; resultSize < 400000; ++resultSize)
    {
        __NOP();
    }

    IotLogDebug("............................fatfs test.....................");
    CoreDebug->DEMCR |= (1 << CoreDebug_DEMCR_TRCENA_Pos);

    for (testSize = 0; testSize < (THROUGHPUT_BUFFER_SIZE / 4); ++testSize)
    {
        testThroughputBuffer[testSize] = testSize;
    }

    sprintf(test_file_name, "%c:", USBDISK + '0');
    fatfsCode = f_mount(&fatfs, test_file_name, 1);
    if (fatfsCode)
    {
        IotLogDebug("fatfs mount error");
        USB_HostMsdFatfsTestDone();
        return;
    }

    sprintf(test_file_name, "%c:/thput.dat", USBDISK + '0');
    IotLogDebug("throughput test:");
    for (testIndex = 0; testIndex < (sizeof(testSizeArray) / 4); ++testIndex)
    {
        fatfsCode = f_unlink(test_file_name); /* delete the file if it is existed */
        if ((fatfsCode != FR_OK) && (fatfsCode != FR_NO_FILE))
        {
            USB_HostMsdFatfsTestDone();
            return;
        }

        fatfsCode = f_open(&file, test_file_name, FA_WRITE | FA_READ | FA_CREATE_ALWAYS); /* create one new file */
        if (fatfsCode)
        {
            USB_HostMsdFatfsTestDone();
            return;
        }

        totalTime = 0;
        testSize  = testSizeArray[testIndex] * 1024;
        while (testSize)
        {
            if (msdFatfsInstance->deviceState != kStatus_DEV_Attached)
            {
                USB_HostMsdFatfsTestDone();
                return;
            }
            DWT->CYCCNT = 0;
            DWT->CTRL |= (1 << DWT_CTRL_CYCCNTENA_Pos);
            fatfsCode = f_write(&file, testThroughputBuffer, THROUGHPUT_BUFFER_SIZE, &resultSize);
            if (fatfsCode)
            {
                IotLogDebug("write error");
                f_close(&file);
                USB_HostMsdFatfsTestDone();
                return;
            }
            totalTime += DWT->CYCCNT;
            DWT->CTRL &= ~(1 << DWT_CTRL_CYCCNTENA_Pos);
            testSize -= THROUGHPUT_BUFFER_SIZE;
        }
        testSize = testSizeArray[testIndex];
        IotLogDebug("    write %dKB data the speed is %d KB/s", testSize,
                 (uint32_t)((uint64_t)testSize * (uint64_t)MCU_CORE_CLOCK / (uint64_t)totalTime));

        fatfsCode = f_lseek(&file, 0);
        if (fatfsCode)
        {
            USB_HostMsdFatfsTestDone();
            return;
        }
        totalTime = 0;
        testSize  = testSizeArray[testIndex] * 1024;
        while (testSize)
        {
            if (msdFatfsInstance->deviceState != kStatus_DEV_Attached)
            {
                USB_HostMsdFatfsTestDone();
                return;
            }
            DWT->CYCCNT = 0;
            DWT->CTRL |= (1 << DWT_CTRL_CYCCNTENA_Pos);
            fatfsCode = f_read(&file, testThroughputBuffer, THROUGHPUT_BUFFER_SIZE, &resultSize);
            if (fatfsCode)
            {
                IotLogDebug("read error");
                f_close(&file);
                USB_HostMsdFatfsTestDone();
                return;
            }
            totalTime += DWT->CYCCNT;
            DWT->CTRL &= ~(1 << DWT_CTRL_CYCCNTENA_Pos);
            testSize -= THROUGHPUT_BUFFER_SIZE;
        }
        testSize = testSizeArray[testIndex];
        IotLogDebug("    read %dKB data the speed is %d KB/s", testSize,
                 (uint32_t)((uint64_t)testSize * (uint64_t)MCU_CORE_CLOCK / (uint64_t)totalTime));

        fatfsCode = f_close(&file);
        if (fatfsCode)
        {
            USB_HostMsdFatfsTestDone();
            return;
        }
    }

    USB_HostMsdFatfsTestDone();
}

#else

static void USB_HostMsdFatfsDisplayFileInfo(FILINFO *fileInfo)
{
    char *fileName;
#if _USE_LFN
    fileName = (fileInfo->lfname[0] ? fileInfo->lfname : fileInfo->fname;
#else
    fileName = fileInfo->fname;
#endif /* _USE_LFN */
    /* note: if this file/directory don't have one attribute, '_' replace the attribute letter ('R' - readonly, 'H' - hide, 'S' - system) */
    IotLogDebug("    %s - %c%c%c - %s - %dBytes - %d-%d-%d %d:%d:%d", (fileInfo->fattrib & AM_DIR) ? "dir" : "fil",
             (fileInfo->fattrib & AM_RDO) ? 'R' : '_',
             (fileInfo->fattrib & AM_HID) ? 'H' : '_',
             (fileInfo->fattrib & AM_SYS) ? 'S' : '_',
             fileName,
             (fileInfo->fsize),
             (uint32_t)((fileInfo->fdate >> 9) + 1980) /* year */,
             (uint32_t)((fileInfo->fdate >> 5) & 0x000Fu) /* month */,
             (uint32_t)(fileInfo->fdate & 0x001Fu) /* day */,
             (uint32_t)((fileInfo->ftime >> 11) & 0x0000001Fu) /* hour */,
             (uint32_t)((fileInfo->ftime >> 5) & 0x0000003Fu) /* minute */,
             (uint32_t)(fileInfo->ftime & 0x0000001Fu) /* second */
             );
}

static FRESULT USB_HostMsdFatfsListDirectory(const TCHAR *path)
{
    FRESULT fatfsCode = FR_OK;
    FILINFO fileInfo;
    DIR dir;
    uint8_t outputLabel = 0;

#if _USE_LFN
    static uint8_t fileNameBuffer[_MAX_LFN];
    fileInfo.lfname = fileNameBuffer;
    fileInfo.lfsize = _MAX_LFN;
#endif /* _USE_LFN */

    fatfsCode = f_opendir(&dir, path);
    if (fatfsCode)
    {
        return fatfsCode;
    }
    while (1)
    {
        fatfsCode = f_readdir(&dir, &fileInfo);
        if ((fatfsCode) || (!fileInfo.fname[0]))
        {
            break;
        }
        outputLabel = 1;
        USB_HostMsdFatfsDisplayFileInfo(&fileInfo);
    }
    if (!outputLabel)
    {
        IotLogDebug("");
    }

    return fatfsCode;
}

#if _USE_FORWARD && _FS_TINY
static uint32_t USB_HostMsdFatfsForward(const uint8_t *data, uint32_t dataLength)
{
    uint32_t resultCount = dataLength;

    if (dataLength == 0)
    {
        return 1;
    }
    else
    {
        do
        {
            IotLogDebug("%c", *data);
            data++;
            resultCount--;
        } while (resultCount);
        return dataLength;
    }
}
#endif

static void USB_HostMsdFatfsMount(usb_host_msd_fatfs_instance_t *msdFatfsInstance)
{
    FRESULT fatfsCode;
    FATFS *fs;
#if 0
    /*fix build warning: declared but never used.*/
    FIL file;
    uint32_t index;
    uint32_t resultSize;
    char *testString;
#endif
#if _USE_LFN
    FILINFO fileInfo;
#endif /* _USE_LFN */
    uint32_t freeClusterNumber;
    uint8_t driverNumberBuffer[3];

#if _USE_LFN
    static uint8_t fileNameBuffer[_MAX_LFN];
    fileInfo.lfname = fileNameBuffer;
    fileInfo.lfsize = _MAX_LFN;
    (void)fileInfo;    /*fix build warning: set but never used.*/
#endif /* _USE_LFN */

    /* time delay */
    for (freeClusterNumber = 0; freeClusterNumber < 10000; ++freeClusterNumber)
    {
        __NOP();
    }

    IotLogInfo("fatfs mount as logiacal driver %d......", USBDISK);
    sprintf((char *)&driverNumberBuffer[0], "%c:", USBDISK + '0');
    fatfsCode = f_mount(&fatfs, (char const *)&driverNumberBuffer[0], 0);
    if (fatfsCode)
    {
        IotLogError("Mount error");
        return;
    }
    IotLogInfo("success");

#if (FF_FS_RPATH >= 2)
    fatfsCode = f_chdrive((char const *)&driverNumberBuffer[0]);
    if (fatfsCode)
    {
        IotLogError("Change current drive error");
        return;
    }
#endif

#if 0
#if FF_USE_MKFS
    MKFS_PARM formatOptions;
    formatOptions.fmt = FM_SFD | FM_ANY;
    IotLogDebug("test f_mkfs......");
    fatfsCode = f_mkfs((char const *)&driverNumberBuffer[0], &formatOptions, testBuffer, FF_MAX_SS);
    if (fatfsCode)
    {
        IotLogError("Make directory error");
        return;
    }
    IotLogDebug("success");
#endif /* FF_USE_MKFS */
#endif

    IotLogInfo("Get Disk information,");
    fatfsCode = f_getfree((char const *)&driverNumberBuffer[0], (DWORD *)&freeClusterNumber, &fs);
    if (fatfsCode)
    {
        IotLogError("Get free info error");
        return;
    }
    if (fs->fs_type == FS_FAT12)
    {
        IotLogInfo("    FAT type = FAT12");
    }
    else if (fs->fs_type == FS_FAT16)
    {
        IotLogInfo("    FAT type = FAT16");
    }
    else
    {
        IotLogInfo("    FAT type = FAT32");
    }
    IotLogInfo("    bytes per cluster = %d; number of clusters=%lu ", fs->csize * 512, fs->n_fatent - 2);
    IotLogInfo("    The free size: %dKB, the total size:%dKB", (freeClusterNumber * (fs->csize) / 2),
             ((fs->n_fatent - 2) * (fs->csize) / 2));

    IotLogDebug("directory operation:");
    IotLogDebug("list root directory:");
    fatfsCode = USB_HostMsdFatfsListDirectory((char const *)&driverNumberBuffer[0]);
    if (fatfsCode)
    {
        return;
    }

#if 0
    IotLogDebug("create directory \"dir_1\"......");
    fatfsCode = f_mkdir(_T("1:/dir_1"));
    if (fatfsCode)
    {
        if (fatfsCode == FR_EXIST)
        {
            IotLogDebug("directory exist");
        }
        else
        {
            IotLogError("error");
            return;
        }
    }
    else
    {
        IotLogDebug("success");
    }
    IotLogDebug("create directory \"dir_2\"......");
    fatfsCode = f_mkdir(_T("1:/dir_2"));
    if (fatfsCode)
    {
        if (fatfsCode == FR_EXIST)
        {
            IotLogDebug("directory exist");
        }
        else
        {
            IotLogError("error");
            return;
        }
    }
    else
    {
        IotLogDebug("success");
    }
    IotLogDebug("create sub directory \"dir_2/sub_1\"......");
    fatfsCode = f_mkdir(_T("1:/dir_1/sub_1"));
    if (fatfsCode)
    {
        if (fatfsCode == FR_EXIST)
        {
            IotLogDebug("directory exist");
        }
        else
        {
            IotLogError("error");
            return;
        }
    }
    else
    {
        IotLogDebug("success");
    }
    IotLogDebug("list root directory:");
    fatfsCode = USB_HostMsdFatfsListDirectory(_T("1:"));
    if (fatfsCode)
    {
        return;
    }
    IotLogDebug("list directory \"dir_1\":");
    fatfsCode = USB_HostMsdFatfsListDirectory(_T("1:/dir_1"));
    if (fatfsCode)
    {
        return;
    }
    IotLogDebug("rename directory \"dir_1/sub_1\" to \"dir_1/sub_2\"......");
    fatfsCode = f_rename(_T("1:/dir_1/sub_1"), _T("1:/dir_1/sub_2"));
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("success");
    IotLogDebug("delete directory \"dir_1/sub_2\"......");
    fatfsCode = f_unlink(_T("1:/dir_1/sub_2"));
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("success");

#if (FF_FS_RPATH >= 2)
    IotLogDebug("get current directory......");
    fatfsCode = f_getcwd((TCHAR *)&testBuffer[0], 256);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("%s", testBuffer);
    IotLogDebug("change current directory to \"dir_1\"......");
    fatfsCode = f_chdir(_T("dir_1"));
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("success");
    IotLogDebug("list current directory:");
    fatfsCode = USB_HostMsdFatfsListDirectory(_T("."));
    if (fatfsCode)
    {
        return;
    }
    IotLogDebug("get current directory......");
    fatfsCode = f_getcwd((TCHAR *)&testBuffer[0], 256);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("%s", testBuffer);
#endif

    IotLogDebug("get directory \"dir_1\" information:");
    fatfsCode = f_stat(_T("1:/dir_1"), &fileInfo);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    USB_HostMsdFatfsDisplayFileInfo(&fileInfo);
    IotLogDebug("change \"dir_1\" timestamp to 2015.10.1, 12:30:0......");
    fileInfo.fdate = ((2015 - 1980) << 9 | 10 << 5 | 1); /* 2015.10.1 */
    fileInfo.ftime = (12 << 11 | 30 << 5);               /* 12:30:00 */
    fatfsCode      = f_utime(_T("1:/dir_1"), &fileInfo);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("success");
    IotLogDebug("get directory \"dir_1\" information:");
    fatfsCode = f_stat(_T("1:/dir_1"), &fileInfo);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    USB_HostMsdFatfsDisplayFileInfo(&fileInfo);

    IotLogDebug("file operation:");
    IotLogDebug("create file \"f_1.dat\"......");
    fatfsCode = f_open(&file, _T("1:/f_1.dat"), FA_WRITE | FA_READ | FA_CREATE_ALWAYS);
    if (fatfsCode)
    {
        if (fatfsCode == FR_EXIST)
        {
            IotLogDebug("file exist");
        }
        else
        {
            IotLogError("error");
            return;
        }
    }
    else
    {
        IotLogDebug("success");
    }
    IotLogDebug("test f_write......");
    for (index = 0; index < 58; ++index)
    {
        testBuffer[index] = 'A' + index;
    }
    testBuffer[58] = '\r';
    testBuffer[59] = '\n';
    fatfsCode      = f_write(&file, testBuffer, 60, (UINT *)&resultSize);
    if ((fatfsCode) || (resultSize != 60))
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    fatfsCode = f_sync(&file);
    if (fatfsCode)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    IotLogDebug("success");
    IotLogDebug("test f_printf......");
    if (f_printf(&file, _T("%s"), "f_printf test") == EOF)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    fatfsCode = f_sync(&file);
    if (fatfsCode)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    IotLogDebug("success");
    IotLogDebug("test f_puts......");
    if (f_puts(_T("f_put test"), &file) == EOF)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    fatfsCode = f_sync(&file);
    if (fatfsCode)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    IotLogDebug("success");
    IotLogDebug("test f_putc......");
    testString = "f_putc test";
    while (*testString)
    {
        if (f_putc(*testString, &file) == EOF)
        {
            IotLogError("error");
            f_close(&file);
            return;
        }
        testString++;
    }
    fatfsCode = f_sync(&file);
    if (fatfsCode)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    IotLogDebug("success");
    IotLogDebug("test f_seek......");
    fatfsCode = f_lseek(&file, 0);
    if (fatfsCode)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    IotLogDebug("success");
    IotLogDebug("test f_gets......");
    testString = f_gets((TCHAR *)&testBuffer[0], 10, &file);
    IotLogDebug("%s", testString);
    IotLogDebug("test f_read......");
    fatfsCode = f_read(&file, testBuffer, 10, (UINT *)&resultSize);
    if (fatfsCode)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    testBuffer[resultSize] = 0;
    IotLogDebug("%s", testBuffer);
#if _USE_FORWARD && _FS_TINY
    IotLogDebug("test f_forward......");
    fatfsCode = f_forward(&file, USB_HostMsdFatfsForward, 10, &resultSize);
    if (fatfsCode)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    IotLogDebug("");
#endif
    IotLogDebug("test f_truncate......");
    fatfsCode = f_truncate(&file);
    if (fatfsCode)
    {
        IotLogError("error");
        f_close(&file);
        return;
    }
    IotLogDebug("success");
    IotLogDebug("test f_close......");
    fatfsCode = f_close(&file);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("success");
    IotLogDebug("get file \"f_1.dat\" information:");
    fatfsCode = f_stat(_T("1:/f_1.dat"), &fileInfo);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    USB_HostMsdFatfsDisplayFileInfo(&fileInfo);
    IotLogDebug("change \"f_1.dat\" timestamp to 2015.10.1, 12:30:0......");
    fileInfo.fdate = ((uint32_t)(2015 - 1980) << 9 | 10 << 5 | 1); /* 2015.10.1 */
    fileInfo.ftime = (12 << 11 | 30 << 5);                         /* 12:30:00 */
    fatfsCode      = f_utime(_T("1:/f_1.dat"), &fileInfo);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("success");
    IotLogDebug("change \"f_1.dat\" to readonly......");
    fatfsCode = f_chmod(_T("1:/f_1.dat"), AM_RDO, AM_RDO);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("success");
    IotLogDebug("get file \"f_1.dat\" information:");
    fatfsCode = f_stat(_T("1:/f_1.dat"), &fileInfo);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    USB_HostMsdFatfsDisplayFileInfo(&fileInfo);
    IotLogDebug("remove \"f_1.dat\" readonly attribute......");
    fatfsCode = f_chmod(_T("1:/f_1.dat"), 0, AM_RDO);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("success");
    IotLogDebug("get file \"f_1.dat\" information:");
    fatfsCode = f_stat(_T("1:/f_1.dat"), &fileInfo);
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    USB_HostMsdFatfsDisplayFileInfo(&fileInfo);
    IotLogDebug("rename \"f_1.dat\" to \"f_2.dat\"......");
    fatfsCode = f_rename(_T("1:/f_1.dat"), _T("1:/f_2.dat"));
    if (fatfsCode)
    {
        if (fatfsCode == FR_EXIST)
        {
            IotLogDebug("file exist");
        }
        else
        {
            IotLogError("error");
            return;
        }
    }
    else
    {
        IotLogDebug("success");
    }
    IotLogDebug("delete \"f_2.dat\"......");
    fatfsCode = f_unlink(_T("1:/f_2.dat"));
    if (fatfsCode)
    {
        IotLogError("error");
        return;
    }
    IotLogDebug("success");
#endif
}

#endif /* MSD_FATFS_THROUGHPUT_TEST_ENABLE */

void USB_HostMsdTask(void *arg)
{
    usb_status_t status;
    usb_host_msd_fatfs_instance_t *msdFatfsInstance = (usb_host_msd_fatfs_instance_t *)arg;

    if (msdFatfsInstance->deviceState != msdFatfsInstance->prevDeviceState)
    {
        msdFatfsInstance->prevDeviceState = msdFatfsInstance->deviceState;
        switch (msdFatfsInstance->deviceState)
        {
            case kStatus_DEV_Idle:
                break;

            case kStatus_DEV_Attached: /* deivce is attached and numeration is done */
                status                = USB_HostMsdInit(msdFatfsInstance->deviceHandle,
                                         &msdFatfsInstance->classHandle); /* msd class initialization */
                g_UsbFatfsClassHandle = msdFatfsInstance->classHandle;
                if (status != kStatus_USB_Success)
                {
                    IotLogDebug("usb host msd init fail");
                    return;
                }
                msdFatfsInstance->runState = kUSB_HostMsdRunSetInterface;
                break;

            case kStatus_DEV_Detached: /* device is detached */
                msdFatfsInstance->deviceState = kStatus_DEV_Idle;
                msdFatfsInstance->runState    = kUSB_HostMsdRunIdle;
                USB_HostMsdDeinit(msdFatfsInstance->deviceHandle,
                                  msdFatfsInstance->classHandle); /* msd class de-initialization */
                msdFatfsInstance->classHandle = NULL;

                IotLogInfo("The USB MSD disk is detached");
                break;

            default:
                break;
        }
    }

    /* run state */
    switch (msdFatfsInstance->runState)
    {
        case kUSB_HostMsdRunIdle:
            vTaskDelay(1000/portTICK_RATE_MS);
            break;

        case kUSB_HostMsdRunSetInterface: /* set msd interface */
            msdFatfsInstance->runState     = kUSB_HostMsdRunIdle;
            msdFatfsInstance->runWaitState = kUSB_HostMsdRunWaitSetInterface;
            status = USB_HostMsdSetInterface(msdFatfsInstance->classHandle, msdFatfsInstance->interfaceHandle, 0,
                                             USB_HostMsdControlCallback, msdFatfsInstance);
            if (status != kStatus_USB_Success)
            {
                IotLogDebug("set interface fail");
            }
            break;

        case kUSB_HostMsdRunMassStorageMount: /* set interface succeed */
#if ((defined MSD_FATFS_THROUGHPUT_TEST_ENABLE) && (MSD_FATFS_THROUGHPUT_TEST_ENABLE))
            USB_HostMsdFatfsThroughputTest(msdFatfsInstance); /* test throughput */
#else
            USB_HostMsdFatfsMount(msdFatfsInstance); /* test msd device */
#endif /* MSD_FATFS_THROUGHPUT_TEST_ENABLE */
            msdFatfsInstance->runState = kUSB_HostMsdRunIdle;
            break;

        default:
            break;
    }
}

usb_status_t USB_HostMsdEvent(usb_device_handle deviceHandle,
                              usb_host_configuration_handle configurationHandle,
                              uint32_t eventCode)
{
    usb_status_t status = kStatus_USB_Success;
    usb_host_configuration_t *configuration;
    uint8_t interfaceIndex;
    usb_host_interface_t *interface;
    uint32_t pid;
    uint32_t vid;
    uint32_t address;
    uint8_t id;

    switch (eventCode)
    {
        case kUSB_HostEventAttach:
            /* judge whether is configurationHandle supported */
            configuration = (usb_host_configuration_t *)configurationHandle;
            for (interfaceIndex = 0; interfaceIndex < configuration->interfaceCount; ++interfaceIndex)
            {
                interface = &configuration->interfaceList[interfaceIndex];
                id        = interface->interfaceDesc->bInterfaceClass;
                if (id != USB_HOST_MSD_CLASS_CODE)
                {
                    continue;
                }
                id = interface->interfaceDesc->bInterfaceSubClass;
                if ((id != USB_HOST_MSD_SUBCLASS_CODE_UFI) && (id != USB_HOST_MSD_SUBCLASS_CODE_SCSI))
                {
                    continue;
                }
                id = interface->interfaceDesc->bInterfaceProtocol;
                if (id != USB_HOST_MSD_PROTOCOL_BULK)
                {
                    continue;
                }
                else
                {
                    if (g_MsdFatfsInstance.deviceState == kStatus_DEV_Idle)
                    {
                        /* the interface is supported by the application */
                        g_MsdFatfsInstance.deviceHandle    = deviceHandle;
                        g_MsdFatfsInstance.interfaceHandle = interface;
                        g_MsdFatfsInstance.configHandle    = configurationHandle;
                        return kStatus_USB_Success;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            status = kStatus_USB_NotSupported;
            break;

        case kUSB_HostEventNotSupported:
            break;

        case kUSB_HostEventEnumerationDone:
            if (g_MsdFatfsInstance.configHandle == configurationHandle)
            {
                if ((g_MsdFatfsInstance.deviceHandle != NULL) && (g_MsdFatfsInstance.interfaceHandle != NULL))
                {
                    /* the device enumeration is done */
                    if (g_MsdFatfsInstance.deviceState == kStatus_DEV_Idle)
                    {
                        g_MsdFatfsInstance.deviceState = kStatus_DEV_Attached;

                        USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDevicePID, &pid);
                        USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceVID, &vid);
                        USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceAddress, &address);
                        IotLogInfo("The USB MSD disk is attached (pid=0x%x ,vid=0x%x) with assigned address=%d" , pid, vid, address);
                    }
                    else
                    {
                        IotLogError("No slot for new arrived disk");
                        status = kStatus_USB_Error;
                    }
                }
            }
            break;

        case kUSB_HostEventDetach:
            if (g_MsdFatfsInstance.configHandle == configurationHandle)
            {
                /* the device is detached */
                g_UsbFatfsClassHandle           = NULL;
                g_MsdFatfsInstance.configHandle = NULL;
                if (g_MsdFatfsInstance.deviceState != kStatus_DEV_Idle)
                {
                    g_MsdFatfsInstance.deviceState = kStatus_DEV_Detached;
                }
            }
            break;

        default:
            break;
    }
    return status;
}

#if ((defined USB_HOST_CONFIG_COMPLIANCE_TEST) && (USB_HOST_CONFIG_COMPLIANCE_TEST))
usb_status_t USB_HostTestEvent(usb_device_handle deviceHandle,
                               usb_host_configuration_handle configurationHandle,
                               uint32_t eventCode)
{
    /* process the same supported device that is identified by configurationHandle */
    static usb_host_configuration_handle s_ConfigHandle = NULL;
    static usb_device_handle s_DeviceHandle             = NULL;
    static usb_host_interface_handle s_InterfaceHandle  = NULL;
    usb_status_t status                                 = kStatus_USB_Success;
    usb_host_configuration_t *configuration;
    uint8_t interfaceIndex;
    usb_host_interface_t *interface;
    uint32_t id;

    switch (eventCode)
    {
        case kUSB_HostEventAttach:
            /* judge whether is configurationHandle supported */
            configuration = (usb_host_configuration_t *)configurationHandle;
            for (interfaceIndex = 0; interfaceIndex < configuration->interfaceCount; ++interfaceIndex)
            {
                interface = &configuration->interfaceList[interfaceIndex];
                USB_HostHelperGetPeripheralInformation(deviceHandle, kUSB_HostGetDeviceVID, &id);
                if (id == 0x1a0a) /* certification Vendor ID */
                {
                    IotLogDebug("cetification test device VID match");
                    s_DeviceHandle    = deviceHandle;
                    s_InterfaceHandle = interface;
                    s_ConfigHandle    = configurationHandle;
                    return kStatus_USB_Success;
                }
            }
            status = kStatus_USB_NotSupported;
            break;

        case kUSB_HostEventNotSupported:
            IotLogError("Unsupported Device");
            break;

        case kUSB_HostEventEnumerationDone:
            if (s_ConfigHandle == configurationHandle)
            {
                USB_HostTestModeInit(s_DeviceHandle);
            }
            break;

        case kUSB_HostEventDetach:
            if (s_ConfigHandle == configurationHandle)
            {
                IotLogDebug("PET test device detach");
                USB_HostCloseDeviceInterface(s_DeviceHandle, s_InterfaceHandle);
                /* the device is detached */
                s_DeviceHandle    = NULL;
                s_InterfaceHandle = NULL;
                s_ConfigHandle    = NULL;
            }
            break;

        default:
            break;
    }
    return status;
}
#endif


static void USB_HostTask(void *param)
{
    while (1)
    {
#if (defined(USB_HOST_CONFIG_EHCI) && (USB_HOST_CONFIG_EHCI > 0U))
        USB_HostEhciTaskFunction(param);
#elif (defined(USB_HOST_CONFIG_IP3516HS) && (USB_HOST_CONFIG_IP3516HS > 0U))
        USB_HostIp3516HsTaskFunction(param);
#else
#error The controller is not supported!
#endif
    }
}

static void USB_HostApplicationTask(void *param)
{
    while(1)
    {
    	USB_HostMsdTask(param);
    }
}

int USB_HostMsdFatfsInit(void)
{
    /* Initialize USB for FS and create tasks */
    usb_status_t status = kStatus_USB_Success;

    USB_HostClockInit();

#if ((defined FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    status = USB_HostInit(CONTROLLER_ID, &g_HostHandle, USB_HostEvent);
    if (status != kStatus_USB_Success)
    {
        IotLogError("host init error");
        return -1;
    }
    USB_HostIsrEnable();

    IotLogInfo("USB Host stack successfully initialized");

    if (xTaskCreate(USB_HostTask, "usb host task", 2000L / sizeof(portSTACK_TYPE), g_HostHandle, configMAX_PRIORITIES - 2, NULL) != pdPASS)
    {
        IotLogError("create host task error");
        return -2;
    }
    if (xTaskCreate(USB_HostApplicationTask, "app task", 2300L / sizeof(portSTACK_TYPE), &g_MsdFatfsInstance, 1,
                    NULL) != pdPASS)
    {
        IotLogError("create mouse task error");
        return -3;
    }

    return 0;
}
