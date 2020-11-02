/* This example illustrates USBX Device Mass Storage */

#include "fx_api.h"
#include "ux_api.h"
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_dcd_mcimx6.h"
#include "ux_device_class_storage.h"
#include "ux_device_class_dfu.h"
#include "ux_device_stack.h"
#include "fsl_debug_console.h"

#include "board_setup.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Ram disk in SDRAM.
 *
 * if RAM_DISK_IN_SDRAM is set, SRAM_DTC will still be used
 * instead of BOARD_SDRAM, it will exceeds the size of SRAM_DTC
 */
/* #define RAM_DISK_IN_SDRAM       1 */
#if RAM_DISK_IN_SDRAM
#define RAM_DISK_SIZE (1024 * 1024 * 24)
#else
#define RAM_DISK_SIZE (1024 * 48)
#endif
#define RAM_DISK_LAST_LBA ((RAM_DISK_SIZE / 512) - 1)

/* Define constants. */
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)

#define DEMO_STACK_SIZE                    (4 * 1024)
#define USBX_MEMORY_SIZE                   (32 * 1024)
#define DEVICE_FRAMEWORK_LENGTH_FULL_SPEED 50

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Define global data structures. */
TX_THREAD demo_thread;
FX_MEDIA ram_disk;
UX_SLAVE_CLASS_STORAGE_PARAMETER storage_parameter;
UX_SLAVE_CLASS_DFU_PARAMETER dfu_parameter;

#if RAM_DISK_IN_SDRAM
#pragma location = ".sdram_noinit"
CHAR ram_disk_memory[RAM_DISK_SIZE];
#else
CHAR *ram_disk_memory;
#endif

/* Buffer for FileX RAM media initialization. */
unsigned char buffer[512];

AT_NONCACHEABLE_SECTION(static char usb_memory[USBX_MEMORY_SIZE]);

UCHAR device_framework_full_speed[] = {
    /* Device descriptor */
    0x12, 0x01, 0x10, 0x01, 0x00, 0x00, 0x00, 0x40, 0x81, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x01,

    /* Configuration descriptor */
    0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0xc0, 0x32,

    /* Interface descriptor */
    0x09, 0x04, 0x00, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,

    /* Endpoint descriptor (Bulk In) */
    0x07, 0x05, 0x81, 0x02, 0x40, 0x00, 0x00,

    /* Endpoint descriptor (Bulk Out) */
    0x07, 0x05, 0x02, 0x02, 0x40, 0x00, 0x00};

#define DEVICE_FRAMEWORK_LENGTH_HIGH_SPEED 60
UCHAR device_framework_high_speed[] = {
    /* Device descriptor */
    0x12, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40, 0x81, 0x07, 0x00, 0x00, 0x01, 0x00, 0x01, 0x02, 0x03, 0x01,

    /* Device qualifier descriptor */
    0x0a, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00,

    /* Configuration descriptor */
    0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0xc0, 0x32,

    /* Interface descriptor */
    0x09, 0x04, 0x00, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,

    /* Endpoint descriptor (Bulk In) */
    0x07, 0x05, 0x81, 0x02, 0x00, 0x01, 0x00,

    /* Endpoint descriptor (Bulk Out) */
    0x07, 0x05, 0x02, 0x02, 0x00, 0x01, 0x00};

/*
 * String Device Framework :
 * Byte 0 and 1 : Word containing the language ID : 0x0904 for US
 * Byte 2       : Byte containing the index of the descriptor
 * Byte 3       : Byte containing the length of the descriptor string
 */
#define STRING_FRAMEWORK_LENGTH 38
UCHAR string_framework[] = {
    /* Manufacturer string descriptor : Index 1 */
    0x09, 0x04, 0x01, 0x0c, 0x45, 0x78, 0x70, 0x72, 0x65, 0x73, 0x20, 0x4c, 0x6f, 0x67, 0x69, 0x63,

    /* Product string descriptor : Index 2 */
    0x09, 0x04, 0x02, 0x0a, 0x46, 0x6c, 0x61, 0x73, 0x68, 0x20, 0x44, 0x69, 0x73, 0x6b,

    /* Serial Number string descriptor : Index 3 */
    0x09, 0x04, 0x03, 0x04, 0x30, 0x30, 0x30, 0x31};

/*
 * Multiple languages are supported on the device, to add
 * a language besides english, the unicode language code must
 * be appended to the language_id_framework array and the length
 * adjusted accordingly.
 */
#define LANGUAGE_ID_FRAMEWORK_LENGTH 2
UCHAR language_id_framework[] = {
    /* English. */
    0x09, 0x04};

/* Define the SysTick cycles which will be loaded on tx_initialize_low_level.s */
int systick_cycles;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Define local function prototypes. */
VOID demo_thread_entry(ULONG arg);
UINT demo_thread_media_read(
    VOID *storage, ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status);
UINT demo_thread_media_write(
    VOID *storage, ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status);
UINT demo_thread_media_status(VOID *storage, ULONG lun, ULONG media_id, ULONG *media_status);
UINT demo_thread_media_flush(VOID *storage, ULONG lun, ULONG number_blocks, ULONG lba, ULONG *media_status);

/* Define external function prototypes. */
extern VOID _fx_ram_driver(FX_MEDIA *media_ptr);

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    /* Initialize the board. */
    board_setup();

    PRINTF("USBX device mass storage example\r\n");

    /* This sentence must be called before tx_kernel_enter(). */
    systick_cycles = (SystemCoreClock / TX_TIMER_TICKS_PER_SECOND) - 1;

    /* Enter the ThreadX kernel. */
    tx_kernel_enter();

    return 0;
}

void tx_application_define(void *first_unused_memory)
{
    CHAR *stack_pointer;
    CHAR *memory_pointer;
    UINT status;

    /* Initialize the free memory pointer. */
    stack_pointer = (CHAR *)first_unused_memory;

    /* Initialize the RAM disk memory. */
    memory_pointer = usb_memory;

    /* Initialize USBX Memory */
    ux_system_initialize(memory_pointer, USBX_MEMORY_SIZE, UX_NULL, 0);

#if !RAM_DISK_IN_SDRAM
    ram_disk_memory = stack_pointer + USBX_MEMORY_SIZE;
#endif

    /* Reset RAM disk. */
    _ux_utility_memory_set(ram_disk_memory, 0, RAM_DISK_SIZE);

    /* Initialize FileX. */
    fx_system_initialize();

    /*
     * The code below is required for installing the device portion of USBX.
     * In this demo, DFU is possible and we have a call back for state change.
     */
    status = _ux_device_stack_initialize(device_framework_high_speed, DEVICE_FRAMEWORK_LENGTH_HIGH_SPEED,
                                         device_framework_full_speed, DEVICE_FRAMEWORK_LENGTH_FULL_SPEED,
                                         string_framework, STRING_FRAMEWORK_LENGTH, language_id_framework,
                                         LANGUAGE_ID_FRAMEWORK_LENGTH, UX_NULL);
    if (status != UX_SUCCESS)
        return;

    /* Store the number of LUN in this device storage instance. */
    storage_parameter.ux_slave_class_storage_parameter_number_lun = 1;

    /* Initialize the storage class parameters for reading/writing to the Flash Disk. */
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_last_lba = RAM_DISK_LAST_LBA;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_block_length   = 512;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_type           = 0;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_removable_flag = 0x80;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_read =
        demo_thread_media_read;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_write =
        demo_thread_media_write;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_status =
        demo_thread_media_status;
    storage_parameter.ux_slave_class_storage_parameter_lun[0].ux_slave_class_storage_media_flush =
        demo_thread_media_flush;

    /* Initilize the device storage class. The class is connected with interface 0 on configuration 1. */
    status = _ux_device_stack_class_register(_ux_system_slave_class_storage_name, _ux_device_class_storage_entry, 1, 0,
                                             (VOID *)&storage_parameter);
    if (status != UX_SUCCESS)
        return;

    /* Create the main demo thread. */
    tx_thread_create(&demo_thread, "tx demo", demo_thread_entry, 0, stack_pointer, DEMO_STACK_SIZE, 20, 20, 1,
                     TX_AUTO_START);

    return;
}

void demo_thread_entry(ULONG arg)
{
    ULONG status;

    /* Format the ram drive. */
    status = fx_media_format(&ram_disk, _fx_ram_driver, ram_disk_memory, buffer, sizeof(buffer), "RAM DISK", 1, 512, 0,
                             RAM_DISK_SIZE / 512, 512, 4, 1, 1);

    /* Check the media format status. */
    if (status != FX_SUCCESS)
    {
        /* Error opening media.  Return to caller. */
        return;
    }

    /* Open the ram_disk. */
    status = fx_media_open(&ram_disk, "RAM DISK", _fx_ram_driver, ram_disk_memory, buffer, sizeof(buffer));

    /* Check the media open status. */
    if (status != FX_SUCCESS)
    {
        /* Error opening media.  Return to caller. */
        return;
    }

    /* Initialize the BSP layer of the USB OTG HS Controller. */
    usb_device_setup();

    /* Register the K64 USB device controllers available in this system */
    _ux_dcd_mcimx6_initialize(usb_device_base());

    /* Init the USB interrupt. */
    usb_device_interrupt_setup(USB_DEVICE_INTERRUPT_PRIORITY);
}

UINT demo_thread_media_status(VOID *storage, ULONG lun, ULONG media_id, ULONG *media_status)
{
    /* The ATA drive never fails. This is just for demo only !!!! */
    return (UX_SUCCESS);
}

UINT demo_thread_media_flush(VOID *storage, ULONG lun, ULONG number_blocks, ULONG lba, ULONG *media_status)
{
    /* The ATA drive never fails. This is just for demo only !!!! */
    return (UX_SUCCESS);
}

UINT demo_thread_media_read(
    VOID *storage, ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status)
{
    UINT status = 0;

    ram_disk.fx_media_driver_logical_sector = lba;
    ram_disk.fx_media_driver_sectors        = number_blocks;
    ram_disk.fx_media_driver_request        = FX_DRIVER_READ;
    ram_disk.fx_media_driver_buffer         = data_pointer;
    _fx_ram_driver(&ram_disk);
    status = ram_disk.fx_media_driver_status;

    return (status);
}

UINT demo_thread_media_write(
    VOID *storage, ULONG lun, UCHAR *data_pointer, ULONG number_blocks, ULONG lba, ULONG *media_status)
{
    UINT status = 0;

    ram_disk.fx_media_driver_logical_sector = lba;
    ram_disk.fx_media_driver_sectors        = number_blocks;
    ram_disk.fx_media_driver_request        = FX_DRIVER_WRITE;
    ram_disk.fx_media_driver_buffer         = data_pointer;
    _fx_ram_driver(&ram_disk);

    status = ram_disk.fx_media_driver_status;

    return (status);
}
