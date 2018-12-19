/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_iMXImageGenerator_h_)
#define _iMXImageGenerator_h_

#include "BootImageGenerator.h"
#include "iMXImage.h"
#include "Keyblob.h"

#define SEC_CSF_HEADER                  	20
#define SEC_CSF_INSTALL_SRK					21
#define SEC_CSF_INSTALL_CSFK				22
#define SEC_CSF_INSTALL_NOCAK			    23
#define SEC_CSF_AUTHENTICATE_CSF			24
#define SEC_CSF_INSTALL_KEY				    25
#define SEC_CSF_AUTHENTICATE_DATA		    26
#define SEC_CSF_INSTALL_SECRET_KEY		    27
#define SEC_CSF_DECRYPT_DATA				28
#define SEC_NOP								29 
#define SEC_SET_MID							30 
#define SEC_SET_ENGINE						31 
#define SEC_INIT							32 
#define SEC_UNLOCK						    33 
#define SEC_XIP_REGION0						34
#define SEC_XIP_REGION1						35

#define SOURCE_FILE_TAG_NAME            ("elfFile")
namespace elftosb
{
/*!
 * \brief Generator for Encore boot images.
 *
 * Takes the abstract model of the output file and processes it into a
 * concrete boot image for the STMP37xx.
 *
 * In order to enable full i.mx28 support, you must call the setSupportHAB() method and
 * pass true.
 */
class iMXImageGenerator : public BootImageGenerator
{
public:
    //! \brief Default constructor.
    iMXImageGenerator()
        : BootImageGenerator()
    {
    }

    //! \brief Builds the resulting boot image from previously added output sections.
    virtual BootImage *generate();

protected:
    void processOptions(iMXImage *image);
    void processSectionOptions(iMXImage::Section *imageSection, OutputSection *modelSection);

    void processOperationSection(OperationSequenceSection *section, iMXImage *image);
};

}; // namespace elftosb

#endif // _iMXImageGenerator_h_
