#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include "../../caaDriver/Inc/stm32f407xx.h"


#define GPIO_PIN_0   (uint16_t)(0x0001)
#define GPIO_PIN_1   (uint16_t)(0x0002)
#define GPIO_PIN_2   (uint16_t)(0x0004)
#define GPIO_PIN_3   (uint16_t)(0x0008)
#define GPIO_PIN_4   (uint16_t)(0x0010)
#define GPIO_PIN_5   (uint16_t)(0x0020)
#define GPIO_PIN_6   (uint16_t)(0x0040)
#define GPIO_PIN_7   (uint16_t)(0x0080)
#define GPIO_PIN_8   (uint16_t)(0x0100)
#define GPIO_PIN_9   (uint16_t)(0x0200)
#define GPIO_PIN_10  (uint16_t)(0x0400)
#define GPIO_PIN_11  (uint16_t)(0x0800)
#define GPIO_PIN_12  (uint16_t)(0x1000)
#define GPIO_PIN_13  (uint16_t)(0x2000)
#define GPIO_PIN_14  (uint16_t)(0x4000)
#define GPIO_PIN_15  (uint16_t)(0x8000)
#define GPIO_PIN_ALL (uint16_t)(0xffff)

typedef enum
{
	GPIO_Pin_Reset = 0U,
	GPIO_Pin_Set   = 1U
}GPIO_PinState_t;

typedef enum
{
	MODE_INPUT  		= 0X0U,
	MODE_OUTPUT 		= 0X01U,
	MODE_ALTERNATE_FUNC = 0X02U,
	MODE_ANALOG			= 0X3U
}GPIO_Mode_t;

typedef enum
{
	OUTPUT_PUSH_PULL  = 0x0U,
	OUTPUT_OPEN_DRAIN = 0X1U
}GPIO_Output_Type_t;

typedef enum
{
	OUTPUT_SPEED_LOW       = 0X0U,
	OUTPUT_SPEED_MEDIUM    = 0X1U,
	OUTPUT_SPEED_HIGH      = 0X2U,
	OUTPUT_SPEED_VERY_HIGH = 0X3U
}GPIO_Output_Speed_t;

typedef enum
{
	PULLUP_PULLDOWN_NO 		 = 0X0U,
	PULLUP_PULLDOWN_PULLUP   = 0X1U,
	PULLUP_PULLDOWN_PULLDOWN = 0X2U,

}GPIO_PullUp_PullDown_t;

typedef enum 
{
	GPIO_AF0  = 0X0U,
	GPIO_AF1  = 0X1U,
	GPIO_AF2  = 0X2U,
	GPIO_AF3  = 0X3U,
	GPIO_AF4  = 0X4U,
	GPIO_AF5  = 0X5U,
	GPIO_AF6  = 0X6U,
	GPIO_AF7  = 0X7U,
	GPIO_AF8  = 0X8U,
	GPIO_AF9  = 0X9U,
	GPIO_AF10 = 0XaU,
	GPIO_AF11 = 0XbU,
	GPIO_AF12 = 0XcU,
	GPIO_AF13 = 0XdU,
	GPIO_AF14 = 0XeU,
	GPIO_AF15 = 0XfU
}GPIO_Alternate_t;

typedef struct
{
	uint32_t pinNumber;
	uint32_t mode;
	uint32_t otype;
	uint32_t ospeed;
	uint32_t pupd;
	uint32_t alternate;
}GPIO_Init_t;

namespace can::driver::gpio
{
	void 			GPIO_Init(GPIO_t* pGPIOx, const GPIO_Init_t& pInit);

	void            GPIO_Toggle_Pin(GPIO_t* pGPIOx, uint16_t pPinNumber);
	void            GPIO_Write_Pin(GPIO_t* pGPIOx, uint16_t pPinNumber, GPIO_PinState_t pPinState);
	GPIO_PinState_t GPIO_Read_Pin(GPIO_t* pGPIOx, uint16_t pPinNumber);
}


#endif /* GPIO_H_ */
