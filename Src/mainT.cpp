#include <stdint.h>

#include  "../caaDriver/Inc/stm32f407xx.h"
#include  "../caaDriver/Inc/Exti.h"
#include "../caaDriver/Inc/Usart.h"
#include "../caaDriver/Inc/Timer.h"
#include <iostream>
#include <string>
#include <cstring>

USART_Handle  mUSartHandle;
TIMER_Handle  mTimerHandle;

static void GPIOConfig();
static void USARTConfig();
static void GPIOInterruptConfig();
static void TIMERConfig();


extern "C" void EXTI0_IRQHandler(void)
{
	uint32_t tVal = EXTI->PR;
	if(tVal & (0x1U << 0)) //İNTERRUP SET EDİLMİS.
	{
		tVal = (0X1U << 0);
		EXTI->PR = tVal;
		can::driver::gpio::GPIO_Toggle_Pin(GPIOD, GPIO_PIN_10);
	}
}

extern "C" void USART2_IRQHandler(void)
{
    can::driver::usart::USART_InterruptHandler(&mUSartHandle);
}

extern "C" void TIM2_IRQHandler(void)
{
    // Donanım buraya atladığında, biz işi kendi kütüphanemizin profesyonel yöneticisine devrediyoruz
    can::driver::timer::TIMER_InterruptHandler(&mTimerHandle);
}
int main(void)
{
    char msgToSend[] = "CAN ACAR\n";
    char msgToReceiced[30];

    GPIOConfig();
	USARTConfig();
	GPIOInterruptConfig();
    TIMERConfig();
    can::driver::timer::TIMER_Start(&mTimerHandle);


    can::driver::interrupt::Exti::EXTIEnableInterrupt(IRQn_t::USART2_IRQ);
    can::driver::usart::USART_TransmitDataIT(&mUSartHandle, (uint8_t*)msgToSend, strlen(msgToSend));
    can::driver::usart::USART_ReceiveDataIT(&mUSartHandle, (uint8_t*)msgToReceiced, 20); //her 20 byte interrup olusacak.
	for(;;)
    {
        can::driver::gpio::GPIO_Toggle_Pin(GPIOD, GPIO_PIN_14);
        for(volatile uint32_t i = 0; i < 1600000U; i++)
        {
            //todo
        }
    }
}

void USARTConfig()
{
    RCC_USART2_CLK_ENABLE();

    mUSartHandle.mInstance                  = USART2;
    mUSartHandle.mInit.mBaudRate            = USART_BaudRate_t::BAUD_115200; 
    mUSartHandle.mInit.mHardwareFLowControl = USART_HardwareFlowControl_t::HW_FLOW_NONE;
    mUSartHandle.mInit.mMode                = USART_MODE::TX_RX;
    mUSartHandle.mInit.mOverSampling        = USART_Sampling_t::OVERSAMPLING_16;
    mUSartHandle.mInit.mParityBit           = USART_Parity_t::NONE;
    mUSartHandle.mInit.mStopBit             = USART_StopBit_t::STOP_BIT_1;
    mUSartHandle.mInit.mWordLength          = USART_WordLength_t::WORD_LENGTH_8_BIT;

    can::driver::usart::USART_Init(&mUSartHandle);
    can::driver::interrupt::Exti::EXTIEnableInterrupt(IRQn_t::USART2_IRQ);

    can::driver::usart::USART_PeriphCMD(&mUSartHandle, FunctionalState_t::ENABLE);
}

void GPIOConfig()
{
    GPIO_Init_t pGpioHandle = {0};
    RCC_GPIOA_CLK_ENABLE();

    pGpioHandle.mode            =  GPIO_Mode_t::MODE_ALTERNATE_FUNC;
    pGpioHandle.pinNumber       =  GPIO_PIN_2 | GPIO_PIN_3;  
    pGpioHandle.otype           =  GPIO_Output_Type_t::OUTPUT_PUSH_PULL;
    pGpioHandle.pupd            =  GPIO_PullUp_PullDown_t::PULLUP_PULLDOWN_NO;
    pGpioHandle.ospeed          =  GPIO_Output_Speed_t::OUTPUT_SPEED_VERY_HIGH; 
    pGpioHandle.alternate       =  GPIO_AF7;
    
    can::driver::gpio::GPIO_Init(GPIOA, pGpioHandle);

    //led config
    pGpioHandle = {0};
    RCC_GPIOD_CLK_ENABLE();

    pGpioHandle.mode            =  GPIO_Mode_t::MODE_OUTPUT;    
    pGpioHandle.pinNumber       =  GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;      
    pGpioHandle.otype           =  GPIO_Output_Type_t::OUTPUT_PUSH_PULL;
    pGpioHandle.pupd            =  GPIO_PullUp_PullDown_t::PULLUP_PULLDOWN_NO;
    pGpioHandle.ospeed          =  GPIO_Output_Speed_t::OUTPUT_SPEED_VERY_HIGH; 

    can::driver::gpio::GPIO_Init(GPIOD, pGpioHandle);
}

void GPIOInterruptConfig()
{
	RCC_SYSCFG_CLK_ENABLE();

	can::driver::interrupt::Exti::EXTILineConfig(EXTI_PortSource_t::EXTI_PortSource_GPIOA,
			EXTI_LineSource_t::EXTI_LineSource_0);

	EXTI_Init_t mExtiInit;

	mExtiInit.mExtiMode         = EXTI_Mode_t::Interrupt;
	mExtiInit.mLineNumber       = EXTI_LineSource_0;
	mExtiInit.mLineCmd          = PinState_t::ENABLE;
	mExtiInit.mTriggerSelection = EXTI_Trigger_t::Rising;
	can::driver::interrupt::Exti::EXTIConfig(&mExtiInit);

	can::driver::interrupt::Exti::EXTIEnableInterrupt(IRQn_t::EXTI_0_IRQ);
}

static void TIMERConfig()
{
    RCC_TIM2_CLK_ENABLE();

    mTimerHandle.mInstance          = TIM2;
    mTimerHandle.mInit.mPrescaler   = 15;
    mTimerHandle.mInit.mPeriod      = 0xffffffffu;
    mTimerHandle.mInit.mCounterMode = TIMER_CounterMode_t::UP;

    can::driver::timer::TIMER_Init(&mTimerHandle);
    // can::driver::interrupt::Exti::EXTIEnableInterrupt(IRQn_t::TIM2_IRQ);
}