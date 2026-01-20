#ifndef _FOC_TYPES_H_
#define _FOC_TYPES_H_
#include "stdint.h"
typedef float foc_float_t;
typedef int32_t foc_int_t;
typedef struct
{
    foc_float_t a;
    foc_float_t b;
    foc_float_t c;
}Three_Phase_t;

typedef struct 
{
    foc_float_t x;
    foc_float_t y;
}Two_Phase_t;


#endif /* _FOC_TYPES_H_ */