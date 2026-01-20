#include "foc_typeds.h"
#include "string.h"
#include <string.h>

void FOC_ThreePhase_Init(Three_Phase_t* self)
{
    if(self!=NULL)
    {
        self->a = 0.0f;
        self->b = 0.0f;
        self->c = 0.0f;
    }
}

void FOC_TwoPhase_Init(Two_Phase_t* self)
{
    if(self!=NULL)
    {
        self->x = 0.0f;
        self->y = 0.0f;
    }
}

void Two_Phase_trim(Two_Phase_t* self,foc_float_t min,foc_float_t max)
{
    if(self!=NULL)
    {
        if(self->x<min) self->x = min;
        else if(self->x>max) self->x = max;

        if(self->y<min) self->y = min;
        else if(self->y>max) self->y = max;
    }
}

void Three_Phase_trim(Three_Phase_t* self,foc_float_t min,foc_float_t max)
{
    if(self!=NULL)
    {
        if(self->a<min) self->a = min;
        else if(self->a>max) self->a = max;

        if(self->b<min) self->b = min;
        else if(self->b>max) self->b = max;

        if(self->c<min) self->c = min;
        else if(self->c>max) self->c = max;
    }
}