/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "iMXImageGenerator.h"
#include "Logging.h"
#include "OptionContext.h"

#define kFlagsOption "flags"
#define kstartAddressOption "startAddress"
#define kIVTOffsetOption "ivtOffset"
#define kInitialLoadSizeOption "initialLoadSize"
#define kDCDFilePathOption "DCDFilePath"
#define kSectionFlagsOption "sectionFlags"
#define kProductVersionOption "productVersion"
#define kComponentVersionOption "componentVersion"
#define kCSTFolderPathOption "cstFolderPath"
#define kEntryPointAddressOption "entryPointAddress"

#define kHdrVersion "Header_Version"
#define kHdrSecConfig "Header_SecurityConfiguration"
#define kHdrUID "Header_UID"
#define kHdrCustCode "Header_CustomerCode"
#define kHdrHashAlg "Header_HashAlgorithm"
#define kHdrEng "Header_Engine"
#define kHdrEngConfig "Header_EngineConfiguration"
#define kHdrCertFormat "Header_CertificateFormat"
#define kHdrSigFormat "Header_SignatureFormat"

#define kInsSRKFile "InstallSRK_Table"
#define kInsSRKSrcIdx "InstallSRK_SourceIndex"
#define kInsSRKHashAlg "InstallSRK_HashAlgorithm"

#define kInsCSFKFile "InstallCSFK_File"
#define kInsCSFKCertFormat "InstallCSFK_CertificateFormat"

#define kInsNOCAKFile "InstallNOCAK_File"
#define kInsNOCAKCertFormat "InstallNOCAK_CertificateFormat"

#define kAutCSFEng "AuthenticateCSF_Engine"
#define kAutCSFEngConfig "AuthenticateCSF_EngineConfiguration"
#define kAutCSFSigFormat "AuthenticateCSF_SignatureFormat"

#define kInsKeyFile "InstallKey_File"
#define kInsKeyVfyIdx "InstallKey_VerificationIndex"
#define kInsKeyCertFormat "InstallKey_CertificateFormat"
#define kInsKeyTgtIdx "InstallKey_TargetIndex"
#define kInsKeyHashAlg "InstallKey_HashAlgorithm"

#define kAutDatVfyIdx "AuthenticateData_VerificationIndex"
#define kAutDatEng "AuthenticateData_Engine"
#define kAutDatEngConfig "AuthenticateData_EngineConfiguration"
#define kAutDatSigFormat "AuthenticateData_SignatureFormat"
#define kAutDatBinding "AuthenticateData_Binding"

#define kSecKeyName "SecretKey_Name"
#define kSecKeyLength "SecretKey_Length"
#define kSecKeyVerIdxOption "SecretKey_VerifyIndex"
#define kSecKeyTgtIdxOption "SecretKey_TargetIndex"

#define kDecryptEng "Decrypt_Engine"
#define kDecryptEngConfig "Decrypt_EngineConfiguration"
#define kDecryptVefIdx "Decrypt_VerifyIndex"
#define kDecryptMacBytes "Decrypt_MacBytes"

#define kSetMIDBank "SetMID_Bank"
#define kSetMIDRow "SetMID_Row"
#define kSetMIDFuse "SetMID_Fuse"
#define kSetMIDBits "SetMID_Bits"

#define kSetEngHashAlg "SetEngine_HashAlgorithm"
#define kSetEngEng "SetEngine_Engine"
#define kSetEngEngConfig "SetEngine_EngineConfiguration"

#define kInitEng "Init_Engine"
#define kInitFeatures "Init_Features"

#define kUnlockEng "Unlock_Engine"
#define kUnlockFeatures "Unlock_features"
#define kUnlockUID "Unlock_UID"

#define kKib0Key "kib0_key"
#define kKib0iv "kib0_iv"
#define kRegion0Key "region0_key"
#define kRegion0iv "region0_iv"
#define kRegion0Lock "region0_lock"
#define kRegion0facRegion0Start "region0_fac_region0_start"
#define kRegion0facRegion0Length "region0_fac_region0_length"
#define kRegion0facRegion0Permission "region0_fac_region0_permission"
#define kRegion0facRegion1Start "region0_fac_region1_start"
#define kRegion0facRegion1Length "region0_fac_region1_length"
#define kRegion0facRegion1Permission "region0_fac_region1_permission"
#define kRegion0facRegion2Start "region0_fac_region2_start"
#define kRegion0facRegion2Length "region0_fac_region2_length"
#define kRegion0facRegion2Permission "region0_fac_region2_permission"

#define kKib1Key "kib1_key"
#define kKib1iv "kib1_iv"
#define kRegion1Key "region1_key"
#define kRegion1iv "region1_iv"
#define kRegion1Lock "region1_lock"
#define kRegion1facRegion0Start "region1_fac_region0_start"
#define kRegion1facRegion0Length "region1_fac_region0_length"
#define kRegion1facRegion0Permission "region1_fac_region0_permission"
#define kRegion1facRegion1Start "region1_fac_region1_start"
#define kRegion1facRegion1Length "region1_fac_region1_length"
#define kRegion1facRegion1Permission "region1_fac_region1_permission"
#define kRegion1facRegion2Start "region1_fac_region2_start"
#define kRegion1facRegion2Length "region1_fac_region2_length"
#define kRegion1facRegion2Permission "region1_fac_region2_permission"

using namespace elftosb;

BootImage *iMXImageGenerator::generate()
{
    iMXImage *image = new iMXImage();

    // process each output section
    section_vector_t::iterator it = m_sections.begin();
    for (; it != m_sections.end(); ++it)
    {
        OutputSection *section = *it;

        OperationSequenceSection *opSection = dynamic_cast<OperationSequenceSection *>(section);
        if (opSection)
        {
            processOperationSection(opSection, image);
            continue;
        }

        Log::log(Logger::WARNING, "warning: unexpected output section type\n");
    }

    // handle global options that affect the image
    processOptions(image);

    return image;
}

void iMXImageGenerator::processOptions(iMXImage *image)
{
    // bail if no option context was set
    if (!m_options)
    {
        return;
    }

	// check for flags 
    if (m_options->hasOption(kFlagsOption))
    {
		const Value *value = m_options->getOption(kFlagsOption);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			image->setFlags(static_cast<uint16_t>(strtoll(*stringValue, NULL, 0)));
		}
		else if (intValue)
        {
            image->setFlags(static_cast<uint16_t>(intValue->getValue()));
        }
        else
        {
            Log::log(Logger::WARNING, "warning: flags option is an unexpected type\n");
        }
    }

	// Check for start-address 
	if (m_options->hasOption(kstartAddressOption))
	{
		const Value *value = m_options->getOption(kstartAddressOption);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			image->setStartAddress(static_cast<uint32_t>(strtoll(*stringValue, NULL, 0)));
		}
		else if (intValue)
		{
		    image->setStartAddress(static_cast<uint32_t>(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: start-address option is an unexpected type\n");
		}
	}

	// Check for ivt-offset 
	if (m_options->hasOption(kIVTOffsetOption))
	{
		const Value *value = m_options->getOption(kIVTOffsetOption);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			image->setIVTOffset(static_cast<uint32_t>(strtoll(*stringValue, NULL, 0)));
		}
		else if (intValue)
		{
			image->setIVTOffset(static_cast<uint32_t>(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: ivt-offset option is an unexpected type\n");
		}
	}

	// Check for initial-load-size 
	if (m_options->hasOption(kInitialLoadSizeOption))
	{
		const Value *value = m_options->getOption(kInitialLoadSizeOption);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			image->setInitialLoadSize(static_cast<uint32_t>(strtoll(*stringValue, NULL, 0)));
		}
		else if (intValue)
		{
			image->setInitialLoadSize(static_cast<uint32_t>(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: initial-load-size option is an unexpected type\n");
		}
	}

	// Check for DCD File Path
	if (m_options->hasOption(kDCDFilePathOption))
	{
		const Value *value = m_options->getOption(kDCDFilePathOption);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			image->setDCDFilePath(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: DCD File path option is an unexpected type\n");
		}
	}

	// Check for CST folder path
	if (m_options->hasOption(kCSTFolderPathOption))
	{
		const Value *value = m_options->getOption(kCSTFolderPathOption);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			image->setCSTFolderPath(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: cst.exe File path option is an unexpected type\n");
		}
	}

	// Check for Entry-point address
	if (m_options->hasOption(kEntryPointAddressOption))
	{
		const Value *value = m_options->getOption(kEntryPointAddressOption);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			image->setEntryPointAddress(static_cast<uint32_t>(strtoll(*stringValue, NULL, 0)));
		}
		else if (intValue)
		{
			image->setEntryPointAddress(static_cast<uint32_t>(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: entry-point address option is an unexpected type\n");
		}
	}

    // handle common options
    processVersionOptions(image);
    processBuildNumberOption(image);
}

void iMXImageGenerator::processSectionOptions(iMXImage::Section *imageSection,
                                                     OutputSection *modelSection)
{
    // Get options context for this output section.
    const OptionContext *context = modelSection->getOptions();
    if (!context)
    {
        return;
    }

/* Beginning of header */
	if (context->hasOption(kHdrVersion))
	{
		const Value *value = context->getOption(kHdrVersion);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setHdrVersion(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: csf header version is an unexpected type\n");
		}
	}

	if (context->hasOption(kHdrSecConfig))
	{
		const Value *value = context->getOption(kHdrSecConfig);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setHdrSecConfig(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: csf header security configuration is an unexpected type\n");
		}
	}

	if (context->hasOption(kHdrUID))
	{
		const Value *value = context->getOption(kHdrUID);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setHdrUID(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: csf header UID is an unexpected type\n");
		}
	}

	if (context->hasOption(kHdrCustCode))
	{
		const Value *value = context->getOption(kHdrCustCode);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setHdrCustCode(*stringValue);
		}
		else if (intValue)
		{
			imageSection->setHdrCustCode(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: csf header customer code is an unexpected type\n");
		}
	}

	if (context->hasOption(kHdrEng))
	{
		const Value *value = context->getOption(kHdrEng);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setHdrEng(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: csf header engine is an unexpected type\n");
		}
	}

	if (context->hasOption(kHdrEngConfig))
	{
		const Value *value = context->getOption(kHdrEngConfig);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setHdrEngConfig(*stringValue);
		}
		else if (intValue) 
		{
			imageSection->setHdrEngConfig(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: csf header engine configuration is an unexpected type\n");
		}
	}

	if (context->hasOption(kHdrCertFormat))
	{
		const Value *value = context->getOption(kHdrCertFormat);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setHdrCertFormat(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: csf header certificate format is an unexpected type\n");
		}
	}

	if (context->hasOption(kHdrSigFormat))
	{
		const Value *value = context->getOption(kHdrSigFormat);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setHdrSigFormat(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: csf header signature format is an unexpected type\n");
		}
	}

	if (context->hasOption(kHdrHashAlg))
	{
		const Value *value = context->getOption(kHdrHashAlg);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setHdrHashAlg(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: csf header hash algorithm is an unexpected type\n");
		}
	}

/* Beginning of Install SRK command */
	if (context->hasOption(kInsSRKFile))
	{
		const Value *value = context->getOption(kInsSRKFile);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInsSRKFile(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: srk file is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsSRKSrcIdx))
	{
		const Value *value = context->getOption(kInsSRKSrcIdx);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setInsSRKSrcIdx(*stringValue);
		}
		else if (intValue)
		{
			imageSection->setInsSRKSrcIdx(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: srk source index is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsSRKHashAlg))
	{
		const Value *value = context->getOption(kInsSRKHashAlg);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInsSRKHashAlg(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: srk hash algorithm is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsCSFKFile))
	{
		const Value *value = context->getOption(kInsCSFKFile);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInsCSFKFile(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: install csf key file is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsCSFKCertFormat))
	{
		const Value *value = context->getOption(kInsCSFKCertFormat);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInsCSFKCertFormat(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: install csf key certificate format is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsNOCAKFile))
	{
		const Value *value = context->getOption(kInsNOCAKFile);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInsNOCAKFile(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: install no CA key file is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsNOCAKCertFormat))
	{
		const Value *value = context->getOption(kInsNOCAKCertFormat);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInsNOCAKCertFormat(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: install no cak certificate format is an unexpected type\n");
		}
	}

	if (context->hasOption(kAutCSFEng))
	{
		const Value *value = context->getOption(kAutCSFEng);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setAutCSFEng(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: authenticate csf engine is an unexpected type\n");
		}
	}

	if (context->hasOption(kAutCSFEngConfig))
	{
		const Value *value = context->getOption(kAutCSFEngConfig);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setAutCSFEngConfig(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: authenticate csf engine configuration is an unexpected type\n");
		}
	}

	if (context->hasOption(kAutCSFSigFormat))
	{
		const Value *value = context->getOption(kAutCSFSigFormat);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setAutCSFSigFormat(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: authenticate csf signature format is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsKeyFile))
	{
		const Value *value = context->getOption(kInsKeyFile);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInsKeyFile(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: install key file is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsKeyVfyIdx))
	{
		const Value *value = context->getOption(kInsKeyVfyIdx);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setInsKeyVfyIdx(*stringValue);
		}
		else if (intValue) 
		{
			imageSection->setInsKeyVfyIdx(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: install key verification index is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsKeyCertFormat))
	{
		const Value *value = context->getOption(kInsKeyCertFormat);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInsKeyCertFormat(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: install key certificate format is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsKeyTgtIdx))
	{
		const Value *value = context->getOption(kInsKeyTgtIdx);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setInsKeyTgtIdx(*stringValue);
		}
		else if (intValue) 
		{
			imageSection->setInsKeyTgtIdx(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: install key target index is an unexpected type\n");
		}
	}

	if (context->hasOption(kInsKeyHashAlg))
	{
		const Value *value = context->getOption(kInsKeyHashAlg);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInsKeyHashAlg(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: install key hash algorithm is an unexpected type\n");
		}
	}

	if (context->hasOption(kAutDatVfyIdx))
	{
		const Value *value = context->getOption(kAutDatVfyIdx);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setAutDatVfyIdx(*stringValue);
		}
		else if (intValue) 
		{
			imageSection->setAutDatVfyIdx(std::to_string(intValue->getValue()));
		}
		else 
		{
			Log::log(Logger::WARNING, "warning: authenticate data verify index is an unexpected type\n");
		}
	}

	if (context->hasOption(kAutDatEng))
	{
		const Value *value = context->getOption(kAutDatEng);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setAutDatEng(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: authenticate data engine is an unexpected type\n");
		}
	}

	if (context->hasOption(kAutDatEngConfig))
	{
		const Value *value = context->getOption(kAutDatEngConfig);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setAutDatEngConfig(*stringValue);
		}
		else if (intValue) 
		{
			imageSection->setAutDatEngConfig(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: authenticate data engine configuration is an unexpected type\n");
		}
	}

	if (context->hasOption(kAutDatSigFormat))
	{
		const Value *value = context->getOption(kAutDatSigFormat);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setAutDatSigFormat(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: authenticate data signature format is an unexpected type\n");
		}
	}

	if (context->hasOption(kAutDatBinding))
	{
		const Value *value = context->getOption(kAutDatBinding);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setAutDatBinding(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: authenticate data binding is an unexpected type\n");
		}
	}

    if (context->hasOption(kSecKeyName))
    {
        const Value *value = context->getOption(kSecKeyName);
        const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
        if (stringValue)
        {
			imageSection->setSecKeyName(*stringValue);
        }
        else
        {
            Log::log(Logger::WARNING, "warning: secret key name is an unexpected type\n");
        }
    }

	if (context->hasOption(kSecKeyLength))
	{
		const Value *value = context->getOption(kSecKeyLength);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setSecKeyLength(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setSecKeyLength(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: secret key length is an unexpected type\n");
		}
	}

    if (context->hasOption(kSecKeyVerIdxOption))
    {
        const Value *value = context->getOption(kSecKeyVerIdxOption);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setSecKeyVfyIdx(atoi(*stringValue));
		}
	    else if (intValue)
        {
            imageSection->setSecKeyVfyIdx(intValue->getValue());
        }
        else
        {
            Log::log(Logger::WARNING, "warning: secret Key verify index is an unexpected type\n");
        }
    }

	if (context->hasOption(kSecKeyTgtIdxOption))
	{
		const Value *value = context->getOption(kSecKeyTgtIdxOption);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setSecKeyTgtIdx(atoi(*stringValue));
		}
	    else if (intValue)
		{
			imageSection->setSecKeyTgtIdx(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: secret key target index is an unexpected type\n");
		}
	}

    if (context->hasOption(kDecryptEng))
    {
        const Value *value = context->getOption(kDecryptEng);
        const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
        if (stringValue)
        {
			imageSection->setDecryptEngine(*stringValue);
        }
        else
        {
            Log::log(Logger::WARNING, "warning: decrypt engine is an unexpected type\n");
        }
    }

	if (context->hasOption(kDecryptEngConfig))
	{
		const Value *value = context->getOption(kDecryptEngConfig);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setDecryptEngConfig(*stringValue);
		}
		else if (intValue)
		{
			imageSection->setDecryptEngConfig(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: decrypt engine configuration is an unexpected type\n");
		}
	}

	if (context->hasOption(kDecryptVefIdx))
	{
		const Value *value = context->getOption(kDecryptVefIdx);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue) 
		{
			imageSection->setDecryptVfyIdx(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setDecryptVfyIdx(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: decrypt verify index is an unexpected type\n");
		}
	}	

	if (context->hasOption(kDecryptMacBytes))
	{
		const Value *value = context->getOption(kDecryptMacBytes);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue) 
		{
			imageSection->setDecryptMacBytes(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setDecryptMacBytes(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: decrypt Mac bytes is an unexpected type\n");
		}
	}

	if (context->hasOption(kSetMIDBank))
	{
		const Value *value = context->getOption(kSetMIDBank);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setSetMIDBank(*stringValue);
		}
		else if (intValue)
		{
			imageSection->setSetMIDBank(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: set-mid bank is an unexpected type\n");
		}
	}

	if (context->hasOption(kSetMIDRow))
	{
		const Value *value = context->getOption(kSetMIDRow);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setSetMIDRow(*stringValue);
		}
		else if (intValue)
		{
			imageSection->setSetMIDRow(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: set-mid row is an unexpected type\n");
		}
	}

	if (context->hasOption(kSetMIDFuse))
	{
		const Value *value = context->getOption(kSetMIDFuse);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setSetMIDFuse(*stringValue);
		}
		else if (intValue)
		{
			imageSection->setSetMIDFuse(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: set-mid fuse is an unexpected type\n");
		}
	}

	if (context->hasOption(kSetMIDBits))
	{
		const Value *value = context->getOption(kSetMIDBits);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setSetMIDBits(*stringValue);
		}
		else if (intValue)
		{
			imageSection->setSetMIDBits(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: set-mid bits is an unexpected type\n");
		}
	}

	if (context->hasOption(kSetEngHashAlg))
	{
		const Value *value = context->getOption(kSetEngHashAlg);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setSetEngHashAlg(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: set-engine hash algorithm is an unexpected type\n");
		}
	}

	if (context->hasOption(kSetEngEng))
	{
		const Value *value = context->getOption(kSetEngEng);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setSetEngEng(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: set-engine engine is an unexpected type\n");
		}
	}

	if (context->hasOption(kSetEngEngConfig))
	{
		const Value *value = context->getOption(kSetEngEngConfig);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setSetEngConfig(*stringValue);
		}
		else if (intValue)
		{
			imageSection->setSetEngConfig(std::to_string(intValue->getValue()));
		}
		else
		{
			Log::log(Logger::WARNING, "warning: set-engine engine configuration is an unexpected type\n");
		}
	}

	if (context->hasOption(kInitEng))
	{
		const Value *value = context->getOption(kInitEng);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInitEng(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: init engine is an unexpected type\n");
		}
	}

	if (context->hasOption(kInitFeatures))
	{
		const Value *value = context->getOption(kInitFeatures);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setInitFeatures(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: init feature is an unexpected type\n");
		}
	}

	if (context->hasOption(kUnlockEng))
	{
		const Value *value = context->getOption(kUnlockEng);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setUnlockEng(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: unlock engine is an unexpected type\n");
		}
	}

	if (context->hasOption(kUnlockFeatures))
	{
		const Value *value = context->getOption(kUnlockFeatures);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setUnlockFeatures(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: unlock feature is an unexpected type\n");
		}
	}

	if (context->hasOption(kUnlockUID))
	{
		const Value *value = context->getOption(kUnlockUID);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setUnlockUID(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: unlock UID is an unexpected type\n");
		}
	}


	// Check for kib0_key
	if (context->hasOption(kKib0Key))
	{
		const Value *value = context->getOption(kKib0Key);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setKib0Key(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: kib0_key is an unexpected type\n");
		}
	}

	// Check for kib0_iv
	if (context->hasOption(kKib0iv))
	{
		const Value *value = context->getOption(kKib0iv);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setKib0iv(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: kib0_iv is an unexpected type\n");
		}
	}

	// Check for region0_key
	if (context->hasOption(kRegion0Key))
	{
		const Value *value = context->getOption(kRegion0Key);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0Key(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_key is an unexpected type\n");
		}
	}

	// Check for region0_iv
	if (context->hasOption(kRegion0iv))
	{
		const Value *value = context->getOption(kRegion0iv);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0iv(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_iv is an unexpected type\n");
		}
	}

	// Check for region0_lock
	if (context->hasOption(kRegion0Lock))
	{
		const Value *value = context->getOption(kRegion0Lock);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0Lock(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0Lock(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_lock is an unexpected type\n");
		}
	}

	// Check for region0_fac_region0_start
	if (context->hasOption(kRegion0facRegion0Start))
	{
		const Value *value = context->getOption(kRegion0facRegion0Start);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0facRegion0Start(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0facRegion0Start(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_fac_region0_start is an unexpected type\n");
		}
	}

	// Check for region0_fac_region0_length
	if (context->hasOption(kRegion0facRegion0Length))
	{
		const Value *value = context->getOption(kRegion0facRegion0Length);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0facRegion0Length(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0facRegion0Length(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_fac_region0_length is an unexpected type\n");
		}
	}

	// Check for region0_fac_region0_permission
	if (context->hasOption(kRegion0facRegion0Permission))
	{
		const Value *value = context->getOption(kRegion0facRegion0Permission);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0facRegion0Permission(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0facRegion0Permission(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_fac_region0_permission is an unexpected type\n");
		}
	}

	// Check for region0_fac_region1_start
	if (context->hasOption(kRegion0facRegion1Start))
	{
		const Value *value = context->getOption(kRegion0facRegion1Start);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0facRegion1Start(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0facRegion1Start(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_fac_region1_start is an unexpected type\n");
		}
	}

	// Check for region0_fac_region1_length
	if (context->hasOption(kRegion0facRegion1Length))
	{
		const Value *value = context->getOption(kRegion0facRegion1Length);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0facRegion1Length(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0facRegion1Length(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_fac_region1_length is an unexpected type\n");
		}
	}

	// Check for region0_fac_region1_permission
	if (context->hasOption(kRegion0facRegion1Permission))
	{
		const Value *value = context->getOption(kRegion0facRegion1Permission);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0facRegion1Permission(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0facRegion1Permission(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_fac_region1_permission is an unexpected type\n");
		}
	}

	// Check for region0_fac_region2_start
	if (context->hasOption(kRegion0facRegion2Start))
	{
		const Value *value = context->getOption(kRegion0facRegion2Start);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0facRegion2Start(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0facRegion2Start(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_fac_region2_start is an unexpected type\n");
		}
	}

	// Check for region0_fac_region2_length
	if (context->hasOption(kRegion0facRegion2Length))
	{
		const Value *value = context->getOption(kRegion0facRegion2Length);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0facRegion2Length(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0facRegion2Length(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_fac_region2_length is an unexpected type\n");
		}
	}

	// Check for region0_fac_region2_permission
	if (context->hasOption(kRegion0facRegion2Permission))
	{
		const Value *value = context->getOption(kRegion0facRegion2Permission);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion0facRegion2Permission(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion0facRegion2Permission(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region0_fac_region2_permission is an unexpected type\n");
		}
	}

	
	// Check for kib1_key
	if (context->hasOption(kKib1Key))
	{
		const Value *value = context->getOption(kKib1Key);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setKib1Key(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: kib1_key is an unexpected type\n");
		}
	}

	// Check for kib1_iv
	if (context->hasOption(kKib1iv))
	{
		const Value *value = context->getOption(kKib1iv);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setKib1iv(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: kib1_iv is an unexpected type\n");
		}
	}

	// Check for region1_key
	if (context->hasOption(kRegion1Key))
	{
		const Value *value = context->getOption(kRegion1Key);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1Key(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_key is an unexpected type\n");
		}
	}

	// Check for region1_iv
	if (context->hasOption(kRegion1iv))
	{
		const Value *value = context->getOption(kRegion1iv);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1iv(*stringValue);
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_iv is an unexpected type\n");
		}
	}

	// Check for region1_lock
	if (context->hasOption(kRegion1Lock))
	{
		const Value *value = context->getOption(kRegion1Lock);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1Lock(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1Lock(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_lock is an unexpected type\n");
		}
	}

	// Check for region1_fac_region0_start
	if (context->hasOption(kRegion1facRegion0Start))
	{
		const Value *value = context->getOption(kRegion1facRegion0Start);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1facRegion0Start(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1facRegion0Start(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_fac_region0_start is an unexpected type\n");
		}
	}

	// Check for region1_fac_region0_length
	if (context->hasOption(kRegion1facRegion0Length))
	{
		const Value *value = context->getOption(kRegion1facRegion0Length);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1facRegion0Length(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1facRegion0Length(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_fac_region0_length is an unexpected type\n");
		}
	}

	// Check for region1_fac_region0_permission
	if (context->hasOption(kRegion1facRegion0Permission))
	{
		const Value *value = context->getOption(kRegion1facRegion0Permission);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1facRegion0Permission(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1facRegion0Permission(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_fac_region0_permission is an unexpected type\n");
		}
	}

	// Check for region1_fac_region1_start
	if (context->hasOption(kRegion1facRegion1Start))
	{
		const Value *value = context->getOption(kRegion1facRegion1Start);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1facRegion1Start(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1facRegion1Start(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_fac_region1_start is an unexpected type\n");
		}
	}

	// Check for region1_fac_region1_length
	if (context->hasOption(kRegion1facRegion1Length))
	{
		const Value *value = context->getOption(kRegion1facRegion1Length);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1facRegion1Length(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1facRegion1Length(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_fac_region1_length is an unexpected type\n");
		}
	}

	// Check for region1_fac_region1_permission
	if (context->hasOption(kRegion1facRegion1Permission))
	{
		const Value *value = context->getOption(kRegion1facRegion1Permission);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1facRegion1Permission(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1facRegion1Permission(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_fac_region1_permission is an unexpected type\n");
		}
	}

	// Check for region1_fac_region2_start
	if (context->hasOption(kRegion1facRegion2Start))
	{
		const Value *value = context->getOption(kRegion1facRegion2Start);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1facRegion2Start(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1facRegion2Start(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_fac_region2_start is an unexpected type\n");
		}
	}

	// Check for region1_fac_region2_length
	if (context->hasOption(kRegion1facRegion2Length))
	{
		const Value *value = context->getOption(kRegion1facRegion2Length);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1facRegion2Length(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1facRegion2Length(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_fac_region2_length is an unexpected type\n");
		}
	}

	// Check for region1_fac_region2_permission
	if (context->hasOption(kRegion1facRegion2Permission))
	{
		const Value *value = context->getOption(kRegion1facRegion2Permission);
		const StringValue *stringValue = dynamic_cast<const StringValue *>(value);
		const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
		if (stringValue)
		{
			imageSection->setRegion1facRegion2Permission(atoi(*stringValue));
		}
		else if (intValue)
		{
			imageSection->setRegion1facRegion2Permission(intValue->getValue());
		}
		else
		{
			Log::log(Logger::WARNING, "warning: region1_fac_region2_permission is an unexpected type\n");
		}
	}

}

void iMXImageGenerator::processOperationSection(OperationSequenceSection *section, iMXImage *image)
{
    iMXImage::Section *newSection = new iMXImage::Section(section->getIdentifier());

    // Deal with options that apply to sections.
    processSectionOptions(newSection, section);

    // add the boot section to the image
    image->addSection(newSection);
}
