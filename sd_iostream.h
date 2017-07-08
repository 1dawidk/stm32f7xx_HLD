#ifndef _SDIOSTREAM_H
#define _SDIOSTREAM_H

#include "stm32f7xx.h"
#include "sdintf.h"

#define SD_IOSTREAM_ACCESS_IN				0x01
#define SD_IOSTREAM_ACCESS_OUT			0x02

#define SD_IOSTREAM_OK							0
#define SD_IOSTREAM_ERROR						-1
#define SD_IOSTREAM_EOF							-2

#define SD_IOSTREAM_READY						1
#define SD_IOSTREAM_NOTREADY				0

namespace SD{		
	
	class fstream{
		public:
			fstream(char *path, uint8_t access, SDIntf *sdIntf);
			uint8_t open();
			uint8_t close();
		
			//Write methods
			int16_t write(const char* s);
			int16_t writeLine(const char* s);
		
			//Read methods
			char* read(uint32_t n);
			char* readLine();
		private:
			SDIntf *sdIntf;
			char *path;
			uint8_t access;
			uint8_t ready;
	};
}

#endif
