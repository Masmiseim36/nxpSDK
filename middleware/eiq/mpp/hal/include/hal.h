/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * HAL functions public header
 */

#ifndef _HAL_H
#define _HAL_H

#include <stdint.h>
#include "mpp_api_types.h"
#include "mpp_config.h"

#include "hal_graphics_dev.h"
#include "hal_os.h"
#include "hal_valgo_dev.h"

#include "hal_debug.h"


int hal_label_rectangle(uint8_t *frame, int width, int height, mpp_pixel_format_t format,
                        mpp_labeled_rect_t *lr);

int hal_img_convert_setup(gfx_dev_t *dev);

int hal_inference_tflite_setup(vision_algo_dev_t *dev);


#endif /* _HAL_H */
