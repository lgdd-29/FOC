#include "Math_lib.h"
#include "foc_typeds.h"
#include "string.h"
#include "math.h"
void Clarke_Transform(Three_Phase_t *in, Two_Phase_t *out)
{
    out->x=in->a;
    out->y=(in->b-in->c)*sqrt(3.0f);
}

void InvClarke_Transform(Two_Phase_t* in, Three_Phase_t* out)
{
    out->a = in->x;
    out->b = (-in->x + sqrt(3.0f) * in->y) / 2.0f;
    out->c = (-in->x - sqrt(3.0f) * in->y) / 2.0f;
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

void Normalize_Angle(foc_float_t* angle_rad)
{
    foc_float_t a=fmod(*angle_rad, 2.0f*PI);
    *angle_rad = a>=0.0f? a : a + 2.0f*PI;
}

void Electrical_Angle_Calc(foc_float_t* mech_angle_rad, foc_float_t pole_pairs, foc_float_t* elec_angle_rad)
{
    *elec_angle_rad = (*mech_angle_rad) * pole_pairs;  //乘以极对数
    Angle_Chance(elec_angle_rad);  //转为弧度
    Normalize_Angle(elec_angle_rad);  //归一化到0~2π
}

void Angle_Chance(foc_float_t* angle)
{
    if(*angle>180.0f)
    {
        *angle=(*angle-360.0f)*PI/180.0f;
    }
    else
    {
        *angle=(*angle)*PI/180.0f;
    }
}
