#include "foc_typeds.h"
#include "stm32f407xx.h"
#include "AS5600.h"
#include "MyI2C.h"
#include "stdlib.h"
#include <stdint.h>

void AS5600_Init(AS5600_Driver_t* self)
{
    self->i2c->MyI2C_Init(self->i2c);
}

uint8_t AS5600_READ(AS5600_Driver_t* self,uint8_t reg)
{
    uint8_t Data;
    uint8_t ack;
    self->i2c->MyI2C_Start(self->i2c);
    self->i2c->MyI2C_Send_Byte(self->i2c,(self->AS5600_ADDRESS<<1)|self->WRITE_OP);
    self->i2c->MyI2C_ReceiveAck(self->i2c,&ack);
    if(ack!=0)
    {
        self->i2c->MyI2C_Stop(self->i2c);
        return -1;
    }
    self->i2c->MyI2C_Send_Byte(self->i2c,reg);
    self->i2c->MyI2C_ReceiveAck(self->i2c,&ack);
    if(ack!=0)
    {
        self->i2c->MyI2C_Stop(self->i2c);
        return -1;
    }
    self->i2c->MyI2C_Start(self->i2c);
    self->i2c->MyI2C_Send_Byte(self->i2c,(self->AS5600_ADDRESS<<1)|self->READ_OP);
    self->i2c->MyI2C_ReceiveAck(self->i2c,&ack);
    if(ack!=0)
    {
        self->i2c->MyI2C_Stop(self->i2c);
        return -1;
    }
    self->i2c->MyI2C_Receive_Byte(self->i2c,&Data);
    self->i2c->MyI2C_SendAck(self->i2c,1);
    self->i2c->MyI2C_Stop(self->i2c);
    return Data;
}

uint16_t AS5600_GetRaw(AS5600_Driver_t* self)
{
    uint8_t Angle_H,Angle_L;
    uint16_t RawAngle;
    Angle_H=AS5600_READ(self,self->ANGLE_1_OP);
    Angle_L=AS5600_READ(self,self->ANGLE_2_OP);
    RawAngle=(((uint16_t)Angle_H&0x0F)<<8)|Angle_L;
    return RawAngle;
}

void AS5600_Calibrarion(AS5600_Driver_t* self)
{
    //TODO 校准函数
    for(uint8_t i=0;i<10;i++)
    {
        self->Angle_error+=AS5600_GetRaw(self);
    }
    self->Angle_error/=10.0f;

}

AS5600_Driver_t* AS5600_Create(MyI2C_Driver_t* i2c,uint8_t READ_OP,uint8_t WRITE_OP,uint8_t ANGLE_1_OP,uint8_t ANGLE_2_OP,uint8_t AS5600_ADDRESS)
{
    AS5600_Driver_t* driver = (AS5600_Driver_t*)malloc(sizeof(AS5600_Driver_t));
    if (driver != NULL)
    {
        //驱动映射
        driver->i2c = i2c;

        //寄存器映射
        driver->READ_OP = READ_OP;
        driver->WRITE_OP = WRITE_OP;
        driver->ANGLE_1_OP = ANGLE_1_OP;
        driver->ANGLE_2_OP = ANGLE_2_OP;
        driver->AS5600_ADDRESS = AS5600_ADDRESS;

        //函数映射
        driver->Init = AS5600_Init;
        driver->GetAngle = AS5600_GetRaw;
        driver->AS5600_Calibrarion = AS5600_Calibrarion;
    }
    return driver;
}