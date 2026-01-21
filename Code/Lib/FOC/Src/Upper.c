#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_tim_ex.h"
#include <stdint.h>
#include "Upper.h"
//Ҫ����ʾ:
//1.float��unsigned long������ͬ�����ݽṹ����
//2.union������������ݴ������ͬ�������ռ�
typedef union
{
    float fdata;
    unsigned long ldata;
} FloatLongType;

//��������fת��Ϊ4���ֽ����ݴ����byte[4]��
//����ָ��
void Float_to_Byte(float f,unsigned char byte[])
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}
//JustfloatЭ�鷢��
void Float_send(float *data_array,UART_HandleTypeDef* huart)
{
    //����Ϊ��������
    unsigned char tail[4] = {0x00, 0x00, 0x80, 0x7f};//����֡β
    //����ͨ�����������ݣ�CH_COUNTΪ�������ݵ�ͨ����
    for (int i = 0; i < CH_COUNT; i++){
        unsigned char byte[4];
        Float_to_Byte(data_array[i],byte); // ��ÿ��floatת��Ϊ4���ֽ�
        Send_array(byte,4,huart);           //4ֽ
    }
    Send_array(tail,4,huart);//֡β
}

void Send_array(unsigned char* byte, uint8_t Number, UART_HandleTypeDef* huart)
{
    int32_t i;

    if(byte == 0 || Number == 0){
        return;
    }

    for(i = 0; i < Number; i++)
    {
        while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) == RESET);

        HAL_UART_Transmit(huart, &byte[i], 1, HAL_MAX_DELAY);
    }

    while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);
}
