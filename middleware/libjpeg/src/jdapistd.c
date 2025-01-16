/*
 * jdapistd.c
 *
 * Copyright (C) 1994-1996, Thomas G. Lane.
 * Modified 2002-2013 by Guido Vollbeding.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains application interface code for the decompression half
 * of the JPEG library.  These are the "standard" API routines that are
 * used in the normal full-decompression case.  They are not used by a
 * transcoding-only application.  Note that if an application links in
 * jpeg_start_decompress, it will end up linking in the entire decompressor.
 * We thus must separate this file from jdapimin.c to avoid linking the
 * whole decompression library into a transcoder.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


/* Forward declarations */
LOCAL(boolean) output_pass_setup JPP((j_decompress_ptr cinfo));

/*
 * Decompression initialization.
 * jpeg_read_header must be completed before calling this.
 *
 * If a multipass operating mode was selected, this will do all but the
 * last pass, and thus may take a great deal of time.
 *
 * Returns FALSE if suspended.  The return value need be inspected only if
 * a suspending data source is used.
 */

GLOBAL(boolean)
jpeg_start_decompress (j_decompress_ptr cinfo)
{
  boolean status = TRUE;
#if LIB_JPEG_USE_HW_ACCEL
  JPEG_DECODER_Type *jpegdec = cinfo->jpegdec;
  cinfo->format_supported = FALSE;
  jpegdec_pixel_format_t format;

  /* JPEGDEC
     1. Do not support CSC or scale
     2. Only support precision of 8 or 12
     3. The jpeg buffer address shall be 16 byte alined
     4. Check if the buffer address or file size is 0, if so means jpeg_stdio_src
        is used, and the file is read section by section, which is not supported.
     5. For the supported jpeg formats, the width and height shall all be divisible by 8
     6. Only support SOF0 and SOF1 marker, which means does not support progressive_mode and arith_code
  */
  if ((cinfo->jpeg_color_space == cinfo->out_color_space) && (cinfo->scale_num == cinfo->scale_denom)
     && ((cinfo->data_precision == 8U) || (cinfo->data_precision == 12))
     && (((uint32_t)cinfo->jpeg_buffer & 0xF) == 0U)
     && ((uint32_t)cinfo->jpeg_buffer != 0U) && (cinfo->jpeg_size != 0U)
     && ((cinfo->image_height & 0x7U) == 0U) && ((cinfo->image_width & 0x7U) == 0U)
     && (cinfo->progressive_mode == FALSE) && (cinfo->arith_code == FALSE))
  {
      switch (cinfo->jpeg_color_space)
      {
          case JCS_GRAYSCALE:
              format = kJPEGDEC_PixelFormatGray;
              cinfo->format_supported = TRUE;
              break;
          case JCS_RGB:
              format = kJPEGDEC_PixelFormatRGB;
              cinfo->format_supported = TRUE;
              break;
          case JCS_CMYK:
          case JCS_YCCK:
              /* JPEGDEC treats formats that have 4 components as the same. */
              format = kJPEGDEC_PixelFormatYCCK;
              cinfo->format_supported = TRUE;
              break;
          case JCS_YCbCr:
              /* Only support YUV444, all other YUV formats are not supported. For YUV422 and YUV420,
                 the libjpeg decodes them and generates YUV444 pixel, but for JPEGDEC, the
                 generated pixel format are the same as the dource format. To align the behaviour
                 these 2 formats are not supported. */
              if ((cinfo->max_v_samp_factor == 1U) && (cinfo->max_h_samp_factor == 1U))
              {
                  cinfo->format_supported = TRUE;
                  format = kJPEGDEC_PixelFormatYUV444;
              }
              break;
          default:
              /* No action, all other formats are not supported. */
              break;
      }
  }
  if (cinfo->format_supported)
  {
      /* The format is supported by JPEGDEC, configure and start the decompress. */
      jpegdec_config_t config;
      jpegdec_decoder_config_t decConfig;

      /* Init JPEG decoder module. */
      JPEGDEC_GetDefaultConfig(&config);

      config.slots = kJPEGDEC_Slot0; /* Enable only one slot. */

      JPEGDEC_Init(jpegdec, &config);

      /* Has to be 16-byte aligned. */
      /* Set decoder configuration. */
      decConfig.jpegBufAddr = (uint32_t)cinfo->jpeg_buffer;
      decConfig.jpegBufSize = JPEG_ALIGN_SIZE(cinfo->jpeg_size, 0x400); /* Has to be integer times of 1K. */
      decConfig.outBufAddr0 = (uint32_t)cinfo->output_buffer;
      decConfig.outBufAddr1 = 0U; /* Only use one planner. */
      decConfig.outBufPitch = cinfo->output_pitch;
      decConfig.pixelFormat = format;
      decConfig.height = cinfo->image_height;
      decConfig.width = cinfo->image_width;
      decConfig.pixelDepth = cinfo->data_precision;
      decConfig.clearStreamBuf = false;
      decConfig.autoStart = false;

      JPEGDEC_ConfigDecoder(jpegdec, &decConfig);

      /* No scaling. */
      cinfo->output_height = cinfo->image_height;
      cinfo->output_width = cinfo->image_width;

      /* Start the decoder. */
      JPEGDEC_StartDecode(jpegdec);
  }
  else
#endif
  {
      if (cinfo->global_state == DSTATE_READY) {
        /* First call: initialize master control, select active modules */
        jinit_master_decompress(cinfo);
        if (cinfo->buffered_image) {
          /* No more work here; expecting jpeg_start_output next */
          cinfo->global_state = DSTATE_BUFIMAGE;
          return TRUE;
        }
        cinfo->global_state = DSTATE_PRELOAD;
      }
      if (cinfo->global_state == DSTATE_PRELOAD) {
        /* If file has multiple scans, absorb them all into the coef buffer */
        if (cinfo->inputctl->has_multiple_scans) {
#ifdef D_MULTISCAN_FILES_SUPPORTED
          for (;;) {
            int retcode;
            /* Call progress monitor hook if present */
            if (cinfo->progress != NULL)
              (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
            /* Absorb some more input */
            retcode = (*cinfo->inputctl->consume_input) (cinfo);
            if (retcode == JPEG_SUSPENDED)
              return FALSE;
            if (retcode == JPEG_REACHED_EOI)
              break;
            /* Advance progress counter if appropriate */
            if (cinfo->progress != NULL &&
                (retcode == JPEG_ROW_COMPLETED || retcode == JPEG_REACHED_SOS)) {
              if (++cinfo->progress->pass_counter >= cinfo->progress->pass_limit) {
                /* jdmaster underestimated number of scans; ratchet up one scan */
                cinfo->progress->pass_limit += (long) cinfo->total_iMCU_rows;
              }
            }
          }
#else
          ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif /* D_MULTISCAN_FILES_SUPPORTED */
        }
        cinfo->output_scan_number = cinfo->input_scan_number;
      } else if (cinfo->global_state != DSTATE_PRESCAN)
        ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
      /* Perform any dummy output passes, and set up for the final pass */
      status = output_pass_setup(cinfo);
  }
  return status;
}

/*
 * Set up for an output pass, and perform any dummy pass(es) needed.
 * Common subroutine for jpeg_start_decompress and jpeg_start_output.
 * Entry: global_state = DSTATE_PRESCAN only if previously suspended.
 * Exit: If done, returns TRUE and sets global_state for proper output mode.
 *       If suspended, returns FALSE and sets global_state = DSTATE_PRESCAN.
 */

LOCAL(boolean)
output_pass_setup (j_decompress_ptr cinfo)
{
  if (cinfo->global_state != DSTATE_PRESCAN) {
    /* First call: do pass setup */
    (*cinfo->master->prepare_for_output_pass) (cinfo);
    cinfo->output_scanline = 0;
    cinfo->global_state = DSTATE_PRESCAN;
  }
  /* Loop over any required dummy passes */
  while (cinfo->master->is_dummy_pass) {
#ifdef QUANT_2PASS_SUPPORTED
    /* Crank through the dummy pass */
    while (cinfo->output_scanline < cinfo->output_height) {
      JDIMENSION last_scanline;
      /* Call progress monitor hook if present */
      if (cinfo->progress != NULL) {
	cinfo->progress->pass_counter = (long) cinfo->output_scanline;
	cinfo->progress->pass_limit = (long) cinfo->output_height;
	(*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
      }
      /* Process some data */
      last_scanline = cinfo->output_scanline;
      (*cinfo->main->process_data) (cinfo, (JSAMPARRAY) NULL,
				    &cinfo->output_scanline, (JDIMENSION) 0);
      if (cinfo->output_scanline == last_scanline)
	return FALSE;		/* No progress made, must suspend */
    }
    /* Finish up dummy pass, and set up for another one */
    (*cinfo->master->finish_output_pass) (cinfo);
    (*cinfo->master->prepare_for_output_pass) (cinfo);
    cinfo->output_scanline = 0;
#else
    ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif /* QUANT_2PASS_SUPPORTED */
  }
  /* Ready for application to drive output pass through
   * jpeg_read_scanlines or jpeg_read_raw_data.
   */
  cinfo->global_state = cinfo->raw_data_out ? DSTATE_RAW_OK : DSTATE_SCANNING;
  return TRUE;
}


/*
 * Read some scanlines of data from the JPEG decompressor.
 *
 * The return value will be the number of lines actually read.
 * This may be less than the number requested in several cases,
 * including bottom of image, data source suspension, and operating
 * modes that emit multiple scanlines at a time.
 *
 * Note: we warn about excess calls to jpeg_read_scanlines() since
 * this likely signals an application programmer error.  However,
 * an oversize buffer (max_lines > scanlines remaining) is not an error.
 */

GLOBAL(JDIMENSION)
jpeg_read_scanlines (j_decompress_ptr cinfo, JSAMPARRAY scanlines,
		     JDIMENSION max_lines)
{
  JDIMENSION row_ctr = 0U;

#if LIB_JPEG_USE_HW_ACCEL
  uint32_t status;
  JPEG_DECODER_Type *jpegdec = cinfo->jpegdec;
  if (cinfo->format_supported)
  {
      status = JPEGDEC_GetStatusFlags(jpegdec, 0);
      if ((status & (kJPEGDEC_DecodeCompleteFlag | kJPEGDEC_ErrorFlags)) != 0U)
      {
          JPEGDEC_ClearStatusFlags(jpegdec, 0, status);
          /* Error occur */
          if ((status & kJPEGDEC_DecodeCompleteFlag) == 0U)
          {
              ERREXIT(cinfo, JERR_HARDWARE_ERROR);
          }
          else
          {
              cinfo->output_scanline = cinfo->output_height;
              row_ctr = cinfo->output_height;
          }
      }

  }
  else
#endif
  {
    if (cinfo->global_state != DSTATE_SCANNING)
      ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
    if (cinfo->output_scanline >= cinfo->output_height) {
      WARNMS(cinfo, JWRN_TOO_MUCH_DATA);
      return 0;
    }

    /* Call progress monitor hook if present */
    if (cinfo->progress != NULL) {
      cinfo->progress->pass_counter = (long) cinfo->output_scanline;
      cinfo->progress->pass_limit = (long) cinfo->output_height;
      (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
    }

    /* Process some data */
    row_ctr = 0;
    (*cinfo->main->process_data) (cinfo, scanlines, &row_ctr, max_lines);
    cinfo->output_scanline += row_ctr;
  }
  return row_ctr;
}


/*
 * Alternate entry point to read raw data.
 * Processes exactly one iMCU row per call, unless suspended.
 */

GLOBAL(JDIMENSION)
jpeg_read_raw_data (j_decompress_ptr cinfo, JSAMPIMAGE data,
		    JDIMENSION max_lines)
{
  JDIMENSION lines_per_iMCU_row;

  if (cinfo->global_state != DSTATE_RAW_OK)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  if (cinfo->output_scanline >= cinfo->output_height) {
    WARNMS(cinfo, JWRN_TOO_MUCH_DATA);
    return 0;
  }

  /* Call progress monitor hook if present */
  if (cinfo->progress != NULL) {
    cinfo->progress->pass_counter = (long) cinfo->output_scanline;
    cinfo->progress->pass_limit = (long) cinfo->output_height;
    (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
  }

  /* Verify that at least one iMCU row can be returned. */
  lines_per_iMCU_row = cinfo->max_v_samp_factor * cinfo->min_DCT_v_scaled_size;
  if (max_lines < lines_per_iMCU_row)
    ERREXIT(cinfo, JERR_BUFFER_SIZE);

  /* Decompress directly into user's buffer. */
  if (! (*cinfo->coef->decompress_data) (cinfo, data))
    return 0;			/* suspension forced, can do nothing more */

  /* OK, we processed one iMCU row. */
  cinfo->output_scanline += lines_per_iMCU_row;
  return lines_per_iMCU_row;
}


/* Additional entry points for buffered-image mode. */

#ifdef D_MULTISCAN_FILES_SUPPORTED

/*
 * Initialize for an output pass in buffered-image mode.
 */

GLOBAL(boolean)
jpeg_start_output (j_decompress_ptr cinfo, int scan_number)
{
  if (cinfo->global_state != DSTATE_BUFIMAGE &&
      cinfo->global_state != DSTATE_PRESCAN)
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  /* Limit scan number to valid range */
  if (scan_number <= 0)
    scan_number = 1;
  if (cinfo->inputctl->eoi_reached &&
      scan_number > cinfo->input_scan_number)
    scan_number = cinfo->input_scan_number;
  cinfo->output_scan_number = scan_number;
  /* Perform any dummy output passes, and set up for the real pass */
  return output_pass_setup(cinfo);
}


/*
 * Finish up after an output pass in buffered-image mode.
 *
 * Returns FALSE if suspended.  The return value need be inspected only if
 * a suspending data source is used.
 */

GLOBAL(boolean)
jpeg_finish_output (j_decompress_ptr cinfo)
{
  if ((cinfo->global_state == DSTATE_SCANNING ||
       cinfo->global_state == DSTATE_RAW_OK) && cinfo->buffered_image) {
    /* Terminate this pass. */
    /* We do not require the whole pass to have been completed. */
    (*cinfo->master->finish_output_pass) (cinfo);
    cinfo->global_state = DSTATE_BUFPOST;
  } else if (cinfo->global_state != DSTATE_BUFPOST) {
    /* BUFPOST = repeat call after a suspension, anything else is error */
    ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  }
  /* Read markers looking for SOS or EOI */
  while (cinfo->input_scan_number <= cinfo->output_scan_number &&
	 ! cinfo->inputctl->eoi_reached) {
    if ((*cinfo->inputctl->consume_input) (cinfo) == JPEG_SUSPENDED)
      return FALSE;		/* Suspend, come back later */
  }
  cinfo->global_state = DSTATE_BUFIMAGE;
  return TRUE;
}

#endif /* D_MULTISCAN_FILES_SUPPORTED */
