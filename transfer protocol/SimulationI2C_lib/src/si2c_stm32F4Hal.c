#include "si2c_stm32F4Hal.h"
#include "stm32f4xx.h"

static bool SI2CHAL_DeInit(void *pvGPIOAddr, uint8_t ucClkPos, uint8_t ucpinpos);
static bool SI2CHAL_Init(void *pvGPIOAddr, uint8_t ucClkPos, uint8_t ucpinpos);
static bool SI2CHAL_Write(void *pvGPIOAddr, uint8_t ucpinpos, bool bbitValue);
static bool SI2CHAL_Read(void *pvGPIOAddr, uint8_t ucpinpos);
static void SI2CHAL_SetDir(void *pvGPIOAddr, uint8_t ucpinpos, IODir xdir);

static iSi2cHAL_Struct iSi2cHAL = {
	SI2CHAL_DeInit,
	SI2CHAL_Init,
	SI2CHAL_Write,
	SI2CHAL_Read,
	SI2CHAL_SetDir
};

piSi2cHAL_Struct piSi2cHAL = &iSi2cHAL;


static uint16_t GetIOHalPos(uint8_t pinpos)
{
	return ((uint16_t)(1 << pinpos));
}

static bool SI2CHAL_DeInit(void *pvGPIOAddr, uint8_t ucClkPos, uint8_t ucpinpos)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    
    GPIO_InitStructure.GPIO_Pin = GetIOHalPos(ucpinpos);
	GPIO_Init((GPIO_TypeDef *)pvGPIOAddr,&GPIO_InitStructure);
	GPIO_ResetBits((GPIO_TypeDef *)pvGPIOAddr, GetIOHalPos(ucpinpos));
	return true;
}

static bool SI2CHAL_Init(void *pvGPIOAddr, uint8_t ucClkPos, uint8_t ucpinpos)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_AHB1PeriphClockCmd(GetIOHalPos(ucClkPos), ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    
    GPIO_InitStructure.GPIO_Pin = GetIOHalPos(ucpinpos);
	GPIO_Init((GPIO_TypeDef *)pvGPIOAddr,&GPIO_InitStructure);
	return true;
}

static bool SI2CHAL_Write(void *pvGPIOAddr, uint8_t ucpinpos, bool bbitValue)
{
	GPIO_WriteBit((GPIO_TypeDef *)pvGPIOAddr, GetIOHalPos(ucpinpos), bbitValue);
	return true;
}
	
static bool SI2CHAL_Read(void *pvGPIOAddr, uint8_t ucpinpos)
{
	return (GPIO_ReadInputDataBit((GPIO_TypeDef *)pvGPIOAddr, GetIOHalPos(ucpinpos)));
}

static void SI2CHAL_SetDir(void *pvGPIOAddr, uint8_t ucpinpos, IODir xdir)
{
	GPIO_TypeDef *pxGPIOx = (GPIO_TypeDef *)pvGPIOAddr;
	uint32_t ulMode = GPIO_Mode_OUT;
	pxGPIOx->MODER  &= ~(GPIO_MODER_MODER0 << (ucpinpos * 2));
	if (IODIR_IN == xdir){
		ulMode = GPIO_Mode_IN;
	}	
	pxGPIOx->MODER |= ((ulMode) << (ucpinpos * 2));
}
	
/******************** (C) COPYRIGHT 2018 LOVE_ELEC   *********END OF FILE******/
