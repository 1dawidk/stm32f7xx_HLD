#include "core.h"

uint32_t Core::APB1Freq=0;
uint32_t Core::APB2Freq=0;
uint32_t Core::SysClkFreq=0;

uint8_t Core::init(uint16_t hse_freq, uint32_t hclk_freq, bool use_icache, bool use_dcache){	
	init_nvic(NVIC_PRIORITYGROUP_4);
	enable_cache(use_icache, use_dcache);
	init_sysTick(SYSTICK_SOURCE_HCLK_DIV8, hclk_freq);
	init_rcc(hse_freq);
	
	return 0;
}

void Core::enable_fpu(){
}

void Core::enable_cache(bool icache, bool dcache){
	if(icache){
		/* Enable I-Cache */
		SCB_InvalidateICache();
		SCB_EnableICache();
	}
	
	if(dcache){
		//Enable MPU
		//Set shareable region
		/* Enable D-Cache */
		SCB_InvalidateDCache();
		SCB_EnableDCache();
	}
}

uint16_t Core::init_rcc(uint16_t hse_freq){
	uint32_t rcc_status=0;
	
	//Reset clock config
	Rcc::Deinit();
	
	//Enable Pwr Clk
	Rcc::SetPeriphClkState(RCC_PERIPHCLK_PWR, RCC_ENABLE);
	Pwr::SetVoltageScaling(PWR_VOLTAGE_SCALE1);
	
	//PLL config values
	uint16_t pllm=hse_freq;
	uint16_t plln=432;
	uint16_t pllp=RCC_PLLP_DIV2;
	uint16_t pllq=9;
	
	//Oscilator init (HSE)
	if(!hse_freq){
		rcc_status+=Rcc::SetPLL(RCC_PLLSOURCE_HSI, 16, plln, pllp, pllq);
		SysClkFreq= 16000000;
	} else {		
		SysClkFreq= pllm*1000000;
		rcc_status+=Rcc::TurnOnOsc(RCC_OSCILLATOR_HSE);
		rcc_status+=Rcc::SetPLL(RCC_PLLSOURCE_HSE, pllm, plln, pllp, pllq);
	}
	
	//Activate the OverDrive to reach the 216 MHz Frequency
	Pwr::EnableOverDrive();
	
	//Clock init (SYSCLK, HCLK, PCLK1, PCLK2)
	rcc_status+= Rcc::SetAHBDivider( RCC_AHB_CLKDIV_1 );
	rcc_status+= Rcc::SetAPB1Divider( RCC_APB_CLKDIV_4 );
	APB1Freq= SysClkFreq/4;
	rcc_status+= Rcc::SetAPB2Divider( RCC_APB_CLKDIV_2 );
	APB2Freq= SysClkFreq/2;
	Flash::SetLatency(FLASH_LATENCY_7);
	
	rcc_status+= Rcc::SetSystemClkSource(RCC_SYSCLK_SOURCE_PLL);
	
	Rcc::SetPeriphClkState(RCC_PERIPHCLK_SYSCFG, RCC_ENABLE);
	
	if(hse_freq)
		rcc_status+= Rcc::TurnOffOsc(RCC_OSCILLATOR_HSI);
	
	if(rcc_status)
		return 16;
	else
		return 216;
}

void Core::init_nvic(uint32_t priorityGrouping){
	#ifdef VECT_TAB_SRAM
  SCB->VTOR = RAMDTCM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
	#else
  SCB->VTOR = FLASH_BASE; /* Vector Table Relocation in Internal FLASH */
	#endif
	
	Nvic::SetPriorityGrouping(priorityGrouping);
	
  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
	Nvic::SetPriority(MemoryManagement_IRQn, 0, 0);
  /* BusFault_IRQn interrupt configuration */
	Nvic::SetPriority(BusFault_IRQn, 0, 0);
  /* UsageFault_IRQn interrupt configuration */
	Nvic::SetPriority(UsageFault_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
	Nvic::SetPriority(PendSV_IRQn, 1, 0);
}

void Core::init_sysTick(uint32_t source, uint32_t sysClkFreq){

	if(source==SYSTICK_SOURCE_HCLK)
		SysTick->LOAD  = (uint32_t)(sysClkFreq/1000 - 1UL);                         /* set reload register */
	else
		SysTick->LOAD = (uint32_t)(sysClkFreq/8000 - 1U);
	
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = source |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
	
	SysTick_Config(sysClkFreq/1000);
	Nvic::SetPriority(SysTick_IRQn, 0, 1);
}

void Core::init_rtc(bool use_lse){
}

//Core info
uint32_t Core::GetAPB1Freq(){
	return APB1Freq;
}

uint32_t Core::GetAPB2Freq(){
	return APB2Freq;
}

uint32_t Core::GetSysClkFreq(){
	return SysClkFreq;
}
