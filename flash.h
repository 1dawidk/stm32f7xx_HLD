#ifndef _HLD_FLASH_H
#define _HLD_FLASH_H

#include "stm32f7xx.h"

#define FLASH_LATENCY_0						0x00
#define FLASH_LATENCY_1						0x01
#define FLASH_LATENCY_2						0x02
#define FLASH_LATENCY_3						0x03
#define FLASH_LATENCY_4						0x04
#define FLASH_LATENCY_5						0x05
#define FLASH_LATENCY_6						0x06
#define FLASH_LATENCY_7						0x07
#define FLASH_LATENCY_8						0x08
#define FLASH_LATENCY_9						0x09
#define FLASH_LATENCY_10					0x0A
#define FLASH_LATENCY_11					0x0B
#define FLASH_LATENCY_12					0x0C
#define FLASH_LATENCY_13					0x0D
#define FLASH_LATENCY_14					0x0E
#define FLASH_LATENCY_15					0x0F


class Flash{
	public:
		static void SetLatency(uint8_t latency);
		static void EnableART();
		static void EnablePrefetch();
};

#endif

