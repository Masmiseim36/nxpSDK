/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <cstring>
#include <algorithm>
#include <sstream>
#include <fstream>
#if WIN32
#include <intrin.h>
#endif
#include <stdio.h>
#include "Keyblob.h"
#include "Logging.h"
#include "aes128_key_wrap_unwrap.h"
#include "crc.h"
#include "RijndaelCTR.h"

#define PRINT_KEYBLOB 0
#define DEBUG_DATA 0

// From bytes_aes.c.
extern "C" void KeyExpansion(unsigned char ckey[], int nbits, unsigned int w[]);

using namespace elftosb;

void Keyblob::encrypt(uint32_t length, uint8_t *data, AESKey<128> *key, AESCounter<128> *counter)
{
    // Create counter mode encrypter.
    RijndaelCTR encrypter(*key, *counter);

    uint32_t tempBlock[k_encryptBlockSize / sizeof(uint32_t)];
    uint32_t bytesEncrypted = 0;
    const uint32_t blockSize = k_encryptBlockSize;

    // length must be a multiple of k_blockSize and k_qspiAlignlength.
    assert(length && !(length % blockSize) && !(length % k_qspiAlignlength));

    // Encrypt all blocks of data from the image.
    while (bytesEncrypted < length)
    {
        memset(tempBlock, 0, sizeof(tempBlock));

        // Copy in plaintext data.
        memcpy(tempBlock, data, blockSize);

// We endian swap each uint32_t in the block and then swap word 0 <-> word 1 and word 2 <-> word 3
// to counter for endian issues and 64 bit writes on the device with a QSPI configuration for 64 bit LE
// this lets unencrypted byte streams as well as this encrypted byte stream be transparently used by the driver
#if WIN32
        tempBlock[0] = _byteswap_ulong(tempBlock[0]);
        tempBlock[1] = _byteswap_ulong(tempBlock[1]);
        tempBlock[2] = _byteswap_ulong(tempBlock[2]);
        tempBlock[3] = _byteswap_ulong(tempBlock[3]);
#else
        tempBlock[0] = __builtin_bswap32(tempBlock[0]);
        tempBlock[1] = __builtin_bswap32(tempBlock[1]);
        tempBlock[2] = __builtin_bswap32(tempBlock[2]);
        tempBlock[3] = __builtin_bswap32(tempBlock[3]);
#endif

        std::swap(tempBlock[0], tempBlock[1]);
        std::swap(tempBlock[2], tempBlock[3]);

        encrypter.encrypt((uint8_t *)tempBlock, sizeof(tempBlock), (uint8_t *)tempBlock);

        // Reverse the above transform so that it is back in its original order when decrypted
        std::swap(tempBlock[0], tempBlock[1]);
        std::swap(tempBlock[2], tempBlock[3]);

#if WIN32
        tempBlock[0] = _byteswap_ulong(tempBlock[0]);
        tempBlock[1] = _byteswap_ulong(tempBlock[1]);
        tempBlock[2] = _byteswap_ulong(tempBlock[2]);
        tempBlock[3] = _byteswap_ulong(tempBlock[3]);
#else
        tempBlock[0] = __builtin_bswap32(tempBlock[0]);
        tempBlock[1] = __builtin_bswap32(tempBlock[1]);
        tempBlock[2] = __builtin_bswap32(tempBlock[2]);
        tempBlock[3] = __builtin_bswap32(tempBlock[3]);
#endif

        // Overwrite plaintext data with enrypted data.
        std::memcpy(data, tempBlock, blockSize);

        bytesEncrypted += blockSize;
        data += blockSize;
    }
}

AESCounter<128> *buildCounter(const std::string &counterVal, uint32_t startAddress)
{
    /* CTRn_x[127-0] = {CTR_W0_x[C0...C3], // 32 bits of pre-programmed CTR
    CTR_W1_x[C4...C7],                     // another 32 bits of CTR
    CTR_W0_x[C0...C3] ^ CTR_W1_x[C4...C7], // exclusive-OR of CTR values
    systemAddress[31-4], 0000b}            // 0-modulo-16 system address */

    assert(counterVal.size() == 16); // We need 16 characters for the counter value
    assert(!(startAddress % 16));    // Start adress has to be 16 byte aligned

    AESCounter<128>::counter_t counter;
    std::string counterW0String = counterVal.substr(0, 8);
    std::string counterW1String = counterVal.substr(8, 8);
    uint32_t counterW0 = strtoul(counterW0String.c_str(), NULL, 16);
    uint32_t counterW1 = strtoul(counterW1String.c_str(), NULL, 16);
    uint32_t counterXOR = counterW0 ^ counterW1;

    // Form the collected data into a 16 byte array
    counter[15] = startAddress & 0xFF;
    counter[14] = (startAddress & 0xFF00) >> 8;
    counter[13] = static_cast<uint8_t>((startAddress & 0xFF0000) >> 16);
    counter[12] = (startAddress & 0xFF000000) >> 24;

    counter[11] = counterXOR & 0xFF;
    counter[10] = (counterXOR & 0xFF00) >> 8;
    counter[9] = static_cast<uint8_t>((counterXOR & 0xFF0000) >> 16);
    counter[8] = (counterXOR & 0xFF000000) >> 24;

    counter[7] = counterW1 & 0xFF;
    counter[6] = (counterW1 & 0xFF00) >> 8;
    counter[5] = static_cast<uint8_t>((counterW1 & 0xFF0000) >> 16);
    counter[4] = (counterW1 & 0xFF000000) >> 24;

    counter[3] = counterW0 & 0xFF;
    counter[2] = (counterW0 & 0xFF00) >> 8;
    counter[1] = static_cast<uint8_t>((counterW0 & 0xFF0000) >> 16);
    counter[0] = (counterW0 & 0xFF000000) >> 24;

    return new AESCounter<128>(counter); // caller must deallocate
}

bool Keyblob::encryptMatchingRange(uint32_t start, uint32_t len, uint8_t *data)
{
    assert(data);
    option_vector_t *options = getOptions();
    unsigned matchedCount = 0;
#if DEBUG_DATA
    std::ofstream qspiBin("enc.bin", std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
#endif

    // Go through each option entry to find matching range.
    // Encrypt the data in place.
    option_vector_t::iterator it = options->begin();
    for (; it != options->end(); ++it)
    {
        uint32_t entryStart = 0;
        uint32_t entryEnd = 0;
        const char *keyHex = NULL;
        const char *counterHex = NULL;

        // Ignore entry if not all options are present.
        if (!getOptionValues(**it, &keyHex, &counterHex, &entryStart, &entryEnd))
        {
            continue;
        }

        // Start address must exactly match region address.
        // Region addersses are already forced to modulo 1024.
        if (entryStart == start)
        {
            // Build counter value.
            std::string counterStr(counterHex);
            AESCounter<128> *counter = buildCounter(counterStr, start);

            // Build key value.
            AESKey<128> key;
            std::stringstream infoStream;
            infoStream << keyHex;
            key.readFromStream(infoStream);

            Log::log(Logger::INFO2, "creating encrypted range 0x%x len 0x%x\n", start, len);
            ++matchedCount;

            encrypt(len, data, &key, counter);
#if DEBUG_DATA
            qspiBin.write(reinterpret_cast<char *>(data), 0x400);
#endif

            delete counter;
            break; // only one entry is processed per base address
        }
    }

    return (matchedCount > 0);
}

void hexToBytes(const char *hexStr, unsigned char *bytes, int numBytes)
{
    for (int i = 0; i < numBytes; ++i)
    {
        char digitBuf[3];
        digitBuf[0] = *hexStr++;
        digitBuf[1] = *hexStr++;
        digitBuf[2] = '\0';

        bytes[i] = (unsigned char)strtoul(digitBuf, NULL, 16);
    }
}

void Keyblob::populateKeyBlob(keyblob_t *blob, uint32_t start, uint32_t end, const char *keyHex, const char *counterHex)
{
    // Populate key value.
    hexToBytes(keyHex, blob->key, sizeof(blob->key));

    // Poplulate Counter value.
    hexToBytes(counterHex, blob->ctr, sizeof(blob->ctr));

    blob->srtaddr = start & ~kRegionAddrMask;
    blob->endaddr = end | kRegionAddrMask;
    // Add default flags to endaddr register.
    blob->endaddr &= ~kFlagMask;
    blob->endaddr |= kKeyFlags;

    // Add CRC32.
    uint32_t result;
    CRC32 crc;
    crc.update(reinterpret_cast<uint8_t *>(blob), kCrc32SizeBytes);
    crc.truncatedFinal(reinterpret_cast<uint8_t *>(&result), sizeof(result));
    blob->key_blob_crc32 = result;
}

uint8_t *Keyblob::createWrappedKeyblobData(uint8_t *kek, uint32_t *byteCount)
{
    assert(kek);
    assert(byteCount);

    option_vector_t *options = getOptions();
    uint32_t entryCount = options->size();
    uint8_t *unwrappedData = new uint8_t[entryCount * sizeof(keyblob_t)];
    keyblob_t *blob = (keyblob_t *)&unwrappedData[0];
    uint8_t *wrappedData = new uint8_t[entryCount * sizeof(keyblob_t)];
    uint8_t *wrapped = wrappedData;

    // Create expanded KeK for AES128 encryption.
    unsigned int *expandedKek = new unsigned int[kExpandedKekSizeInts];
    KeyExpansion(reinterpret_cast<unsigned char *>(&kek[0]), kKeySizeBits, expandedKek);

    option_vector_t::iterator it = options->begin();
    for (; it != options->end(); ++it)
    {
        uint32_t start = 0;
        uint32_t end = 0;
        const char *keyHex = NULL;
        const char *counterHex = NULL;

        // If not all options are present, make it a null entry.
        bool isValidEntry = getOptionValues(**it, &keyHex, &counterHex, &start, &end);

        // Clear this wrapped entry.
        memset(wrapped, 0, kKeyBlobSizeBytes);

        if (isValidEntry)
        {
            // Fill in keyblob data.
            memset(blob, 0, sizeof(keyblob_t));
            populateKeyBlob(blob, start, end, keyHex, counterHex);

#if PRINT_KEYBLOB
            uint8_t *keyData = reinterpret_cast<uint8_t *>(blob);
            Log::log("    ;plaintext\n");
            for (int i = 0; i < kKeyBlobSizeBytes / kOutputLineSize; ++i)
            {
                Log::log("    ;   ");
                for (int j = 0; j < kOutputLineSize - 1; ++j)
                {
                    Log::log("0x%02x, ", *keyData++);
                }
                Log::log("0x%02x\n", *keyData++);
            }
#endif

            // Wrap keyblob entry.
            do_aes128_key_wrap(reinterpret_cast<uint8_t *>(blob), wrapped, expandedKek);

#if PRINT_KEYBLOB
            keyData = wrapped;
            Log::log("    ;wrapped\n");
            for (int i = 0; i < kKeyBlobSizeBytes / kOutputLineSize; ++i)
            {
                printf("    DC8 ");
                for (int j = 0; j < kOutputLineSize - 1; ++j)
                {
                    printf("0x%02x, ", *keyData++);
                }
                printf("0x%02x\n", *keyData++);
            }
#endif
        }

        // Next unwrapped and wrapped keyblob data pointers.
        ++blob;
        wrapped += sizeof(keyblob_t);
    }

    delete[] expandedKek;
    delete[] unwrappedData;

    *byteCount = entryCount * sizeof(keyblob_t);
    return wrappedData; // must be deleted by caller
}

bool Keyblob::getOptionValues(OptionContext &opt, const char **key, const char **ctr, uint32_t *start, uint32_t *end)
{
    assert(key && ctr && start && end);

    if (!(opt.hasOption(kKeyblobOptionNameStart) && opt.hasOption(kKeyblobOptionNameEnd) &&
          opt.hasOption(kKeyblobOptionNameKey) && opt.hasOption(kKeyblobOptionNameCounter)))
    {
        return false;
    }

    if (opt.hasOption(kKeyblobOptionNameStart))
    {
        const Value *value = opt.getOption(kKeyblobOptionNameStart);
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
        if (!intValue)
        {
            Log::log(Logger::WARNING, "invalid type for %s option\n", kKeyblobOptionNameStart);
            return false;
        }
        else
        {
            *start = intValue->getValue();
        }
    }

    if (opt.hasOption(kKeyblobOptionNameEnd))
    {
        const Value *value = opt.getOption(kKeyblobOptionNameEnd);
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
        if (!intValue)
        {
            Log::log(Logger::WARNING, "invalid type for %s option\n", kKeyblobOptionNameEnd);
            return false;
        }
        else
        {
            *end = intValue->getValue();
        }
    }

    if (opt.hasOption(kKeyblobOptionNameKey))
    {
        const Value *value = opt.getOption(kKeyblobOptionNameKey);
        const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
        if (!stringValue)
        {
            Log::log(Logger::WARNING, "invalid type for %s option\n", kKeyblobOptionNameKey);
            return false;
        }
        else
        {
            *key = *stringValue;
        }
    }

    if (opt.hasOption(kKeyblobOptionNameCounter))
    {
        const Value *value = opt.getOption(kKeyblobOptionNameCounter);
        const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
        if (!stringValue)
        {
            Log::log(Logger::WARNING, "invalid type for %s option\n", kKeyblobOptionNameCounter);
            return false;
        }
        else
        {
            *ctr = *stringValue;
        }
    }

    return true;
}
