/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "EncoreBootImageGenerator.h"
#include "Logging.h"
#include "Keyblob.h"
#include "OptionContext.h"

#define kFlagsOption "flags"
#define kSectionFlagsOption "sectionFlags"
#define kProductVersionOption "productVersion"
#define kComponentVersionOption "componentVersion"
#define kAlignmentOption "alignment"
#define kCleartextOption "cleartext"

using namespace elftosb;

BootImage *EncoreBootImageGenerator::generate()
{
    EncoreBootImage *image = new EncoreBootImage();

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

        BinaryDataSection *dataSection = dynamic_cast<BinaryDataSection *>(section);
        if (dataSection)
        {
            processDataSection(dataSection, image);
            continue;
        }

        Log::log(Logger::WARNING, "warning: unexpected output section type\n");
    }

    // handle global options that affect the image
    processOptions(image);

    return image;
}

void EncoreBootImageGenerator::processOptions(EncoreBootImage *image)
{
    // bail if no option context was set
    if (!m_options)
    {
        return;
    }

    if (m_options->hasOption(kFlagsOption))
    {
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(m_options->getOption(kFlagsOption));
        if (intValue)
        {
            image->setFlags(static_cast<uint16_t>(intValue->getValue()));
        }
        else
        {
            Log::log(Logger::WARNING, "warning: flags option is an unexpected type\n");
        }
    }

    // handle common options
    processVersionOptions(image);
    processDriveTagOption(image);
}

void EncoreBootImageGenerator::processSectionOptions(EncoreBootImage::Section *imageSection,
                                                     OutputSection *modelSection)
{
    // Get options context for this output section.
    const OptionContext *context = modelSection->getOptions();
    if (!context)
    {
        return;
    }

    // Check for and handle "sectionFlags" option.
    if (context->hasOption(kSectionFlagsOption))
    {
        const Value *value = context->getOption(kSectionFlagsOption);
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
        if (intValue)
        {
            // set explicit flags for this section
            imageSection->setFlags(intValue->getValue());
        }
        else
        {
            Log::log(Logger::WARNING, "warning: sectionFlags option is an unexpected type\n");
        }
    }

    // Check for and handle "alignment" option.
    if (context->hasOption(kAlignmentOption))
    {
        const Value *value = context->getOption(kAlignmentOption);
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
        if (intValue)
        {
            // verify alignment value
            if (intValue->getValue() < EncoreBootImage::BOOT_IMAGE_MINIMUM_SECTION_ALIGNMENT)
            {
                Log::log(Logger::WARNING, "warning: alignment option value must be 16 or greater\n");
            }

            imageSection->setAlignment(intValue->getValue());
        }
        else
        {
            Log::log(Logger::WARNING, "warning: alignment option is an unexpected type\n");
        }
    }

    // Check for and handle "cleartext" option.
    if (context->hasOption(kCleartextOption))
    {
        const Value *value = context->getOption(kCleartextOption);
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(value);
        if (intValue)
        {
            bool leaveUnencrypted = intValue->getValue() != 0;
            imageSection->setLeaveUnencrypted(leaveUnencrypted);
        }
        else
        {
            Log::log(Logger::WARNING, "warning: cleartext option is an unexpected type\n");
        }
    }
}

void EncoreBootImageGenerator::processOperationSection(OperationSequenceSection *section, EncoreBootImage *image)
{
    EncoreBootImage::BootSection *newSection = new EncoreBootImage::BootSection(section->getIdentifier());

    OperationSequence &sequence = section->getSequence();
    OperationSequence::iterator_t it = sequence.begin();
    for (; it != sequence.end(); ++it)
    {
        Operation *op = *it;

        ProgramOperation *progOp = dynamic_cast<ProgramOperation *>(op);
        if (progOp)
        {
            processProgramOperation(progOp, newSection);
            continue;
        }

        KeywrapOperation *wrapOp = dynamic_cast<KeywrapOperation *>(op);
        if (wrapOp)
        {
            processKeywrapOperation(wrapOp, newSection);
            continue;
        }

        EncryptOperation *encOp = dynamic_cast<EncryptOperation *>(op);
        if (encOp)
        {
            processEncryptOperation(encOp, newSection);
            continue;
        }

        LoadOperation *loadOp = dynamic_cast<LoadOperation *>(op);
        if (loadOp)
        {
            processLoadOperation(loadOp, newSection);
            continue;
        }

        ExecuteOperation *execOp = dynamic_cast<ExecuteOperation *>(op);
        if (execOp)
        {
            processExecuteOperation(execOp, newSection);
            continue;
        }

        BootModeOperation *modeOp = dynamic_cast<BootModeOperation *>(op);
        if (modeOp)
        {
            processBootModeOperation(modeOp, newSection);
            continue;
        }

        FlashEraseOperation *eraseOp = dynamic_cast<FlashEraseOperation *>(op);
        if (eraseOp)
        {
            processFlashEraseOperation(eraseOp, newSection);
            continue;
        }

        ResetOperation *resetOp = dynamic_cast<ResetOperation *>(op);
        if (resetOp)
        {
            processResetOperation(resetOp, newSection);
            continue;
        }

        MemEnableOperation *enableOp = dynamic_cast<MemEnableOperation *>(op);
        if (enableOp)
        {
            processMemEnableOperation(enableOp, newSection);
            continue;
        }

        Log::log(Logger::WARNING, "warning: unexpected operation type\n");
    }

    // Deal with options that apply to sections.
    processSectionOptions(newSection, section);

    // add the boot section to the image
    image->addSection(newSection);
}

void EncoreBootImageGenerator::processLoadOperation(LoadOperation *op, EncoreBootImage::BootSection *section)
{
    DataSource *source = op->getSource();
    DataTarget *target = op->getTarget();

    // other sources get handled the same way
    unsigned segmentCount = source->getSegmentCount();
    unsigned index = 0;
    for (; index < segmentCount; ++index)
    {
        DataSource::Segment *segment = source->getSegmentAt(index);
        DataTarget::AddressRange range = target->getRangeForSegment(*source, *segment);

        // Check for negative and zero length range.
        int64_t rangeDiff = static_cast<int64_t>(range.m_end) - static_cast<int64_t>(range.m_begin);
        if (rangeDiff <= 0)
        {
            continue;
        }
        unsigned rangeLength = static_cast<uint32_t>(rangeDiff);

        // handle a pattern segment as a special case to create a fill command
        DataSource::PatternSegment *patternSegment = dynamic_cast<DataSource::PatternSegment *>(segment);
        if (patternSegment)
        {
            SizedIntegerValue &pattern = patternSegment->getPattern();

            EncoreBootImage::FillCommand *command = new EncoreBootImage::FillCommand();
            command->setAddress(range.m_begin);
            command->setFillCount(rangeLength);
            setFillPatternFromValue(*command, pattern);

            section->addCommand(command);
            continue;
        }

        unsigned allocLength = rangeLength;
        if (m_encryptKeyBlob)
        {
            // Align allocation size to quadspi block
            const unsigned align = Keyblob::k_qspiAlignlength;
            unsigned remainder = rangeLength % align;
            allocLength = remainder ? rangeLength + (align - remainder) : rangeLength;
        }

        // get the data from the segment
        uint8_t *data = new uint8_t[allocLength];
        memset(data, 0, allocLength);
        segment->getData(0, rangeLength, data);

        // Encrypt for OTFAD if keyblob provided.
        if (m_encryptKeyBlob && m_encryptKeyBlob->encryptMatchingRange(range.m_begin, allocLength, data))
        {
            rangeLength = allocLength;
        }

        // create the boot command
        EncoreBootImage::LoadCommand *command = new EncoreBootImage::LoadCommand();
        command->setData(data, rangeLength); // Makes a copy of the data buffer.
        command->setLoadAddress(range.m_begin);
        command->setDCD(op->isDCDLoad());

        LoadExternalOperation *loadExtOp = dynamic_cast<LoadExternalOperation *>(op);
        if (loadExtOp)
        {
            command->setMemoryId(loadExtOp->getMemoryId());
        }

        section->addCommand(command);

        // Free the segment buffer.
        delete[] data;
    }
}

void EncoreBootImageGenerator::setFillPatternFromValue(EncoreBootImage::FillCommand &command,
                                                       SizedIntegerValue &pattern)
{
    uint32_t u32PatternValue = pattern.getValue() & pattern.getWordSizeMask();
    switch (pattern.getWordSize())
    {
        case kWordSize:
        {
            command.setPattern(u32PatternValue);
            break;
        }

        case kHalfWordSize:
        {
            uint16_t u16PatternValue = static_cast<uint16_t>(u32PatternValue);
            command.setPattern(u16PatternValue);
            break;
        }

        case kByteSize:
        {
            uint8_t u8PatternValue = static_cast<uint8_t>(u32PatternValue);
            command.setPattern(u8PatternValue);
        }
    }
}

void EncoreBootImageGenerator::processExecuteOperation(ExecuteOperation *op, EncoreBootImage::BootSection *section)
{
    DataTarget *target = op->getTarget();
    uint32_t arg = static_cast<uint32_t>(op->getArgument());

    EncoreBootImage::JumpCommand *command;
    switch (op->getExecuteType())
    {
        case ExecuteOperation::kJump:
            command = new EncoreBootImage::JumpCommand();
            break;

        case ExecuteOperation::kCall:
            command = new EncoreBootImage::CallCommand();
            break;
    }

    command->setAddress(target->getBeginAddress());
    command->setArgument(arg);
    command->setIsHAB(op->isHAB());
    if (op->isStackPonterSet())
    {
        command->setStackPointer(op->getStackPointer());
        command->setIsStackPointerSet(true);
    }

    section->addCommand(command);
}

void EncoreBootImageGenerator::processBootModeOperation(BootModeOperation *op, EncoreBootImage::BootSection *section)
{
    EncoreBootImage::ModeCommand *command = new EncoreBootImage::ModeCommand();
    command->setBootMode(op->getBootMode());

    section->addCommand(command);
}

void EncoreBootImageGenerator::processResetOperation(ResetOperation *op, EncoreBootImage::BootSection *section)
{
    EncoreBootImage::ResetCommand *command = new EncoreBootImage::ResetCommand();

    section->addCommand(command);
}

void EncoreBootImageGenerator::processFlashEraseOperation(FlashEraseOperation *op,
                                                          EncoreBootImage::BootSection *section)
{
    EncoreBootImage::EraseCommand *command = new EncoreBootImage::EraseCommand();

    // Set command options based on whether this is an erase all operation, erase all unsecure operation, erase all with
    // mem controller operation, or a regular erase.
    FlashEraseExternalOperation *eraseExtOp = dynamic_cast<FlashEraseExternalOperation *>(op);
    FlashEraseAllOperation *eraseAllOp = dynamic_cast<FlashEraseAllOperation *>(op);
    FlashEraseAllUnsecureOperation *eraseAllUnsecOp = dynamic_cast<FlashEraseAllUnsecureOperation *>(op);
    FlashEraseAllExternalOperation *eraseAllExtOp = dynamic_cast<FlashEraseAllExternalOperation *>(op);
    if (eraseAllOp)
    {
        command->setEraseAll(true);
    }
    else if (eraseAllUnsecOp)
    {
        command->setEraseAllUnsecure(true);
    }
    else if (eraseAllExtOp)
    {
        command->setEraseAll(true);
        command->setMemoryId(eraseAllExtOp->getMemoryId());
    }
    else
    {
        if (eraseExtOp)
        {
            command->setMemoryId(eraseExtOp->getMemoryId());
        }
        uint32_t start;
        uint32_t count;
        op->getRange(&start, &count);
        command->setAddressRange(start, count);
    }

    section->addCommand(command);
}

void EncoreBootImageGenerator::processMemEnableOperation(MemEnableOperation *op, EncoreBootImage::BootSection *section)
{
    EncoreBootImage::MemEnableCommand *command = new EncoreBootImage::MemEnableCommand();

    uint32_t start;
    uint32_t count;
    op->getRange(&start, &count);
    command->setAddressRange(start, count);

    command->setMemoryId(op->getMemoryId());

    section->addCommand(command);
}

void EncoreBootImageGenerator::processProgramOperation(ProgramOperation *op, EncoreBootImage::BootSection *section)
{
    DataSource *source = op->getSource();
    DataTarget *target = op->getTarget();

    unsigned segmentCount = source->getSegmentCount();
    if (!segmentCount)
    {
        return;
    }
    if (segmentCount != 1)
    {
        Log::log(Logger::WARNING, "warning: program operation using first segment only\n");
    }

    DataSource::Segment *segment = source->getSegmentAt(0);
    DataTarget::AddressRange range = target->getRangeForSegment(*source, *segment);
    unsigned rangeLength = range.m_end - range.m_begin;

    if ((rangeLength != 4) && (rangeLength != 8))
    {
        Log::log(Logger::WARNING, "warning: program operation requires 4 or 8 byte segment\n");
        return;
    }

    // Handle a pattern segment as a special case.
    DataSource::PatternSegment *patternSegment = dynamic_cast<DataSource::PatternSegment *>(segment);
    if (patternSegment)
    {
        // Only 4 byte pattern is supported.
        if (rangeLength != 4)
        {
            Log::log(Logger::WARNING, "warning: program operation requires a 4 byte pattern segment\n");
            return;
        }

        SizedIntegerValue &pattern = patternSegment->getPattern();

        // Create a fill command to convert the pattern to data.
        EncoreBootImage::FillCommand *command = new EncoreBootImage::FillCommand();
        setFillPatternFromValue(*command, pattern);

        EncoreBootImage::ProgramCommand *progCmd = new EncoreBootImage::ProgramCommand();
        progCmd->setIsEightByte(false);
        progCmd->setMemoryId(op->getMemoryId());
        progCmd->setIndex(range.m_begin);
        progCmd->setDataWord1(command->getPattern());

        section->addCommand(progCmd);
        delete command;
    }
    else
    {
        EncoreBootImage::ProgramCommand *command = new EncoreBootImage::ProgramCommand();
        command->setIsEightByte((rangeLength == 8));
        command->setMemoryId(op->getMemoryId());
        command->setIndex(range.m_begin);

        // get first word from the segment
        uint8_t *data = new uint8_t[8];
        segment->getData(0, 4, data);
        command->setDataWord1(*(uint32_t *)data);

        if (command->isEightByte())
        {
            // get second word from the segment
            segment->getData(0, 8, data);
            command->setDataWord2(*(uint32_t *)(data + 4));
        }

        section->addCommand(command);

        // Free the segment buffer.
        delete[] data;
    }
}

void EncoreBootImageGenerator::processKeywrapOperation(KeywrapOperation *op, EncoreBootImage::BootSection *section)
{
    DataSource *source = op->getSource();
    DataTarget *target = op->getTarget();

    unsigned segmentCount = source->getSegmentCount();
    if (!segmentCount)
    {
        return;
    }
    if (segmentCount != 1)
    {
        Log::log(Logger::WARNING, "warning: keywrap operation using first segment only\n");
    }

    DataSource::Segment *segment = source->getSegmentAt(0);
    DataTarget::AddressRange range = target->getRangeForSegment(*source, *segment);
    unsigned rangeLength = range.m_end - range.m_begin;

    const uint32_t kekSize = Keyblob::getKekSizeBytes();
    if (rangeLength != kekSize)
    {
        Log::log(Logger::WARNING, "warning: keywrap operation requires %d byte key segment\n", kekSize);
        return;
    }

    // Handle a pattern segment as a special case.
    DataSource::PatternSegment *patternSegment = dynamic_cast<DataSource::PatternSegment *>(segment);
    if (patternSegment)
    {
        Log::log(Logger::WARNING, "warning: keywrap operation does not support pattern segment\n");
        return;
    }
    else
    {
        // Get key data (kek) from source.
        uint8_t *kek = new uint8_t[kekSize];
        segment->getData(0, kekSize, kek);

        Log::log(Logger::INFO2, "creating wrapped keyblob\n");

        Keyblob *keyblob = op->getKeyblob();
        assert(keyblob);
        uint32_t byteCount = 0;
        uint8_t *data = keyblob->createWrappedKeyblobData(kek, &byteCount);
        delete[] kek;

        // Create new data source with wrapped key blob.
        source = new UnmappedDataSource(data, byteCount);
        segment = source->getSegmentAt(0);

        // Create an address range, possibly modified by target.
        range = target->getRangeForSegment(*source, *segment);
        unsigned length = range.m_end - range.m_begin;
        delete source;

        // Create load command with wrapped keyblob as source.
        EncoreBootImage::LoadCommand *command = new EncoreBootImage::LoadCommand();
        command->setData(data, length); // Makes a copy of the data buffer.
        command->setLoadAddress(range.m_begin);
        command->setDCD(false);

        delete[] data;

        section->addCommand(command);
    }
}

void EncoreBootImageGenerator::processEncryptOperation(EncryptOperation *op, EncoreBootImage::BootSection *section)
{
    m_encryptKeyBlob = op->getKeyblob();
    assert(m_encryptKeyBlob);

    // Load operation will notice non-null keyblob pointer and encrypt accordingly.
    processLoadOperation(op, section);

    m_encryptKeyBlob = NULL;
}

void EncoreBootImageGenerator::processDataSection(BinaryDataSection *section, EncoreBootImage *image)
{
    EncoreBootImage::DataSection *dataSection = new EncoreBootImage::DataSection(section->getIdentifier());
    dataSection->setData(section->getData(), section->getLength());

    // Handle alignment option.
    processSectionOptions(dataSection, section);

    image->addSection(dataSection);
}
