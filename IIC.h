#include "stm32f10x.h"

/***************************************************************
 说明：以下参数是AT24Cxx的寻址空间，C0x ,X 表示XK 如C01表示1K
 127表示2^7 1Kbit/8=128Byte 128字节
 255表示2^8 2Kbit/8=256Byte 256字节
 512表示2^9 4Kbit/8=512Byte 512字节 
***************************************************************/
#define AT24C01 127
#define AT24C02 255
#define AT24C04 511
#define AT24C08 1023
#define AT24C16 2047
#define AT24C32 4095
#define AT24C64  8191
#define AT24C128 16383
#define AT24C256 32767
/*********************************************************
 - I2C 宏定义：
 - 软件模拟I2C时的管脚，用PB10模拟CLK，用PB11模拟SDA
 - 所以在应用的时候，要打开GPIOB的时钟    
*********************************************************/
#define IIC_SCL_PORT              GPIOB
#define IIC_SCL_CLK               RCC_APB2Periph_GPIOB  
#define IIC_SCL_PIN               GPIO_Pin_10
#define IIC_SDA_PORT              GPIOB
#define IIC_SDA_CLK               RCC_APB2Periph_GPIOB  
#define IIC_SDA_PIN               GPIO_Pin_11
/*********************************************************
 - I2C宏定义：
 - SDA方向设置:
 - SDA_IN(): 此处是将SDA定义为配置为输入模式
 - SDA_OUT():此处是将SDA_OUT配置为推挽输出模式
 说明：这里是配置的快速写法，当然也可以用库函数初始化，
    因为IIC的SDA线，即要作为输入也要作为输出。        
*********************************************************/
        
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}
/*********************************************************
 - I2C宏定义：
 - I2C时钟操作:
 - Set_IIC_SCL: 此处是将SCL拉为高电平
 - Clr_IIC_SCL: 此处是将SCL拉为低电平
 - 说明：通过这两个宏定义就可以模拟IIC的时钟电平SCL的高低变化了     
*********************************************************/
#define Set_IIC_SCL  {GPIO_SetBits(IIC_SCL_PORT,IIC_SCL_PIN);}
#define Clr_IIC_SCL  {GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN);} 
/*********************************************************
 - I2C宏定义：
 - I2C数据线操作:
 - Set_IIC_SDA: 此处是将SDA拉为高电平
 - Clr_IIC_SDA: 此处是将SDA拉为低电平
 - READ_SDA  : 此处是读取SDA线上的位数据
 - 说明：通过这两个宏定义就可以模拟IIC的数据线SDA电平的高低变化了     
*********************************************************/
#define Set_IIC_SDA  {GPIO_SetBits(IIC_SDA_PORT,IIC_SDA_PIN);}
#define Clr_IIC_SDA  {GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN);} 
#define READ_SDA    (GPIO_ReadInputDataBit(IIC_SDA_PORT, IIC_SDA_PIN))

extern u8 AT24CXX_ReadOneByte(u16 ReadAddr);//指定地址读取一个字节
extern void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);//指定地址写入一个字节
extern void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
extern u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);//指定地址开始读取指定长度数据
extern void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);//从指定地址开始写入指定长度的数据
extern void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);//从指定地址开始读出指定长度的数据
extern u8 AT24CXX_Check(void); //检查器件
extern void AT24CXX_Init(void); //初始化IIC


