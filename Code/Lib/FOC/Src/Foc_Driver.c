#include "stm32f407xx.h"
#include "Foc_Driver.h"
#include "Math_lib.h"
#include "string.h"
#include "foc_typeds.h"

void FOC_Run_Impl(FOC_Driver_t* self, foc_float_t Uq)
{
    // 1. 计算电气角
    Electrical_Angle_Calc(&self->electrical_angle, self->pole_pairs, &self->electrical_angle);

    // 2. 设置d轴电压为0，q轴电压为Uq
    self->v_dq.x = 0.0f; // Vd
    self->v_dq.y = Uq;   // Vq

    // 3. 逆Park变换得到αβ坐标系下的电压
    InvPark_Transform(&self->v_dq, &self->v_alpha_beta, self->electrical_angle);

    // 4. 逆Clarke变换得到三相电压
    Three_Phase_t v_abc;
    InvClarke_Transform(&self->v_alpha_beta, &v_abc);

    // 5. 设置PWM占空比
    self->hal.SetPWM(&v_abc);
}

void FOC_Init_Impl(FOC_Driver_t* self)
{
    // 初始化各个成员变量

    // 初始化电压和电流结构体
    FOC_TwoPhase_Init(&self->v_alpha_beta);
    FOC_TwoPhase_Init(&self->v_dq);
    FOC_TwoPhase_Init(&self->I_alpha_beta);
    FOC_TwoPhase_Init(&self->I_dq);

    self->electrical_angle = 0.0f;

    // 绑定函数实现
    self->Init = FOC_Init_Impl;
    self->Run = FOC_Run_Impl;

    self->Init(self);
}