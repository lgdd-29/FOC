#ifndef _PI_H_
#define _PI_H_
#include "stm32f407xx.h"
#include "FOC_typeds.h"

//PARA PI的结构体
typedef struct PI_Driver_t PI_Driver_t;
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
};


PI_Driver_t* PI_Create(foc_float_t Kp, foc_float_t Ki);

#endif

