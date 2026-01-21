#include "stm32f4xx_hal.h"
#include <stdint.h>
#include "Upper.h"
typedef union
{
    float fdata;
    unsigned long ldata;
} FloatLongType;


void Float_to_Byte(float f,unsigned char byte[])
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}

void Float_send(float *data_array,UART_HandleTypeDef* huart)
{

    unsigned char tail[4] = {0x00, 0x00, 0x80, 0x7f};
    for (int i = 0; i < CH_COUNT; i++){
        unsigned char byte[4];
        Float_to_Byte(data_array[i],byte); 
        HAL_UART_Transmit(huart, byte, 4, HAL_MAX_DELAY);           
    }
    HAL_UART_Transmit(huart, tail, 4, HAL_MAX_DELAY);//发送结束标志
}
