#ifndef __CEI_H__
#define __CEI_H__

/* Provisionary CEI definitions */

#define CEI_ENCODER_ENCODEERR -1

/**
 * @brief Holds buffer (pointer and length) information,
 */
typedef struct {
    void *ptr;
    uint32_t size;
} BufferInfo;

/**
 * @brief Defines a function returning a memory size to be allocated.
 * 
 * @param channels Number of channels, which the encoder will encode.
 * @return The number of bytes to be allocated.
 */
typedef int(*CeiFnGetMemorySize)(uint8_t channels);

/**
 * @brief Defines a function initializing an encoder.
 * 
 * @param memory Pointer to memory previously allocated to hold encoder structures.
 * @param sampleRate The source sample rate.
 * @param channels Number of channels which the encoder will encode.
 * 
 * @returns 0 if encoder was initialized successfully, anything other otherwise.
 */
typedef int(*CeiFnEncoderInit)(void *memory, 
                               uint32_t sampleRate, 
                               uint8_t channels);

/**
 * @brief Defines a function retrieving encoder configuration.
 *
 * @param memory Pointer to previously allocated memory with encoder structures. Can be NULL if default configuration is desired.
 * @param config Pointer to an encoder-specific configuration structure.
 *
 * @returns 0 if configuration gathered successfully, anything other otherwise.
 */
typedef int(*CeiFnEncoderGetConfig)(void* memory, void* config);

/**
 * @brief Defines a function applying configuration to an encoder.
 *
 * @param memory Pointer to previously allocated memory with encoder structures.
 * @param config Pointer to an encoder-specific configuration structure.
 *
 * @returns 0 if configuration applied successfully, anything other otherwise.
 */
typedef int(*CeiFnEncoderSetConfig)(void* memory, void* config);

/**
 * @brief Defines a function which encodes data in a given input buffer to a given output buffer. Input buffer will contain 16-bit signed interleaved little-endian samples (s16le).
 * 
 * @param memory Pointer to previously allocated memory with encoder structures.
 * @param source Contains information about the input buffer.
 * @param dest Contains information about the output buffer.
 * 
 * @returns Number of produced bytes or CEI_ENCODER_ENCODEERR. 0 constitutes an EOS condition.
 */
typedef int(*CeiFnEncode)(void *memory, BufferInfo* source, BufferInfo* dest);

typedef struct {
    CeiFnGetMemorySize fnGetMemorySize;
    CeiFnEncoderInit fnInit;
    CeiFnEncoderGetConfig fnGetConfig;
    CeiFnEncoderSetConfig fnSetConfig;
    CeiFnEncode fnEncode;
} CeiEncoderFunctions;

#endif