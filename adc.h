#ifndef _ADC_H
#define _ADC_H

#include "stm32f7xx.h"
#include "adc_channel.h"
#include "hldkernel.h"
#include "rcc.h"
#include "dma.h"
#include "dma_stream_config.h"

#define ADC_RESOLUTION_6			0x03
#define ADC_RESOLUTION_8			0x02
#define ADC_RESOLUTION_10			0x01
#define ADC_RESOLUTION_12			0x00

#define ADC_TEMP_OFF					0
#define ADC_TEMP_ON						1

#define ADC_V25								0.76
#define ADC_AVG_SLOPE					400
#define ADC_TEMP_SMP					0

class ADConverter{
	public:
		ADConverter(ADC_TypeDef *ADCx);
		void 			Init(uint16_t ref_x10, uint8_t resolution, uint8_t noc, ADC_Channel* chGroup, uint8_t tempSensorEn, DMA *dma2, DMA_Stream_TypeDef *stream);
		uint8_t 	Start();
		void 			Stop();
	
		void 			WakeupTempSensor();
		uint16_t 	GetTemperature();
	private:
		ADC_TypeDef *adcHandle;
		uint16_t *buff;
		float factor;
};

#endif
