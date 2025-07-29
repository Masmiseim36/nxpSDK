/*
 * Copyright 2021-2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(__GNUC__) || defined(__ARMCC_VERSION)

        .section .ezhvcode , "ax" @progbits @preinit_array
        .global ezhv_image_start
        .type ezhv_image_start, %object
        .align 2
ezhv_image_start:
        .incbin "ezhv.bin"
        .global ezhv_image_end
        .type ezhv_image_end, %object
ezhv_image_end:
        .global ezhv_image_size
        .type ezhv_image_size, %object
        .align 2
ezhv_image_size:
        .int  ezhv_image_end - ezhv_image_start
        .end

#endif
