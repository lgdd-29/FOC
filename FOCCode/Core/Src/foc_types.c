#include "foc_types.h"
#include "string.h"

void FOC_ThreePhase_Init(Three_Phase_t* self)
{
    if(self != NULL)
    {
        self->a = 0.0f;
        self->b = 0.0f;
        self->c = 0.0f;
    }
}

void FOC_TwoPhase_Init(Two_Phase_t* self)
{
    if(self != NULL)
    {
        self->a = 0.0f;
        self->b = 0.0f;
    }
}

void Two_Phase_trim(Two_Phase_t* self, foc_float_t min, foc_float_t max)
{
    if(self != NULL)
    {
        if(self->a < min) self->a = min;
        else if(self->a > max) self->a = max;

        if(self->b < min) self->b = min;
        else if(self->b > max) self->b = max;
    }
}

void Three_Phase_trim(Three_Phase_t* self, foc_float_t min, foc_float_t max)
{
    if(self != NULL)
    {
        if(self->a < min) self->a = min;
        else if(self->a > max) self->a = max;

        if(self->b < min) self->b = min;
        else if(self->b > max) self->b = max;

        if(self->c < min) self->c = min;
        else if(self->c > max) self->c = max;
    }
}