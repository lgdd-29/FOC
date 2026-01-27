#include "Math_lib.h"
#include "foc_typeds.h"
#include "string.h"
#include "math.h"
//克拉克变换
void Clarke_Transform(Three_Phase_t *in, Two_Phase_t *out)
{
    out->x=in->a;
    out->y=sqrt(3.0f)*(2*in->b+in->a);
}

//克拉克逆变换
void InvClarke_Transform(Two_Phase_t* in, Three_Phase_t* out)
{
    out->a = in->x;
    out->b = (-0.5f * in->x + 0.86602540378f * in->y); // (-x/2 + y*sqrt(3)/2)
    out->c = (-0.5f * in->x - 0.86602540378f * in->y); // (-x/2 - y*sqrt(3)/2)
}
//帕克变换
void Park_Transform(Two_Phase_t *in, Two_Phase_t *out, foc_float_t angle_rad)
{
    foc_float_t sin_angle = sinf(angle_rad);
    foc_float_t cos_angle = cosf(angle_rad);

    out->x = in->x * cos_angle + in->y * sin_angle;
    out->y = -in->x * sin_angle + in->y * cos_angle;
}
//帕克逆变换
void InvPark_Transform(Two_Phase_t *in, Two_Phase_t *out, foc_float_t angle_rad)
{
    foc_float_t sin_angle = sinf(angle_rad);
    foc_float_t cos_angle = cosf(angle_rad);
    out->x = in->x * cos_angle - in->y * sin_angle;
    out->y = in->x * sin_angle + in->y * cos_angle;
}
//角度归一化
void Normalize_Angle(foc_float_t* angle_rad)
{
    foc_float_t a=fmod(*angle_rad, 2.0f*PI);
    *angle_rad = a>=0.0f? a : a + 2.0f*PI;
}
//电角度
void Electrical_Angle_Calc(foc_float_t* mech_angle, foc_float_t pole_pairs, foc_float_t* elec_angle_rad)
{
    *elec_angle_rad = (*mech_angle) * pole_pairs;  //乘以极对数
    Normalize_Angle(elec_angle_rad);  //归一化到0~2π
}
//角度转换弧度
void Angle_Chance(foc_float_t* angle)
{
    if(*angle>2*PI)
    {
        *angle=(*angle-2*PI);
    }
    else
    {
        *angle=(*angle);
    }
}
