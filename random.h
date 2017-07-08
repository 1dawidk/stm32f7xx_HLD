#ifndef _RANDOM_H
#define _RANDOM_H

#include "stm32f7xx.h"
#include "rcc.h"
#include "uOS/uosLocker.h"

class Random{
	public:
		uint8_t Init();
		uint32_t getInt(uint32_t max);
		uint32_t getInt();
		float getFloat();
	private:
		void ReInit();
		uosLocker *locker;
};

#endif
