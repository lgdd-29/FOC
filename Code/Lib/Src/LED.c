#include "stm32f4xx_hal.h"
#include "LED.h"
#include <stdlib.h>
/* USER CODE BEGIN 0 */
static void led_init_private( LED* self) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 1. 使能GPIO时钟
    if (self->port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (self->port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    // ... 其他端口
    
    // 2. 配置GPIO模式为推挽输出
    GPIO_InitStruct.Pin = self->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(self->port, &GPIO_InitStruct);
}
static void led_on(LED* self) {
    HAL_GPIO_WritePin(self->port, self->pin, GPIO_PIN_SET);
}

// 行为2: 关灯
static void led_off( LED* self) {
    HAL_GPIO_WritePin(self->port, self->pin, GPIO_PIN_RESET);
}

// 行为3: 翻转
static void led_toggle( LED* self) {
    HAL_GPIO_TogglePin(self->port, self->pin);
}

// 构造函数：创建并初始化一个LED对象
LED* LED_Create(GPIO_TypeDef* port, uint16_t pin) {
    LED* new_led = (LED*)malloc(sizeof(LED));
    if (new_led == NULL) {
        return NULL; // 内存分配失败
    }
    
    // 初始化成员变量
    new_led->port = port;
    new_led->pin = pin;
    
    // 将函数指针绑定到具体实现
    new_led->Inie = led_init_private;
    new_led->On = led_on;
    new_led->Off = led_off;
    new_led->Toggle = led_toggle;
    
    // 调用初始化
    if (new_led->Inie) new_led->Inie(new_led);
    
    return new_led;
}