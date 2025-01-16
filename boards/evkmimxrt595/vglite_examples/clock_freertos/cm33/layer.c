/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"

#include "layer.h"

/* GLobals */
static gradient_cache_entry_t *g_grad_cache = NULL;

#ifndef TRANSPARENT_VGLITE_COLOUR
/* VGLITE Internal color format is ABGR */
#define TRANSPARENT_VGLITE_COLOUR(a, r, g, b) \
	((uint32_t)(a) << 24) | ((uint32_t)(b) << 16) | ((uint32_t)(g) << 8) | \
	(uint32_t)r
#endif

static uint32_t ARGB_2_VGLITE_COLOR(uint32_t x)
{
	uint32_t r, g, b, a;
	a = ((x & 0xFF000000)>>24);
	r = ((x & 0x00FF0000)>>16);
	g = ((x & 0x0000FF00)>>8);
	b = ((x & 0x000000FF));

	return TRANSPARENT_VGLITE_COLOUR(a, r, g, b);
}

/*  Perform 2D matrix multiplication */
static void mat_mult(vg_lite_matrix_t *mR, vg_lite_float_t *mA, vg_lite_matrix_t *mB)
{
    int row, column;

    for (row = 0; row < 3; row++) {
        for (column = 0; column < 3; column++) {
            mR->m[row][column] =  (mA[row*3 + 0] * mB->m[0][column])
            + (mA[row*3 + 1] * mB->m[1][column])
            + (mA[row*3 + 2] * mB->m[2][column]);
        }
    }
    mR->scaleX  = mB->scaleX;
    mR->scaleY  = mB->scaleY;
    mR->angle   = mB->angle;
}

static int is_matrix_identical(vg_lite_matrix_t * m1, vg_lite_matrix_t * m2)
{
    int row;

    for (row = 0; row < 3; row++) {
        if (m1->m[row][0] != m2->m[row][0])
            return 0;
        if (m1->m[row][1] != m2->m[row][1])
            return 0;
        if (m1->m[row][2] != m2->m[row][2])
            return 0;
    }
    return 1;
}

vg_lite_error_t gradient_cache_init(void)
{
    g_grad_cache = (gradient_cache_entry_t*) pvPortMalloc(MAX_GRADIENT_CACHE * sizeof(gradient_cache_entry_t));
    if (g_grad_cache == NULL) {
        PRINTF("Error: Memory allocation failed for g_grad_cache!\n");
        return VG_LITE_OUT_OF_MEMORY;
    }
    memset(g_grad_cache, 0, MAX_GRADIENT_CACHE * sizeof(g_grad_cache));
    return VG_LITE_SUCCESS;
}

void gradient_cache_free(void)
{
    int i;
    if (g_grad_cache != NULL) {
        for (i = 0; i < MAX_GRADIENT_CACHE; i++) {
            if (g_grad_cache[i].g != NULL) {
                if (g_grad_cache[i].type == eLinearGradientCacheEntry) {
                    vg_lite_clear_linear_grad(&g_grad_cache[i].grad_data.lg.lGradient);
                } else if (g_grad_cache[i].type == eRadialGradientCacheEntry) {
                    vg_lite_clear_radial_grad(&g_grad_cache[i].grad_data.rg.rGradient);
		}
            }
        }
        vPortFree(g_grad_cache);
        g_grad_cache = NULL;
    }
}

void _gradient_stop_color_to_vglite_color(int32_t num_stop_points, stopValue_t *stops, vg_lite_color_ramp_t *vgColorRamp)
{
	int i;

	for (i = 0; i < num_stop_points && i < MAX_GRADIENT_STOP_POINTS; i++) {
		vgColorRamp[i].stop = stops[i].offset;
		vgColorRamp[i].red = ((stops[i].stop_color & 0x00FF0000) >> 16) / 255.0f;
		vgColorRamp[i].green = ((stops[i].stop_color & 0x0000FF00) >> 8) / 255.0f;
		vgColorRamp[i].blue = (stops[i].stop_color & 0x000000FF) / 255.0f;
		vgColorRamp[i].alpha = ((stops[i].stop_color & 0xFF000000) >> 24) / 255.0f;
	}
}

vg_lite_error_t gradient_cache_find(void *grad, GradientCacheEntry_t type,
	vg_lite_matrix_t *transform_matrix, gradient_cache_entry_t **ppcachedEntry)
{
	int unused_idx;
    int i;
    vg_lite_error_t error;
    gradient_cache_entry_t *cachedGradient = NULL;

    /* Reset output pointer to NULL by default, indicating cache search failed. */
    *ppcachedEntry = NULL;

    if (grad == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    if (! (type == eLinearGradientCacheEntry || type == eRadialGradientCacheEntry))
        return VG_LITE_INVALID_ARGUMENT;

    /* Check if path object for given gradient exists */
	for (i=0; i<MAX_GRADIENT_CACHE; i++) {
		cachedGradient = &g_grad_cache[i];
		if (cachedGradient->g == grad)
		{
			*ppcachedEntry = cachedGradient;
			return VG_LITE_SUCCESS;
		}
	}

	/* Find un-used descriptor */
	unused_idx = -1;
	for (i=1; i<MAX_GRADIENT_CACHE; i++) {
		cachedGradient = &g_grad_cache[i];
		if ( cachedGradient->g == NULL ) {
			unused_idx = i;
		}
	}
	if (unused_idx == -1) {
		return VG_LITE_OUT_OF_MEMORY;
	}

	cachedGradient = &g_grad_cache[unused_idx];
	/* Release memory of last gradient */
	if(cachedGradient->type == eLinearGradientCacheEntry){
		vg_lite_clear_linear_grad(&cachedGradient->grad_data.lg.lGradient);
	} else if(cachedGradient->type == eRadialGradientCacheEntry){
		vg_lite_clear_radial_grad(&cachedGradient->grad_data.rg.rGradient);
	}

	/* Allocate and cache requested gradient descriptor */
	if(type == eLinearGradientCacheEntry){
		linearGradient_t *gradient = (linearGradient_t *)grad;

		_gradient_stop_color_to_vglite_color(
				gradient->num_stop_points,
				gradient->stops,
				cachedGradient->vgColorRamp);

		cachedGradient->grad_data.lg.lGradient.count = gradient->num_stop_points;
		cachedGradient->grad_data.lg.params = gradient->linear_gradient;
		error = vg_lite_set_linear_grad(&cachedGradient->grad_data.lg.lGradient,
				cachedGradient->grad_data.lg.lGradient.count,
				cachedGradient->vgColorRamp,
				cachedGradient->grad_data.lg.params,
				VG_LITE_GRADIENT_SPREAD_PAD, 1);
		if (error != VG_LITE_SUCCESS)
			return error;

                if (is_matrix_identical(&cachedGradient->grad_data.lg.lGradient.matrix,
                        transform_matrix) == 0) {
                    cachedGradient->grad_data.lg.lGradient.matrix = *transform_matrix;
                    error = vg_lite_update_linear_grad(&cachedGradient->grad_data.lg.lGradient);
                }
	} else if(type == eRadialGradientCacheEntry){
		radialGradient_t *gradient = (radialGradient_t *)grad;

		_gradient_stop_color_to_vglite_color(
				gradient->num_stop_points,
				gradient->stops,
				cachedGradient->vgColorRamp);

		cachedGradient->grad_data.rg.rGradient.count = gradient->num_stop_points;
		cachedGradient->grad_data.rg.params = gradient->radial_gradient;

		error = vg_lite_set_radial_grad(&cachedGradient->grad_data.rg.rGradient,
				cachedGradient->grad_data.rg.rGradient.count,
				cachedGradient->vgColorRamp,
				cachedGradient->grad_data.rg.params,
				VG_LITE_GRADIENT_SPREAD_PAD, 1);
		if (error != VG_LITE_SUCCESS)
			return error;

                if (is_matrix_identical(&cachedGradient->grad_data.rg.rGradient.matrix,
                        transform_matrix) == 0) {
                    cachedGradient->grad_data.rg.rGradient.matrix = *transform_matrix;
                    error = vg_lite_update_rad_grad(&cachedGradient->grad_data.rg.rGradient);
                }
	}

	if (error != VG_LITE_SUCCESS)
		return error;

	cachedGradient->g = grad;
	cachedGradient->type = type;

	*ppcachedEntry = cachedGradient;
	return VG_LITE_SUCCESS;
}

int layer_draw(vg_lite_buffer_t *rt, UILayers_t *layer, vg_lite_matrix_t *transform_matrix)
{
	vg_lite_error_t error;

	/* matrix_size_in_float an array of 9 float values */
	const int matrix_size_in_float = 9;
	gradient_cache_entry_t *cachedGradient = NULL;

    if (rt == NULL || layer == NULL || transform_matrix == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    for (int i = 0; i < layer->img_info->path_count; i++) {
		vg_lite_matrix_t tmatrix;

		mat_mult(&tmatrix, &layer->img_info->transform[i * matrix_size_in_float], transform_matrix);
	  for (int k=0; k<2; k++) {
		  if (NO_FILL_MODE == layer->mode->hybridPath[2 * i + k].fillType)
			  continue;

		error = vg_lite_set_path_type(&layer->handle[i], layer->mode->hybridPath[2 * i + k].pathType);
		if (error != VG_LITE_SUCCESS) {
			PRINTF("\r\nERROR: Invalid path type!\r\n\r\n");
			return error;
		}

		switch (layer->mode->hybridPath[2 * i+k].fillType) {
		case STROKE:
		case FILL_CONSTANT:
			error = vg_lite_draw(rt, &layer->handle[i],
					layer->mode->fillRule[i],
					&tmatrix,
					VG_LITE_BLEND_NONE,
					ARGB_2_VGLITE_COLOR(layer->color[i]));
			if (error) {
				PRINTF("Error: vg_lite_draw() returned error %d\r\n", error);
				return error;
			}
			break;
		case FILL_LINEAR_GRAD:
			error = gradient_cache_find(layer->mode->linearGrads[i],
					eLinearGradientCacheEntry,
					&tmatrix,
					&cachedGradient);
			if (error != VG_LITE_SUCCESS)
				continue;
			if (cachedGradient == NULL) {
				PRINTF("Error: Failed to get cached linear gradient. Please increase MAX_GRADIENT_CACHE.\n");
				return VG_LITE_OUT_OF_MEMORY;
			}

			error = vg_lite_draw_linear_gradient(rt, &layer->handle[i],
					layer->mode->fillRule[i], transform_matrix,
					&cachedGradient->grad_data.lg.lGradient, 0, VG_LITE_BLEND_NONE, VG_LITE_FILTER_LINEAR);
			if (error != VG_LITE_SUCCESS)
				return error;
			break;
		case FILL_RADIAL_GRAD:
			error = gradient_cache_find(layer->mode->radialGrads[i],
					eRadialGradientCacheEntry,
					&tmatrix,
					&cachedGradient);
			if (error != VG_LITE_SUCCESS)
				return error;
			if (cachedGradient == NULL) {
				PRINTF("Error: Failed to get cached radial gradient. Please increase MAX_GRADIENT_CACHE.\n");
				return VG_LITE_OUT_OF_MEMORY;
			}

			error = vg_lite_draw_radial_gradient(rt, &layer->handle[i],
					layer->mode->fillRule[i], transform_matrix,
					&cachedGradient->grad_data.rg.rGradient, 0, VG_LITE_BLEND_NONE, VG_LITE_FILTER_LINEAR);
			if (error != VG_LITE_SUCCESS)
				return error;
			break;
		default:
			return VG_LITE_INVALID_ARGUMENT;
		}
	  }
    }
    vg_lite_finish();
    return VG_LITE_SUCCESS;
}

vg_lite_error_t layer_init(UILayers_t *layer)
{
    int i;
    vg_lite_error_t vg_err;

    if (layer == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    layer->handle  = (vg_lite_path_t *)pvPortMalloc(layer->img_info->path_count * sizeof(vg_lite_path_t));
    if (layer->handle == NULL) {
		PRINTF("\r\nERROR: Memory allocation failed for path!\r\n\r\n");
		return VG_LITE_OUT_OF_MEMORY;
	}
    memset(layer->handle, 0 , layer->img_info->path_count * sizeof(vg_lite_path_t));
    layer->matrix  = (vg_lite_matrix_t *)pvPortMalloc(layer->img_info->path_count * sizeof(vg_lite_matrix_t));
    if (layer->matrix == NULL) {
		PRINTF("\r\nERROR: Memory allocation failed for matrix!\r\n\r\n");
		return VG_LITE_OUT_OF_MEMORY;
	}
    for (i = 0; i < layer->img_info->path_count; i++) {
        path_info_t *path_info = &layer->img_info->paths_info[i];
        vg_err = vg_lite_init_path(&layer->handle[i],
                    layer->img_info->data_format,
                    VG_LITE_MEDIUM,
                    path_info->path_length,
                    path_info->path_data,
                    path_info->bounding_box[0],
                    path_info->bounding_box[1],
                    path_info->bounding_box[2],
                    path_info->bounding_box[3]);
        if (vg_err != VG_LITE_SUCCESS) {
		    PRINTF("\r\nERROR: Failed to initialize graphic artifacts!\r\n\r\n");
		    return vg_err;
	    }
        layer->handle[i].add_end = path_info->end_path_flag;

        for (int k=0; k<2; k++) {
        if(layer->mode->hybridPath[2*i+k].pathType == VG_LITE_DRAW_STROKE_PATH ||
                layer->mode->hybridPath[2*i+k].pathType == VG_LITE_DRAW_FILL_STROKE_PATH)
        {
            stroke_info_t *stroke_info = &layer->img_info->stroke_info[i];
            vg_err = vg_lite_set_stroke(&layer->handle[i],
                    stroke_info->linecap,
                    stroke_info->linejoin,
                    stroke_info->strokeWidth,
                    stroke_info->miterlimit,
                    stroke_info->dashPattern,
                    stroke_info->dashPatternCnt,
                    stroke_info->dashPhase,
                    ARGB_2_VGLITE_COLOR(stroke_info->strokeColor));

            if (vg_err != VG_LITE_SUCCESS) {
                PRINTF("\r\nERROR: %d Failed to initialize graphic artifacts!\r\n\r\n", __LINE__);
                return vg_err;
            }

            vg_err = vg_lite_set_path_type(&layer->handle[i], VG_LITE_DRAW_STROKE_PATH);
            if (vg_err != VG_LITE_SUCCESS) {
                PRINTF("\r\nERROR: Invalid path type!\r\n\r\n");
                return vg_err;
            }

            vg_err = vg_lite_update_stroke(&layer->handle[i]);
            if (vg_err != VG_LITE_SUCCESS) {
                PRINTF("\r\nERROR: %d Failed to initialize graphic artifacts!\r\n\r\n", __LINE__);
                return vg_err;
            }
        }
        }
	}

	return VG_LITE_SUCCESS;
}

vg_lite_error_t layer_free(UILayers_t *layer)
{
    if (layer == NULL)
        return VG_LITE_INVALID_ARGUMENT;

    if (layer->handle) {
        vPortFree(layer->handle);
        layer->handle = NULL;
    }
    if (layer->matrix) {
        vPortFree(layer->matrix);
        layer->matrix = NULL;
    }

	return VG_LITE_SUCCESS;
}


