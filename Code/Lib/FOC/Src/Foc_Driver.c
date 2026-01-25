#include "PI.h"
#include "stm32f407xx.h"
#include "Foc_Driver.h"
#include "Math_lib.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include "foc_typeds.h"
#include "stdlib.h"

Three_Phase_t Get_PWMval(FOC_Driver_t* self,Three_Phase_t* abc)
{
    Three_Phase_t pwm;
    pwm.a = (abc->a+self->voltage_limit/2)/ self->voltage_limit;
    pwm.b = (abc->b+self->voltage_limit/2)/ self->voltage_limit;
    pwm.c = (abc->c+self->voltage_limit/2)/ self->voltage_limit;
    Three_Phase_trim(&pwm, 0.0f, 1.0f);

    return pwm;
}
// FUN FOC_Run_Impl
void FOC_Run_Impl(FOC_Driver_t* self,foc_float_t dt)
{

    foc_float_t Angle_now=0;
    self->myas5600.Angle_now=self->myas5600.GetAngle(&self->myas5600);  //获取当前角度

    self->site.now=self->myas5600.Angle_now;
    // 2. 设置d轴电压为0，q轴电压为Uq
    self->v_dq.x = 0.0f; // Vd
    self->v_dq.y = self->speed.Speed_OUT(&self->speed,dt,self->myas5600.Angle_Spped);
    //self->v_dq.y = self->site.State_OUT(&self->site,dt,self->myas5600.Angle_now);
    //self->v_dq.y = self->site.State_OUT(&self->site,dt);   // Vq

    // 3. 逆Park变换得到αβ坐标系下的电压
    Angle_now=self->myas5600.Angle_now*7-self->myas5600.Angle_zero;
    Normalize_Angle(&Angle_now);
    InvPark_Transform(&self->v_dq, &self->v_alpha_beta, Angle_now);

    // 4. 逆Clarke变换得到三相电压
    Three_Phase_t v_abc;
    InvClarke_Transform(&self->v_alpha_beta, &v_abc);

    // 计算PWM占空比
    Three_Phase_t v_pwm=Get_PWMval(self,&v_abc);

    // 5. 设置PWM占空比
    Three_Phase_trim(&v_pwm, 0.0f, 1.0f); // 修剪到0-1范围内
    self->hal.SetPWM(&v_pwm);
}

void Angle_zero_GET(FOC_Driver_t* self)
{
    foc_float_t Angle_now=3*PI/2;
    self->v_dq.x=0.0f;
    self->v_dq.y=3.0f;
    Normalize_Angle(&Angle_now);
    InvPark_Transform(&self->v_dq, &self->v_alpha_beta, Angle_now);

    // 4. 逆Clarke变换得到三相电压
    Three_Phase_t v_abc;
    InvClarke_Transform(&self->v_alpha_beta, &v_abc);

    // 计算PWM占空比
    Three_Phase_t v_pwm=Get_PWMval(self,&v_abc);

    // 5. 设置PWM占空比
    Three_Phase_trim(&v_pwm, 0.0f, 1.0f); // 修剪到0-1范围内
    self->hal.SetPWM(&v_pwm);
    HAL_Delay(1000);
    Angle_now=self->myas5600.GetAngle(&self->myas5600)*self->pole_pairs;
    Normalize_Angle(&Angle_now);
    self->myas5600.Angle_zero=Angle_now;
    FOC_TwoPhase_Init(&self->v_dq);
    InvPark_Transform(&self->v_dq, &self->v_alpha_beta, Angle_now);

    // 4. 逆Clarke变换得到三相电压
    InvClarke_Transform(&self->v_alpha_beta, &v_abc);

    // 计算PWM占空比
     v_pwm=Get_PWMval(self,&v_abc);

    // 5. 设置PWM占空比
    Three_Phase_trim(&v_pwm, 0.0f, 1.0f); // 修剪到0-1范围内
    self->hal.SetPWM(&v_pwm);



}

void FOC_Init_Impl(FOC_Driver_t* self)
{
    // 初始化电压和电流结构体
    FOC_TwoPhase_Init(&self->v_alpha_beta);
    FOC_TwoPhase_Init(&self->v_dq);
    FOC_TwoPhase_Init(&self->I_alpha_beta);
    FOC_TwoPhase_Init(&self->I_dq);
    FOC_ThreePhase_Init(&self->I_abc);
    FOC_ThreePhase_Init(&self->I_abc_offset);
    FOC_ThreePhase_Init(&self->I_abc_ture);
    self->site.State_Init(&self->site);
    self->speed.Speed_Init(&self->speed);
    
}

void FOC_Site(FOC_Driver_t* self,foc_float_t expert,foc_float_t kp,foc_float_t ki)
{
    self->site.expert=expert;
    self->site.pi.Kp=kp;
    self->site.pi.Ki=ki;
}
void FOC_Speed(FOC_Driver_t* self,foc_float_t expert,foc_float_t kp,foc_float_t ki)
{
    self->speed.expert=expert;
    self->speed.pi.Kp=kp;
    self->speed.pi.Ki=ki;
}


//FUN FOC_Create
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
        driver->Speed=FOC_Speed;
        driver->Angle_zero_GET=Angle_zero_GET;


        //内部函数映射
        State_Create(&driver->site);
        Speed_Create(&driver->speed);

    }
    return driver;
}