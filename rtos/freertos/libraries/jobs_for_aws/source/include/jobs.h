/*
 * AWS IoT Jobs v1.0.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file jobs.h
 * @brief Client library APIs for the AWS IoT Jobs service.
 *
 * https://docs.aws.amazon.com/iot/latest/developerguide/jobs-api.html#jobs-mqtt-api
 */

#ifndef JOBS_H_
#define JOBS_H_

#include <stddef.h>
#include <stdint.h>

/**
 * @ingroup jobs_constants
 * @brief Maximum length of a thing name for the AWS IoT Jobs Service.
 */
#define JOBS_THINGNAME_MAX_LENGTH    128U      /* per AWS IoT API Reference */

/**
 * @ingroup jobs_constants
 * @brief Maximum length of a job ID for the AWS IoT Jobs Service.
 */
#define JOBS_JOBID_MAX_LENGTH        64U       /* per AWS IoT API Reference */

#ifndef THINGNAME_MAX_LENGTH

/**
 * @brief User defined maximum length of a thing name for the application.
 *
 * <br><b>Default value</b>: @ref JOBS_THINGNAME_MAX_LENGTH
 */
    #define THINGNAME_MAX_LENGTH    JOBS_THINGNAME_MAX_LENGTH
#endif

#ifndef JOBID_MAX_LENGTH

/**
 * @brief User defined maximum length of a job ID for the application.
 *
 * <br><b>Default value</b>: @ref JOBS_JOBID_MAX_LENGTH
 */
    #define JOBID_MAX_LENGTH    JOBS_JOBID_MAX_LENGTH
#endif

#if ( THINGNAME_MAX_LENGTH > JOBS_THINGNAME_MAX_LENGTH )
    #error "The value of THINGNAME_MAX_LENGTH exceeds the AWS IoT Jobs Service limit."
#endif

#if ( JOBID_MAX_LENGTH > JOBS_JOBID_MAX_LENGTH )
    #error "The value of JOBID_MAX_LENGTH exceeds the AWS IoT Jobs Service limit."
#endif

/** @cond DO_NOT_DOCUMENT */

#define JOBS_API_PREFIX                   "$aws/things/"
#define JOBS_API_PREFIX_LENGTH            ( sizeof( JOBS_API_PREFIX ) - 1U )

#define JOBS_API_BRIDGE                   "/jobs/"
#define JOBS_API_BRIDGE_LENGTH            ( sizeof( JOBS_API_BRIDGE ) - 1U )

#define JOBS_API_SUCCESS                  "/accepted"
#define JOBS_API_SUCCESS_LENGTH           ( sizeof( JOBS_API_SUCCESS ) - 1U )

#define JOBS_API_FAILURE                  "/rejected"
#define JOBS_API_FAILURE_LENGTH           ( sizeof( JOBS_API_FAILURE ) - 1U )

#define JOBS_API_JOBSCHANGED              "notify"
#define JOBS_API_JOBSCHANGED_LENGTH       ( sizeof( JOBS_API_JOBSCHANGED ) - 1U )

#define JOBS_API_NEXTJOBCHANGED           "notify-next"
#define JOBS_API_NEXTJOBCHANGED_LENGTH    ( sizeof( JOBS_API_NEXTJOBCHANGED ) - 1U )

#define JOBS_API_GETPENDING               "get"
#define JOBS_API_GETPENDING_LENGTH        ( sizeof( JOBS_API_GETPENDING ) - 1U )

#define JOBS_API_STARTNEXT                "start-next"
#define JOBS_API_STARTNEXT_LENGTH         ( sizeof( JOBS_API_STARTNEXT ) - 1U )

#define JOBS_API_DESCRIBE                 "get"
#define JOBS_API_DESCRIBE_LENGTH          ( sizeof( JOBS_API_DESCRIBE ) - 1U )

#define JOBS_API_UPDATE                   "update"
#define JOBS_API_UPDATE_LENGTH            ( sizeof( JOBS_API_UPDATE ) - 1U )

#define JOBS_API_COMMON_LENGTH( thingNameLength ) \
    ( JOBS_API_PREFIX_LENGTH + ( thingNameLength ) + JOBS_API_BRIDGE_LENGTH )

/** @endcond */

/**
 * @ingroup jobs_constants
 * @brief The size needed to hold the longest topic for a given thing name length.
 * @note This includes space for a terminating NUL character.
 */
#define JOBS_API_MAX_LENGTH( thingNameLength )                    \
    ( JOBS_API_COMMON_LENGTH( thingNameLength ) +                 \
      JOBID_MAX_LENGTH + sizeof( '/' ) + JOBS_API_UPDATE_LENGTH + \
      JOBS_API_SUCCESS_LENGTH + 1U )

/**
 * @ingroup jobs_enum_types
 * @brief Return codes from jobs functions.
 */
typedef enum
{
    JobsError = 0,
    JobsSuccess,       /**< @brief The buffer was properly written or a match was found. */
    JobsNoMatch,       /**< @brief The buffer does not contain a jobs topic. */
    JobsBadParameter,  /**< @brief A function parameter was NULL or has an illegal value. */
    JobsBufferTooSmall /**< @brief The buffer write was truncated. */
} JobsStatus_t;

/**
 * @ingroup jobs_enum_types
 * @brief Topic values for subscription requests.
 *
 * @note The enum values for valid topics must be contiguous,
 * starting with 0.  The last valid topic must be followed
 * by JobsMaxTopic.  This arrangement is necessary since the
 * enum values are used as indexes to arrays of topic strings
 * and lengths.
 *
 * @note The ordering is important, providing a means
 * to divide topics into those that use a job ID
 * and those that do not.
 *
 * @note These constraints are enforced by a unit test.
 */
typedef enum
{
    JobsInvalidTopic = -1,
    JobsJobsChanged,
    JobsNextJobChanged,
    JobsGetPendingSuccess,
    JobsGetPendingFailed,
    JobsStartNextSuccess,
    JobsStartNextFailed,
    /* Topics below use a job ID. */
    JobsDescribeSuccess,
    JobsDescribeFailed,
    JobsUpdateSuccess,
    JobsUpdateFailed,
    JobsMaxTopic
} JobsTopic_t;

/**
 * @brief Populate a topic string for a subscription request.
 *
 * @param[in] buffer  The buffer to contain the topic string.
 * @param[in] length  The size of the buffer.
 * @param[in] thingName  The device's thingName as registered with AWS IoT.
 * @param[in] thingNameLength  The length of the thingName.
 * @param[in] api  The desired Jobs API, e.g., JobsNextJobChanged.
 * @param[out] outLength  The length of the topic string written to the buffer.
 *
 * @return #JobsSuccess if the topic was written to the buffer;
 * #JobsBadParameter if invalid parameters are passed;
 * #JobsBufferTooSmall if the buffer cannot hold the full topic string.
 *
 * When all parameters are valid, the topic string is written to
 * the buffer up to one less than the buffer size.  The topic is
 * ended with a NUL character.
 *
 * @note The thingName parameter does not need a NULL terminator.
 */
/* @[declare_jobs_gettopic] */
JobsStatus_t Jobs_GetTopic( char * buffer,
                            size_t length,
                            const char * thingName,
                            uint16_t thingNameLength,
                            JobsTopic_t api,
                            size_t * outLength );
/* @[declare_jobs_gettopic] */

/**
 * @brief Output a topic value if a Jobs API topic string is present.
 * Optionally, output a pointer to a jobID within the topic and its
 * length.
 *
 * @param[in] topic  The topic string to check.
 * @param[in] length  The length of the topic string.
 * @param[in] thingName  The device's thingName as registered with AWS IoT.
 * @param[in] thingNameLength  The length of the thingName.
 * @param[out] outApi  The jobs topic API value if present, e.g., JobsUpdateSuccess.
 * @param[out] outJobId  The beginning of the jobID in the topic string.
 * @param[out] outJobIdLength  The length of the jobID in the topic string.
 *
 * @return #JobsSuccess if a matching topic was found;
 * #JobsNoMatch if a matching topic was NOT found
 *   (parameter outApi gets JobsInvalidTopic );
 * #JobsBadParameter if invalid parameters are passed.
 *
 * @note The topic and thingName parameters do not need a NULL terminator.
 *
 * @note Not all Jobs APIs have jobIDs within the topic string.
 * NULL and 0 are output when no jobID is present.
 * The parameters jobId and jobIdLength may be NULL.
 */
/* @[declare_jobs_matchtopic] */
JobsStatus_t Jobs_MatchTopic( char * topic,
                              size_t length,
                              const char * thingName,
                              uint16_t thingNameLength,
                              JobsTopic_t * outApi,
                              char ** outJobId,
                              uint16_t * outJobIdLength );
/* @[declare_jobs_matchtopic] */

/**
 * @brief Populate a topic string for a GetPendingJobExecutions request.
 *
 * @param[in] buffer  The buffer to contain the topic string.
 * @param[in] length  The size of the buffer.
 * @param[in] thingName  The device's thingName as registered with AWS IoT.
 * @param[in] thingNameLength  The length of the thingName.
 * @param[out] outLength  The length of the topic string written to the buffer.
 *
 * @return #JobsSuccess if the topic was written to the buffer;
 * #JobsBadParameter if invalid parameters are passed;
 * #JobsBufferTooSmall if the buffer cannot hold the full topic string.
 *
 * When all parameters are valid, the topic string is written to
 * the buffer up to one less than the buffer size.  The topic is
 * ended with a NUL character.
 *
 * @note The thingName parameter does not need a NULL terminator.
 */
/* @[declare_jobs_getpending] */
JobsStatus_t Jobs_GetPending( char * buffer,
                              size_t length,
                              const char * thingName,
                              uint16_t thingNameLength,
                              size_t * outLength );
/* @[declare_jobs_getpending] */

/**
 * @brief Populate a topic string for a StartNextPendingJobExecution request.
 *
 * @param[in] buffer  The buffer to contain the topic string.
 * @param[in] length  The size of the buffer.
 * @param[in] thingName  The device's thingName as registered with AWS IoT.
 * @param[in] thingNameLength  The length of the thingName.
 * @param[out] outLength  The length of the topic string written to the buffer.
 *
 * @return #JobsSuccess if the topic was written to the buffer;
 * #JobsBadParameter if invalid parameters are passed;
 * #JobsBufferTooSmall if the buffer cannot hold the full topic string.
 *
 * When all parameters are valid, the topic string is written to
 * the buffer up to one less than the buffer size.  The topic is
 * ended with a NUL character.
 *
 * @note The thingName parameter does not need a NULL terminator.
 */
/* @[declare_jobs_startnext] */
JobsStatus_t Jobs_StartNext( char * buffer,
                             size_t length,
                             const char * thingName,
                             uint16_t thingNameLength,
                             size_t * outLength );
/* @[declare_jobs_startnext] */

/**
 * @brief Populate a topic string for a DescribeJobExecution request.
 *
 * @param[in] buffer  The buffer to contain the topic string.
 * @param[in] length  The size of the buffer.
 * @param[in] thingName  The device's thingName as registered with AWS IoT.
 * @param[in] thingNameLength  The length of the thingName.
 * @param[out] jobId  The ID of the job to describe.
 * @param[out] jobIdLength  The length of the job ID.
 * @param[out] outLength  The length of the topic string written to the buffer.
 *
 * @return #JobsSuccess if the topic was written to the buffer;
 * #JobsBadParameter if invalid parameters are passed;
 * #JobsBufferTooSmall if the buffer cannot hold the full topic string.
 *
 * When all parameters are valid, the topic string is written to
 * the buffer up to one less than the buffer size.  The topic is
 * ended with a NUL character.
 *
 * @note The thingName and jobId parameters do not need a NULL terminator.
 */
/* @[declare_jobs_describe] */
JobsStatus_t Jobs_Describe( char * buffer,
                            size_t length,
                            const char * thingName,
                            uint16_t thingNameLength,
                            const char * jobId,
                            uint16_t jobIdLength,
                            size_t * outLength );
/* @[declare_jobs_describe] */

/**
 * @brief Populate a topic string for an UpdateJobExecution request.
 *
 * @param[in] buffer  The buffer to contain the topic string.
 * @param[in] length  The size of the buffer.
 * @param[in] thingName  The device's thingName as registered with AWS IoT.
 * @param[in] thingNameLength  The length of the thingName.
 * @param[out] jobId  The ID of the job to describe.
 * @param[out] jobIdLength  The length of the job ID.
 * @param[out] outLength  The length of the topic string written to the buffer.
 *
 * @return #JobsSuccess if the topic was written to the buffer;
 * #JobsBadParameter if invalid parameters are passed;
 * #JobsBufferTooSmall if the buffer cannot hold the full topic string.
 *
 * When all parameters are valid, the topic string is written to
 * the buffer up to one less than the buffer size.  The topic is
 * ended with a NUL character.
 *
 * @note The thingName and jobId parameters do not need a NULL terminator.
 */
/* @[declare_jobs_update] */
JobsStatus_t Jobs_Update( char * buffer,
                          size_t length,
                          const char * thingName,
                          uint16_t thingNameLength,
                          const char * jobId,
                          uint16_t jobIdLength,
                          size_t * outLength );
/* @[declare_jobs_update] */

#endif /* ifndef JOBS_H_ */
