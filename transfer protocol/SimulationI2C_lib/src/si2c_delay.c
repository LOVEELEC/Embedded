/************************ (C) COPYRIGHT Megahuntmicro *************************
 * File Name            : si2c_delay.c
 * Author               : Megahuntmicro
 * Version              : V1.0.0
 * Date                 : 12/07/2015
 * Description          : Simulation I2C delay function file.
 *****************************************************************************/
 
/* Include ------------------------------------------------------------------*/
#include "si2c_delay.h"
/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/	
#define SI2C_SYSCLK_MHZ					(120)
#define SI2C_INSCYC_MACCYC				(1)		// 机器周期与时钟周期之比
/* Private macro ------------------------------------------------------------*/	
/* Private variables --------------------------------------------------------*/	
static uint8_t  fac_us = 0;	//us延时计算因子
static uint16_t fac_ms = 0;	//ms延时计算因子
/* Ptivate function prototypes ----------------------------------------------*/	

/******************************************************************************
* Function Name  : si2c_delay_init
* Description    : simulation i2c delay initialization function
* Input          : NONE
* Output         : NONE
* Return         : NONE
******************************************************************************/
void si2c_delay_init(uint8_t SYSCLK)
{
	
}

/******************************************************************************
* Function Name  : si2c_delay_ms
* Description    : simulation i2c delay ms function
* Input          : nms:delay time(unit:ms)
* Output         : NONE
* Return         : NONE
******************************************************************************/
void si2c_delay_ms(uint16_t nms)
{	 		  	  
	si2c_delay_us(1000 * nms); 	    
}   

/******************************************************************************
* Function Name  : si2c_delay_us
* Description    : simulation i2c delay us function
* Input          : nus:delay time(unit:us)
* Output         : NONE
* Return         : NONE
******************************************************************************/		    								   
void si2c_delay_us(const uint32_t nus)
{		 
	uint32_t count = 0;
	const uint32_t utime = (SI2C_SYSCLK_MHZ * nus / (7 * SI2C_INSCYC_MACCYC));
	do
	{
		if ( ++count > utime ){
		  return ;
		}
	}
	while (1);
}

/************************* (C) COPYRIGHT 2018 LOVE_ELEC ******END OF FILE******/

