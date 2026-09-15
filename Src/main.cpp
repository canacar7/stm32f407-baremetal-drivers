#include <stdint.h>

#include  "../caaDriver/Inc/stm32f407xx.h"
#include  "../caaDriver/Inc/Exti.h"
static void GPIOConfig();
static void GPIOInterruptConfig();


int main(void)
{

	//GPIOConfig();



	for(;;);
}

void GPIOConfig()
{
	RCC_GPIOD_CLK_ENABLE();

	GPIO_Init_t mGpioInit;
	mGpioInit.mode      = GPIO_Mode_t::MODE_OUTPUT;
	mGpioInit.ospeed    = GPIO_Output_Speed_t::OUTPUT_SPEED_LOW;
	mGpioInit.otype     = GPIO_Output_Type_t::OUTPUT_PUSH_PULL;
	mGpioInit.pupd      = GPIO_PullUp_PullDown_t::PULLUP_PULLDOWN_NO;
	mGpioInit.pinNumber = GPIO_PIN_10;
	can::driver::gpio::GPIO_Init(GPIOD, mGpioInit);
}

void GPIOInterruptConfig()
{
	RCC_SYSCFG_CLK_ENABLE();

	can::driver::interrupt::EXTILineConfig(EXTI_PortSource_GPIOC, EXTI_LineSource_10);

	EXTI_Init_t mExtiInit = {0};

	mExtiInit.mExtiMode         = EXTI_Mode_t::Interrupt;
	mExtiInit.mLineNumber       = EXTI_LineSource_10;
	mExtiInit.mLineCmd          = PinState_t::ENABLE;
	mExtiInit.mTriggerSelection = EXTI_Trigger_t::Rising;
	can::driver::interrupt::EXTIConfig(&mExtiInit);
}
