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
    Three_Phase_trim(&pwm, 0.0f, 1.0f);

    return pwm;
}
// TODO FOC_Run_Impl
void FOC_Run_Impl(FOC_Driver_t* self,foc_float_t dt)
{


    foc_float_t Angle_new;
    Angle_new=self->hal.GetAngle();  //获取当前角度
    foc_float_t Angle_error=self->site.expert-Angle_new;  //期望角度-当前角度

    foc_float_t v_q = self->site.pi.Kp * Angle_error;
    (v_q)<(-6)?(-6):((v_q)>(6)?(6):(v_q));

    // 2. 设置d轴电压为0，q轴电压为Uq
    self->v_dq.x = 0.0f; // Vd
    self->v_dq.y = v_q;
    //self->v_dq.y = self->site.State_OUT(&self->site,dt);   // Vq
    Angle_new=Angle_new*7-self->Angle_zero;
    Normalize_Angle(&Angle_new);


    // 3. 逆Park变换得到αβ坐标系下的电压
    Normalize_Angle(&Angle_new);
    InvPark_Transform(&self->v_dq, &self->v_alpha_beta, Angle_new);

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
    self->Angle_zero=0;
    // 初始化电压和电流结构体
    FOC_TwoPhase_Init(&self->v_alpha_beta);
    FOC_TwoPhase_Init(&self->v_dq);
    FOC_TwoPhase_Init(&self->I_alpha_beta);
    FOC_TwoPhase_Init(&self->I_dq);
    self->site.State_Init(&self->site);
    
}

void FOC_Site(FOC_Driver_t* self,foc_float_t expert,foc_float_t kp,foc_float_t ki)
{
    self->site.expert=expert;
    self->site.pi.Kp=kp;
    self->site.pi.Ki=ki;
}

//TODO FOC_Create
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
        driver->Site=FOC_Site;

        //内部函数映射
        State_Create(&driver->site);

    }
    return driver;
}