#ifndef _AS5600_H_
#define _AS5600_H_
#include "FOC_typeds.h"
#include "stm32f407xx.h"
#include "MyI2C.h"
#include <stdint.h>
typedef struct AS5600_Driver_t AS5600_Driver_t;

struct AS5600_Driver_t
{
    MyI2C_Driver_t i2c; //包含I2C驱动
    foc_float_t Angle_now;
    foc_float_t Angle_last;
    foc_float_t Angle_zero;
    foc_float_t Angle_turns;
    foc_float_t Speed_Filtered;
    foc_float_t Angle_Spped;

    //接口映射
    uint8_t READ_OP;
    uint8_t WRITE_OP;
    uint8_t ANGLE_1_OP;
    uint8_t ANGLE_2_OP;
    uint8_t AS5600_ADDRESS;

    //函数映射
    void (*Init)(AS5600_Driver_t* self);
    float (*GetAngle)(AS5600_Driver_t* self);
    float (*GetSpeed)(AS5600_Driver_t* self,foc_float_t dt);
    float (*GetTurns)(AS5600_Driver_t* self);
};

void AS5600_Create(AS5600_Driver_t* self,uint8_t READ_OP,uint8_t WRITE_OP,uint8_t ANGLE_1_OP,uint8_t ANGLE_2_OP,uint8_t AS5600_ADDRESS);

#endif //_AS5600_H_

