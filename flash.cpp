#include "flash.h"

void Flash::SetLatency(uint8_t latency){
	FLASH->ACR|= (FLASH->ACR&0xFFFFFFF0) | (uint32_t)latency;
}

void Flash::EnableART(){
	FLASH->ACR|= FLASH_ACR_ARTEN;
}

void Flash::EnablePrefetch(){
	FLASH->ACR|= FLASH_ACR_PRFTEN;
}
