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
#define NN_VERSION_ENCODE(major, minor, revision)                              \
    (((major) *1000000) + ((minor) *1000) + (revision))
#define NN_VERSION_DECODE_MAJOR(version) ((version) / 1000000)
#define NN_VERSION_DECODE_MINOR(version) (((version) % 1000000) / 1000)
#define NN_VERSION_DECODE_REVISION(version) ((version) % 1000)

#if defined(__GNUC__) && defined(__GNUC_PATCHLEVEL__)
#define NN_GNUC_VERSION                                                        \
    NN_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#elif defined(__GNUC__)
#define NN_GNUC_VERSION NN_VERSION_ENCODE(__GNUC__, __GNUC_MINOR__, 0)
#endif

#if defined(NN_GNUC_VERSION)
#define NN_GNUC_VERSION_CHECK(major, minor, patch)                             \
    (NN_GNUC_VERSION >= NN_VERSION_ENCODE(major, minor, patch))
#else
#define NN_GNUC_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(__CC_ARM) && defined(__ARMCOMPILER_VERSION)
#define NN_ARM_VERSION                                                         \
    NN_VERSION_ENCODE(__ARMCOMPILER_VERSION / 1000000,                         \
                      (__ARMCOMPILER_VERSION % 1000000) / 10000,               \
                      (__ARMCOMPILER_VERSION % 10000) / 100)
#elif defined(__CC_ARM) && defined(__ARMCC_VERSION)
#define NN_ARM_VERSION                                                         \
    NN_VERSION_ENCODE(__ARMCC_VERSION / 1000000,                               \
                      (__ARMCC_VERSION % 1000000) / 10000,                     \
                      (__ARMCC_VERSION % 10000) / 100)
#endif

#if defined(NN_ARM_VERSION)
#define NN_ARM_VERSION_CHECK(major, minor, patch)                              \
    (NN_ARM_VERSION >= NN_VERSION_ENCODE(major, minor, patch))
#else
#define NN_ARM_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(__IAR_SYSTEMS_ICC__)
#if __VER__ > 1000
#define NN_IAR_VERSION                                                         \
    NN_VERSION_ENCODE((__VER__ / 1000000),                                     \
                      ((__VER__ / 1000) % 1000),                               \
                      (__VER__ % 1000))
#else
#define NN_IAR_VERSION NN_VERSION_ENCODE(VER / 100, __VER__ % 100, 0)
#endif
#endif

#if defined(NN_IAR_VERSION)
#define NN_IAR_VERSION_CHECK(major, minor, patch)                              \
    (NN_IAR_VERSION >= NN_VERSION_ENCODE(major, minor, patch))
#else
#define NN_IAR_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(NN_GNUC_VERSION) && !defined(__clang) && !defined(NN_ARM_VERSION)
#define NN_GCC_VERSION NN_GNUC_VERSION
#endif

#if defined(NN_GCC_VERSION)
#define NN_GCC_VERSION_CHECK(major, minor, patch)                              \
    (NN_GCC_VERSION >= NN_VERSION_ENCODE(major, minor, patch))
#else
#define NN_GCC_VERSION_CHECK(major, minor, patch) (0)
#endif

#if defined(__cplusplus) && (__cplusplus >= 201402L)
#define NN_DEPRECATED(since) [[deprecated("Since " #since)]]
#elif _MSC_VER >= 1400
#define NN_DEPRECATED(since) __declspec(deprecated("Since " #since))
#elif _MSC_VER >= 1310
#define NN_DEPRECATED(since) _declspec(deprecated)
#elif NN_IAR_VERSION_CHECK(8, 0, 0)
#define NN_DEPRECATED(since) _Pragma("deprecated")
#elif defined(_GHS_MULTI)
#define NN_DEPRECATED(since)
#else
#define NN_DEPRECATED(since) __attribute__((__deprecated__("Since " #since)))
#endif

#if NN_GCC_VERSION_CHECK(4, 3, 0)
#define NN_UNAVAILABLE(available_since)                                        \
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

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) &&              \
    !defined(__STDC_NO_VLA__) && !defined(__cplusplus) &&                      \
    !defined(NN_PGI_VERSION) && !defined(NN_TINYC_VERSION)
#define NN_ARRAY_PARAM(name) static(name)
#else
#define NN_ARRAY_PARAM(name)
#endif

#if !defined(__cplusplus) &&                                                   \
    ((defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)) ||           \
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
#define NN_VERSION_2_4_18 NN_VERSION_ENCODE(2, 4, 18)
#define NN_VERSION_2_4_25 NN_VERSION_ENCODE(2, 4, 25)
#define NN_VERSION_2_4_28 NN_VERSION_ENCODE(2, 4, 28)
#define NN_VERSION_2_4_30 NN_VERSION_ENCODE(2, 4, 30)
#define NN_VERSION_2_4_32 NN_VERSION_ENCODE(2, 4, 32)
#define NN_VERSION_2_4_42 NN_VERSION_ENCODE(2, 4, 42)

/**
 * This macro defines the target version when compiling against deepview_rt.h
 * and deepview_ops.h and will cause warnings to be generated when the target
 * version does not include a symbol or if a symbol is marked as depracated.
 *
 * By default the target version is the latest version of DeepViewRT for the
 * provided deepview_rt.h file.
 */
#ifndef NN_TARGET_VERSION
#define NN_TARGET_VERSION NN_VERSION_2_4_42
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 0, 0)
#define NN_AVAILABLE_SINCE_2_0 NN_UNAVAILABLE(2.0)
#define NN_DEPRECATED_SINCE_2_0
#else
#define NN_AVAILABLE_SINCE_2_0
#define NN_DEPRECATED_SINCE_2_0 NN_DEPRECATED(2.0)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 1, 0)
#define NN_AVAILABLE_SINCE_2_1 NN_UNAVAILABLE(2.1)
#define NN_DEPRECATED_SINCE_2_1
#else
#define NN_AVAILABLE_SINCE_2_1
#define NN_DEPRECATED_SINCE_2_1 NN_DEPRECATED(2.1)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 2, 0)
#define NN_AVAILABLE_SINCE_2_2 NN_UNAVAILABLE(2.2)
#define NN_DEPRECATED_SINCE_2_2
#else
#define NN_AVAILABLE_SINCE_2_2
#define NN_DEPRECATED_SINCE_2_2 NN_DEPRECATED(2.2)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 3, 0)
#define NN_AVAILABLE_SINCE_2_3 NN_UNAVAILABLE(2.3)
#define NN_DEPRECATED_SINCE_2_3
#else
#define NN_AVAILABLE_SINCE_2_3
#define NN_DEPRECATED_SINCE_2_3 NN_DEPRECATED(2.3)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 4, 0)
#define NN_AVAILABLE_SINCE_2_4 NN_UNAVAILABLE(2.4)
#define NN_DEPRECATED_SINCE_2_4
#else
#define NN_AVAILABLE_SINCE_2_4
#define NN_DEPRECATED_SINCE_2_4 NN_DEPRECATED(2.4)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 4, 18)
#define NN_AVAILABLE_SINCE_2_4_18 NN_UNAVAILABLE(2.4.18)
#define NN_DEPRECATED_SINCE_2_4_18
#else
#define NN_AVAILABLE_SINCE_2_4_18
#define NN_DEPRECATED_SINCE_2_4_18 NN_DEPRECATED(2.4.18)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 4, 25)
#define NN_AVAILABLE_SINCE_2_4_25 NN_UNAVAILABLE(2.4.25)
#define NN_DEPRECATED_SINCE_2_4_25
#else
#define NN_AVAILABLE_SINCE_2_4_25
#define NN_DEPRECATED_SINCE_2_4_25 NN_DEPRECATED(2.4.25)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 4, 28)
#define NN_AVAILABLE_SINCE_2_4_28 NN_UNAVAILABLE(2.4.28)
#define NN_DEPRECATED_SINCE_2_4_28
#else
#define NN_AVAILABLE_SINCE_2_4_28
#define NN_DEPRECATED_SINCE_2_4_28 NN_DEPRECATED(2.4.28)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 4, 30)
#define NN_AVAILABLE_SINCE_2_4_30 NN_UNAVAILABLE(2.4.30)
#define NN_DEPRECATED_SINCE_2_4_30
#else
#define NN_AVAILABLE_SINCE_2_4_30
#define NN_DEPRECATED_SINCE_2_4_30 NN_DEPRECATED(2.4.30)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 4, 32)
#define NN_AVAILABLE_SINCE_2_4_32 NN_UNAVAILABLE(2.4.32)
#define NN_DEPRECATED_SINCE_2_4_32
#else
#define NN_AVAILABLE_SINCE_2_4_32
#define NN_DEPRECATED_SINCE_2_4_32 NN_DEPRECATED(2.4.32)
#endif

#if NN_TARGET_VERSION < NN_VERSION_ENCODE(2, 4, 42)
#define NN_AVAILABLE_SINCE_2_4_42 NN_UNAVAILABLE(2.4.42)
#define NN_DEPRECATED_SINCE_2_4_42
#else
#define NN_AVAILABLE_SINCE_2_4_42
#define NN_DEPRECATED_SINCE_2_4_42 NN_DEPRECATED(2.4.42)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define NN_IMAGE_PROC_UNSIGNED_NORM 0x0001
#define NN_IMAGE_PROC_WHITENING 0x0002
#define NN_IMAGE_PROC_SIGNED_NORM 0x0004
#define NN_IMAGE_PROC_IMAGENET 0x0008
#define NN_IMAGE_PROC_MIRROR 0x1000
#define NN_IMAGE_PROC_FLIP 0x2000

/**
 * Size of the NNEngine structure including reserved space and padding.
 */
#define NN_ENGINE_SIZEOF 1024

/**
 * Size of the NNTensor structure including reserved space and padding.
 */
#define NN_TENSOR_SIZEOF 160

/**
 * Size of the NNContext structure including reserved space and padding.
 */
#define NN_CONTEXT_SIZEOF 512

/**
 * Enumeration of all errors provided by DeepViewRT.  Most functions will
 * return an NNError with NN_SUCCESS being zero. A common usage pattern for
 * client code is to check for err using `if (err) ...` as any error condition
 * will return non-zero.
 */
typedef enum {
    /**
     * Successfull operation, no error.
     */
    NN_SUCCESS = 0,
    /**
     * Internal error without a specific error code, catch-all error.
     */
    NN_ERROR_INTERNAL,
    /**
     * The provided handle is invalid.  This error is typically used by NNEngine
     * when interfacing with another API such as OpenCL or OpenVX which require
     * native handles for their internal API.
     */
    NN_ERROR_INVALID_HANDLE,
    /**
     * Out of memory error, returned if a call to malloc returns NULL or similar
     * error from an underlying engine plugin.
     */
    NN_ERROR_OUT_OF_MEMORY,
    /**
     * Out of resources errors are similar to out of memory though sometimes
     * treated separately by underlying engine plugins.
     */
    NN_ERROR_OUT_OF_RESOURCES,
    /**
     * Signals an API has not been implemented.  Can be caught by the core
     * DeepViewRT library when interfacing with engine plugins to gracefully
     * fallback to the native implementation.
     */
    NN_ERROR_NOT_IMPLEMENTED,
    /**
     * A required parameter was missing or NULL or simply invalid.
     */
    NN_ERROR_INVALID_PARAMETER,
    /**
     * When attempting to run an operation where the input/output tensors are
     * of different types and the operation does not support automatic type
     * conversions.
     */
    NN_ERROR_TYPE_MISMATCH,
    /**
     * When attempting to run an operation and the input/output tensors have
     * invalid or unsupported shape combinations.  Some operations require the
     * shapes to be the same while others, such as arithmetic broadcasting
     * operations, will support various shape combinations but if the provided
     * pairs are invalid then the shape mismatch is returned.
     */
    NN_ERROR_SHAPE_MISMATCH,
    /**
     * The tensor's shape is invalid for the given operation.  It differs from
     * the shape mismatch in that the shape is invalid on its own and not
     * relative to another related tensor.  An example would be a shape with
     * more than one -1 dimension.
     */
    NN_ERROR_INVALID_SHAPE,
    /**
     * The requested ordering was invalid.
     */
    NN_ERROR_INVALID_ORDER,
    /**
     * The requested axis for an operation was invalid or unsupported.
     */
    NN_ERROR_INVALID_AXIS,
    /**
     * A required resource was missing or the reference invalid.
     */
    NN_ERROR_MISSING_RESOURCE,
    /**
     * The requested engine is invalid.
     */
    NN_ERROR_INVALID_ENGINE,
    /**
     * The tensor has no data or the data is not currently accessible.  An
     * example of the latter would be attempting to call @ref nn_tensor_maprw
     * while the tensor was already mapped read-only or write-only.
     */
    NN_ERROR_TENSOR_NO_DATA,
    /**
     * The internal kernel or subroutine required to complete an operation using
     * the engine plugin was missing.  An example would be OpenCL or OpenVX
     * operation where the kernel implementation cannot be located.
     */
    NN_ERROR_KERNEL_MISSING,
    /**
     * The operation does not support the tensor's type.
     */
    NN_ERROR_TENSOR_TYPE_UNSUPPORTED,
    /**
     * For operations which can operate on an array of inputs, the provided list
     * of inputs was too large.
     */
    NN_ERROR_TOO_MANY_INPUTS,
    /**
     * A system error occured when interfacing with an operating system
     * function.  On some systems errno might be updated with the underlying
     * error code.
     */
    NN_ERROR_SYSTEM_ERROR,
    /**
     * When working with a model a reference was made to a layer which did not
     * exist.
     */
    NN_ERROR_INVALID_LAYER,
    /**
     * The model is invalid or corrupted.
     */
    NN_ERROR_MODEL_INVALID,
    /**
     * An operation referenced a model but the model was not provided.
     */
    NN_ERROR_MODEL_MISSING,
    /**
     * The string was too large.
     */
    NN_ERROR_STRING_TOO_LARGE,
    /**
     * The quantization parameters are invalid.
     */
    NN_ERROR_INVALID_QUANT,
} NNError;

/**
 * @enum NNTensorType
 * Enumeration of the data types supported by NNTensors in DeepViewRT.
 */
typedef enum {
    /**
     * Raw byte-stream tensor, useful for encoded tensors such as PNG images.
     * The size of this tensor would be in bytes.
     */
    NNTensorType_RAW = 0,
    /**
     * String tensor data, a single dimension would hold one null-terminated
     * string of variable length.  A standard C char* array.
     */
    NNTensorType_STR = 1,
    /**
     * Signed 8-bit integer tensor data internally @ref int8_t
     */
    NNTensorType_I8 = 2,
    /**
     * Unsigned 8-bit integer tensor data internally @ref uint8_t
     */
    NNTensorType_U8 = 3,
    /**
     * Signed 16-bit integer tensor data internally @ref int16_t
     */
    NNTensorType_I16 = 4,
    /**
     * Unsigned 16-bit integer tensor data internally @ref uint16_t
     */
    NNTensorType_U16 = 5,
    /**
     * Signed 16-bit integer tensor data internally @ref int32_t
     */
    NNTensorType_I32 = 6,
    /**
     * Unsigned 16-bit integer tensor data internally @ref uint32_t
     */
    NNTensorType_U32 = 7,
    /**
     * Signed 16-bit integer tensor data internally @ref int64_t
     */
    NNTensorType_I64 = 8,
    /**
     * Unsigned 16-bit integer tensor data internally @ref uint64_t
     */
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
 * Enumeration of all quantization type provided by DeepViewRT.
 */
typedef enum {
    /**
     * No quantization for tensor.
     */
    NNQuantizationType_None = 0,
    /**
     * Affine quantization with parameters applied globally across the tensor.
     *
     * The scale term is queried from @ref nn_tensor_scales() while the zero
     * term is queried from @ref nn_tensor_zeros().
     *
     * Quantization: \f$ f(x) = \frac{x}{scale} + zero \f$
     *
     * Dequantization: \f$ f(x) = (x - zero) * scale \f$
     */
    NNQuantizationType_Affine_PerTensor = 1,
    /**
     * Affine quantization with separate parameters applied to each channel.
     * Also known as per-axis where the axis is always the channel "C" axis in
     * a NCHW, NHWC, and so-on shaped tensor.
     *
     * Same equation as @ref NNQuantization_Affine_PerTensor but applied
     * per-channel.  The scale and zero_point are vectors of channel length.
     */
    NNQuantizationType_Affine_PerChannel = 2,
    /**
     * Quantized using Dynamic Fixed Point.
     */
    NNQuantizationType_DFP = 3,
} NNQuantizationType;

/**
 * DeepViewRT library initialization options.
 */
typedef intptr_t NNOptions;

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
 * @struct NNTensor
 *
 * Tensors are represented by the @ref NNTensor class.  The dimensions are
 * variable and can be from 1 to 4 dimensions.  Internally the shape of a
 * 1-dimensional tensor would be [N 1 1 1] and a scalar [1 1 1 1].
 *
 * Tensors can exist locally on the CPU or when initialized using an
 * @ref NNEngine object the tensors can be mapped to a buffer on a compute
 * device such as a GPU or NPU using the DeepViewRT OpenCL or OpenVX engine
 * plugins.
 */
typedef struct nn_tensor NNTensor;

/**
 * @struct NNQuantParam
 *
 * Tensor quantization parameter structure.
 */
typedef struct nn_quant_param NNQuantParam;

/**
 * @struct NNModel
 *
 * DeepViewRT Models "RTM" are reprensted in memory through the NNModel type
 * which is meant to point to a static model blob.  This can point directly to
 * the memory of the RTM either loaded into memory, accessed through a memmap
 * or pointed directly to the flash location.  In other words if the RTM is
 * saved into flash which is connected to the memory space then the model does
 * not need to be copied into RAM before being loaded.
 *
 * Models are loaded into an @ref NNContext which handles the dynamic data
 * structures required for operation of the model.
 */
typedef void NNModel;

/**
 * @struct NNModelResource
 *
 * DeepViewRT Models may have resources embedded into them and this datatype is
 * their handle.
 */
typedef void NNModelResource;

/**
 * @struct NNModelParameter
 *
 * DeepViewRT Models use parameters to store various configuration information
 * such as layer parameters.
 */
typedef void NNModelParameter;

/**
 * @struct NNContext
 *
 * DeepViewRT models can be loaded with an NNContext and numerous contexts can
 * be loaded at once.  The context manages the runtime portion of the model
 * including the tensors required to hold intermediate buffers.
 *
 * A context itself requires @ref NN_CONTEXT_SIZEOF bytes though it will also
 * allocate on the heap additional tensor handles required to support models on
 * @ref nn_context_model_load() and these will then be released on a call to
 * @ref nn_context_model_unload().
 *
 * When a context is created an @ref NNEngine plugin may optionally be provided
 * which will take over the management of tensors through the engine plugin and
 * attempting to run models and operators on the compute device enabled by this
 * engine plugin.  If an engine is not provided DeepViewRT will use the default
 * implementation which is optimized for CPU and MCU devices.
 */
typedef struct nn_context NNContext;

/**
 * DeepViewRT library version as "MAJOR.MINOR.PATCH".
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
 *
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
 * @note As of DeepViewRT 2.4.32 this function does not do anything except on
 * RaspberryPi platforms.  This could change in the future so it is safer to
 * call the function for future compatibility.
 *
 * @return NN_SUCCESS after successfully initializing the library.
 * @return NN_ERROR_INTERNAL if the library fails to initialize.
 *
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NN_WARN_UNUSED_RESULT
NNError
nn_init(const NNOptions* options);

/**
 * The actual size of the NNEngine structure.  This will differ from the size
 * defined by @ref NN_ENGINE_SIZEOF as the later is padded for future API
 * extensions while this function returns the actual size currently required.
 *
 * @return NNEngine structure size as reported by @ref sizeof().
 *
 * @public @memberof NNEngine
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_engine_sizeof();

/**
 * Initializes the NNEngine structure using the provided memory or allocating a
 * new buffer is none was provided.
 *
 * When providing memory it must be at least the size returned by
 * @ref nn_engine_sizeof() and for statically initiallized arrays the
 * @ref NN_ENGINE_SIZEOF can be used instead which is padded for future API
 * extensions.
 *
 * @note previous to version 2.4.32 the memory parameter is required otherwise
 * NULL will always be returned and no engine structure is created.
 *
 * @param memory Pointer to the start of where a NNEngine object should be
 * initialized.
 *
 * @return Pointer to the initialized NNEngine structure.
 * @return NULL if memory was NULL and malloc using @ref nn_engine_size()
 *  returns NULL.
 *
 * @public @memberof NNEngine
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNEngine*
nn_engine_init(void* memory);

/**
 * Returns handle of the NNEngine object.
 *
 * @param memory Pointer to the NNEngine structure
 *
 * @public @memberof NNEngine
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_4
NN_API
void*
nn_engine_native_handle(NNEngine* engine);

/**
 * Releases the memory that was being used by the engine.
 *
 * @param engine Pointer to the engine object.
 *
 * @public @memberof NNEngine
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_engine_release(NNEngine* engine);

/**
 * Loads the plugin to provided engine object.  The plugin should point to an
 * engine plugin library either as an absolute or relative path or be found in
 * the standard OS search path for shared libraries.
 *
 * @param engine Pointer to the engine object.
 * @param plugin String of the absolute or relative path to the plugin.
 *
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
 *
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
 * Returns the size of the tensor object for preparing memory allocations.
 *
 * @public @memberof NNTensor
 * @since 2.0
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
 *
 * @return NULL given the memory pointer is a null pointer.
 * @return Pointer to the newly created NNTensor object.
 *
 * @public @memberof NNTensor
 * @since 2.0
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
 *
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
 * Returns the native handle of the tensor object.  This is an internal API for
 * access internal structures.
 *
 * @param tensor Pointer to the tensor object.
 *
 * @private @memberof NNTensor
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
 * @private @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_tensor_set_native_handle(NNTensor* tensor, void* handle);

/**
 * Callback function to free an auxiliary object, called from nn_tensor_release.
 *
 * @private @memberof NNTensor
 * @since 2.1
 */
typedef void(nn_aux_object_free)(NNTensor* tensor);

/**
 * Configures an auxiliary object for the tensor.  This is a private API used
 * for attaching auxiliary buffers.
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

/**
 * Extended version of the auxiliary object API which allows additional objects
 * to be attached to the tensor using name-based indexing.
 *
 * @private @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
void
nn_tensor_set_aux_object_by_name(NNTensor*           tensor,
                                 const char*         name,
                                 void*               aux_object,
                                 nn_aux_object_free* aux_object_free,
                                 bool                buffer_ownership,
                                 bool                name_ownership);

/**
 * Acquire the auxiliary object associated with the given name parameter.
 *
 * @private @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
void*
nn_tensor_aux_object_by_name(NNTensor* tensor, const char* name);

/**
 * Frees the auxiliary object associated with the given name parameter.
 *
 * @private @memberof NNTensor
 * @since 2.4
 */
nn_aux_object_free*
nn_tensor_aux_free_by_name(NNTensor* tensor, const char* name);

/**
 * Retrieves the panel size of the tensor when it has been panel-shuffled for
 * improved tiling performance.  The panel size is the vectorization length.
 *
 * @private @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
int
nn_tensor_panel_size(NNTensor* tensor);

/**
 * Sets the panel size of the tensor.  This is primarily an internal API used
 * to store the vectorization length when shuffling tensors into an optimized
 * tile format.
 *
 * @private @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
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
 *
 * @return NN_SUCCESS if the sync was successful or ignored by engines which do
 * not implement this API.
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
 * This is used for measuring the time an operation takes by capturing the time
 * the operation took into the destination tensor of the operation.  The time
 * is not the time it takes to write to the tensor, this is captured by the
 * @ref nn_tensor_io_time() function, but the time it took the operation to
 * complete (not including map/unmap times).
 *
 * @param tensor Pointer to the tensor object.
 *
 * @return Nanoseconds of processing time for the last operation which wrote
 * into this tensor.
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
 * in nanoseconds of the duration of the last map/unmap pair.  When tensors are
 * mapped to the CPU (no accelerator engine is loaded) then times are expected
 * to be zero time as no mapping is actually required and the internal pointer
 * is simply returned.  When an accelerator engine is used, such as OpenVX,
 * then the io_time measures the time the map/unmap or copy operations took to
 * complete.
 *
 * @param tensor Pointer to the tensor object.
 *
 * @return Nanoseconds of time spent in the map/unmap calls.
 *
 * @public @memberof NNTensor
 * @since 2.1
 */
NN_AVAILABLE_SINCE_2_1
NN_API
int64_t
nn_tensor_io_time(NNTensor* tensor);

/**
 * Writes the  tensor inforamtion to the FILE stream provided.  The format is
 * "[D0 D1 D2 D3]" where D0..D3 are the dimensions provided.  If the data
 * parameter is true the format will be followed by ": ..." where ... is the
 * string representation of the tensor's data.
 *
 * @warning Before version 2.4.32 this function always assumes float32 tensors
 * and will therefore lead to segmentation faults when used with integer
 * tensors.
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
 * Sets the type of a given tensor object.
 *
 * @public @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_tensor_set_type(NNTensor* tensor, NNTensorType type);

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
 *
 * @public @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
char
nn_tensor_axis(const NNTensor* tensor);

/**
 * Returns the zero-points for the tensor and optionally the number of
 * zero-points.
 *
 * @public @memberof NNTensor
 * @since 2.4
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
 *
 * @public @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
void
nn_tensor_set_zeros(NNTensor*      tensor,
                    size_t         n_zeros,
                    const int32_t* zeros,
                    int            own);

/**
 * Configures the channel axis of the tensor.  This refers to the "C" in
 * orderings such as NHWC and NCHW.
 *
 * @public @memberof NNTensor
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_4
NN_API
void
nn_tensor_set_axis(NNTensor* tensor, int32_t axis);

/**
 * Returns the scales array for the tensor and optionally the number of scales.
 *
 * @public @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const float*
nn_tensor_scales(const NNTensor* tensor, size_t* n_scales);

/**
 * Internal API used by the RTM loader to associate quantization parameters to
 * the tensor.
 *
 * @private @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
void
nn_tensor_quant_params(const NNTensor* tensor, NNQuantParam* quant_params);

/**
 * Sets the quantization scales for the tensor.  If n_scales>1 it should match
 * the channel dimension (axis) of the tensor.
 *
 * If own=1 then the tensor will take ownership of the buffer and free it when
 * the tensor is released.  Otherwise the buffer must outlive the tensor.
 *
 * @public @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
void
nn_tensor_set_scales(NNTensor*    tensor,
                     size_t       n_scales,
                     const float* scales,
                     int          own);

/**
 * Returns the quantization type for the tensor.
 *
 * @note This API was missing before version 2.4.32 and instead the
 * quantization format is inferred as affine when scales and zeros are provided
 * and per-tensor vs. per-channel is inferred based on scales/zeros being 1 or
 * greater than 1.
 *
 * @param tensor the tensor object used to query quantization type.
 *
 * @returns @ref NNQuantizationType_None for tensors which do not provide
 *  quantization parameters.
 * @returns @ref NNQuantizationType_Affine_PerTensor for tensors which provide
 *  quantization parameters which map globally to the tensor.
 * @returns @ref NNQuantizationType_Affine_PerChannel for tensors which provide
 *  quantization parameters which map to each channel "C" of the tensor.
 * @returns @ref NNQuantizationType_DFP for tensors which provide DFP
 *  parameters.  Currently unsupported.
 *
 * @public @memberof NNTensor
 * @since 2.4.32
 */
NN_AVAILABLE_SINCE_2_4_32
NN_API
NNQuantizationType
nn_tensor_quantization_type(NNTensor* tensor);

/**
 * Tensor shape comparison.
 *
 * @returns true if both shapes are equal otherwise false.
 *
 * @since 2.0
 * @deprecated 2.3
 */
NN_AVAILABLE_SINCE_2_0
NN_DEPRECATED_SINCE_2_3
NN_API
bool
nn_tensor_shape_equal(const int32_t left[4], const int32_t right[4]);

/**
 * Copys the source shape array to the destination array.
 *
 * @since 2.0
 * @deprecated 2.3
 */
NN_AVAILABLE_SINCE_2_0
NN_DEPRECATED_SINCE_2_3
NN_API
void
nn_tensor_shape_copy(int32_t dst[4], const int32_t src[4]);

/**
 * Returns the offset of a given tensor.  This function can be used to calculate
 * the index across numerous dimensions.
 *
 * @note Avoid using this function as part of inner loops as it requires a
 * multiply and add for each dimenions.  Instead it can be used in an outer loop
 * to get the starting index then increment this index in the inner loop,
 * possibly using the tensor strides.
 *
 * @param tensor the tensor object used in the operation
 * @param n_dims the number of dimensions provided in the @p shape
 * @param shape the multi-dimensional index used to calculate the linear index
 *
 * @return the element index into the tensor based on the muliple dimenional
 * indices provided.
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
 * Returns the offset of a given tensor using variable length dimensions. This
 * works the same as @ref nn_tensor_offset() but uses variable arguments. The
 * user **must** provide @p n_dims number of parameters after the
 * @p n_dims parameter.
 *
 * @param tensor the tensor object used in the operation
 * @param n_dims the number of dimensions to use when calculating the index
 * @param … variable number of shape elements which **must** be of type int32_t
 *
 * @return the element index into the tensor based on the muliple dimenional
 * indices provided.
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
 * @deprecated 2.3
 */
NN_AVAILABLE_SINCE_2_0
NN_DEPRECATED_SINCE_2_3
NN_API
int
nn_tensor_compare(NNTensor* left, NNTensor* right, double tolerance);

/**
 * Reshapes the given tensor to the provided new shape.
 *
 * @param tensor the tensor object used in the operation
 * @param n_dims the number of dimensions which the tensor will contain after
 * the operation completes successfully.  It must also match the number of
 * elements in @p shape.
 * @param shape the new shape for the tensor.  The array must be at least
 * @p n_dims elements in size.
 *
 * @return @ref NN_SUCCESS if the reshape is able to be performed
 * @return @ref NN_ERROR_SHAPE_MISMATCH if the new shape cannot be represented
 * given the previous shape of the tensor.
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
 * @deprecated 2.3
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
 * Loads a tensor with data from a user buffer
 * User has to maintain the buffer and ensure compatibility with NHWC tensor
 * Function will return error if there is a size mismatch
 * i.e (bufsize != nn_tensor_size(tensor)) or tensor is invalid
 *
 * @public @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_copy_buffer(NNTensor* tensor, const void* buffer, size_t bufsize);

/**
 * Requantizes the source tensor into the destination tensor.
 *
 * The source tensor and destination tensor should be either I8 or U8, and
 * per tensor quantized.
 *
 * @public @memberof NNTensor
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
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
 *
 * @public @memberof NNTensor
 * @since 2.4
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
 *
 * @public @memberof NNTensor
 * @since 2.4
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
 *
 * @public @memberof NNTensor
 * @since 2.4
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
 *
 * @public @memberof NNTensor
 * @since 2.4
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
 * nn_tensor_slice copies a slice of the tensor into output. For a version which
 * supports strides see @ref nn_tensor_strided_slice.
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

NN_AVAILABLE_SINCE_2_4_42
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_strided_slice(NNTensor*     output,
                        NNTensor*     input,
                        int32_t       n_axes,
                        const int32_t axes[NN_ARRAY_PARAM(n_axes)],
                        const int32_t head_[NN_ARRAY_PARAM(n_axes)],
                        const int32_t tail_[NN_ARRAY_PARAM(n_axes)],
                        const int32_t strides_[NN_ARRAY_PARAM(n_axes)]);

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
 *
 * @public @memberof NNTensor
 * @since 2.3
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
 * @deprecated 2.3
 */
NN_AVAILABLE_SINCE_2_2
NN_DEPRECATED_SINCE_2_3
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_tensor_load_file(NNTensor* tensor, const char* filename);

/**
 * Loads an image from file into the provided tensor.
 *
 * @public @memberof NNTensor
 * @since 2.2
 * @deprecated 2.3
 */
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
 * Attempts to validate model, this is automatically called by nn_model_load and
 * nn_model_mmap.  The function returns 0 on success, otherwise it will return
 * an error code which can be turned into a string by calling
 * @ref nn_model_validate_error() with the return value from
 * @ref nn_model_validate().
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int
nn_model_validate(const NNModel* memory, size_t size);

/**
 * Returns the string associated with a given error returned from
 * @ref nn_model_validate().
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_validate_error(int err);

/**
 * Returns the name of the given model object.  Names are optional and if the
 * model does not contain a name then NULL will be returned.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_name(const NNModel* model);

/**
 * Currently returns NULL (UPDATE WHEN FUNCTION IS UPDATED)
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_uuid(const NNModel* model);

/**
 * Currently returns 0
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
uint32_t
nn_model_serial(const NNModel* model);

/**
 * Returns the number of labels within a given model object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int
nn_model_label_count(const NNModel* model);

/**
 * Returns the label of the given index within the given model object.  If the
 * model contains no labels or the index is out of range then NULL will be
 * returned.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_label(const NNModel* model, int index);

/**
 * Returns an optional icon resource for the provided label index.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const uint8_t*
nn_model_label_icon(const NNModel* model, int index, size_t* size);

/**
 * Returns the list of model input indices and optionally the number of inputs.
 *
 * If the field is missing from the model NULL is returned.
 *
 * @public @memberof NNModel
 * @since 2.4
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
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const uint32_t*
nn_model_outputs(const NNModel* model, size_t* n_outputs);

/**
 * Returns the number of layers within a given model object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_model_layer_count(const NNModel* model);

/**
 * Returns the name of a layer at a given index within the given model object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_layer_name(const NNModel* model, size_t index);

/**
 * Returns the index of a given layer with the name provided in the given model
 * object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
int
nn_model_layer_lookup(const NNModel* model, const char* name);

/**
 * Returns the type of a layer at the given index within the given model object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_layer_type(const NNModel* model, size_t index);

/**
 * Returns the type ID of the layer.
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
int16_t
nn_model_layer_type_id(const NNModel* model, size_t index);

/**
 * Returns the datatype of a layer at the given index within the given model
 * object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const char*
nn_model_layer_datatype(const NNModel* model, size_t index);

/**
 * Returns the datatype of a layer at the given index within the given model
 * object.
 *
 * @public @memberof NNModel
 * @since 2.0
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
 *
 * @public @memberof NNModel
 * @since 2.4
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
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const float*
nn_model_layer_scales(const NNModel* model, size_t index, size_t* n_scales);

/**
 * Returns the natural data axis for the tensor or -1 if one is not set.
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
int
nn_model_layer_axis(const NNModel* model, size_t index);

/**
 * Returns the shape of a layer at the given index within the given model
 * object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const int32_t*
nn_model_layer_shape(const NNModel* model, size_t index, size_t* n_dims);

/**
 * Returns the number of inputs to a layer at the given index within the given
 * model object.
 *
 * @public @memberof NNModel
 * @since 2.0
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
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const NNModelParameter*
nn_model_layer_parameter(const NNModel* model, size_t layer, const char* key);

/**
 * Returns the shape of the model parameter for layer at index <layer>.
 *
 * @ref nn_model_parameter_shape()
 *
 * Returns NULL if either the parameter is not found or the shape is missing.
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
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
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
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
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
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
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
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
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
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
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
size_t
nn_model_layer_parameter_data_str_len(const NNModel* model,
                                      size_t         layer,
                                      const char*    key);

/**
 * Returns the memory size of the given model object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_model_memory_size(const NNModel* model);

/**
 * Returns the minimum cache size of a given model object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_model_cache_minimum_size(const NNModel* model);

/**
 * Returns the optimum cache size of a given model object.
 *
 * @public @memberof NNModel
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
size_t
nn_model_cache_optimum_size(const NNModel* model);

/**
 * The number of resources defined in the model.
 *
 * @param model pointer to the RTM model
 *
 * @returns number of resources defined in the model.
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
size_t
nn_model_resource_count(const NNModel* model);

/**
 * Retrieves a reference to the resource at the provided index.
 *
 * @param model pointer to the RTM model
 * @param index resource index
 *
 * @returns an @ref NNModelResource pointer for the provided @p index in the
 * given model.
 * @returns NULL if either the model or index are invalid.
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const NNModelResource*
nn_model_resource_at(const NNModel* model, size_t index);

/**
 * Retrieves a reference to the resource with the given name.
 *
 * @param model pointer to the RTM model
 * @param name the unique name of the resource
 *
 * @returns an @ref NNModelResource pointer for the provided unique @p name.
 * @returns NULL if either the @p model or @p name are invalid, NULL, or the
 *  @p name is not found.
 *
 * @public @memberof NNModel
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const NNModelResource*
nn_model_resource(const NNModel* model, const char* name);

/**
 * Returns the shape of the parameter data or NULL if no shape was defined.  If
 * n_dims is non-NULL the number of dimensions will be stored there.  The shape
 * attribute is not required for parameters but can be used either on its own
 * or as part of defining layout of data attributes.
 *
 * @public @memberof NNModelParameter
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const int32_t*
nn_model_parameter_shape(const NNModelParameter* parameter, size_t* n_dims);

/**
 * Returns parameter float data, length of the array is optionally stored into
 * the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 *
 * @public @memberof NNModelParameter
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const float*
nn_model_parameter_data_f32(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter int32_t data, length of the array is optionally stored into
 * the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 *
 * @public @memberof NNModelParameter
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const int32_t*
nn_model_parameter_data_i32(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter int16_t data, length of the array is optionally stored into
 * the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 *
 * @public @memberof NNModelParameter
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const int16_t*
nn_model_parameter_data_i16(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter int8_t data, length of the array is optionally stored into
 * the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 *
 * @public @memberof NNModelParameter
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const int8_t*
nn_model_parameter_data_i8(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter raw data pointer, length of the array is optionally stored
 * into the length parameter if non-NULL.
 *
 * If parameter does not have this data type, then NULL is returned.
 *
 * @public @memberof NNModelParameter
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const uint8_t*
nn_model_parameter_data_raw(const NNModelParameter* parameter, size_t* length);

/**
 * Returns parameter string data at desired index.  This data handler is
 * different from the others which return the array as strings are themselves
 * arrays and need special handling. Refer to @ref
 * nn_model_parameter_data_str_len() to query the size of the data_str array,
 * which refers to the number of strings in this parameter.
 *
 * @public @memberof NNModelParameter
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const char*
nn_model_parameter_data_str(const NNModelParameter* parameter, size_t index);

/**
 * Returns the number of strings in the parameter's data_str attribute.
 *
 * @public @memberof NNModelParameter
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
size_t
nn_model_parameter_data_str_len(const NNModelParameter* parameter);

/**
 * The unique name of the resource as can be used to retrieve the resource using
 * @ref nn_model_resource().
 *
 * @param resource pointer to a @ref NNModelResource retrieved from the model.
 *
 * @returns A string with the name of the resource.
 * @returns NULL if the resource or name is NULL.
 *
 * @public @memberof NNModelResource
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const char*
nn_model_resource_name(const NNModelResource* resource);

/**
 * Returns the meta string for the resource.
 *
 * @param resource pointer to a @ref NNModelResource retrieved from the model.
 *
 * @returns A string with the meta parameter of the resource.
 * @returns NULL if the resource or meta are NULL.
 *
 * @public @memberof NNModelResource
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const char*
nn_model_resource_meta(const NNModelResource* resource);

/**
 * Returns the mime type string for the resource.
 *
 * @param resource pointer to a @ref NNModelResource retrieved from the model.
 *
 * @returns A string with the mime parameter of the resource.
 * @returns NULL if the resource or mime are NULL.
 *
 * @public @memberof NNModelResource
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const char*
nn_model_resource_mime(const NNModelResource* resource);

/**
 * Returns the raw binary data for the resource, the size of the data will be
 * saved in @p data_size if non-NULL.
 *
 * @param resource pointer to a @ref NNModelResource retrieved from the model.
 * @param data_size optional pointer to a size_t to receive the length in bytes
 * of the data, if provided.
 *
 * @returns pointer to the start of the data stream of length @p data_size.
 * @returns NULL if resource has no data associated.
 *
 * @public @memberof NNModelResource
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
const uint8_t*
nn_model_resource_data(const NNModelResource* resource, size_t* data_size);

/**
 * Returns the actual size of the context structure.  This size will be smaller
 * than @ref NN_CONTEXT_SIZEOF which contains additional padding for future
 * extension.  Since @ref nn_context_sizeof() is called dynamically at runtime
 * it can return the true and unpadded size.
 *
 * @public @memberof NNContext
 * @since 2.0
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
 *
 * @public @memberof NNContext
 * @since 2.0
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
 *
 * @public @memberof NNContext
 * @since 2.0
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
 *
 * @public @memberof NNContext
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_context_release(NNContext* context);

/**
 * Callback function for custom user ops.
 *
 * @public @memberof NNContext
 * @since 2.4
 */
typedef NNError(nn_user_ops)(NNContext*  context,
                             const char* opname,
                             size_t      index);

/**
 * @public @memberof NNContext
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
NNError
nn_context_user_ops_register(NNContext* context, nn_user_ops* callback);

/**
 * @public @memberof NNContext
 * @since 2.4
 */
NN_AVAILABLE_SINCE_2_4
NN_API
nn_user_ops*
nn_context_user_ops(NNContext* context);

/**
 * @public @memberof NNContext
 * @since 2.2
 */
NN_AVAILABLE_SINCE_2_2
NN_API
NNTensor*
nn_context_cache(NNContext* context);

/**
 * @public @memberof NNContext
 * @since 2.2
 */
NN_AVAILABLE_SINCE_2_2
NN_API
NNTensor*
nn_context_mempool(NNContext* context);

/**
 * Returns the engine used by the given context object.
 *
 * @public @memberof NNContext
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNEngine*
nn_context_engine(NNContext* context);

/**
 * Returns the currently loaded model blob for the context.
 *
 * @public @memberof NNContext
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
const NNModel*
nn_context_model(NNContext* context);

/**
 * Loads the model provided by the input into the context.
 *
 * @context pointer to the context object
 * @memory pointer to the memory that contains the model
 * @memory_size the size of the memory that is used by the model
 *
 * @public @memberof NNContext
 * @since 2.0
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
 *
 * @public @memberof NNContext
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
void
nn_context_model_unload(NNContext* context);

/**
 * Returns the tensor with the given name within the model provided by the given
 * context object.
 *
 * @public @memberof NNContext
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNTensor*
nn_context_tensor(NNContext* context, const char* name);

/**
 * Returns the tensor at the given index with the model provided by the given
 * context object.
 *
 * @public @memberof NNContext
 * @since 2.0
 */
NN_AVAILABLE_SINCE_2_0
NN_API
NNTensor*
nn_context_tensor_index(NNContext* context, size_t index);

/**
 * Runs the model within the given context object.
 *
 * @public @memberof NNContext
 * @since 2.0
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
 *
 * @public @memberof NNContext
 * @since 2.3
 */
NN_AVAILABLE_SINCE_2_3
NN_WARN_UNUSED_RESULT
NN_API
NNError
nn_context_step(NNContext* context, size_t index);

/**
 * Exposes the free() function
 *
 */
NN_API
void
nn_free(void *ptr);

/**
 * Exposes the malloc() function
 *
 */
NN_API
void *
nn_malloc(size_t size);

#ifdef __cplusplus
}
#endif



#endif /* DEEPVIEW_RT_H */

