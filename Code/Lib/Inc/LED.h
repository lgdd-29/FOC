#ifndef _LED_H_
#define _LED_H_
#include "stm32f4xx_hal.h"
typedef struct LED LED;
struct LED {
    GPIO_TypeDef* port;
    uint16_t pin;

    void (*Inie)(LED* self);
    void (*On)(LED* self);  
    void (*Off)(LED* self); 
    void (*Toggle)(LED* self);
};

LED* LED_Create(GPIO_TypeDef* port, uint16_t pin);

#endif /* _LED_H_ */
