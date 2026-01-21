#include "stm32f407xx.h"
#include "Foc_Driver.h"
#include "Math_lib.h"
#include "string.h"
#include "foc_typeds.h"
#include "stdlib.h"

Three_Phase_t Get_PWMval(FOC_Driver_t* self,Three_Phase_t* abc)
{
    Three_Phase_t pwm;
    pwm.a = abc->a/ self->voltage_limit;
    pwm.b = abc->b/ self->voltage_limit;
    pwm.c = abc->c/ self->voltage_limit;
    return pwm;
}

void FOC_Run_Impl(FOC_Driver_t* self, foc_float_t Uq)
{
    foc_float_t Angle_new;
    Angle_new=self->hal.GetAngle();  //获取当前角度
    // 1. 计算电气角
    Electrical_Angle_Calc(&Angle_new, self->pole_pairs, &self->electrical_angle);

    // 2. 设置d轴电压为0，q轴电压为Uq
    self->v_dq.x = 0.0f; // Vd
    self->v_dq.y = Uq;   // Vq

    // 3. 逆Park变换得到αβ坐标系下的电压
    InvPark_Transform(&self->v_dq, &self->v_alpha_beta, self->electrical_angle);

    // 4. 逆Clarke变换得到三相电压
    Three_Phase_t v_abc;
    InvClarke_Transform(&self->v_alpha_beta, &v_abc);

    // 计算PWM占空比
    Three_Phase_t v_pwm=Get_PWMval(self,&v_abc);

    // 5. 设置PWM占空比
    Three_Phase_trim(&v_pwm, 0.0f, 1.0f); // 修剪到0-1范围内
    self->hal.SetPWM(&v_pwm);
}

void FOC_Init_Impl(FOC_Driver_t* self)
{
    // 初始化各个成员变量

    // 初始化电压和电流结构体
    FOC_TwoPhase_Init(&self->v_alpha_beta);
    FOC_TwoPhase_Init(&self->v_dq);
    FOC_TwoPhase_Init(&self->I_alpha_beta);
    FOC_TwoPhase_Init(&self->I_dq);
}

FOC_Driver_t* FOC_Create(foc_float_t pole_pairs, foc_float_t voltage_limit, FOC_HAL_t hal)
{
    FOC_Driver_t* driver = (FOC_Driver_t*)malloc(sizeof(FOC_Driver_t));
    if (driver != NULL)
    {
        driver->pole_pairs = pole_pairs;
        driver->voltage_limit = voltage_limit;
        driver->hal = hal;

        // 绑定函数实现
        driver->Init = FOC_Init_Impl;
        driver->Run = FOC_Run_Impl;
    }
    return driver;
}