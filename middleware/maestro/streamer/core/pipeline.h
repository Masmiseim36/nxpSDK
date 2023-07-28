/*
 * Copyright 2018-2022 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef PIPELINE_H
#define PIPELINE_H

/*!
 * @file    pipeline.h
 * @brief   This file contains the definition of the pipeline object.
 */
#include "streamer_api.h"
#include "streamer_element.h"

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/**
 * @brief Pipeline state mask
 *
 */
#define PIPELINE_STATE_MASK (0x3)

/*!
 * MAX_ELEMENT_LEVEL
 * Specifies the maximum # of elements that can be present in a single pipeline
 * path.
 */
#define MAX_ELEMENT_LEVEL (5)

/**
 * @brief Get pipeline state
 *
 */
#define PIPELINE_STATE(pPipe) ((pPipe)->state)
/**
 * @brief Get pipeline level head
 *
 */
#define PIPELINE_LEVEL_HEAD(pPipe, level) (&((pPipe)->level_head[level]))

/*!
 * STATE_TRANSITION
 * @param cur: A current state
 * @param next: A next state
 *
 * Given a current state cur and a next state next, calculate the associated
 * #PipelineStateChange transition.
 */
#define STATE_TRANSITION(cur, next) (((cur) << 3) | (next))

/*!
 * STATE_TRANSITION_CURRENT
 * @param trans: A PipelineStateChange
 *
 * Given a state transition trans, extract the current #PipelineState.
 */
#define STATE_TRANSITION_CURRENT(trans) ((PipelineState)((trans) >> 3))

/*!
 * STATE_TRANSITION_NEXT
 * @param trans: A PipelineStateChange
 *
 * Given a state transition trans, extract the next #PipelineState.
 */
#define STATE_TRANSITION_NEXT(trans) ((PipelineState)((trans)&0x7))

/*
 *  ENUMS
 */

/*!
 * PipelineStateChange
 * The different state changes that are passed to the
 * state change functions of elements.
 */
typedef enum
{
    STATE_CHANGE_NULL_TO_READY     = STATE_TRANSITION(STATE_NULL, STATE_READY),     /*!< @brief NULL to READY */
    STATE_CHANGE_READY_TO_PAUSED   = STATE_TRANSITION(STATE_READY, STATE_PAUSED),   /*!< @brief READY to PAUSED */
    STATE_CHANGE_PAUSED_TO_PLAYING = STATE_TRANSITION(STATE_PAUSED, STATE_PLAYING), /*!< @brief PAUSED to PLAYING */
    STATE_CHANGE_PLAYING_TO_PAUSED = STATE_TRANSITION(STATE_PLAYING, STATE_PAUSED), /*!< @brief PLAYING to PAUSED */
    STATE_CHANGE_PAUSED_TO_READY   = STATE_TRANSITION(STATE_PAUSED, STATE_READY),   /*!< @brief PAUSED to READY */
    STATE_CHANGE_READY_TO_NULL     = STATE_TRANSITION(STATE_READY, STATE_NULL)      /*!< @brief READY to NULL */
} PipelineStateChange;

/*
 * STRUCTURES AND OTHER TYPEDEFS
 */

/*!
 * Pipeline
 * Pipeline object structure
 */
struct _Pipeline
{
    PipelineState state;
    /*!< @brief Current Pipeline State */
    unsigned int index;
    /*!< @brief Pipeline Index within Streamer Task */
    osa_msgq_handle_t *app_mq;
    /*!< @brief Application Message Queue */
    StreamElement level_head[MAX_ELEMENT_LEVEL];
    /*!< @brief Element Head Array */
    StreamPipelineType type; /*!< @brief the pipeline type */
    bool repeat;             /*!< @brief should pipeline play current stream repeatedly */

    TrackInfo track_info; /*!< @brief Track info cached */
};
typedef struct _Pipeline Pipeline;

/*
 * GLOBAL VARIABLE DECLARATIONS
 */

/*
 * FUNCTION PROTOTYPES
 */

/*!
 * @brief   Process Pipeline
 * @details Iterate through the elements of a pipeline calling process handlers
 *          for non-blocked elements
 *
 * @param [in] handle:    Pointer to pipeline
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                Success
 * @retval STREAM_ERR_INVALID_ARGS  Invalid Pipeline specified
 *
 */
int32_t process_pipeline(PipelineHandle handle);

/*!
 * @brief create_pipeline
 * @details Create a pipeline object within the streamer task
 *
 * @param [in] handle:  Pointer to pipeline
 * @param [in] index:   Index of pipeline within streamer task
 * @param [in] type:    Pipeline type
 * @param [in] app_mq:  Application message queue
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                Success
 * @retval STREAM_ERR_INVALID_ARGS  Invalid pipeline handle
 * @retval STREAM_ERR_NO_MEM        Unable to allocate memory
 */
int32_t create_pipeline(PipelineHandle *handle, unsigned int index, StreamPipelineType type, osa_msgq_handle_t *app_mq);

/*!
 * @brief destroy_pipeline
 * @details Sets the pipeline state to NULL and destroys the pipeline with all
 *          the elements added.
 *
 * @param [in] handle:   Pointer to pipeline
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                Success
 * @retval STREAM_ERR_INVALID_ARGS  Invalid Pipeline object
 */
int32_t destroy_pipeline(PipelineHandle handle);

/*!
 * @brief get_state_pipeline
 * @details Get the current state of the pipeline
 *
 * @param [in] handle:   Pointer to pipeline
 *
 * @returns Current Pipeline State
 *
 * @retval STATE_NULL           Pipeline is in NULL state
 * @retval STATE_READY          Pipeline is in Ready state
 * @retval STATE_PAUSED         Pipeline is in Paused state
 * @retval STATE_PLAYING        Pipeline is in Playing state
 */
PipelineState get_state_pipeline(PipelineHandle handle);

/*!
 * @brief set_state_pipeline
 *
 * @details Sets a new state of the pipeline if not already in the same state.
 * This takes care of any intermediate states like changing state from NULL to
 * PLAYING.
 *
 * The order of the states is NULL, READY, PAUSED and then PLAYING. Element can
 * switch from any state to another, but in steps of one.
 *
 * Once a state change is requested, the pipeline elements are iterated from the
 * sink elements to the source elements. This makes sure that when changing the
 * state of an element, the downstream elements are in the correct state to
 * process the eventual buffers. In the case of a downwards state change, e.g.
 * PLAYING ->NULL, the sink elements will shut down first which makes the
 * upstream elements shutdown as well since the _push() function returns a
 * FLOW_WRONG_STATE error.
 *
 * @param   handle Pointer to pipeline
 * @param   state New pipeline state
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                    State retrieved successfully.
 * @retval STREAM_ERR_INVALID_ARGS      Invalid pipeline object
 */
int32_t set_state_pipeline(PipelineHandle handle, PipelineState state);

/*!
 * @brief seek_pipeline
 *
 * @details Seeks the playback position to a specified absolute time in msec.
 *          This is valid only in playing and paused state.
 *          Pipeline returns to the last when seek is done.
 *
 * @param handle Pipeline handle
 * @param time_msec Seek time specified in msec.
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                    Seek successful
 * @retval STREAM_ERR_INVALID_ARGS      Invalid Pipeline object
 * @retval STREAM_ERR_NOT_SEEKABLE      Pipeline does not support seeking
 *
 */
int32_t seek_pipeline(PipelineHandle handle, uint32_t time_msec);

/*!
 * @brief seek_relative
 * @details Seeks the playback position to a time specified relative to the
 *          current playback position.  This is valid only in playing and paused
 *          state. Pipeline returns to the last when seek is done.
 *
 * @param handle Pipeline handle
 * @param time_msec Seek time specified in msec.
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                    Seek successful
 * @retval STREAM_ERR_INVALID_ARGS      Invalid Pipeline object
 * @retval STREAM_ERR_INFO_ABSENT       Unable to determine current position
 * @retval STREAM_ERR_NOT_SEEKABLE      Pipeline does not support seeking
 */
int32_t seek_relative(PipelineHandle handle, int32_t time_msec);

/*!
 * @brief query_info_pipeline:
 * @details Function to return info of the current playing/paused stream.
 *
 * @param handle Pipeline handle
 * @param info_type Info type queried
 * @param data Holds return query value
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                    Query successful
 * @retval STREAM_ERR_INFO_ABSENT       Query unsuccessful. Info not present
 * @retval STREAM_ERR_INVALID_ARGS      Invalid pipeline object
 *
 */
int32_t query_info_pipeline(PipelineHandle handle, StreamInfoType info_type, StreamData *data);

/*!
 * @brief add_element_pipeline
 * @details Function to add element in a already created pipeline. If not
 *          already added in the pipeline then it is added in the pipeline so
 *          that it can be linked with other elements in the pipeline.
 *
 * @param pipeline_handle Pipeline handle
 * @param element_handle Element handle
 * @param level Level of the element
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                    Element successfully added to pipeline
 * @retval STREAM_ERR_INVALID_ARGS      Invalid pipeline or element object
 */
int32_t add_element_pipeline(PipelineHandle pipeline_handle, ElementHandle element_handle, int8_t level);

/*!
 * @brief remove_element_pipeline
 * @details Function to remove element in a already created pipeline. This
 *          element should not be linked with any other pipeline before calling
 *          this function.
 *
 * @param pipeline_handle Pipeline handle
 * @param element_handle Element handle
 *
 * @returns Error Status
 *
 * @retval  STREAM_OK                   Element removed from pipeline
 * @retval  STREAM_ERR_INVALID_ARGS     Invalid pipeline or element object
 */
int32_t remove_element_pipeline(PipelineHandle pipeline_handle, ElementHandle element_handle);

/*!
 * @brief get_element_pipeline
 * @details Function to return tag of the current playing/paused stream.
 *
 * @param pipeline_handle Pipeline handle
 * @param type Element type
 * @param key Unique key used in creating the element
 * @param handle Pointer to the element handle
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                Element handle successfully retrieved
 * @retval STREAM_ERR_INVALID_ARGS  Invalid pipeline object or element not found
 */
int32_t get_element_pipeline(PipelineHandle pipeline_handle, StreamElementType type, int8_t key, ElementHandle *handle);

/*!
 * @brief send_msg_pipeline
 * @details The function sends the message to the callback function associated
 *          with the pipeline.
 *
 * @param pipeline Pipeline to send the message to
 * @param msg StreamMessage pointer
 *
 * @returns Error Status
 *
 * @retval STREAM_OK                    Message successfully sent
 * @retval STREAM_ERR_INVALID_ARGS      Invalid pipeline or message object
 */
int32_t send_msg_pipeline(Pipeline *pipeline, StreamMessage *msg);

/*!
 * @brief Set pipeline repeat mode
 * @details Set the repeat mode of a pipeline, if repeat mode is true,
 *          pipeline will play current stream repeatly when received a MSG_EOS
 *          event.
 * @param handle Pointer to a pipeline object
 * @param repeat  repeat true or false
 *
 * @return none
 */
int32_t set_repeat_pipeline(PipelineHandle handle, bool repeat);

/*!
 * @brief Get pipeline repeat mode
 * @param handle Pointer to a pipeline object
 *
 * @return repeat repeat true or false
 */
bool get_repeat_pipeline(PipelineHandle handle);

/*!
 * @brief Clears the pipeline track info cache
 * @details Clears all track info from the pipeline cache
 * @param handle Pointer to a pipeline object
 *
 * @return none
 */
void clear_pipeline_trackinfo(PipelineHandle handle);

/*----------------------------------------------------------------------------*/

#endif /* PIPELINE_H */
