#include "Math_lib.h"
#include "foc_typeds.h"
#include "stm32f407xx.h"
#include "AS5600.h"
#include "MyI2C.h"
#include "stdlib.h"
#include <stdint.h>

void AS5600_Init(AS5600_Driver_t* self)
{
    self->i2c.MyI2C_Init(&self->i2c);
}

float AS5600_READ(AS5600_Driver_t* self)
{
    uint8_t Data1,Data2;
    uint8_t ack;
    uint16_t Angle;
    self->i2c.MyI2C_Start(&self->i2c);
    self->i2c.MyI2C_Send_Byte(&self->i2c,(self->AS5600_ADDRESS<<1)|self->WRITE_OP);
    self->i2c.MyI2C_ReceiveAck(&self->i2c,&ack);
    if(ack!=0)
    {
        self->i2c.MyI2C_Stop(&self->i2c);
        return -1;
    }
    self->i2c.MyI2C_Send_Byte(&self->i2c,self->ANGLE_1_OP);
    self->i2c.MyI2C_ReceiveAck(&self->i2c,&ack);
    if(ack!=0)
    {
        self->i2c.MyI2C_Stop(&self->i2c);
        return -1;
    }
    self->i2c.MyI2C_Start(&self->i2c);
    self->i2c.MyI2C_Send_Byte(&self->i2c,(self->AS5600_ADDRESS<<1)|self->READ_OP);
    self->i2c.MyI2C_ReceiveAck(&self->i2c,&ack);
    if(ack!=0)
    {
        self->i2c.MyI2C_Stop(&self->i2c);
        return -1;
    }
    self->i2c.MyI2C_Receive_Byte(&self->i2c,&Data1);
    self->i2c.MyI2C_SendAck(&self->i2c,0);
    self->i2c.MyI2C_Receive_Byte(&self->i2c,&Data2);
    self->i2c.MyI2C_SendAck(&self->i2c,1);
    self->i2c.MyI2C_Stop(&self->i2c);
    Angle=((Data1<<8)|Data2);
    return (foc_float_t)Angle*2*PI/4096.0f;
}

foc_float_t AS5600_GetSpped(AS5600_Driver_t* self)
{
    static foc_float_t Angle_now=0;
    static foc_float_t Angle_last=0;
    Angle_last=Angle_now;
    
    Angle_now=self->GetAngle(self);
    return 0;


}

void AS5600_Create(AS5600_Driver_t* self,uint8_t READ_OP,uint8_t WRITE_OP,uint8_t ANGLE_1_OP,uint8_t ANGLE_2_OP,uint8_t AS5600_ADDRESS)
{
    if (self != NULL)
    {
        //寄存器映射
        self->READ_OP = READ_OP;
        self->WRITE_OP = WRITE_OP;
        self->ANGLE_1_OP = ANGLE_1_OP;
        self->ANGLE_2_OP = ANGLE_2_OP;
        self->AS5600_ADDRESS = AS5600_ADDRESS;

        //函数映射
        self->Init = AS5600_Init;
        self->GetAngle = AS5600_READ;
        self->GetSpeed=AS5600_GetSpped;

    }
}