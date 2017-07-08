#include "adc.h"

ADConverter::ADConverter(ADC_TypeDef *ADCx){
	this->adc_handle=ADCx;
}

void ADConverter::Init(uint16_t ref_x10, uint8_t resolution, uint8_t noc, ADC_Channel* chGroup,
												uint8_t tempSensorEn, DMA *dma2, DMA_Stream_TypeDef *stream){
	uint8_t cnt=0; //Channel counter
	this->factor = (float)ref_x10/0xFFF;
	this->factor/= 10;


	//Set APB2 Prescaler for ADCCLK
	ADC->CCR= 0x01<<16;
	
	//Enable ADC Periph Clk
	if(adc_handle==ADC1)
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_ADC1, ENABLE);
	else if(adc_handle==ADC2)
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_ADC2, ENABLE);
	else
		Rcc::SetPeriphClkState(RCC_PERIPHCLK_ADC3, ENABLE);
	
	//Init ADC
	adc_handle->CR1|= (((uint32_t)resolution)<<24) | ADC_CR1_SCAN;
	adc_handle->CR2|= ADC_CR2_DMA | ADC_CR2_DDS | ADC_CR2_CONT | ADC_CR2_ADON;
	
	if(tempSensorEn){
		//Init temperature sensor
		adc_handle->SMPR1|= ((uint32_t)ADC_CHANNEL_SMP_480)<<24;
		adc_handle->SQR3|=18; //Temp sensor conversion always first (buff[0])
		cnt++;
	}
	
	for(uint8_t i=0; i<noc; i++){ //Init channels
		//Set sampling rate
		if(chGroup[i].no>9)
			adc_handle->SMPR1|= ((uint32_t)chGroup[i].smp)<< ((chGroup[i].no-10)*3);
		else
			adc_handle->SMPR2|= ((uint32_t)chGroup[i].smp) << (chGroup[i].no*3);
		
		//Set convesrion queue position
		if(cnt<6){
			adc_handle->SQR3|= ((uint32_t)chGroup[i].no) << (cnt*4);
		} else if(cnt<12){
			adc_handle->SQR2|= ((uint32_t)chGroup[i].no) << ((cnt-6)*4);
		} else {
			adc_handle->SQR1|= ((uint32_t)chGroup[i].no) << ((cnt-12)*4);
		}
		cnt++;
	}
	
	if(tempSensorEn)
		noc++;
	
	buff= new uint16_t[noc];	
	
	//Init DMA
	DMAStreamConfig dmaAdcStream;
	dmaAdcStream.ch= DMA_CHANNEL_0;
	dmaAdcStream.direction= DMA_DIRECTION_PER_MEM;
	dmaAdcStream.circural=DMA_CIRCULAR_ENABLE;
	
	//Dma mem
	dmaAdcStream.memInc= DMA_INC_ENABLE;
	dmaAdcStream.memDataSize= DMA_DATASIZE_HALFWORD;
	dmaAdcStream.memoryAdr= (uint32_t)(buff);
	
	//Dma periph
	dmaAdcStream.periphInc= DMA_INC_DISABLE;
	dmaAdcStream.periphDataSize= DMA_DATASIZE_HALFWORD;
	dmaAdcStream.periphAdr= (uint32_t)(&(adc_handle->DR));
	
	dmaAdcStream.priority= DMA_PRIORITY_MEDIUM;

	dma2->registerStream(stream, &dmaAdcStream);
	dma2->startStream(stream, noc);
}

uint8_t ADConverter::Start(){
	adc_handle->CR2|= ADC_CR2_SWSTART;
	
	HLDKernel::delay_ms(10);
	if(adc_handle->SR & (1<<4))
		return 0;
	else
		return 1;
}

void ADConverter::Stop(){
	adc_handle->CR2&= ~ADC_CR2_ADON;
}

void ADConverter::WakeupTempSensor(){
	ADC->CCR|= ADC_CCR_TSVREFE; //Wakeup temp sensor
	HLDKernel::delay_ms(10);
}

uint16_t ADConverter::GetTemperature(){
	#ifdef USE_DCACHE
	SCB_CleanInvalidateDCache_by_Addr((uint32_t *)buff, 2);
	#endif
	return ((((float)buff[0]*factor)-ADC_V25)*ADC_AVG_SLOPE)+25;
	
	/*
		sense= 989
		ref= 33
		
		factor= (33/4095)/10= 0,0008
		Vsense= sense*factor= 0,79
		0,03/2,5
	*/
}
