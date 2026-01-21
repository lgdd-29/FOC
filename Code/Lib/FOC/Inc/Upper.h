#ifndef __UPPER_H
#define __UPPER_H

#define CH_COUNT 1//��������Ҫ���͵�ͨ������

void Float_to_Byte(float f,unsigned char byte[]);
void Float_send(float *data_array,UART_HandleTypeDef* huart);
void Send_array(unsigned char* byte, uint8_t Number, UART_HandleTypeDef* huart);

#endif
