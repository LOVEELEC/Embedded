	该simulation i2c模块实现的是常用的模拟I2C的功能，但是支持动态创建、删除且可同时支持最多32路模拟I2C(目前只开放16组)。
	文档结构如下:
		src 
			multiSimulationChannelI2C.c 包含的是最终面向用户的API实现
			si2c_delay.c 包含的是模拟I2C所用到的延时接口实现
			si2c_stm32F4Hal.c 该文件是面向具体平台的API接口实现(面向stm32F4平台的硬件接口)
		inc
			multiSimulationChannelI2C.h 包含的是最终面向用户的API声明
			si2c_delay.h 包含的是模拟I2C所用到的延时接口定义
			si2c_Hal.h 该文件是面向平台的API接口定义
			si2c_config.h 包含的是模拟i2c所需的配置
	使用示例(stm32F4平台示例)：
		宏定义通道所用物理引脚(可选)：
			#define SI2C_TEST_SDA_GPIO_PORT           	GPIOC
			#define SI2C_TEST_SDA_GPIO_PORT_POS         (2)	   // GPIOA:0 GPIOB:1 GPIOC:2 ......
			#define SI2C_TEST_SDA_PINPOS                (12)   // 对应引脚的偏移

			#define SI2C_TEST_SCL_GPIO_PORT           	GPIOB
			#define SI2C_TEST_SCL_GPIO_PORT_POS         (1)	// GPIOA:0 GPIOB:1 GPIOC:2 ......
			#define SI2C_TEST_SCL_PINPOS                (10)
		创建模拟i2c通道：
			int I2CChannelNum = 0;		// 用于保存创建的通道信息块的位置
			I2CHALStruct i2chalStruct;
			i2chalStruct.axSDAHAL.pvGPIOAddr = BTP_SCCB_SDA_GPIO_PORT;
			i2chalStruct.axSDAHAL.ucIOPos = BTP_SCCB_SDA_GPIO_PORT_POS;
			i2chalStruct.axSDAHAL.ucPinPos = BTP_SCCB_SDA_PINPOS;
			
			i2chalStruct.axSCLHAL.pvGPIOAddr = BTP_SCCB_SCL_GPIO_PORT;
			i2chalStruct.axSCLHAL.ucIOPos = BTP_SCCB_SCL_GPIO_PORT_POS;
			i2chalStruct.axSCLHAL.ucPinPos = BTP_SCCB_SCL_PINPOS;
			
			i2chalStruct.speed = USE_I2C_100KHZ;
			
			I2CChannelNum = lmultiSi2c_CreateNewChannel(&i2chalStruct);
			if (ErrorState == I2CChannelNum){
				return (false);
			}
		发送数据：
			lwrByteRegWithByteData(I2CChannelNum, DEVWR_ADDR, Reg, Data);	// 写入一个字节内容到一个8bit地址区域
			其中：
				I2CChannelNum： 创建模拟通道时生成的信息块位置
				DEVWR_ADDR：目标设备的地址
				Reg： 目标设备要操作的寄存器地址
				Data：要写入的数据
		读取数据：
			lrdByteRegWithByteData(I2CChannelNum, DEVRD_ADDR, Reg, pData); // 从一个8bit地址区域读取一个字节内容	
			其中：
				I2CChannelNum： 创建模拟通道时生成的信息块位置
				DEVWR_ADDR：目标设备的地址
				Reg： 目标设备要操作的寄存器地址
				pData：读取数据的存入位置


	平台移植：
		移植需要修改的地方如下：
			si2c_delay.c中的宏定义：
				#define SI2C_SYSCLK_MHZ					(120)	// 系统主频(MHz)
				#define SI2C_INSCYC_MACCYC				(1)		// 机器周期与时钟周期之比
			参照si2c_stm32F4Hal.c编写目标平台的相关硬件操作代码