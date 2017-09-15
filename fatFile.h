#ifndef FATFILE_H
#define FATFILE_H

#include "stm32f7xx.h"

class FATPartition;

class FATFile {
public:
	FATFile();
	FATFile(char *name, FATPartition *fat_p, uint32_t data_clu, uint32_t entry_clu, uint16_t entry_offset, uint32_t size);
	~FATFile();

	//read, write
	uint16_t Read(uint32_t offset, uint8_t len);
	uint16_t Write();

	//Operator overload
	void operator<<(const char* str);
	void operator>>(char* buff);

	//Getters
	char* GetName();
	uint32_t GetDataClu();
	uint32_t GetEntryClu();
	uint32_t GetSize();
	uint16_t GetEntryOffset();

private:
	char* 				name;
	FATPartition 	*fatP;
	uint32_t 			dataClu;
	uint32_t 			entryClu;
	uint32_t 			size;
	uint16_t 			entryOffset;
	uint32_t			atOffset;
};

#endif
