#ifndef _PI_H_
#define _PI_H_
#include "stm32f407xx.h"
#include "FOC_typeds.h"

typedef struct PI_Driver_t PI_Driver_t;
typedef struct State_Driver_t State_Driver_t;

//PARA PI的结构体
struct PI_Driver_t
{
    foc_float_t Kp;          // 比例增益
    foc_float_t Ki;          // 积分增益

    foc_float_t integral_limit; // 积分限幅值
    foc_float_t output_limit;   // 输出限幅值

    foc_float_t integral;    // 积分项
    foc_float_t Prev_error; // 上一次误差  

    foc_float_t (*PI_OUT)(PI_Driver_t* self,foc_float_t error,foc_float_t dt); // PI控制器输出函数指针
    void (*PI_Reset)(PI_Driver_t* self); // PI控制器重置函数指针
    void (*PI_Init)(PI_Driver_t* self); // PI控制器初始化函数指针
};

//PARA 位置环结构体
struct State_Driver_t
{
    PI_Driver_t pi;
    foc_float_t now;
    foc_float_t expert;

    foc_float_t (*State_OUT)(State_Driver_t* self,foc_float_t dt,foc_float_t Angle_now);
    void (*State_Init)(State_Driver_t* self);
};

void PI_Create(PI_Driver_t* self);
void State_Create(State_Driver_t* self);

#endif

