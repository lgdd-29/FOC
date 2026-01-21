#include "stm32f10x.h"                  // Device header
#include "Upper.h"
//要点提示:
//1.float和unsigned long具有相同的数据结构长度
//2.union据类型里的数据存放在相同的物理空间
typedef union
{
    float fdata;
    unsigned long ldata;
} FloatLongType;

//将浮点数f转化为4个字节数据存放在byte[4]中
//传递指针
void Float_to_Byte(float f,unsigned char byte[])
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}
//Justfloat协议发送
void Float_send(float *data_array)
{
    //参数为浮点数组
    unsigned char tail[4] = {0x00, 0x00, 0x80, 0x7f};//定义帧尾
    //按照通道数发送数据，CH_COUNT为发送数据的通道数
    for (int i = 0; i < CH_COUNT; i++){
        unsigned char byte[4];
        Float_to_Byte(data_array[i],byte); // 将每个float转换为4个字节
        Send_array(byte,4);           // 发送这4个字节
    }
    Send_array(tail,4);//发送数据帧尾
}

void Send_array(unsigned char* byte, uint8_t Number)
{
    uint8_t i;

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
