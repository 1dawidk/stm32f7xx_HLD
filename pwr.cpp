#include "pwr.h"

uint8_t Pwr::SetVoltageScaling(uint32_t scale){
	uint32_t timeout=HLDKernel::GetTimeMs()+25;
	
	PWR->CR1= (PWR->CR1&0xFFFF3FFF)|scale;
	
	while(!(PWR->CSR1&PWR_CSR1_VOSRDY)){
		if(HLDKernel::GetTimeMs() > timeout) return PWR_TIMEOUT;
	}
	
	return PWR_OK;
}

uint8_t Pwr::EnableOverDrive(){
	uint32_t timeout;
	
	PWR->CR1|= PWR_CR1_ODEN;
	timeout=HLDKernel::GetTimeMs()+10;
	while(!(PWR->CSR1&PWR_CSR1_ODRDY))
		if(HLDKernel::GetTimeMs() > timeout) return PWR_TIMEOUT;
	
	PWR->CR1|= PWR_CR1_ODSWEN;
	timeout=HLDKernel::GetTimeMs()+10;
	while(!(PWR->CSR1&PWR_CSR1_ODSWRDY))
		if(HLDKernel::GetTimeMs() > timeout) return PWR_TIMEOUT;
	
	return PWR_OK;	
}
