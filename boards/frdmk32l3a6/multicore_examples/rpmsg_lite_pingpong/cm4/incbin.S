;/*
; * Copyright 2016 Freescale Semiconductor, Inc.
; * Copyright 2016 - 2017 NXP
; *
; * All rights reserved.
; * 
; * SPDX-License-Identifier: BSD-3-Clause
; */

#if defined(__CC_ARM)

        AREA M0CODE, DATA, READONLY, PREINIT_ARRAY, ALIGN=3
        EXPORT m0_image_start
        EXPORT m0_image_end
m0_image_start
        INCBIN core1_image.bin
m0_image_end
        END

#elif defined(__GNUC__) || defined(__ARMCC_VERSION)

        .section .m0code , "ax" @progbits @preinit_array
        .global m0_image_start
        .type m0_image_start, %object
        .align 4
m0_image_start:
        .incbin "core1_image.bin"
        .global m0_image_end
        .type m0_image_end, %object
m0_image_end:
        .global m0_image_size
        .type m0_image_size, %object
        .align 4
m0_image_size:
        .int  m0_image_end - m0_image_start
        .end

#endif
