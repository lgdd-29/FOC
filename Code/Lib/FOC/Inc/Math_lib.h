#ifndef _MATH_LIB_H_
#define _MATH_LIB_H_
#include "foc_typeds.h"
#define PI 3.14159265358979f

typedef struct{
    foc_float_t sin;
    foc_float_t cos;   
}Trig_t;
void Clarke_Transform(Three_Phase_t *in, Two_Phase_t *out);
void InvClarke_Transform(Two_Phase_t* in, Three_Phase_t* out);
void Park_Transform(Two_Phase_t *in, Two_Phase_t *out, foc_float_t angle_rad);
void InvPark_Transform(Two_Phase_t *in, Two_Phase_t *out, foc_float_t angle_rad);
void Normalize_Angle(foc_float_t* angle_rad);
void Electrical_Angle_Calc(foc_float_t* mech_angle_rad, foc_float_t pole_pairs, foc_float_t* elec_angle_rad);
void Angle_Chance(foc_float_t* angle,foc_float_t* new_angle_rad);


#endif //_MATH_LIB_H_
