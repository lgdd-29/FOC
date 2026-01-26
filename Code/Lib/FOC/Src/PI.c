#include "FOC_typeds.h"
#include "stm32f407xx.h"
#include "PI.h"
#include "stdlib.h"
#include "Math_lib.h"


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
    self->integral = 0;    // 清零积分项
    self->Prev_error = 0;  // 清零前一误差
}

void PI_Init(PI_Driver_t* self)
{
    self->Kp = 0;
    self->Ki = 0;   
    self->integral_limit = 12.0f; // 默认积分限幅值
    self->output_limit = 12.0f;   // 默认输出限幅值
    self->integral = 0.0f;
    self->Prev_error = 0.0f;
}



void State_OUT(State_Driver_t* self,foc_float_t dt,foc_float_t Angle_now)
{
    self->now=Angle_now;
    foc_float_t error = self->expert - self->now;
    if(error>PI) error=error-2*PI;
    if(error<-PI) error=error+2*PI;
    self->out=self->pi.PI_OUT(&self->pi,error,dt);
}
//FUN State_Init
void State_Init(State_Driver_t* self)
{
    self->expert=0;
    self->now=0;
    self->out=0;
    self->pi.PI_Init(&self->pi);
}

//FUN Speed_Init
void Speed_Init(Speed_Driver_t* self)
{
    self->expert=0;
    self->now=0;
    self->out=0;
    self->pi.PI_Init(&self->pi);
}
//FUN Speed_OUT
void Speed_OUT(Speed_Driver_t* self,foc_float_t dt,foc_float_t Speed_now)
{
    self->now=Speed_now;
    foc_float_t error=self->expert-self->now;
    _constrain(error,-5,5);
    self->out=self->pi.PI_OUT(&self->pi,error,dt);
}


//FUN PI_Create
void PI_Create(PI_Driver_t* self)
{
    self->PI_OUT = PI_OUT;
    self->PI_Reset = PI_Reset;
    self->PI_Init = PI_Init;
}


void State_Create(State_Driver_t* self)
{
    self->State_OUT=State_OUT;
    self->State_Init=State_Init;

    PI_Create(&self->pi);
}

void Speed_Create(Speed_Driver_t *self)
{
    self->Speed_Init=Speed_Init;
    self->Speed_OUT=Speed_OUT;
    PI_Create(&self->pi);
}
