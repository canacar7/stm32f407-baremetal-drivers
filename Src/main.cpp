// #include <stdint.h>

// #include  "../caaDriver/Inc/stm32f407xx.h"
// #include  "../caaDriver/Inc/Exti.h"
// static void GPIOConfig();
// static void GPIOInterruptConfig();



// extern "C" void EXTI0_IRQHandler(void)
// {
// 	uint32_t tVal = EXTI->PR;
// 	if(tVal & (0x1U << 0)) //İNTERRUP SET EDİLMİS.
// 	{
// 		tVal = (0X1U << 0);
// 		EXTI->PR = tVal;
// 		can::driver::gpio::GPIO_Toggle_Pin(GPIOD, GPIO_PIN_10);
// 	}
// }

// int main(void)
// {

// 	GPIOConfig();
// 	GPIOInterruptConfig();


// 	for(;;);
// }

// void GPIOConfig()
// {
// 	RCC_GPIOD_CLK_ENABLE(); //output icin
// 	RCC_GPIOA_CLK_ENABLE(); //input buton icin

// 	GPIO_Init_t mGpioInit;
// 	mGpioInit.mode      = GPIO_Mode_t::MODE_OUTPUT;
// 	mGpioInit.ospeed    = GPIO_Output_Speed_t::OUTPUT_SPEED_LOW;
// 	mGpioInit.otype     = GPIO_Output_Type_t::OUTPUT_PUSH_PULL;
// 	mGpioInit.pupd      = GPIO_PullUp_PullDown_t::PULLUP_PULLDOWN_NO;
// 	mGpioInit.pinNumber = GPIO_PIN_10;
// 	can::driver::gpio::GPIO_Init(GPIOD, mGpioInit);

// 	mGpioInit.mode      = GPIO_Mode_t::MODE_INPUT;
// 	mGpioInit.pupd      = GPIO_PullUp_PullDown_t::PULLUP_PULLDOWN_PULLDOWN;
// 	mGpioInit.pinNumber = GPIO_PIN_0;
// 	can::driver::gpio::GPIO_Init(GPIOA, mGpioInit);
// }

// void GPIOInterruptConfig()
// {
// 	RCC_SYSCFG_CLK_ENABLE();

// 	can::driver::interrupt::Exti::EXTILineConfig(EXTI_PortSource_t::EXTI_PortSource_GPIOA,
// 			EXTI_LineSource_t::EXTI_LineSource_0);

// 	EXTI_Init_t mExtiInit;

// 	mExtiInit.mExtiMode         = EXTI_Mode_t::Interrupt;
// 	mExtiInit.mLineNumber       = EXTI_LineSource_0;
// 	mExtiInit.mLineCmd          = PinState_t::ENABLE;
// 	mExtiInit.mTriggerSelection = EXTI_Trigger_t::Rising;
// 	can::driver::interrupt::Exti::EXTIConfig(&mExtiInit);

// 	can::driver::interrupt::Exti::EXTIEnableInterrupt(EXTI_IRQ_Number_t::EXTI_0);
// }
