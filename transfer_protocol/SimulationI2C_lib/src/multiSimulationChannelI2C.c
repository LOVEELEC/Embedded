/* Include ------------------------------------------------------------------*/
#include "multiSimulationChannelI2C.h"
/* Private define -----------------------------------------------------------*/	

/* SDA GPIO DIR Func def */
#define SI2C_SDA_IN()           piSi2cHAL->SetDir(axi2cHal[channelNum].axSDAHAL.pvGPIOAddr, \
												  axi2cHal[channelNum].axSDAHAL.ucPinPos,	\
												  IODIR_IN);
#define SI2C_SDA_OUT()          piSi2cHAL->SetDir(axi2cHal[channelNum].axSDAHAL.pvGPIOAddr, \
												  axi2cHal[channelNum].axSDAHAL.ucPinPos,	\
												  IODIR_OUT);

/* GPIO Manipulation Function */
#define SI2C_SCL_HIGH()		    piSi2cHAL->Write(axi2cHal[channelNum].axSCLHAL.pvGPIOAddr, 	\
												 axi2cHal[channelNum].axSCLHAL.ucPinPos,	\
												 1);
#define SI2C_SCL_LOW()		    piSi2cHAL->Write(axi2cHal[channelNum].axSCLHAL.pvGPIOAddr, 	\
												 axi2cHal[channelNum].axSCLHAL.ucPinPos,	\
												 0);
#define SI2C_SDA_HIGH()		    piSi2cHAL->Write(axi2cHal[channelNum].axSDAHAL.pvGPIOAddr, 	\
												 axi2cHal[channelNum].axSDAHAL.ucPinPos,	\
												 1);
#define SI2C_SDA_LOW()		    piSi2cHAL->Write(axi2cHal[channelNum].axSDAHAL.pvGPIOAddr, 	\
												 axi2cHal[channelNum].axSDAHAL.ucPinPos,	\
												 0);
#define READ_SDA				piSi2cHAL->Read (axi2cHal[channelNum].axSDAHAL.pvGPIOAddr, 	\
												 axi2cHal[channelNum].axSDAHAL.ucPinPos)
/* Private typedef ----------------------------------------------------------*/
typedef enum _SIMULATION_I2C_FuncExecStatus_Def
{
    SI2C_SUCCESS,
    SI2C_FAIL
}SI2C_FuncExecStatus;

typedef enum _SIMULATION_I2C_ACK_Status_Def
{
    ACK_SUCCESS,
    ACK_FAIL,
}SI2C_ACK_Status;    
    
typedef enum _SIMULATION_I2C_ACKMODE_Def
{
    SEND_ACK,
    NO_ACK,
} SI2C_ACK_Mode_Def;

/* Private macro ------------------------------------------------------------*/	
/* Private variables --------------------------------------------------------*/	
// Áî®Êù•ËÆ∞ÂΩïÊâÄÂàõÂª∫ÁöÑÊ®°ÊãüI2CÁöÑ‰ø°ÊÅØ
static I2CHALStruct axi2cHal[MAX_I2C_CHANNEL_NUM] = {0x00};	
// Áî®Êù•ËÆ∞ÂΩï‰ø°ÊÅØÂùóÁöÑ‰ΩøÁî®ÊÉÖÂÜµ,ÊØè‰∏™bitË°®Á§∫‰∏Ä‰∏™‰ø°ÊÅØÂùóÁöÑÁä∂ÊÄÅ,1:‰ø°ÊÅØÂùóÁîüÊàêÔºõ0:Êó†‰ø°ÊÅØ(Êú™ÂàõÂª∫)
static uint32_t si2cChannelState = 0x00;						
/* Ptivate function prototypes ----------------------------------------------*/	
static int GetFirstZeroBitPos(uint32_t searchData);
static int isActiveChannelNum(int channelNum);
static bool simulation_i2c_isBusy(int channelNum);
static SI2C_FuncExecStatus simulation_i2c_start(int channelNum);
static SI2C_FuncExecStatus simulation_i2c_stop(int channelNum);
static SI2C_ACK_Status simulation_i2c_wait_ack(int channelNum);
static SI2C_ACK_Status simulation_i2c_ack(int channelNum);
static SI2C_FuncExecStatus simulation_i2c_nack(int channelNum);
static SI2C_ACK_Status simulation_i2c_send_byte(int channelNum, uint8_t send_data);
static uint8_t simulation_i2c_receive_byte(int channelNum, SI2C_ACK_Mode_Def ack_mode);

int lmultiSi2c_CreateNewChannel(I2CHALStruct *pi2cHal)
{
	int channelNum = GetFirstZeroBitPos(si2cChannelState);
	/* */
	if (channelNum >= MAX_I2C_CHANNEL_NUM)	return ErrorState;
	memcpy(&axi2cHal[channelNum], pi2cHal, sizeof(I2CHALStruct));
	axi2cHal[channelNum].speed = SMIULATION_I2C_SPEED_100KHZ;
	if (USE_I2C_400KHZ == axi2cHal[channelNum].speed){
		axi2cHal[channelNum].speed = SMIULATION_I2C_SPEED_400KHZ;
	}
	piSi2cHAL->Init(axi2cHal[channelNum].axSDAHAL.pvGPIOAddr, 
					axi2cHal[channelNum].axSDAHAL.ucIOPos, 
					axi2cHal[channelNum].axSDAHAL.ucPinPos);
	piSi2cHAL->Init(axi2cHal[channelNum].axSCLHAL.pvGPIOAddr, 
					axi2cHal[channelNum].axSCLHAL.ucIOPos, 
					axi2cHal[channelNum].axSCLHAL.ucPinPos);
	si2cChannelState |= (1 << channelNum);
	return channelNum;
}

int lmultiSi2c_DestoryChannel(int channelNum)
{
	if (ErrorState == isActiveChannelNum(channelNum))	return ErrorState;
	piSi2cHAL->DeInit(axi2cHal[channelNum].axSDAHAL.pvGPIOAddr, 
					  axi2cHal[channelNum].axSDAHAL.ucIOPos, 
					  axi2cHal[channelNum].axSDAHAL.ucPinPos);
	piSi2cHAL->DeInit(axi2cHal[channelNum].axSCLHAL.pvGPIOAddr, 
					  axi2cHal[channelNum].axSCLHAL.ucIOPos, 
					  axi2cHal[channelNum].axSCLHAL.ucPinPos);
	si2cChannelState &= ~(1 << channelNum);
	return SUCCESS_STATE;
}

int lwrByteRegWithByteData(int channelNum, uint8_t addr, uint8_t Reg, uint8_t Data)
{
	simulation_i2c_start(channelNum);
	simulation_i2c_send_byte(channelNum, (addr & (~1)));
	simulation_i2c_send_byte(channelNum, Reg);
	simulation_i2c_send_byte(channelNum, Data);
	simulation_i2c_stop(channelNum);
	return SUCCESS_STATE;
}

int lrdByteRegWithByteData(int channelNum, uint8_t addr, uint8_t Reg, uint8_t *Data)
{
	uint8_t ucrxData = 0x00;
	simulation_i2c_start(channelNum);
	simulation_i2c_send_byte(channelNum, (addr & (~1)));
	simulation_i2c_send_byte(channelNum, Reg);
	simulation_i2c_stop(channelNum);
	
	simulation_i2c_start(channelNum);
	simulation_i2c_send_byte(channelNum, (addr | (1)));
	*Data = simulation_i2c_receive_byte(channelNum, NO_ACK);
	simulation_i2c_stop(channelNum);
	return SUCCESS_STATE;
}

int lwrHalfWordRegWithHalfWordData(int channelNum, uint8_t addr, uint16_t Reg, uint16_t Data)
{
	simulation_i2c_start(channelNum);
	simulation_i2c_send_byte(channelNum, (addr & (~1)));
	simulation_i2c_send_byte(channelNum, ((Reg >> 8) & 0x00FF));
	simulation_i2c_send_byte(channelNum, (Reg & 0x00FF));
	simulation_i2c_send_byte(channelNum, ((Data >> 8) & 0x00FF));
	simulation_i2c_send_byte(channelNum, (Data & 0x00FF));		
	simulation_i2c_stop(channelNum);
	return SUCCESS_STATE;
}

int lrdHalfWordRegWithHalfWordData(int channelNum, uint8_t addr, uint16_t Reg, uint16_t *Data)
{
	simulation_i2c_start(channelNum);
	simulation_i2c_send_byte(channelNum, (addr & (~1)));
	simulation_i2c_send_byte(channelNum, ((Reg >> 8) & 0x00FF));
	simulation_i2c_send_byte(channelNum, (Reg & 0x00FF));
	simulation_i2c_stop(channelNum);
	
	simulation_i2c_start(channelNum);
	simulation_i2c_send_byte(channelNum, (addr | (1)));
	*Data = (simulation_i2c_receive_byte(channelNum, SEND_ACK) << 8);
	*Data |= simulation_i2c_receive_byte(channelNum, NO_ACK);
	simulation_i2c_stop(channelNum);
	return SUCCESS_STATE;
}

int lwrHalfWordRegWithByteData(int channelNum, uint8_t addr, uint16_t Reg, uint8_t Data)
{
	simulation_i2c_start(channelNum);
	simulation_i2c_send_byte(channelNum, (addr & (~1)));
	simulation_i2c_send_byte(channelNum, ((Reg >> 8) & 0x00FF));
	simulation_i2c_send_byte(channelNum, (Reg & 0x00FF));
	simulation_i2c_send_byte(channelNum, Data);		
	simulation_i2c_stop(channelNum);
	return SUCCESS_STATE;
}

int lrdHalfWordRegWithByteData(int channelNum, uint8_t addr, uint16_t Reg, uint8_t *Data)
{
	simulation_i2c_start(channelNum);
	simulation_i2c_send_byte(channelNum, (addr & (~1)));
	simulation_i2c_send_byte(channelNum, ((Reg >> 8) & 0x00FF));
	simulation_i2c_send_byte(channelNum, (Reg & 0x00FF));
	simulation_i2c_stop(channelNum);
	
	simulation_i2c_start(channelNum);
	simulation_i2c_send_byte(channelNum, (addr | (1)));
	*Data = simulation_i2c_receive_byte(channelNum, NO_ACK);
	simulation_i2c_stop(channelNum);
	return SUCCESS_STATE;
}


static int GetFirstZeroBitPos(uint32_t searchData)
{
	uint8_t bitCount = 0;
	if (searchData == 0xFFFFFFFF)	return ErrorState;
	while (bitCount < MAX_I2C_CHANNEL_NUM){
		if (!(searchData & 0x01)){
			return (bitCount);
		}
		bitCount++;
		searchData >>= 1;
	}
	return bitCount;
}

static int isActiveChannelNum(int channelNum)
{
	if (ErrorState == channelNum){
		return ErrorState;
	}
	if (channelNum > MAX_I2C_CHANNEL_NUM){
		return ErrorState;
	}
	if (!(si2cChannelState & (1 << channelNum))){
		return ErrorState;
	}
	return SUCCESS_STATE;
}


/******************************************************************************
* Function Name  : simulation_i2c_isBusy
* Description    : simulaition i2c Bus Status Detect
* Input          : NONE
* Output         : NONE
* Return         : Results of the simulation I2C BUS Status
******************************************************************************/
static bool simulation_i2c_isBusy(int channelNum)
{ 
	SI2C_SDA_IN(); 
    if(READ_SDA){
        return false;
    }
    return true;
}

/******************************************************************************
* Function Name  : simulation_i2c_start
* Description    : simulaition i2c start function
* Input          : NONE
* Output         : NONE
* Return         : Results of the simulation I2C start function
******************************************************************************/
static SI2C_FuncExecStatus simulation_i2c_start(int channelNum)
{
	if (ErrorState == isActiveChannelNum(channelNum))	return SI2C_FAIL;
    SI2C_SDA_OUT();     //sdaœﬂ ‰≥ˆ
	SI2C_SDA_HIGH();	  	  
	SI2C_SCL_HIGH();
	si2c_delay_us(axi2cHal[channelNum].speed);
 	SI2C_SDA_LOW();//START:when CLK is high,DATA change form high to low 
	si2c_delay_us(axi2cHal[channelNum].speed);
	SI2C_SCL_LOW();//«Ø◊°I2C◊‹œﬂ£¨◊º±∏∑¢ÀÕªÚΩ” ’ ˝æ› 
	si2c_delay_us(axi2cHal[channelNum].speed);
    return SI2C_SUCCESS;
}

/******************************************************************************
* Function Name  : simulation_i2c_stop
* Description    : simulaition i2c stop function
* Input          : NONE
* Output         : NONE
* Return         : Results of the simulation I2C stop function
******************************************************************************/
static SI2C_FuncExecStatus simulation_i2c_stop(int channelNum)
{
	if (ErrorState == isActiveChannelNum(channelNum))	return SI2C_FAIL;
    SI2C_SDA_OUT();     //sdaœﬂ ‰≥ˆ
    /* ∑¿÷πœﬂ…œ◊¥Ã¨≤ª∂‘£¨‘Ï≥…≤ªƒ‹Stop */
    SI2C_SCL_LOW();
	SI2C_SDA_LOW();	  	//STOP:when CLK is high DATA change form low to high  
	SI2C_SCL_HIGH();
	si2c_delay_us(axi2cHal[channelNum].speed);
    /* Õ£÷π */
	SI2C_SDA_HIGH();    //∑¢ÀÕI2C◊‹œﬂΩ· ¯–≈∫≈
    si2c_delay_us(axi2cHal[channelNum].speed);
	SI2C_SCL_HIGH();
	si2c_delay_us(axi2cHal[channelNum].speed);
    return SI2C_SUCCESS;
}

/******************************************************************************
* Function Name  : simulation_i2c_wait_ack
* Description    : simulatipn i2c wait ack function
* Input          : NONE
* Output         : NONE
* Return         : ACK response results
******************************************************************************/
static SI2C_ACK_Status simulation_i2c_wait_ack(int channelNum)
{
    uint8_t ucErrTime=0;
	if (ErrorState == isActiveChannelNum(channelNum))	return ACK_FAIL;
    SI2C_SCL_LOW();          // ±÷” ‰≥ˆ0 
    si2c_delay_us(1);	
	SI2C_SDA_OUT();
	SI2C_SDA_HIGH();
    SI2C_SDA_IN();      //SDA…Ë÷√Œ™ ‰»Î  
    si2c_delay_us(axi2cHal[channelNum].speed - 1);	   
	SI2C_SCL_HIGH();
    si2c_delay_us(axi2cHal[channelNum].speed);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			simulation_i2c_stop(channelNum);
			return ACK_FAIL;
		}
	}
	SI2C_SCL_LOW();          // ±÷” ‰≥ˆ0 	  
    return ACK_SUCCESS;
}

/******************************************************************************
* Function Name  : simulation_i2c_ack
* Description    : simulation i2c ack function 
* Input          : NONE
* Output         : NONE 
* Return         : ACK response to the execution result
******************************************************************************/
static SI2C_ACK_Status simulation_i2c_ack(int channelNum)
{
	if (ErrorState == isActiveChannelNum(channelNum))	return ACK_FAIL;
    SI2C_SCL_LOW();
    si2c_delay_us(1);
	SI2C_SDA_OUT();
	SI2C_SDA_LOW();
	si2c_delay_us(axi2cHal[channelNum].speed - 1);
	SI2C_SCL_HIGH();
	si2c_delay_us(axi2cHal[channelNum].speed);
	SI2C_SCL_LOW();
//	si2c_delay_us(SmiuI2c_Speed);
//	SI2C_SDA_HIGH();
    return ACK_SUCCESS;
}

/******************************************************************************
* Function Name  : simulation_i2c_nack
* Description    : simulation i2c nack function 
* Input          : NONE
* Output         : NONE 
* Return         : NACK response to the execution result
******************************************************************************/
static SI2C_FuncExecStatus simulation_i2c_nack(int channelNum)
{
	if (ErrorState == isActiveChannelNum(channelNum))	return SI2C_FAIL;
    SI2C_SCL_LOW();
	si2c_delay_us(1);
    SI2C_SDA_OUT();
	SI2C_SDA_HIGH();
	si2c_delay_us(axi2cHal[channelNum].speed - 1);
	SI2C_SCL_HIGH();
	si2c_delay_us(axi2cHal[channelNum].speed);
	SI2C_SCL_LOW();
    return SI2C_SUCCESS;
}

/******************************************************************************
* Function Name  : simulation_i2c_send_byte
* Description    : simulation i2c send byte function
* Input          : send_data:Data to be transmitted
* Output         : NONE
* Return         : Results of the simulation I2C send byte function
******************************************************************************/
static SI2C_ACK_Status simulation_i2c_send_byte(int channelNum, uint8_t send_data)
{
    uint8_t t;   
	if (ErrorState == isActiveChannelNum(channelNum))	return ACK_FAIL;
	SI2C_SDA_OUT(); 	    
    for(t = 0; t < 8; t++)
    {   
        SI2C_SCL_LOW();	  
        si2c_delay_us(1);          
        if ((send_data & 0x80) >> 7)
        {
            SI2C_SDA_HIGH();	
        }else{
            SI2C_SDA_LOW();
        }
        send_data <<= 1; 	  
		si2c_delay_us(axi2cHal[channelNum].speed - 1);   
		SI2C_SCL_HIGH();
		si2c_delay_us(axi2cHal[channelNum].speed); 
//		SI2C_SCL_LOW();	
//		si2c_delay_us(SmiuI2c_Speed);
    }
    SI2C_SCL_LOW();	
    si2c_delay_us(axi2cHal[channelNum].speed);
    return simulation_i2c_wait_ack(channelNum);
}

/******************************************************************************
* Function Name  : simulation_i2c_send_byte
* Description    : simulation i2c receive byte function
* Input          : NONE
* Output         : NONE
* Return         : received data
******************************************************************************/
static uint8_t simulation_i2c_receive_byte(int channelNum, SI2C_ACK_Mode_Def ack_mode)
{
    uint8_t rx_data = 0, i;
	SI2C_SDA_IN();//SDA…Ë÷√Œ™ ‰»Î
    for(i = 0; i < 8; i++ )
	{
        SI2C_SCL_LOW(); 
        si2c_delay_us(axi2cHal[channelNum].speed);
		SI2C_SCL_HIGH();
        si2c_delay_us(axi2cHal[channelNum].speed);
        rx_data <<= 1;
        if(READ_SDA)
            rx_data++; 
    }	
    if (ack_mode != SEND_ACK)
        simulation_i2c_nack(channelNum);//∑¢ÀÕnACK
    else
        simulation_i2c_ack(channelNum); //∑¢ÀÕACK 
    return rx_data;
}

/*********************** (C) COPYRIGHT 2018 LOVE_ELEC   ******END OF FILE******/
