#include "dsi.h"

void Dsi::Init(){
	Rcc::SetPeriphClkState(RCC_PERIPHCLK_DSI, RCC_ENABLE);
	Rcc::SetPeriphClkState(RCC_PERIPHCLK_LTDC, RCC_ENABLE);
	
	// IDF= 		5 	-> OK
	// NDIV= 		96	-> OK
	// ODF= 		1		-> OK
	// Tx pre= 	3		->
	
	dsiHandle->WRPCR= (0x9 << 11) | (96 << 2);
}