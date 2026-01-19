#ifndef __LED_H
#define __LED_H
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"// led.h - LED 灯的抽象层

typedef struct LED LED;
// 定义LED对象的“类”
struct LED
{ 
    GPIO_TypeDef* port;      // GPIO端口 (如 GPIOA)
    uint16_t pin;            // GPIO引脚号 (如 GPIO_PIN_5)
    
    // 行为（函数指针）- 对外提供的接口
    void (*init)( LED* self);
    void (*on)( LED* self);
    void (*off)( LED* self);
    void (*toggle)( LED* self);
};

// 构造函数 - 用于创建和初始化LED对象
LED* LED_Create(GPIO_TypeDef* GPIOx, uint16_t pin);

#endif // LED_H




