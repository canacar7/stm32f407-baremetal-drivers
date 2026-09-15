#ifndef INC_RCC_H_
#define INC_RCC_H_

#include  "stm32f407xx.h"

namespace can::driver::rcc
{
    void setBit(volatile uint32_t &reg, uint32_t bitMask)
    {
    	SET_BIT(reg, bitMask);
    	volatile uint32_t dump = READ_BIT(reg,bitMask);
    	(void)dump;
    }

    void clearBit(volatile uint32_t &reg, uint32_t bitMask)
    {
    	CLEAR_BIT(reg, bitMask);
    }
}




#endif /* INC_RCC_H_ */
