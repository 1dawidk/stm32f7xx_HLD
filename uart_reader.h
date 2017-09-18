#ifndef UART_READER_H
#define UART_READER_H

#include "stm32f7xx.h"

class UDS;

class UARTReader {
	public:
	/* Inicjalizacja zmiennych readera */
		UARTReader(volatile uint32_t *dma_cnt, uint8_t *uart_buff, uint16_t buff_size);
	 
	/* Ustawia [start] na [dmaCnt]*/
		void SetCurrentAsStart();
	
	/* Czyta ciag znaków az do napotkania \r\n albo gdy [start]==[dmaCnt]
		 Dla [start]==[dmaCnt] zwraca -1
		 Dla znalezionego \r\n zwraca ilosc znaków od [start] do \r\n (bez \r\n) */
		int16_t ReadLineAlloc(char** buff);
		int16_t ReadLine(char* buff, uint16_t max_len, UDS *uds);
	
	
	/* DEBUG */
		char* GetFullBuffer();
	
	private:
		volatile uint32_t *dmaCnt;
		volatile uint8_t	*UARTBuff;
		uint16_t					buffSize;
		volatile uint16_t start;
};

#endif
