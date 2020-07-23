/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "SB2Image.h"
#include <stdexcept>
#include <algorithm>
#include <time.h>
#include "crc.h"
#include "SHA1.h"
#include "Random.h"
#include "rijndael.h"
#include "RijndaelCBCMAC.h"
#include "Logging.h"
#include "EndianUtilities.h"
#include "format_string.h"
#define MBEDTLS_FS_IO
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/x509_csr.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/md.h"
#include "mbedtls/error.h"
#include "EncoreBootImage.h"
#include "aes128_key_wrap_unwrap.h"
#include "SearchPath.h"

using namespace elftosb;

//! Utility to return the byte length of a number of cipher blocks.
extern size_t sizeOfCipherBlocks(unsigned count);

//! Utility to return the number of cipher blocks required to hold an object
//! that is \a s bytes long.
extern size_t numberOfCipherBlocks(size_t s);

//! Utility to calculate the byte length for the cipher blocks required to hold
//! and object that is \a bytes long.
extern size_t sizeInCipherBlocks(size_t s);

//! Utility to return the number of bytes of padding required to fill out
//! the last cipher block in a set of cipher blocks large enough to hold
//! an object that is \a s bytes large. The result may be 0 if \a s is
//! an even multiple of the cipher block size.
extern size_t sizeOfPaddingForCipherBlocks(size_t s);

SB2Image::SB2Image()
    : m_headerFlags(0)
    , m_productVersion()
    , m_componentVersion()
    , m_buildNumber(0)
{
    m_signDataSection = NULL;
}

SB2Image::~SB2Image()
{
    // dispose of all sections
    section_iterator_t it = beginSection();
    for (; it != endSection(); ++it)
    {
        delete *it;
    }
//    if(m_signDataSection)
//        delete m_signDataSection;
}

//! \exception std::runtime_error Raised if \a newSection has the same tag as a previously
//!		added section.
void SB2Image::addSection(Section *newSection)
{
    // check for another section with this tag
    section_iterator_t it = beginSection();
    for (; it != endSection(); ++it)
    {
        if ((*it)->getIdentifier() == newSection->getIdentifier())
        {
            throw std::runtime_error("new section with non-unique tag");
        }
    }

    // no conflicting section tags, so add it
    m_sections.push_front(newSection);

    // tell the image who owns it now
    newSection->setImage(this);
}

SB2Image::section_iterator_t SB2Image::findSection(Section *section)
{
    return std::find(beginSection(), endSection(), section);
}

void SB2Image::setProductVersion(const version_t &version)
{
    m_productVersion = version;
}

void SB2Image::setComponentVersion(const version_t &version)
{
    m_componentVersion = version;
}

//! \todo Optimize writing section data. Right now it only writes one block at a
//!		time, which is of course quite slow (in relative terms).
//!	\todo Refactor this into several different methods for writing each region
//!		of the image. Use a context structure to keep track of shared data between
//!		each of the methods.
//! \todo Refactor the section and boot tag writing code to only have a single
//!		copy of the block writing and encryption loop.
void SB2Image::writeToStream(std::ostream &stream)
{
    // always generate the session key or DEK even if image is unencrypted
    int mbedtlsError = 0;
    mbedtls_md_context_t hmac_sha256_ctx;
    memset(m_digestHmac, 0, sizeof(m_digestHmac));
    
    // prepare to compute HMACs with the HMAC-SHA256 algorithm [5][6]. The MAC is generated over the entire header.
    if (isEncrypted())
    {
        mbedtls_md_init(&hmac_sha256_ctx);
        mbedtlsError = mbedtls_md_setup(&hmac_sha256_ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
        assert(mbedtlsError == 0);
        mbedtls_md_hmac_starts(&hmac_sha256_ctx, m_macKey, sizeof(AESKey<256>::key_t)); 
    }
    // prepare to compute SHA-256 digest over entire image
    mbedtls_sha256_context sha256_ctx;
    mbedtls_sha256_init(&sha256_ctx);
    mbedtls_sha256_starts(&sha256_ctx, false /*isSHA224 = false*/);

    // count of total blocks written to the file
    unsigned fileBlocksWritten = 0;

    // we need some pieces of the header down below
    sb2_header_t imageHeader;
    if (isSigned())
    {
        prepareCertBlockHeader();

        prepareSignDataSection();
    }
    prepareImageHeader(imageHeader);

    // write plaintext header
    {
        // write header
        assert(sizeOfPaddingForCipherBlocks(sizeof(sb2_header_t)) == 0);
        stream.write(reinterpret_cast<char *>(&imageHeader), sizeof(imageHeader));
        fileBlocksWritten += numberOfCipherBlocks(sizeof(imageHeader));

        if (isEncrypted())
        {
            // update H-MAC over image header
            mbedtls_md_hmac_update(&hmac_sha256_ctx, reinterpret_cast<uint8_t *>(&imageHeader), sizeof(imageHeader));
        }
         
        // update SHA-256
        mbedtls_sha256_update(&sha256_ctx, reinterpret_cast<uint8_t *>(&imageHeader), sizeof(imageHeader));
    }

    if (isEncrypted())
    {
        // finished with the HMAC over header
        mbedtls_md_hmac_finish(&hmac_sha256_ctx, m_digestHmac);
        // write HMAC result for this key, then update SHA-256
        stream.write(reinterpret_cast<const char *>(m_digestHmac), sizeof(m_digestHmac));
        // update SHA-256
        mbedtls_sha256_update(&sha256_ctx, reinterpret_cast<uint8_t *>(m_digestHmac), sizeof(m_digestHmac));

        fileBlocksWritten += numberOfCipherBlocks(sizeof(m_digestHmac));
    }
          
    // Add m_keyBlob - Wrap session key and MAC key using kek aes-ecb
    if (isEncrypted())
    {
        uint8_t plaintext[(2*256)/8];
        memset(plaintext, 0, sizeof(plaintext));
        uint8_t kek[256/8];
        memcpy(plaintext, (const unsigned char*)m_dek, 32);
        memcpy(plaintext+32, (const unsigned char*)m_macKey, 32);
        memset(m_keyBlob, 0, sizeof(m_keyBlob));
        memcpy(kek, (const unsigned char*)m_keys[0], 32);
        do_aes_key_wrap_N8(plaintext, m_keyBlob, kek);
        // write m_keyBlob to the stream, then update SHA256
        stream.write(reinterpret_cast<const char *>(m_keyBlob), sizeof(m_keyBlob));
        // update SHA-256
        mbedtls_sha256_update(&sha256_ctx, reinterpret_cast<uint8_t *>(m_keyBlob), sizeof(m_keyBlob));
        fileBlocksWritten += numberOfCipherBlocks(sizeof(m_keyBlob));
    }

    // write sections and boot tags
    {
        section_iterator_t it = beginSection();
        for (; it != endSection(); ++it)
        {
            section_iterator_t itCopy = it;
            bool isLastSection = (++itCopy == endSection());
            size_t offset;
            Section *section = *it;
            cipher_block_t block, streamBlock;
            unsigned blockCount = section->getBlockCount();
            unsigned blocksWritten = 0;
            mbedtls_aes_context ctxAES;
            uint32_t hmacSeqCount = section->getHmacEntries();
            uint32_t blockCountInHmacSeq = 0;
			uint32_t hmacSeqLen = 0;
			uint32_t lastHmacSeqLen = 0;
			if (hmacSeqCount != 0)
			{
				hmacSeqLen = blockCount / hmacSeqCount;
				lastHmacSeqLen = hmacSeqLen + (blockCount % hmacSeqCount);
			}
            uint32_t completedSeq = 0;
            uint32_t offsetToSectionData = 0;
            cipher_block_t * pEncryptedSectionBlocks = NULL;
            pEncryptedSectionBlocks = new cipher_block_t[blockCount];

            offset = 0;
            mbedtls_aes_init(&ctxAES);

            // Compute the number of padding blocks needed to align the section. This first
            // call to getPadBlockCountForOffset() passes an offset that excludes
            // the boot tag for this section.
            unsigned paddingBlocks = getPadBlockCountForSection(section, fileBlocksWritten);

            // Insert nop commands as padding to align the start of the section, if
            // the section has special alignment requirements.
            NopCommand nop;
            while (paddingBlocks--)
            {
                blockCount = nop.getBlockCount();
                blocksWritten = 0;
                while (blocksWritten < blockCount)
                {
                    nop.getBlocks(blocksWritten, 1, &block);

                    if (isEncrypted())
                    {
                        uint32_t nonce[4];
                        nonce[0] = imageHeader.nonce[0];
                        nonce[1] = imageHeader.nonce[1];
                        nonce[2] = imageHeader.nonce[2];
                        nonce[3] = imageHeader.nonce[3] + fileBlocksWritten;


                        mbedtls_aes_setkey_enc(&ctxAES, m_dek, sizeof(AESKey<256>::key_t) * 8);
                        mbedtls_aes_crypt_ctr(&ctxAES, sizeof(cipher_block_t), &offset, (uint8_t*)nonce, streamBlock,
                            block, block);
                    }

                    stream.write(reinterpret_cast<char *>(&block), sizeof(cipher_block_t));
                    // update SHA-256
                    mbedtls_sha256_update(&sha256_ctx, reinterpret_cast<uint8_t *>(&block), sizeof(block));

                    blocksWritten++;
                    fileBlocksWritten++;
                }
            }

            // write boot tag
            TagCommand tag(*section);
            tag.setLast(isLastSection);
            if (!isLastSection)
            {
                // If this isn't the last section, the tag needs to include any
                // padding for the next section in its length, otherwise the ROM
                // won't be able to find the next section's boot tag.
                unsigned nextSectionOffset = fileBlocksWritten + section->getBlockCount() + 1;
                tag.setSectionLength(section->getBlockCount() + getPadBlockCountForSection(*itCopy, nextSectionOffset));
            }
            blockCount = tag.getBlockCount();
            blocksWritten = 0;

            // Prepare for the hmac of boot tag section header
            if (isEncrypted())
            {
                mbedtls_md_init(&hmac_sha256_ctx);
                mbedtlsError = mbedtls_md_setup(&hmac_sha256_ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
                assert(mbedtlsError == 0);
                mbedtls_md_hmac_starts(&hmac_sha256_ctx, m_macKey, sizeof(AESKey<256>::key_t));
            }

            while (blocksWritten < blockCount)
            {
                tag.getBlocks(blocksWritten, 1, &block);

                if (isEncrypted())
                {
                    uint32_t nonce[4];
                    nonce[0] = imageHeader.nonce[0];
                    nonce[1] = imageHeader.nonce[1];
                    nonce[2] = imageHeader.nonce[2];
                    nonce[3] = imageHeader.nonce[3] + fileBlocksWritten;

                    // re-init after encrypt to update IV
                    mbedtls_aes_setkey_enc(&ctxAES, m_dek, sizeof(AESKey<256>::key_t) * 8);
                    mbedtls_aes_crypt_ctr(&ctxAES, sizeof(cipher_block_t), &offset, (uint8_t*)nonce, streamBlock,
                        block, block);
                }

                stream.write(reinterpret_cast<char *>(&block), sizeof(cipher_block_t));
                // update SHA-256
                mbedtls_sha256_update(&sha256_ctx, reinterpret_cast<uint8_t *>(&block), sizeof(block));

                if (isEncrypted())
                {
                    // update H-MAC over the boot tag
                    mbedtls_md_hmac_update(&hmac_sha256_ctx, block, sizeof(cipher_block_t));
                }

                blocksWritten++;
                fileBlocksWritten++;
            }

            if (isEncrypted())
            {
                // finished with the HMAC over boot tag
                memset(m_digestHmac, 0, sizeof(m_digestHmac));

                mbedtls_md_hmac_finish(&hmac_sha256_ctx, m_digestHmac);
                // write HMAC result for the boot tag, then update SHA-256
                stream.write(reinterpret_cast<const char *>(m_digestHmac), sizeof(m_digestHmac));
                // update SHA-256
                mbedtls_sha256_update(&sha256_ctx, reinterpret_cast<uint8_t *>(m_digestHmac), sizeof(m_digestHmac));

                fileBlocksWritten += numberOfCipherBlocks(sizeof(m_digestHmac));
            }
            // reinit cipher for section data

            // write section data hmacs
            blockCount = section->getBlockCount();
            blocksWritten = 0;
            offsetToSectionData = fileBlocksWritten +
                (hmacSeqCount * numberOfCipherBlocks(sizeof(m_digestHmac)));
            blockCountInHmacSeq = 0;
            while (blocksWritten < blockCount)
            {
                if ((blockCountInHmacSeq == 0) && isEncrypted())
                {
                    mbedtls_md_init(&hmac_sha256_ctx);
                    mbedtlsError = mbedtls_md_setup(&hmac_sha256_ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
                    assert(mbedtlsError == 0);
                    mbedtls_md_hmac_starts(&hmac_sha256_ctx, m_macKey, sizeof(AESKey<256>::key_t));
                }
     
                section->getBlocks(blocksWritten, 1, &block);

                // Only encrypt the section contents if the entire boot image is encrypted
                // and the section doesn't have the "leave unencrypted" flag set. Even if the
                // section is unencrypted the boot tag will remain encrypted.
                if (isEncrypted() && (!section->getLeaveUnencrypted()))
                {
                    // re-init after encrypt to update IV
                    uint32_t nonce[4];
                    nonce[0] = imageHeader.nonce[0];
                    nonce[1] = imageHeader.nonce[1];
                    nonce[2] = imageHeader.nonce[2];
                    nonce[3] = imageHeader.nonce[3] + offsetToSectionData + blocksWritten;
                    
                    mbedtls_aes_setkey_enc(&ctxAES, m_dek, sizeof(AESKey<256>::key_t) * 8);
                    mbedtls_aes_crypt_ctr(&ctxAES, sizeof(cipher_block_t), &offset, (uint8_t*)nonce, streamBlock,
                        block, block);
                }
                // Save the block to be written later
                memcpy(reinterpret_cast<uint8_t *>(pEncryptedSectionBlocks[blocksWritten]), reinterpret_cast<uint8_t *>(&block), sizeof(block));

                blocksWritten++;

                if (isEncrypted())
                {
                    // update H-MAC over the boot tag
                    mbedtls_md_hmac_update(&hmac_sha256_ctx, block, sizeof(cipher_block_t));

                    blockCountInHmacSeq++;

                    if (blockCountInHmacSeq >= hmacSeqLen || blocksWritten >= blockCount)
                    {
                        memset(m_digestHmac, 0, sizeof(m_digestHmac));

                        // finished with the HMAC over section data for hmacSeqLen
                        mbedtls_md_hmac_finish(&hmac_sha256_ctx, m_digestHmac);
                        // write HMAC result for the section data, then update SHA-256
                        stream.write(reinterpret_cast<const char *>(m_digestHmac), sizeof(m_digestHmac));
                        // update SHA-256
                        mbedtls_sha256_update(&sha256_ctx, reinterpret_cast<uint8_t *>(m_digestHmac), sizeof(m_digestHmac));

                        fileBlocksWritten += numberOfCipherBlocks(sizeof(m_digestHmac));
                        blockCountInHmacSeq = 0;
                        completedSeq ++;

                        // if we are in the last sequence
                        if (completedSeq == (hmacSeqCount - 1))
                            hmacSeqLen = lastHmacSeqLen;

                    }
                }
            }
            // Write section data
            for(int i=0; i<blockCount; i++)
            {
                stream.write(reinterpret_cast<char *>(pEncryptedSectionBlocks[i]), sizeof(cipher_block_t));
                // update SHA-256
                mbedtls_sha256_update(&sha256_ctx, reinterpret_cast<uint8_t *>(pEncryptedSectionBlocks[i]), sizeof(cipher_block_t));
                fileBlocksWritten ++;
            }
            delete [] pEncryptedSectionBlocks;
        }
    }

    // write signature of SHA-256 digest over entire image
    if(isSigned())
    {
        int ret;
        int cert_idx = m_certBlockHeader.certificateCount - 1;
        // allocate enough room for digest and bytes to pad out to the next cipher block
        const unsigned padBytes = sizeOfPaddingForCipherBlocks(sizeof(sha256_digest_t));
        unsigned digestBlocksSize = sizeof(sha256_digest_t) + padBytes;
        smart_array_ptr<uint8_t> digestBlocks = new uint8_t[digestBlocksSize];

        mbedtls_sha256_finish(&sha256_ctx, digestBlocks);

        // Load and parse the certificate chain
        mbedtls_x509_crt cert_chain;
        mbedtls_x509_crt_init(&cert_chain);
        if((ret = mbedtls_x509_crt_parse_der(&cert_chain, (const unsigned char *)m_certTable.entries[cert_idx].x509Certificate,
                m_certTable.entries[cert_idx].x509CertificateLengthInBytes)) != 0)
        {
            Log::log(Logger::ERROR, "error: certificate parse failed: %d\n", ret);
            return;
        }
        // FM:todo differentiate between CA and certificate for signing the code
        // assuming the last certificate to be the code signing certificate - need to fix this

        // Get the public key from the certificate.
        mbedtls_rsa_context *rsa_pub_key_ctx = (mbedtls_rsa_context *)(cert_chain.pk.pk_ctx);

        mbedtls_pk_context pk_ctx;
        mbedtls_pk_init(&pk_ctx);

        //FM:todo handle password protected private files

        if ((ret = mbedtls_pk_parse_keyfile(&pk_ctx, m_privKeyPath, NULL)) != 0)
        {
            Log::log(Logger::ERROR, "error: private key parse failed: %d\n", ret);
            return;
        }
        // Get the private key.
        mbedtls_rsa_context *rsa_priv_key_ctx = (mbedtls_rsa_context *)(pk_ctx.pk_ctx);

        // Generate the RSA signature
        // Init deterministic random bit generator.
        mbedtls_ctr_drbg_context ctr_drbg;
        mbedtls_ctr_drbg_init(&ctr_drbg);
        // Init entropy.
        mbedtls_entropy_context entropy;
        mbedtls_entropy_init(&entropy);
        uint32_t signatureSizeInBytes = rsa_priv_key_ctx->len;

        const unsigned padding = sizeOfPaddingForCipherBlocks(signatureSizeInBytes);
        unsigned signatureBlocksSize = signatureSizeInBytes + padding;
        smart_array_ptr<uint8_t> signatureBlocks = new uint8_t[signatureSizeInBytes];


        if((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0)) != 0)
        {
            Log::log(Logger::ERROR, "error: Seed random bit generator failed: %d\n", ret);
            return;
        }

        if ((ret = mbedtls_rsa_rsassa_pkcs1_v15_sign(rsa_priv_key_ctx, mbedtls_ctr_drbg_random, &ctr_drbg, MBEDTLS_RSA_PRIVATE,
            MBEDTLS_MD_SHA256, 256, digestBlocks.get(), signatureBlocks)) != 0)
        {
            Log::log(Logger::ERROR, "error: Signature generation failed: %d\n", ret);
            return;
        }

        // Verify the signature

        if ((ret = mbedtls_rsa_rsassa_pkcs1_v15_verify(rsa_pub_key_ctx, mbedtls_ctr_drbg_random, &ctr_drbg, MBEDTLS_RSA_PUBLIC,
            MBEDTLS_MD_SHA256, 256, digestBlocks.get(), signatureBlocks.get())) != 0)
        {
            Log::log(Logger::ERROR, "error: Signature verification failed: %d\n", ret);
            return;
        }


        // set the pad bytes to random values
        RandomNumberGenerator rng;
        rng.generateBlock(&(signatureBlocks.get())[signatureSizeInBytes], padding);

        // write to the stream
        stream.write(reinterpret_cast<char *>(signatureBlocks.get()), signatureSizeInBytes);
    }
}

void SB2Image::prepareCertBlockHeader(void)
{
    m_certBlockHeader.signature[0] = 'c';
    m_certBlockHeader.signature[1] = 'e';
    m_certBlockHeader.signature[2] = 'r';
    m_certBlockHeader.signature[3] = 't';
    m_certBlockHeader.headerMajorVersion = ENDIAN_HOST_TO_LITTLE_U32(1);
    m_certBlockHeader.headerMinorVersion = 0;
    m_certBlockHeader.headerLengthInBytes = ENDIAN_HOST_TO_LITTLE_U32(sizeof(certificate_block_header_t));
    m_certBlockHeader.flags = 0;
    m_certBlockHeader.buildNumber = ENDIAN_HOST_TO_LITTLE_U32(m_buildNumber);
    m_certBlockHeader.totalImageLengthInBytes = 0; //!< It will be updated later
    m_certBlockHeader.certificateCount = ENDIAN_HOST_TO_LITTLE_U32(m_certFilePaths.size());
    m_certBlockHeader.certificateTableLengthInBytes = 0; // It will be updated later
}

void SB2Image::prepareSignDataSection()
{
    int rootKeyCertIndex = 0;
    uint32_t tag = 's'|('i'<<8)|('g'<<16)|('n'<<24);
    
    m_signDataSection = new SB2Image::DataSection(tag);
    
    m_signDataSection->setLeaveUnencrypted(TRUE);
    m_signDataSection->setHmacEntries(1);
    m_certTable.entries = new cert_entry_t[m_certBlockHeader.certificateCount];

    uint32_t size_tracker=0;

    // copy certificates
    string_vector_t::iterator it = m_certFilePaths.begin();
    for (int i=0; it != m_certFilePaths.end(); ++it, i++)
    {
        try {
            std::string &certPath = *it;

            std::string actualPath;
            bool found = PathSearcher::getGlobalSearcher().search(certPath, PathSearcher::kFindFile, true, actualPath);
            if (!found)
            {
                throw std::runtime_error(format_string("unable to find cert file %s\n", certPath.c_str()));
            }

            FILE * certFile = NULL;
            certFile = fopen(actualPath.c_str(), "rb");
            if (certFile == NULL)
            {
                throw std::runtime_error(format_string("unable to open cert file %s\n", certPath.c_str()));
            }

            // Get CRT file length.
            fseek(certFile, 0, SEEK_END);
            uint32_t certSize = ftell(certFile);
            uint32_t alignedCertSize = ((certSize % 4) ? certSize + (4 - (certSize % 4)) : certSize);
            fseek(certFile, 0, SEEK_SET);
            size_tracker += sizeof(uint32_t) + alignedCertSize;

            m_certTable.entries[i].x509CertificateLengthInBytes = alignedCertSize;
            m_certTable.entries[i].x509Certificate = new uint8_t[alignedCertSize];
            memset(m_certTable.entries[i].x509Certificate, 0, alignedCertSize);
            fread(m_certTable.entries[i].x509Certificate, 1, certSize, certFile);
            
            if(certFile != NULL)
                fclose(certFile);

        }
        catch (std::exception &e)
        {
            Log::log(Logger::ERROR, "error: %s\n", e.what());
            return;
        }
        catch (...)
        {
            Log::log(Logger::ERROR, "error: unexpected exception\n");
            return;
        }
    }
    uint32_t certTableLength = size_tracker;
    // generate root key hash table from root key certificates
    for (int i=0; i<MAX_ROOT_KEY_HASH_TABLE_SIZE; i++)
    {
        size_tracker += sizeof(sha256_digest_t);
        memset(m_rkhTable.entries[i].rkh, 0, sizeof(sha256_digest_t));
    }
    {
        string_vector_t::iterator it = m_rootKeyCertFilePaths.begin();
        for (rootKeyCertIndex = 0; it != m_rootKeyCertFilePaths.end(); ++it, rootKeyCertIndex++)
        {
            if (rootKeyCertIndex >= MAX_ROOT_KEY_HASH_TABLE_SIZE)
            {
                // We are done;
                break;
            }
            try {
                std::string &certPath = *it;

                std::string actualPath;
                bool found = PathSearcher::getGlobalSearcher().search(certPath, PathSearcher::kFindFile, true, actualPath);
                if (!found)
                {
                    throw std::runtime_error(format_string("unable to find cert file %s\n", certPath.c_str()));
                }

                // Load and parse the certificate chain
                int ret;
                mbedtls_x509_crt cert_chain;
                mbedtls_x509_crt_init(&cert_chain);
                if ((ret = mbedtls_x509_crt_parse_file(&cert_chain, (const char *)certPath.c_str())) != 0)
                {
                    Log::log(Logger::ERROR, "error: certificate parse failed: %d\n", ret);
                    return;
                }

                // Get the public key from the certificate.
                mbedtls_rsa_context *rsa_pub_key_ctx = (mbedtls_rsa_context *)(cert_chain.pk.pk_ctx);
                mbedtls_sha256_context rootKeySha256_ctx;
                mbedtls_sha256_init(&rootKeySha256_ctx);
                mbedtls_sha256_starts(&rootKeySha256_ctx, false /*isSHA224 = false*/);
                uint8_t * modulus = new uint8_t[mbedtls_mpi_size(&rsa_pub_key_ctx->N)];

                uint8_t * exponent = new uint8_t[mbedtls_mpi_size(&rsa_pub_key_ctx->E)];

                mbedtls_mpi_write_binary(&rsa_pub_key_ctx->N, modulus, mbedtls_mpi_size(&rsa_pub_key_ctx->N));
                mbedtls_mpi_write_binary(&rsa_pub_key_ctx->E, exponent, mbedtls_mpi_size(&rsa_pub_key_ctx->E));

                // Generate hash for Modulus
                mbedtls_sha256_update(&rootKeySha256_ctx, modulus, mbedtls_mpi_size(&rsa_pub_key_ctx->N));
            
                // Generate hash for Exponent
                mbedtls_sha256_update(&rootKeySha256_ctx, exponent, mbedtls_mpi_size(&rsa_pub_key_ctx->E));

                mbedtls_sha256_finish(&rootKeySha256_ctx, m_rkhTable.entries[rootKeyCertIndex].rkh);
            }
            catch (std::exception &e)
            {
                Log::log(Logger::ERROR, "error: %s\n", e.what());
                return;
            }
            catch (...)
            {
                Log::log(Logger::ERROR, "error: unexpected exception\n");
                return;
            }
        }
    }
    // update certificateTableLengthInBytes of certificate block header
    m_certBlockHeader.certificateTableLengthInBytes = ENDIAN_HOST_TO_LITTLE_U32(certTableLength);
    
    size_tracker += sizeof(m_certBlockHeader);

    m_signDataSection->setDataNoCopy(NULL, size_tracker);

    // add section to the section list
    addSection(m_signDataSection);

    // update the cert header with image size and overwrite the header in the section bytes
    m_certBlockHeader.totalImageLengthInBytes = ENDIAN_HOST_TO_LITTLE_U32(getImageSize() * sizeof(cipher_block_t));

    {
        // prepare data for the section
        uint8_t *data = new uint8_t[size_tracker];
        memset(data, 0, size_tracker);
        uint8_t *t=data;
        memcpy(t, reinterpret_cast<uint8_t *>(&m_certBlockHeader), sizeof(certificate_block_header_t));
        t += sizeof(m_certBlockHeader);
        for(int i=0; i<m_certBlockHeader.certificateCount; i++)
        {
            memcpy(t, reinterpret_cast<uint8_t *>(&m_certTable.entries[i].x509CertificateLengthInBytes), sizeof(uint32_t));
            t += sizeof(uint32_t);
            memcpy(t, m_certTable.entries[i].x509Certificate, m_certTable.entries[i].x509CertificateLengthInBytes);
            t += m_certTable.entries[i].x509CertificateLengthInBytes;
        }

        sha256_digest_t hashOfHashes;
        mbedtls_sha256_context hashOfHashes_ctx;
        mbedtls_sha256_init(&hashOfHashes_ctx);
        mbedtls_sha256_starts(&hashOfHashes_ctx, false /*isSHA224 = false*/);

        for (int i = 0; i<MAX_ROOT_KEY_HASH_TABLE_SIZE; i++)
        {
            memcpy(t, m_rkhTable.entries[i].rkh, sizeof(sha256_digest_t));
            t += sizeof(sha256_digest_t);
            // Generate hash of hash
            mbedtls_sha256_update(&hashOfHashes_ctx, m_rkhTable.entries[i].rkh, sizeof(sha256_digest_t));
        }

        // Set data for the sign section
        m_signDataSection->setData(data, size_tracker);

        mbedtls_sha256_finish(&hashOfHashes_ctx, hashOfHashes);

        // Save hashOfHashes in the output m_hashFilePath
        FILE * hashFile = NULL;
        hashFile = fopen(m_hashFilePath, "wb");
        if (hashFile == NULL)
        {
            throw std::runtime_error(format_string("unable to open otuput hash file %s\n", m_hashFilePath));
        }            
        if (fwrite(hashOfHashes, 1, sizeof(sha256_digest_t), hashFile) != sizeof(sha256_digest_t))
        {
            int err = ferror(hashFile);
            throw std::runtime_error(
                format_string("failed to write data to the file \"%s\" (%d)", m_hashFilePath, err));
        }

        fclose(hashFile);

        delete [] data;
    }
}

void SB2Image::prepareImageHeader(sb2_header_t &header)
{
    // get identifier for the first bootable section
    Section *firstBootSection = findFirstBootableSection();
    section_id_t firstBootSectionID = 0;
    RandomNumberGenerator rng;

    if (firstBootSection)
    {
        firstBootSectionID = firstBootSection->getIdentifier();
    }

    // fill in header fields
    rng.generateBlock((uint8_t*)header.nonce, sizeof(header.nonce));
    // clear nonce bytes at offsets 31 and 63
    header.nonce[2] &= ~0x80000000;
    header.nonce[3] &= ~0x80000000;
    header.reserved = 0;
    header.m_signature[0] = 'S';
    header.m_signature[1] = 'T';
    header.m_signature[2] = 'M';
    header.m_signature[3] = 'P';
    header.m_majorVersion = ROM_BOOT_IMAGE_MAJOR_VERSION;
    header.m_minorVersion = ROM_BOOT_IMAGE_MINOR_VERSION;
    header.m_flags = ENDIAN_HOST_TO_LITTLE_U16(m_headerFlags);
    
    header.m_imageBlocks = ENDIAN_HOST_TO_LITTLE_U32(getImageSize());
    header.m_firstBootableSectionID = ENDIAN_HOST_TO_LITTLE_U32(firstBootSectionID);
    header.m_headerBlocks = ENDIAN_HOST_TO_LITTLE_U16((uint16_t)numberOfCipherBlocks(sizeof(header)));
    header.m_signature2[0] = 's';
    header.m_signature2[1] = 'g';
    header.m_signature2[2] = 't';
    header.m_signature2[3] = 'l';
    header.m_timestamp = ENDIAN_HOST_TO_LITTLE_U64(getTimestamp());

    // Prepare version fields by converting them to the correct byte order.
    header.m_productVersion = m_productVersion;
    header.m_componentVersion = m_componentVersion;
    header.m_productVersion.fixByteOrder();
    header.m_componentVersion.fixByteOrder();
    header.m_buildNumber = m_buildNumber;
    if(isSigned())
    {
        header.m_offsetToCertificateBlockInBytes = ENDIAN_HOST_TO_LITTLE_U32(
            (getSectionOffset(m_signDataSection) * sizeof(cipher_block_t)) + // offset to sign data section past the tag block
            (sizeof(m_digestHmac)) * 2 );  // size of hmac table, only two entries for sign section
    }
    else
    {
        header.m_offsetToCertificateBlockInBytes = 0;
    }
    header.m_keyBlobBlock = ENDIAN_HOST_TO_LITTLE_U32(getKeyBlobBlock());
    header.m_keyBlobBlockCount = ENDIAN_HOST_TO_LITTLE_U32(getKeyBlobBlockCount());
    header.m_maxSectionMacCount = ENDIAN_HOST_TO_LITTLE_U16(getMaxSectionMacCount());
    // the fields are dependant on others
    header.m_firstBootTagBlock = ENDIAN_HOST_TO_LITTLE_U32(getFirstBootTagBlock());

    // generate random pad bytes
    rng.generateBlock(header.m_padding1, sizeof(header.m_padding1));
}

//! Returns the number of microseconds since 00:00 1-1-2000. In actuality, the timestamp
//! is only accurate to seconds, and is simply extended out to microseconds.
//!
//! \todo Use the operating system's low-level functions to get a true microsecond
//!		timestamp, instead of faking it like we do now.
//! \bug The timestamp might be off an hour.
uint64_t SB2Image::getTimestamp()
{
#if WIN32
    struct tm epoch = { 0, 0, 0, 1, 0, 100, 0, 0 }; // 00:00 1-1-2000
#else
    struct tm epoch = { 0, 0, 0, 1, 0, 100, 0, 0, 1, 0, NULL }; // 00:00 1-1-2000
#endif
    time_t epochTime = mktime(&epoch);
    time_t now = time(NULL);
    now -= epochTime;
    uint64_t microNow = uint64_t(now) * 1000000; // convert to microseconds
    return microNow;
}

uint32_t SB2Image::getKeyBlobBlock()
{
    if (isEncrypted())
    {
        const uint32_t headerBlocks = numberOfCipherBlocks(sizeof(sb2_header_t));
        uint32_t keyBlobBlock = headerBlocks;
        keyBlobBlock += numberOfCipherBlocks(sizeof(m_digestHmac));

        return keyBlobBlock;
    }
    return 0;
}

uint32_t SB2Image::getKeyBlobBlockCount()
{
    if (isEncrypted())
    {
        return numberOfCipherBlocks(sizeof(m_keyBlob));
    }
    return 0;
}

uint16_t SB2Image::getMaxSectionMacCount()
{
    // add up sections before this one
    section_iterator_t it = beginSection();
    uint16_t SectionMacCount = 0;
    for (; it != endSection(); ++it)
    {
        Section *thisSection = *it;

        // add hmac entries
        SectionMacCount += thisSection->getHmacEntries();
    }

    return SectionMacCount;
}

uint32_t SB2Image::getFirstBootTagBlock()
{
    SB2Image::Section *section = findFirstBootableSection();
    
    return (getSectionOffset(section) - 1); // offset is past the tag block, hence -1 to include tag
}

//! Scans the section list looking for the first section which has
//! the #ROM_SECTION_BOOTABLE flag set on it.
SB2Image::Section *SB2Image::findFirstBootableSection()
{
    section_iterator_t it = beginSection();
    for (; it != endSection(); ++it)
    {
        if ((*it)->getFlags() & ROM_SECTION_BOOTABLE)
        {
            return *it;
        }
    }

    // no bootable sections were found
    return NULL;
}

//! The boot tag for \a section is taken into account, thus making the
//! result offset point to the first block of the actual section data.
//!
//! \note The offset will only be valid if all encryption keys and all
//! sections have already been added to the image.
uint32_t SB2Image::getSectionOffset(Section *section)
{
    // start with boot image headers
    uint32_t offset = numberOfCipherBlocks(sizeof(sb2_header_t));       // header
    if(isEncrypted())
    {
        offset += numberOfCipherBlocks(sizeof(m_digestHmac));
        offset += numberOfCipherBlocks(sizeof(m_keyBlob));
    }
    // add up sections before this one
    section_iterator_t it = beginSection();
    for (; it != endSection() && *it != section; ++it)
    {
        Section *thisSection = *it;

        // insert padding for section alignment
        offset += getPadBlockCountForSection(thisSection, offset);

        // add one for boot tag associated with this section
        offset++;

        // add hmac entries
        offset += ((thisSection->getHmacEntries() + 1) * numberOfCipherBlocks(sizeof(m_digestHmac)));

        // now add the section's contents
        offset += thisSection->getBlockCount();
    }

    // and add padding for this section
    offset += getPadBlockCountForSection(section, offset);

    // skip over this section's boot tag
    offset++;

    return offset;
}

//! Computes the number of blocks of padding required to align \a section while
//! taking into account the boot tag that gets inserted before the section contents.
unsigned SB2Image::getPadBlockCountForSection(Section *section, unsigned offset)
{
    // Compute the number of padding blocks needed to align the section. This first
    // call to getPadBlockCountForOffset() passes an offset that excludes
    // the boot tag for this section.
    unsigned paddingBlocks = section->getPadBlockCountForOffset(offset);

    // If the pad count comes back as 0 then we need to try again with an offset that
    // includes the boot tag. This is all because we're aligning the section contents
    // start and not the section's boot tag.
    if (paddingBlocks == 0)
    {
        paddingBlocks = section->getPadBlockCountForOffset(offset + 1);
    }
    // Otherwise if we get a nonzero pad amount then we need to subtract the block
    // for the section's boot tag from the pad count.
    else
    {
        paddingBlocks--;
    }

    return paddingBlocks;
}

uint32_t SB2Image::getImageSize()
{

    // determine to total size of the image
    const uint32_t headerBlocks = numberOfCipherBlocks(sizeof(sb2_header_t));
    uint32_t imageBlocks = headerBlocks;
    if (isEncrypted())
    {
        imageBlocks += numberOfCipherBlocks(sizeof(m_digestHmac));
        imageBlocks += numberOfCipherBlocks(sizeof(m_keyBlob));
    }  
    // add in each section's size
    section_iterator_t it = beginSection();
    for (; it != endSection(); ++it)
    {
        // add in this section's size, padding to align it, and its boot tag
        imageBlocks += getPadBlockCountForSection(*it, imageBlocks);
        imageBlocks += (*it)->getBlockCount();
        imageBlocks++;
        // add hmac entries
        imageBlocks += (((*it)->getHmacEntries() + 1) * numberOfCipherBlocks(sizeof(m_digestHmac)));

    }

    return imageBlocks;
}

void SB2Image::debugPrint() const
{
    const_section_iterator_t it = beginSection();
    for (; it != endSection(); ++it)
    {
        const Section *section = *it;
        section->debugPrint();
    }
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \return A new boot command instance.
//! \retval NULL The boot command pointed to by \a blocks was not recognized as a known
//!     command type.
//!
//! \exception std::runtime_error This exception indicates that a command was recognized
//!     but contained invalid data. Compare this to a NULL result which indicates that
//!     no command was recognized at all.
SB2Image::BootCommand *SB2Image::BootCommand::createFromData(const cipher_block_t *blocks,
                                                                           unsigned count,
                                                                           unsigned *consumed)
{
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    BootCommand *command = NULL;

    switch (header->m_tag)
    {
        case ROM_NOP_CMD:
            command = new NopCommand();
            break;
        case ROM_TAG_CMD:
            command = new TagCommand();
            break;
        case ROM_LOAD_CMD:
            command = new LoadCommand();
            break;
        case ROM_FILL_CMD:
            command = new FillCommand();
            break;
        case ROM_MODE_CMD:
            command = new ModeCommand();
            break;
        case ROM_JUMP_CMD:
            command = new JumpCommand();
            break;
        case ROM_CALL_CMD:
            command = new CallCommand();
            break;
        case ROM_ERASE_CMD:
            command = new EraseCommand();
            break;
        case ROM_RESET_CMD:
            command = new ResetCommand();
            break;
        case ROM_MEM_ENABLE_CMD:
            command = new MemEnableCommand();
            break;
        case ROM_PROG_CMD:
            command = new ProgramCommand();
            break;
        default:
            throw std::runtime_error(format_string("unrecognized boot command tag 0x%02x", header->m_tag));
    }

    if (command)
    {
        command->initFromData(blocks, count, consumed);
    }
    return command;
}

//! The checksum algorithm is totally straightforward, except that the
//! initial checksum byte value is set to 0x5a instead of 0.
uint8_t SB2Image::BootCommand::calculateChecksum(const boot_command_t &header)
{
    const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&header);
    uint8_t checksum = 0x5a;
    int i;

    // start at one to skip checksum field
    for (i = 1; i < sizeof(header); ++i)
    {
        checksum += bytes[i];
    }

    return checksum;
}

//! The default implementation returns 0, indicating that no blocks are
//! available.
unsigned SB2Image::BootCommand::getBlockCount() const
{
    return 1 + getDataBlockCount();
}

//! Up to \a maxCount cipher blocks are copied into the buffer pointed to by
//! the \a data argument. The index of the first block to copy is
//! held in the \a offset argument.
//!
//! \param offset Starting block number to copy. Zero means the first available block.
//! \param maxCount Up to this number of blocks may be copied into \a data. Must be 1 or greater.
//! \param data Buffer for outgoing cipher blocks. Must have enough room to hold
//!		\a maxCount blocks.
//!
//! \return The number of cipher blocks copied into \a data.
//! \retval 0 No more blocks are available and nothing was written to \a data.
//!
//! \exception std::out_of_range If \a offset is invalid.
unsigned SB2Image::BootCommand::getBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data)
{
    assert(data);
    assert(maxCount >= 1);

    // check for valid offset
    if (offset >= getBlockCount())
    {
        throw std::out_of_range("invalid offset");
    }

    // handle the command header block separately
    if (offset == 0)
    {
        assert(sizeof(boot_command_t) == sizeof(cipher_block_t));

        boot_command_t header;
        fillCommandHeader(header);
        memcpy(data, &header, sizeof(header));

        return 1;
    }

    // handle any data blocks
    return getDataBlocks(offset - 1, maxCount, data);
}

//! The checksum field of \a testHeader is always computed and checked against itself.
//! All other fields are compared to the corresponding value set in \a modelHeader
//! if the appropriate flag is set in \a whichFields. For example, the m_address fields
//! in \a testHeader and \a modelHeader are compared when the CMD_ADDRESS_FIELD bit
//! is set in \a whichFields. An exception is thrown if any comparison fails.
//!
//! \param modelHeader The baseline header to compare against. Only those fields that
//!		have corresponding bits set in \a whichFields need to be set.
//! \param testHeader The actual command header which is being validated.
//! \param whichFields A bitfield used to determine which fields of the boot command
//!		header are compared. Possible values are:
//!			- CMD_TAG_FIELD
//!			- CMD_FLAGS_FIELD
//!			- CMD_ADDRESS_FIELD
//!			- CMD_COUNT_FIELD
//!			- CMD_DATA_FIELD
//!
//! \exception std::runtime_error Thrown if any requested validation fails.
void SB2Image::BootCommand::validateHeader(const boot_command_t *modelHeader,
                                                  const boot_command_t *testHeader,
                                                  unsigned whichFields)
{
    // compare all the fields that were requested
    if ((whichFields & CMD_TAG_FIELD) && (testHeader->m_tag != modelHeader->m_tag))
    {
        throw std::runtime_error("invalid tag field");
    }

    if ((whichFields & CMD_FLAGS_FIELD) && (testHeader->m_flags != modelHeader->m_flags))
    {
        throw std::runtime_error("invalid flags field");
    }

    if ((whichFields & CMD_ADDRESS_FIELD) && (testHeader->m_address != modelHeader->m_address))
    {
        throw std::runtime_error("invalid address field");
    }

    if ((whichFields & CMD_COUNT_FIELD) && (testHeader->m_count != modelHeader->m_count))
    {
        throw std::runtime_error("invalid count field");
    }

    if ((whichFields & CMD_DATA_FIELD) && (testHeader->m_data != modelHeader->m_data))
    {
        throw std::runtime_error("invalid data field");
    }

    // calculate checksum
    uint8_t testChecksum = calculateChecksum(*testHeader);
    if (testChecksum != testHeader->m_checksum)
    {
        throw std::runtime_error("invalid checksum");
    }
}

//! Since the NOP command has no data, this method just validates the command header.
//! All fields except the checksum are expected to be set to 0.
//!
//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error Thrown if header fields are invalid.
void SB2Image::NopCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    const boot_command_t model = { 0, ROM_NOP_CMD, 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header,
                   CMD_TAG_FIELD | CMD_FLAGS_FIELD | CMD_ADDRESS_FIELD | CMD_COUNT_FIELD | CMD_DATA_FIELD);

    *consumed = 1;
}

//! All fields of the boot command header structure are set to 0, except
//! for the checksum. This includes the tag field since the tag value for
//! the #ROM_NOP_CMD is zero. And since all fields are zeroes the checksum
//! remains the initial checksum value of 0x5a.
void SB2Image::NopCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    header.m_flags = 0;
    header.m_address = 0;
    header.m_count = 0;
    header.m_data = 0;
    header.m_checksum = calculateChecksum(header); // do this last
}

void SB2Image::NopCommand::debugPrint() const
{
    Log::log(Logger::INFO2, "\tNOOP\n");
}

//! The identifier, length, and flags fields are taken from \a section.
//!
//! \todo How does length get set correctly if the length is supposed to include
//!		this command?
SB2Image::TagCommand::TagCommand(const Section &section)
{
    m_sectionIdentifier = section.getIdentifier();
    m_sectionLength = section.getBlockCount();
    m_sectionFlags = section.getFlags() | (m_isLast ? ROM_LAST_TAG : 0);
    m_hmacEntries = section.getHmacEntries();
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error Thrown if header fields are invalid.
void SB2Image::TagCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    const boot_command_t model = { 0, ROM_TAG_CMD, 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header, CMD_TAG_FIELD);

    // read fields from header
    m_isLast = (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_LAST_TAG) != 0;
    m_sectionIdentifier = ENDIAN_LITTLE_TO_HOST_U32(header->m_address);
    m_sectionLength = ENDIAN_LITTLE_TO_HOST_U32(header->m_count);
    m_sectionFlags = ENDIAN_LITTLE_TO_HOST_U16(header->m_flags);
    m_hmacEntries = ENDIAN_LITTLE_TO_HOST_U32(header->m_data);
    *consumed = 1;
}

//! This method currently assumes that the next tag command will come immediately
//! after the data for this section.
void SB2Image::TagCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    header.m_flags = ENDIAN_HOST_TO_LITTLE_U16(m_sectionFlags);
    header.m_address = ENDIAN_HOST_TO_LITTLE_U32(m_sectionIdentifier);
    header.m_count = ENDIAN_HOST_TO_LITTLE_U32(m_sectionLength);
    header.m_data = ENDIAN_HOST_TO_LITTLE_U32(m_hmacEntries);
    header.m_checksum = calculateChecksum(header); // do this last
}

void SB2Image::TagCommand::debugPrint() const
{
    Log::log(Logger::INFO2, "  BTAG | sec=0x%08x | cnt=0x%08x | flg=0x%08x\n", m_sectionIdentifier, m_sectionLength,
             m_sectionFlags);
}

//! All fields are set to zero.
//!
SB2Image::LoadCommand::LoadCommand()
    : BootCommand()
    , m_data()
    , m_padCount(0)
    , m_length(0)
    , m_address(0)
    , m_loadDCD(false)
    , m_memoryId(0)
{
    fillPadding();
}

SB2Image::LoadCommand::LoadCommand(uint32_t address, const uint8_t *data, uint32_t length)
    : BootCommand()
    , m_data()
    , m_padCount(0)
    , m_length(0)
    , m_address(address)
    , m_loadDCD(false)
    , m_memoryId(0)
{
    fillPadding();
    setData(data, length);
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error This exception is thrown if the actual CRC of the load
//!     data does not match the CRC stored in the command header. Also thrown if the
//!     \a count parameter is less than the number of data blocks needed for the length
//!     specified in the command header or if header fields are invalid.
void SB2Image::LoadCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    // check static fields
    const boot_command_t model = { 0, ROM_LOAD_CMD, 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header, CMD_TAG_FIELD);

    // read fields from header
    m_address = ENDIAN_LITTLE_TO_HOST_U32(header->m_address);
    m_length = ENDIAN_LITTLE_TO_HOST_U32(header->m_count);
    unsigned crc = ENDIAN_LITTLE_TO_HOST_U32(header->m_data);
    unsigned dataBlockCount = numberOfCipherBlocks(m_length);
    m_padCount = sizeOfPaddingForCipherBlocks(dataBlockCount);
    m_loadDCD = (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_LOAD_DCD) != 0;
    m_memoryId = MAKE_MEMORYID((ENDIAN_LITTLE_TO_HOST_U16(header->m_flags)  & ROM_MEM_GROUP_ID_MASK) >> ROM_MEM_GROUP_ID_SHIFT \
                            , (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_MEM_DEVICE_ID_MASK) >> ROM_MEM_DEVICE_ID_SHIFT);

    // make sure there are enough blocks
    if (count - 1 < dataBlockCount)
    {
        throw std::runtime_error("not enough cipher blocks for load data");
    }

    // copy data
    setData(reinterpret_cast<const uint8_t *>(blocks + 1), m_length);

    // copy padding
    if (m_padCount)
    {
        const uint8_t *firstPadByte = reinterpret_cast<const uint8_t *>(blocks + (1 + dataBlockCount)) - m_padCount;
        memcpy(m_padding, firstPadByte, m_padCount);
    }

    // check CRC
    uint32_t actualCRC = calculateCRC();
    if (actualCRC != crc)
    {
        throw std::runtime_error("load data failed CRC check");
    }

    *consumed = 1 + dataBlockCount;
}

//! The only thing unique in the load command header is the
//! #elftosb::SB2Image::boot_command_t::m_data. It contains a CRC-32 over the
//! load data, plus any bytes of padding in the last data cipher block.
void SB2Image::LoadCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    uint16_t flags = m_loadDCD ? ROM_LOAD_DCD : 0;
    flags |= (DEVICEID(m_memoryId) << ROM_MEM_DEVICE_ID_SHIFT) & ROM_MEM_DEVICE_ID_MASK;
    flags |= (GROUPID(m_memoryId) << ROM_MEM_GROUP_ID_SHIFT) & ROM_MEM_GROUP_ID_MASK;
    header.m_flags = ENDIAN_HOST_TO_LITTLE_U32(flags);
    header.m_address = ENDIAN_HOST_TO_LITTLE_U32(m_address);
    header.m_count = ENDIAN_HOST_TO_LITTLE_U32(m_length);
    header.m_data = ENDIAN_HOST_TO_LITTLE_U32(calculateCRC());

    // do this last
    header.m_checksum = calculateChecksum(header);
}

//! A CRC-32 is calculated over the load data, including any pad bytes
//! that are required in the last data cipher block. Including the
//! pad bytes in the CRC makes it vastly easier for the ROM to calculate
//! the CRC for validation.
uint32_t SB2Image::LoadCommand::calculateCRC() const
{
    uint32_t result;
    CRC32 crc;
    crc.update(m_data, m_length);
    if (m_padCount)
    {
        // include random padding in the CRC
        crc.update(m_padding, m_padCount);
    }
    crc.truncatedFinal(reinterpret_cast<uint8_t *>(&result), sizeof(result));

    return result;
}

//! A local copy of the load data is made. This copy will be disposed of when this object
//! is destroyed. This means the caller is free to deallocate \a data after this call
//! returns. It also means the caller can pass a pointer into the middle of a buffer for
//! \a data and not worry about ownership issues.
void SB2Image::LoadCommand::setData(const uint8_t *data, uint32_t length)
{
    assert(data);
    assert(length);

    uint8_t *dataCopy = new uint8_t[length];
    memcpy(dataCopy, data, length);

    m_data = dataCopy;
    m_length = length;

    m_padCount = sizeOfPaddingForCipherBlocks(m_length);
}

//! \return The number of cipher blocks required to hold the load data,
//!		rounded up as necessary.
unsigned SB2Image::LoadCommand::getDataBlockCount() const
{
    // round up to the next cipher block
    return numberOfCipherBlocks(m_length);
}

//! Up to \a maxCount data blocks are copied into the buffer pointed to by
//! the \a data argument. This is only a request for \a maxCount blocks.
//! A return value of 0 indicates that no more blocks are available. The
//! index of the first block to copy is held in the \a offset argument.
//! If there are pad bytes needed to fill out the last data block, they
//! will be filled with random data in order to add to the "whiteness" of
//! the data on both sides of encryption.
//!
//! \param offset Starting block number to copy. Zero means the first available block.
//! \param maxCount Up to this number of blocks may be copied into \a data. Must be 1 or greater.
//! \param data Buffer for outgoing data blocks. Must have enough room to hold
//!		\a maxCount blocks.
//!
//! \return The number of data blocks copied into \a data.
//! \retval 0 No more blocks are available and nothing was written to \a data.
//!
//! \exception std::out_of_range Thrown when offset is invalid.
//!
//! \todo fill pad bytes with random bytes
unsigned SB2Image::LoadCommand::getDataBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data)
{
    assert(data);
    assert(maxCount != 0);

    uint32_t blockCount = getDataBlockCount();

    // check offset
    if (offset >= blockCount)
    {
        throw std::out_of_range("invalid offset");
    }

    // figure out how many blocks to return
    unsigned resultBlocks = blockCount - offset;
    if (resultBlocks > maxCount)
    {
        resultBlocks = maxCount;

        // exclude last block if there is padding
        if (m_padCount && (offset != blockCount - 1) && (offset + resultBlocks == blockCount))
        {
            resultBlocks--;
        }
    }

    // if there are pad bytes, handle the last block specially
    if (m_padCount && offset == blockCount - 1)
    {
        // copy the remainder of the load data into the first part of the result block
        unsigned remainderLength = sizeof(cipher_block_t) - m_padCount;
        memcpy(data, &m_data[sizeof(cipher_block_t) * offset], remainderLength);

        // copy pad bytes we previously generated into the last part of the result block
        // data is a cipher block pointer, so indexing is done on cipher block
        // boundaries, thus we need a byte pointer to index properly
        uint8_t *bytePtr = reinterpret_cast<uint8_t *>(data);
        memcpy(bytePtr + remainderLength, &m_padding, m_padCount);
    }
    else
    {
        memcpy(data, &m_data[sizeof(cipher_block_t) * offset], sizeof(cipher_block_t) * resultBlocks);
    }

    return resultBlocks;
}

//! Fills #m_padding with random bytes that may be used to fill up the last data
//! cipher block.
void SB2Image::LoadCommand::fillPadding()
{
    RandomNumberGenerator rng;
    rng.generateBlock(m_padding, sizeof(m_padding));
}

void SB2Image::LoadCommand::debugPrint() const
{
    uint16_t flags = m_loadDCD ? ROM_LOAD_DCD : 0;
    flags |= (DEVICEID(m_memoryId) << ROM_MEM_DEVICE_ID_SHIFT) & ROM_MEM_DEVICE_ID_MASK;
    flags |= (GROUPID(m_memoryId) << ROM_MEM_GROUP_ID_SHIFT) & ROM_MEM_GROUP_ID_MASK;
    Log::log(Logger::INFO2, "  LOAD | adr=0x%08x | len=0x%08x | crc=0x%08x | flg=0x%04x\n", m_address, m_length,
             calculateCRC(), ENDIAN_HOST_TO_LITTLE_U32(flags));
}

//! The pattern, address, and count are all initialized to zero, and the pattern
//! size is set to a word.
SB2Image::FillCommand::FillCommand()
    : BootCommand()
    , m_address(0)
    , m_count(0)
    , m_pattern(0)
{
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error Thrown if header fields are invalid.
void SB2Image::FillCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    // check static fields
    const boot_command_t model = { 0, ROM_FILL_CMD, 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header, CMD_TAG_FIELD | CMD_FLAGS_FIELD);

    // read fields from header
    m_address = ENDIAN_LITTLE_TO_HOST_U32(header->m_address);
    m_count = ENDIAN_LITTLE_TO_HOST_U32(header->m_count);
    m_pattern = ENDIAN_LITTLE_TO_HOST_U32(header->m_data);

    *consumed = 1;
}

void SB2Image::FillCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    header.m_flags = 0;
    header.m_address = ENDIAN_HOST_TO_LITTLE_U32(m_address);
    header.m_count = ENDIAN_HOST_TO_LITTLE_U32(m_count);
    header.m_data = ENDIAN_HOST_TO_LITTLE_U32(m_pattern);
    header.m_checksum = calculateChecksum(header); // do this last
}

//! Extends the pattern across 32 bits.
//!
void SB2Image::FillCommand::setPattern(uint8_t pattern)
{
    m_pattern = (pattern << 24) | (pattern << 16) | (pattern << 8) | pattern;
}

//! Extends the pattern across 32 bits.
//!
void SB2Image::FillCommand::setPattern(uint16_t pattern)
{
    m_pattern = (pattern << 16) | pattern;
}

void SB2Image::FillCommand::setPattern(uint32_t pattern)
{
    m_pattern = pattern;
}

void SB2Image::FillCommand::debugPrint() const
{
    Log::log(Logger::INFO2, "  FILL | adr=0x%08x | len=0x%08x | ptn=0x%08x\n", m_address, m_count, m_pattern);
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error Thrown if header fields are invalid.
void SB2Image::ModeCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    // check static fields
    const boot_command_t model = { 0, ROM_MODE_CMD, 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header, CMD_TAG_FIELD | CMD_FLAGS_FIELD | CMD_ADDRESS_FIELD | CMD_COUNT_FIELD);

    // read fields from header
    m_mode = ENDIAN_LITTLE_TO_HOST_U32(header->m_data);

    *consumed = 1;
}

void SB2Image::ModeCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    header.m_flags = 0;
    header.m_address = 0;
    header.m_count = 0;
    header.m_data = ENDIAN_HOST_TO_LITTLE_U32(m_mode);
    header.m_checksum = calculateChecksum(header); // do this last
}

void SB2Image::ModeCommand::debugPrint() const
{
    Log::log(Logger::INFO2, "  MODE | mod=0x%08x\n", m_mode);
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error Thrown if header fields are invalid.
void SB2Image::EraseCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    // check static fields
    const boot_command_t model = { 0, ROM_ERASE_CMD, 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header, CMD_TAG_FIELD);

    // read fields from header
    m_doEraseAll = (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_ERASE_ALL_MASK) != 0;
    m_doEraseAllUnsecure = (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_ERASE_ALL_UNSECURE_MASK) != 0;
    m_memoryId = MAKE_MEMORYID((ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_MEM_GROUP_ID_MASK) >> ROM_MEM_GROUP_ID_SHIFT\
        , (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_MEM_DEVICE_ID_MASK) >> ROM_MEM_DEVICE_ID_SHIFT);
    m_startAddress = ENDIAN_LITTLE_TO_HOST_U32(header->m_address);
    m_byteCount = ENDIAN_LITTLE_TO_HOST_U32(header->m_count);

    *consumed = 1;
}

void SB2Image::EraseCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    uint16_t flags = (m_doEraseAll ? ROM_ERASE_ALL_MASK : 0);
    flags |= (m_doEraseAllUnsecure ? ROM_ERASE_ALL_UNSECURE_MASK : 0);
    flags |= (DEVICEID(m_memoryId) << ROM_MEM_DEVICE_ID_SHIFT) & ROM_MEM_DEVICE_ID_MASK;
    flags |= (GROUPID(m_memoryId) << ROM_MEM_GROUP_ID_SHIFT) & ROM_MEM_GROUP_ID_MASK;
    header.m_flags = ENDIAN_HOST_TO_LITTLE_U16(flags);
    header.m_address = ENDIAN_HOST_TO_LITTLE_U32(m_startAddress);
    header.m_count = ENDIAN_HOST_TO_LITTLE_U32(m_byteCount);
    header.m_data = 0;
    header.m_checksum = calculateChecksum(header); // do this last
}

void SB2Image::EraseCommand::debugPrint() const
{
    uint16_t flags = (m_doEraseAll ? ROM_ERASE_ALL_MASK : 0);
    flags |= (m_doEraseAllUnsecure ? ROM_ERASE_ALL_UNSECURE_MASK : 0);
    flags |= (DEVICEID(m_memoryId) << ROM_MEM_DEVICE_ID_SHIFT) & ROM_MEM_DEVICE_ID_MASK;
    flags |= (GROUPID(m_memoryId) << ROM_MEM_GROUP_ID_SHIFT) & ROM_MEM_GROUP_ID_MASK;
    Log::log(Logger::INFO2, "  ERAS | adr=0x%08x | cnt=0x%08x | flg=0x%04x\n", m_startAddress, m_byteCount, ENDIAN_HOST_TO_LITTLE_U16(flags));
}

void SB2Image::EraseCommand::setAddressRange(uint32_t startAddress, uint32_t count)
{
    m_doEraseAll = false;
    m_doEraseAllUnsecure = false;
    m_startAddress = startAddress;
    m_byteCount = count;
}

void SB2Image::EraseCommand::getAddressRange(uint32_t *startAddress, uint32_t *count) const
{
    assert(startAddress && count);
    *startAddress = m_startAddress;
    *count = m_byteCount;
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error Thrown if header fields are invalid.
void SB2Image::ProgramCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    // check static fields
    const boot_command_t model = { 0, ROM_PROG_CMD, 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header, CMD_TAG_FIELD);

    // read fields from header
    m_isEightByte = (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_PROG_8BYTE_MASK) != 0;
    m_memoryId = MAKE_MEMORYID((ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_MEM_GROUP_ID_MASK) >> ROM_MEM_GROUP_ID_SHIFT\
        , (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_MEM_DEVICE_ID_MASK) >> ROM_MEM_DEVICE_ID_SHIFT);
    m_index = ENDIAN_LITTLE_TO_HOST_U32(header->m_address);
    m_dataWord1 = ENDIAN_LITTLE_TO_HOST_U32(header->m_count);
    m_dataWord2 = (m_isEightByte ? ENDIAN_LITTLE_TO_HOST_U32(header->m_data) : 0);

    *consumed = 1;
}

void SB2Image::ProgramCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    uint16_t flags = (m_isEightByte ? ROM_PROG_8BYTE_MASK : 0);
    flags |= (DEVICEID(m_memoryId) << ROM_MEM_DEVICE_ID_SHIFT) & ROM_MEM_DEVICE_ID_MASK;
    flags |= (GROUPID(m_memoryId) << ROM_MEM_GROUP_ID_SHIFT) & ROM_MEM_GROUP_ID_MASK;
    header.m_flags = ENDIAN_HOST_TO_LITTLE_U16(flags);
    header.m_address = ENDIAN_HOST_TO_LITTLE_U32(m_index);
    header.m_count = ENDIAN_HOST_TO_LITTLE_U32(m_dataWord1);
    header.m_data = ENDIAN_HOST_TO_LITTLE_U32(m_dataWord2);
    header.m_checksum = calculateChecksum(header); // do this last
}

void SB2Image::ProgramCommand::debugPrint() const
{
    uint16_t flags = (m_isEightByte ? ROM_PROG_8BYTE_MASK : 0);
    flags |= (DEVICEID(m_memoryId) << ROM_MEM_DEVICE_ID_SHIFT) & ROM_MEM_DEVICE_ID_MASK;
    flags |= (GROUPID(m_memoryId) << ROM_MEM_GROUP_ID_SHIFT) & ROM_MEM_GROUP_ID_MASK;
    Log::log(Logger::INFO2, "  PROG | idx=0x%08x | wd1=0x%08x | wd2=0x%08x | flg=0x%04x\n", m_index, m_dataWord1,
             m_dataWord2, ENDIAN_HOST_TO_LITTLE_U32(flags));
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error Thrown if header fields are invalid.
void SB2Image::JumpCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    // check static fields
    const boot_command_t model = { 0, getTag(), 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header, CMD_TAG_FIELD);

    // read fields from header
    m_address = ENDIAN_LITTLE_TO_HOST_U32(header->m_address);
    m_argument = ENDIAN_LITTLE_TO_HOST_U32(header->m_data);
    m_isHAB = (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_HAB_EXEC) != 0;
    m_isStackPointerSet = (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_JUMP_SP_MASK) != 0;
    m_stackPointer = (m_isStackPointerSet ? ENDIAN_LITTLE_TO_HOST_U32(header->m_count) : 0);

    *consumed = 1;
}

void SB2Image::JumpCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    uint16_t flags = (m_isHAB ? ROM_HAB_EXEC : 0);
    flags |= (m_isStackPointerSet ? ROM_JUMP_SP_MASK : 0);
    header.m_flags = ENDIAN_HOST_TO_LITTLE_U16(flags);
    header.m_address = ENDIAN_HOST_TO_LITTLE_U32(m_address);
    header.m_count = (m_isStackPointerSet ? ENDIAN_HOST_TO_LITTLE_U32(m_stackPointer) : 0);
    header.m_data = ENDIAN_HOST_TO_LITTLE_U32(m_argument);
    header.m_checksum = calculateChecksum(header); // do this last
}

void SB2Image::JumpCommand::debugPrint() const
{
    uint16_t flags = (m_isHAB ? ROM_HAB_EXEC : 0);
    if (m_isStackPointerSet)
    {
        flags |= ROM_JUMP_SP_MASK;
        Log::log(Logger::INFO2, "  JUMP | sp=0x%08x | adr=0x%08x | arg=0x%08x | flg=0x%04x\n", m_stackPointer,
                 m_address, m_argument, flags);
    }
    else
    {
        Log::log(Logger::INFO2, "  JUMP | adr=0x%08x | arg=0x%08x | flg=0x%04x\n", m_address, m_argument, flags);
    }
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error Thrown if header fields are invalid.
void SB2Image::ResetCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    // check static fields
    const boot_command_t model = { 0, getTag(), 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header,
                   CMD_TAG_FIELD | CMD_FLAGS_FIELD | CMD_ADDRESS_FIELD | CMD_COUNT_FIELD | CMD_DATA_FIELD);

    *consumed = 1;
}

void SB2Image::ResetCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    header.m_flags = 0;
    header.m_address = 0;
    header.m_count = 0;
    header.m_data = 0;
    header.m_checksum = calculateChecksum(header); // do this last
}

void SB2Image::ResetCommand::debugPrint() const
{
    Log::log(Logger::INFO2, "  RESET\n");
}

void SB2Image::CallCommand::debugPrint() const
{
    Log::log(Logger::INFO2, "  CALL | adr=0x%08x | arg=0x%08x | flg=0x%04x\n", m_address, m_argument,
             m_isHAB ? ROM_HAB_EXEC : 0);
}

//! \param blocks Pointer to the raw data blocks.
//! \param count Number of blocks pointed to by \a blocks.
//! \param[out] consumed On exit, this points to the number of cipher blocks that were occupied
//!		by the command. Should be at least 1 for every command. This must not be NULL
//!		on entry!
//!
//! \exception std::runtime_error Thrown if header fields are invalid.
void SB2Image::MemEnableCommand::initFromData(const cipher_block_t *blocks, unsigned count, unsigned *consumed)
{
    // check static fields
    const boot_command_t model = { 0, ROM_MEM_ENABLE_CMD, 0, 0, 0, 0 };
    const boot_command_t *header = reinterpret_cast<const boot_command_t *>(blocks);
    validateHeader(&model, header, CMD_TAG_FIELD | CMD_DATA_FIELD);

    // read fields from header
    m_memoryId = MAKE_MEMORYID((ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_MEM_GROUP_ID_MASK) >> ROM_MEM_GROUP_ID_SHIFT\
        , (ENDIAN_LITTLE_TO_HOST_U16(header->m_flags) & ROM_MEM_DEVICE_ID_MASK) >> ROM_MEM_DEVICE_ID_SHIFT);
    m_startAddress = ENDIAN_LITTLE_TO_HOST_U32(header->m_address);
    m_byteCount = ENDIAN_LITTLE_TO_HOST_U32(header->m_count);

    *consumed = 1;
}

void SB2Image::MemEnableCommand::fillCommandHeader(boot_command_t &header)
{
    header.m_tag = getTag();
    uint16_t flags = (DEVICEID(m_memoryId) << ROM_MEM_DEVICE_ID_SHIFT) & ROM_MEM_DEVICE_ID_MASK;
    flags |= (GROUPID(m_memoryId) << ROM_MEM_GROUP_ID_SHIFT) & ROM_MEM_GROUP_ID_MASK;
    header.m_flags = ENDIAN_HOST_TO_LITTLE_U16(flags);
    header.m_address = ENDIAN_HOST_TO_LITTLE_U32(m_startAddress);
    header.m_count = ENDIAN_HOST_TO_LITTLE_U32(m_byteCount);
    header.m_data = 0;
    header.m_checksum = calculateChecksum(header); // do this last
}

void SB2Image::MemEnableCommand::debugPrint() const
{
    uint16_t flags = (DEVICEID(m_memoryId) << ROM_MEM_DEVICE_ID_SHIFT) & ROM_MEM_DEVICE_ID_MASK;
    flags |= (GROUPID(m_memoryId) << ROM_MEM_GROUP_ID_SHIFT) & ROM_MEM_GROUP_ID_MASK;
    Log::log(Logger::INFO2, "  ENA  | adr=0x%08x | cnt=0x%08x | flg=0x%04x\n", m_startAddress, m_byteCount, ENDIAN_HOST_TO_LITTLE_U16(flags));
}

void SB2Image::MemEnableCommand::setAddressRange(uint32_t startAddress, uint32_t count)
{
    m_startAddress = startAddress;
    m_byteCount = count;
}

void SB2Image::MemEnableCommand::getAddressRange(uint32_t *startAddress, uint32_t *count) const
{
    assert(startAddress && count);
    *startAddress = m_startAddress;
    *count = m_byteCount;
}

//! Only if the section has been assigned a boot image owner object will this
//! method be able to fill in the #section_header_t::m_offset field. If no
//! boot image has been set the offset will be set to 0.
void SB2Image::Section::fillSectionHeader(section_header_t &header)
{
    header.m_tag = getIdentifier();
    header.m_offset = 0;
    header.m_length = ENDIAN_HOST_TO_LITTLE_U32(getBlockCount());
    header.m_flags = ENDIAN_HOST_TO_LITTLE_U32(getFlags());

    // if we're attached to an image, we can compute our real offset
    if (m_image)
    {
        header.m_offset = ENDIAN_HOST_TO_LITTLE_U32(m_image->getSectionOffset(this));
    }
}

//! The alignment will never be less than 16, since that is the size of the
//! cipher block which is the basic unit of the boot image format. If an
//! alignment less than 16 is set it will be ignored.
//!
//! \param alignment Alignment in bytes for this section. Must be a power of two.
//!		Ignored if less than 16.
void SB2Image::Section::setAlignment(unsigned alignment)
{
    if (alignment > BOOT_IMAGE_MINIMUM_SECTION_ALIGNMENT)
    {
        m_alignment = alignment;
    }
}

//! This method calculates the number of padding blocks that need to be inserted
//! from a given offset for the section to be properly aligned. The value returned
//! is the number of padding blocks that should be inserted starting just after
//! \a offset to align the first cipher block of the section contents. The section's
//! boot tag is \i not taken into account by this method, so the caller must
//! deal with that herself.
//!
//! \param offset Start offset in cipher blocks (not bytes).
//!
//! \return A number of cipher blocks of padding to insert.
unsigned SB2Image::Section::getPadBlockCountForOffset(unsigned offset)
{
    // convert alignment from byte to block alignment
    unsigned blockAlignment = m_alignment >> 4;

    unsigned nextAlignmentOffset = (offset + blockAlignment - 1) / blockAlignment * blockAlignment;

    return nextAlignmentOffset - offset;
}

SB2Image::BootSection::~BootSection()
{
    deleteCommands();
}

void SB2Image::BootSection::deleteCommands()
{
    // dispose of all sections
    iterator_t it = begin();
    for (; it != end(); ++it)
    {
        delete *it;
    }
}

//! Always returns at least 1 for the required tag command.
//!
unsigned SB2Image::BootSection::getBlockCount() const
{
    unsigned count = 0;

    const_iterator_t it = begin();
    for (; it != end(); ++it)
    {
        count += (*it)->getBlockCount();
    }

    return count;
}

//! Up to \a maxCount cipher blocks are copied into the buffer pointed to by
//! the \a data argument. A return value of 0 indicates that
//! no more blocks are available. The index of the first block to copy is
//! held in the \a offset argument.
//!
//! \param offset Starting block number to copy. Zero means the first available block.
//! \param maxCount Up to this number of blocks may be copied into \a data.
//! \param data Buffer for outgoing cipher blocks. Must have enough room to hold
//!		\a maxCount blocks.
//!
//! \return The number of cipher blocks copied into \a data.
//! \retval 0 No more blocks are available and nothing was written to \a data.
unsigned SB2Image::BootSection::getBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data)
{
    assert(data);
    assert(maxCount >= 1);

    unsigned currentOffset = 0;
    unsigned readCount = maxCount;

    iterator_t it = begin();
    for (; it != end(); ++it)
    {
        BootCommand *command = *it;
        unsigned commandBlocks = command->getBlockCount();

        // this should never be false!
        assert(offset >= currentOffset);

        // skip forward until we hit the requested offset
        if (offset >= currentOffset + commandBlocks)
        {
            currentOffset += commandBlocks;
            continue;
        }

        // read from this command
        unsigned commandOffset = offset - currentOffset;
        unsigned commandRemaining = commandBlocks - commandOffset;
        if (readCount > commandRemaining)
        {
            readCount = commandRemaining;
        }
        return command->getBlocks(commandOffset, readCount, data);
    }

    return 0;
}

//! The entire contents of the section must be in memory, pointed to by \a blocks.
//! Any commands that had previously been added to the section are disposed of.
//!
//! \param blocks Pointer to the section contents.
//! \param count Number of blocks pointed to by \a blocks.
//!
//! \exception std::runtime_error Thrown if a boot command cannot be created from
//!		the cipher block stream.
void SB2Image::BootSection::fillFromData(const cipher_block_t *blocks, unsigned count)
{
    // start with an empty slate
    deleteCommands();

    const cipher_block_t *currentBlock = blocks;
    unsigned remaining = count;
    while (remaining)
    {
        // try to create a command from the next cipher block. the number of
        // blocks the command used up is returned in consumed.
        unsigned consumed;
        BootCommand *command = BootCommand::createFromData(currentBlock, remaining, &consumed);
        if (!command)
        {
            throw std::runtime_error("invalid boot section data");
        }

        addCommand(command);

        // update loop counters
        remaining -= consumed;
        currentBlock += consumed;
    }
}

void SB2Image::BootSection::debugPrint() const
{
    Log::log(Logger::INFO2, "Boot Section 0x%08x:\n", m_identifier);

    const_iterator_t it = begin();
    for (; it != end(); ++it)
    {
        const BootCommand *command = *it;
        command->debugPrint();
    }
}

//! A copy is made of \a data. Any previously assigned data is disposed of.
//!
void SB2Image::DataSection::setData(const uint8_t *data, unsigned length)
{
    m_data = new uint8_t[length];
    memcpy(m_data.get(), data, length);
    m_length = length;
}

//! The section takes ownership of \a data and will dispose of it using the
//! array delete operator upon its destruction.
void SB2Image::DataSection::setDataNoCopy(const uint8_t *data, unsigned length)
{
    m_data = data;
    m_length = length;
}

unsigned SB2Image::DataSection::getBlockCount() const
{
    return numberOfCipherBlocks(m_length);
}

unsigned SB2Image::DataSection::getBlocks(unsigned offset, unsigned maxCount, cipher_block_t *data)
{
    assert(data);
    assert(maxCount != 0);

    unsigned blockCount = getBlockCount();
    unsigned padCount = sizeOfPaddingForCipherBlocks(m_length);

    // check offset
    if (offset >= blockCount)
    {
        throw std::out_of_range("invalid offset");
    }

    // figure out how many blocks to return
    unsigned resultBlocks = blockCount - offset;
    if (resultBlocks > maxCount)
    {
        resultBlocks = maxCount;

        // exclude last block if there is padding
        if (padCount && (offset != blockCount - 1) && (offset + resultBlocks == blockCount))
        {
            resultBlocks--;
        }
    }

    // if there are pad bytes, handle the last block specially
    if (padCount && offset == blockCount - 1)
    {
        // copy the remainder of the load data into the first part of the result block
        unsigned remainderLength = sizeof(cipher_block_t) - padCount;
        memcpy(data, &m_data[sizeOfCipherBlocks(offset)], remainderLength);

        // set pad bytes to zeroes.
        // data is a cipher block pointer, so indexing is done on cipher block
        // boundaries, thus we need a byte pointer to index properly
        uint8_t *bytePtr = reinterpret_cast<uint8_t *>(data);
        memset(bytePtr + remainderLength, 0, padCount);
    }
    else
    {
        memcpy(data, &m_data[sizeOfCipherBlocks(offset)], sizeOfCipherBlocks(resultBlocks));
    }

    return resultBlocks;
}

void SB2Image::DataSection::debugPrint() const
{
    Log::log(Logger::INFO2, "Data Section 0x%08x: (%d bytes, %d blocks)\n", m_identifier, m_length, getBlockCount());
}
