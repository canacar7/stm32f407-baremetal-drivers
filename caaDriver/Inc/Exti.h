/*
 * Exti.h
 *
 *  Created on: Sep 15, 2026
 *      Author: can
 */

#ifndef INC_EXTI_H_
#define INC_EXTI_H_
#include <stdint.h>
#include "stm32f407xx.h"

	enum EXTI_PortSource_t : uint8_t
	{
		EXTI_PortSource_GPIOA = 0,
		EXTI_PortSource_GPIOB,
		EXTI_PortSource_GPIOC,
		EXTI_PortSource_GPIOD,
		EXTI_PortSource_GPIOE,
		EXTI_PortSource_GPIOF,
		EXTI_PortSource_GPIOG,
		EXTI_PortSource_GPIOH,
		EXTI_PortSource_GPIOI,
		EXTI_PortSource_GPIOJ,
		EXTI_PortSource_GPIOK
	};

	enum EXTI_LineSource_t : uint8_t
	{
		EXTI_LineSource_0 = 0,
		EXTI_LineSource_1,
		EXTI_LineSource_2,
		EXTI_LineSource_3,
		EXTI_LineSource_4,
		EXTI_LineSource_5,
		EXTI_LineSource_6,
		EXTI_LineSource_7,
		EXTI_LineSource_8,
		EXTI_LineSource_9,
		EXTI_LineSource_10,
		EXTI_LineSource_11,
		EXTI_LineSource_12,
		EXTI_LineSource_13,
		EXTI_LineSource_14,
		EXTI_LineSource_15
	};

	enum EXTI_Mode_t : uint8_t
	{
		Interrupt = 0x00u,
		Event 	  = 0x04u,
	};

	enum EXTI_Trigger_t : uint8_t
	{
		Falling    	     = 0X00u,
		Rising  	     = 0x01u,
		RisingAndFalling = 0x03u
	};

	struct EXTI_Init_t
	{
		PinState_t     mLineCmd;
		EXTI_Mode_t    mExtiMode;
		EXTI_Trigger_t mTriggerSelection;
		uint8_t 	   mLineNumber;
	};

namespace can::driver::interrupt
{

	class Exti
	{
		void EXTILineConfig(EXTI_PortSource_t pPort, EXTI_LineSource_t pLine);
		void EXTIConfig(EXTI_Init_t* pExtiInit);
	};
}

/*
 * 1-) stm32f407xx.h da SYSCFG ve extı REGLERI TANIMLANDI.
 * 2-) LIONECONFİG ile interrptın aktif oalcagı port ve line secilir.
 * 		LineConfig metoudndan once lineconfig paremetlerini vermek icin enum tipiyle tanımlama yapdım. LİneCOnfig port ve line bilgisini aldıktan sonra ilgili regi
 * 		bularak guncellemelidir.
 *
 *	SYSCFG mantıksal oalrak yerlestirelim. İf else yapilari guzel durmaz ayrıca
 *	shiftlemek ve kapilari kullanmak hız acısından da verimli.
 *
 *
 *
 */
#endif /* INC_EXTI_H_ */
