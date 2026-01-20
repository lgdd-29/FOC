#include "Math_lia.h"
#include "foc_types.h"
#include "string.h"
#include "math.h"
void Clarke_Transform(Three_Phase_t *in, Two_Phase_t *out)
{
    out->x=in->a;
    out->y=(in->a+2.0f*in->b)/sqrt(3.0f);
}

void Park_Transform(Two_Phase_t *in, Two_Phase_t *out, foc_float_t angle_rad)
{
    foc_float_t sin_angle = sinf(angle_rad);
    foc_float_t cos_angle = cosf(angle_rad);

    out->x = in->x * cos_angle + in->y * sin_angle;
    out->y = -in->x * sin_angle + in->y * cos_angle;
}

void InvPark_Transform(Two_Phase_t *in, Two_Phase_t *out, foc_float_t angle_rad)
{
    foc_float_t sin_angle = sinf(angle_rad);
    foc_float_t cos_angle = cosf(angle_rad);
    out->x = in->x * cos_angle - in->y * sin_angle;
    out->y = in->x * sin_angle + in->y * cos_angle;
}
