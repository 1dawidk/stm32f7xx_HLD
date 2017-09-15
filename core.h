#ifndef _HLD_CORE_H
#define _HLD_CORE_H

#include "stm32f7xx.h"
#include "HLDKernel.h"
#include "nvic.h"
#include "rcc.h"
#include "pwr.h"
#include "flash.h"
#include "core_cm7.h"

#define SYSTICK_SOURCE_HCLK				(0x00000000UL)
#define SYSTICK_SOURCE_HCLK_DIV8	(0x00000004UL)

class Core{
	public:
		static uint8_t 	Init(uint16_t hse_freq, uint32_t hclk_freq, bool use_icache, bool use_dcache);
	
		//Core info
		static uint32_t GetAPB1Freq();
		static uint32_t GetAPB2Freq();
		static uint32_t GetSysClkFreq();
	
	private:
		static void 		EnableCache(bool icache, bool dcache);
		static void 		EnableFpu();
		static uint16_t InitRcc(uint16_t hse_freq);
		static void 		InitNvic(uint32_t priority_grouping);
		static void 		InitSysTick(uint32_t source, uint32_t sys_clk_freq);
		static void 		InitRtc(bool use_lse);
	
		//Freqs
		static uint32_t SysClkFreq;
		static uint32_t APB1Freq;
		static uint32_t APB2Freq;
};

#endif
