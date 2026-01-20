#ifndef _FOC_TYPEDS_H_
#define _FOC_TYPEDS_H_
#include "stm32f4xx_hal.h"
typedef float foc_float_t;
typedef int32_t foc_int_t;


typedef struct{
    foc_float_t a;
    foc_float_t b;
    foc_float_t c;
}Three_Phase_t;

typedef struct{
    foc_float_t x;
    foc_float_t y;
}Two_Phase_t;
void FOC_ThreePhase_Init(Three_Phase_t* self);
void FOC_TwoPhase_Init(Two_Phase_t* self);
void Two_Phase_trim(Two_Phase_t* self,foc_float_t min,foc_float_t max);
void Three_Phase_trim(Three_Phase_t* self,foc_float_t min,foc_float_t max);

#endif //_FOC_TYPEDS_H_

