/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(_EncoreBootImageGenerator_h_)
#define _EncoreBootImageGenerator_h_

#include "BootImageGenerator.h"
#include "EncoreBootImage.h"
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
class EncoreBootImageGenerator : public BootImageGenerator
{
public:
    //! \brief Default constructor.
    EncoreBootImageGenerator()
        : BootImageGenerator()
        , m_encryptKeyBlob(NULL)
    {
    }

    //! \brief Builds the resulting boot image from previously added output sections.
    virtual BootImage *generate();

    //! \brief Enable or disable HAB support.
    void setSupportHAB(bool supportHAB) { m_supportHAB = supportHAB; }
protected:
    bool m_supportHAB;         //!< True if HAB features are enabled.
    Keyblob *m_encryptKeyBlob; //!< Keyblob to use during load if encrypting for OTFAD.

    void processOptions(EncoreBootImage *image);
    void processSectionOptions(EncoreBootImage::Section *imageSection, OutputSection *modelSection);

    void processOperationSection(OperationSequenceSection *section, EncoreBootImage *image);
    void processDataSection(BinaryDataSection *section, EncoreBootImage *image);

    void processLoadOperation(LoadOperation *op, EncoreBootImage::BootSection *section);
    void processExecuteOperation(ExecuteOperation *op, EncoreBootImage::BootSection *section);
    void processBootModeOperation(BootModeOperation *op, EncoreBootImage::BootSection *section);
    void processFlashEraseOperation(FlashEraseOperation *op, EncoreBootImage::BootSection *section);
    void processResetOperation(ResetOperation *op, EncoreBootImage::BootSection *section);
    void processMemEnableOperation(MemEnableOperation *op, EncoreBootImage::BootSection *section);
    void processProgramOperation(ProgramOperation *op, EncoreBootImage::BootSection *section);
    void processKeywrapOperation(KeywrapOperation *op, EncoreBootImage::BootSection *section);
    void processEncryptOperation(EncryptOperation *op, EncoreBootImage::BootSection *section);

    void setFillPatternFromValue(EncoreBootImage::FillCommand &command, SizedIntegerValue &pattern);
};

}; // namespace elftosb

#endif // _EncoreBootImageGenerator_h_
