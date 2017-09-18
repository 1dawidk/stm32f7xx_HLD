#include "uart_reader.h"
#include "DebugSystem/uds.h"

UARTReader::UARTReader(volatile uint32_t *dma_cnt, uint8_t *uart_buff, uint16_t buff_size){
	this->dmaCnt= dma_cnt;
	this->UARTBuff=uart_buff;
	this->buffSize= buff_size;
}

void UARTReader::SetCurrentAsStart(){
	start= buffSize-(*dmaCnt);
}

int16_t UARTReader::ReadLineAlloc(char** buff){
	uint8_t eol_flag=0;
	uint16_t shift=0;
	uint16_t idx= start+shift;
	
	while( idx!=(*dmaCnt) && eol_flag!=2 ){
		if(UARTBuff[idx]=='\r' || UARTBuff[idx]=='\n')
			eol_flag++;
		else
			eol_flag=0;

		shift++;
		
		idx=start+shift;
		if(idx>=buffSize)
			idx-=buffSize;
	}
	
	if(shift==2){
		start+=2;
		if(start>=buffSize)
			start-=buffSize;
		return 0; //Empty line
	}
	
	if(eol_flag==2){
		*buff= new char[shift-1];
	
		for(uint16_t i=0; i<shift-2; i++){
			if(start+i>=buffSize){
				(*buff)[i]= UARTBuff[start+i-buffSize];
			} else {
				(*buff)[i]= UARTBuff[start+i];
			}
		}
	
		(*buff)[shift-2]=0;
		
		start+= shift+1;
		if(start>=buffSize)
			start-=buffSize;
	
		return shift-2;
	}else
		return -1;
}

int16_t UARTReader::ReadLine(char* buff, uint16_t max_len, UDS *uds){
	uint8_t eol_flag=0;
	uint16_t shift=0;
	uint16_t idx=start+shift;
	
	while( idx!=buffSize-(*dmaCnt) && eol_flag!=2 ){
		if(UARTBuff[idx]=='\r' || UARTBuff[idx]=='\n')
			eol_flag++;
		else
			eol_flag=0;

		shift++;
		
		idx=start+shift;
		if(idx>=buffSize)
			idx-=buffSize;
	}
	
	if(shift==2){
		start+=2;
		if(start>=buffSize)
			start-=buffSize;
		return 0; //Empty line
	}
	
	if(eol_flag==2){
		if(shift-2>max_len){
			eol_flag=3;
			shift= max_len+2;
		}
		
		for(uint16_t i=0; i<shift-2; i++){
			if(start+i>=buffSize){
				buff[i]= UARTBuff[start+i-buffSize];
			} else {
				buff[i]= UARTBuff[start+i];
			}
		}
		
		buff[shift-2]=0;
		
		start+= shift;
		if(start>=buffSize)
			start-=buffSize;
	
		if(eol_flag==3){
			start-=2;
			return -2;	// Only part of line read
		} else{
			return shift-2;	// Full line read
		}
	}else
		return -1;	// No line available
}


char* UARTReader::GetFullBuffer(){
	return (char*)UARTBuff;
}