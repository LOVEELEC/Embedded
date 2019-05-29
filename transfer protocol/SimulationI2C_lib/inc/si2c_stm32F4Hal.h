
#ifndef __SI2C_STM32F4HAL_H__
#define __SI2C_STM32F4HAL_H__
 
#ifdef __cplusplus
extern "C" {
#endif
	
#include "stdint.h"
#include "stdbool.h"

typedef enum _IODir_Def{
	IODIR_IN = 1,
	IODIR_OUT = 2,
} IODir;


/* 为适用不同硬件平台故采用函数指针的方式操作硬件接口 */
typedef struct _iSi2cHAL_Struct_Def{
	bool (*DeInit)(void *pvGPIOAddr, uint8_t ucClkPos, uint8_t ucpinpos);
	bool (*Init)(void *pvGPIOAddr, uint8_t ucClkPos, uint8_t ucpinpos);
	bool (*Write)(void *pvGPIOAddr, uint8_t ucpinpos, bool bbitValue);
	bool (*Read)(void *pvGPIOAddr, uint8_t ucpinpos);
	void (*SetDir)(void *pvGPIOAddr, uint8_t ucpinpos, IODir xdir);
}iSi2cHAL_Struct, *piSi2cHAL_Struct;

extern piSi2cHAL_Struct piSi2cHAL;
#ifdef __cplusplus
}
#endif	 

#endif	/* __SI2C_STM32F4HAL_H__ */
/******************** (C) COPYRIGHT 2018 LOVE_ELEC   *********END OF FILE******/
