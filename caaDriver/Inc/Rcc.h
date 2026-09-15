#ifndef INC_RCC_H_
#define INC_RCC_H_

#include  "stm32f407xx.h"


#define RCC_GPIOA_CLK_ENABLE()			do{ SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);  \
											volatile uint32_t dummy = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN); \
											(void)dummy; \
										}while(0)

#define RCC_GPIOB_CLK_ENABLE()			do{ SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);  \
											volatile uint32_t dummy = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); \
											(void)dummy; \
										}while(0)

#define RCC_GPIOC_CLK_ENABLE()			do{ SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);  \
											volatile uint32_t dummy = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); \
											(void)dummy; \
										}while(0)

#define RCC_GPIOD_CLK_ENABLE()			do{ SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN);  \
											volatile uint32_t dummy = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); \
											(void)dummy; \
										}while(0)

#define RCC_SYSCFG_CLK_ENABLE()			do{ SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);  \
											volatile uint32_t dummy = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); \
											(void)dummy; \
										}while(0)

#define RCC_GPIOA_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN)
#define RCC_GPIOB_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN)
#define RCC_GPIOC_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN)
#define RCC_GPIOD_CLK_DISABLE()			CLEAR_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN)
#define RCC_SYSCFG_CLK_DISABLE()		CLEAR_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN)


namespace can::driver::rcc
{
    void setBit(volatile uint32_t &reg, uint32_t bitMask);
    void clearBit(volatile uint32_t &reg, uint32_t bitMask);
}


#endif /* INC_RCC_H_ */
