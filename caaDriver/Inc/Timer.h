/*
 * Timer.h
 *
 *  Created on: Sep 30, 2026
 *      Author: can
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "stm32f407xx.h"


//CR1 reginde yer alan DIR bitleri. Counter down or up
enum class TIMER_CounterMode_t : uint32_t
{
	UP 	   = (0X0U << 4u),
	DOWN   = (0x1u << 4u)
};

enum class TIMER_DmaInterrupt_t
{
	UIE 	= (0x0u << 0u),
	CC1E 	= (0X1U << 1U), 
	CC2E 	= (0X1U << 2U)
};

enum class TIMER_FlagStatus_t : uint32_t
{
	TIMER_FLAG_RESET = 0X0U,
	TIMER_FLAG_SET 	= 0X01
};


enum class TIMER_State_t : uint8_t
{
	TIMER_STATE_RESET = 0X0U,
	TIMER_STATE_READY = 0X1U,
	TIMER_STATE_BUSY  = 0X2U
};


struct TIMER_Init_t
{
	uint32_t 		    mPrescaler;
	uint32_t 		    mPeriod;
	TIMER_CounterMode_t mCounterMode;
};


struct TIMER_Handle
{
	TIM_t* mInstance = nullptr;
	TIMER_Init_t 	 mInit;
	TIMER_State_t		mState;

	void (*UpdataISR_Function)(TIMER_Handle* pHandle);
};



namespace can::driver::timer
{
	void TIMER_Init(TIMER_Handle* pHandle);

	void TIMER_Start(TIMER_Handle* pHandle);
	void TIMER_Stop(TIMER_Handle* pHandle);

	void TIMER_StartIT(TIMER_Handle* pHandle);
	void TIMER_StopIT(TIMER_Handle* pHandle);
	
	void TIMER_InterruptHandler(TIMER_Handle* pHandle);
	TIMER_FlagStatus_t getFlagStatus(TIMER_Handle* pHandle, uint32_t flagName);
	void clearFlag(TIMER_Handle* pHandle, uint32_t flagName);

	uint32_t timer_us();
};

#endif /* INC_TIMER_H_ */
