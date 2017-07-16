#ifndef _FATPARTITION_H
#define _FATPARTITION_H

#include "stm32f7xx.h"
#include "sdCard.h"
#include "fatFile.h"
#include <string.h>

#define FAT_OK							0
#define FAT_EC_UNKNOWN			0x1581
#define FAT_EC_DAMAGED			0x1582
#define FAT_EC_NOFILE				0x1583
#define FAT_EC_INVALID_PATH	0x1584
#define FAT_EC_EOC					0x1585
#define FAT_EC_BAD_SECTOR		0x1586
#define FAT_EC_NOTTHESAME		0x1507

//Boot sector offsets
#define FAT_BPB_BYTSPERSEC_Offset		11
#define FAT_BPB_SECPERCLUS_Offset		13
#define FAT_BPB_RSVDSECCNT_Offset		14
#define FAT_BPB_NUMFATS_Offset			16
#define FAT_BPB_TOTSEC32_Offset			32
#define FAT_BPB_FATSZ32_Offset			36

//FAT entry offsets
#define FAT_DIR_SHORTNAME_Offset		0
#define FAT_DIR_ATTR_Offset					11
#define FAT_DIR_CRTTIMETENTH_Offset	13
#define	FAT_DIR_CRTTIME_Offset			14
#define FAT_DIR_CRTDATE_Offset			16
#define FAT_DIR_LSTACCDATE_Offset		18
#define FAT_DIR_FSTCLUSHI_Offset		20
#define FAT_DIR_WRTTIME_Offset			22
#define FAT_DIR_WRTDATE_Offset			24
#define FAT_DIR_FSTCLUSLO_Offset		26
#define FAT_DIR_FILESIZE_Offset			28

#define FAT_DIR_LAST_FLAG						0x00
#define FAT_DIR_FREE_FLAG						0xe5

#define FAT_ATTR_READ_ONLY					0x01
#define FAT_ATTR_HIDDEN							0x02
#define FAT_ATTR_SYSTEM							0x04
#define FAT_ATTR_VOLUME_ID					0x08
#define FAT_ATTR_DIRECTORY					0x10
#define FAT_ATTR_ARCHIVE						0x20
#define FAT_ATTR_LONG_NAME					0x0F

//Cluster entry values
#define FAT_CLUSTER_EMPTY						0x00000000
#define FAT_CLUSTER_RSVD_INT				0x00000001
#define FAT_CLUSTER_USED_L					0x00000002
#define FAT_CLUSTER_USED_H					0x0FFFFFEF
#define FAT_CLUSTER_BAD							0x0FFFFFF7
#define FAT_CLUSTER_EOC_L						0x0FFFFFF8
#define FAT_CLUSTER_EOC_H						0x0FFFFFFF

#define FAT_FILETYPE_DIRECTORY			0x01
#define FAT_FILETYPE_FILE						0x02


class FATPartition{
	public:
		//Init
		FATPartition(SdCard *sdCard, uint32_t firstSectorLBA);
		~FATPartition();
		uint16_t Init();
		uint16_t IsValid();
	
		//File handling
		uint16_t FindFile(FATFile *file);
		uint16_t CreateFile(char *path, char *name, uint8_t type);
		uint16_t SetFileSize(FATFile *file);
		uint8_t* GetFileBuff(int8_t idx);
		uint16_t ReadFileSec(uint32_t sec, int8_t *buffId);
		uint16_t WriteFileBytes(uint32_t sec, uint16_t offset, uint8_t *nBuff, uint16_t len);
		
		uint32_t GetFirstCluSec(uint32_t clu);
	private:
		SdCard *sdCard;
		
		//Find file submethods
		uint16_t 	FindFileDataClu(char *subPath, uint8_t dir, uint32_t *entryClu, uint16_t *offset, uint32_t *dataClu);
		uint8_t 	GetPathDepth(char *path);
		uint16_t	IsPathValid(char *path);
		uint16_t	CompareShortName(char *subPath, uint8_t dir, uint16_t offset);
		uint16_t 	CompareLongName(char *subPath, uint8_t dir, uint32_t *clu, uint16_t *offset);
	
		uint16_t	GetNextCluster(uint32_t clu, uint32_t *nextClu);
	
		uint8_t		*fatBuff;
		uint8_t		*dataBuff;
		uint8_t		*filesBuff[4];
		uint8_t		buffEmptyDirtyFlags;
		uint32_t	onFilesBuffSec[4];
		uint32_t 	onFATBuffSec;
		uint32_t	onDataBuffSec;
		uint32_t	firstSecLBA;
		uint32_t	fatStartSec;
		uint32_t	dataStartSec;
		uint8_t		numFATs;
		uint8_t 	secPerClu;
};

#endif
