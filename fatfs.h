#ifndef _FATFS_H
#define _FATFS_H

#include "stm32f7xx.h"
#include "HighLayerDrivers\sdmmc.h"

#define FATFS_OK								0
#define FATFS_ERROR							1

#define FATFS_BOOT_SECTOR				0

#define FATFS_BS_JMPBOOT				0
#define FATFS_BS_OEMNAME				3
#define FATFS_BPB_BYTSPERSEC		11
#define FATFS_BPB_SECPERCLUS		13
#define FATFS_BPB_RSVDSECCNT		14
#define FATFS_BPB_NUMFATS				16
#define FATFS_BPB_ROOTENTCNT		17
#define FATFS_BPB_TOTSEC16			19
#define FATFS_BPB_MEDIA					21
#define FATFS_BPB_FATSZ16				22
#define FATFS_BPB_SECPERTRK			24
#define FATFS_BPB_NUMHEADS			26
#define FATFS_BPB_HIDDSEC				28
#define FATFS_BPB_TOTSEC32			32
#define FATFS_BPB_FATSZ32				36
#define FATFS_BPB_EXTFLAGS			40
#define FATFS_BPB_FSVER					42
#define FATFS_BPB_ROOTCLUS			44
#define FATFS_BPB_FSINFO				48
#define FATFS_BPB_BKBOOTSEC			50
#define FATFS_BS_DRVNUM					64
#define FATFS_BS_BOOTSIG				66
#define FATFS_BS_VOLID					67
#define FATFS_BS_VOLLAB					71
#define FATFS_BS_FILSYSTYPE			82

#define FATFS_BS_511_VAL				0xAA
#define FATFS_BS_510_VAL				0x55

class SdCard;

class FATfs{
	public:
		FATfs(SdCard *sdCard);
		uint8_t Init();
	private:
		SdCard *sdCard;
		uint8_t blockBuffer[512];
};

#endif
