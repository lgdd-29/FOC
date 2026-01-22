#ifndef _FOC_DRIVER_H_
#define _FOC_DRIVER_H_

#include "foc_typeds.h"
#include "Math_lib.h"
#include "PI.h"

//PARA FOC的结构体
typedef struct FOC_Driver_t FOC_Driver_t;

typedef struct
{  
    void (*SetPWM)(Three_Phase_t* duty);
    foc_float_t (*GetAngle)(void);
}FOC_HAL_t;

struct FOC_Driver_t
{
    FOC_HAL_t hal;

    State_Driver_t site;

    foc_float_t pole_pairs;
    foc_float_t voltage_limit;

    Two_Phase_t v_alpha_beta;
    Two_Phase_t v_dq;
    Two_Phase_t I_alpha_beta;
    Two_Phase_t I_dq;
    foc_float_t electrical_angle;

    void (*Init)(FOC_Driver_t* self);  
    void (*Run)(FOC_Driver_t* self,foc_float_t dt);
    void (*Site)(FOC_Driver_t* self,foc_float_t expert,foc_float_t kp,foc_float_t ki);
};
FOC_Driver_t* FOC_Create(foc_float_t pole_pairs, foc_float_t voltage_limit, FOC_HAL_t hal);

#endif // _FOC_DRIVER_H_