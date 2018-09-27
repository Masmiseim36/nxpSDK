/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "iMXImage.h"
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
#include "iMXImageGenerator.h"

#define CSF_PADDING_SIZE         0x2000 // CSF data is padded to 8k size
#define KEYBLOB_ADDRESS_MULTIPLE 0x1000 // Blob address starts where csf padding ends, we use this macro to make it ends at a 4K boundary for nand boot to work
#define KEYBLOB_SIZE             0x200  // Max size for the key blob

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

iMXImage::iMXImage()
    : m_headerFlags(0)
    , m_productVersion()
    , m_componentVersion()
    , m_dcdFilePath(NULL)
    , m_outputFilePath(NULL)
    , m_pSourceFile(NULL)
    , m_keyBlobAddress(0)
    , m_entryPointAddress(0)
{
}

iMXImage::~iMXImage()
{
    // dispose of all sections
    section_iterator_t it = beginSection();
    for (; it != endSection(); ++it)
    {
        delete *it;
    }
}

//! \exception std::runtime_error Raised if \a newSection has the same tag as a previously
//!		added section.
void iMXImage::addSection(Section *newSection)
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
    m_sections.push_back(newSection);

    // tell the image who owns it now
    newSection->setImage(this);
}

iMXImage::section_iterator_t iMXImage::findSection(Section *section)
{
    return std::find(beginSection(), endSection(), section);
}

void iMXImage::setProductVersion(const version_t &version)
{
    m_productVersion = version;
}

void iMXImage::setComponentVersion(const version_t &version)
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
void iMXImage::writeToStream(std::ostream &stream)
{
	/* File names for temporary intermediate files */
	std::string csfPath("csf.bin");
	std::string tempBinary("temp.bin");
	std::string inputCSF("input.csf");

	/* cstPath is used to below to call cst.exe with necessary parameters, 
		the command line is constructed later below */
	char cstPath[512];

	UnmappedDataSource *dsDcd = NULL; //!< Data source object for the DCD
	UnmappedDataSource *dsCsf = NULL; //!< Data source object for the CSF data - output from cst.exe
	PaddingDataSource *dsImgPadding = NULL; //!< For padding the extra bytes to make elf image data multiple of 16
	PaddingDataSource *dsCsfPadding = NULL; //!< For padding csf space

	uint32_t csfOffset = 0;
	uint32_t imgPaddingSize = 0;
	uint32_t sizePadding = 0;
	uint32_t keyBlobSize = 0;
	uint32_t defaultEntryPoint = 0;

	m_pSourceFile->open();
	DataSource *dsElf = (DataSource *)m_pSourceFile->createDataSource();

	DataSourceImager *dsElfImage = NULL;
	dsElfImage = createDataSourceImager(dsElf, NULL, NULL, NULL, NULL, NULL);

	IVTDataSource ivt; //! Data source for IVT data structure
	
	if (m_entryPointAddress)
	{
		// If bd file options contains entry point address.
		ivt.setFieldByName("entry", m_entryPointAddress);
	}
	else if (m_pSourceFile->hasEntryPoint())
	{
		// If bd file options doesn't contain entry point address but source file is .elf
		ivt.setFieldByName("entry", m_pSourceFile->getEntryPointAddress());
	}
	else 
	{
		// If Source file doesn't have entry point address and no option in bd file, then binary source file uses default entry-point address.
		ivt.setFieldByName("entry", defaultEntryPoint);
		Log::log(Logger::WARNING, "warning: Default entry point address %x is used\n", defaultEntryPoint);

	}
	ivt.setFieldByName("self", m_startAddress + m_ivtOffset);
	ivt.setFieldByName("boot_data", m_startAddress + m_ivtOffset + sizeof(hab_ivt_t));

	// If dcd file is provided then add dcd pointer to IVT and create DCD data source object
	if(m_dcdFilePath)
	{
		BinarySourceFile dcd(m_dcdFilePath);
		dcd.open();
		dsDcd = (UnmappedDataSource *) dcd.createDataSource();
		unsigned dcdOffset = m_ivtOffset + sizeof(hab_ivt_t) + sizeof(boot_data_t);
		if (dsDcd->getLength() > (m_initialLoadSize - dcdOffset))
		{
			Log::log(Logger::ERROR, "error: dcd file too big to fit in the first 8K bytes\n");
			return;
		}
        /* Locate dcd in ivt*/
		ivt.setFieldByName("dcd", m_startAddress + dcdOffset);
		dsDcd->setBaseAddress(ivt.getIVT().dcd);
	}

	BootDataSource bd(ivt.getBaseAddress() + sizeof(hab_ivt_t)); //! Data source for boot data structure
	bd.setFieldByName("start", m_startAddress);
	bd.setFieldByName("flags", isPlugin() ? BOOTDATA_PLUGIN_FLAG : 0);

	/* If image is to be signed then update the csf offset in the IVT */
	if (isSigned())
	{
		/* Add a padding section after elf followed by the csf */
		csfOffset = dsElfImage->getLength() + (16 - (dsElfImage->getLength() % 16));

		csfOffset = (((csfOffset + KEYBLOB_ADDRESS_MULTIPLE - 1) / KEYBLOB_ADDRESS_MULTIPLE) * KEYBLOB_ADDRESS_MULTIPLE);

		sizePadding = CSF_PADDING_SIZE;
		if(isEncrypted())
		{
			keyBlobSize = KEYBLOB_SIZE;
		}
		imgPaddingSize = csfOffset - (dsElfImage->getLength());
		dsImgPadding = new PaddingDataSource(m_startAddress + dsElfImage->getLength(), imgPaddingSize, 0x00);
		dsCsfPadding = new PaddingDataSource(m_startAddress + csfOffset, sizePadding, 0x00);
		setKeyBlobAddress(m_startAddress + csfOffset + CSF_PADDING_SIZE);
		ivt.setFieldByName("csf", m_startAddress + csfOffset);
	}
	
	/* 
       Length should be the entire image legth with all data structures and any padding, 
       from start address to end of csf and padding (for signed/encrypted image) or
       from start address to end of image (for unsigned image)
			Here is the boot image format elftosb creates
            start address to ivt-offset - all zeroes
            followed by 32 bytes of ivt data structure
            followed by 32 bytes of boot data structure
            followed by DCD data if dcd input file is specified
            followed by any padding between dcd and end of initial load size
            followed by text and ro sections of elf image padded to make the size multiple of 16.
                If encrypted image is specified in the options flag then the image data will be replaced
				with the encrypted data done-in-place by the cst.exe
            followed by csf data, the binary output from cst.exe - if options flag specified image to be signed/encrypted
			followed by padding to make csf data length hard coded to 8K
    */

	bd.setFieldByName("size", dsElfImage->getLength() + keyBlobSize);

	/* Generate DataSourceImager object with all the padding - before cst is called
       This is just to generate the total image size less keyBlobSize */
	DataSourceImager * dsImager;
	dsImager = createDataSourceImager(&ivt, &bd, dsDcd, dsElf, dsImgPadding, dsCsfPadding);   // for image size
	if (dsImager == NULL)
	{
		Log::log(Logger::ERROR, "error: out of memory\n");
		return;
	}

	/* If only unsigned image is needed then we are done - push out the data from ds imager object to the stream */
	if(!isSigned())
	{
		stream.write(reinterpret_cast<char *>(dsImager->getData()), dsImager->getLength());
		createNoPaddingImage(dsImager);
		// For unsigned image we are done
		Log::log("iMX bootable image generated successfully\n");
		return;
	}	

	/* Update size field of boot data source for encrypted/signed image */
	bd.setFieldByName("size", dsImager->getLength() + keyBlobSize);

	/* 
	   Recreate the dsImager object with ivt, bd, dcd, image data and padding, 
       this image will be sent to cst for signing 
    */
	delete dsImager;

	dsImager = createDataSourceImager(&ivt, &bd, dsDcd, dsElf, dsImgPadding, NULL);  // for signing
	if (dsImager == NULL)
	{
		Log::log(Logger::ERROR, "error: out of memory\n");
		return;
	}

	/* Push out dsImager data to temp binary for signing */
	FILE *pf = fopen(tempBinary.c_str(), "wb");
	fwrite(dsImager->getData(), sizeof(uint8_t), dsImager->getLength(), pf);
	fclose(pf);

/*
	if(Log::getLogger()->getFilterLevel() != Logger::DEBUG)
	{
		pf = fopen("backup.bin", "wb");
		fwrite(dsImager->getData(), sizeof(uint8_t), dsImager->getLength(), pf);
		fclose(pf);
	}
*/	
	/* 
       Generate input.csf text file with all the csf commands for signing/encryption. 
       Input csf file will be passed to cst.exe using -i command
     */
    generateInputCSF(tempBinary.c_str(), inputCSF.c_str(), &ivt, &bd, dsDcd, dsElf);

	generateCSTfolderPath();
	
	/* Create the cst command line string */
	sprintf(cstPath, "%s -i %s -o %s", m_cstFolderPath.c_str(), inputCSF.c_str(), csfPath.c_str());

	/* Using system call to execute cst - system call is available across windows, linux and mac */
    /* Todo: resolve the cst.exe name for linux/mac, ".exe" may not be part of the executable name */
	if( system(cstPath) != 0)
	{
		Log::log(Logger::ERROR, "error: failed to run code signing tool\n");
/*		if(Log::getLogger()->getFilterLevel() != Logger::DEBUG)
		{
			remove(tempBinary.c_str());
			remove(inputCSF.c_str());
			remove(csfPath.c_str());
		}
*/		return;
	}

	/* 
	   Create data source for the csf.bin, use BinarySourceFile object to open the file and 
       then use its createDataSouce API for generating datasource
     */
	BinarySourceFile csf(csfPath);
	csf.open();
	dsCsf = (UnmappedDataSource *)csf.createDataSource(sizePadding);
	dsCsf->setBaseAddress(ivt.getIVT().csf);

	/* 
       Recreate dsImager object for final boot image. For encrypted images just create one 
       object with temp.bin as cst has replaced its image data with encrypted image data 
     */
	if (!isEncrypted())
	{
		delete dsImager;
		dsImager = createDataSourceImager(&ivt, &bd, dsDcd, dsElf, dsImgPadding, dsCsf);
		if (dsImager == NULL)
		{
			Log::log(Logger::ERROR, "error: out of memory\n");
			return;
		}
	}
	else
	{
		// Encrypted data will be in the tempBinary along with ivt, bd, dcd but csf
		BinarySourceFile encryptBin(tempBinary);
		encryptBin.open();
		UnmappedDataSource * dsEncryptBin = (UnmappedDataSource *)encryptBin.createDataSource();
		dsEncryptBin->setBaseAddress(m_startAddress);

		delete dsImager;
		dsImager = createDataSourceImager(dsEncryptBin, dsCsf, NULL, NULL, NULL, NULL);
		if (dsImager == NULL)
		{
			Log::log(Logger::ERROR, "error: out of memory\n");
			return;
		}
	}
/*
	if (Log::getLogger()->getFilterLevel() != Logger::DEBUG)
	{
		remove(tempBinary.c_str());
		remove(inputCSF.c_str());
		remove(csfPath.c_str());
	}
*/

	/* Push the final image to the output stream */
	stream.write(reinterpret_cast<char *>(dsImager->getData()), dsImager->getLength());

	/* 
       Also output another copy of the final image stripping data before the IVT, 
       name the copy by adding _nopadding to the final image name.  
     */
	createNoPaddingImage(dsImager);
	Log::log("iMX bootable image generated successfully\n");
	if(isEncrypted())
	{
		Log::log("Key Blob Address is 0x%x.\n", getKeyBlobAddress());
		Log::log("Key Blob data should be placed at Offset :0x%x in the image\n", csfOffset + CSF_PADDING_SIZE);
	}
	
/* That's all folks! */
}

void iMXImage::createNoPaddingImage(DataSourceImager *dsImager)
{
	/* Add "_nopadding" to the output boot image name for the copy of boot image that starts from IVT */
	std::string outputFilePath(m_outputFilePath);
	size_t dotPos = outputFilePath.find_last_of(".");
	if (dotPos == std::string::npos)
	{
		outputFilePath += "_nopadding";
	}
	else
	{
		std::string extension = outputFilePath.substr(dotPos);

		outputFilePath = outputFilePath.substr(0, dotPos);
		outputFilePath += "_nopadding" + extension;
	}

	std::ofstream outputStream(outputFilePath, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
	if (outputStream.is_open())
	{
		/* Skip until ivt offset to write data the nopadding copy */
		outputStream.write(reinterpret_cast<char *>(dsImager->getData() + m_ivtOffset), dsImager->getLength() - m_ivtOffset);
	}
}

DataSourceImager *  iMXImage::createDataSourceImager(DataSource *d1, DataSource *d2, DataSource *d3, DataSource *d4, DataSource *d5, DataSource *d6)
{
	/* 
       Creates dsImager with the input data sources, max 6 data sources. Caller to set NULL to the extra 
       datasources if fewer than 6 data source are needed to create the imager.
     */
	DataSourceImager * dsImager = new DataSourceImager();
	if(dsImager == NULL)
		return NULL;

	dsImager->setBaseAddress(m_startAddress);
	dsImager->setFillPattern(0x00);

	if (d1)
	{
		dsImager->addDataSource(d1);
	}
	if (d2)
	{
		dsImager->addDataSource(d2);
	}
	if (d3)
	{
		dsImager->addDataSource(d3);
	}
	if (d4)
	{
		dsImager->addDataSource(d4);
	}
	if (d5)
	{
		dsImager->addDataSource(d5);
	}
	if (d6)
	{
		dsImager->addDataSource(d6);
	}
	return dsImager;
}

void iMXImage::generateInputCSF(const char * binFile, const char * csfFile, IVTDataSource *pIVT, BootDataSource *pBD, UnmappedDataSource * pDCD, DataSource *pElf)
{
	/* Creates input csf text file with CSF sections specified in the bd file */

	FILE *pfCSF = fopen(csfFile, "w");

	Section *pSectionHeader = NULL;
	Section *pSectionInstallSRK = NULL;
	Section *pSectionInstallCSFK = NULL;
	Section *pSectionInstallNOCAK = NULL;
	Section *pSectionAuthenticateCSF = NULL;
	Section *pSectionInstallKey = NULL;
	Section *pSectionAuthenticateData = NULL;
	Section *pSectionInsSecretKey = NULL;
	Section *pSectionDecryptData = NULL;
	Section *pSectionNOP = NULL;
	Section *pSectionSetMID = NULL;
	Section *pSectionSetEngine = NULL;
	Section *pSectionInit = NULL;
	Section *pSectionUnlock = NULL;
	
	section_iterator_t it = beginSection();
	for (; it != endSection(); ++it)
	{
		Section *thisSection = *it;

		if (thisSection->getIdentifier() == SEC_CSF_HEADER)
			pSectionHeader = thisSection;
		else if (thisSection->getIdentifier() == SEC_CSF_INSTALL_SRK)
			pSectionInstallSRK = thisSection;
		else if (thisSection->getIdentifier() == SEC_CSF_INSTALL_CSFK)
			pSectionInstallCSFK = thisSection;
		else if (thisSection->getIdentifier() == SEC_CSF_INSTALL_NOCAK)
			pSectionInstallNOCAK = thisSection;
		else if (thisSection->getIdentifier() == SEC_CSF_AUTHENTICATE_CSF)
			pSectionAuthenticateCSF = thisSection;
		else if (thisSection->getIdentifier() == SEC_CSF_INSTALL_KEY)
			pSectionInstallKey = thisSection;
		else if (thisSection->getIdentifier() == SEC_CSF_AUTHENTICATE_DATA)
			pSectionAuthenticateData = thisSection;
		else if (thisSection->getIdentifier() == SEC_CSF_INSTALL_SECRET_KEY)
			pSectionInsSecretKey = thisSection;
		else if (thisSection->getIdentifier() == SEC_CSF_DECRYPT_DATA)
			pSectionDecryptData = thisSection;
		else if (thisSection->getIdentifier() == SEC_NOP)
			pSectionNOP = thisSection;
		else if (thisSection->getIdentifier() == SEC_SET_MID)
			pSectionSetMID = thisSection;
		else if (thisSection->getIdentifier() == SEC_SET_ENGINE)
			pSectionSetEngine = thisSection;
		else if (thisSection->getIdentifier() == SEC_INIT)
			pSectionInit = thisSection;
		else if (thisSection->getIdentifier() == SEC_UNLOCK)
			pSectionUnlock = thisSection;
	}

	fprintf(pfCSF, "[Header]\n");
	if (pSectionHeader->getHdrVersion().length())
	{
		fprintf(pfCSF, "    Version = %s\n", pSectionHeader->getHdrVersion().c_str());
	}
	if (pSectionHeader->getHdrEng().length())
	{
		fprintf(pfCSF, "    Engine = %s\n", pSectionHeader->getHdrEng().c_str());
	}
	if (pSectionHeader->getHdrEngConfig().length())
	{
		fprintf(pfCSF, "    Engine Configuration = %s\n", pSectionHeader->getHdrEngConfig().c_str());
	}
	if (pSectionHeader->getHdrCertFormat().length())
	{
		fprintf(pfCSF, "    Certificate Format = %s\n", pSectionHeader->getHdrCertFormat().c_str());
	}
	if (pSectionHeader->getHdrSigFormat().length())
	{
		fprintf(pfCSF, "    Signature Format = %s\n", pSectionHeader->getHdrSigFormat().c_str());
	}
	if (pSectionHeader->getHdrHashAlg().length())
	{
		fprintf(pfCSF, "    Hash Algorithm = %s\n", pSectionHeader->getHdrHashAlg().c_str());
	}
	if(pSectionHeader->getHdrUID().length())
	{
		fprintf(pfCSF, "    UID = %s\n", pSectionHeader->getHdrUID().c_str());
	}
	if (pSectionHeader->getHdrCustCode().length())
	{
		fprintf(pfCSF, "    CODE = %s\n", pSectionHeader->getHdrCustCode().c_str());
	}
	if (pSectionHeader->getHdrSecConfig().length())
	{
		fprintf(pfCSF, "    Security Configuration = %s\n", pSectionHeader->getHdrSecConfig().c_str());
	}

	fprintf(pfCSF, "\n[Install SRK]\n");
	fprintf(pfCSF, "    File = \"%s\"\n", pSectionInstallSRK->getInsSRKFile().c_str());
	fprintf(pfCSF, "    Source Index = %s\n", pSectionInstallSRK->getInsSRKSrcIdx().c_str());
	if (pSectionInstallSRK->getInsSRKHashAlg().length())
	{
		fprintf(pfCSF, "    Hash Algorithm = %s\n", pSectionInstallSRK->getInsSRKHashAlg().c_str());
	}

	if (pSectionInstallCSFK)
	{
		fprintf(pfCSF, "\n[Install CSFK]\n");
		fprintf(pfCSF, "    File = \"%s\"\n", pSectionInstallCSFK->getInsCSFKFile().c_str());
		if(pSectionInstallCSFK->getInsCSFKCertFormat().length())
		{
			fprintf(pfCSF, "    Certificate Format = %s\n", pSectionInstallCSFK->getInsCSFKCertFormat().c_str());
		}
	}
	if (pSectionInstallNOCAK)
	{
		fprintf(pfCSF, "\n[Install NOCAK]\n");
		fprintf(pfCSF, "    File = \"%s\"\n", pSectionInstallNOCAK->getInsNOCAKFile().c_str());
		if (pSectionInstallNOCAK->getInsNOCAKCertFormat().length())
		{
			fprintf(pfCSF, "    Certificate Format = %s\n", pSectionInstallNOCAK->getInsNOCAKCertFormat().c_str());
		}
	}

	if (pSectionAuthenticateCSF)
	{
		fprintf(pfCSF, "\n[Authenticate CSF]\n");
		if (pSectionAuthenticateCSF->getAutCSFEng().length())
		{
			fprintf(pfCSF, "    Engine = %s\n", pSectionAuthenticateCSF->getAutCSFEng().c_str());
		}
		if (pSectionAuthenticateCSF->getAutCSFEngConfig().length())
		{
			fprintf(pfCSF, "    Engine Configuration = %s\n", pSectionAuthenticateCSF->getAutCSFEngConfig().c_str());
		}
		if (pSectionAuthenticateCSF->getAutCSFSigFormat().length())
		{
			fprintf(pfCSF, "    Signature Format = %s\n", pSectionAuthenticateCSF->getAutCSFSigFormat().c_str());
		}
	}

	if (pSectionInstallKey)
	{
		fprintf(pfCSF, "\n[Install Key]\n");
		fprintf(pfCSF, "    File = \"%s\"\n", pSectionInstallKey->getInsKeyFile().c_str());
		if (pSectionInstallKey->getInsKeyVfyIdx().length())
		{
			fprintf(pfCSF, "    Verification Index = %s\n", pSectionInstallKey->getInsKeyVfyIdx().c_str());
		}
		if (pSectionInstallKey->getInsKeyTgtIdx().length())
		{
			fprintf(pfCSF, "    Target Index = %s\n", pSectionInstallKey->getInsKeyTgtIdx().c_str());
		}
		if (pSectionInstallKey->getInsKeyCertFormat().length())
		{
			fprintf(pfCSF, "    Certificate Format = %s\n", pSectionInstallKey->getInsKeyCertFormat().c_str());
		}
		if (pSectionInstallKey->getInsKeyHashAlg().length())
		{
			fprintf(pfCSF, "    Hash Algorithm = %s\n", pSectionInstallKey->getInsKeyHashAlg().c_str());
		}
	}
	
	if (pSectionAuthenticateData)
	{
		fprintf(pfCSF, "\n[Authenticate Data]\n");
		if (pSectionAuthenticateData->getAutDatVfyIdx().length())
		{
			fprintf(pfCSF, "    Verification Index = %s\n", pSectionAuthenticateData->getAutDatVfyIdx().c_str());
		}
		if (pSectionAuthenticateData->getAutDatEng().length())
		{
			fprintf(pfCSF, "    Engine = %s\n", pSectionAuthenticateData->getAutDatEng().c_str());
		}
		if (pSectionAuthenticateData->getAutDatEngConfig().length())
		{
			fprintf(pfCSF, "    Engine Configuration = %s\n", pSectionAuthenticateData->getAutDatEngConfig().c_str());
		}
		if (pSectionAuthenticateData->getAutDatSigFormat().length())
		{
			fprintf(pfCSF, "    Signature Format = %s\n", pSectionAuthenticateData->getAutDatSigFormat().c_str());
		}
		if (pSectionAuthenticateData->getAutDatBinding().length())
		{
			fprintf(pfCSF, "    Binding = %s\n", pSectionAuthenticateData->getAutDatBinding().c_str());
		}
	}
	
	// If IVT and BD are not contiguous segments
	if (pBD->getBaseAddress() != pIVT->getBaseAddress() + pIVT->getLength())
	{
		fprintf(pfCSF, "    Blocks = 0x%x 0x%x 0x%x \"%s\",\\\n", pIVT->getBaseAddress(), pIVT->getBaseAddress() - m_startAddress, pIVT->getLength(), binFile);
		fprintf(pfCSF, "             0x%x 0x%x 0x%x \"%s\"", pBD->getBaseAddress(), pBD->getBaseAddress() - m_startAddress, pBD->getLength(), binFile);
	}
	else
	{
		// If IVT and BD are contiguous segments, then combine them into one segment block
		fprintf(pfCSF, "    Blocks = 0x%x 0x%x 0x%x \"%s\"", pIVT->getBaseAddress(), pIVT->getBaseAddress() - m_startAddress, pIVT->getLength() + pBD->getLength(), binFile);
	}

	if(pDCD)
	{
		if (isEncrypted())
			fprintf(pfCSF, ",\\\n             0x%x 0x%x 0x%x \"%s\"", pDCD->getBaseAddress(), pDCD->getBaseAddress() - m_startAddress, pDCD->getLength(), binFile);
		else
			fprintf(pfCSF, ",\\\n             0x%x 0x%x 0x%x \"%s\"", pDCD->getBaseAddress(), pDCD->getBaseAddress() - m_startAddress, pDCD->getLength(), binFile);
	}
 
	if (isEncrypted())
	{
		fprintf(pfCSF, "\n[Install Secret Key]\n");
		fprintf(pfCSF, "    Verification index = %d\n", pSectionInsSecretKey->getSecKeyVfyIdx());
		fprintf(pfCSF, "    Target index = %d\n", pSectionInsSecretKey->getSecKeyTgtIdx());
		fprintf(pfCSF, "    Key = \"%s\"\n", pSectionInsSecretKey->getSecKeyName().c_str());
		fprintf(pfCSF, "    Key Length = %d\n", pSectionInsSecretKey->getSecKeyLength());
		fprintf(pfCSF, "    Blob address = 0x%x\n", getKeyBlobAddress());

		fprintf(pfCSF, "\n[Decrypt Data]\n");
		fprintf(pfCSF, "    Engine = %s\n", pSectionDecryptData->getDecryptEngine().c_str());
		if (pSectionDecryptData->getDecryptEngConfig().length())
		{
			fprintf(pfCSF, "    Engine Configuration = %s\n", pSectionDecryptData->getDecryptEngConfig().c_str());
		}
		fprintf(pfCSF, "    Verification index = %d\n", pSectionDecryptData->getDecryptVfyIdx());
		fprintf(pfCSF, "    Mac Bytes = %d\n", pSectionDecryptData->getDecryptMacBytes());
		fprintf(pfCSF, "    Blocks = ");
	}
	else
	{
		fprintf(pfCSF, ",\\\n");
	}
	
	uint32_t previousSegmentBaseAddress = 0;
	uint32_t previousSegmentLength = 0;

	uint32_t currentSegmentBaseAddress = 0;
	uint32_t currentSegmentLength = 0;

	for (int i = 0; i<pElf->getSegmentCount(); i++)
	{
		DataSource::Segment *segment = pElf->getSegmentAt(i);
		currentSegmentBaseAddress = segment->getBaseAddress();
		currentSegmentLength = segment->getLength();

		// If current segment is not contiguous with previous
		if (currentSegmentBaseAddress != previousSegmentBaseAddress + previousSegmentLength)
		{
			if (i != 0)
			{
				fprintf(pfCSF, "             0x%x 0x%x 0x%x \"%s\",\\\n", previousSegmentBaseAddress, previousSegmentBaseAddress - m_startAddress,
					previousSegmentLength, binFile);
			}
			previousSegmentBaseAddress = currentSegmentBaseAddress;
			previousSegmentLength = currentSegmentLength;
		}
		else //if (currentSegmentBaseAddress == previousSegmentBaseAddress + previousSegmentLength)
		{
			previousSegmentLength += currentSegmentLength;
		}

		// Last segment block case: It should be aligned, block length is multiple of 16
		if (i == (pElf->getSegmentCount() - 1))
		{
			fprintf(pfCSF, "             0x%x 0x%x 0x%x \"%s\"\n", previousSegmentBaseAddress, previousSegmentBaseAddress - m_startAddress,
				(((previousSegmentLength + 16 - 1) / 16) * 16), binFile);
		}
	}

	if (pSectionNOP)
	{
		fprintf(pfCSF, "\n[NOP]\n");
	}

	if (pSectionSetMID)
	{
		fprintf(pfCSF, "\n[Set MID]\n");
		fprintf(pfCSF, "    Bank = %s\n", pSectionSetMID->getSetMIDBank().c_str());
		fprintf(pfCSF, "    Row = %s\n", pSectionSetMID->getSetMIDRow().c_str());
		fprintf(pfCSF, "    Fuse = %s\n", pSectionSetMID->getSetMIDFuse().c_str());
		fprintf(pfCSF, "    Bits = %s\n", pSectionSetMID->getSetMIDBits().c_str());
	}

	if (pSectionSetEngine)
	{
		fprintf(pfCSF, "\n[Set Engine]\n");
		fprintf(pfCSF, "    Hash Algorithm = %s\n", pSectionSetEngine->getSetEngHashAlg().c_str());
		fprintf(pfCSF, "    Engine = %s\n", pSectionSetEngine->getSetEngEng().c_str());
		if (pSectionSetEngine->getSetEngConfig().length())
		{
			fprintf(pfCSF, "    Engine Configuration = %s\n", pSectionSetEngine->getSetEngConfig().c_str());
		}
	}

	if (pSectionInit)
	{
		fprintf(pfCSF, "\n[Init]\n");
		fprintf(pfCSF, "    Engine = %s\n", pSectionInit->getInitEng().c_str());
		if (pSectionInit->getInitFeatures().length())
		{
			fprintf(pfCSF, "    Features = %s\n", pSectionInit->getInitFeatures().c_str());
		}
	}

	if (pSectionUnlock)
	{
		fprintf(pfCSF, "\n[Unlock]\n");
		fprintf(pfCSF, "    Engine = %s\n", pSectionUnlock->getUnlockEng().c_str());
		if (pSectionUnlock->getUnlockFeatures().length())
		{
			fprintf(pfCSF, "    Features = %s\n", pSectionUnlock->getUnlockFeatures().c_str());
		}
		if (pSectionUnlock->getUnlockUID().length())
		{
			fprintf(pfCSF, "    UID = %s\n", pSectionUnlock->getUnlockUID().c_str());
		}
	}

	fclose(pfCSF);
}

//! Returns the number of microseconds since 00:00 1-1-2000. In actuality, the timestamp
//! is only accurate to seconds, and is simply extended out to microseconds.
//!
//! \todo Use the operating system's low-level functions to get a true microsecond
//!		timestamp, instead of faking it like we do now.
//! \bug The timestamp might be off an hour.
uint64_t iMXImage::getTimestamp()
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

void iMXImage::debugPrint() const
{
    const_section_iterator_t it = beginSection();
    for (; it != endSection(); ++it)
    {
        const Section *section = *it;
        section->debugPrint();
    }
}

void iMXImage::Section::debugPrint() const
{
	Log::log(Logger::INFO2, "\tSection: 0x%x\n", getIdentifier());
}

void iMXImage::generateCSTfolderPath() {
	if (m_cstFolderPath == "")
	{
		#ifdef _WIN32
				m_cstFolderPath += "cst.exe";
		#elif __APPLE__
				m_cstFolderPath += "cst.app";
		#elif linux
				m_cstFolderPath += "cst";
		#endif
	}
	else 
	{
		#ifdef _WIN32
				m_cstFolderPath += "/windows/cst.exe";
		#elif __APPLE__
				m_cstFolderPath += "/mac/cst.app";
		#elif linux
				m_cstFolderPath += "/linux64/cst";
		#endif
	}
}