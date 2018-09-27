/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_iMXImage_h_)
#define _iMXImage_h_

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include "BootImage.h"
#include "rijndael.h"
#include "smart_ptr.h"
#include "AESKey.h"
#include "StExecutableImage.h"
#include "mbedtls/sha256.h"
#include "DataSource.h"
#include "ELFSourceFile.h"
#include "IVTDataSource.h"
#include "DataSourceImager.h"

namespace elftosb
{
//! An AES-256 cipher block is bytes.
typedef uint8_t cipher_block_t[16];

//! A SHA-256 digest is 256 bits, or 32 bytes.
typedef uint8_t sha256_digest_t[32];

//! Unique identifier type for a section.
typedef uint32_t section_id_t;

//! An array of strings.
typedef std::vector<std::string> string_vector_t;

/*!
 * \brief Class to manage SB2 image files.
 *
 * Initially this class will only support generation of boot images, but
 * its design will facilitate the addition of the ability to read an
 * image and examine its contents.
 *
 * A boot image is composed of the following regions:
 * - Header
 * - HMAC on header
 * - Key blob
 * - Section data (incl sign section)
 * - Authentication
 *
 * Multiple sections are within a boot image are fully supported. Two general types
 * of sections are supported with this class. Bootable sections, represented by the
 * iMXImage::BootSection class, contain a sequence of commands to be
 * interpreted by the boot ROM. Data sections are represented by the
 * iMXImage::DataSection class and can contain any arbitrary data.
 *
 * An image can either be encrypted or unencrypted. The image uses a session key,
 * or DEK (data encryption key), and the key dictionary to support any number of keys
 * using a single image. The header and section table are always unencrypted even
 * in encrypted images. This allows host utilities to access the individual
 * sections without needing to have access to an encryption key.
 *
 * To construct a boot image, first create an instance of iMXImage. Then
 * create instances of the iMXImage::BootSection or iMXImage::DataSection
 * for each of the sections in the image. For bootable sections, create and add
 * the desired boot command objects. These are all subclasses of
 * iMXImage::BootCommand.
 *
 * If the boot image is to be encrypted, you need to add keys, which are instances
 * of the AES128Key class. If no keys are added, the entire boot image will be unencrypted.
 *
 * When the image is fully constructed, it can be written to any std::ostream with
 * a call to writeToStream(). The same image can be written to streams any
 * number of times.
 */
class iMXImage : public BootImage
{
public:
    
    //! \brief shifters for the flag constants
    enum
    {
        ROM_DISPLAY_PROGRESS_SHIFT = 0,
        ROM_VERBOSE_PROGRESS_SHIFT = 1,
        ROM_ENCRYPTED_SHIFT = 2,
        ROM_SIGNED_SHIFT = 3,
		ROM_PLUGIN_SHIFT = 4
    };

    //! \brief Flag constants for the m_flags field of #elftosb::SB2BootImage::sb2_header_t.
    enum
    {
        ROM_DISPLAY_PROGRESS = (1 << ROM_DISPLAY_PROGRESS_SHIFT), //!< Print progress reports.
        ROM_VERBOSE_PROGRESS = (1 << ROM_VERBOSE_PROGRESS_SHIFT), //!< Progress reports are verbose.
		ROM_ENCRYPTED = (1 << ROM_ENCRYPTED_SHIFT), //!< The SB file is encrypted if set. 
		ROM_SIGNED = (1 << ROM_SIGNED_SHIFT),    //!< The SB file is signed. 
		ROM_PLUGIN = (1 << ROM_PLUGIN_SHIFT)    //!< The SB file is a plugin image.
    };

public:
    // Forward declaration.
    class Section;

 /*!
  * \brief Base class for a section of an Encore boot image.
  *
  * Provides methods to manage the unique identifier that all sections have, and
  * to set the boot image object which owns the section. There are also virtual
  * methods to get header flags and fill in the header used in the section
  * table. Subclasses must implement at least fillSectionHeader().
  */
    class Section 
    {
    public:
        //! \brief Default constructor.
        Section():
            m_identifier(0)
            , m_flags(0)
        {
        }

        //! \brief Constructor taking the unique identifier for this section.
        Section(uint32_t identifier):
            m_identifier(identifier)
            , m_flags(0)
        {
        }

		//! \name Owner image
		//@{
		//! \brief Called when the section is added to an image.
		void setImage(iMXImage *image) { m_image = image; }
		//! \brief Returns a pointer to the image that this section belongs to.
		iMXImage *getImage() const { return m_image; }
		//@}
		

		//! \name Identifier
        //@{
        inline void setIdentifier(uint32_t identifier) { m_identifier = identifier; }
        inline uint32_t getIdentifier() const { return m_identifier; }
        //@}

        //@}
		void setHdrVersion(std::string HdrVersion) { m_HdrVersion = HdrVersion; }
		std::string getHdrVersion() { return m_HdrVersion; }

		void setHdrSecConfig(std::string HdrSecConfig) { m_HdrSecConfig = HdrSecConfig; }
		std::string getHdrSecConfig() { return m_HdrSecConfig; }

		void setHdrUID(std::string HdrUID) { m_HdrUID = HdrUID; }
		std::string getHdrUID() { return m_HdrUID; }

		void setHdrCustCode(std::string HdrCustCode) { m_HdrCustCode = HdrCustCode; }
		std::string getHdrCustCode() { return m_HdrCustCode; }

		void setHdrHashAlg(std::string HdrHashAlg) { m_HdrHashAlg = HdrHashAlg; }
		std::string getHdrHashAlg() { return m_HdrHashAlg; }

		void setHdrEng(std::string HdrEng) { m_HdrEng = HdrEng; }
		std::string getHdrEng() { return m_HdrEng; }

		void setHdrEngConfig(std::string HdrEngConfig) { m_HdrEngConfig = HdrEngConfig; }
		std::string getHdrEngConfig() { return m_HdrEngConfig; }

		void setHdrCertFormat(std::string HdrCertFormat) { m_HdrCertFormat = HdrCertFormat; }
		std::string getHdrCertFormat() { return m_HdrCertFormat; }

		void setHdrSigFormat(std::string HdrSigFormat) { m_HdrSigFormat = HdrSigFormat; }
		std::string getHdrSigFormat() { return m_HdrSigFormat; }


		void setInsSRKFile(std::string InsSRKFile) { m_InsSRKFile = InsSRKFile; }
		std::string getInsSRKFile() { return m_InsSRKFile; }

		void setInsSRKSrcIdx(std::string InsSRKSrcIdx) { m_InsSRKSrcIdx = InsSRKSrcIdx; }
		std::string getInsSRKSrcIdx() { return m_InsSRKSrcIdx; }

		void setInsSRKHashAlg(std::string InsSRKHashAlg) { m_InsSRKHashAlg = InsSRKHashAlg; }
		std::string getInsSRKHashAlg() { return m_InsSRKHashAlg; }


		void setInsCSFKFile(std::string InsCSFKFile) { m_InsCSFKFile = InsCSFKFile; }
		std::string getInsCSFKFile() { return m_InsCSFKFile; }

		void setInsCSFKCertFormat(std::string InsCSFKCertFormat) { m_InsCSFKCertFormat = InsCSFKCertFormat; }
		std::string getInsCSFKCertFormat() { return m_InsCSFKCertFormat; }


		void setInsNOCAKFile(std::string InsNOCAKFile) { m_InsNOCAKFile = InsNOCAKFile; }
		std::string getInsNOCAKFile() { return m_InsNOCAKFile; }

		void setInsNOCAKCertFormat(std::string InsNOCAKCertFormat) { m_InsNOCAKCertFormat = InsNOCAKCertFormat; }
		std::string getInsNOCAKCertFormat() { return m_InsNOCAKCertFormat; }


		void setAutCSFEng(std::string AutCSFEng) { m_AutCSFEng = AutCSFEng; }
		std::string getAutCSFEng() { return m_AutCSFEng; }

		void setAutCSFEngConfig(std::string AutCSFEngConfig) { m_AutCSFEngConfig = AutCSFEngConfig; }
		std::string getAutCSFEngConfig() { return m_AutCSFEngConfig; }

		void setAutCSFSigFormat(std::string AutCSFSigFormat) { m_AutCSFSigFormat = AutCSFSigFormat; }
		std::string getAutCSFSigFormat() { return m_AutCSFSigFormat; }


		void setInsKeyFile(std::string InsKeyFile) { m_InsKeyFile = InsKeyFile; }
		std::string getInsKeyFile() { return m_InsKeyFile; }

		void setInsKeyVfyIdx(std::string InsKeyVfyIdx) { m_InsKeyVfyIdx = InsKeyVfyIdx; }
		std::string getInsKeyVfyIdx() { return m_InsKeyVfyIdx; }

		void setInsKeyCertFormat(std::string InsKeyCertFormat) { m_InsKeyCertFormat = InsKeyCertFormat; }
		std::string getInsKeyCertFormat() { return m_InsKeyCertFormat; }

		void setInsKeyTgtIdx(std::string InsKeyTgtIdx) { m_InsKeyTgtIdx = InsKeyTgtIdx; }
		std::string getInsKeyTgtIdx() { return m_InsKeyTgtIdx; }

		void setInsKeyHashAlg(std::string InsKeyHashAlg) { m_InsKeyHashAlg = InsKeyHashAlg; }
		std::string getInsKeyHashAlg() { return m_InsKeyHashAlg; }


		void setAutDatVfyIdx(std::string AutDatVfyIdx) { m_AutDatVfyIdx = AutDatVfyIdx; }
		std::string getAutDatVfyIdx() { return m_AutDatVfyIdx; }

		void setAutDatEng(std::string AutDatEng) { m_AutDatEng = AutDatEng; }
		std::string getAutDatEng() { return m_AutDatEng; }

		void setAutDatEngConfig(std::string AutDatEngConfig) { m_AutDatEngConfig = AutDatEngConfig; }
		std::string getAutDatEngConfig() { return m_AutDatEngConfig; }

		void setAutDatSigFormat(std::string AutDatSigFormat) { m_AutDatSigFormat = AutDatSigFormat; }
		std::string getAutDatSigFormat() { return m_AutDatSigFormat; }

		void setAutDatBinding(std::string AutDatBinding) { m_AutDatBinding = AutDatBinding; }
		std::string getAutDatBinding() { return m_AutDatBinding; }

		void setSecKeyVfyIdx(int idx) { m_secretKeyVfyIdx = idx; }
		int getSecKeyVfyIdx() { return m_secretKeyVfyIdx; }

		void setSecKeyTgtIdx(int idx) { m_secretKeyTgtIdx = idx; }
		int getSecKeyTgtIdx() { return m_secretKeyTgtIdx; }

		void setSecKeyName(std::string keyName) { m_secretKeyName = keyName; }
        std::string getSecKeyName() { return m_secretKeyName; }

		void setSecKeyLength(int len) { m_secretKeyLength = len; }
		int getSecKeyLength() {return m_secretKeyLength; }


		void setDecryptEngine(std::string eng) { m_decryptEngine = eng; }
		std::string getDecryptEngine() { return m_decryptEngine; }

		void setDecryptEngConfig(std::string DecryptEngConfig) { m_decryptEngConfig = DecryptEngConfig; }
		std::string getDecryptEngConfig() { return m_decryptEngConfig; }

		void setDecryptVfyIdx(int idx) { m_decryptVfyIdx = idx; }
		int getDecryptVfyIdx() { return m_decryptVfyIdx; }

		void setDecryptMacBytes(int macBytes) { m_decryptMacBytes = macBytes; }
		int getDecryptMacBytes() { return m_decryptMacBytes; }


		void setSetMIDBank(std::string bank) { m_setMIDBank = bank; }
		std::string getSetMIDBank() { return m_setMIDBank; }

		void setSetMIDRow(std::string row) { m_setMIDRow = row; }
		std::string getSetMIDRow() { return m_setMIDRow; }

		void setSetMIDFuse(std::string fuse) { m_setMIDFuse = fuse; }
		std::string getSetMIDFuse() { return m_setMIDFuse; }

		void setSetMIDBits(std::string bits) { m_setMIDBits = bits; }
		std::string getSetMIDBits() { return m_setMIDBits; }


		void setSetEngHashAlg(std::string hash) { m_setEngHashAlg = hash; }
		std::string getSetEngHashAlg() { return m_setEngHashAlg; }

		void setSetEngEng(std::string eng) { m_setEngEng = eng; }
		std::string getSetEngEng() { return m_setEngEng; }

		void setSetEngConfig(std::string engConfig) { m_setEngEngConfig = engConfig; }
		std::string getSetEngConfig() { return m_setEngEngConfig; }


		void setInitEng(std::string eng) { m_initEng = eng; }
		std::string getInitEng() { return m_initEng; }

		void setInitFeatures(std::string feature) { m_initFeatures = feature; }
		std::string getInitFeatures() { return m_initFeatures; }


		void setUnlockEng(std::string eng) { m_unlockEng = eng; }
		std::string getUnlockEng() { return m_unlockEng; }

		void setUnlockFeatures(std::string feature) { m_unlockFeatures = feature; }
		std::string getUnlockFeatures() { return m_unlockFeatures; }

		void setUnlockUID(std::string uid) { m_unlockUID = uid; }
		std::string getUnlockUID() { return m_unlockUID; }


		void setKib0Key(const char * key) { m_kib0_key = key; }
		void setKib0iv(const char * iv) { m_kib0_iv = iv; }
		void setRegion0Key(const char * key) { m_region0_key = key; }
		void setRegion0iv(const char * iv) { m_region0_iv = iv; }
		void setRegion0Lock(uint8_t lock) { m_region0_lock = lock; }
		void setRegion0facRegion0Start(uint32_t start) { m_region0_fac_region0_start = start; }
		void setRegion0facRegion0Length(uint32_t len) { m_region0_fac_region0_length = len; }
		void setRegion0facRegion0Permission(uint32_t perm) { m_region0_fac_region0_permission = perm; }
		void setRegion0facRegion1Start(uint32_t start) { m_region0_fac_region1_start = start; }
		void setRegion0facRegion1Length(uint32_t len) { m_region0_fac_region1_length = len; }
		void setRegion0facRegion1Permission(uint32_t perm) { m_region0_fac_region1_permission = perm; }
		void setRegion0facRegion2Start(uint32_t start) { m_region0_fac_region2_start = start; }
		void setRegion0facRegion2Length(uint32_t len) { m_region0_fac_region2_length = len; }
		void setRegion0facRegion2Permission(uint32_t perm) { m_region0_fac_region2_permission = perm; }


		void setKib1Key(const char * key) { m_kib1_key = key; }
		void setKib1iv(const char * iv) { m_kib1_iv = iv; }
		void setRegion1Key(const char * key) { m_region1_key = key; }
		void setRegion1iv(const char * iv) { m_region1_iv = iv; }
		void setRegion1Lock(uint8_t lock) { m_region1_lock = lock; }
		void setRegion1facRegion0Start(uint32_t start) { m_region1_fac_region0_start = start; }
		void setRegion1facRegion0Length(uint32_t len) { m_region1_fac_region0_length = len; }
		void setRegion1facRegion0Permission(uint32_t perm) { m_region1_fac_region0_permission = perm; }
		void setRegion1facRegion1Start(uint32_t start) { m_region1_fac_region1_start = start; }
		void setRegion1facRegion1Length(uint32_t len) { m_region1_fac_region1_length = len; }
		void setRegion1facRegion1Permission(uint32_t perm) { m_region1_fac_region1_permission = perm; }
		void setRegion1facRegion2Start(uint32_t start) { m_region1_fac_region2_start = start; }
		void setRegion1facRegion2Length(uint32_t len) { m_region1_fac_region2_length = len; }
		void setRegion1facRegion2Permission(uint32_t perm) { m_region1_fac_region2_permission = perm; }


		//! \brief Print out a string representation of the object.
		virtual void debugPrint() const;
        //@}

    protected:
        uint32_t m_identifier;    //!< Unique identifier for this section.
        uint32_t m_flags;         //!< Section flags set by the user.
		iMXImage *m_image; //!< The image to which this section belongs.

		std::string m_HdrVersion;
		std::string m_HdrSecConfig;
		std::string m_HdrUID;
		std::string m_HdrCustCode;
		std::string m_HdrHashAlg;
		std::string m_HdrEng;
		std::string m_HdrEngConfig;
		std::string m_HdrCertFormat;
		std::string m_HdrSigFormat;
		std::string m_InsSRKFile;
		std::string m_InsSRKSrcIdx;
		std::string m_InsSRKHashAlg;
		std::string m_InsCSFKFile;
		std::string m_InsCSFKCertFormat;
		std::string m_InsNOCAKFile;
		std::string m_InsNOCAKCertFormat;
		std::string m_AutCSFEng;
		std::string m_AutCSFEngConfig;
		std::string m_AutCSFSigFormat;
		std::string m_InsKeyFile;
		std::string m_InsKeyVfyIdx;
		std::string m_InsKeyCertFormat;
		std::string m_InsKeyTgtIdx;
		std::string m_InsKeyHashAlg;
		std::string m_AutDatVfyIdx;
		std::string m_AutDatEng;
		std::string m_AutDatEngConfig;
		std::string m_AutDatSigFormat;
		std::string m_AutDatBinding;

		int m_secretKeyVfyIdx;
		int m_secretKeyTgtIdx;
		std::string m_secretKeyName;
		int m_secretKeyLength;

		std::string m_decryptEngine;
		std::string m_decryptEngConfig;
		int m_decryptVfyIdx;
		int m_decryptMacBytes;

		std::string m_setMIDBank;
		std::string m_setMIDRow;
		std::string m_setMIDFuse;
		std::string m_setMIDBits;

		std::string m_setEngHashAlg;
		std::string m_setEngEng;
		std::string m_setEngEngConfig;

		std::string m_initEng;
		std::string m_initFeatures;

		std::string m_unlockEng;
		std::string m_unlockFeatures;
		std::string m_unlockUID;

		std::string m_kib0_key;
		std::string m_kib0_iv;
		std::string m_region0_key;
		std::string m_region0_iv;
		uint8_t m_region0_lock;
		uint32_t m_region0_fac_region0_start;
		uint32_t m_region0_fac_region0_length;
		uint32_t m_region0_fac_region0_permission;
		uint32_t m_region0_fac_region1_start;
		uint32_t m_region0_fac_region1_length;
		uint32_t m_region0_fac_region1_permission;
		uint32_t m_region0_fac_region2_start;
		uint32_t m_region0_fac_region2_length;
		uint32_t m_region0_fac_region2_permission;

		std::string m_kib1_key;
		std::string m_kib1_iv;
		std::string m_region1_key;
		std::string m_region1_iv;
		uint8_t m_region1_lock;
		uint32_t m_region1_fac_region0_start;
		uint32_t m_region1_fac_region0_length;
		uint32_t m_region1_fac_region0_permission;
		uint32_t m_region1_fac_region1_start;
		uint32_t m_region1_fac_region1_length;
		uint32_t m_region1_fac_region1_permission;
		uint32_t m_region1_fac_region2_start;
		uint32_t m_region1_fac_region2_length;
		uint32_t m_region1_fac_region2_permission;

	};

public:
    typedef std::list<Section *> section_list_t;                     //!< List of image sections.
    typedef section_list_t::iterator section_iterator_t;             //!< Iterator over sections.
    typedef section_list_t::const_iterator const_section_iterator_t; //!< Const iterator over sections.

public:
    //! \brief Default constructor.
    iMXImage();

    //! \brief Destructor.
    virtual ~iMXImage();

	//! \brief Specify the drive tag to be set in the output file header.
	virtual void setDriveTag(uint16_t tag) {};

	//! \brief Specify the drive tag to be set in the output file header.
	virtual void setBuildNumber(uint16_t tag) {};

    void setStartAddress(unsigned int addr) {m_startAddress = addr;}
	void setIVTOffset(unsigned int offset) { m_ivtOffset = offset; }
    void setInitialLoadSize(unsigned int loadSize) {m_initialLoadSize = loadSize;}
    void setDCDFilePath(const char * path) {m_dcdFilePath = path;}
	void setOutputFilePath(const char * filepath) {m_outputFilePath = filepath;}
	void createNoPaddingImage(DataSourceImager *dsImager);
	DataSourceImager * createDataSourceImager(DataSource *d1, DataSource *d2, DataSource *d3, DataSource *d4, DataSource *d5, DataSource *d6);
	void setSourceFile(SourceFile *pSourceFile) { m_pSourceFile = pSourceFile; }
	void setCSTFolderPath(const char * cstPath) { m_cstFolderPath = cstPath; }
	void setEntryPointAddress(unsigned int entry) { m_entryPointAddress = entry; }

    //! \name Sections
    //@{
    void addSection(Section *newSection);
    inline unsigned sectionCount() const { return (unsigned)m_sections.size(); }
    inline section_iterator_t beginSection() { return m_sections.begin(); }
    inline section_iterator_t endSection() { return m_sections.end(); }
    inline const_section_iterator_t beginSection() const { return m_sections.begin(); }
    inline const_section_iterator_t endSection() const { return m_sections.end(); }
    section_iterator_t findSection(Section *section);
    //@}

    //! \brief The image is encrypted if there is at least one key.
    inline bool isEncrypted() const { return (getFlags() & ROM_ENCRYPTED); }
    inline bool isSigned() const { return (getFlags() & ROM_SIGNED || getFlags() & ROM_ENCRYPTED); }
	inline bool isPlugin() const { return (getFlags() & ROM_PLUGIN); }
    //@}

    //! \name Versions
    //@{
    virtual void setProductVersion(const version_t &version);
    virtual void setComponentVersion(const version_t &version);
    //@}

    //! \name Flags
    //@{
    inline void setFlags(uint16_t flags) { m_headerFlags = flags; }
    inline uint32_t getFlags() const { return m_headerFlags; }
    //@}

    //! \brief Returns the preferred ".sb" extension for Encore boot images.
    virtual std::string getFileExtension() const { return ".bin"; }
    //! \name Output
    //@{
    //! \brief Write the boot image to an output stream.
    virtual void writeToStream(std::ostream &stream);
    //@}

    //! \brief Print out a string representation of the object.
    virtual void debugPrint() const;

protected:
	uint16_t m_headerFlags;       //!< Flags field in the boot image header.
	version_t m_productVersion;   //!< Product version.
    version_t m_componentVersion; //!< Component version.
    section_list_t m_sections;    //!< Sections contained in this image.
    unsigned int m_startAddress;
	unsigned int m_ivtOffset;
    unsigned int m_initialLoadSize;
	const char * m_dcdFilePath;
	const char * m_outputFilePath;
	SourceFile *m_pSourceFile;
	unsigned m_keyBlobAddress;
	std::string m_cstFolderPath;
	unsigned int m_entryPointAddress;

    uint64_t getTimestamp();
    unsigned getPadBlockCountForSection(Section *section, unsigned offset);
	void generateInputCSF(const char * binFile, const char * csfFile, IVTDataSource *pIVT, BootDataSource *pBD, UnmappedDataSource * pDCD, DataSource *pElf);
	void setKeyBlobAddress(unsigned address) { m_keyBlobAddress = address; }
	unsigned getKeyBlobAddress() { return m_keyBlobAddress; }
	void generateCSTfolderPath();

};
}; // namespace elftosb

#endif // _iMXImage_h_
