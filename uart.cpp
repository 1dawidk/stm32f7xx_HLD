#include "uart.h"
#include <stdlib.h>

UART::UART(USART_TypeDef *USARTx, uint8_t dataLen, uint8_t oversampling, uint8_t sampling,
						uint8_t parity, uint16_t baudRate, uint16_t rxBuffSize, uint16_t txBuffSize){
	
	this->uartHandle= USARTx;
							
	rxBuff= (uint8_t *)malloc(rxBuffSize);
	this->rxBuffSize= rxBuffSize;
	
	txBuff= (uint8_t *)malloc(txBuffSize);
	this->txBuffSize=txBuffSize;
	
	this->dataLen= dataLen;
	this->oversampling= oversampling;
	this->sampling= sampling;
	this->parity= parity;
	this->baudrate=baudRate;
							
	useTxDMA=0;
	useRxDMA=0;
}

uint8_t UART::Init(GPIO_TypeDef *GPIOx, uint16_t tx_pin, uint16_t rx_pin){
	uint16_t cr1=0x00;
	uint16_t cr3=0x00;
	
	//Select clock source
	if(uartHandle==USART1){
		RCC->DCKCFGR2|=RCC_DCKCFGR2_USART1SEL_0;
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_USART1, ENABLE);
		Gpio::InitAf(GPIOx,tx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
		Gpio::InitAf(GPIOx,rx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
	}else if(uartHandle==USART2){
		RCC->DCKCFGR2|=RCC_DCKCFGR2_USART2SEL_0;
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_USART2, ENABLE);
		Gpio::InitAf(GPIOx,tx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
		Gpio::InitAf(GPIOx,rx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
	}else if(uartHandle==USART3){
		RCC->DCKCFGR2|=RCC_DCKCFGR2_USART3SEL_0;
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_USART3, ENABLE);
		Gpio::InitAf(GPIOx,tx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
		Gpio::InitAf(GPIOx,rx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
	}else if(uartHandle==UART4){
		RCC->DCKCFGR2|=RCC_DCKCFGR2_UART4SEL_0;
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_UART4, ENABLE);
		Gpio::InitAf(GPIOx,tx_pin,GPIO_PP,GPIO_AF8,GPIO_PULLUP);
		Gpio::InitAf(GPIOx,rx_pin,GPIO_PP,GPIO_AF8,GPIO_PULLUP);
	}else if(uartHandle==UART5){
		RCC->DCKCFGR2|=RCC_DCKCFGR2_UART5SEL_0;
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_UART5, ENABLE);
		Gpio::InitAf(GPIOx,tx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
		Gpio::InitAf(GPIOx,rx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
	}else if(uartHandle==USART6){
		RCC->DCKCFGR2|=RCC_DCKCFGR2_USART6SEL_0;
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_USART6, ENABLE);
		Gpio::InitAf(GPIOx,tx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
		Gpio::InitAf(GPIOx,rx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
	}else if(uartHandle==UART7){
		RCC->DCKCFGR2|=RCC_DCKCFGR2_UART7SEL_0;
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_UART7, ENABLE);
		Gpio::InitAf(GPIOx,tx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
		Gpio::InitAf(GPIOx,rx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
	}else if(uartHandle==UART8){
		RCC->DCKCFGR2|=RCC_DCKCFGR2_UART8SEL_0;
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_UART8, ENABLE);
		Gpio::InitAf(GPIOx,tx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
		Gpio::InitAf(GPIOx,rx_pin,GPIO_PP,GPIO_AF7,GPIO_PULLUP);
	}
	
	//Initialize UART GPIO
	
	
	//Select Oversampling 8/16
	if(oversampling)
		cr1|=USART_CR1_OVER8;
	
	//Select Rx Sampling 3/1 clk pls
	if(sampling)
		cr3|=USART_CR3_ONEBIT;
	
	//Set BaudRate
	//Tx/Rx Baud= fclk/USARTDIV - 16 oversampling
	//Tx/Rx Baud= 2*fclk/USARTDIV - 8 oversampling
	if(oversampling){
		uartHandle->BRR=3750;
	} else {
		uartHandle->BRR=1875;
	}
	
	//Set data length and parity bit
	if(dataLen==UART_DATALEN_7)
		cr1|=USART_CR1_M_1;
	else if(dataLen==UART_DATALEN_9)
		cr1|=USART_CR1_M_0;
	
	if(parity){
		cr1|=USART_CR1_PCE;
		
		//Set odd/even parity
		if(parity==UART_PARITY_ODD)
			cr1|=USART_CR1_PS;
	}
	
	//Set registers
	uartHandle->CR1=cr1;
	uartHandle->CR3=cr3;
	
	//Enable Transmit and Receive
	EnableTx();
	EnableRx();
	
	//Return status code
	return 0;
}

void UART::Enable(){
	//Enable USART
	uartHandle->CR1|=USART_CR1_UE;
}

void UART::SetTxDMAStream(DMA *dma, DMA_Stream_TypeDef *stream, uint32_t channel){
	DMAStreamConfig *dmaStreamConfig=new DMAStreamConfig();
	
	//Assign DMA Handle and Stream handle
	this->dmaTxHandle= dma;
	dmaTxStreamHandle=stream;

	
	//Set DMA UART stream config
	dmaStreamConfig->ch=channel;
	dmaStreamConfig->priority=DMA_PRIORITY_VERY_HIGH;
	dmaStreamConfig->direction=DMA_DIRECTION_MEM_PER;
	
	dmaStreamConfig->memDataSize=DMA_DATASIZE_BYTE;
	dmaStreamConfig->periphDataSize=DMA_DATASIZE_BYTE;
	
	dmaStreamConfig->periphAdr=(uint32_t)&(uartHandle->TDR);
	dmaStreamConfig->memoryAdr=(uint32_t)(txBuff);
	
	dmaStreamConfig->periphInc=DMA_INC_DISABLE;
	dmaStreamConfig->memInc=DMA_INC_ENABLE;
	
	dmaStreamConfig->circural=DMA_CIRCULAR_DISABLE;
	dmaStreamConfig->fc=DMA_FLOWCONTROL_DISABLE;
	
	//Register UART DMA stream
	dmaTxHandle->RegisterStream(dmaTxStreamHandle, dmaStreamConfig);
	
	//Enable UART Tx DMA mode
	uartHandle->CR3|=USART_CR3_DMAT;
	
	useTxDMA=1;
	
	//Clear memory
	delete dmaStreamConfig;
}

void UART::SetRxDMAStream(DMA *dma, DMA_Stream_TypeDef *stream, uint32_t channel){
	DMAStreamConfig *dmaStreamConfig=new DMAStreamConfig();
	
	//Assign DMA Handle and Stream Handle
	this->dmaRxHandle= dma;
	dmaRxStreamHandle=stream;

	//Set DMA UART stream config
	dmaStreamConfig->ch=channel;
	dmaStreamConfig->priority=DMA_PRIORITY_VERY_HIGH;
	dmaStreamConfig->direction=DMA_DIRECTION_PER_MEM;
	
	dmaStreamConfig->memDataSize=DMA_DATASIZE_BYTE;
	dmaStreamConfig->periphDataSize=DMA_DATASIZE_BYTE;
	
	dmaStreamConfig->periphAdr=(uint32_t)&(uartHandle->RDR);
	dmaStreamConfig->memoryAdr=(uint32_t)(rxBuff);
	
	dmaStreamConfig->periphInc=DMA_INC_DISABLE;
	dmaStreamConfig->memInc=DMA_INC_ENABLE;
	
	dmaStreamConfig->circural=DMA_CIRCULAR_ENABLE;
	dmaStreamConfig->fc=DMA_FLOWCONTROL_DISABLE;
	
	//Register UART DMA stream
	dmaRxHandle->RegisterStream(dmaRxStreamHandle, dmaStreamConfig);
	
	//Enable UART Tx DMA mode
	uartHandle->CR3|=USART_CR3_DMAR;
	
	//Enable DMA TC irq
	/*HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);*/

	//Start Stream
	dmaRxHandle->StartStream(dmaRxStreamHandle, rxBuffSize);
	
	useRxDMA=1;
	
	//Clear memory
	delete dmaStreamConfig;
}

void UART::EnableRx(){
	uartHandle->CR1|=USART_CR1_RE;
}

void UART::EnableTx(){
	uartHandle->CR1|=USART_CR1_TE;
}

void UART::DisableRx(){
	uartHandle->CR1&=~USART_CR1_RE;
}

void UART::DisableTx(){
	uartHandle->CR1&=~USART_CR1_TE;
}

void UART::Transmit(char *data){
	uint16_t i=0;
	
	while(!CheckTC());
	
	//Rewrite data to buffer
	while(data[i]!=0){
		txBuff[i]= data[i];
		i++;
	}
	
	uartHandle->ICR|=USART_ICR_TCCF;
	//Start transmition
	StartTransmition(i);
}

void UART::Transmit(const char* format, ...){
	va_list arg_list;
	va_start(arg_list, format);
	
	while(!CheckTC());
	
	//Rewrite data to buffer
	uint16_t i=vsprintf((char *)txBuff, format, arg_list);
	
	uartHandle->ICR|=USART_ICR_TCCF;
	//Start transmition
	StartTransmition(i);
}

void UART::Transmit(const char* format, va_list arg_list){
	while(!CheckTC());
	
	//Rewrite data to buffer
	uint16_t i=vsprintf((char *)txBuff, format, arg_list);
	
	uartHandle->ICR|=USART_ICR_TCCF;
	//Start transmition
	StartTransmition(i);
}

void UART::StartTransmition(uint8_t i){
	#ifdef USE_DCACHE
	SCB_CleanDCache();
	#endif
	
	if(useTxDMA){
		//Restart DMA stream
		dmaTxHandle->StartStream(dmaTxStreamHandle, i);
		//Clear TC Flag
		uartHandle->ICR|= 0x00021B5F;
	} else {
	}
}

uint8_t UART::CheckTC(){
	return (uartHandle->ISR & USART_ISR_TC);
}

UARTReader* UART::CreateReader(){
	UARTReader* reader= NULL;
	
	if(useRxDMA){
		reader= new UARTReader(&(dmaRxStreamHandle->NDTR), rxBuff, rxBuffSize);
	}
	
	return reader;
}
