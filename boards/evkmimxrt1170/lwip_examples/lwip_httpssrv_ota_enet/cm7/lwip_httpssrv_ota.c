/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#include "lwip/api.h"

#include "pin_mux.h"
#include "board.h"

#include "ksdk_mbedtls.h"
#include "httpsrv.h"
#include "httpsrv_port.h"

#include "mbedtls/certs.h"

#include "mcuboot_app_support.h"
#include "mflash_drv.h"

#include "timers.h"

#include "fsl_debug_console.h"

#include "fsl_enet.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG LWIP_DBG_ON
#endif
#ifndef HTTPD_STACKSIZE
#define HTTPD_STACKSIZE 1000
#endif
#ifndef HTTPD_PRIORITY
#define HTTPD_PRIORITY DEFAULT_THREAD_PRIO
#endif
#ifndef DEBUG_WS
#define DEBUG_WS 0
#endif

#define OTA_UPDATE_PART     (BL_FEATURE_SECONDARY_IMG_START - FlexSPI_AMBA_BASE)
#define OTA_MAX_IMAGE_SIZE  (BL_FEATURE_PRIMARY_IMG_PARTITION_SIZE - BL_IMG_HEADER_SIZE)
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
    const clock_sys_pll1_config_t sysPll1Config = {
        .pllDiv2En = true,
    };
    CLOCK_InitSysPll1(&sysPll1Config);

#if BOARD_NETWORK_USE_100M_ENET_PORT
    clock_root_config_t rootCfg = {.mux = 4, .div = 10}; /* Generate 50M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet1, &rootCfg);
#else
    clock_root_config_t rootCfg = {.mux = 4, .div = 4}; /* Generate 125M root clock. */
    CLOCK_SetRootClock(kCLOCK_Root_Enet2, &rootCfg);
#endif
}

void IOMUXC_SelectENETClock(void)
{
#if BOARD_NETWORK_USE_100M_ENET_PORT
    IOMUXC_GPR->GPR4 |= IOMUXC_GPR_GPR4_ENET_REF_CLK_DIR_MASK; /* 50M ENET_REF_CLOCK output to PHY and ENET module. */
#else
    IOMUXC_GPR->GPR5 |= IOMUXC_GPR_GPR5_ENET1G_RGMII_EN_MASK; /* bit1:iomuxc_gpr_enet_clk_dir
                                                                 bit0:GPR_ENET_TX_CLK_SEL(internal or OSC) */
#endif
}

void BOARD_ENETFlexibleConfigure(enet_config_t *config)
{
#if BOARD_NETWORK_USE_100M_ENET_PORT
    config->miiMode = kENET_RmiiMode;
#else
    config->miiMode = kENET_RgmiiMode;
#endif
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
#define FORM_DATA_NAME_SIZE    (20)

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

    if (ctx->state == MULTIPART_ERROR)
    {
        return -1;
    }

    if (ctx->state != MULTIPART_EXPECT_DATA)
    {
        return 0;
    }

    /* Copy data from receive buffer to caller buffer while searching for boundary string using a state machine */
    while (read_total != len)
    {
        /* If the buffer contains just partially matched boundary string (or is completely empty) we need to receive
         * more data */
        if (ctx->buf_start + match_idx >= ctx->buf_end)
        {
            uint32_t read;

            /* Move the unprocessed data (partially matched boundary string) to the beginning of the buffer */
            memmove(ctx->buffer, ctx->buf_start, ctx->buf_end - ctx->buf_start);
            ctx->buf_end -= ctx->buf_start - ctx->buffer;
            ctx->buf_start = ctx->buffer;

            /* Top up the buffer */
            read = HTTPSRV_cgi_read(ctx->ses_handle, ctx->buf_end, ctx->buf_size - (ctx->buf_end - ctx->buf_start));
            if (read == 0)
            {
                /* End od stream unexpected at this point */
                ctx->state = MULTIPART_ERROR;
                return -1;
            }
            ctx->buf_end += read;
        }

        /* If there is a match with boundary string */
        if (ctx->buf_start[match_idx] == ctx->boundary[match_idx])
        {
            /* If this is the last character of the bundary string */
            if (++match_idx == ctx->boundary_len)
            {
                /* Boundary found, consume it and exit the loop (end of data part) */
                ctx->buf_start += match_idx;
                break;
            }
            continue;
        }

        /* Mismatch, reset matching index */
        match_idx = 0;

        /* The character is not part of valid boundary string for sure, copy it to the caller provided buffer */
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

/* receive update image and store it in the flash partition */
int32_t store_update_image(struct multipart_read_ctx *ctx, uint32_t partition_log_addr, uint32_t partition_size)
{
    int32_t retval = 0;

    uint32_t partition_phys_addr;

    uint32_t chunk_flash_addr;
    int32_t chunk_len;

    uint32_t next_erase_addr;

    /* Page buffers */
    uint32_t page_size;
    uint32_t *page_buffer;

    /* Received/processed data counter */
    uint32_t total_processed = 0;

    /* Preset result code to indicate "no error" */
    int32_t mflash_result = 0;

    /* Obtain physical address of FLASH to perform operations with */
    partition_phys_addr = mflash_drv_log2phys((void *)partition_log_addr, partition_size);
    if (partition_phys_addr == MFLASH_INVALID_ADDRESS)
        return -1;

    /* Check partition alignment */
    if (!mflash_drv_is_sector_aligned(partition_phys_addr) || !mflash_drv_is_sector_aligned(partition_size))
    {
        PRINTF("store_update_image: partition not aligned\r\n");
        return -1;
    }

    /* Allocate page buffer(s) is one go */
    page_size   = MFLASH_PAGE_SIZE;
    page_buffer = httpsrv_mem_alloc(page_size);
    if (page_buffer == NULL)
    {
        PRINTF("store_update_image: page buffer allocation error\r\n");
        return -1;
    }

    /* Pre-set address of area not erased so far */
    next_erase_addr  = partition_phys_addr;
    chunk_flash_addr = partition_phys_addr;
    total_processed  = 0;

    do
    {
        /* The data is epxected for be received by page sized chunks (except for the last one) */
        chunk_len = multipart_read_data(ctx, (uint8_t *)page_buffer, page_size);

        if (chunk_len > 0)
        {
            if (chunk_flash_addr >= partition_phys_addr + partition_size)
            {
                /* Partition boundary exceeded */
                PRINTF("\rstore_update_image: partition boundary exceedded");
                retval = -1;
                break;
            }

            /* Perform erase when encountering next sector */
            if (chunk_flash_addr >= next_erase_addr)
            {
                mflash_result = mflash_drv_sector_erase(next_erase_addr);
                if (mflash_result != 0)
                {
                    break;
                }
                next_erase_addr += MFLASH_SECTOR_SIZE;
            }

            /* Clear the unused portion of the buffer (applicable to the last chunk) */
            if (chunk_len < page_size)
            {
                memset((uint8_t *)page_buffer + chunk_len, 0xff, page_size - chunk_len);
            }

            /* Program the page */
            mflash_result = mflash_drv_page_program(chunk_flash_addr, page_buffer);
            if (mflash_result != 0)
            {
                break;
            }

            total_processed += chunk_len;
            chunk_flash_addr += chunk_len;

            PRINTF("\rstore_update_image: processed %i bytes", total_processed);
        }

    } while (chunk_len == page_size);

    /* If there was error reading multipart content, report failure */
    if ((chunk_len < 0))
    {
        PRINTF("store_update_image: error reading data\r\n");
        retval = -1;
    }

    /* Check result of the last flash operation */
    if (mflash_result != 0)
    {
        /* Error during flash operation */
        PRINTF("\rstore_update_image: FLASH ERROR AT 0x%x\r\n", chunk_flash_addr);
        retval = -1;
    }

    /* Unless retval is already set (to an error code) */
    if (retval == 0)
    {
        PRINTF("\r\nstore_update_image: upload complete\r\n");
        retval = total_processed;
    }

    /* Clean up */
    httpsrv_mem_free(page_buffer);

    return retval;
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
                    PRINTF("Could not get update partition info\r\n");
                    response.status_code = HTTPSRV_CODE_INTERNAL_ERROR;
                }
                else if ((stored = store_update_image(mpr_ctx, update_partition.start, update_partition.size)) <= 0)
                {
                    /* Error during upload */
                    PRINTF("Error during upload\r\n");
                    response.status_code = HTTPSRV_CODE_INTERNAL_ERROR;
                }
                else if (bl_verify_image((void *)update_partition.start, stored) <= 0)
                {
                    /* Image validation failed */
                    PRINTF("Image validation failed\r\n");
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
    PRINTF("SystemReset\r\n");
    vTaskDelay(100 / portTICK_PERIOD_MS);
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
        /* There is an update waiting, instruct bootloader to test it */
        status_t status;
        status = bl_update_image_state(kSwapType_ReadyForTest);
        PRINTF("update_image_state(%i): %i\r\n", kSwapType_ReadyForTest, status);
    }

    if (reboot_timer == NULL)
    {
        /* Actual reboot is delayed to give the HTTP server a chance to send the content generated by CGI and gracefully
         * close the connection */
        reboot_timer = xTimerCreate("reboot_timer", pdMS_TO_TICKS(3000), pdFALSE, NULL, reboot_timer_callback);
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

    if (1 || ota_status == OTA_STATUS_TESTING)
    {
        /* There is an update under test, instruct bootloader to make it permanent */
        status_t status;
        status = bl_update_image_state(kSwapType_Permanent);
        PRINTF("update_image_state(%i): %i\r\n", kSwapType_Permanent, status);
        ota_status = OTA_STATUS_NONE;
    }

    return (response.content_length);
}

#if HTTPSRV_CFG_MBEDTLS_ENABLE
static HTTPSRV_TLS_PARAM_STRUCT tls_params;
#endif

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
 * @brief Initializes lwIP stack.
 */
int initNetwork(void);

/*!
 * @brief The main function containing server thread.
 */
static void main_thread(void *arg)
{
    LWIP_UNUSED_ARG(arg);

    initNetwork();

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

    SCB_DisableDCache();

    IOMUXC_SelectENETClock();

#if BOARD_NETWORK_USE_100M_ENET_PORT
    BOARD_InitEnetPins();
    GPIO_PinInit(GPIO9, 11, &gpio_config);
    GPIO_PinInit(GPIO12, 12, &gpio_config);
    /* Pull up the ENET_INT before RESET. */
    GPIO_WritePinOutput(GPIO9, 11, 1);
    GPIO_WritePinOutput(GPIO12, 12, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO12, 12, 1);
    SDK_DelayAtLeastUs(6, CLOCK_GetFreq(kCLOCK_CpuClk));
#else
    BOARD_InitEnet1GPins();
    GPIO_PinInit(GPIO11, 14, &gpio_config);
    /* For a complete PHY reset of RTL8211FDI-CG, this pin must be asserted low for at least 10ms. And
     * wait for a further 30ms(for internal circuits settling time) before accessing the PHY register */
    GPIO_WritePinOutput(GPIO11, 14, 0);
    SDK_DelayAtLeastUs(10000, CLOCK_GetFreq(kCLOCK_CpuClk));
    GPIO_WritePinOutput(GPIO11, 14, 1);
    SDK_DelayAtLeastUs(30000, CLOCK_GetFreq(kCLOCK_CpuClk));

    EnableIRQ(ENET_1G_MAC0_Tx_Rx_1_IRQn);
    EnableIRQ(ENET_1G_MAC0_Tx_Rx_2_IRQn);
#endif
    CRYPTO_InitHardware();

    mflash_drv_init();

    /* Query bootloader for state */
    if (bl_get_image_state(&state) != kStatus_Success)
    {
        /* Error retrieving status, the bootloader might not be installed correctly */
        ota_status = OTA_STATUS_BOOTLOADER_ERROR;
    }
    else if (state == kSwapType_Testing)
    {
        /* Device is executing in test mode */
        ota_status = OTA_STATUS_TESTING;
    }

    PRINTF("Build " __DATE__ " " __TIME__ "\r\n");

    /* create server thread in RTOS */
    if (sys_thread_new("main", main_thread, NULL, HTTPD_STACKSIZE, HTTPD_PRIORITY) == NULL)
        LWIP_ASSERT("main(): Task creation failed.", 0);

    /* run RTOS */
    vTaskStartScheduler();

    /* should not reach this statement */
    for (;;)
        ;
}
