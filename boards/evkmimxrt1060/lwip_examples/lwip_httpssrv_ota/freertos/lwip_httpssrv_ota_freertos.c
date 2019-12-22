/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "lwip/opt.h"

#if LWIP_SOCKET
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/netifapi.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"
#include "lwip/sockets.h"
#include "netif/etharp.h"

#include "enet_ethernetif.h"
#include "board.h"

#include "ksdk_mbedtls.h"
#include "httpsrv.h"
#include "httpsrv_port.h"

// dm #include "certs_buff.h"
#include "mbedtls/certs.h"

#include "ota_bootloader_supp.h"
#include "mflash_drv.h"

#include "timers.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 0
#define configIP_ADDR3 102

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 0
#define configGW_ADDR3 100

/* MAC address configuration. */
#define configMAC_ADDR                     \
    {                                      \
        0x02, 0x12, 0x13, 0x10, 0x15, 0x11 \
    }

/* Address of PHY interface. */
#define EXAMPLE_PHY_ADDRESS BOARD_ENET0_PHY_ADDRESS

#define TRNG0 TRNG

/* System clock name. */
#define EXAMPLE_CLOCK_NAME kCLOCK_CoreSysClk


#ifndef EXAMPLE_NETIF_INIT_FN
/*! @brief Network interface initialization function. */
#define EXAMPLE_NETIF_INIT_FN ethernetif0_init
#endif /* EXAMPLE_NETIF_INIT_FN */

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG LWIP_DBG_ON
#endif
#ifndef HTTPD_STACKSIZE
#define HTTPD_STACKSIZE DEFAULT_THREAD_STACKSIZE
#endif
#ifndef HTTPD_PRIORITY
#define HTTPD_PRIORITY DEFAULT_THREAD_PRIO
#endif
#ifndef DEBUG_WS
#define DEBUG_WS 0
#endif

#define OTA_UPDATE_PART (BL_FEATURE_SECONDARY_IMG_START - FlexSPI_AMBA_BASE)
#define OTA_MAX_IMAGE_SIZE (BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE - BL_IMG_HEADER_SIZE)
#define OTA_IMAGE_LOAD_ADDR (BL_FEATURE_PRIMARY_IMG_START + BL_IMG_HEADER_SIZE)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static int cgi_ota_upload(HTTPSRV_CGI_REQ_STRUCT *param);
static int cgi_ota_reboot(HTTPSRV_CGI_REQ_STRUCT *param);
static int cgi_ota_accept(HTTPSRV_CGI_REQ_STRUCT *param);

static int ssi_ota_status(HTTPSRV_SSI_PARAM_STRUCT *param);
static int ssi_disabled_input(HTTPSRV_SSI_PARAM_STRUCT *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static struct netif netif;
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
static mem_range_t non_dma_memory[] = NON_DMA_MEMORY_ARRAY;
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
/* FS data.*/
extern const HTTPSRV_FS_DIR_ENTRY httpsrv_fs_data[];

const HTTPSRV_CGI_LINK_STRUCT cgi_lnk_tbl[] = {
    {"upload", cgi_ota_upload},
    {"reboot", cgi_ota_reboot},
    {"accept", cgi_ota_accept},
    {0, 0} // DO NOT REMOVE - last item - end of table
};

const HTTPSRV_SSI_LINK_STRUCT ssi_lnk_tbl[] = {
    {"ota_status", ssi_ota_status},
    {"disabled_input", ssi_disabled_input},
    {0, 0} // DO NOT REMOVE - last item - end of table
};

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitModuleClock(void)
{
    const clock_enet_pll_config_t config = {.enableClkOutput = true, .enableClkOutput25M = false, .loopDivider = 1};
    CLOCK_InitEnetPll(&config);
}

void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 1000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}


enum ota_status_t
{
    OTA_STATUS_NONE = 0,
    OTA_STATUS_UPLOADED,
    OTA_STATUS_TESTING,
    OTA_STATUS_BOOTLOADER_ERROR,
} ota_status = OTA_STATUS_NONE;

char *ota_status_strings[] = {
    "Select file with updated firmware and click <b>Upload</b>.",
    "Update image uploaded, click <b>Reboot</b> to start in test mode.",
    "Running in test mode, click <b>Accept update</b> to make it permanent or <br>Reboot</b> for rollback.",
    "<b>Error</b> - check bootloader installation.",
};

/* Server Side Include callback for OTA status. */
static int ssi_ota_status(HTTPSRV_SSI_PARAM_STRUCT *param)
{
    char *status_string = ota_status_strings[ota_status];
    HTTPSRV_ssi_write(param->ses_handle, status_string, strlen(status_string));
    return (0);
}

/* Server Side Include callback to disable inputs that are of no use at the moment. */
static int ssi_disabled_input(HTTPSRV_SSI_PARAM_STRUCT *param)
{
    int disabled = 0;

    if (strcmp(param->com_param, "accept") == 0)
    {
        disabled = (ota_status != OTA_STATUS_TESTING);
    }

    if (disabled)
    {
        HTTPSRV_ssi_write(param->ses_handle, "disabled", strlen("disabled"));
    }

    return (0);
}

#define MULTIPART_READ_BUFSIZE (200)
#define MULTIPART_MAX_BOUNDARY (70)
#define FORM_DATA_NAME_SIZE (20)

enum multipart_state
{
    MULTIPART_END = 0,
    MULTIPART_EXPECT_HEADERS,
    MULTIPART_EXPECT_DATA,
    MULTIPART_ERROR,
};

struct multipart_read_ctx
{
    uint32_t ses_handle;

    enum multipart_state state;

    /* Boundary string is 1..70 chars long. Keep 2 more reserved for CRLF */
    char boundary[MULTIPART_MAX_BOUNDARY + 2];
    uint32_t boundary_len; /* Actual lenght of the stored boundary string */

    char buffer[MULTIPART_READ_BUFSIZE];
    uint32_t buf_size; /* Size of the buffer (allocated space), this is to allow for possible dynamic allocation of the
                          buffer */
    char *buf_start;   /* Pointer to valid (not consumed so far) data in the buffer */
    char
        *buf_end; /* Points to location following the valid data, i.e. it may point one byte past the allocated space */

    char form_data_name[FORM_DATA_NAME_SIZE + 1]; /* Extra char for null termination */
};

static int multipart_read_init(struct multipart_read_ctx *ctx, uint32_t ses_handle)
{
    int read;

    memset(ctx, 0, sizeof(*ctx));

    ctx->ses_handle = ses_handle;
    ctx->buf_size   = MULTIPART_READ_BUFSIZE;
    ctx->buf_end = ctx->buf_start = ctx->buffer;

    /* Fill in the buffer with data */
    read = HTTPSRV_cgi_read(ctx->ses_handle, ctx->buffer, ctx->buf_size);
    if (read <= 0)
    {
        ctx->state = MULTIPART_ERROR;
        return -1;
    }
    ctx->buf_end += read;

    /* Determine length of boundary string by scanning its first occurence */
    while (ctx->buf_start < ctx->buf_end)
    {
        if (*ctx->buf_start == ' ' || *ctx->buf_start == '\r')
        {
            /* End of boundary string */
            break;
        }
        ctx->boundary_len++;
        ctx->buf_start++;
    }

    if (ctx->buf_start == ctx->buf_end)
    {
        /* End of buffer reached while boundary end was not found */
        ctx->state = MULTIPART_ERROR;
        return -1;
    }

    if (ctx->boundary_len < 1 || ctx->boundary_len > 70)
    {
        /* Length of  boundary string is out of spec */
        ctx->state = MULTIPART_ERROR;
        return -1;
    }

    /*  RFC: The boundary delimiter MUST occur at the beginning of a line.
        Implementation: Use 2 reserved bytes to prepend boundary with CRLF for convenient matching using a state machine
     */
    ctx->boundary[0] = '\r';
    ctx->boundary[1] = '\n';
    memcpy(ctx->boundary + 2, ctx->buffer, ctx->boundary_len);
    ctx->boundary_len += 2;

    /* There may be whitespaces at the end of boundary line, skip them */
    while (ctx->buf_start < ctx->buf_end && *ctx->buf_start == ' ')
    {
        ctx->buf_start++;
    }

    /* There should be at least 2 chars for line termination remaining */
    if (ctx->buf_end - ctx->buf_start < 2)
    {
        /* Either the buffer is small to fit a single line or we reached the end of the stream */
        ctx->state = MULTIPART_ERROR;
        return -1;
    }

    /* Consume CRLF at the end to the boundary line */
    if (ctx->buf_start[0] != '\r' || ctx->buf_start[1] != '\n')
    {
        ctx->state = MULTIPART_ERROR;
        return -1;
    }
    ctx->buf_start += 2;

    /* Expect headers */
    ctx->state = MULTIPART_EXPECT_HEADERS;

    return 0;
}

static int multipart_proc_header(struct multipart_read_ctx *ctx, char *buffer)
{
    char *param_ptr;

    if (strncmp(buffer, "Content-Disposition:", 20) == 0)
    {
        param_ptr = buffer + 20;
        if (strstr(param_ptr, " form-data;"))
        {
            char *name_ptr;
            char *end_ptr;

            name_ptr = strstr(param_ptr, " name=\"");
            if (name_ptr)
            {
                name_ptr += 7;
                end_ptr = strchr(name_ptr, '\"');
                if (end_ptr)
                {
                    int len = (end_ptr - name_ptr) < FORM_DATA_NAME_SIZE ? (end_ptr - name_ptr) : FORM_DATA_NAME_SIZE;
                    strncpy(ctx->form_data_name, name_ptr, len);
                }
            }
        }
    }
    return 0;
}

static void multipart_clear_headers(struct multipart_read_ctx *ctx)
{
    memset(ctx->form_data_name, 0, sizeof(ctx->form_data_name));
}

static int multipart_read_headers(struct multipart_read_ctx *ctx)
{
    char *line_start;
    char *line_lf;
    int read;
    int num_headers = 0;

    if (ctx->state != MULTIPART_EXPECT_HEADERS)
    {
        return 0;
    }

    multipart_clear_headers(ctx);

    /* Process buffer line by line. End of line is \n or \r\n */
    while (1)
    {
        line_start = ctx->buf_start;
        line_lf    = memchr(line_start, '\n', ctx->buf_end - ctx->buf_start);

        if (line_lf == NULL)
        {
            /* No end of line found in the buffer */
            if (ctx->buf_end - ctx->buf_start == ctx->buf_size)
            {
                /* The buffer is full but probably not large enough to keep the whole header line */
                ctx->state = MULTIPART_ERROR;
                return -1;
            }

            /* Move unprocessed data to the beginning of the buffer */
            memmove(ctx->buffer, ctx->buf_start, ctx->buf_end - ctx->buf_start);
            ctx->buf_end -= ctx->buf_start - ctx->buffer;
            ctx->buf_start = ctx->buffer;

            /* Top up the buffer */
            read = HTTPSRV_cgi_read(ctx->ses_handle, ctx->buf_end, ctx->buf_size - (ctx->buf_end - ctx->buf_start));
            if (read == 0)
            {
                /* End od stream */
                ctx->state = MULTIPART_ERROR;
                return -1;
            }
            ctx->buf_end += read;

            /* And restart parsing */
            continue;
        }

        /* Null terminate the line */
        *line_lf = '\0';
        if ((line_lf > line_start) && (*(line_lf - 1) == '\r'))
        {
            /* Discard optional CR */
            *(line_lf - 1) = '\0';
        }

        /* Move start of valid data in the buffer according to data consumed */
        ctx->buf_start = line_lf + 1;

        /* Empty line indicates end of headers */
        if (*line_start == '\0')
        {
            break;
        }

        /* Process the header */
        multipart_proc_header(ctx, line_start);
        num_headers++;
    }

    ctx->state = MULTIPART_EXPECT_DATA;
    return num_headers;
}

static int32_t multipart_read_data(struct multipart_read_ctx *ctx, uint8_t *buffer, int32_t len)
{
    int match_idx  = 0;
    int read_total = 0;

    if (ctx->state != MULTIPART_EXPECT_DATA)
    {
        return 0;
    }

    while (read_total != len)
    {
        if (ctx->buf_start + match_idx >= ctx->buf_end)
        {
            /* Boundary matching is beyond buffer end */
            uint32_t read;

            /* Move unprocessed data to the beginning of the buffer */
            memmove(ctx->buffer, ctx->buf_start, ctx->buf_end - ctx->buf_start);
            ctx->buf_end -= ctx->buf_start - ctx->buffer;
            ctx->buf_start = ctx->buffer;

            /* Top up the buffer */
            read = HTTPSRV_cgi_read(ctx->ses_handle, ctx->buf_end, ctx->buf_size - (ctx->buf_end - ctx->buf_start));
            if (read == 0)
            {
                /* End od stream unexpected at this point */
                ctx->state = MULTIPART_ERROR;
                break;
            }
            ctx->buf_end += read;
        }

        if (ctx->buf_start[match_idx] == ctx->boundary[match_idx])
        {
            if (++match_idx == ctx->boundary_len)
            {
                /* Boundary found, consume it and exit the loop*/
                ctx->buf_start += match_idx;
                break;
            }
            continue;
        }

        /* Mismatch, reset matching index */
        match_idx = 0;

        /* Copy character to the caller provided buffer */
        if (buffer != NULL)
        {
            *buffer++ = *ctx->buf_start;
        }
        ctx->buf_start++;

        read_total++;
    }

    if (match_idx == ctx->boundary_len)
    {
        /* Boundary was matched, presume that headers will follow unless further reading of the stream indicates the
         * processing should stop */
        ctx->state = MULTIPART_EXPECT_HEADERS;

        /* For simplicity of implementation, the closing double dash of last boundary is not strictly required.
           Just read until encountering single dash, LF or end of stream.  */
        do
        {
            /* Make sure the buffer is not empty */
            if (ctx->buf_end == ctx->buf_start)
            {
                uint32_t read;
                ctx->buf_start = ctx->buf_end = ctx->buffer;
                read                          = HTTPSRV_cgi_read(ctx->ses_handle, ctx->buffer, ctx->buf_size);
                if (read == 0)
                {
                    /* End od stream unexpected at this point */
                    ctx->state = MULTIPART_ERROR;
                    break;
                }
                ctx->buf_end += read;
            }
            if (*ctx->buf_start == '-')
            {
                /* Dash found, assume end of multipart content, rest of the buffer will be ignored */
                ctx->state = MULTIPART_END;
                break;
            }
        } while (*ctx->buf_start++ != '\n');
    }

    return (read_total);
}

int32_t validate_update_image(uint32_t flash_addr, uint32_t size)
{
    uint8_t *image_ptr;

    image_ptr = mflash_drv_mmap(flash_addr, size);
    if (image_ptr == NULL)
    {
        return -1;
    }

#if OTA_DEVEL_MODE
    /* Fixes image header required by ota_bootloader by setting the length and generating the proper checksum.
     * This is to provide easy way of testing/debugging of OTA enabled application.
     * Should not be used in production environment! */
    if (((boot_image_header_t *)image_ptr)->image_size == 0)
    {
        /* The image header declares zero image size, so it is a dummy header - update it */
        boot_image_header_t bih;

        memcpy(&bih, image_ptr, sizeof(bih));
        bih.image_size  = size - bih.header_size;
        bih.checksum[0] = bl_crc32(image_ptr + bih.header_size, bih.image_size);
        bih.algorithm   = IMG_CHK_ALG_CRC32;

        /* Write boot image header back to FLASH */
        if (mflash_drv_write(flash_addr, (uint8_t *)&bih, sizeof(bih)) != 0)
        {
            return -1;
        }

        /* Declare image as valid */
        return 1;
    }
#endif

    /* Verify image and return the result of verification */
    return bl_verify_image(image_ptr, size);
}

int32_t store_update_image(struct multipart_read_ctx *ctx, uint32_t flash_addr)
{
    int32_t result;

    uint8_t buffer[512];
    uint32_t chunk_addr = flash_addr;
    uint32_t chunk_len;

    uint32_t total_stored = 0;

    do
    {
        chunk_len = multipart_read_data(ctx, buffer, sizeof(buffer));
        if (chunk_len > 0)
        {
            result = mflash_drv_write(chunk_addr, buffer, chunk_len);
            if (result != 0)
            {
                /* Error during flash operation */
                return -1;
            }
            chunk_addr += chunk_len;
            total_stored += chunk_len;
        }

    } while (chunk_len > 0);

    return total_stored;
}

/* Common Gateway Interface callback for OTA update. */
static int cgi_ota_upload(HTTPSRV_CGI_REQ_STRUCT *param)
{
    struct multipart_read_ctx *mpr_ctx;

    HTTPSRV_CGI_RES_STRUCT response = {0};
    response.ses_handle             = param->ses_handle;
    response.status_code            = HTTPSRV_CODE_OK;

    if (param->request_method == HTTPSRV_REQ_POST)
    {
        mpr_ctx = (struct multipart_read_ctx *)httpsrv_mem_alloc(sizeof(struct multipart_read_ctx));
        if (mpr_ctx == NULL)
        {
            response.status_code = HTTPSRV_CODE_INTERNAL_ERROR;
            return (response.content_length);
        }

        multipart_read_init(mpr_ctx, param->ses_handle);

        while ((mpr_ctx->state == MULTIPART_EXPECT_HEADERS) && (response.status_code == HTTPSRV_CODE_OK))
        {
            int headers;
            headers = multipart_read_headers(mpr_ctx);
            if (headers <= 0)
            {
                response.status_code = HTTPSRV_CODE_BAD_REQ;
                break;
            }
            if (0 == strcmp(mpr_ctx->form_data_name, "update_file"))
            {
                partition_t update_partition;
                int32_t stored;
                if (bl_get_update_partition_info(&update_partition) != kStatus_Success)
                {
                    /* Could not get update partition info */
                    response.status_code = HTTPSRV_CODE_INTERNAL_ERROR;
                }
                else if ((stored = store_update_image(mpr_ctx, update_partition.start - FlexSPI_AMBA_BASE)) <= 0)
                {
                    /* Error during upload */
                    response.status_code = HTTPSRV_CODE_INTERNAL_ERROR;
                }
                else if (validate_update_image(update_partition.start - FlexSPI_AMBA_BASE, stored) <= 0)
                {
                    /* Image validation failed */
                    response.status_code = HTTPSRV_CODE_INTERNAL_ERROR;
                }
                else
                {
                    /* Image ok */
                    ota_status = OTA_STATUS_UPLOADED;
                }
            }
            else
            {
                /* Discard unknown multipart data block */
                multipart_read_data(mpr_ctx, NULL, -1);
            }
        }

        httpsrv_mem_free(mpr_ctx);

        /* Write the response using chunked transmission coding. */
        response.content_type = HTTPSRV_CONTENT_TYPE_HTML;
        /* Set content length to -1 to indicate unknown content length. */
        response.content_length = -1;
        response.data           = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
        response.data_length    = strlen(response.data);
        HTTPSRV_cgi_write(&response);

        if (response.status_code == HTTPSRV_CODE_OK)
        {
            response.data        = "<html><head><title>File upload successfull!</title>";
            response.data_length = strlen(response.data);
        }
        else
        {
            response.data        = "<html><head><title>File upload failed!</title>";
            response.data_length = strlen(response.data);
        }

        HTTPSRV_cgi_write(&response);
        response.data        = "<meta http-equiv=\"refresh\" content=\"0; url=ota.shtml\"></head><body></body></html>";
        response.data_length = strlen(response.data);
        HTTPSRV_cgi_write(&response);
        response.data_length = 0;
        HTTPSRV_cgi_write(&response);
    }

    return (response.content_length);
}

void reboot_timer_callback(TimerHandle_t timer)
{
    NVIC_SystemReset();
}

/* Common Gateway Interface callback for device reboot. */
static int cgi_ota_reboot(HTTPSRV_CGI_REQ_STRUCT *param)
{
    /* Static variable is used to avoid repetitive creation of the timer as that it expected to happen just once (reboot
     * follows) */
    static TimerHandle_t reboot_timer = NULL;

    HTTPSRV_CGI_RES_STRUCT response = {0};
    response.ses_handle             = param->ses_handle;
    response.status_code            = HTTPSRV_CODE_OK;

    /* Write the response using chunked transmission coding. */
    response.content_type = HTTPSRV_CONTENT_TYPE_HTML;
    /* Set content length to -1 to indicate unknown content length. */
    response.content_length = -1;
    response.data           = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
    response.data_length    = strlen(response.data);
    HTTPSRV_cgi_write(&response);
    response.data        = "<html><head><title>Going down for reboot</title>";
    response.data_length = strlen(response.data);
    HTTPSRV_cgi_write(&response);
    response.data = "<meta http-equiv=\"refresh\" content=\"0; url=ota_reboot.html\"></head><body></body></html>";
    response.data_length = strlen(response.data);
    HTTPSRV_cgi_write(&response);
    response.data_length = 0;
    HTTPSRV_cgi_write(&response);

    if (ota_status == OTA_STATUS_UPLOADED)
    {
        /* There is and update waiting, instruct bootloader to test it */
        bl_update_image_state(kSwapType_ReadyForTest);
    }

    if (reboot_timer == NULL)
    {
        /* Actual reboot is delayed to give the HTTP server a chance to send the content generated by CGI and gracefully
         * close the connection */
        reboot_timer = xTimerCreate("reboot_timer", pdMS_TO_TICKS(5000), pdFALSE, NULL, reboot_timer_callback);
        xTimerStart(reboot_timer, 0);
    }

    return (response.content_length);
}

/* Common Gateway Interface callback for accepting the update. */
static int cgi_ota_accept(HTTPSRV_CGI_REQ_STRUCT *param)
{
    HTTPSRV_CGI_RES_STRUCT response = {0};
    response.ses_handle             = param->ses_handle;
    response.status_code            = HTTPSRV_CODE_OK;

    /* Write the response using chunked transmission coding. */
    response.content_type = HTTPSRV_CONTENT_TYPE_HTML;
    /* Set content length to -1 to indicate unknown content length. */
    response.content_length = -1;
    response.data           = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">";
    response.data_length    = strlen(response.data);
    HTTPSRV_cgi_write(&response);
    response.data        = "<html><head><title>Accept update</title>";
    response.data_length = strlen(response.data);
    HTTPSRV_cgi_write(&response);
    response.data        = "<meta http-equiv=\"refresh\" content=\"0; url=ota.shtml\"></head><body></body></html>";
    response.data_length = strlen(response.data);
    HTTPSRV_cgi_write(&response);
    response.data_length = 0;
    HTTPSRV_cgi_write(&response);

    if (ota_status == OTA_STATUS_TESTING)
    {
        /* There is and update waiting, instruct bootloader to test it */
        bl_update_image_state(kSwapType_Permanent);
        ota_status = OTA_STATUS_NONE;
    }

    return (response.content_length);
}

#if HTTPSRV_CFG_MBEDTLS_ENABLE
static HTTPSRV_TLS_PARAM_STRUCT tls_params;
#endif

/*!
 * @brief Initializes lwIP stack.
 */
static void stack_init(void)
{
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyAddress = EXAMPLE_PHY_ADDRESS,
        .clockName  = EXAMPLE_CLOCK_NAME,
        .macAddress = configMAC_ADDR,
#if defined(FSL_FEATURE_SOC_LPC_ENET_COUNT) && (FSL_FEATURE_SOC_LPC_ENET_COUNT > 0)
        .non_dma_memory = non_dma_memory,
#endif /* FSL_FEATURE_SOC_LPC_ENET_COUNT */
    };

    CRYPTO_InitHardware();

    tcpip_init(NULL, NULL);

    IP4_ADDR(&netif_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&netif_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&netif_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    netifapi_netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    LWIP_PLATFORM_DIAG(("\r\n************************************************"));
    LWIP_PLATFORM_DIAG(("mbedTLS HTTPS Server example"));
    LWIP_PLATFORM_DIAG(("************************************************"));
    LWIP_PLATFORM_DIAG((" IPv4 Address     : %u.%u.%u.%u", ((u8_t *)&netif_ipaddr)[0], ((u8_t *)&netif_ipaddr)[1],
                        ((u8_t *)&netif_ipaddr)[2], ((u8_t *)&netif_ipaddr)[3]));
    LWIP_PLATFORM_DIAG((" IPv4 Subnet mask : %u.%u.%u.%u", ((u8_t *)&netif_netmask)[0], ((u8_t *)&netif_netmask)[1],
                        ((u8_t *)&netif_netmask)[2], ((u8_t *)&netif_netmask)[3]));
    LWIP_PLATFORM_DIAG((" IPv4 Gateway     : %u.%u.%u.%u", ((u8_t *)&netif_gw)[0], ((u8_t *)&netif_gw)[1],
                        ((u8_t *)&netif_gw)[2], ((u8_t *)&netif_gw)[3]));
    LWIP_PLATFORM_DIAG(("************************************************"));
}

/*!
 * @brief Initializes server.
 */
static void http_server_socket_init(void)
{
    HTTPSRV_PARAM_STRUCT params;
    uint32_t httpsrv_handle;

    /* Init Fs*/
    HTTPSRV_FS_init(httpsrv_fs_data);

    /* Init HTTPSRV parameters.*/
    memset(&params, 0, sizeof(params));
    params.root_dir   = "";
    params.index_page = "/index.html";
    /* params.auth_table  = auth_realms; */
    params.cgi_lnk_tbl = cgi_lnk_tbl;
    params.ssi_lnk_tbl = ssi_lnk_tbl;
#if HTTPSRV_CFG_MBEDTLS_ENABLE
    tls_params.certificate_buffer      = (const unsigned char *)mbedtls_test_srv_crt;
    tls_params.certificate_buffer_size = mbedtls_test_srv_crt_len;
    tls_params.private_key_buffer      = (const unsigned char *)mbedtls_test_srv_key;
    tls_params.private_key_buffer_size = mbedtls_test_srv_key_len;

    params.tls_param = &tls_params;
#endif
    /* Init HTTP Server.*/
    httpsrv_handle = HTTPSRV_init(&params);
    if (httpsrv_handle == 0)
    {
        LWIP_PLATFORM_DIAG(("HTTPSRV_init() is Failed"));
    }
}

/*!
 * @brief The main function containing server thread.
 */
static void main_thread(void *arg)
{
    LWIP_UNUSED_ARG(arg);

    stack_init();
    http_server_socket_init();

    vTaskDelete(NULL);
}

/*!
 * @brief Main function.
 */
int main(void)
{
    uint32_t state;

    gpio_pin_config_t gpio_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitModuleClock();

    IOMUXC_EnableMode(IOMUXC_GPR, kIOMUXC_GPR_ENET1TxClkOutputDir, true);

    /* Data cache must be temporarily disabled to be able to use sdram */
    SCB_DisableDCache();

    GPIO_PinInit(GPIO1, 9, &gpio_config);
    GPIO_PinInit(GPIO1, 10, &gpio_config);
    /* pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO1, 10, 1);
    GPIO_WritePinOutput(GPIO1, 9, 0);
    delay();
    GPIO_WritePinOutput(GPIO1, 9, 1);
    mflash_drv_init();

    /* Query bootloader for state */
    if (bl_get_image_state(&state) != kStatus_Success)
    {
        /* Error retrieving status, the bootloader might not be installed correctly */
        ota_status = OTA_STATUS_BOOTLOADER_ERROR;
    }
    else if (state == kSwapType_Test)
    {
        /* Device is executiing in test mode */
        ota_status = OTA_STATUS_TESTING;
    }

    /* create server thread in RTOS */
    if (sys_thread_new("main", main_thread, NULL, HTTPD_STACKSIZE, HTTPD_PRIORITY) == NULL)
        LWIP_ASSERT("main(): Task creation failed.", 0);

    /* run RTOS */
    vTaskStartScheduler();

    /* should not reach this statement */
    for (;;)
        ;
}

#endif /* LWIP_SOCKET */
