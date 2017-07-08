#ifndef _I2C_DEF_H
#define _I2C_DEF_H

#define I2C_SPEED_STANDARD			0
#define I2C_SPEED_FAST					1
#define I2C_SPEED_FAST_PLUS			2

#define I2C_ADDRESSING_7_BIT		0
#define I2C_ADDRESSING_10_BIT		1

#define I2C_STD_PRESC						0x2
#define I2C_STD_SCLL						0x68
#define I2C_STD_SCLH						0x47
#define I2C_STD_SDADEL					0x2
#define I2C_STD_SCLDEL					0x0

#define I2C_FS_PRESC						0x6
#define I2C_FS_SCLL							0xD
#define I2C_FS_SCLH							0x3
#define I2C_FS_SDADEL						0x0
#define I2C_FS_SCLDEL						0x0

#define I2C_FSP_PRESC						0x0
#define I2C_FSP_SCLL						0x22
#define I2C_FSP_SCLH						0x9
#define I2C_FSP_SDADEL					0x0
#define I2C_FSP_SCLDEL					0x2

#define I2C_AF_ON								0x0
#define I2C_AF_OFF							0x1

#define I2C_DNF_DISABLE					0x0
#define I2C_DNF_1								0x1
#define I2C_DNF_2								0x2
#define I2C_DNF_3								0x3
#define I2C_DNF_4								0x4
#define I2C_DNF_5								0x5
#define I2C_DNF_6								0x6
#define I2C_DNF_7								0x7
#define I2C_DNF_8								0x8
#define I2C_DNF_9								0x9
#define I2C_DNF_10							0xA
#define I2C_DNF_11							0xB
#define I2C_DNF_12							0xC
#define I2C_DNF_13							0xD
#define I2C_DNF_14							0xE
#define I2C_DNF_15							0xF


#endif
