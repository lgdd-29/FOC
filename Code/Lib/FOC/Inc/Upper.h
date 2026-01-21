#ifndef __UPPER_H
#define __UPPER_H

#define CH_COUNT 1//这里是需要发送的通道数量

void Float_to_Byte(float f,unsigned char byte[]);
void Float_send(float *data_array);
void Send_array(unsigned char* byte, uint8_t Number);

#endif
