#include "Math_lib.h"
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

float AS5600_READ(AS5600_Driver_t* self)
{
    uint8_t Data1,Data2;
    uint8_t ack;
    uint16_t Angle;
    self->i2c->MyI2C_Start(self->i2c);
    self->i2c->MyI2C_Send_Byte(self->i2c,(self->AS5600_ADDRESS<<1)|self->WRITE_OP);
    self->i2c->MyI2C_ReceiveAck(self->i2c,&ack);
    if(ack!=0)
    {
        self->i2c->MyI2C_Stop(self->i2c);
        return -1;
    }
    self->i2c->MyI2C_Send_Byte(self->i2c,self->ANGLE_1_OP);
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
    self->i2c->MyI2C_Receive_Byte(self->i2c,&Data1);
    self->i2c->MyI2C_SendAck(self->i2c,0);
    self->i2c->MyI2C_Receive_Byte(self->i2c,&Data2);
    self->i2c->MyI2C_SendAck(self->i2c,1);
    self->i2c->MyI2C_Stop(self->i2c);
    Angle=((Data1<<8)|Data2);
    return (foc_float_t)Angle*2*PI/4096.0f;
}

void AS5600_Calibrarion(AS5600_Driver_t* self)
{
    // 校准函数：计算100次读数的平均值作为角度误差
    for(uint8_t i=0;i<100;i++)
    {
        self->Angle_error+=AS5600_READ(self);  // 累加100次角度读数
    }
    self->Angle_error/=100.0f;  // 计算平均值作为校准后的角度误差

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
        driver->GetAngle = AS5600_READ;
        driver->AS5600_Calibrarion = AS5600_Calibrarion;
    }
    return driver;
}