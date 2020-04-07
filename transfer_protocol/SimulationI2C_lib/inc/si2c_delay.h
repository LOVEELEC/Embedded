/************************ (C) COPYRIGHT Megahuntmicro *************************
 * File Name            : si2c_delay.h
 * Author               : Megahuntmicro
 * Version              : V1.0.0
 * Date                 : 12/07/2015
 * Description          : Simulation I2C delay API interface file.
 *****************************************************************************/
 
 
#ifndef __SI2C_DELAY_H__
#define __SI2C_DELAY_H__
 
 
#ifdef __cplusplus
extern "C" {
#endif
	
/* Include ------------------------------------------------------------------*/
#include "stm32f4xx.h"
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/	
/* Exported macro -----------------------------------------------------------*/	
/* Exported functions -------------------------------------------------------*/	
void si2c_delay_init(uint8_t SYSCLK);
void si2c_delay_ms(uint16_t nms);
void si2c_delay_us(uint32_t nus);
/* Exported variables -------------------------------------------------------*/	
 
#ifdef __cplusplus
}
#endif	 

#endif	/* __SI2C_DELAY_H__ */

/************************* (C) COPYRIGHT 2018 LOVE_ELEC ******END OF FILE******/

