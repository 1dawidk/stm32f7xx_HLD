#ifndef _DSI_H
#define _DSI_H

#include "stm32f7xx.h"
#include "HLD/rcc.h"

class Dsi{
	public:
		Dsi();
		void Init();
	
	private:
		DSI_TypeDef *dsiHandle;
};

#endif