/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include <sstream>

#include "AESKey.h"
#include "crc.h"
#include "json.h"
#include "TrustZonePresetGenerator.h"
#include "Logging.h"

#include "AuthImageGenerator.h"

#define FAIL_RETURN false;
#define SUCCESS_RETURN true;
#define MAX_PATH 150
#define AUTH_IMAGE_ALIGNMENT 4
#define ES_IMG_TAIL_BYTELEN (sizeof(es_header_t) + sizeof(ctrInitVector))
#define MAX_ROOT_CERT_COUNT 4
#define HMACHEADEROFSET 64

AuthImageGenerator::AuthImageGenerator() {}

void AuthImageGenerator::execute(const string &confFilePath, const chip_family_t family) {
	configuration conf{};
	if (parseConfiguration(confFilePath, conf, family)) {
		if (!createImage(conf, family))
			throw runtime_error("Creation of authenticated image file failed. Please see the previous output to identify possible problem.");
	}
	else
		throw runtime_error(("Parsing of configuration file " + confFilePath + " failed. Please see the previous output to identify possible problem.\nTIP: Use json validator to help with correction json formating problems if there are any.\n").c_str());
}

bool AuthImageGenerator::parseConfiguration(const string &confFilePath, configuration &conf, const chip_family_t family) {
	if (family == kLPC_skbootFamily)
		return parseConfigurationLpcSboot(confFilePath, conf);
	else if (family == kKinetisK3Family)
		return parseConfigurationK3Sboot(confFilePath, conf);
	else if (family == kLPC54X0XXFamily)
		return parseConfigurationLPC54X0XXSboot(confFilePath, conf);
	else
		throw runtime_error("Not supported device family for parsing master boot image json configuration.");
}

bool AuthImageGenerator::createImage(const configuration &conf, const chip_family_t family) {
	if (family == kLPC_skbootFamily)
		return createImageLpcSboot(conf);
	else if (family == kKinetisK3Family)
		return createImageK3Sboot(conf);
	else if (family == kLPC54X0XXFamily)
		return createImageLPC54X0XXSboot(conf);
	else
		throw runtime_error("Not supported device family for creating master boot image file.");
}

bool AuthImageGenerator::parseConfigurationLpcSboot(const string &confFilePath, configuration &conf)
{
	string tmp;
	auto error = false;
	stringstream ss;
	ss << "Parsing configuration file: " << confFilePath << ".\n";
	Log::log(Logger::INFO, ss.str());
	ss.str("");
	jute::jValue jsonConf;

	try {
		jsonConf = jute::parser::parse_file(confFilePath);
	}
	catch (runtime_error &e) {
		throw runtime_error(("Cannot parse json configuration file: " + confFilePath + " - " + e.what()).c_str());
	}
	catch (...) {
		throw runtime_error("Cannot parse json configuration file: " + confFilePath);
	}

	if (jsonConf["family"].get_type() == jute::JSTRING) {
		conf.family = jsonConf["family"].as_string();
		transform(conf.family.begin(), conf.family.end(), conf.family.begin(), ::tolower);
		if (conf.family != LPC55XX && conf.family != RT6XX && conf.family != NIOBE4MINI && conf.family != RT5XX) {
			ss << "\tUnexpected \"family\" value (" << conf.family << ") from configuration file: " << confFilePath << ".\n";
			ss << "\tSupported only " << LPC55XX << ", " << NIOBE4MINI << ", " << RT5XX <<" and " << RT6XX << "." << endl;
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
	}
	else {
		ss << "\tCannot read \"family\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (jsonConf["inputImageFile"].get_type() == jute::JSTRING)
		conf.imageFile = jsonConf["inputImageFile"].as_string();
	else {
		ss << "\tCannot read \"inputImageFile\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (jsonConf["outputImageExecutionTarget"].get_type() == jute::JSTRING) {
		string backup = tmp = jsonConf["outputImageExecutionTarget"].as_string();
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		tmp.erase(remove_if(tmp.begin(), tmp.end(), [](char ch) { return isspace(static_cast<unsigned char>(ch)); }), tmp.end());
		if ((conf.family == LPC55XX || conf.family == NIOBE4MINI) && tmp != "internalflash(xip)") {
			ss << "\tUnsupported value (" << backup << ") of \"outputImageAuthenticationType\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected values [\"Internal flash (XIP)\"] for " << conf.family << " family.\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}else if ((conf.family == RT6XX || conf.family == RT5XX) && tmp != "externalflash(xip)" && tmp != "ram") {
			ss << "\tUnsupported value (" << backup << ") of \"outputImageAuthenticationType\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected values [\"External flash (XIP)\", \"RAM\"]  for " << conf.family << " family.\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
		if (tmp == "internalflash(xip)" || tmp == "externalflash(xip)")
			tmp = "xip";
		conf.imageType = tmp;
	}
	else {
		ss << "\tCannot read \"outputImageExecutionTarget\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (jsonConf["outputImageAuthenticationType"].get_type() == jute::JSTRING) {
		string backup = tmp = jsonConf["outputImageAuthenticationType"].as_string();
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		tmp.erase(remove_if(tmp.begin(), tmp.end(), [](char ch) { return isspace(static_cast<unsigned char>(ch)); }), tmp.end());
		if ((conf.family == LPC55XX || conf.family == NIOBE4MINI) && !(tmp == "crc" || tmp == "signed")) {
			ss << "\tUnsupported value (" << tmp << ") of \"outputImageAuthenticationType\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected values [\"CRC\", \"Signed\"] " << conf.family << " family with \"outputImageExecutionTarget\" set to \"Internal flash(XIP)\".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}else if ((conf.family == RT6XX || conf.family == RT5XX) && conf.imageType == "xip" && !(tmp == "crc" || tmp == "signed")) {
			ss << "\tUnsupported value (" << tmp << ") of \"outputImageAuthenticationType\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected values [\"CRC\", \"Signed\"] for " << conf.family << " family with \"outputImageExecutionTarget\" set to \"External flash(XIP)\".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
		else if ((conf.family == RT6XX || conf.family == RT5XX) && conf.imageType == "ram" && !(tmp == "crc" || tmp == "signed" || tmp == "encrypted+signed")) {
			ss << "\tUnsupported value (" << tmp << ") of \"outputImageAuthenticationType\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected values [\"CRC\", \"Signed\", \"Encryped + Signed\"] for " << conf.family << " family with \"outputImageExecutionTarget\" set to \"RAM\".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
		conf.imageType += tmp;
	}
	else {
		ss << "\tCannot read \"outputImageAuthenticationType\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (conf.imageType == "ramsigned" || conf.imageType == "ramencrypted+signed") {
		if (jsonConf["deviceKeySource"].get_type() == jute::JSTRING) {
			string backup = tmp = jsonConf["deviceKeySource"].as_string();
			transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
			tmp.erase(remove_if(tmp.begin(), tmp.end(), [](char ch) { return isspace(static_cast<unsigned char>(ch)); }), tmp.end());
			if (tmp == "otp")
				conf.deviceKeySrc = deviceKeySource::OTP;
			else if (tmp == "keystore")
				conf.deviceKeySrc = deviceKeySource::keyStore;
			else {
				ss << "\tUnsupported value (" << backup << ") of \"deviceKeySource\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
				ss << "\tExpected values [\"Key Store\", \"OTP\"]\n";
				Log::log(Logger::ERROR, ss.str());
				ss.str("");
				error = true;
			}
		}
		else {
			ss << "\tCannot read \"deviceKeySource\" from configuration file: " << confFilePath << ".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
		
		if (conf.deviceKeySrc != deviceKeySource::OTP) {
			if (jsonConf["useKeyStore"].get_type() == jute::JBOOLEAN)
				conf.useKeyStore = jsonConf["useKeyStore"].as_bool();
			else {
				ss << "\tCannot read \"useKeyStore\" from configuration file: " << confFilePath << ".\n\tKey store disable by default.\n";
				Log::log(Logger::INFO, ss.str());
				ss.str("");
			}

			if (conf.useKeyStore) {
				if (jsonConf["keyStoreFile"].get_type() == jute::JSTRING)
					conf.keyStoreFile = jsonConf["keyStoreFile"].as_string();
				else {
					ss << "\tCannot read \"keyStoreFile\" from configuration file: " << confFilePath << ".\n\tKey store will be only reserved.\n";
					Log::log(Logger::INFO, ss.str());
					ss.str("");
				}
			}
		}
	}

	if (conf.imageType == "ramsigned" || conf.imageType == "ramencrypted+signed" || conf.imageType == "xipsigned" ) {		
		if (conf.imageType != "xipsigned") {
			if (jsonConf["outputImageEncryptionKeyFile"].get_type() == jute::JSTRING)
				conf.imageEncryptionKeyFile = jsonConf["outputImageEncryptionKeyFile"].as_string();
			else {
				ss << "\tCannot read \"outputImageEncryptionKeyFile\" from configuration file: " << confFilePath << ".\n";
				Log::log(Logger::ERROR, ss.str());
				ss.str("");
				error = true;
			}
		}

		for (int i = 0; i < MAX_ROOT_CERT_COUNT; i++) {
			tmp = "rootCertificate" + to_string(i) + "File";
			if (jsonConf[tmp].get_type() == jute::JSTRING)
				conf.rootCertFiles.push_back(string(jsonConf[tmp].as_string()));
			else {
				ss << "\tCannot read \"" << tmp << "\" from configuration file: " << confFilePath << ".\n\tSkiping " << tmp << ".\n";
				Log::log(Logger::INFO, ss.str());
				ss.str("");
				conf.rootCertFiles.push_back(string(""));
			}
		}

		if (jsonConf["mainCertChainId"].get_type() == jute::JNUMBER) {
			conf.mainCertId = jsonConf["mainCertChainId"].as_int();
			if (conf.mainCertId > MAX_ROOT_CERT_COUNT - 1) {
				ss << "\tWrong value of \"mainCertChainId\" loaded from configuration file: " << confFilePath << ".\n";
				ss << "\tLoaded: " << conf.mainCertId << " value should be from 0  to " << MAX_ROOT_CERT_COUNT - 1 << ", using 0 as default.\n";
				Log::log(Logger::INFO, ss.str());
				ss.str("");
				conf.mainCertId = 0;
			}
		}
		else {
			ss << "\tCannot read \"mainCertChainId\" from configuration file: " << confFilePath << ".\n\tUsing 0 as default.\n";
			Log::log(Logger::INFO, ss.str());
			ss.str("");
		}

		if (conf.rootCertFiles[conf.mainCertId] == "") {
			ss << "\tSelected main root certificate \"mainCertChainId\" (" << (int)conf.mainCertId << ") from configuration file: " << confFilePath <<
				" don't have set corresponding value for \"rootCertificate" << (int)conf.mainCertId << "\".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}

		short i = 0;
		tmp = "chainCertificate" + to_string(conf.mainCertId) + "File";
		while (jsonConf[tmp + to_string(i)].get_type() == jute::JSTRING) {
			conf.chainCertFiles.push_back(string(jsonConf[tmp + to_string(i)].as_string()));
			i++;
		}

		ss << "\tMain certificate chain " << (int)conf.mainCertId << " with " << i + 1 << " certificate(s) was fetched from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::DEBUG, ss.str());
		ss.str("");

		if (jsonConf["mainCertPrivateKeyFile"].get_type() == jute::JSTRING)
			conf.mainCertPrivateKeyFile = jsonConf["mainCertPrivateKeyFile"].as_string();
		else {
			ss << "\tCannot read \"mainCertPrivateKeyFile\" from configuration file: " << confFilePath << ".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
	}
	if (conf.family == RT5XX || conf.family == RT6XX || conf.family == NIOBE4MINI) {
		if (jsonConf["enableHwUserModeKeys"].get_type() == jute::JBOOLEAN)
			conf.enableHwUserModeKeys = jsonConf["enableHwUserModeKeys"].as_bool();
		else {
			ss << "\tCannot read \"enableHwUserModeKeys\" from configuration file: " << confFilePath << ". Disabled by default.\n";
			Log::log(Logger::INFO, ss.str());
			ss.str("");
			conf.enableHwUserModeKeys = false;
		}
	}

	if (jsonConf["imageLinkAddress"].get_type() != jute::JUNKNOWN) {
		string tmp(jsonConf["imageLinkAddress"].as_string());
		try {
			conf.imageLinkAddress = stoul(tmp, nullptr, 16);

			ss << "\tUsed \"imageLinkAddress\" value: " << conf.imageLinkAddress << "." << endl;
			Log::log(Logger::INFO2, ss.str());
			ss.str("");
		}
		catch (invalid_argument) {
			ss << "\tCannot parse \"imageLinkAddress\" value: " << jsonConf["imageLinkAddress"].as_string() << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected hexadecimal string in format \"0xHEXVALUE\" or only \"HEXVALUE\".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
		catch (out_of_range) {
			ss << "\tValue of \"imageLinkAddress\" is too big: " << jsonConf["imageLinkAddress"].as_string() << " from configuration file: " << confFilePath << ".\n";
			ss << "\tUse max value as 0xffffffff.\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
	}
	else {
		ss << "\tCannot read \"imageLinkAddress\" from configuration file: " << confFilePath << ". Using 0x0 as default.\n";
		Log::log(Logger::WARNING, ss.str());
		ss.str("");
	}

	if (jsonConf["imageBuildNumber"].get_type() != jute::JUNKNOWN) {
		string tmp(jsonConf["imageBuildNumber"].as_string());
		try {
			conf.buildNumber = stoul(tmp, nullptr, 16);

			ss << "\tUsed \"imageBuildNumber\" value: " << conf.buildNumber << "." << endl;
			Log::log(Logger::INFO2, ss.str());
			ss.str("");
		}
		catch (invalid_argument) {
			ss << "\tCannot parse \"imageBuildNumber\" value: " << jsonConf["imageBuildNumber"].as_string() << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected hexadecimal string in format \"0xHEXVALUE\" or only \"HEXVALUE\".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
		catch (out_of_range) {
			ss << "\tValue of \"imageBuildNumber\" is too big: " << jsonConf["imageBuildNumber"].as_string() << " from configuration file: " << confFilePath << ".\n";
			ss << "\tUse max value as 0xffffffff.\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
	}
	else {
		conf.buildNumber = FOUR_CHAR_CODE(1, 0, 0, 0);
		ss << "\tCannot read \"imageBuildNumber\" from configuration file: " << confFilePath << ". Using "<< conf.buildNumber << " as default.\n";
		Log::log(Logger::WARNING, ss.str());
		ss.str("");
	}

	if (jsonConf["enableTrustZone"].get_type() == jute::JBOOLEAN)
		conf.trustZone = jsonConf["enableTrustZone"].as_bool();
	else {
		ss << "\tCannot read \"enableTrustZone\" from configuration file: " << confFilePath << ".\n\tTrust zone disabled by default.\n";
		Log::log(Logger::INFO, ss.str());
		ss.str("");
	}

	if (conf.trustZone) {
		if (jsonConf["trustZonePresetFile"].get_type() == jute::JSTRING) {
			conf.trustZonePresetFile = jsonConf["trustZonePresetFile"].as_string();
			if (conf.trustZonePresetFile.length() > 0) {
				auto pos = conf.trustZonePresetFile.find_last_of('.');
				if (pos != string::npos) {
					auto type = conf.trustZonePresetFile.substr(pos);
					transform(type.begin(), type.end(), type.begin(), ::tolower);
					if (type == ".bin")
						conf.generateTrustZone = false;
					else if (type == ".json")
						conf.generateTrustZone = true;
					else
						pos = -1;					
				}
				if(pos == string::npos) {
					ss << "\tUnexpected value of \"trustZonePresetFile\" (" << conf.trustZonePresetFile << ") from configuration file: " << confFilePath << ".\n";
					ss << "\tPlease specify valid path to TZ-Preset .bin file or TZ-Preset .json configuration file or keep it empty to disable TZ-preset configuration in image.\n" << "\tTZ-Preset disabled in image.\n";
					Log::log(Logger::ERROR, ss.str());
					ss.str("");
					error = true;
				}
			}
		}
		else {
			ss << "\tCannot read \"trustZonePresetFile\" from configuration file: " << confFilePath << ".\n\tTZ-Preset disabled in image.\n";
			Log::log(Logger::INFO, ss.str());
			ss.str("");
		}
	}

	if (jsonConf["masterBootOutputFile"].get_type() == jute::JSTRING)
		conf.masterBootOutputFile = jsonConf["masterBootOutputFile"].as_string();
	else {
		ss << "\tCannot read \"masterBootOutputFile\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}
	if (error)
		Log::log(Logger::WARNING, "\tParsed with error(s).\n" );
	else 
		Log::log(Logger::INFO, "\tSuccess.\n");

	return !error;
}

int AuthImageGenerator::createImageLpcSboot(const configuration &conf)
{
	FILE *imageFile = NULL, *configFile = NULL, *usedRootCrtFile = NULL, *outputFile = NULL, *trustZonePresetFile = NULL;
	uint32_t signatureSizeInBytes = 0, rootCertCount = 0, usedRootCrtAlignedSize = 0, certCount = 0, rkhOffset = 0, status = SUCCESS_RETURN;
	uint8_t *buffer = NULL, *signature = NULL, ctrInitVector[16] = { 0 }, *userKey = NULL, *keyStore = NULL;
	const char* configdata = NULL;
	vector<string> crtList;
	string usedRootCertPath;
	int ret = -1, step = 1;
	size_t totalImageSize = 0, crtTableAlignedSize = 0, usedRootCrtSize = 0, crtTableSize = 0, userKeySize = 0, keyStoreSize = 0, imgTailSize = 0;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_rsa_context *rsa_priv_key_ctx = NULL;
	rkh_table_t rkh;
	mbedtls_x509_crt *trustCrt = NULL;
	_headerMac_t s_headerMac{};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Check the image file.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO, "Starting processing image....\n");
	Log::log(Logger::INFO2, "%d. Check of the image file.\n", step);
	configdata = conf.imageFile.c_str();
	imageFile = fopen(configdata, "rb");
	if (imageFile == NULL)
	{
		Log::log(Logger::ERROR, "\tCannot open image file %s.\n", configdata);
		cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}
	// Get image length in bytes.
	fseek(imageFile, 0, SEEK_END);
	size_t imageSize = ftell(imageFile);
	fseek(imageFile, 0, SEEK_SET);
	// Get the word aligned length.
	size_t imageAlignedSize = 0;
	if (imageSize % AUTH_IMAGE_ALIGNMENT)
	{
		imageAlignedSize = imageSize + (AUTH_IMAGE_ALIGNMENT - (imageSize % AUTH_IMAGE_ALIGNMENT));
	}
	else
	{
		imageAlignedSize = imageSize;
	}
	Log::log(Logger::INFO2, "\tSuccess. (File %s: Size = %lu bytes, AlignedSize = %lu bytes)\n", configdata, imageSize, imageAlignedSize);
	step++; //2
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Read image type
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d. Fetching of image configuration: execution target and security.\n", step);
	uint32_t imageType = kSKBOOT_ImageTypePlainSigned;
	configdata = conf.imageType.c_str();
	if (strncmp("ramsigned", configdata, MAX_PATH) == 0)
	{
		imageType = kSKBOOT_ImageTypePlainSigned;
		Log::log(Logger::INFO2, "\tRAM - plain signed: image will be signed based on provided configuration.\n");
	}
	else if (strncmp("ramcrc", configdata, MAX_PATH) == 0)//CRC
	{
		imageType = kSKBOOT_ImageTypePlainCrc;
		Log::log(Logger::INFO2, "\tRAM - plain with CRC: security configuration will be skipped.\n");
	}
	else if (strncmp("ramencrypted+signed", configdata, MAX_PATH) == 0)
	{
		imageType = kSKBOOT_ImageTypeEncryptedSigned;
		Log::log(Logger::INFO2, "\tRAM - encrypted signed: image will be encrypted and signed based on provided configuration.\n");
	}
	else if (strncmp("xipsigned", configdata, MAX_PATH) == 0)
	{
		imageType = kSKBOOT_ImageTypeXipPlainSigned;
		
		Log::log(Logger::INFO2, "\t%s flash (XIP) - plain signed: image will be signed based on provided configuration.\n", conf.family == LPC55XX ? "External": "Internal");
	}
	else if (strncmp("xipcrc", configdata, MAX_PATH) == 0) //CRC
	{
		imageType = kSKBOOT_ImageTypeXipPlainCrc;
		Log::log(Logger::INFO2, "\t%s flash (XIP) - plain with CRC: security configuration will be skipped.\n", conf.family == LPC55XX ? "External" : "Internal");
	}
	else
	{
		Log::log(Logger::ERROR, "Unexpected image type value: %s.\n", configdata);
		cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}
	Log::log(Logger::INFO2, "\tSuccess.\n");

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Check Image Link Address
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.1 Checking image link address configuration.\n", step);
	Log::log(Logger::INFO2, "\tImage link address will be set to: 0x%08x\n", conf.imageLinkAddress);
	uint32_t imageLinkAddress = conf.imageLinkAddress;
	Log::log(Logger::INFO2, "\tSuccess.\n");

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Checking trust zone configuration
	//////////////////////////////////////////////////////////////////////////////////////////////////// 
	Log::log(Logger::INFO2, "%d.2 Checking image trust zone configuration.\n", step);
	uint32_t imageTypeTZM = 0;
	size_t trustZonePresetFileSize = 0;
	unique_ptr<vector<uint32_t>> trustZonedata;
	if (conf.trustZone == false) //TZ-M Disabled
	{
		imageTypeTZM |= SKBOOT_IMG_HDR_TZM_IMAGE_TYPE_MASK;
		Log::log(Logger::INFO2, "\tTrust zone disabled image -> configuration of TZM-M_Preset and TZM-M_PresetFile is ignored.\n");
	}
	else
	{
		if (conf.trustZonePresetFile.length() > 0)
		{
			Log::log(Logger::INFO2, "\tTrust zone enabled image: configuration of TZM-M_Preset enabled -> TZM-M_PresetFile will be loaded and included in image.\n");
			imageTypeTZM |= SKBOOT_IMG_HDR_TZM_PRESET_MASK;
			if (conf.generateTrustZone) {
				Log::log(Logger::INFO2, "\tGenerating TZ-Preset configuration based on %s configuration file:.\n", conf.trustZonePresetFile.c_str());
				try {
					auto tpg = TrustZonePresetGenerator();
					trustZonedata = tpg.getTzpPreset(conf.trustZonePresetFile, conf.family);
					trustZonePresetFileSize = (*trustZonedata).size() * sizeof(uint32_t);
				}
				catch (runtime_error e) {
					Log::log(Logger::ERROR, "\tCannot generate TZM-M_Preset configuration: %s\n", e.what());
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}
				catch (...) {
					Log::log(Logger::ERROR, "\tCannot generate TZM-M_Preset configuration: Unexpected error.\n");
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}
			}
			else {
				Log::log(Logger::INFO2, "%d.2.1 Checking trust zone preset file %s.\n",step,  conf.trustZonePresetFile.c_str());
				const auto tZPFileName = conf.trustZonePresetFile.c_str();
				trustZonePresetFile = fopen(tZPFileName, "rb");
				if (NULL == trustZonePresetFile)
				{
					Log::log(Logger::ERROR, "\tCannot open TZM-M_PresetFile file %s.\n", tZPFileName);
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}
				fseek(trustZonePresetFile, 0, SEEK_END);
				trustZonePresetFileSize = ftell(trustZonePresetFile);
				fseek(trustZonePresetFile, 0, SEEK_SET);
				if (trustZonePresetFileSize % AUTH_IMAGE_ALIGNMENT)
				{
					Log::log(Logger::ERROR, "Unexpected size of TZM-M_PresetFile: %lu bytes, byte count should be divisible by %d without the remainder.\n", trustZonePresetFileSize, AUTH_IMAGE_ALIGNMENT);
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}
				Log::log(Logger::INFO2, "\tSuccess: Trust zone preset file %s verified. (size: %lu bytes)\n", tZPFileName, trustZonePresetFileSize);
			}
		}
		else
		{
			Log::log(Logger::INFO2, "\tTrust zone enabled image: configuration of TZM-M_Preset disabled -> TZM-M_PresetFile is ignored and not used.\n");
		}
	}

	uint32_t enableHwKeys = 0x0u;
	if (conf.family == RT5XX || conf.family == RT6XX || conf.family == NIOBE4MINI) {
		Log::log(Logger::INFO2, "%d.3 Checking image HW user mode keys enablement for all security levels.\n", step);
		enableHwKeys = conf.enableHwUserModeKeys ? SKBOOT_IMG_HDR_HW_USER_KEY_EN : 0x0u;
		if (conf.enableHwUserModeKeys)
			Log::log(Logger::INFO2, "\tHW user mode key enabled.\n");
		else
			Log::log(Logger::INFO2, "\tHW user mode key disabled.\n");
		Log::log(Logger::INFO2, "\tSuccess.\n");
	}
	
	if (imageType == kSKBOOT_ImageTypeXipPlainCrc || imageType == kSKBOOT_ImageTypePlainCrc)
	{
		Log::log(Logger::INFO2, "Start to generate CRC image!\n");
		totalImageSize = imageAlignedSize;
	}
	else
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Checking key store configuration
		//////////////////////////////////////////////////////////////////////////////////////////////////// 
		if ((imageType == kSKBOOT_ImageTypePlainSigned || imageType == kSKBOOT_ImageTypeEncryptedSigned) && deviceKeySource::keyStore == conf.deviceKeySrc)
		{
			Log::log(Logger::INFO2, "%d.3 Checking key store configuration.\n", step);
			if (conf.useKeyStore)
			{
				imageType |= SKBOOT_IMG_HDR_KEY_STORE_INCLUDED_MASK;
				Log::log(Logger::INFO2, "\tKey store enabled image.\n");
			}
			else
			{
				Log::log(Logger::INFO2, "\tKey store disabled image.\n");
			}
			Log::log(Logger::INFO2, "\tSuccess.\n");
		}
		step++;
		Log::log(Logger::INFO2, "Start to generate signed image!\n");
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Load the Root certificate files.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. Load the root certificates.\n", step);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Load the count of root certificate files.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.1 Load the count of root certificates.\n", step);
		rootCertCount = conf.rootCertFiles.size();
		if ((rootCertCount > MAX_ROOT_CERT_COUNT) || (rootCertCount == 0))
		{
			Log::log(Logger::ERROR, "Invalid count of root certificates. Should between 1 - 4.\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		Log::log(Logger::INFO2, "\tSuccess. (Root Certificate Count = %d)\n", rootCertCount);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Load the Index of used root certificate files.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.2 Load selected certificate chain id, used to sign this image.\n", step);
		uint32_t usedRootCertIndex = conf.mainCertId;
		if ((usedRootCertIndex >= MAX_ROOT_CERT_COUNT) || (usedRootCertIndex >= rootCertCount))
		{
			Log::log(Logger::INFO2, "Invalid index of selected certificate chain. Should between 0 to %d.\n", rootCertCount - 1);
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		Log::log(Logger::INFO2, "\tSuccess. (Selected certificatate chain index = %d)\n", usedRootCertIndex);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Load all the Root certificate files.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.3 Load all root certificates.\n", step);
		memset(&rkh, 0, sizeof(rkh_table_t));

		for (int i = 0; i < MAX_ROOT_CERT_COUNT; i++) {
			if (conf.rootCertFiles[i].size() == 0) {
				if (usedRootCertIndex == i) 
				{
					Log::log(Logger::ERROR, "\tRoot certificate %d, which is part of selected certificate chain, cannot be empty.\n", i);
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}
				else
				{
					continue;
				}
			}

			const char* rootCertFile = (conf.rootCertFiles[i]).c_str();
			FILE *rootCrtFile = fopen(rootCertFile, "rb");
			if (rootCrtFile == NULL)
			{
				Log::log(Logger::INFO2, "\tCannot open root certificate file %d (%s).\n", i, rootCertFile);
				cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
			fclose(rootCrtFile);
			if (conf.mainCertId == i)
			{
				usedRootCertPath = rootCertFile;
			}
			mbedtls_x509_crt rootCrt;
			mbedtls_x509_crt_init(&rootCrt);

			ret = mbedtls_x509_crt_parse_file(&rootCrt, rootCertFile);
			if (ret != 0)
			{
				Log::log(Logger::ERROR, "\tFailed to parse root certificate file %d(%s)(error code = %d).\nExpected X.509 certificate in DER format.\n", i, rootCertFile, ret);
				cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}

			uint32_t flags;
			ret = mbedtls_x509_crt_verify(&rootCrt, &rootCrt, NULL, NULL, &flags, NULL, NULL);
			if (ret != 0)
			{
				Log::log(Logger::ERROR, "\tRoot certificate %d must be selfsigned.\n", i);
				cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
			else
			{
				Log::log(Logger::INFO2, "\tRoot certificate %d is self signed.\n", i);
			}

			size_t n_size = mbedtls_mpi_size(&((mbedtls_rsa_context *)rootCrt.pk.pk_ctx)->N);
			size_t e_size = mbedtls_mpi_size(&((mbedtls_rsa_context *)rootCrt.pk.pk_ctx)->E);
			uint8_t *buf = (uint8_t *)calloc(1, n_size + e_size);
			mbedtls_mpi_write_binary(&((mbedtls_rsa_context *)rootCrt.pk.pk_ctx)->N, buf, n_size);
			mbedtls_mpi_write_binary(&((mbedtls_rsa_context *)rootCrt.pk.pk_ctx)->E, buf + n_size, e_size);
			mbedtls_sha256(buf, n_size + e_size, rkh.entries[i].rkh, false);

			mbedtls_x509_crt_free(&rootCrt);
			Log::log(Logger::INFO2, "\tSuccess. (Root Certificate %d = %s)\n", i, rootCertFile);
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//  Calculate used root certificate size.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.4 Calculate size of root certificates.\n", step);
		usedRootCrtFile = fopen(usedRootCertPath.c_str(), "rb");
		if (usedRootCrtFile == NULL)
		{
			Log::log(Logger::ERROR, "\tCannot open the selected root certificate file %s.\n", usedRootCertPath.c_str());
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		// Get image length in bytes.
		fseek(usedRootCrtFile, 0, SEEK_END);
		usedRootCrtSize = ftell(usedRootCrtFile);
		fseek(usedRootCrtFile, 0, SEEK_SET);

		if (usedRootCrtSize % AUTH_IMAGE_ALIGNMENT)
		{
			usedRootCrtAlignedSize = usedRootCrtSize + (AUTH_IMAGE_ALIGNMENT - (usedRootCrtSize % AUTH_IMAGE_ALIGNMENT));
		}
		else
		{
			usedRootCrtAlignedSize = usedRootCrtSize;
		}

		Log::log(Logger::INFO2, "\tSuccess. (Root Certificate Size = %lu bytes, Aligned Size = %d bytes)\n", usedRootCrtSize, usedRootCrtAlignedSize);
		step++;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Load all certificates.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. Load all certificates in selected certificate chain.\n", step);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Load the count of the certificates.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.1 Load the count of chained certificates in selected certificate chain.\n", step);
		certCount = conf.chainCertFiles.size();
		if (certCount < 0)
		{
			Log::log(Logger::ERROR, "Invalid count of the certificates chained in selected certificate chain.\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		Log::log(Logger::INFO2, "\tSuccess. (Certificate count = %d)\n", certCount);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Load all certificates.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.2 Load and parse certificates in selected certificate chain.\n", step);
		trustCrt = (mbedtls_x509_crt *)malloc(sizeof(mbedtls_x509_crt));
		mbedtls_x509_crt_init(trustCrt);
		ret = mbedtls_x509_crt_parse_file(trustCrt, usedRootCertPath.c_str());
		if (ret != 0)
		{
			Log::log(Logger::ERROR, "\tFailed to parse selected root certificate file (%s)(error code = %d).\n", usedRootCertPath.c_str(), ret);
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		if (certCount > 0) {
			for (uint32_t i = 0; i < certCount; i++)
			{
				const char* chainCertFile = conf.chainCertFiles[i].c_str();
				FILE *crtFile = fopen(chainCertFile, "rb");
				if (crtFile == NULL)
				{
					Log::log(Logger::INFO2, "\tCannot open certificate file %d(%s)(error code = %d).\n", i, chainCertFile, ret);
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}
				fclose(crtFile);

				mbedtls_x509_crt *testCrt = (mbedtls_x509_crt *)malloc(sizeof(mbedtls_x509_crt));
				mbedtls_x509_crt_init(testCrt);
				ret = mbedtls_x509_crt_parse_file(testCrt, chainCertFile);
				if (ret != 0)
				{
					Log::log(Logger::ERROR, "\tFailed to parse certificate file %d(%s)(error code = %d).\n", i, chainCertFile, ret);
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}

				uint32_t flags;
				ret = mbedtls_x509_crt_verify(testCrt, trustCrt, NULL, NULL, &flags, NULL, NULL);
				if (ret != 0)
				{
					Log::log(Logger::WARNING, "\tWARNING!!!: Verify of certificate chain failed at index %d (%s)(error code = %d).\n\tWARNING!!!: Certificate chain is not following rules, image may not be accepted by Bootloader.\n", i, chainCertFile, ret);
				}
				mbedtls_x509_crt_free(trustCrt);
				free(trustCrt);
				trustCrt = testCrt;
				testCrt = NULL;

				crtList.push_back(chainCertFile);
				Log::log(Logger::INFO2, "\tSuccess. ( Chained certificate %d (%s))\n", i, chainCertFile);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Calculate all the certificates size.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.3 Calculate size of all chained certificates.\n", step);
		size_t crtFileTotalSize = 0;

		size_t i = 0;
		for (vector<string>::iterator it = crtList.begin(); it != crtList.end(); it++, i++)
		{
			FILE *crtFile = fopen(it->c_str(), "rb");
			if (crtFile == NULL)
			{
				Log::log(Logger::ERROR, "\tCannot open the used certificate file %s.\n", it->c_str());
				cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
			fseek(crtFile, 0, SEEK_END);
			size_t crtFileSize = ftell(crtFile);
			fseek(crtFile, 0, SEEK_SET);
			fclose(crtFile);

			uint32_t crtFileAlignedSize;
			if (crtFileSize % AUTH_IMAGE_ALIGNMENT)
			{
				crtFileAlignedSize = crtFileSize + (AUTH_IMAGE_ALIGNMENT - (crtFileSize % AUTH_IMAGE_ALIGNMENT));
			}
			else
			{
				crtFileAlignedSize = crtFileSize;
			}

			crtFileTotalSize += crtFileAlignedSize;

			Log::log(Logger::INFO2, "\tSuccess. (Chained certificate %d size = %lu bytes, Aligned size = %d bytes)\n", i, crtFileSize, crtFileAlignedSize);
		}
		Log::log(Logger::INFO2, "\tSuccess. (Chained certificates total size = %lu bytes)\n", crtFileTotalSize);
		step++;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Load and parse the private key.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. Load and parse the private key of certificate used for signing of output image.\n", step);
		mbedtls_pk_context pk_ctx;
		mbedtls_pk_init(&pk_ctx);
		if ((ret = mbedtls_pk_parse_keyfile(&pk_ctx, conf.mainCertPrivateKeyFile.c_str(), NULL)) != 0)
		{
			Log::log(Logger::ERROR, "Parse private key failed.(%d)(%s)\n", ret, conf.mainCertPrivateKeyFile.c_str());
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		// Get the private key.
		rsa_priv_key_ctx = (mbedtls_rsa_context *)(pk_ctx.pk_ctx);
		Log::log(Logger::INFO2, "\tSuccess. (Private Key File Path = %s\n", conf.mainCertPrivateKeyFile.c_str());
		signatureSizeInBytes = rsa_priv_key_ctx->len;
		step++;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Generate the entire image data
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. Calculate certificate table size.\n", step);

		// Calculate CRT table size and align it to word boundary.
		crtTableSize = usedRootCrtAlignedSize + crtFileTotalSize + 4 * (certCount + 1 /*root*/);
		// Align the crt table size to word boundary.

		if (crtTableSize % AUTH_IMAGE_ALIGNMENT)
		{
			crtTableAlignedSize = crtTableSize + (AUTH_IMAGE_ALIGNMENT - (crtTableSize % AUTH_IMAGE_ALIGNMENT));
		}
		else
		{
			crtTableAlignedSize = crtTableSize;
		}
		Log::log(Logger::INFO2, "\tSuccess.\n");
		// Calculate entire image size including image header and crt.
		totalImageSize = imageAlignedSize + sizeof(certificate_block_header_t) + crtTableAlignedSize + sizeof(rkh_table_t);
		step++;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Initialization of random number generator for mbedtls
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d Initialization of random number generator.\n", step);
		// mbedTLS drbg will be used to generate random numbers
		// Init deterministic random bit generator.
		mbedtls_ctr_drbg_init(&ctr_drbg);
		// Init entropy.
		mbedtls_entropy_context entropy;
		mbedtls_entropy_init(&entropy);

		ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
		if (ret != 0)
		{
			Log::log(Logger::INFO2, "Seed random bit generator failed.%d\n", ret);
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		Log::log(Logger::INFO2, "\tSuccess.\n");
	}
	
	if ((imageType == kSKBOOT_ImageTypeEncryptedSigned) || (imageType == kSKBOOT_ImageTypeEncryptedSignedKeyStoreIncluded))
	{
		// Malloc a buffer to contain the entire image data plus ES image tail.
		imgTailSize = ES_IMG_TAIL_BYTELEN;
		size_t allocSize = totalImageSize + ES_IMG_TAIL_BYTELEN + trustZonePresetFileSize;
		buffer = (uint8_t *)malloc(allocSize);
		// Init the buffer. padding bytes for the aligned are 0x00s.
		memset(buffer, 0, allocSize);
	}
	else
	{
		// Malloc a buffer to contain the entire image data.
		size_t allocSize = totalImageSize + trustZonePresetFileSize;
		buffer = (uint8_t *)malloc(allocSize);
		// Init the buffer. padding bytes for the aligned are 0x00s.
		memset(buffer, 0, allocSize);
	}
	step++; //8
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Cp the original image data
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d. Load the input image data to buffer.\n", step);
	// Load image file to buffer.
	ret = fread(buffer, 1, imageSize, imageFile);
	Log::log(Logger::INFO2, "\tSuccess.\n");
	if (ret != imageSize)
	{
		Log::log(Logger::ERROR, "Fail reading image file.\n", configdata);
		cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}

	/* LoadToRam signed images all have header MAC and optionally key store inserted at offset 64 bytes */
	if ((imageType == kSKBOOT_ImageTypeEncryptedSignedKeyStoreIncluded) || (imageType == kSKBOOT_ImageTypePlainSignedKeyStoreIncluded)
		|| (imageType == kSKBOOT_ImageTypeEncryptedSigned) || (imageType == kSKBOOT_ImageTypePlainSigned))
	{
		step++;
		configdata = conf.imageEncryptionKeyFile.c_str();
		Log::log(Logger::INFO2, "%d. Load the HMAC/image encryption key (%s).\n", step, configdata);
		// Read userkey
		ifstream userKeyFile(conf.imageEncryptionKeyFile, ios_base::in);
		
		if (userKeyFile.fail())
		{
			Log::log(Logger::ERROR, "Cannot open ImageEncryptionKey file %s.\n", configdata);
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		userKeyFile.seekg(0, ios::end);
		userKeySize = (size_t)userKeyFile.tellg() / 2;
		userKeyFile.seekg(0, ios::beg);
		try {
			if (userKeySize >= 16 && userKeySize < 32) {
				userKeySize = 16;
				AESKey<128> aes(userKeyFile);
				userKey = new uint8_t[userKeySize];
				aes.getKey((AESKey<128>::key_t*)userKey);
			}
			else if (userKeySize >= 32) {
				userKeySize = 32;
				AESKey<256> aes(userKeyFile);
				userKey = new uint8_t[userKeySize];
				aes.getKey((AESKey<256>::key_t*)userKey);
			}
			else {
				Log::log(Logger::INFO2, "ImageEncryptionKey have unexpected size (16/32): %u\n", userKeySize);
				cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
			userKeyFile.close();
		}
		catch (exception &e) {
			Log::log(Logger::ERROR, "Cannot parse ImageEncryptionKey key: %s.\n", e.what());
			userKeyFile.close();
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		Log::log(Logger::INFO2, "\tSuccess.\n");
	}

	// add header MAC for all LoadToRam images
	size_t headerMacSizeInBytes = 0;
	if ((imageType == kSKBOOT_ImageTypeEncryptedSignedKeyStoreIncluded) || (imageType == kSKBOOT_ImageTypePlainSignedKeyStoreIncluded)
		|| (imageType == kSKBOOT_ImageTypeEncryptedSigned) || (imageType == kSKBOOT_ImageTypePlainSigned))
	{
		headerMacSizeInBytes = sizeof(s_headerMac);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Add key store
	////////////////////////////////////////////////////////////////////////////////////////////////////
	if ((imageType == kSKBOOT_ImageTypeEncryptedSignedKeyStoreIncluded) || (imageType == kSKBOOT_ImageTypePlainSignedKeyStoreIncluded))
	{
		step++;
		Log::log(Logger::INFO2, "%d. Key store configuration.\n", step);
		if (conf.deviceKeySrc == deviceKeySource::OTP)
			Log::log(Logger::INFO2, "\tDevice is using OTP, skiping Key Store.\n", step);
		else if (conf.useKeyStore) {
			if (conf.keyStoreFile.length() > 0)
			{
				Log::log(Logger::INFO2, "%d.1 Add key store from specified file.\n", step);
				FILE *keyStoreFile = NULL;
				configdata = conf.keyStoreFile.c_str();
				keyStoreFile = fopen(configdata, "rb");
				if (NULL == keyStoreFile)
				{
					Log::log(Logger::INFO2, "Cannot open key store file %s.\n", configdata);
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}

				fseek(keyStoreFile, 0, SEEK_END);
				auto keyStoreFileSize = ftell(keyStoreFile);
				Log::log(Logger::INFO2, "\tKey Store File Path = %s, size = %lu bytes\n", configdata, keyStoreFileSize);
				fseek(keyStoreFile, 0, SEEK_SET);

				if (LPC55XX == conf.family || NIOBE4MINI == conf.family)
				{
					keyStoreSize = sizeof(skboot_key_store_lpc55xx_t);
					keyStore = new uint8_t[keyStoreSize]{};
					if (keyStoreSize != keyStoreFileSize)
					{
						Log::log(Logger::ERROR, "\tWrong size of key store file for %s. (expected: %d bytes, fetched: %lu bytes)\n", conf.family.c_str(), keyStoreSize, keyStoreFileSize);
						cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
						return FAIL_RETURN;
					}
					ret = fread(keyStore, 1, keyStoreSize, keyStoreFile);
				}
				else if (RT6XX == conf.family || RT5XX == conf.family)
				{
					keyStoreSize = sizeof(skboot_key_store_lpc68xx_t);
					keyStore = new uint8_t[keyStoreSize]{};
					if (keyStoreSize != keyStoreFileSize)
					{
						Log::log(Logger::ERROR, "\tWrong size of key store file for %s. (expected: %d bytes, fetched: %lu bytes)\n", conf.family.c_str(), keyStoreSize, keyStoreFileSize);
						cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
						return FAIL_RETURN;
					}
					ret = fread(keyStore, 1, keyStoreSize, keyStoreFile);
				}
				else
				{
					Log::log(Logger::ERROR, "Unsupported device family: %s.\n", conf.family.c_str());
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}
				if (ret != keyStoreSize)
				{
					Log::log(Logger::ERROR, "Fail reading key store file.\n");
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}
			}
			else
			{
				Log::log(Logger::INFO2, "%d.1 Reserve key store space only.\n", step);
				if (LPC55XX == conf.family || NIOBE4MINI == conf.family)
				{
					keyStoreSize = sizeof(skboot_key_store_lpc55xx_t);
					keyStore = new uint8_t[keyStoreSize]{};
				}
				else if (RT6XX == conf.family || RT5XX == conf.family)
				{
					keyStoreSize = sizeof(skboot_key_store_lpc68xx_t);
					keyStore = new uint8_t[keyStoreSize]{};
				}
				else
				{
					Log::log(Logger::ERROR, "Unsupported device family: %s.\n", conf.family.c_str());
					cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
					return FAIL_RETURN;
				}
				Log::log(Logger::INFO, "\tKey store with size %lu bytes will be reserved in image from address: 0x%x.\n", keyStoreSize, HMACHEADEROFSET + headerMacSizeInBytes);
			}
		}
		Log::log(Logger::INFO2, "\tSuccess.\n");
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Add trust zone preset record if specified.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	if (trustZonePresetFileSize > 0) {
		step++;
		Log::log(Logger::INFO2, "%d. Add trust zone preset data.\n", step);
		if (conf.generateTrustZone) {
			auto k{ 0 };
			uintptr_t address = (uintptr_t)buffer + totalImageSize + imgTailSize;
			for (auto const value : *trustZonedata) {
				*(uint32_t*)(address + k) = value;
				k += sizeof(uint32_t);
			}
		}
		else {
			ret = fread(buffer + totalImageSize + imgTailSize, 1, trustZonePresetFileSize, trustZonePresetFile);
			if (ret != trustZonePresetFileSize)
			{
				Log::log(Logger::ERROR, "Fail reading trust zone preset data from file.\n");
				cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
		}
		Log::log(Logger::INFO2, "\tSuccess.\n");
	}

	if ((imageType == kSKBOOT_ImageTypeEncryptedSigned) || (imageType == kSKBOOT_ImageTypeEncryptedSignedKeyStoreIncluded))
	{
		step++;
		Log::log(Logger::INFO2, "%d. Updating image header before encryption.\n", step);
		// Modify the header offsize.
		*(uint32_t *)(buffer + 0x28) = imageAlignedSize;
		*(uint32_t *)(buffer + 0x24) = imageType | imageTypeTZM | enableHwKeys;
		*(uint32_t *)(buffer + 0x20) = totalImageSize + ES_IMG_TAIL_BYTELEN + signatureSizeInBytes + keyStoreSize + headerMacSizeInBytes + trustZonePresetFileSize;
		*(uint32_t *)(buffer + 0x34) = imageLinkAddress;
		Log::log(Logger::INFO2, "\tSuccess. (Image Type = 0x%08x, Image load address = 0x%x, Total Image size = %d bytes)\n", *(uint32_t *)(buffer + 0x24), *(uint32_t *)(buffer + 0x34), *(uint32_t *)(buffer + 0x20));
		step++;

		Log::log(Logger::INFO2, "%d. Encryption - Generate random IV.\n", step);
		// Generate random ctrInitVector
		if (0 != mbedtls_ctr_drbg_random(&ctr_drbg, ctrInitVector, sizeof(ctrInitVector)))
		{
			Log::log(Logger::ERROR, "Cannot generate random init vector for AES CTR encryption.\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		Log::log(Logger::INFO2, "\tSuccess. (IV: 0x%08x 0x%08x 0x%08x 0x%08x)\n", ((uint32_t*)ctrInitVector)[0], ((uint32_t*)ctrInitVector)[1], ((uint32_t*)ctrInitVector)[2], ((uint32_t*)ctrInitVector)[3]);

		// Save ctrInitVector to ES_IMG_TAIL
		memcpy(&buffer[totalImageSize + ES_IMG_TAIL_BYTELEN - sizeof(ctrInitVector)], ctrInitVector, sizeof(ctrInitVector));

		uint8_t *firmwareDecryptKey;
		size_t keySize;
		if (conf.deviceKeySrc == deviceKeySource::OTP) {
			/* for OTP case, firmware decrypt key is a derived key */
			firmwareDecryptKey = new uint8_t[32]{ 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
			0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0,
			0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
			keySize = 32;
			mbedtls_aes_context aesCtx;
			mbedtls_aes_init(&aesCtx);
			mbedtls_aes_setkey_enc(&aesCtx, userKey, 256);
			mbedtls_aes_crypt_ecb(&aesCtx, MBEDTLS_AES_ENCRYPT, &firmwareDecryptKey[0], &firmwareDecryptKey[0]);
			mbedtls_aes_crypt_ecb(&aesCtx, MBEDTLS_AES_ENCRYPT, &firmwareDecryptKey[16], &firmwareDecryptKey[16]);
			mbedtls_aes_free(&aesCtx);
			Log::log(Logger::INFO2, "%d.1 Encryption - Encrypting input plain image data using derived key from %s key.\n", step, conf.imageEncryptionKeyFile.c_str());
			
			Log::log(Logger::DEBUG2, "\tDerived encryption key: ");
			for (size_t i = 0; i < keySize; i++)
			{
				Log::log(Logger::DEBUG2, "%02x", firmwareDecryptKey[i]);
			}
			Log::log(Logger::DEBUG2, "\n");
		}
		else {
			firmwareDecryptKey = userKey;
			keySize = userKeySize;
			Log::log(Logger::INFO2, "%d.1 Encryption - Encrypting input plain image data using %s key.\n", step, conf.imageEncryptionKeyFile.c_str());
		}
					
		// Encrypt original image data
		size_t nc_off = 0;
		uint8_t stream_block[16] {};
		mbedtls_aes_context aesCtx;
		mbedtls_aes_init(&aesCtx);
		mbedtls_aes_setkey_enc(&aesCtx, firmwareDecryptKey, keySize * 8);
		mbedtls_aes_crypt_ctr(&aesCtx, imageAlignedSize, &nc_off, ctrInitVector, stream_block, buffer, buffer);
		mbedtls_aes_crypt_ctr(&aesCtx, trustZonePresetFileSize, &nc_off, ctrInitVector, stream_block, buffer + totalImageSize + ES_IMG_TAIL_BYTELEN, buffer + totalImageSize + ES_IMG_TAIL_BYTELEN);
		mbedtls_aes_free(&aesCtx);

		if (firmwareDecryptKey != userKey) {
			delete[] firmwareDecryptKey;
			firmwareDecryptKey = nullptr;
		}

		// Save encrypted vector table to ES_IMG_TAIL
		memcpy(&buffer[totalImageSize], buffer, sizeof(es_header_t));

		totalImageSize += ES_IMG_TAIL_BYTELEN;
		Log::log(Logger::INFO2, "\tSuccess.\n");
	}
	totalImageSize += trustZonePresetFileSize;

	step++;
	Log::log(Logger::INFO2, "%d. Updating image header.\n", step);
	// Modify the header offsize.
	*(uint32_t *)(buffer + 0x28) = imageAlignedSize;
	*(uint32_t *)(buffer + 0x24) = imageType | imageTypeTZM | enableHwKeys;
	*(uint32_t *)(buffer + 0x20) = totalImageSize + signatureSizeInBytes + keyStoreSize + headerMacSizeInBytes;
	*(uint32_t *)(buffer + 0x34) = imageLinkAddress;

	Log::log(Logger::INFO2, "\tSuccess. (Image Type = 0x%08x, Image load address = 0x%x, Total Image size = %d bytes)\n", *(uint32_t *)(buffer + 0x24), *(uint32_t *)(buffer + 0x34), *(uint32_t *)(buffer + 0x20));
	
	if (imageType != kSKBOOT_ImageTypeXipPlainCrc && imageType != kSKBOOT_ImageTypePlainCrc)
	{
		step++;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Init certificate header
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. Init certificate header.\n", step);
		certificate_block_header_t *header = (certificate_block_header_t *)(buffer + imageAlignedSize);

		header->signature = SIGNATURE_TAG;
		header->headerMajorVersion = 1;
		header->headerMinorVersion = 0;
		header->headerLengthInBytes = sizeof(certificate_block_header_t);
		header->flags = 0;
		header->buildNumber = conf.buildNumber;
		header->totalImageLengthInBytes = totalImageSize;
		header->certificateCount = 1 /*root cert*/ + certCount;
		header->certificateTableLengthInBytes = crtTableAlignedSize;

		uint32_t crtTableOffset = imageAlignedSize + sizeof(certificate_block_header_t);
		uint32_t crtOffset = crtTableOffset;
		Log::log(Logger::INFO2, "\tSuccess.\n");
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Add root certificate
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.1 Add root certificate.\n", step);
		*(uint32_t *)(buffer + crtOffset) = usedRootCrtAlignedSize;
		crtOffset += 4;
		fread(buffer + crtOffset, 1, usedRootCrtSize, usedRootCrtFile);
		crtOffset += usedRootCrtAlignedSize;
		Log::log(Logger::INFO2, "\tSuccess.\n");
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Add certificate
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.2 Add chained certificates from selected chain.\n", step);
		int i = 0;
		for (vector<string>::iterator it = crtList.begin(); it != crtList.end(); it++, i++)
		{
			FILE *crtFile = fopen(it->c_str(), "rb");
			if (crtFile == NULL)
			{
				Log::log(Logger::ERROR, "\tCannot open the used certificate file %s.\n", usedRootCertPath.c_str());
				cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
			fseek(crtFile, 0, SEEK_END);
			size_t crtFileSize = ftell(crtFile);
			fseek(crtFile, 0, SEEK_SET);

			uint32_t crtFileAlignedSize;
			if (crtFileSize % AUTH_IMAGE_ALIGNMENT)
			{
				crtFileAlignedSize = crtFileSize + (AUTH_IMAGE_ALIGNMENT - (crtFileSize % AUTH_IMAGE_ALIGNMENT));
			}
			else
			{
				crtFileAlignedSize = crtFileSize;
			}

			*(uint32_t *)(buffer + crtOffset) = crtFileAlignedSize;
			crtOffset += 4;
			fread(buffer + crtOffset, 1, crtFileSize, crtFile);
			crtOffset += crtFileAlignedSize;
			fclose(crtFile);
		}

		if ((crtOffset - crtTableOffset) != crtTableSize)
		{
			Log::log(Logger::ERROR, "\tgenerate certificate table failed. table size is %d, but should be %lu.\n", crtOffset - crtTableOffset, crtTableSize);
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		Log::log(Logger::INFO2, "\tSuccess. (Certificate table size = %d bytes)\n", crtOffset - crtTableOffset);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Add root hash key table
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d.3 Add root hash key table (RKTH).\n", step);
		rkhOffset = crtOffset;
		memcpy(buffer + rkhOffset, &rkh, sizeof(rkh_table_t));
		Log::log(Logger::INFO2, "\tSuccess.\n");
	}

	if (imageType != kSKBOOT_ImageTypeXipPlainCrc && imageType != kSKBOOT_ImageTypePlainCrc)
	{
		step++;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Generate the digest.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. Generate output image sha256 digest for image signature.\n", step);
		// Buffer contains digest.
		uint8_t digest[32] = { 0 };

		// Call mbedtls to calculate digest.
		mbedtls_sha256_context sha256_ctx;
		mbedtls_sha256_init(&sha256_ctx);
		mbedtls_sha256_starts(&sha256_ctx, false /*isSHA224 = false*/);
		mbedtls_sha256_update(&sha256_ctx, buffer, totalImageSize);
		mbedtls_sha256_finish(&sha256_ctx, digest);

		Log::log(Logger::INFO2, "\tSuccess.\n");
		Log::log(Logger::INFO2, "\tSHA256 digest:");
		for (size_t i = 0; i < 32; i++)
		{
			Log::log(Logger::INFO2, " %#x", digest[i]);
		}
		Log::log(Logger::INFO2, "\n");
		step++;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Generate the signature.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. Generate output image signature.\n", step);
		// Buffer contains signature.
		signature = (uint8_t *)malloc(signatureSizeInBytes);
		memset(signature, 0, signatureSizeInBytes);

		ret = mbedtls_rsa_rsassa_pkcs1_v15_sign(rsa_priv_key_ctx, mbedtls_ctr_drbg_random, &ctr_drbg, MBEDTLS_RSA_PRIVATE,
			MBEDTLS_MD_SHA256, 256, digest, signature);
		if (ret != 0)
		{
			Log::log(Logger::ERROR, "Generate signature failed.(%d)\n", ret);
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}

		Log::log(Logger::INFO2, "\tSuccess.\n");
		Log::log(Logger::INFO2, "\tSignature:");
		for (size_t i = 0; i < signatureSizeInBytes; i++)
		{
			Log::log(Logger::INFO2, " %#x", signature[i]);
		}
		Log::log(Logger::INFO2, "\n");
		step++;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Verify the signature.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. Verify signature.\n", step);
		mbedtls_rsa_context *rsa_pub_key_ctx = (mbedtls_rsa_context *)(trustCrt->pk.pk_ctx);
		ret = mbedtls_rsa_rsassa_pkcs1_v15_verify(rsa_pub_key_ctx, NULL, NULL, MBEDTLS_RSA_PUBLIC, MBEDTLS_MD_SHA256, 256,
			digest, signature);
		free(rsa_pub_key_ctx);
		if (ret != 0)
		{
			Log::log(Logger::ERROR, "Verify signature failed.(%d)\n", ret);
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}

		Log::log(Logger::INFO2, "\tSuccess.\n");
	}

	if (imageType == kSKBOOT_ImageTypeXipPlainCrc || imageType == kSKBOOT_ImageTypePlainCrc)
	{
		step++; 
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Generate the CRC.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. CRC calculation.\n", step);
		uint32_t crcResult = 0;
		CRC32 crc;
		//skipping 4 bytes from image, because this 4 bytes will be used for storing CRC checksum
		crc.update(buffer, 0x28);
		crc.update(buffer + 0x2C, totalImageSize - 0x2C);
		crc.truncatedFinal(reinterpret_cast<uint8_t *>(&crcResult), sizeof(crcResult));

		Log::log(Logger::INFO2, "\tCRC32 checksum: 0x%X\n", crcResult);
		Log::log(Logger::INFO2, "\tSuccess.\n");
		*(uint32_t *)(buffer + 0x28) = crcResult;
	}
	
	if (imageType == kSKBOOT_ImageTypeEncryptedSignedKeyStoreIncluded || imageType == kSKBOOT_ImageTypePlainSignedKeyStoreIncluded
		|| imageType == kSKBOOT_ImageTypeEncryptedSigned || imageType == kSKBOOT_ImageTypePlainSigned)
	{
		step++;
		Log::log(Logger::INFO2, "%d. Calculate HMAC of output image header using derived key from %s key.\n", step, conf.imageEncryptionKeyFile.c_str());
		/* compute MAC of the 1st 64 bytes */
		/* MAC = HMAC-SHA256(key = AES_ECB(SBKEK, zeroAesBlock), buffer, 64) */
		uint8_t zeroKey[16] = { 0 };
		mbedtls_aes_context aesCtx;
		mbedtls_aes_init(&aesCtx);
		mbedtls_aes_setkey_enc(&aesCtx, userKey, userKeySize * 8);
		mbedtls_aes_crypt_ecb(&aesCtx, MBEDTLS_AES_ENCRYPT, zeroKey, zeroKey);
		mbedtls_aes_free(&aesCtx);

		Log::log(Logger::DEBUG2, "\tDerived HMAC key: ");
		for (size_t i = 0; i < sizeof(zeroKey); i++)
		{
			Log::log(Logger::DEBUG2, "%02x", zeroKey[i]);
		}
		Log::log(Logger::DEBUG2, "\n");

		mbedtls_md_hmac(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), zeroKey, sizeof(zeroKey), buffer, HMACHEADEROFSET, (uint8_t*)&s_headerMac);
	}

	if (imageType != kSKBOOT_ImageTypeXipPlainCrc && imageType != kSKBOOT_ImageTypePlainCrc) {
		step++;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		// Output the root key hash table SHA256 hash.
		////////////////////////////////////////////////////////////////////////////////////////////////////
		Log::log(Logger::INFO2, "%d. Output the root certificates SHA256 hash (RKTH).\n", step);
	
		uint8_t rkhtHash[32];
		mbedtls_sha256_context rkhtHash_sha256_ctx;
		mbedtls_sha256_init(&rkhtHash_sha256_ctx);
		mbedtls_sha256_starts(&rkhtHash_sha256_ctx, false /*isSHA224 = false*/);
		mbedtls_sha256_update(&rkhtHash_sha256_ctx, (const unsigned char *)(buffer + rkhOffset), sizeof(rkh_table_t));
		mbedtls_sha256_finish(&rkhtHash_sha256_ctx, (uint8_t *)rkhtHash);

		Log::log(Logger::INFO2, "\tSuccess.\n");
		Log::log(Logger::INFO, "\tRKTH: ");
		for (size_t i = 0; i < 32; i++)
		{
			Log::log(Logger::INFO, "%02x", rkhtHash[i]);
		}
		Log::log(Logger::INFO, "\n");
	}

	step++;
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Output the image.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d. Creating output image file.\n", step);
	configdata = conf.masterBootOutputFile.c_str();
	outputFile = fopen(configdata, "wb");
	if (outputFile == NULL)
	{
		Log::log(Logger::ERROR, "\tCannot open/create output file %s.\n", configdata);
		cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}
	Log::log(Logger::INFO2, "\tSuccess.\n");

	Log::log(Logger::INFO2, "%d.1 Writing output image to file.\n", step);
	if (imageType == kSKBOOT_ImageTypeXipPlainCrc || imageType == kSKBOOT_ImageTypePlainCrc) //CRC
	{
		ret = fwrite(buffer, 1, totalImageSize, outputFile);
		if (ret != totalImageSize)
		{
			Log::log(Logger::ERROR, "\tWrite CRC image to output file failed.\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		Log::log(Logger::INFO, "\tSuccess. (CRC image file: %s created)\n", conf.masterBootOutputFile.c_str());
		cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return SUCCESS_RETURN;
	}/* LoadToRam signed images all have header MAC and optionally key store inserted at offset 64 bytes */
	else if (imageType == kSKBOOT_ImageTypeEncryptedSignedKeyStoreIncluded || imageType == kSKBOOT_ImageTypePlainSignedKeyStoreIncluded
		|| imageType == kSKBOOT_ImageTypeEncryptedSigned || imageType == kSKBOOT_ImageTypePlainSigned)
	{
		/* Now write outputFile */
		ret = fwrite(buffer, 1, HMACHEADEROFSET, outputFile);
		if (ret != HMACHEADEROFSET)
		{
			Log::log(Logger::INFO2, "\tWrite image header data to output file failed..\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		/* MAC of the initial 64 bytes (header) */
		ret = fwrite(&s_headerMac, 1, headerMacSizeInBytes, outputFile);
		if (ret != headerMacSizeInBytes)
		{
			Log::log(Logger::ERROR, "\tWrite header MAC data to output file failed..\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		/* optional key store */
		if ((imageType == kSKBOOT_ImageTypeEncryptedSignedKeyStoreIncluded) || (imageType == kSKBOOT_ImageTypePlainSignedKeyStoreIncluded))
		{
			ret = fwrite(keyStore, 1, keyStoreSize, outputFile);
			if (ret != keyStoreSize)
			{
				Log::log(Logger::INFO2, "\tWrite key store data to output file failed..\n");
				cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
		}
		ret = fwrite(buffer + 64, 1, totalImageSize - HMACHEADEROFSET, outputFile);
		if (ret != (totalImageSize - HMACHEADEROFSET))
		{
			Log::log(Logger::ERROR, "\tWrite total image data to output file failed..\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		ret = fwrite(signature, 1, signatureSizeInBytes, outputFile);
		if (ret != signatureSizeInBytes)
		{
			Log::log(Logger::ERROR, "\tWrite signature to output file failed..\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
	}
	else
	{
		ret = fwrite(buffer, 1, totalImageSize, outputFile);
		if (ret != totalImageSize)
		{
			Log::log(Logger::ERROR, "\tWrite total image data to output file failed..\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}

		ret = fwrite(signature, 1, signatureSizeInBytes, outputFile);
		if (ret != signatureSizeInBytes)
		{
			Log::log(Logger::INFO2, "\tWrite signature to output file failed..\n");
			cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
	}
	configdata = conf.masterBootOutputFile.c_str();
	Log::log(Logger::INFO, "\tSuccess. (Signed image %s created.)\n", configdata);
	
	cleanUpLpcSboot(userKey, keyStore, imageFile, usedRootCrtFile, outputFile, trustZonePresetFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
	return SUCCESS_RETURN;
}

void AuthImageGenerator::cleanUpLpcSboot(uint8_t *userKey, uint8_t * keyStore, FILE * imageFile, FILE * usedRootCrtFile, FILE * outputFile, FILE * trustZonePresetFile, uint8_t * buffer, uint8_t * signature, mbedtls_x509_crt * trustCrt, mbedtls_rsa_context * rsa_priv_key_ctx)
{
	if (imageFile != NULL)
	{
		fclose(imageFile);
	}
	if (usedRootCrtFile != NULL)
	{
		fclose(usedRootCrtFile);
	}
	if (outputFile != NULL)
	{
		fclose(outputFile);
	}
	if (trustZonePresetFile != NULL)
	{
		fclose(trustZonePresetFile);
	}
	if (buffer != NULL)
	{
		free(buffer);
	}
	if (signature != NULL)
	{
		free(signature);
	}
	if (trustCrt != NULL)
	{
		free(trustCrt);
	}
	if (rsa_priv_key_ctx != NULL)
	{
		free(rsa_priv_key_ctx);
	}
	if (keyStore != NULL)
	{
		delete keyStore;
	}
	if (userKey != NULL)
	{
		delete[] userKey;
	}
}

bool AuthImageGenerator::parseConfigurationK3Sboot(const string &confFilePath, configuration &conf)
{
	string tmp;
	auto error = false;
	stringstream ss;
	ss << "Parsing configuration file: " << confFilePath << ".\n";
	Log::log(Logger::INFO, ss.str());
	ss.str("");
	jute::jValue jsonConf;

	try {
		jsonConf = jute::parser::parse_file(confFilePath);
	}
	catch (runtime_error &e) {
		throw runtime_error(("Cannot parse json configuration file: " + confFilePath + " - " + e.what()).c_str());
	}
	catch (...) {
		throw runtime_error("Cannot parse json configuration file: " + confFilePath);
	}

	if (jsonConf["family"].get_type() == jute::JSTRING) {
		conf.family = jsonConf["family"].as_string();
		transform(conf.family.begin(), conf.family.end(), conf.family.begin(), ::tolower);
		if (conf.family != "k32w0x") {
			ss << "\tUnexpected \"family\" value (" << conf.family << ") from configuration file: " << confFilePath << ".\n";
			ss << "\tSupported only k32w0x." << endl;
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
	}
	else {
		ss << "\tCannot read \"family\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (jsonConf["inputImageFile"].get_type() == jute::JSTRING)
		conf.imageFile = jsonConf["inputImageFile"].as_string();
	else {
		ss << "\tCannot read \"inputImageFile\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}	

	for (int i = 0; i < MAX_ROOT_CERT_COUNT; i++) {
		tmp = "rootCertificate" + to_string(i) + "File";
		if (jsonConf[tmp].get_type() == jute::JSTRING)
			conf.rootCertFiles.push_back(string(jsonConf[tmp].as_string()));
		else {
			ss << "\tCannot read \"" << tmp << "\" from configuration file: " << confFilePath << ".\n\tSkiping " << tmp << ".\n";
			Log::log(Logger::INFO, ss.str());
			ss.str("");
			conf.rootCertFiles.push_back(string(""));
		}
	}

	if (jsonConf["mainCertChainId"].get_type() == jute::JNUMBER) {
		conf.mainCertId = jsonConf["mainCertChainId"].as_int();
		if (conf.mainCertId > MAX_ROOT_CERT_COUNT - 1) {
			ss << "\tWrong value of \"mainCertChainId\" loaded from configuration file: " << confFilePath << ".\n";
			ss << "\tLoaded: " << conf.mainCertId << " value should be from 0  to " << MAX_ROOT_CERT_COUNT - 1 << ", using 0 as default.\n";
			Log::log(Logger::INFO, ss.str());
			ss.str("");
			conf.mainCertId = 0;
		}
	}
	else {
		ss << "\tCannot read \"mainCertChainId\" from configuration file: " << confFilePath << ".\n\tUsing 0 as default.\n";
		Log::log(Logger::INFO, ss.str());
		ss.str("");
	}

	if (conf.rootCertFiles[conf.mainCertId] == "") {
		ss << "\tSelected main root certificate/certificate chain \"mainCertChainId\" (" << (int)conf.mainCertId << ") from configuration file: " << confFilePath <<
			" don't have set corresponding value for \"rootCertificate" << (int)conf.mainCertId << "\".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	short i = 0;
	tmp = "chainCertificate" + to_string(conf.mainCertId) + "File";
	while (jsonConf[tmp + to_string(i)].get_type() == jute::JSTRING) {
		conf.chainCertFiles.push_back(string(jsonConf[tmp + to_string(i)].as_string()));
		i++;
	}

	ss << "\tMain certificate chain " << (int)conf.mainCertId << " with " << i + 1 << " certificate(s) was fetched from configuration file: " << confFilePath << ".\n";
	Log::log(Logger::DEBUG, ss.str());
	ss.str("");

	if (jsonConf["mainCertPrivateKeyFile"].get_type() == jute::JSTRING)
		conf.mainCertPrivateKeyFile = jsonConf["mainCertPrivateKeyFile"].as_string();
	else {
		ss << "\tCannot read \"mainCertPrivateKeyFile\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (jsonConf["masterBootOutputFile"].get_type() == jute::JSTRING)
		conf.masterBootOutputFile = jsonConf["masterBootOutputFile"].as_string();
	else {
		ss << "\tCannot read \"masterBootOutputFile\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (error)
		Log::log(Logger::WARNING, "\tParsed with error(s).\n");
	else
		Log::log(Logger::INFO, "\tSuccess.\n");

	return !error;
}

int AuthImageGenerator::createImageK3Sboot(const configuration &conf)
{
	uint32_t status = SUCCESS_RETURN;
	FILE *imageFile = NULL, *configFile = NULL, *usedRootCrtFile = NULL, *outputFile = NULL;
	uint8_t *buffer = NULL, *signature = NULL;
	char *configdata = NULL;
	mbedtls_x509_crt * trustCrt = NULL;
	mbedtls_rsa_context * rsa_priv_key_ctx = NULL;
	std::vector<std::string> crtList;
	std::string usedRootCertPath;
	int ret = -1, step = 1;

	Log::log(Logger::INFO, "Start to generate authenticated image!\n");

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Open image file.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Log::log(Logger::INFO2, "%d. Open the image file.(%s)\n", step, conf.imageFile.c_str());
	imageFile = fopen(conf.imageFile.c_str(), "rb");
	if (imageFile == NULL)
	{
		Log::log(Logger::INFO2, "\tCannot open image file %s.\n", conf.imageFile.c_str());
		return FAIL_RETURN;
		
	}
	// Get image length in bytes.
	fseek(imageFile, 0, SEEK_END);
	size_t imageSize = ftell(imageFile);
	fseek(imageFile, 0, SEEK_SET);
	// Get the word aligned length.
	size_t imageAlignedSize = 0;
	if (imageSize % AUTH_IMAGE_ALIGNMENT)
	{
		imageAlignedSize = imageSize + (AUTH_IMAGE_ALIGNMENT - (imageSize % AUTH_IMAGE_ALIGNMENT));
	}
	else
	{
		imageAlignedSize = imageSize;
	}

	Log::log(Logger::INFO2, "\tSuccess.(Size = %#x, AlignedSize = %#x)\n", imageSize, imageAlignedSize);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load the Root certificate files.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "3. Load the root certificates.\n");
	configdata = (char *)calloc(1, MAX_PATH);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load the count of root certificate files.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.1 Load the count of root certificates.\n", step);
	int rootCertCount = conf.rootCertFiles.size();
	if ((rootCertCount > MAX_ROOT_CERT_COUNT) || (rootCertCount == 0))
	{
		Log::log(Logger::ERROR, "Invalid count of root certificates. Should between 1 - 4.\n");
		cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}
	Log::log(Logger::INFO2, "\tSuccess. (Root Certificate Count = %d)\n", rootCertCount);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load the Index of used root certificate files.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.2 Load selected certificate chain id, used to sign this image.\n", step);
	uint32_t usedRootCertIndex = conf.mainCertId;
	if ((usedRootCertIndex >= MAX_ROOT_CERT_COUNT) || (usedRootCertIndex >= rootCertCount))
	{
		Log::log(Logger::INFO2, "Invalid index of selected certificate chain. Should between 0 to %d.\n", rootCertCount - 1);
		cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}
	Log::log(Logger::INFO2, "\tSuccess. (Selected certificatate chain index = %d)\n", usedRootCertIndex);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load all the Root certificate files.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.3 Load all root certificates.\n", step);
	rkh_table_t rkh;
	memset(&rkh, 0, sizeof(rkh_table_t));

	for (int i = 0; i < MAX_ROOT_CERT_COUNT; i++) {
		if (conf.rootCertFiles[i].size() == 0) {
			if (usedRootCertIndex == i)
			{
				Log::log(Logger::ERROR, "\tRoot certificate %d, which is part of selected certificate chain, cannot be empty.\n", i);
				cleanUpK3Sboot( imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
			else
			{
				continue;
			}
		}

		const char* rootCertFile = (conf.rootCertFiles[i]).c_str();
		FILE *rootCrtFile = fopen(rootCertFile, "rb");
		if (rootCrtFile == NULL)
		{
			Log::log(Logger::INFO2, "\tCannot open root certificate file %d (%s).\n", i, rootCertFile);
			cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		fclose(rootCrtFile);
		if (conf.mainCertId == i)
		{
			usedRootCertPath = rootCertFile;
		}
		mbedtls_x509_crt rootCrt;
		mbedtls_x509_crt_init(&rootCrt);

		ret = mbedtls_x509_crt_parse_file(&rootCrt, rootCertFile);
		if (ret != 0)
		{
			Log::log(Logger::ERROR, "\tFailed to parse root certificate file %d(%s)(error code = %d).\nExpected X.509 certificate in DER format.\n", i, rootCertFile, ret);
			cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}

		uint32_t flags;
		ret = mbedtls_x509_crt_verify(&rootCrt, &rootCrt, NULL, NULL, &flags, NULL, NULL);
		if (ret != 0)
		{
			Log::log(Logger::ERROR, "\tRoot certificate %d must be selfsigned.\n", i);
			cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}
		else
		{
			Log::log(Logger::INFO2, "\tRoot certificate %d is self signed.\n", i);
		}

		size_t n_size = mbedtls_mpi_size(&((mbedtls_rsa_context *)rootCrt.pk.pk_ctx)->N);
		size_t e_size = mbedtls_mpi_size(&((mbedtls_rsa_context *)rootCrt.pk.pk_ctx)->E);
		uint8_t *buf = (uint8_t *)calloc(1, n_size + e_size);
		mbedtls_mpi_write_binary(&((mbedtls_rsa_context *)rootCrt.pk.pk_ctx)->N, buf, n_size);
		mbedtls_mpi_write_binary(&((mbedtls_rsa_context *)rootCrt.pk.pk_ctx)->E, buf + n_size, e_size);
		mbedtls_sha256(buf, n_size + e_size, rkh.entries[i].rkh, false);

		mbedtls_x509_crt_free(&rootCrt);
		Log::log(Logger::INFO2, "\tSuccess. (Root Certificate %d = %s)\n", i, rootCertFile);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Calculate used root certificate size.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.4 Calculate size of root certificates.\n", step);
	usedRootCrtFile = fopen(usedRootCertPath.c_str(), "rb");
	int usedRootCrtSize = 0, usedRootCrtAlignedSize = 0;
	if (usedRootCrtFile == NULL)
	{
		Log::log(Logger::ERROR, "\tCannot open the selected root certificate file %s.\n", usedRootCertPath.c_str());
		cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}
	// Get image length in bytes.
	fseek(usedRootCrtFile, 0, SEEK_END);
	usedRootCrtSize = ftell(usedRootCrtFile);
	fseek(usedRootCrtFile, 0, SEEK_SET);

	if (usedRootCrtSize % AUTH_IMAGE_ALIGNMENT)
	{
		usedRootCrtAlignedSize = usedRootCrtSize + (AUTH_IMAGE_ALIGNMENT - (usedRootCrtSize % AUTH_IMAGE_ALIGNMENT));
	}
	else
	{
		usedRootCrtAlignedSize = usedRootCrtSize;
	}

	Log::log(Logger::INFO2, "\tSuccess. (Root Certificate Size = %lu bytes, Aligned Size = %d bytes)\n", usedRootCrtSize, usedRootCrtAlignedSize);
	step++;

	Log::log(Logger::INFO2, "\t Success.(Used Root Certificate Size = %d, Aligned Size = %d)\n", usedRootCrtSize,	usedRootCrtAlignedSize);
	step++;
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load all certificates.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d. Load all certificates in selected certificate chain.\n", step);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load the count of the certificates.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.1 Load the count of chained certificates in selected certificate chain.\n", step);
	int certCount = conf.chainCertFiles.size();
	if (certCount < 0)
	{
		Log::log(Logger::ERROR, "Invalid count of the certificates chained in selected certificate chain.\n");
		cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}
	Log::log(Logger::INFO2, "\tSuccess. (Certificate count = %d)\n", certCount);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load all certificates.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.2 Load and parse certificates in selected certificate chain.\n", step);
	trustCrt = (mbedtls_x509_crt *)malloc(sizeof(mbedtls_x509_crt));
	mbedtls_x509_crt_init(trustCrt);
	ret = mbedtls_x509_crt_parse_file(trustCrt, usedRootCertPath.c_str());
	if (ret != 0)
	{
		Log::log(Logger::ERROR, "\tFailed to parse selected root certificate file (%s)(error code = %d).\n", usedRootCertPath.c_str(), ret);
		cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}
	if (certCount > 0) {
		for (uint32_t i = 0; i < certCount; i++)
		{
			const char* chainCertFile = conf.chainCertFiles[i].c_str();
			FILE *crtFile = fopen(chainCertFile, "rb");
			if (crtFile == NULL)
			{
				Log::log(Logger::INFO2, "\tCannot open certificate file %d(%s)(error code = %d).\n", i, chainCertFile, ret);
				cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
			fclose(crtFile);

			mbedtls_x509_crt *testCrt = (mbedtls_x509_crt *)malloc(sizeof(mbedtls_x509_crt));
			mbedtls_x509_crt_init(testCrt);
			ret = mbedtls_x509_crt_parse_file(testCrt, chainCertFile);
			if (ret != 0)
			{
				Log::log(Logger::ERROR, "\tFailed to parse certificate file %d(%s)(error code = %d).\n", i, chainCertFile, ret);
				cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}

			uint32_t flags;
			ret = mbedtls_x509_crt_verify(testCrt, trustCrt, NULL, NULL, &flags, NULL, NULL);
			if (ret != 0)
			{
				Log::log(Logger::ERROR, "\tVerify certificat chain failed at index %d (%s)(error code = %d).\n", i, chainCertFile, ret);
				cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
				return FAIL_RETURN;
			}
			mbedtls_x509_crt_free(trustCrt);
			free(trustCrt);
			trustCrt = testCrt;
			testCrt = NULL;

			crtList.push_back(chainCertFile);
			Log::log(Logger::INFO2, "\tSuccess. ( Certificate %d = %s)\n", i, chainCertFile);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Calculate all the certificates size.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.3 Calculate size of all chained certificates.\n", step);
	size_t crtFileTotalSize = 0;

	size_t i = 0;
	for (vector<string>::iterator it = crtList.begin(); it != crtList.end(); it++, i++)
	{
		FILE *crtFile = fopen(it->c_str(), "rb");
		if (crtFile == NULL)
		{
			Log::log(Logger::ERROR, "\tCannot open the used certificate file %s.\n", it->c_str());
			cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
			return FAIL_RETURN;
		}

		fseek(crtFile, 0, SEEK_END);
		size_t crtFileSize = ftell(crtFile);
		fseek(crtFile, 0, SEEK_SET);
		fclose(crtFile);

		uint32_t crtFileAlignedSize;
		if (crtFileSize % AUTH_IMAGE_ALIGNMENT)
		{
			crtFileAlignedSize = crtFileSize + (AUTH_IMAGE_ALIGNMENT - (crtFileSize % AUTH_IMAGE_ALIGNMENT));
		}
		else
		{
			crtFileAlignedSize = crtFileSize;
		}

		crtFileTotalSize += crtFileAlignedSize;

		Log::log(Logger::INFO2, "\tSuccess. (Certificate %d size = %lu bytes, Aligned size = %d bytes)\n", i, crtFileSize, crtFileAlignedSize);
	}
	Log::log(Logger::INFO2, "\tSuccess. (Certificate total size = %lu bytes)\n", crtFileTotalSize);
	step++;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Generate the entire image data
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d. Generate the entire image data.\n", step);

	// Calculate CRT table size and align it to word boundary.
	size_t crtTableSize = usedRootCrtAlignedSize + crtFileTotalSize + 4 * (certCount + 1 /*root*/);
	// Align the crt table size to word boundary.

	size_t crtTableAlignedSize = 0;
	if (crtTableSize % AUTH_IMAGE_ALIGNMENT)
	{
		crtTableAlignedSize = crtTableSize + (AUTH_IMAGE_ALIGNMENT - (crtTableSize % AUTH_IMAGE_ALIGNMENT));
	}
	else
	{
		crtTableAlignedSize = crtTableSize;
	}

	// Calculate entire image size£¬including image£¬ header and crt.
	size_t totalImageSize =
		imageAlignedSize + sizeof(certificate_block_header_t) + crtTableAlignedSize + sizeof(rkh_table_t);

	// Malloc a buffer to contain the entire image data.
	buffer = (uint8_t *)malloc(totalImageSize);
	// Init the buffer. padding bytes for the aligned are 0x00s.
	memset(buffer, 0, totalImageSize);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Cp the original image data
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.1 Copy the original image data.\n", step);
	// Load image file to buffer.
	fread(buffer, 1, imageSize, imageFile);

	// Modify the header offsize.
	uint32_t headerOffset = imageAlignedSize;
	*(uint32_t *)(buffer + 0x28) = headerOffset;
	Log::log(Logger::INFO2, "\tSuccess.\n");
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Init certificate header
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.2 Init certificate header.\n", step);
	certificate_block_header_t *header = (certificate_block_header_t *)(buffer + imageAlignedSize);

	header->signature = SIGNATURE_TAG;
	header->headerMajorVersion = 1;
	header->headerMinorVersion = 0;
	header->headerLengthInBytes = sizeof(certificate_block_header_t);
	header->flags = 0;
	header->buildNumber = FOUR_CHAR_CODE(1, 0, 0, 0);
	header->totalImageLengthInBytes = totalImageSize;
	header->certificateCount = 1 /*root cert*/ + certCount;
	header->certificateTableLengthInBytes = crtTableAlignedSize;

	uint32_t crtTableOffset = imageAlignedSize + sizeof(certificate_block_header_t);
	uint32_t crtOffset = crtTableOffset;
	Log::log(Logger::INFO2, "\tSuccess.\n");
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Add root certificate
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.3 Add root certificate.\n", step);
	*(uint32_t *)(buffer + crtOffset) = usedRootCrtAlignedSize;
	crtOffset += 4;
	fread(buffer + crtOffset, 1, usedRootCrtSize, usedRootCrtFile);
	crtOffset += usedRootCrtAlignedSize;
	Log::log(Logger::INFO2, "\tSuccess.\n");
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Add certificate
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.4 Add certificate.\n", step);
	for (std::vector<std::string>::iterator it = crtList.begin(); it != crtList.end(); it++, i++)
	{
		FILE *crtFile = fopen(it->c_str(), "rb");
		if (crtFile == NULL)
		{
			Log::log(Logger::INFO2, "\tCannot open the used certificate file %s.\n", usedRootCertPath.c_str());
			return FAIL_RETURN;			
		}
		fseek(crtFile, 0, SEEK_END);
		size_t crtFileSize = ftell(crtFile);
		fseek(crtFile, 0, SEEK_SET);

		uint32_t crtFileAlignedSize;
		if (crtFileSize % AUTH_IMAGE_ALIGNMENT)
		{
			crtFileAlignedSize = crtFileSize + (AUTH_IMAGE_ALIGNMENT - (crtFileSize % AUTH_IMAGE_ALIGNMENT));
		}
		else
		{
			crtFileAlignedSize = crtFileSize;
		}

		*(uint32_t *)(buffer + crtOffset) = crtFileAlignedSize;
		crtOffset += 4;
		fread(buffer + crtOffset, 1, crtFileSize, crtFile);
		crtOffset += crtFileAlignedSize;
	}

	// crtOffset = crtOffset + (AUTH_IMAGE_ALIGNMENT - (crtOffset % AUTH_IMAGE_ALIGNMENT));

	if ((crtOffset - crtTableOffset) != crtTableSize)
	{
		Log::log(Logger::INFO2, "\tgenerate certificate table failed. table size is %d, but should be %d.\n", crtOffset - crtTableOffset,
			crtTableSize);
		return FAIL_RETURN;		
	}
	Log::log(Logger::INFO2, "\tSuccess.(Certificate table size = %d)\n", crtOffset - crtTableOffset);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Add root hash key table
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d.5 Add root hash key table.\n", step);
	uint32_t rkhOffset = crtOffset;
	memcpy(buffer + rkhOffset, &rkh, sizeof(rkh_table_t));
	Log::log(Logger::INFO2, "\tSuccess.\n");
	step++;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load and parse the private key.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d. Load and parse the private key of certificate used for signing of output image.\n", step);
	mbedtls_pk_context pk_ctx;
	mbedtls_pk_init(&pk_ctx);
	if ((ret = mbedtls_pk_parse_keyfile(&pk_ctx, conf.mainCertPrivateKeyFile.c_str(), NULL)) != 0)
	{
		Log::log(Logger::ERROR, "Parse private key failed.(%d)(%s)\n", ret, conf.mainCertPrivateKeyFile.c_str());
		cleanUpK3Sboot(imageFile, usedRootCrtFile, outputFile, buffer, signature, trustCrt, rsa_priv_key_ctx);
		return FAIL_RETURN;
	}
	// Get the private key.
	rsa_priv_key_ctx = (mbedtls_rsa_context *)(pk_ctx.pk_ctx);
	Log::log(Logger::INFO2, "\tSuccess. (Private Key File Path = %s\n", conf.mainCertPrivateKeyFile.c_str());
	uint32_t signatureSizeInBytes = rsa_priv_key_ctx->len;
	step++;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Generate the digest.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d. Generate ouput image digest.\n", step);
	// Buffer contains digest.
	uint8_t digest[32] = { 0 };

	// Call mbedtls to calculate digest.
	mbedtls_sha256_context sha256_ctx;
	mbedtls_sha256_init(&sha256_ctx);
	mbedtls_sha256_starts(&sha256_ctx, false /*isSHA224 = false*/);
	mbedtls_sha256_update(&sha256_ctx, buffer, totalImageSize);
	mbedtls_sha256_finish(&sha256_ctx, digest);

	Log::log(Logger::INFO2, "\tSuccess.\n");
	Log::log(Logger::INFO2, "\tSHA256 digest:");
	for (size_t i = 0; i < 32; i++)
	{
		Log::log(Logger::INFO2, " %#x", digest[i]);
	}
	Log::log(Logger::INFO2, "\n");
	step++;
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Generate the signature.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Log::log(Logger::INFO2, "%d. Generate output image signature.\n", step);
	// Buffer contains signature.
	signatureSizeInBytes = rsa_priv_key_ctx->len;
	signature = (uint8_t *)malloc(signatureSizeInBytes);
	memset(signature, 0, signatureSizeInBytes);

	// Init deterministic random bit generator.
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_ctr_drbg_init(&ctr_drbg);
	// Init entropy.
	mbedtls_entropy_context entropy;
	mbedtls_entropy_init(&entropy);

	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
	if (ret != 0)
	{
		Log::log(Logger::INFO2, "Seed random bit generator failed.%d\n", ret);
		return FAIL_RETURN;		
	}

	ret = mbedtls_rsa_rsassa_pkcs1_v15_sign(rsa_priv_key_ctx, mbedtls_ctr_drbg_random, &ctr_drbg, MBEDTLS_RSA_PRIVATE,
		MBEDTLS_MD_SHA256, 256, digest, signature);
	if (ret != 0)
	{
		Log::log(Logger::INFO2, "Generate signature failed.(%d)\n", ret);
		return FAIL_RETURN;		
	}

	Log::log(Logger::INFO2, "\tSuccess.\n");
	Log::log(Logger::INFO2, "\tSignature:");
	for (size_t i = 0; i < signatureSizeInBytes; i++)
	{
		Log::log(Logger::INFO2, " %#x", signature[i]);
	}
	Log::log(Logger::INFO2, "\n");
	step++;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Verify the signature.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Log::log(Logger::INFO2, "%d. Verify signature.\n", step);

	// ret = mbedtls_rsa_rsassa_pkcs1_v15_verify(rsa_pub_key_ctx, mbedtls_ctr_drbg_random, &ctr_drbg,
	// MBEDTLS_RSA_PUBLIC,
	//                                          MBEDTLS_MD_SHA256, 256, digest, signature);
	mbedtls_rsa_context *rsa_pub_key_ctx = (mbedtls_rsa_context *)(trustCrt->pk.pk_ctx);
	ret = mbedtls_rsa_rsassa_pkcs1_v15_verify(rsa_pub_key_ctx, NULL, NULL, MBEDTLS_RSA_PUBLIC, MBEDTLS_MD_SHA256, 256,
		digest, signature);
	if (ret != 0)
	{
		Log::log(Logger::INFO2, "Verify signature failed.(%d)\n", ret);
		return FAIL_RETURN;		
	}

	Log::log(Logger::INFO2, "\tSuccess.\n");
	step++;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Output the root key hash table SHA256 hash.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Log::log(Logger::INFO2, "%d. Output the root certificates SHA256 hash (RKTH).\n", step);

	uint8_t rkhtHash[32];
	mbedtls_sha256_context rkhtHash_sha256_ctx;
	mbedtls_sha256_init(&rkhtHash_sha256_ctx);
	mbedtls_sha256_starts(&rkhtHash_sha256_ctx, false /*isSHA224 = false*/);
	mbedtls_sha256_update(&rkhtHash_sha256_ctx, (const unsigned char *)(buffer + rkhOffset), sizeof(rkh_table_t));
	mbedtls_sha256_finish(&rkhtHash_sha256_ctx, (uint8_t *)rkhtHash);

	Log::log(Logger::INFO2, "\tSuccess.\n");
	Log::log(Logger::INFO, "\tRKTH: ");
	for (size_t i = 0; i < 32; i++)
	{
		Log::log(Logger::INFO, "%02x", rkhtHash[i]);
	}
	Log::log(Logger::INFO, "\n");
	step++;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Output the authenticated image.
	////////////////////////////////////////////////////////////////////////////////////////////////////

	Log::log(Logger::INFO2, "%d. Output the authenticated image.\n", step);

	outputFile = fopen(conf.masterBootOutputFile.c_str(), "wb");
	if (outputFile == NULL)
	{
		Log::log(Logger::INFO2, "\tCannot open/create output file %s.\n", conf.masterBootOutputFile.c_str());
		return FAIL_RETURN;		
	}

	ret = fwrite(buffer, 1, totalImageSize, outputFile);
	if (ret != totalImageSize)
	{
		Log::log(Logger::INFO2, "\tWrite total image data to output file failed..\n");
		return FAIL_RETURN;		
	}

	ret = fwrite(signature, 1, signatureSizeInBytes, outputFile);
	if (ret != signatureSizeInBytes)
	{
		Log::log(Logger::INFO2, "\tWrite signature to output file failed..\n");
		return FAIL_RETURN;		
	}
	Log::log(Logger::INFO, "\tSuccess. (Signed image %s created.)\n", conf.masterBootOutputFile.c_str());
	return SUCCESS_RETURN;
}

void AuthImageGenerator::cleanUpK3Sboot(FILE * imageFile, FILE * usedRootCrtFile, FILE * outputFile, uint8_t * buffer, uint8_t * signature, mbedtls_x509_crt * trustCrt, mbedtls_rsa_context * rsa_priv_key_ctx)
{
	if (imageFile != NULL)
	{
		fclose(imageFile);
	}
	if (usedRootCrtFile != NULL)
	{
		fclose(usedRootCrtFile);
	}
	if (outputFile != NULL)
	{
		fclose(outputFile);
	}
	if (buffer != NULL)
	{
		free(buffer);
	}
	if (signature != NULL)
	{
		free(signature);
	}
	if (trustCrt != NULL)
	{
		free(trustCrt);
	}
	if (rsa_priv_key_ctx != NULL)
	{
		free(rsa_priv_key_ctx);
	}
}

//! Process the JSON file to extract the operations that is to be performed
//!
//! \param[in]     confFilePath Path to the JSON configuration file to be processed.
//! \param[out]    conf Extracted Configuration information
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
bool AuthImageGenerator::parseConfigurationLPC54X0XXSboot(const string &confFilePath, configuration &conf)
{
	string tmp;
	auto error = false;
	stringstream ss;
	ss << "1. Parsing configuration file: " << confFilePath << " for image creation parameters.\n";
	Log::log(Logger::INFO, ss.str());
	ss.str("");
	jute::jValue jsonConf;

	try {
		jsonConf = jute::parser::parse_file(confFilePath);
	}
	catch (runtime_error &e) {
		throw runtime_error(("Cannot parse json configuration file: " + confFilePath + " - " + e.what()).c_str());
	}

	if (jsonConf["family"].get_type() == jute::JSTRING) {
		conf.family = jsonConf["family"].as_string();
		transform(conf.family.begin(), conf.family.end(), conf.family.begin(), ::tolower);
		if (conf.family != LPC54X0XX) {
			ss << "\tERROR! Unexpected \"family\" value of \"" << conf.family << "\" encountered. ";
			ss << "The following value(s) are the only values expected - \"" << LPC54X0XX << "\"." << endl;
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
	}
	else {
		ss << "\tERROR! Cannot read \"family\" from configuration file.\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (jsonConf["inputImageFile"].get_type() == jute::JSTRING)
	{
		conf.imageFile = jsonConf["inputImageFile"].as_string();
		ss << "\tRead \"inputImageFile\" of \"" << conf.imageFile << "\".\n";
		Log::log(Logger::INFO2, ss.str());
		ss.str("");
	}
	else {
		ss << "\tCannot read \"inputImageFile\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}


	if (jsonConf["outputImageExecutionTarget"].get_type() == jute::JSTRING) {
		string backup = tmp = jsonConf["outputImageExecutionTarget"].as_string();
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		tmp.erase(remove_if(tmp.begin(), tmp.end(), [](char ch) { return isspace(static_cast<unsigned char>(ch)); }), tmp.end());
		if (tmp != "externalflash(xip)" && tmp != "ram") {
			ss << "\tUnsupported value (" << backup << ") of \"outputImageExecutionTarget\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected values [\"External flash (XIP)\", \"RAM\"]  for " << conf.family << " family.\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
		if (tmp == "externalflash(xip)")
			tmp = "xip";
		conf.imageType = tmp;
	}
	else {
		ss << "\tCannot read \"outputImageExecutionTarget\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (jsonConf["outputImageAuthenticationType"].get_type() == jute::JSTRING) {
		string backup = tmp = jsonConf["outputImageAuthenticationType"].as_string();
		transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
		tmp.erase(remove_if(tmp.begin(), tmp.end(), [](char ch) { return isspace(static_cast<unsigned char>(ch)); }), tmp.end());
		if (conf.imageType == "xip" && !(tmp == "crc" || tmp == "none")) {
			ss << "\tUnsupported value (" << tmp << ") of \"outputImageAuthenticationType\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected values [\"CRC\", \"none\"] for " << conf.family << " family with \"outputImageExecutionTarget\" set to \"External flash(XIP)\".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
		else if (conf.imageType == "ram" && !(tmp == "crc" || tmp == "none" || tmp == "encrypted" || tmp == "signed" || tmp == "encrypted+signed" || tmp == "signed+encrypted")) {
			ss << "\tUnsupported value (" << tmp << ") of \"outputImageAuthenticationType\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
			ss << "\tExpected values [\"Encrypted\", \"Signed\", \"Encrypted + Signed\" , \"Signed + Encrypted\"] for " << conf.family << " family with \"outputImageExecutionTarget\" set to \"RAM\".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
		conf.imageType += "+" + tmp;
		ss << "\tSetting \"imageType\" to \"" << conf.imageType << "\".\n";
		Log::log(Logger::INFO2, ss.str());
		ss.str("");
	}
	else {
		ss << "\tCannot read \"outputImageAuthenticationType\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (jsonConf["DebugCompatability"].get_type() == jute::JBOOLEAN)
	{
		conf.DebugCompatability = jsonConf["DebugCompatability"].as_bool();
		ss << "\tRead \"DebugCompatability\" of \"" << conf.DebugCompatability << "\".\n";
		Log::log(Logger::INFO, ss.str());
		ss.str("");
	}
	else
		conf.DebugCompatability = false;

	if (jsonConf["BypassCertificateChecks"].get_type() == jute::JBOOLEAN)
	{
		conf.BypassCertificateChecks = jsonConf["BypassCertificateChecks"].as_bool();
		ss << "\tRead \"BypassCertificateChecks\" of \"" << conf.BypassCertificateChecks << "\".\n";
		Log::log(Logger::INFO, ss.str());
		ss.str("");
	}
	else
		conf.BypassCertificateChecks = false;

	if (conf.imageType.find("signed") != -1) {

		if (jsonConf["preformattedSignature"].get_type() == jute::JBOOLEAN)
		{
			conf.preformattedSignature = jsonConf["preformattedSignature"].as_bool();
			ss << "\tRead \"preformattedSignature\" of \"" << conf.preformattedSignature << "\".\n";
			Log::log(Logger::INFO2, ss.str());
			ss.str("");
		}
		else {
			conf.preformattedSignature = false;
			ss << "\tCannot read \"preformattedSignature\" from configuration file: " << confFilePath << ".\n\tPreformatted Signature disabled by default.\n";
			Log::log(Logger::INFO, ss.str());
			ss.str("");
		}
	}

	if (conf.imageType.find("encrypted") != -1) {

		if (jsonConf["outputImageEncryptionKeyFile"].get_type() == jute::JSTRING)
		{
			conf.imageEncryptionKeyFile = jsonConf["outputImageEncryptionKeyFile"].as_string();
			ss << "\tUsing \"imageEncryptionKeyFile\" of \"" << conf.imageEncryptionKeyFile << "\".\n";
			Log::log(Logger::INFO2, ss.str());
			ss.str("");
		}
		else {
			ss << "\tCannot read \"outputImageEncryptionKeyFile\" from configuration file: " << confFilePath << ".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;

		}
		if (jsonConf["useKeyStore"].get_type() == jute::JBOOLEAN)
		{
			conf.useKeyStore = jsonConf["useKeyStore"].as_bool();
			ss << "\tRead \"useKeyStore\" of \"" << conf.useKeyStore << "\".\n";
			Log::log(Logger::INFO2, ss.str());
			ss.str("");
		}

		else {
			ss << "\tCannot read \"useKeyStore\" from configuration file: " << confFilePath << ".\n\tKey store disable by default.\n";
			Log::log(Logger::INFO, ss.str());
			ss.str("");
		}

		if (jsonConf["deviceKeySource"].get_type() == jute::JSTRING) {
			conf.deviceKeySource = jsonConf["deviceKeySource"].as_string();
			if (conf.deviceKeySource != "OTP" && conf.deviceKeySource != "Key Store") {
				ss << "\tUnsupported value (" << conf.deviceKeySource << ") of \"deviceKeySource\" for selected family " << conf.family << " from configuration file: " << confFilePath << ".\n";
				ss << "\tExpected values are [\"OTP\", \"Key Store\"] for " << conf.family << " family.\n";
				Log::log(Logger::ERROR, ss.str());
				ss.str("");
				error = true;
			}
		}
		else {
			ss << "\tCannot read \"deviceKeySource\" from configuration file: " << confFilePath << ".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}

		if (conf.useKeyStore) {
			if (jsonConf["keyStoreFile"].get_type() == jute::JSTRING)
			{
				conf.keyStoreFile = jsonConf["keyStoreFile"].as_string();
				ss << "\tRead \"keyStoreFile\" of \"" << conf.keyStoreFile << "\".\n";
				Log::log(Logger::INFO2, ss.str());
				ss.str("");
			}
			else {
				ss << "\tCannot read \"keyStoreFile\" from configuration file: " << confFilePath << ".\n\tKey store will be only reserved.\n";
				Log::log(Logger::INFO, ss.str());
				ss.str("");
			}
		}
	}
	if (conf.imageType.find("signed") != -1) {

		/* This is optionally needed if we are signing the image */
		if (jsonConf["rootOfTrustKeyFile"].get_type() == jute::JSTRING)
		{
			conf.rootOfTrustKeyFile = jsonConf["rootOfTrustKeyFile"].as_string();
			ss << "\tRead \"rootOfTrustKeyFile\" of \"" << conf.rootOfTrustKeyFile << "\".\n";
			Log::log(Logger::INFO2, ss.str());
			ss.str("");
		}
		else
			conf.rootOfTrustKeyFile = "";

		if (jsonConf["rootOfTrustKeyFilePassword"].get_type() == jute::JSTRING)
		{
			conf.rootOfTrustKeyFilePassword = jsonConf["rootOfTrustKeyFilePassword"].as_string();
		}
		else
			conf.rootOfTrustKeyFilePassword = "";

		/* These ARE needed if we are signing the image */
		if (jsonConf["imageKeyCertificate"].get_type() == jute::JSTRING)
		{
			conf.imageKeyCertificate = jsonConf["imageKeyCertificate"].as_string();
			ss << "\tRead \"imageKeyCertificate\" of \"" << conf.imageKeyCertificate << "\".\n";
			Log::log(Logger::INFO2, ss.str());
			ss.str("");
		}
		else
		{
			ss << "\tCannot read \"imageKeyCertificate\" from configuration file: " << confFilePath << ". This MUST be provided for signed configurations..\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}

		if (jsonConf["privateImageKey"].get_type() == jute::JSTRING)
		{
			conf.privateImageKey = jsonConf["privateImageKey"].as_string();
			ss << "\tRead \"privateImageKey\" of \"" << conf.privateImageKey << "\".\n";
			Log::log(Logger::INFO2, ss.str());
			ss.str("");
		}
		else
		{
			ss << "\tCannot read \"privateImageKey\" from configuration file: " << confFilePath << ". This MUST be provided for signed configurations.\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}

		if (jsonConf["privateImageKeyPassword"].get_type() == jute::JSTRING)
		{
			conf.privateImageKeyPassword = jsonConf["privateImageKeyPassword"].as_string();
		}
		else
		{
			conf.privateImageKeyPassword = "";
		}
	}

	if (jsonConf["imageLinkAdressFromImage"].get_type() == jute::JBOOLEAN)
		conf.imageLinkAddressFromImage = jsonConf["imageLinkAdressFromImage"].as_bool();
	else {
		ss << "\tCannot read \"imageLinkAdressFromImage\" from configuration file: " << confFilePath << ".\n\tDisabled by default and image load address fetched from \"imageLinkAddress\".\n";
		Log::log(Logger::INFO, ss.str());
		ss.str("");
		conf.imageLinkAddressFromImage = false;
	}

	if (!conf.imageLinkAddressFromImage) {
		if (jsonConf["imageLinkAddress"].get_type() != jute::JUNKNOWN) {
			string tmp(jsonConf["imageLinkAddress"].as_string());
			try {
				conf.imageLinkAddress = stoul(tmp, nullptr, 16);
				// Only certain values are allowed
				if ((conf.imageLinkAddress != 0x00000000) && (conf.imageLinkAddress != 0x20000000))
				{
					ss << "\tERROR! Value for \"imageLinkAddress\" of " << jsonConf["imageLinkAddress"].as_string() << " is not allowed. Use 0x00000000 or 0x20000000 only.\n";
					Log::log(Logger::ERROR, ss.str());
					ss.str("");
					error = true;
				}
				else
				{
					ss << "\tUsing a fixed \"imageLinkAddress\" value of 0x" << std::hex << conf.imageLinkAddress << "." << endl;
					Log::log(Logger::INFO2, ss.str());
					ss.str("");
				}
			}
			catch (invalid_argument) {
				ss << "\tCannot parse \"imageLinkAddress\" value: " << jsonConf["imageLinkAddress"].as_string() << " from configuration file: " << confFilePath << ".\n";
				ss << "\tExpected hexadecimal string in format \"0xHEXVALUE\" or only \"HEXVALUE\".\n";
				Log::log(Logger::ERROR, ss.str());
				ss.str("");
				error = true;

			}
			catch (out_of_range) {
				ss << "\tValue of \"imageLinkAddress\" is too big: " << jsonConf["imageLinkAddress"].as_string() << " from configuration file: " << confFilePath << ".\n";
				ss << "\tUse max value as 0xffffffff.\n";
				Log::log(Logger::ERROR, ss.str());
				ss.str("");
				error = true;
			}
		}
		else {
			ss << "\tCannot read \"imageLinkAddress\" from configuration file: " << confFilePath << ".\n";
			Log::log(Logger::ERROR, ss.str());
			ss.str("");
			error = true;
		}
	}
	else
	{
		ss << "\tUsing \"imageLinkAddress\" from input image." << endl;
		Log::log(Logger::INFO2, ss.str());
		ss.str("");
	}

	if (jsonConf["masterBootOutputFile"].get_type() == jute::JSTRING)
	{
		conf.masterBootOutputFile = jsonConf["masterBootOutputFile"].as_string();
		ss << "\tRead \"masterBootOutputFile\" of \"" << conf.masterBootOutputFile << "\".\n";
		Log::log(Logger::INFO2, ss.str());
		ss.str("");
	}

	else {
		ss << "\tCannot read \"masterBootOutputFile\" from configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
		error = true;
	}

	if (error)
		Log::log(Logger::WARNING, "\tParsed with error(s).\n");
	else
		Log::log(Logger::INFO, "\tSuccess.\n");

	return !error;
}

//! If the existing file is present, get the IV vectors from it.
//! Debug functionality for encryption test purposes only, to 
//! generate expected content across multiple runs
//! \param[in] pImageFile Image file to examine.
//! \param[in] pImageHeader Encrypted image header to update with previous IVs (if found)
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::TakeIVFromExistingFile(const char * pImageFile, ImageHeaderEncrypted *pImageHeader)
{
	FILE *imageFile = NULL;
	size_t ImageSize;
	uint8_t *pImage = NULL;
	ImageHeaderEncrypted *pExistingImageHeader = NULL;

	memset(pImageHeader->image_iv, 0x55, sizeof(pImageHeader->image_iv));
	memset(pImageHeader->header_iv, 0xAA, sizeof(pImageHeader->header_iv));

	imageFile = fopen(pImageFile, "rb");
	if (imageFile == NULL)
	{
		return;
	}

	fseek(imageFile, 0, SEEK_END);
	ImageSize = ftell(imageFile);
	fseek(imageFile, 0, SEEK_SET);

	pImage = (uint8_t *)malloc(ImageSize);

	ImageVectorTable * pFileVectorTable = NULL;

	fread(pImage, 1, ImageSize, imageFile);
	fclose(imageFile);
	pFileVectorTable = (ImageVectorTable *)pImage;

	if (ImageSize >= sizeof(ImageVectorTable))
	{
		if (pFileVectorTable->image_marker == IMG_MARKER)
		{
			if ((pFileVectorTable->header_offset < ImageSize) && ((ImageSize - pFileVectorTable->header_offset) >= sizeof(ImageHeaderEncrypted)))
			{
				pExistingImageHeader = (ImageHeaderEncrypted *)(pImage + pFileVectorTable->header_offset);

				if (pExistingImageHeader->header_marker == KS_HEADER_MARKER)
				{
					/* A key store is present, the image header itself is elsewhere. */
					pExistingImageHeader = NULL;
					if (((ImageSize - pFileVectorTable->header_offset) >= sizeof(ImageHeaderKeyStore)))
					{
						ImageHeaderKeyStore  * pKeyStoreHeader = (ImageHeaderKeyStore  *)(pImage + pFileVectorTable->header_offset);
						if ((pKeyStoreHeader->img_offset < ImageSize) && ((ImageSize - pKeyStoreHeader->img_offset) >= sizeof(ImageHeaderEncrypted)))
						{
							pExistingImageHeader = (ImageHeaderEncrypted *)(pImage + pKeyStoreHeader->img_offset);
						}
					}
				}

				if (pExistingImageHeader && pExistingImageHeader->header_marker == IMG_HEADER_MARKER)
				{
					if (pExistingImageHeader->img_type.encrypted)
					{
						memcpy(pImageHeader->image_iv, pExistingImageHeader->image_iv, sizeof(pImageHeader->image_iv));
						memcpy(pImageHeader->header_iv, pExistingImageHeader->header_iv, sizeof(pImageHeader->header_iv));
					}
				}
			}
		}
	}
	free(pImage);
}

//! Helper class for managing the image buffer.
LPC54X0XXImageHelper::LPC54X0XXImageHelper()
{
	this->pImageBuffer = NULL;
	this->ImageBufferHeaderOffset = 0;
	this->NoSignInitialBlock = 0;
	this->ImageSize = 0;
	this->plainImageSize = 0;

	/* Pre-compute the CRC look up table */
	uint32_t polynomial = 0xEDB88320;
	for (uint32_t i = 0; i < sizeof(this->crc_lut_table) / sizeof(this->crc_lut_table[0]); i++)
	{
		uint32_t c = i;
		for (size_t j = 0; j < 8; j++)
		{
			if (c & 1) {
				c = polynomial ^ (c >> 1);
			}
			else {
				c >>= 1;
			}
		}
		this->crc_lut_table[i] = c;
	}
}

//! Helper class for managing the image buffer.
LPC54X0XXImageHelper::~LPC54X0XXImageHelper()
{
	if (pImageBuffer != NULL)
	{
		free(pImageBuffer);
	}

}

//! Open the image file ad read it into the image buffer
//!
//! \param[in] step Step number for logging purposes.
//! \param[in] conf Configuration information
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::OpenImage(int step, const AuthImageGenerator::configuration &conf)
{
	FILE *imageFile = NULL;

	stringstream ss;
	ImageVectorTable * pFileVectorTable = NULL;
	ImageHeaderCommon *		pImageHeader;
	int substep = 1;

	try
	{
		Log::log(Logger::INFO2, "\t%d.%d Open the image binary file.(%s)\n", step, substep++, conf.imageFile.c_str());
		imageFile = fopen(conf.imageFile.c_str(), "rb");
		if (imageFile == NULL)
		{
			ss << "\tCannot open image file '" << conf.imageFile.c_str() << "' Reason: '" << std::strerror(errno) << ".";
			throw std::runtime_error(ss.str());
		}
		// Allocate a buffer to hold the entire contents of the file
		fseek(imageFile, 0, SEEK_END);
		ImageSize = ftell(imageFile);
		fseek(imageFile, 0, SEEK_SET);
		pImageBuffer = (uint8_t *)malloc(ImageSize);
		this->plainImageSize = ImageSize;
		if (pImageBuffer == NULL)
		{
			Log::log(Logger::ERROR, "%d. Unable to allocate %d bytes for image buffer\n", ImageSize);
			throw std::runtime_error("Memory Allocation Error.");
		}

		// Load image file to buffer.
		Log::log(Logger::INFO2, "\t%d.%d Copy the original image data.\n", step, substep++);
		if (fread(pImageBuffer, 1, ImageSize, imageFile) != ImageSize)
		{
			throw std::runtime_error("File Read Error.");
		}
		pFileVectorTable = (ImageVectorTable *)pImageBuffer;
		if (ImageSize < sizeof(ImageVectorTable))
		{
			ss << "\tImage file '" << conf.imageFile.c_str() << "' does not have sufficient content size to contain the image marker.\n";
			throw std::runtime_error(ss.str());
		}
		if (pFileVectorTable->image_marker != IMG_MARKER)
		{
			ss << "\tImage file '" << conf.imageFile.c_str() << "' does not have a valid image marker : Expected 0x" << std::hex << IMG_MARKER << " but read 0x" << std::hex << pFileVectorTable->image_marker << ".\n";
			throw std::runtime_error(ss.str());
		}

		ImageBufferHeaderOffset = 0;
		NoSignInitialBlock = 0;

		// If this file has a key store then we may have to adjust things. If this is already encrypted, and has a Key store then all we can
		// be reasonably expected to do is sign it. 
		if ((pFileVectorTable->vector0 == KS_HEADER_MARKER) && (pFileVectorTable->vector1 == KS_HEADER_IMGTYPE))
		{
			ImageHeaderKeyStore * pImageHeaderKS = (ImageHeaderKeyStore *)(pImageBuffer);
			Log::log(Logger::INFO2, "\t%d.%d Dealing with presence of KEYSTORE in input file.\n", step, substep++);
			pFileVectorTable = (ImageVectorTable *)(pImageBuffer + pImageHeaderKS->img_offset);
			ImageBufferHeaderOffset += pImageHeaderKS->img_offset;
			this->NoSignInitialBlock = pImageHeaderKS->img_offset; /* This area is NOT signed */
		}

		ImageBufferHeaderOffset += pFileVectorTable->header_offset;

		if ((ImageBufferHeaderOffset >= ImageSize) || ((ImageSize - ImageBufferHeaderOffset) < MIN_IMAGE_HEADER_SIZE) || (ImageBufferHeaderOffset & 0x3))
		{
			ss << "\tImage file '" << conf.imageFile.c_str() << "' header offset is invalid.\n";
			throw std::runtime_error(ss.str());
		}

		pImageHeader = (ImageHeaderCommon *)(pImageBuffer + ImageBufferHeaderOffset);

		Log::log(Logger::DEBUG, "%d.1 The original file image header is located at file offset 0x%08X.\n", step, pFileVectorTable->header_offset);

		if (pImageHeader->Common.header_marker != IMG_HEADER_MARKER)
		{
			ss << "\tImage file '" << conf.imageFile.c_str() << "' does not have a valid header marker : Expected 0x" << std::hex << IMG_HEADER_MARKER << " but read 0x" << std::hex << pImageHeader->NonSecure.header_marker << ".\n";
			throw std::runtime_error(ss.str());
		}

		if (pImageHeader->Common.img_type.encrypted)
		{
			if ((ImageSize - ImageBufferHeaderOffset) < MAX_IMAGE_HEADER_SIZE)
			{
				ss << "\tImage file '" << conf.imageFile.c_str() << "' header offset is invalid.\n";
				throw std::runtime_error(ss.str());
			}
		}

		if (pImageHeader->Common.img_type.encrypted)
		{
			if (pImageHeader->Encrypted.image_marker != IMG_MARKER)
			{
				ss << "\tImage file '" << conf.imageFile.c_str() << "' does not have a valid image marker : Expected 0x" << std::hex << IMG_MARKER << " but read 0x" << std::hex << pImageHeader->Encrypted.image_marker << ".\n";
				throw std::runtime_error(ss.str());
			}
		}

	}
	catch (const std::runtime_error)
	{
		if (imageFile)
		{
			fclose(imageFile);
		}
		throw;
	}

	fclose(imageFile);
}

//! Test for presense of file, and raise an informative exception if it is not there
//!
//! \param[in] File Path to file to check
//! \param[in] FileDesc Informational message , if exception is thrown
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::maybe_raise_fileaccess_fault(const char * File, const char * FileDesc)
{
	FILE *iFile = NULL;

	iFile = fopen(File, "rb");
	if (iFile == NULL)
	{
		stringstream ss;
		ss << "\tCannot access " << FileDesc << " '" << File << "' Reason: '" << std::strerror(errno) << "'.";
		throw std::runtime_error(ss.str());
	}
	fclose(iFile);

}

//! Test for mbedtls error, and raise an informative exception if one exists
//!
//! \param[in] error Return value from an mbedtls api call
//! \param[in] Message Informational message , if exception is thrown
//! \param[in] mbedtlsfunc mbedtls function that returned "error"
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::maybe_raise_mbedtls_fault(int error, const char * Message, const char * mbedtlsfunc)
{
	if (error != 0x0)
	{
		char mbed_tls_error_string[MAX_TLS_ERROR_STRING_SUPPORTED];
		stringstream ss;
		mbedtls_strerror(error, mbed_tls_error_string, sizeof(mbed_tls_error_string));
		ss << mbedtlsfunc << ". " << Message << " Reason: " << mbed_tls_error_string << ".\n";
		throw std::runtime_error(ss.str());
	}
}

//! Insert PUF datastore\header to the content that is currently in the image buffer
//! Test for mbedtls error, and raise an informative exception if one exists
//!
//! \param[in] error Return value from an mbedtls api call
//! \param[in] Message Informational message , if exception is thrown
//! \param[in] mbedtlsfunc mbedtls function that returned "error"
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::maybe_raise_mbedtls_fault(int error, string Message, const char * mbedtlsfunc)
{
	if (error != 0x0)
	{
		char mbed_tls_error_string[MAX_TLS_ERROR_STRING_SUPPORTED];
		stringstream ss;
		mbedtls_strerror(error, mbed_tls_error_string, sizeof(mbed_tls_error_string));
		ss << mbedtlsfunc << ". " << Message << " Reason: " << mbed_tls_error_string << ".\n";
		throw std::runtime_error(ss.str());
	}
}

//! Generate random image and header IVs, and random padding for encryption
//!
//! \param[in] step Step number for logging purposes.
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::GenerateRandomEncryptionFields(int step)
{
	int ret = 0;
	ImageHeaderEncrypted *				pImageHeader = (ImageHeaderEncrypted *)(this->pImageBuffer + this->ImageBufferHeaderOffset);;
	mbedtls_ctr_drbg_context	ctr_drbg;
	mbedtls_entropy_context		entropy;

	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctr_drbg);

	try
	{
		Log::log(Logger::DEBUG, "%d. Generating random AES IVs.\n", step);

		ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
		maybe_raise_mbedtls_fault(ret, "Cannot generate random init vector for AES GCM encryption", "mbedtls_ctr_drbg_seed");

		ret = mbedtls_ctr_drbg_random(&ctr_drbg, pImageHeader->image_iv, sizeof(pImageHeader->image_iv));
		maybe_raise_mbedtls_fault(ret, "Cannot generate random init vector for AES GCM encryption", "mbedtls_ctr_drbg_random");

		ret = mbedtls_ctr_drbg_random(&ctr_drbg, pImageHeader->header_iv, sizeof(pImageHeader->header_iv));
		maybe_raise_mbedtls_fault(ret, "Cannot generate random init vector for AES GCM encryption", "mbedtls_ctr_drbg_random");

		ret = mbedtls_ctr_drbg_random(&ctr_drbg, pImageHeader->padding, sizeof(pImageHeader->padding));
		maybe_raise_mbedtls_fault(ret, "Cannot generate random init vector for AES GCM encryption", "mbedtls_ctr_drbg_random");
	}
	catch (const std::runtime_error)
	{
		mbedtls_ctr_drbg_free(&ctr_drbg);
		mbedtls_entropy_free(&entropy);
		throw;
	}

	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
}

//! Read an AES128 or AES256 hex string from a file.
//!
//! \param[in] step Step number for logging purposes.
//! \param[in] pKeyFile Path to file containing AES128/AES256 hex digitals
//! \param[out] AesKey 32 bytes of storage which will be filled with extracted key.
//! \param[out] AesKeySize 16 : AES128    32 : AES256
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::GetAesFileFromFile(int step, const char * pKeyFile, uint8_t * AesKey, uint32_t &AesKeySize)
{
	stringstream ss;

	ifstream kekFile(pKeyFile, ios_base::in);

	if (kekFile.fail())
	{
		ss << "\tCannot open ImageEncryptionKey file '" << pKeyFile << ".\n";
		throw std::runtime_error(ss.str());
	}
	kekFile.seekg(0, ios::end);
	AesKeySize = (size_t)kekFile.tellg() != 0 ? kekFile.tellg() / 2 : 0;
	kekFile.seekg(0, ios::beg);

	if ((AesKeySize != 16) && (AesKeySize != 32)) {
		ss << "ImageEncryptionKey have unexpected size (16/32):" << AesKeySize << ".\n";
		throw std::runtime_error(ss.str());
	}

	memset(AesKey,0x0,32);
	try {

		if (AesKeySize == 16) {
			AESKey<128> aes(kekFile);
			memcpy(AesKey, (uint8_t*)aes.getKey(), AesKeySize);
		}
		else if (AesKeySize == 32) {
			AESKey<256> aes(kekFile);
			memcpy(AesKey, (uint8_t*)aes.getKey(), AesKeySize);
		}
		else {
			ss << "ImageEncryptionKey have unexpected size (16/32):" << AesKeySize << ".\n";
			throw std::runtime_error(ss.str());
		}
		kekFile.close();
	}
	catch (exception &e) {
		kekFile.close();
		ss << "Cannot parse ImageEncryptionKey key: :" << e.what() << ".\n";
		throw std::runtime_error(ss.str());
	}

}

//! Encrypt the content that is currently in the image buffer
//!
//! \param[in] step Step number for logging purposes.
//! \param[in] conf Configuration information
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::EncryptImage(int step, const AuthImageGenerator::configuration &conf)
{
	uint32_t					AesKeySize = AES128_KEY_BYTES;
	int ret = 0;
	uint8_t						AesKey[MAX_KEY_BYTES];
	stringstream ss;
	ImageHeaderCommon *				pImageHeader = NULL;
	mbedtls_gcm_context			gcm;
	int substep = 1;

	mbedtls_gcm_init(&gcm);

	try
	{
		if ((this->pImageBuffer == NULL) || (this->ImageSize == 0x0))
		{
			throw std::runtime_error("This image is empty and cannot be encrypted.");
		}

		pImageHeader = (ImageHeaderCommon *)(this->pImageBuffer + this->ImageBufferHeaderOffset);
		if (pImageHeader->Common.img_type.encrypted)
		{
			throw std::runtime_error("This image has been encrypted already.");
		}
		if (this->ImageSize == 0x0)
		{
			throw std::runtime_error("This image is empty and cannot be encrypted.");
		}

		this->ImageSize += sizeof(ImageHeaderEncrypted);

		this->pImageBuffer = (uint8_t *)realloc(this->pImageBuffer, this->ImageSize);
		if (this->pImageBuffer == NULL)
		{
			throw std::runtime_error("Failed to re-allocate memory for image buffer before performing encryption.");
		}
		(void)memmove(this->pImageBuffer + sizeof(ImageHeaderEncrypted), this->pImageBuffer, this->ImageSize - sizeof(ImageHeaderEncrypted));
		pImageHeader = (ImageHeaderCommon *)this->pImageBuffer;

		memcpy(this->pImageBuffer, this->pImageBuffer + sizeof(ImageHeaderEncrypted) + this->ImageBufferHeaderOffset, MIN_IMAGE_HEADER_SIZE);
		this->ImageBufferHeaderOffset = 0;

		pImageHeader->Encrypted.img_type.encrypted = true;
		pImageHeader->Encrypted.img_type.secure_boot = true;
		pImageHeader->Encrypted.img_type.nocrc = false;
		pImageHeader->Encrypted.img_type.xip = false;
		pImageHeader->Encrypted.image_marker = IMG_MARKER;
		pImageHeader->Encrypted.img_len = ImageSize - sizeof(ImageHeaderEncrypted);
		pImageHeader->Encrypted.header_offset = 0x0;
		if (!conf.imageLinkAddressFromImage)
		{
			pImageHeader->Encrypted.load_addr = conf.imageLinkAddress;
		}
		else
		{
			if ((pImageHeader->Encrypted.load_addr != 0x00000000) && (pImageHeader->Encrypted.load_addr != 0x20000000))
			{
				ss << "\tImage file '" << conf.imageFile.c_str() << "' does not have a valid image load address : Expected 0x00000000 or 0x20000000 but read 0x" << std::hex << pImageHeader->Encrypted.load_addr << ".\n";
				throw std::runtime_error(ss.str());
			}
		}

		GenerateRandomEncryptionFields(step);

		if (conf.DebugCompatability)
		{
			Log::log(Logger::DEBUG, "%d. Replacing random AES IVs with fixed ones.\n", step);
			TakeIVFromExistingFile(conf.masterBootOutputFile.c_str(), &pImageHeader->Encrypted);

			memset(pImageHeader->Encrypted.padding, 0x0, sizeof(pImageHeader->Encrypted.padding));
		}
		Log::log(Logger::INFO2, "\t%d.%d Load the image encryption key.\n", step, substep++);
		GetAesFileFromFile(step, conf.imageEncryptionKeyFile.c_str(), AesKey, AesKeySize);

		if ((conf.deviceKeySource == "Key Store") && (AesKeySize != 32))
		{
			ss << "\tImage encryption key file '" << conf.imageEncryptionKeyFile.c_str() << "' needs to be AES256 for PUF encryption.\n";
			throw std::runtime_error(ss.str());
		}
		else
			if ((conf.deviceKeySource == "OTP") && (AesKeySize != 16))
			{
				ss << "\tImage encryption key file '" << conf.imageEncryptionKeyFile.c_str() << "' needs to be AES128 for OTP encryption.\n";
				throw std::runtime_error(ss.str());
			}


		Log::log(Logger::INFO2, "\t%d.%d Perform image encryption.\n", step, substep++);
		ret = mbedtls_gcm_setkey(&gcm,
			MBEDTLS_CIPHER_ID_AES,
			AesKey,
			AesKeySize * 8);
		maybe_raise_mbedtls_fault(ret, "mbedtls_gcm_setkey failed to set the key for AES cipher", "mbedtls_gcm_setkey");

		/*
		Encrypt the input file using image_iv and the key...no additional data.
		For encryption, the output buffer can be the same as the input buffer.
		*/
		ret = mbedtls_gcm_crypt_and_tag(&gcm,
			MBEDTLS_GCM_ENCRYPT,
			ImageSize - sizeof(ImageHeaderEncrypted),
			pImageHeader->Encrypted.image_iv,
			IV_BYTES,
			NULL,
			0x0,
			pImageBuffer + sizeof(ImageHeaderEncrypted),
			pImageBuffer + sizeof(ImageHeaderEncrypted),
			TAG_BYTES,
			pImageHeader->Encrypted.image_tag);
		maybe_raise_mbedtls_fault(ret, "mbedtls_gcm_crypt_and_tag failed to encrypt the data", "mbedtls_gcm_crypt_and_tag");

		/* Authenticate the header*/
		ret = mbedtls_gcm_crypt_and_tag(&gcm,
			MBEDTLS_GCM_ENCRYPT,
			0x0,
			pImageHeader->Encrypted.header_iv,
			IV_BYTES,
			(const unsigned char *)pImageHeader,
			sizeof(ImageHeaderEncrypted) - sizeof(pImageHeader->Encrypted.header_tag),
			NULL,
			NULL,
			sizeof(pImageHeader->Encrypted.header_tag),
			pImageHeader->Encrypted.header_tag);
		maybe_raise_mbedtls_fault(ret, "mbedtls_gcm_crypt_and_tag failed to encrypt the data", "mbedtls_gcm_crypt_and_tag");
	}
	catch (const std::runtime_error)
	{
		mbedtls_gcm_free(&gcm);
		throw;
	}
	mbedtls_gcm_free(&gcm);
}

/** @brief Load a PKSC8 PEM Format Private Key.
*
*  @param [in]		step		Step Number
*  @param [in,out]  pk          Private PK Context
*  @param [in,out]  priv_ctx    Private RSA Context
*  @param [out]		KeyFile		File containing key
*  @param [out]		Password	Password (Empty string for no password)
*/
void LPC54X0XXImageHelper::GetPrivateKeyFromFile(int step, mbedtls_pk_context &pk, mbedtls_rsa_context * &priv_ctx, const char * KeyFile, const char * Password)
{
	int ret = 0;

	try
	{
		size_t num_key_bits;
		stringstream			ss;

		maybe_raise_fileaccess_fault(KeyFile, "Private RSA Key File");

		if (strcmp(Password, "") == 0)
		{
			ret = mbedtls_pk_parse_keyfile(&pk, KeyFile, NULL);
		}
		else
		{
			ret = mbedtls_pk_parse_keyfile(&pk, KeyFile, Password);
		}

		if (ret == MBEDTLS_ERR_PK_KEY_INVALID_FORMAT)
		{
			if (mbedtls_pk_parse_public_keyfile(&pk, KeyFile) == 0)
			{
				ss.str("");
				ss.clear();
				ss << "Private RSA key is not present in '" << KeyFile << "'. Public key is not sufficient.";
				throw std::runtime_error(ss.str());
			}
		}

		ss.str("");
		ss.clear();
		ss << "Parsing of private key '" << KeyFile << "' failed.";
		maybe_raise_mbedtls_fault(ret, ss.str(), "mbedtls_pk_parse_keyfile");

		priv_ctx = (mbedtls_rsa_context *)(pk.pk_ctx);

		num_key_bits = priv_ctx->len * 8;
		if (num_key_bits != SIG_LEN_BIT)
		{
			ss.str("");
			ss.clear();
			ss << "Private RSA key '" << KeyFile << "' should be 2048 bits but is " << num_key_bits << " bits.";
			throw std::runtime_error(ss.str());
		}
	}
	catch (const std::runtime_error)
	{
		throw;
	}
}

/** @brief Preformat a RSA2K Signature to speed up operations on the device.
*  Preformat-signature = Sig * R mod N.
*  Python code listing that can be used to convert the RSA signature to
*  Montgomery reduced form is included in the User Guide. This is a
*  implementation of same.
*  Using the mbedtls MPI library to perform the "bignum" operations
*  MPI => Multi-precision integer
*
*  @param [in]  pSig    The signature to preformat
*  @param [in]  N       Public modulus
*  @param [in]  E       Public exponent
*  @param [out] pPFSig  The prefomatted signature
*/
void LPC54X0XXImageHelper::PreFormatSignature(
	const SignatureType * const pSig,
	const mbedtls_mpi           N,
	const mbedtls_mpi           E,
	SignatureType *             pPFSig)
{
	int			i;
	int			j;
	int			ret;
	mbedtls_mpi mpiConstVal;
	mbedtls_mpi mpiSig;
	mbedtls_mpi mpiPFSig;
	mbedtls_mpi mpiReducer;

	mbedtls_mpi_init(&mpiSig);
	mbedtls_mpi_init(&mpiPFSig);
	mbedtls_mpi_init(&mpiReducer);
	mbedtls_mpi_init(&mpiConstVal);

	try
	{
		ret = mbedtls_mpi_read_binary(&mpiSig, pSig->Sig, sizeof(pSig->Sig));
		maybe_raise_mbedtls_fault(ret, "\"bignum\" conversion of input signature failed.", "Preformatting Signature");

		/// Compute R', which is R mod N. 
		ret = mbedtls_mpi_lset(&mpiConstVal, 3);
		maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_lset failed.", "Preformatting Signature");
		if (mbedtls_mpi_cmp_abs(&E, &mpiConstVal) == 0)
		{
			/* If exp = 3, R is 2 ^ (2 / 3 Nwidth) and so no mod */
			ret = mbedtls_mpi_lset(&mpiReducer, 1);
			maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_lset failed.", "Preformatting Signature");

			ret = mbedtls_mpi_shift_l(&mpiReducer, (RSA_KEY_SIZE * 2) / 3);
			maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_shift_l failed.", "Preformatting Signature");
		}
		else
		{
			/* else R is 2^n where n is 1 bit longer than Nmod, so 1 followed by 64 0 words */
			ret = mbedtls_mpi_lset(&mpiReducer, 1);
			maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_lset failed.", "Preformatting Signature");

			ret = mbedtls_mpi_shift_l(&mpiReducer, RSA_KEY_SIZE);
			maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_shift_l failed.", "Preformatting Signature");

			ret = mbedtls_mpi_mod_mpi(&mpiReducer, &mpiReducer, &N);
			maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_mod_mpi failed.", "Preformatting Signature");
		}

		// Calculate the Montgomery sign representation
		// mont_sign = (int_sign * reducer) % n
		ret = mbedtls_mpi_mul_mpi(&mpiPFSig, &mpiSig, &mpiReducer);
		maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_mul_mpi failed.", "Preformatting Signature");
		ret = mbedtls_mpi_mod_mpi(&mpiPFSig, &mpiPFSig, &N);
		maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_mod_mpi failed.", "Preformatting Signature");

		/* Make sure the number we have is the right size*/
		if (mbedtls_mpi_size(&mpiPFSig) != RSA_KEY_SIZE / (sizeof(mbedtls_mpi_uint) * 8))
		{
			ret = mbedtls_mpi_shrink(&mpiPFSig, RSA_KEY_SIZE / (sizeof(mbedtls_mpi_uint) * 8));
			maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_shrink failed.", "mbedtls_mpi_shrink");
		}

		/* Return the calculated signature */
		ret = mbedtls_mpi_write_binary(&mpiPFSig, pPFSig->Sig, sizeof(pPFSig->Sig));
		maybe_raise_mbedtls_fault(ret, "mbedtls_mpi_write_binary failed.", "mbedtls_mpi_write_binary");

		/* Reverse the even length array */
		i = SIG_LEN - 1;
		j = 0;
		while (i > j)
		{
			uint8_t temp = pPFSig->Sig[i];
			pPFSig->Sig[i] = pPFSig->Sig[j];
			pPFSig->Sig[j] = temp;
			i--;
			j++;
		}
	}
	catch (std::exception)
	{
		mbedtls_mpi_free(&mpiPFSig);
		mbedtls_mpi_free(&mpiSig);
		mbedtls_mpi_free(&mpiReducer);
		mbedtls_mpi_free(&mpiConstVal);
		throw;
	}

	mbedtls_mpi_free(&mpiPFSig);
	mbedtls_mpi_free(&mpiSig);
	mbedtls_mpi_free(&mpiReducer);
	mbedtls_mpi_free(&mpiConstVal);
}

/** @brief Insert a certificate into the image and allocate space for the signature
*
*  @param [in] step			Step Number
*  @param [in] substep		SubStep Number
*  @param [in] conf 		Configuration
*  @param [in] bPreformat   Preformatted signatures
*  @param [in] CertFile     Path to V2 or V3 format certificate file
*  @param [in] pk_rk		ROTK key (if available)
*/
void LPC54X0XXImageHelper::ImageInsertCertAndAllocSigSpace(int step,
	int &substep,
	const AuthImageGenerator::configuration &conf,
	mbedtls_pk_context &pk_ik,
	mbedtls_pk_context &pk_rk)
{
	CertificateDetails      Cert;
	int						ret = 0;
	uint8_t *				pCert = NULL;
	FILE *					ImageKeyCrtFile = NULL;
	size_t					CertSize = 0;
	size_t					CertPlusSignatureSize = 0;
	mbedtls_rsa_context		rsa;

	try
	{
		mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V15, 0);

		ImageKeyCrtFile = fopen(conf.imageKeyCertificate.c_str(), "rb");
		if (ImageKeyCrtFile == NULL)
		{
			stringstream			ss;
			ss << " Error '" << errno << "' (" << strerror(errno) << ") opening certificate file '" << conf.imageKeyCertificate.c_str() << "'.";
			throw std::runtime_error(ss.str());
		}

		fseek(ImageKeyCrtFile, 0, SEEK_END);
		CertSize = ftell(ImageKeyCrtFile);
		fseek(ImageKeyCrtFile, 0, SEEK_SET);

		if ((CertSize != V2_CERT_SIZE) && (CertSize != V3_CERT_SIZE))
		{
			stringstream			ss;
			ss << " Error '" << "Certificate file '" << conf.imageKeyCertificate.c_str() << "' is size " << CertSize << " bytes but should be either " << V2_CERT_SIZE << " bytes or " << V3_CERT_SIZE << " bytes.";
			throw std::runtime_error(ss.str());
		}
		CertPlusSignatureSize = CertSize + SIG_LEN;
		ImageSize += CertPlusSignatureSize;
		pImageBuffer = (uint8_t *)realloc(pImageBuffer, ImageSize);
		pCert = pImageBuffer + ImageSize - CertPlusSignatureSize;
		memset(pImageBuffer + ImageSize - CertPlusSignatureSize, 0x55, CertPlusSignatureSize);
		if (fread(pCert, 1, CertSize, ImageKeyCrtFile) != CertSize)
		{
			stringstream			ss;
			ss << " Error '" << errno << "' (" << strerror(errno) << ") reading certificate file '" << conf.imageKeyCertificate.c_str() << "'.";
			throw std::runtime_error(ss.str());
		}

		switch (CertSize)
		{
		case V2_CERT_SIZE:
			Cert.ImageKeyPublic = ((ImageKeyCertificateV2 *)pCert)->img_key_pub;
			Cert.RootOfTrustKeyPublic = ((ImageKeyCertificateV2 *)pCert)->rot_pub;
			break;
		default:
		case V3_CERT_SIZE:
			Cert.ImageKeyPublic = ((ImageKeyCertificateV3 *)pCert)->img_key_pub;
			Cert.RootOfTrustKeyPublic = ((ImageKeyCertificateV3 *)pCert)->rot_pub;
			break;
		}

		/* Get the hash of the cert */
		ret = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
			pImageBuffer + ImageSize - CertSize - SIG_LEN,
			CertSize - SIG_LEN,
			Cert.Hash);
		maybe_raise_mbedtls_fault(ret, "Hashing of certificate failed.", "mbedtls_md");

		Cert.pSig = (SignatureType *)(pImageBuffer + ImageSize - SIG_LEN - SIG_LEN);

		if (!conf.BypassCertificateChecks)
		{
			if (!conf.preformattedSignature)
			{
				stringstream			ss;
				/*
				Using the ROTK pub from within the Cert to verify the signature
				*/
				ss.str("");
				ss.clear();
				ss << "Copying Cert.RootOfTrustKeyPublic.modulus from '" << conf.imageKeyCertificate.c_str() << "'";
				ret = mbedtls_mpi_read_binary(&rsa.N, Cert.RootOfTrustKeyPublic.modulus, 256);
				maybe_raise_mbedtls_fault(ret, ss.str(), "mbedtls_mpi_read_binary");

				ss.str("");
				ss.clear();
				ss << "Copying Cert.RootOfTrustKeyPublic.exponent from '" << conf.imageKeyCertificate.c_str() << "'";
				ret = mbedtls_mpi_read_binary(&rsa.E, (uint8_t*)&Cert.RootOfTrustKeyPublic.exponent, 4);
				maybe_raise_mbedtls_fault(ret, ss.str(), "mbedtls_mpi_read_binary");

				ss.str("");
				ss.clear();
				ss << "Checking Cert.RootOfTrustKeyPublic from '" << conf.imageKeyCertificate.c_str() << "'";
				rsa.len = mbedtls_mpi_bitlen(&rsa.N) / 8;
				ret = mbedtls_rsa_check_pubkey(&rsa);
				maybe_raise_mbedtls_fault(ret, ss.str(), "mbedtls_rsa_check_pubkey");

				ss.str("");
				ss.clear();
				ss << "Verifying that certificate '" << conf.imageKeyCertificate.c_str() << "' has a non pre-formatted signature using Cert.RootOfTrustKeyPublic";
				ret = mbedtls_rsa_rsassa_pkcs1_v15_verify(&rsa,
					NULL,
					NULL,
					MBEDTLS_RSA_PUBLIC,
					MBEDTLS_MD_SHA256,
					sizeof(Cert.Hash),
					Cert.Hash,
					Cert.pSig->Sig);
				maybe_raise_mbedtls_fault(ret, ss.str(), "mbedtls_rsa_rsassa_pkcs1_v15_verify");
			}

			if (pk_rk.pk_ctx)
			{
				// The ROTK private key was provided so we can perform extra checks
				if (conf.preformattedSignature)
				{
					mbedtls_rsa_context *	ctx = (mbedtls_rsa_context *)(pk_rk.pk_ctx);
					SignatureType			Sig;
					SignatureType			PFSig;

					// We have the ROTK key so we can sign and then preformat the
					// signature to see if what we have matches
					ret = mbedtls_rsa_rsassa_pkcs1_v15_sign(ctx,
						NULL,
						NULL,
						MBEDTLS_RSA_PRIVATE,
						MBEDTLS_MD_SHA256,
						sizeof(Cert.Hash),
						Cert.Hash,
						Sig.Sig);
					maybe_raise_mbedtls_fault(ret, "mbedtls_rsa_rsassa_pkcs1_v15_sign failed.", "mbedtls_rsa_rsassa_pkcs1_v15_sign");

					// Reduce the signature and compare against what we have in the certificate
					PreFormatSignature(&Sig, ctx->N, ctx->E, &PFSig);
					if (memcmp(PFSig.Sig, Cert.pSig->Sig, SIG_LEN))
					{
						stringstream			ss;
						ss << "Certificate '" << conf.imageKeyCertificate.c_str() << "' does not have a pre-formatted signature generated using provided ROTK file '" << conf.rootOfTrustKeyFile.c_str() << "'.";
						throw std::runtime_error(ss.str());
					}
					// The ROTK private key we have is the same as what was used to
					// sign this certificate.
				}
				else
				{
					stringstream			ss;
					ss << "Certificate '" << conf.imageKeyCertificate.c_str() << "' does not have a non pre-formatted signature generated using provided ROTK file '" << conf.rootOfTrustKeyFile.c_str() << "'.";
					/* Use the verify capability of the provided ROTK file to check that the certificate matches the ROTK */
					ret = mbedtls_pk_verify(&pk_rk, MBEDTLS_MD_SHA256, Cert.Hash, 0, Cert.pSig->Sig, SIG_LEN);
					maybe_raise_mbedtls_fault(ret, ss.str(), "mbedtls_pk_verify");
				}
			}
		}

	}
	catch (const std::runtime_error)
	{
		mbedtls_rsa_free(&rsa);
		if (ImageKeyCrtFile != NULL)
		{
			fclose(ImageKeyCrtFile);
		}
		throw;
	}

	mbedtls_rsa_free(&rsa);
	if (ImageKeyCrtFile != NULL)
	{
		fclose(ImageKeyCrtFile);
	}
}

//! Insert PUF datastore\header to the content that is currently in the image buffer
//!
//! \param[in] step Step number for logging purposes.
//! \param[in] conf Configuration information
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::ImageInsertPUF(int step, const AuthImageGenerator::configuration &conf)
{
	int						ret = 0;
	stringstream			ss;
	ImageHeaderCommon *		pImageHeader = NULL;
	FILE *					keyStoreFile = NULL;

	try
	{
		if ((this->pImageBuffer == NULL) || (this->ImageSize == 0x0))
		{
			throw std::runtime_error("This image is empty and cannot have a PUF store.");
		}

		pImageHeader = (ImageHeaderCommon *)(this->pImageBuffer + this->ImageBufferHeaderOffset);
		if (!pImageHeader->Common.img_type.encrypted)
		{
			throw std::runtime_error("This image should be already encrypted before insertion of a PUF store.");
		}

		uint32_t AdditionalSize = sizeof(ImageHeaderKeyStore) + sizeof(skboot_key_store_lpc54x0xx_t);
		this->ImageSize += AdditionalSize;

		this->pImageBuffer = (uint8_t *)realloc(this->pImageBuffer, this->ImageSize);
		if (this->pImageBuffer == NULL)
		{
			throw std::runtime_error("Failed to re-allocate memory for image buffer before performing inserting PUF Store.");
		}
		(void)memmove(this->pImageBuffer + AdditionalSize, this->pImageBuffer, this->ImageSize - AdditionalSize);
		pImageHeader = (ImageHeaderCommon *)this->pImageBuffer;

		memcpy(this->pImageBuffer, this->pImageBuffer + AdditionalSize, sizeof(ImageHeaderKeyStore));
		this->ImageBufferHeaderOffset = 0;

		uint32_t ks_offset = 0x80;
		uint32_t img_offset = AdditionalSize;
		uint32_t puf_delay = KS_PUF_DELAY_DEFAULT;

		pImageHeader->KeyStore.header_marker = KS_HEADER_MARKER;
		pImageHeader->KeyStore.img_type = KS_HEADER_IMGTYPE;
		pImageHeader->KeyStore.header_marker = KS_HEADER_MARKER;
		pImageHeader->KeyStore.img_type = KS_HEADER_IMGTYPE;
		pImageHeader->KeyStore.header_offset = 0;
		pImageHeader->KeyStore.image_marker = IMG_MARKER;
		pImageHeader->KeyStore.key_offset = ks_offset;
		pImageHeader->KeyStore.key_store_len = KS_STORE_SIZE;
		pImageHeader->KeyStore.img_offset = img_offset;
		pImageHeader->KeyStore.puf_init_delay = puf_delay;
		memset(pImageHeader->KeyStore.reserved4, 0xFF, sizeof(pImageHeader->KeyStore.reserved4));

		step++;
		Log::log(Logger::INFO2, "%d. Key store configuration.\n", step);
		if (conf.useKeyStore)
		{
			const char* configdata = NULL;
			if (conf.keyStoreFile.length() > 0)
			{
				Log::log(Logger::INFO2, "%d.1 Add key store from specified file.\n", step);

				configdata = conf.keyStoreFile.c_str();
				keyStoreFile = fopen(configdata, "rb");
				if (NULL == keyStoreFile)
				{
					Log::log(Logger::INFO2, "Cannot open key store file %s.\n", configdata);
					throw std::runtime_error("Cannot open key store file.");
				}

				fseek(keyStoreFile, 0, SEEK_END);
				auto keyStoreFileSize = ftell(keyStoreFile);
				Log::log(Logger::INFO2, "\tKey Store File Path = %s, size = %lu bytes\n", configdata, keyStoreFileSize);
				fseek(keyStoreFile, 0, SEEK_SET);

				size_t keyStoreSize = sizeof(skboot_key_store_lpc54x0xx_t);
				if (keyStoreSize != keyStoreFileSize)
				{
					Log::log(Logger::ERROR, "\tWrong size of key store file for %s. (expected: %d bytes, fetched: %lu bytes)\n", LPC54X0XX, keyStoreSize, keyStoreFileSize);
					throw std::runtime_error("Wrong size of key store file.");
				}
				ret = fread(this->pImageBuffer + pImageHeader->KeyStore.key_offset, 1, pImageHeader->KeyStore.key_store_len, keyStoreFile);

				if (ret != keyStoreSize)
				{
					Log::log(Logger::ERROR, "Fail reading key store file.\n");
					throw std::runtime_error("Fail reading key store file.");
				}
			}
			else
			{
				Log::log(Logger::INFO2, "%d.1 Reserve key store space only.\n", step);
				Log::log(Logger::INFO, "\tKey store with size %lu bytes will be reserved in image from address: 0x%x.\n",
					pImageHeader->KeyStore.key_store_len,
					pImageHeader->KeyStore.key_offset);
				memset(this->pImageBuffer + pImageHeader->KeyStore.key_offset, 0x55, pImageHeader->KeyStore.key_store_len);
			}
		}
		Log::log(Logger::INFO2, "\tSuccess.\n");
		Log::log(Logger::INFO2, "%d. Performing PUF.\n", step);
		if (keyStoreFile != NULL)
		{
			fclose(keyStoreFile);
		}
	}
	catch (const std::runtime_error)
	{
		if (keyStoreFile != NULL)
		{
			fclose(keyStoreFile);
		}
		throw;
	}

}

//! Sign the content that is currently in the image buffer.
//!
//! \param[in] step Step number for logging purposes.
//! \param[in] conf Configuration information
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::SignImage(int step, const AuthImageGenerator::configuration &conf)
{
	int						ret = 0;
	ImageHeaderSigned *	    pImageHeader = NULL;
	stringstream			ss;
	mbedtls_rsa_context *	rsa_ik_priv_ctx = NULL;
	mbedtls_rsa_context *	rsa_rk_priv_ctx = NULL;

	SignatureType * 		pCertSig = NULL;
	SignatureType *			pImageSig = NULL;

	mbedtls_pk_context		pk_ik_priv;
	mbedtls_pk_context		pk_rk_priv;

	unsigned char			hash[SHA256_HASH_LENGTH_IN_BYTES];
	int substep = 1;

	try
	{
		Log::log(Logger::INFO2, "\t%d.%d Performing Signing.\n", step, substep++);

		mbedtls_pk_init(&pk_ik_priv);
		mbedtls_pk_init(&pk_rk_priv);

		try
		{
			ss.str("");
			ss.clear();
			ss << "\t" << step << "." << substep++ << " Loading private image key '" << conf.privateImageKey.c_str() << "'.\n";
			Log::log(Logger::INFO2, ss.str());
			GetPrivateKeyFromFile(step, pk_ik_priv, rsa_ik_priv_ctx, conf.privateImageKey.c_str(), conf.privateImageKeyPassword.c_str());

			if (conf.rootOfTrustKeyFile != "")
			{
				ss.str("");
				ss.clear();
				ss << "\t" << step << "." << substep++ << " Loading private rotk key '" << conf.rootOfTrustKeyFile.c_str() << "'.\n";
				Log::log(Logger::INFO2, ss.str());
				GetPrivateKeyFromFile(step, pk_rk_priv, rsa_rk_priv_ctx, conf.rootOfTrustKeyFile.c_str(), conf.rootOfTrustKeyFilePassword.c_str());
			}

			ss.str("");
			ss.clear();
			ss << "\t" << step << "." << substep++ << " Inserting certificate '" << conf.imageKeyCertificate.c_str() << "' into image.\n";
			Log::log(Logger::INFO2, ss.str());
			ImageInsertCertAndAllocSigSpace(step, substep, conf, pk_ik_priv, pk_rk_priv);
			pImageHeader = (ImageHeaderSigned *)(pImageBuffer + ImageBufferHeaderOffset);

			if (pImageHeader->img_type.signed_)
			{
				throw std::runtime_error("This image has been signed already.");
			}

			pCertSig = (SignatureType *)(pImageBuffer + ImageSize - SIG_LEN - SIG_LEN);
			pImageSig = (SignatureType *)(pImageBuffer + ImageSize - SIG_LEN);

			pImageHeader->img_type.signed_ = true;
			pImageHeader->img_type.secure_boot = true;
			pImageHeader->img_type.nocrc = false;
			pImageHeader->img_type.xip = false;

			if (conf.DebugCompatability)
			{
				pImageHeader->reserved[1] = IMG_MARKER;
			}

			pImageHeader->img_type.montformat = conf.preformattedSignature;
			if (pImageHeader->img_type.encrypted)
			{
				pImageHeader->img_len += sizeof(ImageHeaderEncrypted);
			}
			else
			{
				pImageHeader->img_len = this->plainImageSize - this->NoSignInitialBlock;
			}
			if (!conf.imageLinkAddressFromImage)
			{
				pImageHeader->load_addr = conf.imageLinkAddress;
			}
			else
			{
				if ((pImageHeader->load_addr != 0x00000000) && (pImageHeader->load_addr != 0x20000000))
				{
					ss << "\tImage file '" << conf.imageFile.c_str() << "' does not have a valid image load address : Expected 0x00000000 or 0x20000000 but read 0x" << std::hex << pImageHeader->load_addr << ".\n";
					throw std::runtime_error(ss.str());
				}
			}

			/* Get the hash of the image */
			ret = mbedtls_md(mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), pImageBuffer + this->NoSignInitialBlock, ImageSize - SIG_LEN - this->NoSignInitialBlock, hash);
			maybe_raise_mbedtls_fault(ret, "mbedtls_md failed.", "mbedtls_md");

			// Sign with the image key
			ret = mbedtls_rsa_rsassa_pkcs1_v15_sign(rsa_ik_priv_ctx,
				NULL,
				NULL,
				MBEDTLS_RSA_PRIVATE,
				MBEDTLS_MD_SHA256,
				sizeof(hash),
				hash,
				pImageSig->Sig);
			maybe_raise_mbedtls_fault(ret, "mbedtls_rsa_rsassa_pkcs1_v15_sign failed.", "mbedtls_rsa_rsassa_pkcs1_v15_sign");

			if (conf.preformattedSignature)
			{
				Log::log(Logger::INFO2, "\t%d.%d Performing signature reduction to speed up operations.\n", step, substep++);
				PreFormatSignature(pImageSig, rsa_ik_priv_ctx->N, rsa_ik_priv_ctx->E, pImageSig);
			}
		}
		catch (std::exception) {
			mbedtls_pk_free(&pk_ik_priv);
			mbedtls_pk_free(&pk_rk_priv);
			throw;
		}

		mbedtls_pk_free(&pk_ik_priv);
		mbedtls_pk_free(&pk_rk_priv);
	}
	catch (const std::runtime_error)
	{
		throw;
	}
}

//! Perform CRC calculation.
//!
//! \param[in] initial Initial value for CRC calculation.
//! \param[in] offset 32 bit word offset into image buffer
//! \param[in] len Number of 32 bit words to include in calculation 
//! \return Updated CRC value
uint32_t LPC54X0XXImageHelper::CRC_Update(uint32_t initial, size_t offset, size_t len)
{
	uint32_t c = initial ^ 0xFFFFFFFF;
	const uint8_t* u = pImageBuffer + offset;
	for (size_t i = 0; i < len; ++i)
	{
		c = this->crc_lut_table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
	}
	return c ^ 0xFFFFFFFF;

}

//! Perform CRC on the content that is currently in the image buffer.
//!
//! \param[in] step Step number for logging purposes.
//! \param[in] conf Configuration information
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
void LPC54X0XXImageHelper::CRCImage(int step, const AuthImageGenerator::configuration &conf)
{
	ImageHeaderNonSecure *			pImageHeader = (ImageHeaderNonSecure *)(pImageBuffer + ImageBufferHeaderOffset);
	uint32_t crcResult = 0;
	stringstream ss;
	try
	{
		Log::log(Logger::INFO2, "%d. Performing CRC.\n", step);

		pImageHeader->img_type.signed_ = false;
		pImageHeader->img_type.encrypted = false;
		pImageHeader->img_type.nocrc = false;
		pImageHeader->img_type.secure_boot = false;
		pImageHeader->img_type.xip = false;
		pImageHeader->crc_value = 0x0;
		pImageHeader->img_len = this->plainImageSize - 4;

		if (!conf.imageLinkAddressFromImage)
		{
			pImageHeader->load_addr = conf.imageLinkAddress;
		}
		else
		{
			if ((pImageHeader->load_addr != 0x00000000) && (pImageHeader->load_addr != 0x20000000))
			{
				ss << "\tImage file '" << conf.imageFile.c_str() << "' does not have a valid image load address : Expected 0x00000000 or 0x20000000 but read 0x" << std::hex << pImageHeader->load_addr << ".\n";
				throw std::runtime_error(ss.str());
			}
		}

		// Calculate the CRC, excluding the CRC location
		crcResult = CRC_Update(0, 0x0, ImageBufferHeaderOffset + IMG_HEADER_CRC_OFFSET);
		crcResult = CRC_Update(crcResult, ImageBufferHeaderOffset + IMG_HEADER_CRC_OFFSET + IH_CRC_SIZE, ImageSize - ImageBufferHeaderOffset - IMG_HEADER_CRC_OFFSET - IH_CRC_SIZE);

		if (conf.DebugCompatability)
		{
			pImageHeader->reserved[1] = IMG_MARKER;
		}

		pImageHeader->crc_value = crcResult;

		Log::log(Logger::INFO2, "\tCRC32 checksum: 0x%X\n", crcResult);
		Log::log(Logger::INFO2, "\tSuccess.\n");

	}
	catch (const std::runtime_error)
	{
		throw;
	}
}

size_t LPC54X0XXImageHelper::GetImageSize()
{
	return ImageSize;
}

const uint8_t * const LPC54X0XXImageHelper::GetImage()
{
	return (const uint8_t * const)pImageBuffer;
}

//! Create image based on the configuration structure.
//!
//! \param[in] conf Configuration information
//! \exception std::runtime_error This exception will be thrown any issues
//!		were encountered 
int AuthImageGenerator::createImageLPC54X0XXSboot(const configuration &conf)
{
	LPC54X0XXImageHelper IHelper;
	stringstream ss;
	const char* configdata = NULL;
	vector<boot_image_action_t> actList;
	int step = 1;
	FILE *outputFile = NULL;

	actList.push_back(bia_LoadFiles);

	configdata = conf.imageType.c_str();
	if (strncmp("xip+none", configdata, MAX_PATH) == 0)
	{
		Log::log(Logger::INFO2, "\tExternal flash (XIP) - Image is executed-in-place without CRC check.\n");
	}
	else if (strncmp("ram+none", configdata, MAX_PATH) == 0)
	{
		Log::log(Logger::INFO2, "\tRAM - Image is loaded to RAM without CRC check.\n");
	}
	else if (strncmp("xip+crc", configdata, MAX_PATH) == 0)
	{
		actList.push_back(bia_CRC);
		Log::log(Logger::INFO2, "\tExternal flash (XIP) - Image is executed-in-place with CRC check.\n");
	}
	else if (strncmp("ram+crc", configdata, MAX_PATH) == 0)
	{
		actList.push_back(bia_CRC);
		Log::log(Logger::INFO2, "\tRAM - Image is loaded to RAM with CRC check.\n");
	}
	else if (strncmp("ram+signed", configdata, MAX_PATH) == 0)
	{
		actList.push_back(bia_Sign);
		Log::log(Logger::INFO2, "\tRAM - Image will be signed.\n");
	}
	else if (strncmp("ram+encrypted", configdata, MAX_PATH) == 0)
	{
		actList.push_back(bia_Encrypt);
		if (conf.useKeyStore)
		{
			actList.push_back(bia_PUFStore);
		}
		Log::log(Logger::INFO2, "\tRAM - Image will be encrypted.\n");
	}
	else if (strncmp("ram+encrypted+signed", configdata, MAX_PATH) == 0)
	{
		actList.push_back(bia_Sign);
		actList.push_back(bia_Encrypt);
		if (conf.useKeyStore)
		{
			actList.push_back(bia_PUFStore);
		}
		Log::log(Logger::INFO2, "\tRAM - Image will be encrypted and then signed.\n");
	}
	else if (strncmp("ram+signed+encrypted", configdata, MAX_PATH) == 0)
	{
		actList.push_back(bia_Encrypt);
		actList.push_back(bia_Sign);
		if (conf.useKeyStore)
		{
			actList.push_back(bia_PUFStore);
		}
		Log::log(Logger::INFO2, "\tRAM - Image will be signed and then encrypted.\n");
	}
	else
	{
		Log::log(Logger::ERROR, "Unexpected image type value: %s.\n", configdata);
		cleanUpLPC54X0XXSboot(outputFile);
		return FAIL_RETURN;
	}
	actList.push_back(bia_OutputImage);

	for (vector<boot_image_action_t>::iterator it = actList.begin(); it != actList.end(); it++)
	{
		step++;
		switch (*it)
		{
		case bia_LoadFiles:
			try
			{
				Log::log(Logger::INFO, "%d. Opening Input Image.\n", step);
				IHelper.OpenImage(step, conf);
				Log::log(Logger::INFO, "\tSuccess.\n");
			}
			catch (const std::invalid_argument) {
				Log::log(Logger::ERROR, ss.str());
				cleanUpLPC54X0XXSboot(outputFile);
				return FAIL_RETURN;
			}
			break;
		case bia_CRC:
			try
			{
				Log::log(Logger::INFO, "%d. Calculating Image CRC.\n", step);
				IHelper.CRCImage(step, conf);
				Log::log(Logger::INFO, "\tSuccess.\n");
			}
			catch (const std::invalid_argument)
			{
				Log::log(Logger::ERROR, ss.str());
				cleanUpLPC54X0XXSboot(outputFile);
				return FAIL_RETURN;
			}
			break;
		case bia_Encrypt:
			try
			{
				Log::log(Logger::INFO, "%d. Encrypting Image.\n", step);
				IHelper.EncryptImage(step, conf);
				Log::log(Logger::INFO, "\tSuccess.\n");
			}
			catch (const std::invalid_argument)
			{
				Log::log(Logger::ERROR, ss.str());
				cleanUpLPC54X0XXSboot(outputFile);
				return FAIL_RETURN;
			}
			break;
		case bia_Sign:
			try
			{
				Log::log(Logger::INFO, "%d. Signing Image.\n", step);
				IHelper.SignImage(step, conf);
				Log::log(Logger::INFO, "\tSuccess.\n");
			}
			catch (const std::invalid_argument)
			{
				Log::log(Logger::ERROR, ss.str());
				cleanUpLPC54X0XXSboot(outputFile);
				return FAIL_RETURN;
			}
			break;
		case bia_PUFStore:
			try
			{
				Log::log(Logger::INFO, "%d. Inserting PUF.\n", step);
				IHelper.ImageInsertPUF(step, conf);
				Log::log(Logger::INFO, "\tSuccess.\n");
			}
			catch (const std::invalid_argument)
			{
				Log::log(Logger::ERROR, ss.str());
				cleanUpLPC54X0XXSboot(outputFile);
				return FAIL_RETURN;
			}
			break;
		case bia_OutputImage:
			Log::log(Logger::INFO, "%d. Creating output image file.\n", step);
			configdata = conf.masterBootOutputFile.c_str();
			outputFile = fopen(configdata, "wb");
			if (outputFile == NULL)
			{
				Log::log(Logger::ERROR, "\tCannot open/create output file %s.\n", configdata);
				ss << "\tCannot open/create output file :" << configdata << ".\n";
				throw std::invalid_argument(ss.str());
			}
			Log::log(Logger::INFO2, "\t%d.1 Writing output image to file.\n", step);

			if (IHelper.GetImageSize() != fwrite(IHelper.GetImage(), 1, IHelper.GetImageSize(), outputFile))
			{
				Log::log(Logger::ERROR, "\tCannot write output file %s.\n", configdata);
				ss << "\tCannot open/create output file :" << configdata << ".\n";
				throw std::invalid_argument(ss.str());
			}

			configdata = conf.masterBootOutputFile.c_str();
			Log::log(Logger::INFO, "\tSuccess. (Image %s created.)\n", configdata);
			break;
		}
	}

	cleanUpLPC54X0XXSboot(outputFile);
	return SUCCESS_RETURN;
}

/** @brief Clean up used resources, close files etc
*
*  @param [in] outputFile
*/
void AuthImageGenerator::cleanUpLPC54X0XXSboot(FILE * outputFile)
{
	if (outputFile != NULL)
	{
		fclose(outputFile);
	}
}