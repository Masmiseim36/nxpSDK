/*
 * Copyright (c) 2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_SB2ImageGenerator_h_)
#define _SB2ImageGenerator_h_

#include "BootImageGenerator.h"
#include "SB2Image.h"
#include "Keyblob.h"

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
class SB2ImageGenerator : public BootImageGenerator
{
public:
    //! \brief Default constructor.
    SB2ImageGenerator()
        : BootImageGenerator()
    {
    }

    //! \brief Builds the resulting boot image from previously added output sections.
    virtual BootImage *generate();

protected:
    void processOptions(SB2Image *image);
    void processSectionOptions(SB2Image::Section *imageSection, OutputSection *modelSection);

    void processOperationSection(OperationSequenceSection *section, SB2Image *image);
    void processDataSection(BinaryDataSection *section, SB2Image *image);

    void processLoadOperation(LoadOperation *op, SB2Image::BootSection *section);
    void processExecuteOperation(ExecuteOperation *op, SB2Image::BootSection *section);
    void processBootModeOperation(BootModeOperation *op, SB2Image::BootSection *section);
    void processFlashEraseOperation(FlashEraseOperation *op, SB2Image::BootSection *section);
    void processResetOperation(ResetOperation *op, SB2Image::BootSection *section);
    void processMemEnableOperation(MemEnableOperation *op, SB2Image::BootSection *section);
    void processProgramOperation(ProgramOperation *op, SB2Image::BootSection *section);

    void setFillPatternFromValue(SB2Image::FillCommand &command, SizedIntegerValue &pattern);
};

}; // namespace elftosb

#endif // _SB2ImageGenerator_h_
