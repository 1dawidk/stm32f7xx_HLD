#ifndef _MPU_H
#define _MPU_H

#include "stm32f7xx.h"
#include "mpu_region_config.h"

#define   MPU_REGION_SIZE_32B      	((uint8_t)0x04U)
#define   MPU_REGION_SIZE_64B      	((uint8_t)0x05U)
#define   MPU_REGION_SIZE_128B     	((uint8_t)0x06U) 
#define   MPU_REGION_SIZE_256B     	((uint8_t)0x07U) 
#define   MPU_REGION_SIZE_512B     	((uint8_t)0x08U) 
#define   MPU_REGION_SIZE_1KB      	((uint8_t)0x09U)  
#define   MPU_REGION_SIZE_2KB      	((uint8_t)0x0AU)
#define   MPU_REGION_SIZE_4KB      	((uint8_t)0x0BU) 
#define   MPU_REGION_SIZE_8KB      	((uint8_t)0x0CU) 
#define   MPU_REGION_SIZE_16KB     	((uint8_t)0x0DU) 
#define   MPU_REGION_SIZE_32KB     	((uint8_t)0x0EU) 
#define   MPU_REGION_SIZE_64KB     	((uint8_t)0x0FU) 
#define   MPU_REGION_SIZE_128KB    	((uint8_t)0x10U)
#define   MPU_REGION_SIZE_256KB    	((uint8_t)0x11U)
#define   MPU_REGION_SIZE_512KB    	((uint8_t)0x12U)
#define   MPU_REGION_SIZE_1MB      	((uint8_t)0x13U) 
#define   MPU_REGION_SIZE_2MB      	((uint8_t)0x14U) 
#define   MPU_REGION_SIZE_4MB      	((uint8_t)0x15U) 
#define   MPU_REGION_SIZE_8MB      	((uint8_t)0x16U) 
#define   MPU_REGION_SIZE_16MB     	((uint8_t)0x17U)
#define   MPU_REGION_SIZE_32MB     	((uint8_t)0x18U)
#define   MPU_REGION_SIZE_64MB     	((uint8_t)0x19U)
#define   MPU_REGION_SIZE_128MB    	((uint8_t)0x1AU)
#define   MPU_REGION_SIZE_256MB    	((uint8_t)0x1BU)
#define   MPU_REGION_SIZE_512MB    	((uint8_t)0x1CU)
#define   MPU_REGION_SIZE_1GB      	((uint8_t)0x1DU) 
#define   MPU_REGION_SIZE_2GB      	((uint8_t)0x1EU) 
#define   MPU_REGION_SIZE_4GB      	((uint8_t)0x1FU)

#define 	MPU_ENABLE							 	1
#define 	MPU_DISABLE								0

class Mpu{
	public:
		static void enable();
		static void disable();
		static void configRegion(MpuRegionConfig *mpuRegionConfig);
	private:
		static uint8_t no;
};

#endif
