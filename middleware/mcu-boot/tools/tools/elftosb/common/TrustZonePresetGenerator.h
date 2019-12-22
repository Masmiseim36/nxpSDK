/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TRUSTZONEGENERATOR_
#define _TRUSTZONEGENERATOR_

#include <string>
#include <memory>
#include <map>
#include "json.h"
using namespace std;

/*!
* \brief Generator for TrustZone-M preset files.
*
* Takes the JSON specififcation of registers values and creating simple bin file.
* Bin file can be later attached into master boot image with TZ-M preset.
* Currently support of lpc55xx and rt6xx 
*/
class TrustZonePresetGenerator {
public:
	TrustZonePresetGenerator();
	//! \brief Used for creation of TZ-M preset data bin file based on provided JSON cofiguration.
	void execute(const string& confFilePath);
	//! \brief Used for on the fly creation of TZ-M preset data during master boot image genaration. Created based on provided JSON cofiguration.
	unique_ptr<vector<uint32_t>> getTzpPreset(const string& confFilePath, const string& family);

private:
	string outputFile;
	string family;
	string revision;
	//! \brief Connecting all particular processing steps in specific order.
	bool process(const string& confFilePath);
	//! \brief Saving TZ-M preset data to bin file.
	bool saveFile(unique_ptr<vector<uint32_t>> buffer);
	/*! 
	* \brief Creating TZ-M preset data vector. If register is missing in JSON specification, default value is used from device specification.
	* Registers are processed in right order based on device specification.
	*/
	unique_ptr<vector<uint32_t>> generateTzp(jute::jValue tzp, const string& confFilePath);
	//! \brief Parsing and validating information from JSON configuration file
	jute::jValue parseFile(const string &confFilePath);
	//! \brief Returns device configuration.
	std::vector<pair<string, uint32_t>>* getVector(const string family);
	//! \brief Returns device configuration for rt6xx A0.
	std::vector<pair<string, uint32_t>>* getRt6xxVectorA0();
	//! \brief Returns device configuration for rt6xx B0.
	std::vector<pair<string, uint32_t>>* getRt6xxVectorB0();
	//! \brief Returns device configuration for lpc55xx A0.
	std::vector<pair<string, uint32_t>>* getLpc55xxVectorA0();
	//! \brief Returns device configuration for lpc55xx A1.
	std::vector<pair<string, uint32_t>>* getLpc55xxVectorA1();
	//! \brief Returns device configuration for lpc55s3x.
	std::vector<pair<string, uint32_t>>* getLpc55s3xVector();
	//! \brief Returns device configuration for rt5xx.
	std::vector<pair<string, uint32_t>>* getRt5xxVector();
};

#endif /*_TRUSTZONEGENERATOR_*/