#include "stm32f4xx_it.h"                // Device header
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
void Float_send(float *data_array)
{
    //����Ϊ��������
    unsigned char tail[4] = {0x00, 0x00, 0x80, 0x7f};//����֡β
    //����ͨ�����������ݣ�CH_COUNTΪ�������ݵ�ͨ����
    for (int i = 0; i < CH_COUNT; i++){
        unsigned char byte[4];
        Float_to_Byte(data_array[i],byte); // ��ÿ��floatת��Ϊ4���ֽ�
        Send_array(byte,4);           // ������4���ֽ�
    }
    Send_array(tail,4);//��������֡β
}

void Send_array(unsigned char* byte, uint8_t Number)
{
    int32_t i;

    if(byte == 0 || Number == 0){
        return;
    }

    for(i = 0; i < Number; i++)
    {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

        USART_SendData(USART2, byte[i]);
    }

    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}
