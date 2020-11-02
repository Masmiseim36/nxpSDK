
/* This example illustrates USBX Host Mass Storage. */

/* Include necessary system files. */
#include "fx_api.h"
#include "ux_api.h"
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_hcd_ehci.h"
#include "ux_host_class_hub.h"
#include "ux_host_class_storage.h"
#include "fsl_debug_console.h"

#include "board_setup.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Define constants. */
#define USBX_MEMORY_SIZE (100 * 1024)
#define DEMO_STACK_SIZE  (2 * 1024)
#define DEMO_BUFFER_SIZE 1024

#define USB_HOST_INTERRUPT_PRIORITY (1U)

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Define global data structures. */
TX_THREAD tx_demo_thread;
TX_SEMAPHORE tx_demo_semaphore;

UX_HOST_CLASS *class_driver;
UX_HOST_CLASS_STORAGE *storage;
UX_HOST_CLASS_STORAGE_MEDIA *storage_media;

UINT status;
UINT transfer_completed;
ULONG class_driver_index;
ULONG requested_length;

FX_FILE my_file;
FX_MEDIA *media;

CHAR file_name[64];
UCHAR local_buffer[1024];
UCHAR storage_buffer[DEMO_BUFFER_SIZE];

AT_NONCACHEABLE_SECTION(static char usb_memory[USBX_MEMORY_SIZE]);

/* Define the SysTick cycles which will be loaded on tx_initialize_low_level.s */
int systick_cycles;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Define local function prototypes.  */
void demo_thread_entry(ULONG arg);
void demo_read_file();
UINT demo_class_storage_get(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    /* Initialize the board. */
    board_setup();

    PRINTF("USBX host mass storage example\r\n");

    /* This sentence must be called before tx_kernel_enter(). */
    systick_cycles = (SystemCoreClock / TX_TIMER_TICKS_PER_SECOND) - 1;

    /* Enter the ThreadX kernel. */
    tx_kernel_enter();

    return 0;
}

/* Define what the initial system looks like. */
void tx_application_define(void *first_unused_memory)
{
    CHAR *stack_pointer;
    CHAR *memory_pointer;

    /* Initialize the free memory pointer */
    stack_pointer = (CHAR *)first_unused_memory;

    memory_pointer = usb_memory;

    /* Initialize FileX.  */
    fx_system_initialize();

    /* Initialize USBX memory. */
    ux_system_initialize(memory_pointer, USBX_MEMORY_SIZE, UX_NULL, 0);

    /* Create the main demo thread. */
    tx_thread_create(&tx_demo_thread, "tx demo", demo_thread_entry, 0, stack_pointer, DEMO_STACK_SIZE, 30, 30, 1,
                     TX_AUTO_START);
}

void demo_thread_entry(ULONG arg)
{
    usb_host_setup();

    /* The code below is required for installing the host portion of USBX.  */
    status = ux_host_stack_initialize(NULL);
    if (status != UX_SUCCESS)
        return;

    /* Register the HUB class. */
    status = ux_host_stack_class_register(_ux_system_host_class_hub_name, _ux_host_class_hub_entry);
    if (status != UX_SUCCESS)
        return;

    /* Register storage class. */
    status = ux_host_stack_class_register(_ux_system_host_class_storage_name, _ux_host_class_storage_entry);
    if (status != UX_SUCCESS)
        return;

    usb_host_interrupt_setup(USB_HOST_INTERRUPT_PRIORITY);
    /* Register all the USB host controllers available in this system.  */

    status = ux_host_stack_hcd_register((UCHAR *)"ehci test", _ux_hcd_ehci_initialize, usb_host_base(), 0x0);
    if (status != UX_SUCCESS)
        return;

    while (1)
    {
        /* Find the storage class. */
        status = demo_class_storage_get();

        if (status != UX_SUCCESS)
            continue;

        /* Find first file on media. */
        status = fx_directory_first_entry_find(media, file_name);

        if (status != UX_SUCCESS)
            continue;

        demo_read_file();

        tx_thread_sleep(10);
    }
}

void demo_read_file()
{
    ULONG files_count = 0;
    FX_FILE my_file;
    ULONG requested_length;
    UINT file_attribute;
    ULONG error_count = 0;

    /* We come here if there is at least a file in the directory. */
    do
    {
        /* Increment file count. */
        files_count++;

        /* Reset file attributes. */
        file_attribute = 0;

        /* Try to read the file attributes. */
        status = fx_file_attributes_read(media, file_name, &file_attribute);

        PRINTF("Find File: %s\r\n", file_name);

        /* If this is a directory, pass. */
        if (!(file_attribute & 0x18) && (status == UX_SUCCESS))
        {
            /* Try to open the file. */
            status = fx_file_open(media, &my_file, file_name, FX_OPEN_FOR_READ);
            if (status != UX_SUCCESS)
                break;

            /* Read the entire file. */
            while (status == UX_SUCCESS)
            {
                /* Read the file in blocks */
                status = fx_file_read(&my_file, local_buffer, 1024, &requested_length);

                /* Check if status OK. */
                if (status != UX_SUCCESS)
                {
                    error_count++;
                    break;
                }

                /* Check if end of file. */
                if (requested_length != 1024)
                {
                    break;
                }
            }

            /* Finished reading file either at the end or because of error. */
            fx_file_close(&my_file);
        }

        /* Ask FileX to find another file. */
        status = fx_directory_next_entry_find(media, file_name);
        if (status != UX_SUCCESS)
        {
            break;
        }

    } while (1);

    return;
}

UINT demo_class_storage_get(void)
{
    UX_HOST_CLASS *class;

    /* Find the main storage container */
    status = ux_host_stack_class_get(_ux_system_host_class_storage_name, &class);
    if (status != UX_SUCCESS)
        return (status);

    /* We get the first instance of the storage device */
    do
    {
        status = ux_host_stack_class_instance_get(class, 0, (void **)&storage);
        tx_thread_sleep(10);
    } while (status != UX_SUCCESS);

    /* We still need to wait for the storage status to be live */
    while (storage->ux_host_class_storage_state != UX_HOST_CLASS_INSTANCE_LIVE)
        tx_thread_sleep(10);

    /* We try to get the first media attached to the class container. */
    while (class->ux_host_class_media == UX_NULL)
    {
        tx_thread_sleep(10);
    }

    /* Setup media pointer. */
    storage_media = (UX_HOST_CLASS_STORAGE_MEDIA *)class->ux_host_class_media;
    media         = &storage_media->ux_host_class_storage_media;

    return (UX_SUCCESS);
}
