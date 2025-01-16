;/*
; * Copyright 2024 NXP
; *
; * All rights reserved.
; *
; * SPDX-License-Identifier: BSD-3-Clause
; */

#if defined(__CC_ARM)

        AREA ezhv_code, DATA, READONLY, PREINIT_ARRAY, ALIGN=3
        EXPORT ezhv_image_start
        EXPORT ezhv_image_end
ezhv_image_start
        INCBIN ezhv_image.bin
ezhv_image_end
        END

#elif defined(__GNUC__) || defined(__ARMCC_VERSION)

        .section .ezhv_code, "ax" @progbits @preinit_array
        .global ezhv_image_start
        .type ezhv_image_start, %object
        .align 4
ezhv_image_start:
        .incbin "ezhv_image.bin"
        .global ezhv_image_end
        .type ezhv_image_end, %object
ezhv_image_end:
        .global ezhv_image_size
        .type ezhv_image_size, %object
        .align 4
ezhv_image_size:
        .int  ezhv_image_end - ezhv_image_start
        .end

#endif
