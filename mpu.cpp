#include "mpu.h"

uint8_t Mpu::no=0;

void Mpu::enable(){
	MPU->CTRL|= 0x0001;
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
	
	__DSB();
	__ISB();
}

void Mpu::disable(){
	__DMB();
	SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;
  MPU->CTRL = 0;
}

void Mpu::configRegion(MpuRegionConfig *mpuRegionConfig){
	MPU->RNR= no & 0x000f;
	
	MPU->RBAR= mpuRegionConfig->baseAddress & (0xFFFFFFA0);
	MPU->RASR|= 0x01 | (mpuRegionConfig->shareable<<MPU_RASR_SIZE_Pos);
	
	no++;
}