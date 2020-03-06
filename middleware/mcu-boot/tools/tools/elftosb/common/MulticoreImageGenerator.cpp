#include <sstream>
#include "MulticoreImageGenerator.h"
#include "Logging.h"

unique_ptr<uint8_t[]> MulticoreImageGenerator::getRelocationTableData(uint32_t mainImageSize, size_t &sizeOfGeneratedTable) {
	unique_ptr<LoadTableItem[]> loadItems = make_unique<LoadTableItem[]>(images.size());
	auto i = 0;
	for (auto &image : images)
	{
		loadItems[i].flags = LTI_LOAD;
		loadItems[i].segment_size = image->getSize();
		loadItems[i].dest_addr = image->loadAddress;
		loadItems[i].src_addr = mainImageSize;
		mainImageSize += image->getSize();
		i++;
	}
	LoadTable loadTab;
	loadTab.number_of_entries = images.size();
	loadTab.marker = 0x4C54424C;
	loadTab.version = 0;
	loadTab.entriesPtr = mainImageSize;
	sizeOfGeneratedTable = images.size() * sizeof(LoadTableItem) + sizeof(LoadTable);
	auto buffer = make_unique<uint8_t[]>(sizeOfGeneratedTable);
	memcpy(buffer.get(), (uint8_t*)&loadItems[0], images.size() * sizeof(LoadTableItem));
	memcpy(buffer.get() + (images.size() * sizeof(LoadTableItem)), (uint8_t*)&loadTab, sizeof(LoadTable));
	return move(buffer);
}

void MulticoreImageGenerator::Image::load() {
	if (isLoaded)
		return;
	ifstream input;
	input.open(this->path, ios::binary);
	if (input.fail()) {
		stringstream ss;
		ss << "\tCannot open file: " << this->path << " specified in multicore images list.";
		throw runtime_error(ss.str());
	}
	input.seekg(0, ios::end);
	size_t size;
	size_t imageSize = input.tellg();
	if (imageSize % MULTICORE_IMAGE_ALIGNMENT)
		size = imageSize + (MULTICORE_IMAGE_ALIGNMENT - (imageSize % MULTICORE_IMAGE_ALIGNMENT));
	else
		size = imageSize;
	this->size = size;
	data = make_unique<uint8_t[]>(size);
	input.seekg(0, ios::beg);
	input.read((char*)(data.get()), imageSize);
	this->isLoaded = true;
}

size_t MulticoreImageGenerator::Image::getSize() {
	if (isLoaded)
		return size;
	load();
	return size;
}

unique_ptr<uint8_t[]> &&MulticoreImageGenerator::Image::getData() {
	if (isLoaded)
		return move(data);
	load();
	return move(data);
}

size_t MulticoreImageGenerator::getSize() {
	size_t totalSize = 0;
	for (auto &image : images)
	{  
		totalSize += image->getSize();
	}
	return totalSize;
}

unique_ptr<uint8_t[]> MulticoreImageGenerator::getData(uint32_t mainImageSize, size_t &dataSize) {
	size_t realocTableSize;
	auto realocTable = getRelocationTableData(mainImageSize, realocTableSize);
	dataSize = getSize() + realocTableSize;
	auto usedSize = 0;
	auto buffer = make_unique<uint8_t[]>(dataSize); 
	for (auto &image : images)
	{  
		memcpy(buffer.get() + usedSize, image->getData().get(), image->getSize());
		usedSize += image->getSize();
	}
	memcpy(buffer.get() + usedSize, realocTable.get(), realocTableSize);
	return move(buffer);
}

bool MulticoreImageGenerator::parseConfiguration(jute::jValue jsonConf, const string &confFilePath) {
	auto error = false;
	stringstream ss;
	if (jsonConf["multicoreImages"].get_type() == jute::JARRAY && jsonConf["multicoreImages"].size() > 0) {
		int i = 0;
		while (jsonConf["multicoreImages"].size() > i ) {
			auto image = unique_ptr<Image>(new Image());
			if (jsonConf["multicoreImages"][i]["address"].get_type() != jute::JUNKNOWN) {
				string tmp(jsonConf["multicoreImages"][i]["address"].as_string());
				try {
					image->loadAddress = stoul(tmp, nullptr, 16);

					ss << "\tUsed \"multicoreImages\" \"" << i << "\" \"address\" value: " << image->loadAddress << "." << endl;
					Log::log(Logger::INFO2, ss.str());
					ss.str("");
				}
				catch (invalid_argument) {
					ss << "\tCannot parse \"multicoreImages\" \"" << i << "\" \"address\" value: " << jsonConf["load"][i]["address"].as_string() << " from configuration file: " << confFilePath << ".\n";
					ss << "\tExpected hexadecimal string in format \"0xHEXVALUE\" or only \"HEXVALUE\".\n";
					Log::log(Logger::ERROR, ss.str());
					ss.str("");
					error = true;
				}
				catch (out_of_range) {
					ss << "\tValue of \"multicoreImages\" \"" << i << "\" \"address\" is too big: " << jsonConf["load"][i]["address"].as_string() << " from configuration file: " << confFilePath << ".\n";
					ss << "\tUse max value as 0xffffffff.\n";
					Log::log(Logger::ERROR, ss.str());
					ss.str("");
					error = true;
				}

			}
			else {
				ss << "\tCannot read \"multicoreImages\" \"" << i << "\" \"address\" from configuration file: " << confFilePath << ".\n";
				Log::log(Logger::ERROR, ss.str());
				ss.str("");
				error = true;
				break;
			}
			if (jsonConf["multicoreImages"][i]["file"].get_type() != jute::JUNKNOWN) {
				image->path = jsonConf["multicoreImages"][i]["file"].as_string();
			}
			else {
				ss << "\tCannot read \"multicoreImages\" \"" << i << "\" \"file\" from configuration file: " << confFilePath << ".\n";
				Log::log(Logger::ERROR, ss.str());
				ss.str("");
				error = true;
				break;
			}
			addImage(unique_ptr<Image>(move(image)));
			i++;
		}
		isMulticore = true;
	}
	else {
		ss << "\tNo \"multicoreImages\" section present in configuration file: " << confFilePath << ".\n";
		Log::log(Logger::ERROR, ss.str());
		ss.str("");
	}
	return error;
};