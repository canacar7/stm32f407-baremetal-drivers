/*
 * Timer.cpp
 *
 *  Created on: Sep 30, 2026
 *      Author: can
 */

#include <Timer.h>

namespace can::driver::timer
{
	static void Timer2_LedToggle_Callback(TIMER_Handle* pHandle)
	{
		can::driver::gpio::GPIO_Toggle_Pin(GPIOD, GPIO_PIN_15);
	}

	void TIMER_Init(TIMER_Handle* pHandle)
	{
		//timer yukari mi assagi mi sayacagi ayarlandi
		pHandle->mInstance->CR1 &= ~(0x1u << 4u);
		pHandle->mInstance->CR1 |= static_cast<uint32_t>(pHandle->mInit.mCounterMode);

		pHandle->mInstance->PSC = pHandle->mInit.mPrescaler;
		pHandle->mInstance->ARR = pHandle->mInit.mPeriod;

		//preload degerlerini SHADOW reglerine aktar.
		pHandle->mInstance->EGR |= (0X1U << 0u);

		//egr nin urettigi sahte kesmeleri temizle
		pHandle->mInstance->SR = ~(0X1U << 0U);

		pHandle->mState = TIMER_State_t::TIMER_STATE_READY;	
		pHandle->UpdataISR_Function = Timer2_LedToggle_Callback;
		/*
			1*************** PSC
			fSayici = fClk / (PSC + 1)
			FCLK = 16 MHZ

			OR: Sayicinin saniyede 1000 kere artmasini, yani 1 ms hızında saymasini sitersek
			fSayici = 1000 olamlidir. 
			PSC = 15999 yazdıgimizda bu durumu saglamis oluruz. 

			2************** ARR
			PSC ile timer hizini ayarladik. ARR registeri ise bu sayicinin maksimum
			kaca kadar cıkabilcegini belirler. SAyici ARR degerine ulastiginda sifirlanir ve
			kesme uretir. 

			fUpdae = fSayici / (ARR + 1
			fsayici = 1000Hz(OR1 De bulduk)
			Eger 1 sn lik kesme istersek; 
			1 = 1000 / (ARR + 1) 
			ARR = 999 cikar. 

			3************ GENEL FORMUL 
			fUpdate = fClk / (PSC + 1) * (ARR + 1)


			//Projede PSC = 15 ARR = 0XFFFFFFFF olarak kulalnılcak amacı; 
				-fSayici = 1 000 000 Hz 1Mhz 
				-sayici saniyede 1 milyon kez artıyorsa sayıcının(CNT regi) her bir artışı tam olarak 
					1mikro saniye olcer. 
				-tasma suresi sayici 0 dan 4.294.967.295 degerine 4.294.967.295 / 1 000 000 = 4249 sn yani 71 dk da ulasır.
				!!!!Kısacası kesmeyi kapalı tutarsak, çipin arka tarafında sürekli calisan ve hicbir islem gucu tuketmeyen ve 71 dkda
				basa saran super hassa bir kronometre elde edilir.
		*/
	}


	void TIMER_Start(TIMER_Handle* pHandle)
	{
		//cr1 reginde 0biti (CEN Counter Enable) 1 set edilir.
		pHandle->mInstance->CR1 |= (0x1u << 0u);
		pHandle->mState = TIMER_State_t::TIMER_STATE_BUSY;
	}

	void TIMER_Stop(TIMER_Handle* pHandle)
	{
		//cr1 reginde 0biti (CEN Counter Enable) 0 set edilir.
		pHandle->mInstance->CR1 &= ~(0x1u << 0u);
		pHandle->mState = TIMER_State_t::TIMER_STATE_READY;
	}

	void TIMER_StartIT(TIMER_Handle* pHandle)
	{
		pHandle->mInstance->DIER |=  (0x1u << 0u);
		TIMER_Start(pHandle);
	}

	void TIMER_StopIT(TIMER_Handle* pHandle)
	{
		pHandle->mInstance->DIER &= ~(0X1U << 0U);
		TIMER_Stop(pHandle);
	}

	void TIMER_InterruptHandler(TIMER_Handle* pHandle)
	{
		//SR bitindeki 0 bite bakariz. UPDATE ınterruyp kontrolu
		if(TIMER_FlagStatus_t::TIMER_FLAG_RESET  != getFlagStatus(pHandle, (uint32_t)(0x1u << 0u)))
		{
			//Eger bunu yapmazsak donanim surekli kesme uretir. Islemci kitlenir.
			clearFlag(pHandle, (uint32_t)(0x1u << 0u));

			//kullanicinin ISR metodu teklenir.
			if(nullptr != pHandle->UpdataISR_Function)
			{
				pHandle->UpdataISR_Function(pHandle);
			}
		}
	}
	
	TIMER_FlagStatus_t getFlagStatus(TIMER_Handle* pHandle, uint32_t flagName)
	{
		if((pHandle->mInstance->SR & flagName) != 0)
			return TIMER_FlagStatus_t::TIMER_FLAG_SET;
		else
			return TIMER_FlagStatus_t::TIMER_FLAG_RESET;
	}

	void clearFlag(TIMER_Handle* pHandle, uint32_t flagName)
	{
		pHandle->mInstance->SR = ~(flagName);
	}

	uint32_t timer_us()
	{
		return TIM2->CNT;
	}

};

