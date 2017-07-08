#include "sdintf.h"

//Constructor
SDIntf::SDIntf(SDMMC_TypeDef *sdmmc_h, UART *uart, DMA *dma2){
	stateMachine= State::Off;
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

	stateMachine= State::Idle;
}

void SDIntf::CardDetectHandler(){
	if(Gpio::ReadInput(senseSeg, sensePin)){
		if(stateMachine!=State::Idle)
			OnCardDetach();
	} else {
		if(stateMachine==State::Idle)
			OnCardInsert();
	}
}

uint16_t SDIntf::IsCardReady(){
	if(stateMachine==State::CardReady)
		return 1;
	else
		return 0;
}

uint16_t SDIntf::MountFAT(uint8_t pId){	
	if(stateMachine==State::CardReady){
		//Read first sector
		uint8_t r=sdCard->ReadBlock(0, 0, 0);
		
		//Check if read finished successfully
		if(r!=SDMMC_READ_FINISHED_OK)
			return SDINTF_EC_CARD_ERROR;
		
		//Get MBR (first) sector
		uint8_t *mbrSector= (uint8_t *)sdCard->GetDataBuffer(0);
		
		//Create partition object and init FAT partition
		partition= new FATPartition(sdCard, mbrSector[MBR_PART0_START+16*pId + MBR_PART_LBASTART_OFFSET]);
		partition->Init();
		
		//Check if FAT partition is VALID
		if(!partition->IsValid()){
			delete(partition);
			return SDINTF_EC_MOUNT_ERROR;
		}
		
		return SDINTF_OK;
	}
	
	return SDINTF_EC_UNKNOWN_ERROR;
}

void SDIntf::OnCardDetach(){
	stateMachine= State::Idle;
	delete(sdCard);
	
	//Disable SDMMC CK Pin
	sdmmc->SetPowerSupply(_DISABLE_);
	sdmmc->SetCK(_DISABLE_);
}

void SDIntf::OnCardInsert(){
	uint8_t initState;
	stateMachine= State::CardDetected;

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
	
	for(uint8_t n=0; n<SD_INIT_N; n++){
		//Init SD Card sequence
		initState= sdCard->Init(busWidth);
		if(initState==SDCARD_EC_WRONGVOLTAGE || initState==SDCARD_OK)
			break;
		
		HLDKernel::delay_ms(100);
	}

	//Check Card init state
	if(!initState){
		stateMachine= State::CardReady;
	} else{
		stateMachine= State::CardError;
		sdmmc->SetPowerSupply(_DISABLE_);
		sdmmc->SetCK(_DISABLE_);
	}
}

uint32_t* SDIntf::GetCSD(){
	return sdCard->GetCSD();
}
