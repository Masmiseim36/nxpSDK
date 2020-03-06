#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <memory>
#include "json.h"

using namespace std;

#define MULTICORE_IMAGE_ALIGNMENT 4

#define LTI_LOAD		(1 << 0)
#define LTI_INIT		(1 << 1)
#define LTI_OVERLAP		(1 << 2)

typedef struct { 
    uint32_t src_addr; 
    uint32_t dest_addr; 
    uint32_t segment_size; 
    uint32_t flags; 
} LoadTableItem; 

typedef struct { 
    uint32_t marker; // always set to 0x4C54424C
    uint32_t version; 
    uint32_t number_of_entries; 
    uint32_t entriesPtr; 
} LoadTable;

class MulticoreImageGenerator {
public:
	class Image {
	public:
		string path;
		uint32_t loadAddress;
		unique_ptr<uint8_t[]> &&getData();
		size_t getSize();
	private:
		bool isLoaded;
		size_t size;
		unique_ptr<uint8_t[]> data;
		void load();
	};

	bool parseConfiguration(jute::jValue jsonConf, const string &confFilePath);
	unique_ptr<uint8_t[]> getData(uint32_t mainImageSize, size_t &dataSize);
	inline bool isMulticoreImage() { return isMulticore; };
private:
	inline void addImage(unique_ptr<Image> &&image) { images.push_back(move(image)); };
	unique_ptr<uint8_t[]> getRelocationTableData(uint32_t mainImageSize, size_t &sizeOfGeneratedTable);
	size_t getSize();
	vector<unique_ptr<Image>> images;
	bool isMulticore;
};