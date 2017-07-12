#include "sdintf.h"

//Constructor
SDIntf::SDIntf(SDMMC_TypeDef *sdmmc_h, UART *uart, DMA *dma2){
	stateMachine= SDINTF_STATE_OFF;
	this->sdmmc= new SDMMC(sdmmc_h, dma2);
	this->uart= uart;
}

void SDIntf::Init(SDConfig *sdConfig){
	//Setup DMA for SDMMC Fifo
	sdmmc->PrepareDMA();
	
	//Init GPIOs
	Gpio::InitAf(sdConfig->cmdSeg, sdConfig->cmdPin, GPIO_PP, GPIO_AF12, GPIO_NOPULL);
	Gpio::InitAf(sdConfig->ckSeg, sdConfig->ckPin, GPIO_PP, GPIO_AF12, GPIO_NOPULL);
	Gpio::InitAf(sdConfig->dataSeg, sdConfig->d0Pin, GPIO_PP, GPIO_AF12, GPIO_NOPULL);
	
	if(sdConfig->dataBusWidth==SDINTF_BUSWIDTH_4){
		Gpio::InitAf(sdConfig->dataSeg, sdConfig->d1Pin, GPIO_PP, GPIO_AF12, GPIO_NOPULL);
		Gpio::InitAf(sdConfig->dataSeg, sdConfig->d2Pin, GPIO_PP, GPIO_AF12, GPIO_NOPULL);
		Gpio::InitAf(sdConfig->dataSeg, sdConfig->d3Pin, GPIO_PP, GPIO_AF12, GPIO_NOPULL);
	}
	
	busWidth= sdConfig->dataBusWidth;
	
	//Init SD Sense Pin
	Gpio::InitInput(sdConfig->senseSeg, sdConfig->sensePin, GPIO_PULLUP);
	Exti::ConfigLineInterrupt(sdConfig->senseSeg, sdConfig->sensePin, EXTI_EDGE_RISING | EXTI_EDGE_FALLING,
														sdConfig->senseEXTIProirity, sdConfig->senseEXTISubPriority);
	
	Rcc::SetPeriphClkState(RCC_PERIPHCLK_SDMMC1, _ENABLE_);
	Rcc::SetDedicatedClkSrc(RCC_DEDICCLK_SDMMC1, RCC_DEDICCLK_SDMMC1_HCLK);

	stateMachine= SDINTF_STATE_IDLE;
}

void SDIntf::CardDetectHandler(){
	if(Gpio::ReadInput(senseSeg, sensePin)){
		if(stateMachine!=SDINTF_STATE_IDLE)
			OnCardDetach();
	} else {
		if(stateMachine==SDINTF_STATE_IDLE)
			OnCardInsert();
	}
}

uint16_t SDIntf::IsCardReady(){
	if(stateMachine==SDINTF_STATE_CARD_READY)
		return SDINTF_OK;
	else
		return stateMachine;
}

uint16_t SDIntf::MountFAT(uint8_t pId){	
	if(stateMachine==SDINTF_STATE_CARD_READY){
		//Read first sector
		uint8_t r=sdCard->ReadBlock(0, 0, HLD_DEVICECODE_SDINTF);
		
		//Check if read finished successfully
		if(r!=SDMMC_READ_FINISHED_OK)
			return SDINTF_EC_CARD_ERROR;
		
		//Get MBR (first) sector
		uint8_t *mbrSector= (uint8_t *)sdCard->GetDataBuffer(0);
		uint32_t firstLBASec= mbrSector[MBR_PART0_START+16*pId + MBR_PART_LBASTART_OFFSET] |
														mbrSector[MBR_PART0_START+16*pId + MBR_PART_LBASTART_OFFSET + 1]<<8 |
														mbrSector[MBR_PART0_START+16*pId + MBR_PART_LBASTART_OFFSET + 2]<<16 |
														mbrSector[MBR_PART0_START+16*pId + MBR_PART_LBASTART_OFFSET + 3]<<24;
		
		//Create partition object and init FAT partition
		partition= new FATPartition(sdCard,firstLBASec);
		partition->Init();
		
		//Check if FAT partition is VALID
		if(partition->IsValid()){
			delete(partition);
			return SDINTF_EC_MOUNT_ERROR;
		}
		
		return SDINTF_OK;
	}
	
	return SDINTF_EC_UNKNOWN_ERROR;
}

void SDIntf::OnCardDetach(){
	stateMachine= SDINTF_STATE_IDLE;
	delete(sdCard);
	
	//Disable SDMMC CK Pin
	sdmmc->SetPowerSupply(_DISABLE_);
	sdmmc->SetCK(_DISABLE_);
}

void SDIntf::OnCardInsert(){
	uint8_t initState;
	stateMachine= SDINTF_STATE_CARD_DETECTED;

	//Enable SDMMC CK and set 400kHz CK freq
	sdmmc->SetBusWidth(SDMMC_BUSWIDTH_1);
	sdmmc->SetCKFreq(SDMMC_400KHZ_DIV);
	sdmmc->SetPowerSupply(_ENABLE_);
	sdmmc->SetCK(_ENABLE_);	
	
	//Set Data Length and Timeout
	sdmmc->SetBlockSize(SDMMC_BLOCKSIZE_512);
	sdmmc->SetDataLen(512);
	sdmmc->SetTimeout(200000);
	
	//Create SD Card object
	sdCard= new SdCard(sdmmc, uart);
	
	for(uint8_t n=0; n<SDINTF_INIT_TRIES; n++){
		//Init SD Card sequence
		initState= sdCard->Init(busWidth);
		if(initState==SDCARD_EC_WRONGVOLTAGE || initState==SDCARD_OK)
			break;
		
		HLDKernel::delay_ms(100);
	}

	//Check Card init state
	if(!initState){
		stateMachine= SDINTF_STATE_CARD_READY;
	} else{
		stateMachine= SDINTF_STATE_CARD_ERROR;
		sdmmc->SetPowerSupply(_DISABLE_);
		sdmmc->SetCK(_DISABLE_);
	}
}

FATPartition* SDIntf::GetFATPartition(){
	return partition;
}

uint32_t* SDIntf::GetCSD(){
	return sdCard->GetCSD();
}
