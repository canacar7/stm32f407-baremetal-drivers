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

	enum IRQn_t : uint8_t
	{
		WWDG_IRQ       = 0X00u,
		PWD_IRQ        = 0X01u,
		TAMP_STAMP_IRQ = 0X02u,
		RTC_WKUP_IRQ   = 0X03u,
		FLASH_IRQ      = 0X04u,
		RCC_IRQ        = 0X05u,
		EXTI_0_IRQ     = 0X06u,
		EXTI_1_IRQ     = 0X07u,
		EXTI_2_IRQ     = 0X08u,
		EXTI_3_IRQ     = 0X09u,
		EXTI_4_IRQ     = 0X0au,
		TIM2_IRQ 	   = 0X1cu,
		USART1_IRQ     = 0x25u,
		USART2_IRQ     = 0X26u
	};

namespace can::driver::interrupt
{
	class Exti
	{
	public:
		static void EXTILineConfig(EXTI_PortSource_t pPort, EXTI_LineSource_t pLine);
		static void EXTIConfig(EXTI_Init_t* pExtiInit);
		static void EXTIEnableInterrupt(IRQn_t pIRQNumber);
		static void EXTIDisableInterrupt(IRQn_t pIRQNumber);

	};
}
/*
 *
 *
 * 1-) SYSCFG konfigurasyonu yapıldı
 * 2-) EXTI Konfigurasyommnu yapıldı
 *
 */

/*
 1. NVIC icin mikroislemcinin User Guide dokumanina bakilir.
 		Cevresel birimlerin kesmelerini yoneten Nested Vectored Interrupt Controller (NVIC) alani incelenir.

 2. Mimari geregi 1 ile 240 arasinda kesme hatti gelebilir ve 0 ile 255 arasinda oncelik seviyesi belirlenebilir.

 3. Ilgili register'lara baktigimizda Interrupt Set-Enable Register, Interrupt Clear-Enable Register
  	  ve Interrupt Priority Register en kritik olanlardir.

 4. Aktif etmek icin Set-Enable, pasif etmek icin Clear-Enable,
  	  oncelik duzeni icin Priority register'i kullanilir.

 5. Set-Enable tarafinda NVIC_ISER0 ile NVIC_ISER7 arasinda toplam 8 adet register mevcuttur.

 6. Bu register'lari kontrol ederken 0 yazmanin bir islevi yoktur;
 	 	 1 yazildiginda ilgili hattin kesmesi aktif edilir.

 7. Bu 8 adet register, kendisine baglanan IRQ numarasi ile iliskilidir.
  	  Ornegin A portunun 0. pininden kesme alacaksak bunu EXTI0 uzerinden aliriz.

 8. Konfigurasyonu tamamlamak adina EXTI0 hattinin IRQ numarasini bulmamiz gerekir.

 9. Reference Manual icindeki "Interrupts and Events" bolumunde bulunan "Vector Table for STM32"
  	  tablosunun en s"ol sutunundaki "Position" degeri bizim IRQ numaramizdir.
  	  EXTI Line0 kesmesinin IRQ numarasi 6'dir.

 10. IRQ numarasini bulduktan sonra mikroislemcinin User Guide dokumanindan
 	 	 bu numaranin hangi register araligina denk geldigine bakilir
 	 	 (0-31 arasi ISER0, 32-63 arasi ISER1 gibi).
 	 	 IRQ numarasi 6 oldugu icin bu hat NVIC_ISER0 register'inda yer alacaktir.

 11. Hangi register ve bitin etkilenecegi su sekilde hesaplanir:
 	 	 6 / 32 islemi ile NVIC_ISER0 register'i secilir,
 	  	 6 % 32 islemi ile 6. bitin setlenmesi gerektigi bulunur.


 12. STM32 islemcisinin Cortex-M4 User Guide dokumaninda yer alan tanimlar
    stm32f407xx.h dosyasinda tanimlanir.
    NVIC register adresleri volatile uint32_t pointer olarak
    #define NVIC_ISER0 ((volatile uint32_t*)(0xE000E100UL))
    biciminde olusturulur.

 13. Buradaki tanimlamaya dikkat edilmelidir.
    Pointer tipi uint32_t* oldugu icin bu adrese 1 eklemek,
    adres degerini 4 byte ileri kaydirarak
    dogrudan bir sonraki NVIC_ISER register'ina gecis saglar.

 14. Ardindan NVIC_EnableInterrupt fonksiyonu yazilir.
    Arguman olarak uint8_t IRQ_Number degeri alinir.
    Bu degere gore mikroislemcinin ilgili NVIC register'i konfigure edilir.

 15. Metot yazilirken bolme islemi kritik onem tasir.
    Bir register 32 bit (2^5) tuttugundan dolayi
    IRQ numarasi 5 birim saga kaydirilarak (IRQ_Number >> 5)
    NVIC_ISER0 adresine eklenir ve dogru NVIC_ISERx register'ina ulasilir.

 16. Son adimda mod 32 islemi yapilarak (IRQ_Number & 0x1F)
    ilgili register icindeki bit pozisyonu bulunur.
    Bu bit set edilerek kesme hatti aktif (enable) hale getirilir.

*/
#endif /* INC_EXTI_H_ */




























