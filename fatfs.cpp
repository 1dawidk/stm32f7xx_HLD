#include "fatfs.h"
#include "SdCard.h"

FATfs::FATfs(SdCard *sdCard){
	this->sdCard= sdCard;
}
