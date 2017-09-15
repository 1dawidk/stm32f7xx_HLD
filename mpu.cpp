#include "mpu.h"

uint8_t Mpu::no=0;

void Mpu::Enable(){
	MPU->CTRL|= 0x0001;
	SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
	
	__DSB();
	__ISB();
}

void Mpu::Disable(){
	__DMB();
	SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;
  MPU->CTRL = 0;
}

void Mpu::ConfigRegion(MpuRegionConfig *mpu_region_config){
	MPU->RNR= no & 0x000f;
	
	MPU->RBAR= mpu_region_config->baseAddress & (0xFFFFFFA0);
	MPU->RASR|= 0x01 | (mpu_region_config->shareable<<MPU_RASR_SIZE_Pos);
	
	no++;
}