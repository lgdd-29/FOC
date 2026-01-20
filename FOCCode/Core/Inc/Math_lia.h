#ifndef _MATH_LIA_H_
#define _MATH_LIA_H_

#include "foc_types.h"
# define PI_FOC 3.14159265358979f

typedef struct
{
    foc_float_t sin;
    foc_float_t cos;
}Trig_t;

void Clarke_Transform(Three_Phase_t* in, Two_Phase_t* out);
void Park_Transform(Two_Phase_t* in, Two_Phase_t* out, foc_float_t angle_rad);
void InvPark_Transform(Two_Phase_t* in, Two_Phase_t* out, foc_float_t angle_rad);



#endif /* _MATH_LIA_H_ */