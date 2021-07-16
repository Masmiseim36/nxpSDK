/**
 * Copyright 2018 by Au-Zone Technologies.  All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential.
 *
 * Authorization to use this header is provided through a DeepViewRT license.
 *
 * @file deepview_rt.h DeepViewRT core library header.
 *
 * DeepViewRT-M is distributed as a pair of header files and a platform
 * specific library file, plus documentation.  The library file comes in a
 * number of flavours for the various supported platforms.  All public DeepView
 * structures and functions use the NN and nn_ prefix to denote our namespace.
 */

#ifndef DEEPVIEW_RT_H
#define DEEPVIEW_RT_H

#ifndef DEEPVIEW_RT_NO_INCLUDES
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#endif /* DEEPVIEW_RT_NO_INCLUDES */

/*
 * Header API Macros from Hedly - https://nemequ.github.io/hedley
 * NN namespace has been updated to NN for DeepViewRT Namespace and
 * functionality stripped down to reduce size of headers.
 */
#define NN_VERSION_ENCODE(major, minor, revision) \
    (((major) *1000000) + ((minor) *1000) + (revision))
#define NN_VERSION_DECODE_MAJOR(version) ((version) / 1000000)
#define NN_VERSION_DECODE_MINOR(version) (((version) % 1000000) / 1000)
#define NN_VERSION_DECODE_REVISION(version) ((version) % 1000)

#if defined(__GNUC__) && defined(__GNUC_PATCHLEVEL__)
#define NN_GNUC_VERSION \
    NN_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined(__GNUC__)
#define NN_GNUC_VERSION NN_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
#endif

#if defined(NN_GNUC_VERSION)
#define NN_GNUC_VERSION_CHECK(major, minor, patch) \
    (NN_GNUC_VERSION >= NN_VERSION_ENCODE(major, minor, patch))
#else
#define NN_GNUC_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(__CC_ARM) && defined(__ARMCOMPILER_VERSION)
#define NN_ARM_VERSION                                           \
    NN_VERSION_ENCODE(__ARMCOMPILER_VERSION / 1000000,           \
                      (__ARMCOMPILER_VERSION % 1000000) / 10000, \
                      (__ARMCOMPILER_VERSION % 10000) / 100)
#elif defined(__CC_ARM) && defined(__ARMCC_VERSION)
#define NN_ARM_VERSION                                     \
    NN_VERSION_ENCODE(__ARMCC_VERSION / 1000000,           \
                      (__ARMCC_VERSION % 1000000) / 10000, \
                      (__ARMCC_VERSION % 10000) / 100)
#endif

#if defined(NN_ARM_VERSION)
#define NN_ARM_VERSION_CHECK(major, minor, patch) \
    (NN_ARM_VERSION >= NN_VERSION_ENCODE(major, minor, patch))
#else
#define NN_ARM_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(__IAR_SYSTEMS_ICC__)
#if __VER__ > 1000
#define NN_IAR_VERSION                           \
    NN_VERSION_ENCODE((__VER__ / 1000000),       \
                      ((__VER__ / 1000) % 1000), \
                      (__VER__ % 1000))
#else
#define NN_IAR_VERSION NN_VERSION_ENCODE(VER / 100, __VER__ % 100, 0)
#endif
#endif

#if defined(NN_IAR_VERSION)
#define NN_IAR_VERSION_CHECK(major, minor, patch) \
    (NN_IAR_VERSION >= NN_VERSION_ENCODE(major, minor, patch))
#else
#define NN_IAR_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(NN_GNUC_VERSION) && !defined(__clang) && !defined(NN_ARM_VERSION)
#define NN_GCC_VERSION NN_GNUC_VERSION
#endif

#if defined(NN_GCC_VERSION)
#define NN_GCC_VERSION_CHECK(major, minor, patch) \
    (NN_GCC_VERSION >= NN_VERSION_ENCODE(major, minor, patch))
#else
#define NN_GCC_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(__cplusplus) && (__cplusplus >= 201402L)
#define NN_DEPRECATED(since) [[deprecated("Since " #since)]]
#define NN_DEPRECATED_FOR(since, replacement) \
    [[deprecated("Since " #since "; use " #replacement)]]
#elif _MSC_VER >= 1400
#define NN_DEPRECATED(since) __declspec(deprecated("Since " #since))
#define NN_DEPRECATED_FOR(since, replacement) \
    __declspec(deprecated("Since " #since "; use " #replacement))
#elif _MSC_VER >= 1310
#define NN_DEPRECATED(since) _declspec(deprecated)
#define NN_DEPRECATED_FOR(since, replacement) __declspec(deprecated)
#elif NN_IAR_VERSION_CHECK(8, 0, 0)
#define NN_DEPRECATED(since) _Pragma("deprecated")
#define NN_DEPRECATED_FOR(since, replacement) _Pragma("deprecated")
#elif defined(_GHS_MULTI)
#define NN_DEPRECATED(since)
#define NN_DEPRECATED_FOR(since, replacement)
#else
#define NN_DEPRECATED(since) __attribute__((__deprecated__("Since " #since)))
#define NN_DEPRECATED_FOR(since, replacement) \
    __attribute__((__deprecated__("Since " #since "; use " #replacement)))
#endif

#if NN_GCC_VERSION_CHECK(4, 3, 0)
#define NN_UNAVAILABLE(available_since) \
    __attribute__((__warning__("Not available until " #available_since)))
#else
#define NN_UNAVAILABLE(available_since)
#endif

#if defined(__cplusplus) && (__cplusplus >= 201703L)
#define NN_WARN_UNUSED_RESULT [[nodiscard]]
#elif defined(_Check_return_) /* SAL */
#define NN_WARN_UNUSED_RESULT _Check_return_
#elif !defined(__IAR_SYSTEMS_ICC__)
#define NN_WARN_UNUSED_RESULT __attribute__((__warn_unused_result__))
#else
#define NN_WARN_UNUSED_RESULT
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
#define NN_PRIVATE
#define NN_PUBLIC __declspec(dllexport)
#define NN_IMPORT __declspec(dllimport)
#else
#define NN_PRIVATE __attribute__((__visibility__("hidden")))
#define NN_PUBLIC __attribute__((__visibility__("default")))
#define NN_IMPORT extern
#endif

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && \
    !defined(__STDC_NO_VLA__) && !defined(__cplusplus) &&         \
    !defined(NN_PGI_VERSION) && !defined(NN_TINYC_VERSION)
#define NN_ARRAY_PARAM(name) static(name)
#else
#define NN_ARRAY_PARAM(name)
#endif

#if !defined(__cplusplus) &&                                         \
    ((defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)) || \
     defined(_Static_assert))
#define NN_STATIC_ASSERT(expr, message) _Static_assert(expr, message)
#elif (defined(__cplusplus) && (__cplusplus >= 201703L)) || (_MSC_VER >= 1600)
#define NN_STATIC_ASSERT(expr, message) static_assert(expr, message)
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
#define NN_STATIC_ASSERT(expr, message) static_assert(expr)
#else
#define NN_STATIC_ASSERT(expr, message)
#endif

#ifdef NN_API_STATIC
#define NN_API
#else
#ifdef NN_API_EXPORT
#define NN_API NN_PUBLIC
#else
#define NN_API NN_IMPORT
#endif
#endif

#define NN_VERSION_2_0 NN_VERSION_ENCODE(2, 0, 0)
#define NN_VERSION_2_1 NN_VERSION_ENCODE(2, 1, 0)
#define NN_VERSION_2_2 NN_VERSION_ENCODE(2, 2, 0)
#define NN_VERSION_2_3 NN_VERSION_ENCODE(2, 3, 0)
#define NN_VERSION_2_4 NN_VERSION_ENCODE(2, 4, 0)

#ifndef NN_TARGET_VERSION
#define NN_TARGET_VERSION NN_VERSION_2_4
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 0, 0)
#define NN_AVAILABLE_SINCE_2_0 NN_UNAVAILABLE(2.0)
#define NN_DEPRECATED_SINCE_2_0
#define NN_DEPRECATED_SINCE_2_0_FOR(replacement)
#else
#define NN_AVAILABLE_SINCE_2_0
#define NN_DEPRECATED_SINCE_2_0 NN_DEPRECATED(2.0)
#define NN_DEPRECATED_SINCE_2_0_FOR(replacement) \
    NN_DEPRECATED_FOR(2.0, replacement)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 1, 0)
#define NN_AVAILABLE_SINCE_2_1 NN_UNAVAILABLE(2.1)
#define NN_DEPRECATED_SINCE_2_1
#define NN_DEPRECATED_SINCE_2_1_FOR(replacement)
#else
#define NN_AVAILABLE_SINCE_2_1
#define NN_DEPRECATED_SINCE_2_1 NN_DEPRECATED(2.1)
#define NN_DEPRECATED_SINCE_2_1_FOR(replacement) \
    NN_DEPRECATED_FOR(2.1, replacement)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 2, 0)
#define NN_AVAILABLE_SINCE_2_2 NN_UNAVAILABLE(2.2)
#define NN_DEPRECATED_SINCE_2_2
#define NN_DEPRECATED_SINCE_2_2_FOR(replacement)
#else
#define NN_AVAILABLE_SINCE_2_2
#define NN_DEPRECATED_SINCE_2_2 NN_DEPRECATED(2.2)
#define NN_DEPRECATED_SINCE_2_2_FOR(replacement) \
    NN_DEPRECATED_FOR(2.2, replacement)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 3, 0)
#define NN_AVAILABLE_SINCE_2_3 NN_UNAVAILABLE(2.3)
#define NN_DEPRECATED_SINCE_2_3
#define NN_DEPRECATED_SINCE_2_3_FOR(replacement)
#else
#define NN_AVAILABLE_SINCE_2_3
#define NN_DEPRECATED_SINCE_2_3 NN_DEPRECATED(2.3)
#define NN_DEPRECATED_SINCE_2_3_FOR(replacement) \
    NN_DEPRECATED_FOR(2.3, replacement)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 4, 0)
#define NN_AVAILABLE_SINCE_2_4 NN_UNAVAILABLE(2.4)
#define NN_DEPRECATED_SINCE_2_4
#define NN_DEPRECATED_SINCE_2_4_FOR(replacement)
#else
#define NN_AVAILABLE_SINCE_2_4
#define NN_DEPRECATED_SINCE_2_4 NN_DEPRECATED(2.4)
#define NN_DEPRECATED_SINCE_2_4_FOR(replacement) \
    NN_DEPRECATED_FOR(2.4, replacement)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define NN_IMAGE_PROC_UNSIGNED_NORM 0x0001
#define NN_IMAGE_PROC_WHITENING 0x0002
#define NN_IMAGE_PROC_SIGNED_NORM 0x0004
#define NN_IMAGE_PROC_MIRROR 0x1000
#define NN_IMAGE_PROC_FLIP 0x2000

/**
 * Enumeration of all errors provided by DeepViewRT.
 */
typedef enum {
    NN_SUCCESS = 0,
    NN_ERROR_INTERNAL,
    NN_ERROR_INVALID_HANDLE,
    NN_ERROR_OUT_OF_MEMORY,
    NN_ERROR_OUT_OF_RESOURCES,
    NN_ERROR_NOT_IMPLEMENTED,
    NN_ERROR_INVALID_PARAMETER,
    NN_ERROR_TYPE_MISMATCH,
    NN_ERROR_SHAPE_MISMATCH,
    NN_ERROR_INVALID_SHAPE,
    NN_ERROR_INVALID_ORDER,
    NN_ERROR_INVALID_AXIS,
    NN_ERROR_MISSING_RESOURCE,
    NN_ERROR_INVALID_ENGINE,
    NN_ERROR_TENSOR_NO_DATA,
    NN_ERROR_KERNEL_MISSING,
    NN_ERROR_TENSOR_TYPE_UNSUPPORTED,
    NN_ERROR_TOO_MANY_INPUTS,
    NN_ERROR_SYSTEM_ERROR,
    NN_ERROR_INVALID_LAYER,
    NN_ERROR_MODEL_INVALID,
    NN_ERROR_MODEL_MISSING,
    NN_ERROR_STRING_TOO_LARGE,
    NN_ERROR_INVALID_QUANT,
} NNError;


/**
 * Enumeration of all quantization type provided by DeepViewRT.
 */
typedef enum {
  NN_Quantization_None = 0,
  NN_Quantization_Affine_PerTensor = 1,
  NN_Quantization_Affine_PerChannel = 2,
  NN_Quantization_DFP =3,
  NN_Quantization_Unknown
} NNQuantizationType;


/**
 * This option tells nn_init to load a license file from the path provided in
 * the next option element which MUST be provided.
 *
 * Example:
 * NNOptions options[] = { NN_OPTIONS_LICENSE_PATH, "/etc/deepview.lic", NULL };
 * NNError err = nn_init(options);
 * if (err) {
 *     printf("nn_init failed: %s\n", nn_strerror(err));
 * }
 */
#define NN_OPTIONS_LICENSE_PATH 1

/**
 * This option tells nn_init to load a license blob from the pointer provided in
 * the next option element which MUST be provided.  When using this option then
 * the NN_OPTIONS_LICENSE_SIZE MUST also be provided which holds the size of the
 * blob data.
 */
#define NN_OPTIONS_LICENSE_BLOB 2

/**
 * This option is used along with NN_OPTIONS_LICENSE_BLOB to define the size of
 * the provided blob data.
 */
#define NN_OPTIONS_LICENSE_SIZE 3

/**
 * This option forces SSE2 to be enabled, skipping probing of instructions.
 */
#define NN_OPTIONS_ENABLE_SSE2 10

/**
 * This option forces SSE2 to be disabled, skipping the probing of instructions.
 */
#define NN_OPTIONS_DISABLE_SSE2 11

/**
 * This option forces AVX2 to be enabled, skipping probing of instructions.
 */
#define NN_OPTIONS_ENABLE_AVX2 20

/**
 * This option forces AVX2 to be disabled, skipping the probing of instructions.
 */
#define NN_OPTIONS_DISABLE_AVX2 21

/**
 * This option forces FMA to be enabled, skipping probing of instructions.
 */
#define NN_OPTIONS_ENABLE_FMA 22

/**
 * This option forces FMA to be disabled, skipping the probing of instructions.
 */
#define NN_OPTIONS_DISABLE_FMA 23

/**
 * This option forces NEON-VFP3 to be enabled, skipping probing of instructions.
 */
#define NN_OPTIONS_ENABLE_VFP3 30

/**
 * This option forces NEON-VFP3 to be disabled, skipping the probing of
 * instructions.
 */
#define NN_OPTIONS_DISABLE_VFP3 31

/**
 * This option forces NEON-VFP4 to be enabled, skipping probing of instructions.
 */
#define NN_OPTIONS_ENABLE_VFP4 40

/**
 * This option forces NEON-VFP4 to be disabled, skipping the probing of
 * instructions.
 */
#define NN_OPTIONS_DISABLE_VFP4 41

typedef intptr_t NNOptions;

/**
 * DeepViewRT-M library version.
 *
 * @return library version string
 *
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_version();

/**
 * Returns the string associated with a given error.
 *
 * @see NNError
 *
 * @param error The NNError to be represented as a string.
 * @return The string representation when the error is valid.
 * @return NULL when the error is not valid.
 *
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_strerror(NNError error);

/**
 * Initializes the library with optional parameters.  This function _MUST_ be
 * called before any others (though nn_version and nn_strerror are safe) and
 * _MUST_ not be called again unless care is taken to protect this call.
 *
 */
NN_API
NN_WARN_UNUSED_RESULT
NNError
nn_init(const NNOptions* options);

/**
 * Size of the NNEngine structure including reserved space and padding.
 */
#define NN_ENGINE_SIZEOF 1024

/**
 * @struct NNEngine
 *
 * Engine structure provides the means to implement custom tensor and kernel
 * implementations which implement the DeepViewRT inference backend. As an
 * example the OpenCL backend is provided as a plugin which exposes an NNEngine
 * which maps NNTensors to cl_mem objects and kernels as OpenCL kernels.
 */
typedef struct nn_engine NNEngine;

/**
 * Returns the size of the engine object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_engine_sizeof();

/**
 * Initializes the NNEngine structure using the provided memory or allocating a
 * new buffer is none was provided.
 *
 * @param memory Pointer to the start of where a NNEngine object should be
 * initialized.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNEngine*
nn_engine_init(void* memory);

/**
 * Releases the memory that was being used by the engine.
 *
 * @param engine Pointer to the engine object.
 * @return NN_ERROR_INVALID_ENGINE given the engine pointer is NULL.
 * @return NN_SUCCESS given that the engine pointer is not NULL.
 *
 * @public @memberof NNEngine
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_engine_release(NNEngine* engine);

/**
 * Loads the given plugin to provided engine object.
 *
 * @param engine Pointer to the engine object.
 * @param plugin String of the absolute or relative path to the plugin.
 * @return NN_ERROR_INVALID ENGINE given the engine pointer is NULL or
 * the plugin does not have the necessary functions.
 * @return NN_ERROR_MISSING_RESOURCE given the plugin dll cannot be found.
 * @return The error returned by the plugin's init function given a valid engine
 * and dll.
 *
 * @public @memberof NNEngine
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_engine_load(NNEngine* engine, const char* plugin);

/**
 * Unloads the plugin from the given engine object.
 *
 * @param engine Pointer to the engine object.
 * @return NN_ERROR_INVALID_ENGINE given the engine pointer is NULL.
 * @return NN_ERROR_INTERNAL if the plugin dll could not be closed properly.
 * @return The NNError from the plugin's cleanup function.
 *
 * @public @memberof NNEngine
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_engine_unload(NNEngine* engine);

/**
 * Returns the name of the engine object.
 *
 * @param engine Pointer to the engine object.
 *
 * @public @memberof NNEngine
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_engine_name(NNEngine* engine);

/**
 * Returns the version of the engine object.
 *
 * @param engine Pointer to the engine object.
 *
 * @public @memberof NNEngine
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_engine_version(NNEngine* engine);

/**
 * Size of the NNTensor structure including reserved space and padding.
 */
#define NN_TENSOR_SIZEOF 160

/**
 * @enum NNTensorType
 * Enumeration of the data types supported by NNTensors in DeepViewRT.
 */
typedef enum {
    /**
     * Raw byte-stream tensor, useful for encoded tensors such as PNG images.
     */
    NNTensorType_RAW = 0,
    /**
     * String tensor data, a single dimension would hold one null-terminated
     * string of variable length.
     */
    NNTensorType_STR = 1,
    /**
     * Unsigned 8-bit integer tensor data.
     */
    NNTensorType_I8  = 2,
    NNTensorType_U8  = 3,
    NNTensorType_I16 = 4,
    NNTensorType_U16 = 5,
    NNTensorType_I32 = 6,
    NNTensorType_U32 = 7,
    NNTensorType_I64 = 8,
    NNTensorType_U64 = 9,
    /**
     * Half precision (16-bit) floating point tensor data.
     */
    NNTensorType_F16 = 10,
    /**
     * Single precision (32-bit) floating point tensor data.
     */
    NNTensorType_F32 = 11,
    /**
     * Double precision (64-bit) floating point tensor data.
     */
    NNTensorType_F64 = 12
} NNTensorType;

/**
 * @struct NNTensor
 *
 * Tensor structure.
 */
typedef struct nn_tensor NNTensor;


typedef struct nn_quant_param {
    /**
     * Length of the zeros array.
     */
    size_t n_zeros;
    /**
     * Quantization zero-points.
     */
    const int32_t* zeros;
    /**
     * Length of the scales array.
     */
    size_t n_scales;
    /**
     * Quantization scales.
     */
    const float* scales;

	size_t element_size;

	NNTensorType buffer_type;

}NNQuantParam_t;


/**
 * Returns the size of the tensor object for preparing memory allocations.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_tensor_sizeof();

/**
 * Initializes the tensor using provided memory.  The memory MUST be at least
 * the size returned by @ref nn_tensor_sizeof().  This size does not include
 * the actual tensor data which is allocated separately, either by requesting
 * the implementation to allocate the buffer or attaching to externally
 * allocated memory.
 *
 * The tensor created by this function has no data associated to it and is of
 * rank-0.
 *
 * @param memory The pointer to be initialized to a NNTensor object.
 * @param engine Pointer to the engine object.
 * @return NULL given the memory pointer is a null pointer.
 * @return Pointer to the newly created NNTensor object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNTensor*
nn_tensor_init(void* memory, NNEngine* engine);

/**
 * Releases the memory used by the tensor object.
 *
 * @param tensor Pointer to the tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_tensor_release(NNTensor* tensor);

/**
 * Returns the engine owning this tensor, could be NULL.
 *
 * @param tensor Pointer to the tensor object.
 * @return Pointer to the engine object to which the tensor is associated.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNEngine*
nn_tensor_engine(NNTensor* tensor);

/**
 * Returns the handle of the tensor object.
 *
 * @param tensor Pointer to the tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void*
nn_tensor_native_handle(NNTensor* tensor);

/**
 * Sets the tensor objects native handle to the one provided.
 *
 * @param tensor Pointer to the tensor object.
 * @param handle Pointer to the handle object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_tensor_set_native_handle(NNTensor* tensor, void* handle);

/**
 * Callback function to free an auxiliary object, called from nn_tensor_release.
 *
 * @since 2.1
 */
typedef void(nn_aux_object_free)(NNTensor* tensor);

/**
 * Configures an auxiliary object for the tensor.  This is a private API used
 * for attaching auxiliary buffers, for example for supporting codec engines.
 *
 * @private @memberof NNTensor
 * @since 2.1
 */
NN_AVAILABLE_SINCE_2_1
NN_API
void
nn_tensor_set_aux_object(NNTensor*           tensor,
                         void*               aux_object,
                         nn_aux_object_free* aux_object_free);


/**
 * Returns the auxiliary object for the tensor, or NULL if none is attached.
 *
 * @private @memberof NNTensor
 * @since 2.1
 */
NN_AVAILABLE_SINCE_2_1
NN_API
void*
nn_tensor_aux_object(NNTensor* tensor);

/**
 * Returns the auxiliary object's free function, or NULL if none is attached.
 *
 * @private @memberof NNTensor
 * @since 2.3
 */
NN_AVAILABLE_SINCE_2_3
NN_API
nn_aux_object_free*
nn_tensor_aux_free(NNTensor* tensor);



void
nn_tensor_set_aux_object_by_name(NNTensor* tensor,
                                 const char* name,
                                 void* aux_object,
                                 nn_aux_object_free* aux_object_free,
                                 bool buffer_ownership,
                                 bool name_ownership);


void*
nn_tensor_aux_object_by_name(NNTensor* tensor,
                             const char* name);


nn_aux_object_free*
nn_tensor_aux_free_by_name(NNTensor* tensor,
                           const char* name);


/**
 * Private API - read panel size of the tensor.
 */
NN_API
int
nn_tensor_panel_size(NNTensor* tensor);

/**
 * Private API - set the panel size of the tensor.
 */
NN_API
void
nn_tensor_set_panel_size(NNTensor* tensor, int panel_size);

/**
 * Synchronize the tensor and all preceeding events in the chain.
 *
 * This is used for engines which may not immediately evaluate tensor operations
 * but instead pass events around, this call will synchronize the event chain
 * leading to this tensor.
 *
 * @param tensor Pointer to the tensor object.
 * @return NN_SUCCESS if the engine does not support a tensor_sync function.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_sync(NNTensor* tensor);

/**
 * Returns the time information stored in the tensor.  The time is returned
 * in nanoseconds of the duration of the last operation the wrote into this
 * tensor.  causes a nn_tensor_sync on the target tensor.
 *
 * @param tensor Pointer to the tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int64_t
nn_tensor_time(NNTensor* tensor);

/**
 * Returns the I/O time information stored in the tensor.  The time is returned
 * in nanoseconds of the duration of the last map/unmap pair.
 *
 * @param tensor Pointer to the tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.1
 */
NN_AVAILABLE_SINCE_2_1
NN_API
int64_t
nn_tensor_io_time(NNTensor* tensor);

/**
 * Writes the given tensor object to the FILE stream provided.
 *
 * @param out Pointer to the FILE stream where the tensor will be written to.
 * @param tensor Pointer to the tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_tensor_printf(NNTensor* tensor, bool data, FILE* out);

/**
 * Assigns the tensor parameters and optionally data pointer.  The default
 * implementation uses the data buffer as the internal storage for tensor data
 * and it MUST outlive the tensor.  For engine plugins they may choose how to
 * use the data but for the OpenCL example if data is provided it will be copied
 * into the OpenCL buffer then otherwise never used again.  If NULL is provided
 * for data the OpenCL engine would create the memory and leave it unassigned.
 *
 * If using the default implementation and leaving data NULL then all operations
 * which require data will fail.  The most dynamic tensor setup with optional
 * data would be to call assign to setup the parameters with NULL data, then
 * calling @ref nn_tensor_native_handle to see if one was created, if not the
 * data buffer can be malloc'ed followed by a call to @ref
 * nn_tensor_set_native_handle with this buffer.  One could also call
 * nn_tensor_assign a second time with data set to the malloc'ed data.
 *
 * @param tensor Pointer to the given tensor object.
 * @param type The data type that the tensor is storing (The type of the
 * provided data).
 * @param n_dims The number of dimensions in the provided tensor.
 * @param shape The shape of the given tensor.
 * @param data The new tensor data to be placed within the tensor provided.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_assign(NNTensor*     tensor,
                 NNTensorType  type,
                 int32_t       n_dims,
                 const int32_t shape[NN_ARRAY_PARAM(n_dims)],
                 void*         data);

/**
 * Maps the tensor using the memory from the parent tensor.
 *
 * @param tensor Pointer to the tensor object where the view will be stored.
 * @param type The data type that the tensor is storing.
 * @param n_dims The number of dimensions in the provided tensor.
 * @param shape The shape of the given tensor.
 * @param parent Pointer to the tensor object that holds the original tensor.
 * @param offset TO BE DETERMINED.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_view(NNTensor*     tensor,
               NNTensorType  type,
               int32_t       n_dims,
               const int32_t shape[NN_ARRAY_PARAM(n_dims)],
               NNTensor*     parent,
               int32_t       offset);

/**
 * Allocates the internal memory for the tensor.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_alloc(NNTensor*     tensor,
                NNTensorType  type,
                int32_t       n_dims,
                const int32_t shape[NN_ARRAY_PARAM(n_dims)]);

/**
 * Returns the shape of the given tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const int32_t*
nn_tensor_shape(const NNTensor* tensor);

/**
 * Returns the strides of the given tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const int32_t*
nn_tensor_strides(const NNTensor* tensor);

/**
 * Returns the number of dimensions of the given tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int32_t
nn_tensor_dims(const NNTensor* tensor);

/**
 * Maps the tensor's memory and returns the client accessible pointer.  This is
 * the read-only version which causes the engine to download buffers to the CPU
 * memory space if required but will not flush back to the device on unmap.
 *
 * If the tensor is already mapped read-only or read-write a pointer is returned
 * and the reference count increased, if it was already mapped write-only NULL
 * is returned.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const void*
nn_tensor_mapro(NNTensor* tensor);

/**
 * Maps the tensor's memory and returns the client accessible pointer.  This is
 * the read-write version which causes the engine to download buffers to the CPU
 * memory space if required and will also flush back to the device on unmap.
 *
 * If the tensor is already mapped read-only it needs to be unmapped before
 * calling maprw otherwise NULL is returned.  A tensor already mapped as rw will
 * simply increase the reference count.  A write-only mapped tensor will also
 * return NULL.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void*
nn_tensor_maprw(NNTensor* tensor);

/**
 * Maps the tensor's memory and returns the client accessible pointer.  This is
 * the write-only version which will not cause a download of the buffers to the
 * CPU memory space on map but will upload to the device on unmap.
 *
 * If the tensor is already mapped write-only or read-write a pointer is
 * returned and the reference count increased.  If it was previously mapped as
 * read-only NULL is returned.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void*
nn_tensor_mapwo(NNTensor* tensor);

/**
 * Returns the tensor's mapping count, 0 means the tensor is unmapped.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int
nn_tensor_mapped(const NNTensor* tensor);

/**
 * Releases the tensor mapping, if the reference count reaches 0 it will be
 * fully unmapped and will force the flush to the device, if required.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_tensor_unmap(NNTensor* tensor);

/**
 * Returns the type of a given tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNTensorType
nn_tensor_type(const NNTensor* tensor);

/**
 * Returns the element size of a given tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_tensor_element_size(const NNTensor* tensor);

/**
 * Calculates the total tensor volume (product of dimensions).
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int32_t
nn_tensor_volume(const NNTensor* tensor);

/**
 * Calculates the total byte size of the tensor (volume * element_size).
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int32_t
nn_tensor_size(const NNTensor* tensor);

/**
 * Returns the natural data axis of the tensor.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
char
nn_tensor_axis(const NNTensor* tensor);

/**
 * Returns the zero-points for the tensor and optionally the number of
 * zero-points.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const int32_t*
nn_tensor_zeros(const NNTensor* tensor, size_t* n_zeros);

/**
 * Sets the quantization zero-points for the tensor.  If n_zeros>1 it should
 * match the channel dimension (axis) of the tensor.
 *
 * If own=1 then the tensor will take ownership of the buffer and free it when
 * the tensor is released.  Otherwise the buffer must outlive the tensor.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
void
nn_tensor_set_zeros(NNTensor*      tensor,
                    size_t         n_zeros,
                    const int32_t* zeros,
                    int            own);

NN_AVAILABLE_SINCE_2_4
NN_API
void
nn_tensor_set_axis(NNTensor*      tensor,
				   int32_t         axis);

/**
 * Returns the scales array for the tensor and optionally the number of scales.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const float*
nn_tensor_scales(const NNTensor* tensor, size_t* n_scales);


NN_AVAILABLE_SINCE_2_4
NN_API
void
nn_tensor_quant_params(const NNTensor* tensor, NNQuantParam_t *quant_params);

/**
 * Sets the quantization scales for the tensor.  If n_scales>1 it should match
 * the channel dimension (axis) of the tensor.
 *
 * If own=1 then the tensor will take ownership of the buffer and free it when
 * the tensor is released.  Otherwise the buffer must outlive the tensor.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
void
nn_tensor_set_scales(NNTensor*    tensor,
                     size_t       n_scales,
                     const float* scales,
                     int          own);

/**
 * Tensor shape comparison.
 */
NN_AVAILABLE_SINCE_2_0
NN_DEPRECATED_SINCE_2_3
NN_API
bool
nn_tensor_shape_equal(const int32_t left[4], const int32_t right[4]);

/**
 * Copys the source shape array to the destination array.
 */
NN_AVAILABLE_SINCE_2_0
NN_DEPRECATED_SINCE_2_3
NN_API
void
nn_tensor_shape_copy(int32_t dst[4], const int32_t src[4]);

/**
 * Returns the offset of a given tensor.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int
nn_tensor_offset(const NNTensor* tensor,
                 int32_t         n_dims,
                 const int32_t   shape[NN_ARRAY_PARAM(n_dims)]);

/**
 * Returns the offset of a given tensor using variable length dimensions.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int
nn_tensor_offsetv(const NNTensor* tensor, int32_t n_dims, ...);

/**
 * Element-wise comparison of two tensors within a given tolerance, returning
 * total number of errors relative to the left tensor.  If the two tensors are
 * incompatible the volume of the left tensor is returned (all elements
 * invalid).
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_DEPRECATED_SINCE_2_3
NN_API
int
nn_tensor_compare(NNTensor* left, NNTensor* right, double tolerance);

/**
 * Reshapes the given tensor to the provided new shape.
 *
 * @tensor pointer to the tensor object
 * @shape pointer to the new shape for the tensor
 * @n_dims the number of dimensions in the new shape
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_reshape(NNTensor*     tensor,
                  int32_t       n_dims,
                  const int32_t shape[NN_ARRAY_PARAM(n_dims)]);

/**
 * Shuffles (transpose) the tensor moving the current dimensions into the
 * ordering defined in the order parameter.
 *
 * For example a traditional matrix transpose is done using order[] = { 1, 0 }
 * in other words, the 0 dimension of the output references the 1 dimension of
 * the input and the 1 dimension of the output references the 0 dimension of the
 * input.
 *
 * Another example would be shuffling an NCHW tensor to NHWC using order[] = {
 * 0, 2, 3, 1 }
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_shuffle(NNTensor*     output,
                  NNTensor*     input,
                  int32_t       n_dims,
                  const int32_t order[NN_ARRAY_PARAM(n_dims)]);

/**
 * Fills the tensor with the provided constant.  The constant is captured
 * as double precision (64-bit floating point) which has 53-bits of precision
 * on whole numbers.  This means the constant CANNOT represent all 64-bit
 * integers but it CAN represent all 32-bit and lower integers.  If full
 * 64-bit integer support is required @ref nn_tensor_map can be used though
 * it is less efficient with some engines because of the addition memory
 * transfer required.
 *
 * The double will be cast appropriately to the target tensor's type before
 * filling the tensor.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_fill(NNTensor* tensor, double constant);

/**
 * Randomizes the data within the tensor.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_DEPRECATED_SINCE_2_3
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_randomize(NNTensor* tensor);

/**
 * Copies the contents of source tensor into destination tensor.
 *
 * This operation only copies the data and does not affect the
 * destination tensor's properties.  The destination tensor must
 * have an equal or larger volume.  If required data will be converted.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_copy(NNTensor* dest, NNTensor* source);


/**
 * Requantizes the source tensor into the destination tensor.
 *
 * The source tensor and destination tensor should be either I8 or U8, and
 * per tensor quantized.
 *
 */

NN_API
NNError
nn_tensor_requantize(NNTensor* dest, NNTensor* source);

/**
 * Quantizes the source tensor into the destination tensor.
 *
 * The source tensor should be float and the destination integer.  If the
 * destination tensor does not have quantization parameters they will be
 * calculated from the source tensor and stored into the destination tensor.
 *
 * When calculating the quantization parameters if axis is a valid axis* then
 * per-channel quantization will be performed along the axis, otherwise
 * per-tensor quantization will be performed.  If the destination tensor has
 * quantization parameters axis is ignored.
 *
 * Valid Axis: (axis > 0 && axis < n_dims)
 */
NN_AVAILABLE_SINCE_2_4
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_quantize(NNTensor* dest, NNTensor* source, int axis);

/**
 * Quantizes the source buffer into the destination tensor.
 *
 * The source tensor should be float and the destination integer.  If the
 * destination tensor does not have quantization parameters they will be
 * calculated from the source buffer and stored into the destination tensor.
 *
 * When calculating the quantization parameters if axis is a valid axis* then
 * per-channel quantization will be performed along the axis, otherwise
 * per-tensor quantization will be performed. If the destination tensor has
 * quantization parameters axis is ignored.
 *
 * Valid Axis: (axis > 0 && axis < n_dims)
 */
NN_AVAILABLE_SINCE_2_4
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_quantize_buffer(NNTensor*    dest,
                          size_t       buffer_length,
                          const float* buffer,
                          int          axis);

/**
 * De-quantizes the source tensor into the destination tensor.
 *
 * The source tensor should be integer and the destination float.  The source
 * tensor must have quantization parameters otherwise the operation will simply
 * cast the integer data to float.
 */
NN_AVAILABLE_SINCE_2_4
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_dequantize(NNTensor* dest, NNTensor* source);

/**
 * De-quantizes the source tensor into the destination buffer.
 *
 * The source tensor should be integer and the destination float.  The source
 * tensor must have quantization parameters otherwise the operation will simply
 * cast the integer data to float.
 *
 * The buffer must be at least buffer_length*sizeof(float) size in bytes.
 */
NN_AVAILABLE_SINCE_2_4
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_dequantize_buffer(NNTensor* source,
                            size_t    buffer_length,
                            float*    buffer);

/**
 * nn_tensor_concat concatenates all of the given input tensors into
 * the given output tensor.
 *
 * @output pointer to the output tensor
 * @inputs list of pointers to the input tensors
 * @n_inputs the number of inputs
 * @axis the axis along which to concatenate the inputs
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_concat(NNTensor* output,
                 int32_t   n_inputs,
                 NNTensor* inputs[NN_ARRAY_PARAM(n_inputs)],
                 int32_t   axis);

/**
 * nn_tensor_slice copies a slice of the tensor into output.
 *
 * The axes, head, and tail must be of length n_axes or NULL.  Calling slice
 * with axes==NULL will ignore head/tail and is effectively @ref nn_tensor_copy.
 *
 * When head is NULL all axes are assumed to start at 0.  When tail is NULL all
 * axes are assumed to end at (len(axis) - head) for the given axis.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_slice(NNTensor*     output,
                NNTensor*     input,
                int32_t       n_axes,
                const int32_t axes[NN_ARRAY_PARAM(n_axes)],
                const int32_t head[NN_ARRAY_PARAM(n_axes)],
                const int32_t tail[NN_ARRAY_PARAM(n_axes)]);

/**
 * nn_tensor_padding calculates the paddings for the given tensor, padtype,
 * window, stride, and dilation given n_dims being queried from the tensor's
 * nn_tensor_dims().
 *
 * The paddings pointer must point to an array of 2 * n_dims elements into which
 * the function will write the head/tail padding tuples for each of the n_dims
 * provided dimensions.  The padded_shape parameter must point to an array of
 * n_dims elemens which will receive the output (padded) shape.
 *
 * The padtype can be "VALID" or "SAME".  When padtype is "SAME" padded_shape
 * will equal the shape of the input tensor and the paddings will be provided to
 * achieve this shape.  When padtype is "VALID" then paddings will be all zeros
 * and the padded_shape will provide the target output shape given the provided
 * parameters.
 */
NN_AVAILABLE_SINCE_2_3
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_padding(NNTensor*      tensor,
                  const char*    padtype,
                  const int32_t* window,
                  const int32_t* stride,
                  const int32_t* dilation,
                  int32_t*       padded_shape,
                  int32_t*       paddings);

/**
 * nn_tensor_pad implements a padded Tensor to Tensor copy.  This can be used to
 * achieve the various convolution padding strategies (SAME, FULL).  For example
 * SAME conv2d would use the following padded_copy before running the conv2d
 * layer.
 *
 * output_shape = { input_shape[0],
 *                  int(ceil(float(input_shape[1]) /
 * strides[1])), int(ceil(float(input_shape[2]) / strides[2])), weights_shape[3]
 * };
 *
 * pad_height = (output_shape[1] - 1) * strides[1] + weights_shape[0] -
 * input_shape[1]; pad_width  = (output_shape[2] - 1) * strides[2] +
 * weights_shape[1] - input_shape[2];
 *
 * @output pointer to the output tensor
 * @input pointer to the input tensor
 * @head lead-in length of the pad for dimension NHWC
 * @tail lead-out length of the pad for dimension NHWC
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_pad(NNTensor*     output,
              NNTensor*     input,
              const int32_t head[4],
              const int32_t tail[4],
              double        constant);

/**
 * Loads an image from file into the provided tensor.
 *
 * @public @memberof NNTensor
 * @since 2.2
 */
NN_AVAILABLE_SINCE_2_2
NN_DEPRECATED_SINCE_2_3
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_load_file(NNTensor* tensor, const char* filename);

NN_AVAILABLE_SINCE_2_2
NN_DEPRECATED_SINCE_2_3
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_load_file_ex(NNTensor* tensor, const char* filename, uint32_t proc);

/**
 * Loads an image from the provided buffer and decodes it accordingly, the
 * function uses the images headers to find an appropriate decoder.  The
 * function will handle any required casting to the target tensor's format.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_load_image(NNTensor* tensor, const void* image, size_t image_size);

/**
 * Loads an image from the provided buffer and decodes it accordingly, the
 * function uses the images headers to find an appropriate decoder.  The
 * function will handle any required casting to the target tensor's format and
 * will apply image standardization (compatible with tensorflow's
 * tf.image.per_image_standardization) if the proc parameter is set to
 * NN_IMAGE_PROC_WHITENING.
 *
 * When called with proc==0 it is the same as nn_tensor_load_image().
 *
 * NN_IMAGE_PROC_UNSIGNED_NORM
 * NN_IMAGE_PROC_WHITENING_NORM
 * NN_IMAGE_PROC_SIGNED_NORM
 *
 * @public @memberof NNTensor
 * @since 2.1
 */
NN_AVAILABLE_SINCE_2_1
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_load_image_ex(NNTensor*   tensor,
                        const void* image,
                        size_t      image_size,
                        uint32_t    proc);

/**
 * Loads a video frame from virtual or physical memory into the tensor, handling
 * any required conversions (such as casting to floating point, if required).
 * The frame must have a stride calculated from with and a known fourcc code,
 * for example YUYV would need stride to be width*2 whereas NV12 would required
 * stride to be width. For planar formats each plane must be packed
 * sequentially, so for NV12 the UV planes must follow immediately after the Y
 * plane.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_DEPRECATED_SINCE_2_3
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_load_frame(NNTensor*     tensor,
                     void*         memory,
                     void*         physical,
                     uint32_t      fourcc,
                     int32_t       width,
                     int32_t       height,
                     const int32_t roi[4]);

/**
 * Loads a video frame from virtual or physical memory into the tensor, handling
 * any required conversions (such as casting to floating point, if required).
 * The frame must have a stride calculated from with and a known fourcc code,
 * for example YUYV would need stride to be width*2 whereas NV12 would required
 * stride to be width. For planar formats each plane must be packed
 * sequentially, so for NV12 the UV planes must follow immediately after the Y
 * plane.
 *
 * When called with proc==0 it is the same as nn_tensor_load_frame().
 *
 * @public @memberof NNTensor
 * @since 2.1
 */
NN_AVAILABLE_SINCE_2_1
NN_DEPRECATED_SINCE_2_3
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_load_frame_ex(NNTensor*     tensor,
                        void*         memory,
                        void*         physical,
                        uint32_t      fourcc,
                        int32_t       width,
                        int32_t       height,
                        const int32_t roi[4],
                        uint32_t      proc);

/**
 * Size of the NNContext structure including reserved space and padding.
 */
#define NN_CONTEXT_SIZEOF 512

/**
 * @struct NNModel
 *
 * Model handle.
 */
typedef void NNModel;

/**
 * @struct NNModelResource
 *
 * Model resource handle.
 */
typedef void NNModelResource;

/**
 * @struct NNModelParameter
 *
 * Model parameter handle.
 */
typedef void NNModelParameter;

/**
 * @struct NNContext
 *
 * Model context handle.
 */
typedef struct nn_context NNContext;

/**
 * Attempts to validate model, this is automatically called by nn_model_load and
 * nn_model_mmap.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int
nn_model_validate(const NNModel* memory, size_t size);

/**
 * Returns the string associated with a given error.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_validate_error(int err);

/**
 * Returns the name of the given model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_name(const NNModel* model);

/**
 * Currently returns NULL (UPDATE WHEN FUNCTION IS UPDATED)
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_uuid(const NNModel* model);

/**
 * Currently returns 0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
uint32_t
nn_model_serial(const NNModel* model);

/**
 * Returns the number of labels within a given model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int
nn_model_label_count(const NNModel* model);

/**
 * Returns the label of the given index within the given model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_label(const NNModel* model, int index);

/**
 * Returns an optional icon resource for the provided label index.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const uint8_t*
nn_model_label_icon(const NNModel* model, int index, size_t* size);

/**
 * Returns the list of model input indices and optionally the number of inputs.
 *
 * If the field is missing from the model 0 is returned.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const uint32_t*
nn_model_inputs(const NNModel* model, size_t* n_inputs);

/**
 * Returns the list of model output indices and optionally the number of
 * outputs.
 *
 * If the field is missing from the model 0 is returned.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const uint32_t*
nn_model_outputs(const NNModel* model, size_t* n_outputs);

/**
 * Returns the number of layers within a given model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_model_layer_count(const NNModel* model);

/**
 * Returns the name of a layer at a given index within the given model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_layer_name(const NNModel* model, size_t index);

/**
 * Returns the index of a given layer with the name provided in the given model
 * object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int
nn_model_layer_lookup(const NNModel* model, const char* name);

/**
 * Returns the type of a layer at the given index within the given model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_layer_type(const NNModel* model, size_t index);

/**
 * Returns the type ID of the layer.
 */
NN_API
int16_t
nn_model_layer_type_id(const NNModel* model, size_t index);

/**
 * Returns the datatype of a layer at the given index within the given model
 * object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_layer_datatype(const NNModel* model, size_t index);

/**
 * Returns the datatype of a layer at the given index within the given model
 * object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNTensorType
nn_model_layer_datatype_id(const NNModel* model, size_t index);

/**
 * Returns the array of quantization zero-points, and optionally the number of
 * zero-points in the array.  The length will either be 0, 1, or equal to the
 * number of channels in an NHWC/NCHW tensor.
 *
 * The channel axis can be queried using @ref nn_model_layer_axis().
 *
 * If no quantization parameters are available then n_zeros will be 0.
 * If the tensor is quantized using full tensor quantization n_zeros will be 1.
 * If the tensor is quantized using per-channel quantization n_zeros will be C
 * which will equal the channel dimension of the tensor.  For an NHWC tensor it
 * would equal shape[3].
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const int32_t*
nn_model_layer_zeros(const NNModel* model, size_t index, size_t* n_zeros);

/**
 * Returns the array of quantization scales, and optionally the number of scales
 * in the array.  The length will either be 0, 1, or equal to the number of
 * channels in an NHWC/NCHW tensor.
 *
 * The channel axis can be queried using @ref nn_model_layer_axis().
 *
 * If no quantization parameters are available then n_scales will be 0.
 * If the tensor is quantized using full tensor quantization n_scales will be 1.
 * If the tensor is quantized using per-channel quantization n_scales will be C
 * which will equal the channel dimension of the tensor.  For an NHWC tensor it
 * would equal shape[3].
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const float*
nn_model_layer_scales(const NNModel* model, size_t index, size_t* n_scales);

/**
 * Returns the natural data axis for the tensor or -1 if one is not set.
 */
NN_AVAILABLE_SINCE_2_4
NN_API
int
nn_model_layer_axis(const NNModel* model, size_t index);

/**
 * Returns the shape of a layer at the given index within the given model
 * object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const int32_t*
nn_model_layer_shape(const NNModel* model, size_t index, size_t* n_dims);

/**
 * Returns the number of inputs to a layer at the given index within the given
 * model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_model_layer_inputs(const NNModel*   model,
                      size_t           index,
                      const uint32_t** inputs);

/**
 * Returns an NNModelParameter from the model at the layer index defined by
 * layer using the parameter key.  If the layer does not contain this parameter
 * NULL is returned.
 */
NN_API
const NNModelParameter*
nn_model_layer_parameter(const NNModel* model, size_t layer, const char* key);

/**
 * Returns the shape of the model parameter for layer at index <layer>.
 *
 * @ref nn_model_parameter_shape()
 *
 * Returns NULL if either the parameter is not found or the shape is missing.
 */
NN_API
const int32_t*
nn_model_layer_parameter_shape(const NNModel* model,
                               size_t         layer,
                               const char*    key,
                               size_t*        n_dims);

/**
 * Returns float data for parameter <key> at layer index <layer>.  This is a
 * convenience wrapper around acquiring the parameter followed by acquiring the
 * data.
 *
 * @ref nn_model_parameter_data_f32()
 *
 * Returns NULL if either the parameter is not found or the data is missing.
 */
NN_API
const float*
nn_model_layer_parameter_data_f32(const NNModel* model,
                                  size_t         layer,
                                  const char*    key,
                                  size_t*        length);

/**
 * Returns int16 data for parameter <key> at layer index <layer>.  This is a
 * convenience wrapper around acquiring the parameter followed by acquiring the
 * data.
 *
 * @ref nn_model_parameter_data_i16()
 *
 * Returns NULL if either the parameter is not found or the data is missing.
 */
NN_API
const int16_t*
nn_model_layer_parameter_data_i16(const NNModel* model,
                                  size_t         layer,
                                  const char*    key,
                                  size_t*        length);

/**
 * Returns raw data for parameter <key> at layer index <layer>.  This is a
 * convenience wrapper around acquiring the parameter followed by acquiring the
 * data.
 *
 * @ref nn_model_parameter_data_raw()
 *
 * Returns NULL if either the parameter is not found or the data is missing.
 */
NN_API
const uint8_t*
nn_model_layer_parameter_data_raw(const NNModel* model,
                                  size_t         layer,
                                  const char*    key,
                                  size_t*        length);

/**
 * Returns string data for parameter <key> at layer index <layer> for string
 * array element <index>.  This is a convenience wrapper around acquiring the
 * parameter followed by acquiring the data.
 *
 * @ref nn_model_parameter_data_str()
 *
 * Returns NULL if either the parameter is not found or the data is missing.
 */
NN_API
const char*
nn_model_layer_parameter_data_str(const NNModel* model,
                                  size_t         layer,
                                  const char*    key,
                                  size_t         index);

/**
 * Returns number of string elements in the data_str array for the specified
 * layer and parameter key.  This is a convenience wrapper around acquiring the
 * parameter followed by acquiring the data.
 *
 * @ref nn_model_parameter_data_str_len()
 *
 * Returns number of string elements in the array.
 */
NN_API
size_t
nn_model_layer_parameter_data_str_len(const NNModel* model,
                                      size_t         layer,
                                      const char*    key);

/**
 * Returns the shape of the parameter data or NULL if no shape was defined.  If
 * n_dims is non-NULL the number of dimensions will be stored there.  The shape
 * attribute is not required for parameters but can be used either on its own
 * or as part of defining layout of data attributes.
 */
NN_API
const int32_t*
nn_model_parameter_shape(const NNModelParameter* parameter, size_t* n_dims);

/**
 * Returns parameter float data, length of the array is optionally stored into
 * the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 */
NN_API
const float*
nn_model_parameter_data_f32(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter int32_t data, length of the array is optionally stored into
 * the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 */
NN_API
const int32_t*
nn_model_parameter_data_i32(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter int16_t data, length of the array is optionally stored into
 * the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 */
NN_API
const int16_t*
nn_model_parameter_data_i16(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter int8_t data, length of the array is optionally stored into
 * the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 */
NN_API
const int8_t*
nn_model_parameter_data_i8(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter raw data pointer, length of the array is optionally stored
 * into the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 */
NN_API
const uint8_t*
nn_model_parameter_data_raw(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter string data at desired index.  This data handler is
 * different from the others which return the array as strings are themselves
 * arrays and need special handling. Refer to @ref
 * nn_model_parameter_data_str_len() to query the size of the data_str array,
 * which refers to the number of strings in this parameter.
 */
NN_API
const char*
nn_model_parameter_data_str(const NNModelParameter* parameter, size_t index);

/**
 * Returns the number of strings in the parameter's data_str attribute.
 */
NN_API
size_t
nn_model_parameter_data_str_len(const NNModelParameter* parameter);

/**
 * Returns the memory size of the given model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_model_memory_size(const NNModel* model);

/**
 * Returns the minimum cache size of a given model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_model_cache_minimum_size(const NNModel* model);

/**
 * Returns the optimum cache size of a given model object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_model_cache_optimum_size(const NNModel* model);

/**
 *
 */
NN_API
size_t
nn_model_resource_count(const NNModel* model);

/**
 *
 */
NN_API
const NNModelResource*
nn_model_resource_at(const NNModel* model, size_t index);

/**
 *
 */
NN_API
const NNModelResource*
nn_model_resource(const NNModel* model, const char* name);

/**
 *
 */
NN_API
const char*
nn_model_resource_name(const NNModelResource* resource);

/**
 *
 */
NN_API
const char*
nn_model_resource_meta(const NNModelResource* resource);

/**
 *
 */
NN_API
const char*
nn_model_resource_mime(const NNModelResource* resource);

/**
 *
 */
NN_API
const uint8_t*
nn_model_resource_data(const NNModelResource* resource, size_t* data_size);

/**
 * Implementation not found.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_context_sizeof();

/**
 * Initializes an NNContext and allocates required memories.  If any of the
 * pointers are NULL malloc will be called automatically to create the memory
 * using the provided sizes.  For memory_size and cache_size if these are 0
 * then they will not be initialized.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNContext*
nn_context_init(NNEngine* engine,
                size_t    memory_size,
                void*     memory,
                size_t    cache_size,
                void*     cache);

/**
 * Initializes an NNContext into the provided memory which *MUST* be at least
 * NN_CONTEXT_SIZEOF bytes.  If any of the pointers are NULL malloc will be
 * called automatically to create the memory using the provided sizes.  For
 * memory_size and cache_size if these are 0 then they will not be initialized.
 */
NN_API
NNContext*
nn_context_init_ex(void*     context_memory,
                   NNEngine* engine,
                   size_t    memory_size,
                   void*     memory,
                   size_t    cache_size,
                   void*     cache);

/**
 * Release the memory being used by the given context object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_context_release(NNContext* context);

/**
 * Callback function for custom user ops.
 */
typedef NNError(nn_user_ops)(NNContext*  context,
                             const char* opname,
                             size_t      index);

/**
 *
 */
NN_API
NNError
nn_context_user_ops_register(NNContext* context, nn_user_ops* callback);

/**
 *
 */
NN_API
nn_user_ops*
nn_context_user_ops(NNContext* context);

/**
 *
 */
NN_AVAILABLE_SINCE_2_2
NN_API
NNTensor*
nn_context_cache(NNContext* context);

/**
 *
 */
NN_AVAILABLE_SINCE_2_2
NN_API
NNTensor*
nn_context_mempool(NNContext* context);

/**
 * Returns the engine used by the given context object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNEngine*
nn_context_engine(NNContext* context);

/**
 * Returns the currently loaded model blob for the context.
 */
NN_API
const NNModel*
nn_context_model(NNContext* context);

/**
 * Loads the model provided by the input into the context.
 *
 * @context pointer to the context object
 * @memory pointer to the memory that contains the model
 * @memory_size the size of the memory that is used by the model
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_context_model_load(NNContext*  context,
                      size_t      memory_size,
                      const void* memory);

/**
 * Frees the memory used by the model within the given context object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_context_model_unload(NNContext* context);

/**
 * Returns the tensor with the given name within the model provided by the given
 * context object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNTensor*
nn_context_tensor(NNContext* context, const char* name);

/**
 * Returns the tensor at the given index with the model provided by the given
 * context object.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNTensor*
nn_context_tensor_index(NNContext* context, size_t index);

/**
 * Runs the model within the given context object.
 */
NN_AVAILABLE_SINCE_2_0
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_context_run(NNContext* context);

/**
 * Runs layer with index from model within the given context object.
 * If index is invalid NN_ERROR_INVALID_LAYER is returned, this can be
 * used to determine when at the end of the model.
 */
NN_AVAILABLE_SINCE_2_3
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_context_step(NNContext* context, size_t index);

/**
 * Implementation not found.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_context_label(const NNContext* context);

/**
 * Implementation not found.
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_context_outputs(const NNContext* context,
                   size_t           n_outputs,
                   const NNTensor** outputs);

#ifdef __cplusplus
}
#endif

#endif /* DEEPVIEW_RT_H */
