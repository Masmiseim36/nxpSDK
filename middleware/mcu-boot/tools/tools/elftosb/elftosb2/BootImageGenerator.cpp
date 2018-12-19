/*
 * Copyright (c) 2008-2015 Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "BootImageGenerator.h"
#include "Logging.h"

//! Name of product version option.
#define kProductVersionOption "productVersion"

//! Name of component version option.
#define kComponentVersionOption "componentVersion"

//! Name of option that specifies the drive tag for this .sb file.
#define kDriveTagOption "driveTag"

//! Name of option that specifies the build number for this .sb file.
#define kBuildNumberOption "buildNumber"

using namespace elftosb;

void BootImageGenerator::processVersionOptions(BootImage *image)
{
    // bail if no option context was set
    if (!m_options)
    {
        return;
    }

    const StringValue *stringValue;
    version_t version;

    // productVersion
    if (m_options->hasOption(kProductVersionOption))
    {
        stringValue = dynamic_cast<const StringValue *>(m_options->getOption(kProductVersionOption));
        if (stringValue)
        {
            version.set(*stringValue);
            image->setProductVersion(version);
        }
        else
        {
            Log::log(Logger::WARNING, "warning: productVersion option is an unexpected type\n");
        }
    }

    // componentVersion
    if (m_options->hasOption(kComponentVersionOption))
    {
        stringValue = dynamic_cast<const StringValue *>(m_options->getOption(kComponentVersionOption));
        if (stringValue)
        {
            version.set(*stringValue);
            image->setComponentVersion(version);
        }
        else
        {
            Log::log(Logger::WARNING, "warning: componentVersion option is an unexpected type\n");
        }
    }
}

void BootImageGenerator::processDriveTagOption(BootImage *image)
{
    if (m_options->hasOption(kDriveTagOption))
    {
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(m_options->getOption(kDriveTagOption));
        if (intValue)
        {
            image->setDriveTag(static_cast<uint16_t>(intValue->getValue()));
        }
        else
        {
            Log::log(Logger::WARNING, "warning: driveTag option is an unexpected type\n");
        }
    }
}

void BootImageGenerator::processBuildNumberOption(BootImage *image)
{
    if (m_options->hasOption(kBuildNumberOption))
    {
        const IntegerValue *intValue = dynamic_cast<const IntegerValue *>(m_options->getOption(kBuildNumberOption));
        if (intValue)
        {
            image->setBuildNumber(static_cast<uint16_t>(intValue->getValue()));
        }
        else
        {
            Log::log(Logger::WARNING, "warning: buildNumber option is an unexpected type\n");
        }
    }
}
