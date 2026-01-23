#ifndef _MY_I2C_H_
#define _MY_I2C_H_
#include "stm32f407xx.h"
#include <stdint.h>
typedef struct MyI2C_Driver_t MyI2C_Driver_t;
struct MyI2C_Driver_t
{
    GPIO_TypeDef* SCL_port;
    uint16_t SCL_pin;
    GPIO_TypeDef* SDA_port;
    uint16_t SDA_pin;

    void (*MyI2C_Init)(MyI2C_Driver_t* self);
    void (*MyI2C_Send_Byte)(MyI2C_Driver_t* self, uint8_t data);
    void (*MyI2C_Receive_Byte)(MyI2C_Driver_t* self, uint8_t* data);
    void (*MyI2C_SendAck)(MyI2C_Driver_t* self,uint8_t ack);
    void (*MyI2C_ReceiveAck)(MyI2C_Driver_t* self, uint8_t* ack);
    void (*MyI2C_Start)(MyI2C_Driver_t* self);
    void (*MyI2C_Stop)(MyI2C_Driver_t* self);
};
void MyI2C_Create(MyI2C_Driver_t* self,GPIO_TypeDef* SCL_port,uint16_t SCL_pin,GPIO_TypeDef* SDA_port,uint16_t SDA_pin);

#endif
