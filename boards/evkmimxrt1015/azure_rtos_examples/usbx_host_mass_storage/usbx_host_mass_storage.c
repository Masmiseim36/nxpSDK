
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
#ifndef USBX_MEMORY_SIZE
#define USBX_MEMORY_SIZE            (60 * 1024)
#endif

#ifndef USBX_MEMORY_CACHESAFE_SIZE
#define USBX_MEMORY_CACHESAFE_SIZE  (60 * 1024)
#endif

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
CHAR demo_stack[DEMO_STACK_SIZE];

UINT usb_memory[USBX_MEMORY_SIZE / sizeof(UINT)];
AT_NONCACHEABLE_SECTION_ALIGN(char usb_memory_cachesafe[USBX_MEMORY_CACHESAFE_SIZE], 64);

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

/* this function is for the macro UX_HCD_EHCI_EXT_USBPHY_HIGHSPEED_MODE_SET in ux_user.h */
void usbphy_set_highspeed_mode(void *regs, int on_off)
{
    USB_Type* usb_base[] = USB_BASE_PTRS;
    USBPHY_Type* usbphy_base[] = USBPHY_BASE_PTRS;
    uint32_t ehci_base;
    UX_HCD_EHCI *hcd_ehci;
    int i;

    hcd_ehci = (UX_HCD_EHCI *)regs;
    if (hcd_ehci->ux_hcd_ehci_base == 0)
        return;

    /* the first value in USB_BASE_ADDRS is always zero, so skip it */
    for (i = 1; i < sizeof(usb_base) / sizeof(usb_base[0]); i++)
    {
        ehci_base = (uint32_t)(&usb_base[i]->CAPLENGTH);
        if ((uint32_t)hcd_ehci->ux_hcd_ehci_base == ehci_base) {
            if (on_off)
                usbphy_base[i]->CTRL_SET = USBPHY_CTRL_SET_ENHOSTDISCONDETECT_MASK;
            else
                usbphy_base[i]->CTRL_CLR = USBPHY_CTRL_CLR_ENHOSTDISCONDETECT_MASK;
        }
    }
}

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
    TX_THREAD_NOT_USED(first_unused_memory);

    /* Initialize FileX.  */
    fx_system_initialize();

    /* Initialize USBX memory. */
    ux_system_initialize((VOID *)usb_memory, USBX_MEMORY_SIZE, usb_memory_cachesafe, USBX_MEMORY_CACHESAFE_SIZE);

    /* Create the main demo thread. */
    tx_thread_create(&tx_demo_thread, "tx demo", demo_thread_entry, 0, (VOID *)demo_stack, DEMO_STACK_SIZE, 20, 20, 1,
                     TX_AUTO_START);
}

void demo_thread_entry(ULONG arg)
{
    usb_host_setup();

    /* The code below is required for installing the host portion of USBX.  */
    status = ux_host_stack_initialize(NULL);
    if (status != UX_SUCCESS)
        goto err;

    /* Register the HUB class. */
    status = ux_host_stack_class_register(_ux_system_host_class_hub_name, _ux_host_class_hub_entry);
    if (status != UX_SUCCESS)
        goto err;

    /* Register storage class. */
    status = ux_host_stack_class_register(_ux_system_host_class_storage_name, _ux_host_class_storage_entry);
    if (status != UX_SUCCESS)
        goto err;

    usb_host_interrupt_setup(USB_HOST_INTERRUPT_PRIORITY);
    /* Register all the USB host controllers available in this system.  */

    status = ux_host_stack_hcd_register((UCHAR *)"ehci test", _ux_hcd_ehci_initialize, usb_host_base(), 0x0);
    if (status != UX_SUCCESS)
        goto err;

    while (1)
    {
        /* Find the storage class. */
        status = demo_class_storage_get();
        if (status == UX_SUCCESS) {

            /* Find first file on media. */
            status = fx_directory_first_entry_find(media, file_name);
            if (status == FX_SUCCESS) {
                demo_read_file();
            }
        }

        tx_thread_sleep(4 * TX_TIMER_TICKS_PER_SECOND);
    }

err:
    PRINTF("demo_thread_entry: FAIL(0x%x)\r\n", status);
    while(1)
        ;
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
