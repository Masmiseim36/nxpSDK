/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DSP_IMAGE_COPY_TO_RAM
#define DSP_IMAGE_COPY_TO_RAM 1
#endif

#if DSP_IMAGE_COPY_TO_RAM

#if defined(__GNUC__) || defined(__ARMCC_VERSION)

        .section .text.dspcode , "ax" @progbits @preinit_array
        .global hifi1_vector_image_start
        .type hifi1_vector_image_start, %object
        .align 2
hifi1_vector_image_start:
        .incbin "hifi1/binary/dsp_vector_release.bin"
        .global hifi1_vector_image_end
        .type hifi1_vector_image_end, %object
hifi1_vector_image_end:
        .global hifi1_text_image_start
        .type hifi1_text_image_start, %object
        .align 2
hifi1_text_image_start:
        .incbin "hifi1/binary/dsp_text_release.bin"
        .global hifi1_text_image_end
        .type hifi1_text_image_end, %object
hifi1_text_image_end:
        .global hifi1_data_image_start
        .type hifi1_data_image_start, %object
        .align 2
hifi1_data_image_start:
        .incbin "hifi1/binary/dsp_data_release.bin"
        .global hifi1_data_image_end
        .type hifi1_data_image_end, %object
hifi1_data_image_end:
        .global hifi1_vector_image_size
        .type hifi1_vector_image_size, %object
        .align 2
hifi1_vector_image_size:
        .int  hifi1_vector_image_end - hifi1_vector_image_start
        .global hifi1_text_image_size
        .type hifi1_text_image_size, %object
        .align 2
hifi1_text_image_size:
        .int  hifi1_text_image_end - hifi1_text_image_start
        .global hifi1_data_image_size
        .type hifi1_data_image_size, %object
        .align 2
hifi1_data_image_size:
        .int  hifi1_data_image_end - hifi1_data_image_start
        .end

#endif

#endif
