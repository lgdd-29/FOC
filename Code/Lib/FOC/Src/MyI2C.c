#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "MyI2C.h"
#include "stdlib.h"
void MyI2C_W_SCL(MyI2C_Driver_t* self, uint8_t BitValue)
{
    HAL_GPIO_WritePin(self->SCL_port, self->SCL_pin, (GPIO_PinState)BitValue);
    HAL_Delay(2);
}

void MyI2C_W_SDA(MyI2C_Driver_t* self, uint8_t BitValue)
{
    HAL_GPIO_WritePin(self->SDA_port, self->SDA_pin, (GPIO_PinState)BitValue);
    HAL_Delay(2);
}

uint8_t MyI2C_R_SDA(MyI2C_Driver_t* self)
{
    return HAL_GPIO_ReadPin(self->SDA_port, self->SDA_pin);
    HAL_Delay(2);
}

void MyI2C_Start(MyI2C_Driver_t* self)
{
    MyI2C_W_SDA(self, 1);
    MyI2C_W_SCL(self, 1);
    MyI2C_W_SDA(self, 0);
    MyI2C_W_SCL(self, 0);
}

void MyI2C_Stop(MyI2C_Driver_t* self)
{
    MyI2C_W_SDA(self, 0);
    MyI2C_W_SCL(self, 1);
    MyI2C_W_SDA(self, 1);
}

void MyI2C_Send_Byte(MyI2C_Driver_t* self, uint8_t data)
{
    for (int i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(self, (data & 0x80) >> 7);
        MyI2C_W_SCL(self, 1);
        MyI2C_W_SCL(self, 0);
        data <<= 1;
    }
}

void MyI2C_ReceiveByte(MyI2C_Driver_t* self, uint8_t* data)
{
    *data = 0;
    MyI2C_W_SDA(self, 1);
    for (int i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(self, 1);
        if(MyI2C_R_SDA(self))
        {
            *data |= (0x80 >> i);
        }
        MyI2C_W_SCL(self, 0);
    }
}

void MyI2C_SendAck(MyI2C_Driver_t* self, uint8_t ack)
{
    MyI2C_W_SDA(self, ack);
    MyI2C_W_SCL(self, 1);
    MyI2C_W_SCL(self, 0);
}

void MyI2C_ReceiveAck(MyI2C_Driver_t* self, uint8_t* ack)
{
    MyI2C_W_SDA(self, 1);
    MyI2C_W_SCL(self, 1);
    *ack = MyI2C_R_SDA(self);
    MyI2C_W_SCL(self, 0);
}

void MyI2C_Init(MyI2C_Driver_t* self)
{
    //时钟选择
    if (self->SCL_port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (self->SCL_port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (self->SCL_port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (self->SCL_port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (self->SCL_port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
    if (self->SDA_port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (self->SDA_port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (self->SDA_port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (self->SDA_port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (self->SDA_port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure SCL pin
    GPIO_InitStruct.Pin = self->SCL_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(self->SCL_port, &GPIO_InitStruct);

    // Configure SDA pin
    GPIO_InitStruct.Pin = self->SDA_pin;
    HAL_GPIO_Init(self->SDA_port, &GPIO_InitStruct);

    // Set both pins high
    MyI2C_W_SCL(self, 1);
    MyI2C_W_SDA(self, 1);
}

MyI2C_Driver_t MyI2C_Creat(GPIO_TypeDef* SCL_port,uint16_t SCL_pin,GPIO_TypeDef* SDA_port,uint16_t SDA_pin)
{
    MyI2C_Driver_t* myi2c=(MyI2C_Driver_t*)malloc(sizeof(MyI2C_Driver_t));
    //引脚分配
    myi2c->SCL_port = SCL_port;
    myi2c->SCL_pin = SCL_pin;
    myi2c->SDA_port = SDA_port;
    myi2c->SDA_pin = SDA_pin;

    //函数映射
    myi2c->MyI2C_Init = MyI2C_Init;
    myi2c->MyI2C_Send_Byte = MyI2C_Send_Byte;
    myi2c->MyI2C_Receive_Byte = MyI2C_ReceiveByte;
    myi2c->MyI2C_SendAck = MyI2C_SendAck;
    myi2c->MyI2C_ReceiveAck = MyI2C_ReceiveAck;
    myi2c->MyI2C_Start = MyI2C_Start;
    myi2c->MyI2C_Stop = MyI2C_Stop;

    return *myi2c;
}