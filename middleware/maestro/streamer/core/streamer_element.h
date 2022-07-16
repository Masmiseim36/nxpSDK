/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef STREAMER_ELEMENT_H
#define STREAMER_ELEMENT_H

/*!
 * @file    streamer_element.h
 * @brief   Header for Streamer Element
 */

#include "streamer_api.h"
#include "pad.h"

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/*----------------------------- Generic element -----------------------------*/

/* Element flags */

/** @brief Get element parent pipeline */
#define ELEMENT_PARENT_PIPELINE(pstelem) ((Pipeline *)((pstelem)->parent))
/** @brief Get element parent (void) */
#define ELEMENT_PARENT(pstelem) ((pstelem)->parent)
/** @brief Get element key */
#define ELEMENT_KEY(pstelem) ((pstelem)->key)
/** @brief Get element depth */
#define ELEMENT_DEPTH(pstelem) ((pstelem)->depth)
/** @brief Get element type */
#define ELEMENT_TYPE(pstelem) ((pstelem)->type)
/** @brief Get element state */
#define ELEMENT_STATE(pstelem) ((pstelem)->state)
/** @brief Get element sibling */
#define ELEMENT_SIBLING(pstelem) ((pstelem)->sibling)
/** @brief Get element source pad */
#define ELEMENT_SRC_PAD(pstelem, i) ((StreamPad *)&(pstelem)->src_pad[i])
/** @brief Get element sink pad */
#define ELEMENT_SINK_PAD(pstelem, i) ((StreamPad *)&(pstelem)->sink_pad[i])

/** @brief Maximum number of pads */
#define MAX_NUM_PADS (3)
/** @brief Maximum location path length */
#define MAX_LOCATION_PATH_LENGTH (255)
/** @brief Maximum device name length */
#define MAX_DEVICE_NAME_LENGTH (32)

/*!
 * AUDIO_SET_FORMAT: Set output format
 * a- interleaved
 * b- endianness
 * c- signed
 * d- word length
 */
#define AUDIO_SET_FORMAT(a, b, c, d) (((d << 4) & 0xFF0) | ((c << 2) & 0x4) | ((b << 1) & 0x2) | ((a << 0) & 0x1))

/*
 *  ENUMS
 */

/*
 * STRUCTURES AND OTHER TYPEDEFS
 */

/*!
 * @brief Stream Element
 * @details Generic Element data structure
 */
struct _StreamElement
{
    /** @brief Streamer element change state func */
    int32_t (*change_state)(struct _StreamElement *, PipelineState);
    /** @brief Streamer element set property func */
    int32_t (*set_property)(struct _StreamElement *, uint16_t prop, uintptr_t val);
    /** @brief Streamer element get property func */
    int32_t (*get_property)(struct _StreamElement *, uint16_t prop, uint64_t *val_ptr);
    /** @brief Streamer element parent */
    void *parent;
    /** @brief Streamer element key */
    int8_t key;
    /** @brief Streamer element depth */
    int8_t depth;
    /** @brief Streamer element state */
    PipelineState state;
    /** @brief Streamer element type */
    StreamElementType type;
    /** @brief Streamer element source pads */
    PadSrc src_pad[MAX_NUM_PADS];
    /** @brief Streamer element sink pads */
    PadSink sink_pad[MAX_NUM_PADS];
    /** @brief Streamer element sibling */
    struct _StreamElement *sibling;
};
typedef struct _StreamElement StreamElement;

/*!
 * Element Initialization Entry
 * Specify the size of a specific element type data structure and provide a
 * function to initialize the data.
 */
typedef struct
{
    uint32_t size;                                   /*!< @brief Size of the element */
    int32_t (*init_element)(StreamElement *element); /*!< @brief Element initialization function */
} ElementInit;

/*
 * GLOBAL VARIABLE DECLARATIONS
 */

/*
 * FUNCTION PROTOTYPES
 */

/*!
 * @brief activate_pads_element
 * @details This function iterates through all the pads of the elements, first
 *          src pads and then sink, and calls the activate pad function.
 *
 * @param element Pointer to element handle
 * @param activate Activate or Deactivate the pads?
 *
 * @returns bool
 */
uint8_t activate_pads_element(StreamElement *element, uint8_t activate);

/*!
 * @brief create_element
 * @details Creates a new element of the required type. A byte key which should
 *          be unique within the same type of element.
 * @param handle Pointer to element handle
 * @param type Element type
 * @param key Unique key within the same type of elements to retrieve the
 *        handle from the pipeline later.
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                        Element created successfully
 * @retval STREAM_ERR_NO_MEM                Error allocating memory
 * @retval STREAM_ERR_ELEMENT_NOT_FOUND     Element type not found
 */
int32_t create_element(ElementHandle *handle, StreamElementType type, int8_t key);

/*!
 * @brief destroy_element
 * @details Function to destroy element. Make sure that this element is not
 *          linked to any other element and not a part of pipeline.
 * @param handle Element handle
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                    Element successfully destroyed
 * @retval STREAM_ERR_ELEMENT_NOT_FREE  Element still associated with a pipeline
 * @retval STREAM_ERR_INVALID_ARGS      Invalid element object
 */
int32_t destroy_element(ElementHandle handle);

/*!
 * @brief link_elements
 * @details Link elements from the source pad to sink pad.
 *
 * @param src_handle:       Element with the source pad.
 * @param src_pad_index:    Index of the source pad of element.
 * @param sink_handle:      Element with the sink pad.
 * @param sink_pad_index:   Index of the sink pad of element.
 *
 * @returns Error Status
 * @retval STREAM_OK                    Elements linked successfully
 * @retval STREAM_ERR_INVALID_ARGS      Invalid argument
 * @retval STREAM_ERR_ELEMENT_LINKED    Element already linked
 */
int32_t link_elements(ElementHandle src_handle, int8_t src_pad_index, ElementHandle sink_handle, int8_t sink_pad_index);

/*!
 * @brief unlink_elements
 * @details Unlink elements from the source pad to sink pad.
 *
 * @param src_handle: Element with the source pad.
 * @param src_pad_index: Index of the source pad of element.
 * @param sink_handle: Element with the sink pad.
 * @param sink_pad_index: Index of the sink pad of element.
 *
 * @returns Error Status
 * @retval STREAM_OK                            Elements unlinked successfully
 * @retval STREAM_ERR_ELEMENT_PADS_NOT_LINKED   Elements not linked
 * @retval STREAM_ERR_INVALID_ARGS              Invalid arguments
 */
int32_t unlink_elements(ElementHandle src_handle,
                        int8_t src_pad_index,
                        ElementHandle sink_handle,
                        int8_t sink_pad_index);

/*!
 * @brief get_element_info:
 * @details Function to return the element info.
 *
 * @param handle: Element handle
 * @param ptype: ElementType pointer to return the element type
 * @param pkey: int8_t pointer to return the uinque element key
 *
 * @returns Error Status
 * @retval STREAM_OK                    Element info successfully retrieved
 * @retval STREAM_ERR_INVALID_ARGS      Invalid argument provided
 *
 */
int32_t get_element_info(ElementHandle handle, StreamElementType *ptype, int8_t *pkey);

/*!
 * @brief send_msg_element
 * @details The function creates and sends a message to the parent pipeline

 * @param element Element from which the message is sent from
 * @param msg StreamMessage type
 * @param data Data associated with the message
 *
 * @returns Error Status
 * @retval  STREAM_OK                   Message sent successfully
 * @retval  STREAM_ERR_INVALID_ARGS     Invalid element/pipeline object
 */
int32_t send_msg_element(StreamElement *element, uint32_t msg, uintptr_t data);

/*!
 * @brief This function gets property value of element
 * @param element_hdl element handle
 * @param prop            property id
 * @param val_ptr        pointer to the address of value
 * @returns int32_t error codes
 */
int32_t element_get_property(ElementHandle element_hdl, uint16_t prop, uint64_t *val_ptr);

/*!
 * @brief this function sets property value of element
 * @param element_hdl element handle
 * @param prop            property id
 * @param val              value to be set
 * @returns int32_t error codes
 */
int32_t element_set_property(ElementHandle element_hdl, uint16_t prop, uintptr_t val);

#endif /* STREAMER_ELEMENT_H */
/* End of file */
