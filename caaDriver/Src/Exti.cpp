/*
 * Exti.cpp
 *
 *  Created on: Sep 15, 2026
 *      Author: can
 */

#include <Exti.h>

namespace can::driver::interrupt
{
	void EXTILineConfig(EXTI_PortSource_t pPort, EXTI_LineSource_t pLine)
	{
		// 0000 0000 &  0000 0011 ---> A PORTU(0)
		// 0000 0001 &  0000 0011 ---> B PORTU(1)
		// 0000 0010 &  0000 0011 ---> C PORTU(2) ...
		uint8_t tShift = (pLine & 0X3U) << 2;
		uint32_t tValue;

		tValue = SYSCFG->EXTICR[pLine >> 2];
		tValue &= ~(0xFU << tShift);
		tValue |= ((uint32_t)pPort << tShift);

		SYSCFG->EXTICR[pLine >> 2] = tValue;
	}

	void EXTIConfig(EXTI_Init_t* pExtiInit)
	{
		uint32_t tRegValue = (uint32_t)EXTI_BASE_ADDR;

		EXTI->IMR &= ~(0X1U << pExtiInit->mLineNumber);
		EXTI->EMR &= ~(0X1U << pExtiInit->mLineNumber);

		if(pExtiInit->mLineCmd != PinState_t::DISABLE)
		{
			tRegValue += (uint32_t)pExtiInit->mExtiMode; //interrupt ise 0x00, event ise 0x04
			*(volatile uint32_t*)(tRegValue) |= (pExtiInit->mLineCmd << pExtiInit->mLineNumber); //İlgili interrupt yada event aktif edilii

			EXTI->RTSR &= ~(0X1U << pExtiInit->mLineNumber);
			EXTI->FTSR &= ~(0X1U << pExtiInit->mLineNumber);

			if(pExtiInit->mTriggerSelection == EXTI_Trigger_t::RisingAndFalling)
			{
				EXTI->RTSR |= (0X1U << pExtiInit->mLineNumber);
				EXTI->FTSR |= (0X1U << pExtiInit->mLineNumber);
			}
			else if(pExtiInit->mTriggerSelection == EXTI_Trigger_t::Rising)
				EXTI->RTSR |= (0X1U << pExtiInit->mLineNumber);
			else if(pExtiInit->mTriggerSelection == EXTI_Trigger_t::Falling)
				EXTI->FTSR |= (0X1U << pExtiInit->mLineNumber);
		}
		else
		{
			tRegValue = (uint32_t)(EXTI_BASE_ADDR);
			tRegValue += (uint32_t)pExtiInit->mExtiMode; //interrupt ise 0x00, event ise 0x04
			*(volatile uint32_t*)(tRegValue) |= (0X1U << pExtiInit->mLineNumber); //İlgili interrupt yada event aktif edilii
		}
	}
}


/*
 * EXTILineConfig
 *	Tvalue port bilgisine gore bulunur. Hangi reg de oldugnu bulduk. 4 e bolduk cunku
 *		Her EXTI regi 4 tane konfigurasyonu alabilmekte. OR.  EXTI0 EXTI1 EXTI2 EXTI3 biciminde.
 *		LineSOurve a gorede hangi ofset de oldugnu bulmalıyız.
 *
 *  EXTIConfig
 *  	Ilk olarak IMR ve EMR reglerini clear yapmam gerekli.
 *  	mLİneCMD disable degilse; IMR VE EMR registerlarından birini linenumber a gore set etmeliyim.
 *
 */
