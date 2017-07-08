#ifndef _UART_H
#define _UART_H

/**    USAGE
	*	
	*	- Create UART object
	* - Pass UART_TypeDef, Data length, Oversampling, Sampling, Parity, Baudrate,
	*		Rx buffer length, Tx buffer length to Constructor
	* - Use "Init" method passing Pins Segment, Tx pin, Rx pin
	*	- (For DMA Rx usage) Use method "setTxDMAStream" to init Rx DMA passing 
	*											 Dma object, DMA_Stream_TypeDef, DMA Channel
	*	- (For DMA Tx usage) Use method "setTxDMAStream" to init Tx DMA passing 
	*											 Dma object, DMA_Stream_TypeDef, DMA Channel
	* - Enable UART using "enable" method
	*
	*/

#include "HighLayerDrivers/gpio.h"
#include "HighLayerDrivers/dma.h"
#include "rcc.h"
#include "stdarg.h"
#include "stdio.h"

#define UART_OVERSAMPLING_16		0
#define UART_OVERSAMPLING_8			1

#define UART_SAMPLING_3					0
#define UART_SAMPLING_1					1

#define UART_DATALEN_7					0
#define UART_DATALEN_8					1
#define UART_DATALEN_9					2

#define UART_PARITY_NONE				0
#define UART_PARITY_ODD					1
#define UART_PARITY_EVEN				2

class UART{
	public:
		UART(USART_TypeDef *USARTx, uint8_t dataLen, uint8_t oversampling, uint8_t sampling, uint8_t parity, uint16_t baudRate, uint16_t rxBuffSize, uint16_t txBuffSize);
		uint8_t init(GPIO_TypeDef *GPIOx, uint16_t tx_pin, uint16_t rx_pin);
		void setTxDMAStream(DMA *dma, DMA_Stream_TypeDef *stream, uint32_t channel);
		void setRxDMAStream(DMA *dma, DMA_Stream_TypeDef *stream, uint32_t channel);
		void enable();
	
		void enable_rx();
		void disable_rx();
		void enable_tx();
		void disable_tx();
	
		void transmit(char *data);
		void transmit(const char* format, ...);
		void transmit(const char* format, va_list arg_list);
		void start_transmition(uint8_t i);
		uint8_t checkTC();
	private:
		USART_TypeDef *uart_handle;
	
		uint8_t useTxDMA;
		DMA *dmaTx_handle;
		DMA_Stream_TypeDef *dmaTxStream_handle;
	
		uint8_t useRxDMA;
		DMA *dmaRx_handle;
		DMA_Stream_TypeDef *dmaRxStream_handle;
	
		uint16_t clkSrc;
		uint16_t baudrate;
		uint8_t dataLen;
		uint8_t oversampling;
		uint8_t sampling;
		uint8_t parity;
	
		uint8_t *rxBuff;
		uint16_t rxBuffSize;
		uint8_t *txBuff;
		uint16_t txBuffSize;
};

#endif

