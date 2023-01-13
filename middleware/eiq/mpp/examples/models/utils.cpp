/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Utility functions for output tensor post-processing
 */


#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "fsl_common.h"

#define EOL "\r\n"

static int32_t area(box_data *box) {
    return (box->right - box->left +1) * (box->bottom - box->top +1);
}

float iou(box_data* box1, box_data* box2){
    assert(box1 != NULL);
    assert(box2 != NULL);

    int32_t x_1 = MAX(box1->left, box2->left);
    int32_t x_2 = MAX(box1->right, box2->right);
    int32_t y_1 = MAX(box1->top, box2->top);
    int32_t y_2 = MAX(box1->bottom, box2->bottom);
    int32_t w = MAX(0, x_2 - x_1 + 1);
    int32_t h = MAX(0, y_2 - y_1 + 1);

    float area_of_intersection = (float)w * (float)h;
    float area_of_union = (float)box1->area + (float)box2->area - area_of_intersection;

    return area_of_intersection / area_of_union;
}

static void swap_box_pointers(box_data* box_pointers[], int32_t i, int32_t j) {
	box_data* tmp = box_pointers[i];
	box_pointers[i] = box_pointers[j];
	box_pointers[j] = tmp;
}

/* Inserts boxes in box_pointers then sorts the array by score.
 * Boxes with low scores are filtered out (NULL).
 * Also pre-computes the area of remaining boxes in box->area.
 * Returns the number of boxes that have a score above score_thr.
 */
static int32_t sort_boxes(box_data boxes[], box_data* box_pointers[], int32_t num_boxes, float score_thr) {

	/* initialize array of pointers to boxes */
	for(int32_t i = 0; i < num_boxes; i++) {
		box_pointers[i] = &boxes[i];
	}

	/* bubble sort... */
	/* TODO: replace with quicksort */
	for(int32_t i = 0; i < num_boxes-1; i++) {
		for(int32_t j = 0; j < num_boxes-1-i; j++ ) {
			if(box_pointers[j+1]->score > box_pointers[j]->score) {
				swap_box_pointers(box_pointers, j, j+1);
			}
		}
	}

	/* filter boxes with low scores */
	/* and pre-compute area of boxes */
	int32_t n_selected_boxes = -1;
	for(int32_t i = 0; i < num_boxes; i++) {
		if(box_pointers[i]->score < score_thr) {
			box_pointers[i] = 0;

			// Remember the index of the last box that had a positive score.
			// Since the boxes are sorted by score, it is the number of selected boxes.
			if (n_selected_boxes < 0) {
				n_selected_boxes = i;
			}

			continue;
		}

		box_pointers[i]->area = area(box_pointers[i]);
	}

	return n_selected_boxes;
}
/* Filters boxes using NMS threshold on IoU.
 * Boxes that are filtered out are set to NULL.
 */
static void filter_boxes(box_data* box_pointers[], int32_t num_boxes, float nms_thr) {

	for(int32_t i = 0; i < num_boxes; i++) {

		box_data* curr_box = box_pointers[i];

		/* ignore box if already suppressed */
		if(!curr_box) {
			continue;
		}

		int32_t curr_label = curr_box->label;

		for(int32_t j = i+1; j < num_boxes; j++) {

			box_data* candidate = box_pointers[j];

			/* ignore box if already suppressed */
			if(!candidate) {
				continue;
			}

			/* ignore box if label is different */
			if(candidate->label != curr_label) {
				continue;
			}

			/* remove box if IoU is too high */
			box_pointers[j] = iou(curr_box, candidate) >= nms_thr ? NULL : candidate;
		}
	}
}

void nms(box_data boxes[], box_data* box_pointers[], int32_t num_boxes, float nms_thr, float score_thr) {
    assert(box_pointers != NULL);
    assert(boxes != NULL);
	/* sort and filter boxes by score */
    int32_t n_selected_boxes = sort_boxes(boxes, box_pointers, num_boxes, score_thr);
	/* filter boxes by IoU */
	filter_boxes(box_pointers, n_selected_boxes, nms_thr);
}
