#include "stm32f407xx.h"
#include "PI.h"
#include "stdlib.h"

//FUN PI_OUT
foc_float_t PI_OUT(PI_Driver_t* self,foc_float_t error,foc_float_t dt)
{
    // 计算比例项
    foc_float_t P_out = self->Kp * error;

    // 计算积分项
    self->integral += error * dt*self->Ki;
    
    // 积分限幅
    if (self->integral > self->integral_limit)
        self->integral = self->integral_limit;
    else if (self->integral < -self->integral_limit)
        self->integral = -self->integral_limit;

    foc_float_t I_out = self->integral;

    // 计算总输出
    foc_float_t output = P_out + I_out;

    // 输出限幅
    if (output > self->output_limit)
        output = self->output_limit;
    else if (output < -self->output_limit)
        output = -self->output_limit;

    return output;
}

// FUN PI_Reset
void PI_Reset(PI_Driver_t* self)
{
    self->integral = 0;
    self->Prev_error = 0;
}

//FUN PI_Create
PI_Driver_t* PI_Create(foc_float_t Kp, foc_float_t Ki)
{
    PI_Driver_t* pi = (PI_Driver_t*)malloc(sizeof(PI_Driver_t));
    if (pi != NULL)
    {
        pi->Kp = Kp;
        pi->Ki = Ki;
        pi->integral_limit = 1000.0f; // 默认积分限幅值
        pi->output_limit = 1000.0f;   // 默认输出限幅值
        pi->integral = 0.0f;
        pi->Prev_error = 0.0f;
        pi->PI_OUT = PI_OUT;
        pi->PI_Reset = PI_Reset;
    }
    return pi;
}