
#ifndef __MULTISIMULATIONCHANNELI2C_H__
#define __MULTISIMULATIONCHANNELI2C_H__
 
 
#ifdef __cplusplus
extern "C" {
#endif
	
/* Include ------------------------------------------------------------------*/
#include "si2c_config.h"  
/* Private define -----------------------------------------------------------*/		
#define MAX_I2C_CHANNEL_NUM						(16)	
#define SMIULATION_I2C_SPEED_100KHZ             (5)          // 10us()
#define SMIULATION_I2C_SPEED_400KHZ             (2)          // 2.5us
/* Exported types -----------------------------------------------------------*/
typedef enum _FUNCSTATE_DEF{
	ErrorState = -1,
	SUCCESS_STATE = 0,
} FUNCSTATE;
typedef enum _SIMULATION_I2C_SPEED_Def
{
    USE_I2C_100KHZ,
    USE_I2C_400KHZ
} SIMULATION_I2C_SPEED;

typedef struct _IOHALStruct_Def{
	void *pvGPIOAddr;
	uint8_t ucIOPos;
	uint8_t ucPinPos;
} IOHALStruct, *pIOHALStruct;
	
typedef struct _I2CHALStruct_Def{
	IOHALStruct axSDAHAL;
	IOHALStruct axSCLHAL;
	SIMULATION_I2C_SPEED speed;
} I2CHALStruct, *pI2CHALStruct;
/* Exported constants -------------------------------------------------------*/	
/* Exported macro -----------------------------------------------------------*/	
/* Exported functions -------------------------------------------------------*/	
int lmultiSi2c_CreateNewChannel(I2CHALStruct *pi2cHal);
int lmultiSi2c_DestoryChannel(int channelNum);
int lwrByteRegWithByteData(int channelNum, uint8_t addr, uint8_t Reg, uint8_t Data);
int lrdByteRegWithByteData(int channelNum, uint8_t addr, uint8_t Reg, uint8_t *Data);
int lwrHalfWordRegWithHalfWordData(int channelNum, uint8_t addr, uint16_t Reg, uint16_t Data);
int lrdHalfWordRegWithHalfWordData(int channelNum, uint8_t addr, uint16_t Reg, uint16_t *Data);
int lwrHalfWordRegWithByteData(int channelNum, uint8_t addr, uint16_t Reg, uint8_t Data);
int lrdHalfWordRegWithByteData(int channelNum, uint8_t addr, uint16_t Reg, uint8_t *Data);
/* Exported variables -------------------------------------------------------*/	

	
	 
#ifdef __cplusplus
}
#endif	 

#endif	/* __MULTISIMULATIONCHANNELI2C_H__ */
/*********************** (C) COPYRIGHT 2018 LOVE_ELEC   ******END OF FILE******/

