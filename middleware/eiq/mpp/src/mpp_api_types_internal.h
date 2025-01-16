/*
 * Copyright 2020-2024 NXP.
 *
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Internal types definitions
 */
#ifndef _MPP_API_TYPES_INTERNAL
#define _MPP_API_TYPES_INTERNAL

#include "hal_camera_dev.h"
#include "hal_display_dev.h"
#include "hal_graphics_dev.h"
#include "hal_valgo_dev.h"
#include "hal_static_image.h"
#include "hal_types.h"
#include "stddef.h"

/** \internal */

/* maximum number of parameters passed from element to element */
#define MAX_ELEM_PARAMS_SIZE 8

/* maximum MPP priorty - heap depth */
#define MAX_MPP_HEAP_PRIO   8


/* max device name length*/
#define MAX_DEV_NAME        32

/* max camera private area */
#define CAMERA_MAX_PRIV_SIZE 256

/* max number of splits for a pipeline */
#define MPP_MAX_SPLIT_NUM    4

/* max number of branches per SPLIT */
#define MPP_MAX_BRANCH_NUM    4

#define MPP_STRIPE_NUM   16

#define container_of(ptr, type, member)

/* stringification */
#define STRING(s) STRING_(s)
#define STRING_(s) #s

/* labeled rectangle tag identification */
#define MPP_LBL_RECT_TAG    ((uint16_t)0x4c52)

/* source type id */
typedef enum
_mpp_src_type_e {MPP_SRC_INVALID,
				 MPP_SRC_CAMERA,
				 MPP_SRC_STATIC_IMAGE,
				 MPP_SRC_FILE,
				 MPP_SRC_NUM}
_mpp_src_type_t;

/* sink type id */
typedef enum
_mpp_sink_type_e {MPP_SINK_INVALID,
                  MPP_SINK_DISPLAY,
                  MPP_SINK_NULL,
                  MPP_SINK_NUM
}_mpp_sink_type_t;

typedef enum
_mpp_elem_type_e {MPP_TYPE_SINK,
                  MPP_TYPE_SOURCE,
                  MPP_TYPE_PROC,
                  MPP_TYPE_NUM
}_mpp_elem_type_t;

/* mpp build status */
typedef enum
_mpp_status_e {
	MPP_CREATED, /* pipeline created */
	MPP_OPENED,  /* a source has been added, elements can be added */
	MPP_CLOSED,  /* sink/split/joined element added */
}_mpp_status_t;

/* mpp operational status */
typedef enum
_mpp_oper_status_e {
	MPP_NOT_STARTED, /* pipeline not started */
	MPP_STOPPED,     /* pipeline stopped */
	MPP_RUNNING,     /* pipeline is processing */
} _mpp_oper_status_t;

/* buffer status */
typedef enum _mpp_buf_status_e {
    MPP_BUFFER_NONE = 0,    /* uninitialized status */
    MPP_BUFFER_READY,       /* buffer content ready to be read */
    MPP_BUFFER_EMPTY,       /* buffer empty can be written */
    MPP_BUFFER_READING,     /* buffer currently read */
    MPP_BUFFER_WRITTING,    /* buffer currently written */
} _mpp_buf_status_t;

/* Basic pipeline structure
 */
typedef struct _mpp_s _mpp_t;
typedef int (*dequeue_func_t) (_mpp_t *mpp);
typedef int (*enqueue_func_t) (_mpp_t *mpp);

typedef enum param_id_e {
    PARAM_ID_INVAL,
    PARAM_ID_WIDTH,
    PARAM_ID_HEIGHT,
    PARAM_ID_PIXEL_FORMAT,
    /*to be continued */
    PARAM_ID_MAX
}param_id_t;

typedef struct param_s
{
    /*parameter id */
    param_id_t id;
    /* address where the parameter is stored */
    void *param_addr;
} param_t;

typedef struct
{
    unsigned short frame_id;   /* the frame id */
    mpp_pixel_format_t format;
    int width;      /* image height */
    int height;     /* image width */
    int stripe_num; /* stripe number. 0 means no stripe*/
    int status;     /* status of the buffer */
    hw_buf_desc_t hw_req_prod;  /* buffer hw requirement from producer */
    hw_buf_desc_t hw_req_cons;  /* buffer hw requirement from consumer */
    hw_buf_desc_t *hw;          /* pointer to above producer/consumer buffer requirement finally selected */
} buf_desc_t;

typedef struct
{
    bool inplace;
    mpp_memory_policy_t mem_policy;
    int nb_in_buf;   /* number of elements in array 'i_buf_desc' */
    int nb_out_buf;   /* number of elements in array 'o_buf_desc' */
    buf_desc_t *in_buf[MAX_INPUT_PORTS]; /* input buffer descriptor (ignored if HAL_MEM_ALLOC_OUPUT/NONE) */
    buf_desc_t *out_buf[MAX_OUTPUT_PORTS]; /* output buffer descriptor (ignored if HAL_MEM_ALLOC_INPUT/NONE) */
    unsigned short last_frame_id[MAX_INPUT_PORTS]; /* frame id of the last processed input buffer(s) */
} io_desc_t;

/* forward declaration */
struct _elem_s;
typedef struct _elem_s _elem_t;

struct _mpp_s {
	/*creation params*/
	mpp_params_t params;

	/* build status */
	_mpp_status_t status;
	/* operational status */
	_mpp_oper_status_t oper_status;
	/* semaphore to avoid status change while processing: */
	/* locked when active, released when inactive */
	hal_sema_t status_sema;

	/* priority */
	unsigned int prio;

    /* first added element */
    _elem_t *first_elem;

    /* last added element */
    _elem_t *last_elem;
    /* total number of splits */
    unsigned int split_cnt;

    /* execution heap (array of pointer to mpps) */
    _mpp_t **exec_heap;

    /* element where mpp is hooked (NULL for first mpp) */
    _elem_t *hook;

    mpp_stats_t *stats;
};

/* camera source */
typedef struct _camera_dev_s {
    char name[MAX_DEV_NAME+1];
    /* parameters */
    mpp_camera_params_t params;
	/* HAL/FWK type */
	camera_dev_t dev;
}_camera_dev_t;

/* static image source */
typedef struct _static_image_s {
    /* parameters */
    mpp_img_params_t params;
    /* HAL/FWK type */
    static_image_t elt;
}_static_image_t;

/* file source */
typedef struct _file_dev_s {
	/* file info */
	_mpp_t *mpp;
}_file_dev_t;


/* display sink */
typedef struct _display_dev_s {
    char name[MAX_DEV_NAME+1];
    /* parameters */
    mpp_display_params_t params;
    /* HAL/FWK type */
    display_dev_t dev;
} _display_dev_t;

typedef int (*elem_entry_t)(_elem_t *);
typedef int (*buf_desc_func_t)(io_desc_t *);
struct _elem_s {
    /* element type */
    _mpp_elem_type_t type;

    /* element id */
    union {
        mpp_element_id_t proc_typ;  /* for processing type */
        _mpp_src_type_t src_typ;    /* for source type */
        _mpp_sink_type_t sink_typ;  /* for sink type */
    };

    /* element user parameters (processing type) */
    mpp_element_params_t params;

    /* element process function */
    union {
        elem_entry_t entry;             /* entry fct for processing type */
        dequeue_func_t src_dequeue;     /* dequeue fct for source type */
        enqueue_func_t sink_enqueue;    /* enqueue fct for sink type */
    };

    /* the possible HALs behind the element */
    union {
        gfx_dev_t *gfx;
        vision_algo_dev_t *valgo;
        _camera_dev_t *cam;
        _display_dev_t *disp;
        _static_image_t *img;
    } dev;

    /* the IO buffers descriptors */
    io_desc_t io;

    /* owning mpp */
    _mpp_t *mpp;

    _elem_t *prev;  /* previous element in pipeline */
    _elem_t *next[MPP_MAX_BRANCH_NUM];   /* next elements in pipeline */
};

typedef unsigned int (*elem_setup_func_t)(_elem_t *);
elem_setup_func_t get_setup_function(mpp_element_id_t id);

static inline int can_add(mpp_element_id_t id)
{
    switch(id) {
    case MPP_ELEMENT_LABELED_RECTANGLE:
    case MPP_ELEMENT_TEST:
    case MPP_ELEMENT_CONVERT:
    case MPP_ELEMENT_INFERENCE:
        return 1;
    default:
        return 0;
    }
}

/* labeled rectangle update function */
uint32_t mpp_lbl_rectangle_update (_elem_t *elem, mpp_element_params_t *params);

/* image convert update function */
unsigned int mpp_convert_update(_elem_t *elem, mpp_element_params_t *params);

/* inference update function */
uint32_t mpp_inference_update(_elem_t *elem, mpp_element_params_t *params);

/* create element and link it to its mpp */
int mpp_create_elem(_mpp_t *mpp, _elem_t **p_elem);

/** \endinternal */
#endif
