/*
 * stm32f407xx.h
 *
 *  Created on: Sep 7, 2026
 *      Author: can
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_


#include  <stdint.h>

#define SET_BIT(REG, BIT)    ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)  ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)   ((REG) & (BIT))

/*
 * Memory Base Addr
 */
#define FLASH_BASE_ADDR		(0x08000000UL)	/* Flash bellek, 1 MB */
#define SRAM1_BASE_ADDR		(0x20000000UL)	/* Ana SRAM, degiskenlerin tutuldugu alan, 112 KB */
#define SRAM2_BASE_ADDR		(0x2001C000UL)	/* Yardimci SRAM, 16 KB */
#define CCM_BASE_ADDR		(0x10000000UL)	/* Core Coupled Memory, 64 KB, DMA erisemez */

/*
 * Bus Domain Base Addr
 */
#define PERIPH_BASE_ADDR	(0x40000000UL)					/* Tum cevresel birimlerin basladigi adres */

#define APB1_BASE_ADDR		(PERIPH_BASE_ADDR + 0x00000000UL)	/* APB1 bus domain -> 0x40000000 */
#define APB2_BASE_ADDR		(PERIPH_BASE_ADDR + 0x00010000UL)	/* APB2 bus domain -> 0x40010000 */
#define AHB1_BASE_ADDR		(PERIPH_BASE_ADDR + 0x00020000UL)	/* AHB1 bus domain -> 0x40020000 */
#define AHB2_BASE_ADDR		(PERIPH_BASE_ADDR + 0x10000000UL)	/* AHB2 bus domain -> 0x50000000 */

/*
 * APB1 Peripheral Base Addr
 * Her cevresel birime 0x400 (1 KB) pencere ayrilmistir.
 * Aradaki bazi araliklar reserved oldugu icin sira kesintisiz degildir.
 */
#define TIM2_BASE_ADDR		(APB1_BASE_ADDR + 0x0000UL)
#define TIM3_BASE_ADDR		(APB1_BASE_ADDR + 0x0400UL)
#define TIM4_BASE_ADDR		(APB1_BASE_ADDR + 0x0800UL)
#define TIM5_BASE_ADDR		(APB1_BASE_ADDR + 0x0C00UL)
#define TIM6_BASE_ADDR		(APB1_BASE_ADDR + 0x1000UL)
#define TIM7_BASE_ADDR		(APB1_BASE_ADDR + 0x1400UL)
#define TIM12_BASE_ADDR		(APB1_BASE_ADDR + 0x1800UL)
#define TIM13_BASE_ADDR		(APB1_BASE_ADDR + 0x1C00UL)
#define TIM14_BASE_ADDR		(APB1_BASE_ADDR + 0x2000UL)

#define RTC_BKP_BASE_ADDR	(APB1_BASE_ADDR + 0x2800UL)	/* RTC ve Backup registerlari */
#define WWDG_BASE_ADDR		(APB1_BASE_ADDR + 0x2C00UL)	/* Window watchdog */
#define IWDG_BASE_ADDR		(APB1_BASE_ADDR + 0x3000UL)	/* Independent watchdog */

#define I2S2EXT_BASE_ADDR	(APB1_BASE_ADDR + 0x3400UL)
#define SPI2_BASE_ADDR		(APB1_BASE_ADDR + 0x3800UL)	/* SPI2 / I2S2 */
#define SPI3_BASE_ADDR		(APB1_BASE_ADDR + 0x3C00UL)	/* SPI3 / I2S3 */
#define I2S3EXT_BASE_ADDR	(APB1_BASE_ADDR + 0x4000UL)

#define USART2_BASE_ADDR	(APB1_BASE_ADDR + 0x4400UL)
#define USART3_BASE_ADDR	(APB1_BASE_ADDR + 0x4800UL)
#define UART4_BASE_ADDR		(APB1_BASE_ADDR + 0x4C00UL)
#define UART5_BASE_ADDR		(APB1_BASE_ADDR + 0x5000UL)

#define I2C1_BASE_ADDR		(APB1_BASE_ADDR + 0x5400UL)
#define I2C2_BASE_ADDR		(APB1_BASE_ADDR + 0x5800UL)
#define I2C3_BASE_ADDR		(APB1_BASE_ADDR + 0x5C00UL)

#define CAN1_BASE_ADDR		(APB1_BASE_ADDR + 0x6400UL)
#define CAN2_BASE_ADDR		(APB1_BASE_ADDR + 0x6800UL)

#define PWR_BASE_ADDR		(APB1_BASE_ADDR + 0x7000UL)	/* Power control */
#define DAC_BASE_ADDR		(APB1_BASE_ADDR + 0x7400UL)

/*
 * APB2 Peripheral Base Addr
 */
#define TIM1_BASE_ADDR                (APB2_BASE_ADDR + 0x0000UL)     /* Advanced timer */
#define TIM8_BASE_ADDR                (APB2_BASE_ADDR + 0x0400UL)     /* Advanced timer */

#define USART1_BASE_ADDR    		  (APB2_BASE_ADDR + 0x1000UL)
#define USART6_BASE_ADDR      		  (APB2_BASE_ADDR + 0x1400UL)

#define ADC1_BASE_ADDR                (APB2_BASE_ADDR + 0x2000UL)
#define ADC2_BASE_ADDR                (APB2_BASE_ADDR + 0x2100UL)
#define ADC3_BASE_ADDR                (APB2_BASE_ADDR + 0x2200UL)

#define SDIO_BASE_ADDR                (APB2_BASE_ADDR + 0x2C00UL)
#define SPI1_BASE_ADDR                (APB2_BASE_ADDR + 0x3000UL)
#define SYSCFG_BASE_ADDR      		  (APB2_BASE_ADDR + 0x3800UL)     /* EXTI pin secimi burada yapilir */
#define EXTI_BASE_ADDR                (APB2_BASE_ADDR + 0x3C00UL)     /* External interrupt */

#define TIM9_BASE_ADDR                (APB2_BASE_ADDR + 0x4000UL)
#define TIM10_BASE_ADDR               (APB2_BASE_ADDR + 0x4400UL)
#define TIM11_BASE_ADDR               (APB2_BASE_ADDR + 0x4800UL)


  /*
   * AHB1 Peripheral Base Addr
   */
  #define GPIOA_BASE_ADDR               (AHB1_BASE_ADDR + 0x0000UL)
  #define GPIOB_BASE_ADDR               (AHB1_BASE_ADDR + 0x0400UL)
  #define GPIOC_BASE_ADDR               (AHB1_BASE_ADDR + 0x0800UL)
  #define GPIOD_BASE_ADDR               (AHB1_BASE_ADDR + 0x0C00UL)
  #define GPIOE_BASE_ADDR               (AHB1_BASE_ADDR + 0x1000UL)
  #define GPIOF_BASE_ADDR               (AHB1_BASE_ADDR + 0x1400UL)
  #define GPIOG_BASE_ADDR               (AHB1_BASE_ADDR + 0x1800UL)
  #define GPIOH_BASE_ADDR               (AHB1_BASE_ADDR + 0x1C00UL)
  #define GPIOI_BASE_ADDR               (AHB1_BASE_ADDR + 0x2000UL)

  #define CRC_BASE_ADDR         		(AHB1_BASE_ADDR + 0x3000UL)
  #define RCC_BASE_ADDR         		(AHB1_BASE_ADDR + 0x3800UL)     /* Clock kontrolu, en cok kullanacagin blok */
  #define FLASH_R_BASE_ADDR     		(AHB1_BASE_ADDR + 0x3C00UL)     /* Flash INTERFACE registerlari (ACR, KEYR, CR...) */
  #define BKPSRAM_BASE_ADDR     		(AHB1_BASE_ADDR + 0x4000UL)     /* Backup SRAM, 4 KB, VBAT ile korunur */

  #define DMA1_BASE_ADDR                (AHB1_BASE_ADDR + 0x6000UL)
  #define DMA2_BASE_ADDR                (AHB1_BASE_ADDR + 0x6400UL)

  #define ETH_MAC_BASE_ADDR     		(AHB1_BASE_ADDR + 0x8000UL)
  #define USB_OTG_HS_BASE_ADDR  		(AHB1_BASE_ADDR + 0x20000UL)    /* -> 0x40040000 */


	/*
	 * NVIC ADRESS
	 */
  #define NVIC_ISER0    ((volatile uint32_t*)0xE000E100UL)
  #define NVIC_ICER0    ((volatile uint32_t*)0xE000E180UL)


typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFR[2];
}GPIO_t;

typedef struct {
    volatile uint32_t CR;            // 0x00: Clock Control Register
    volatile uint32_t PLLCFGR;       // 0x04: PLL Configuration Register
    volatile uint32_t CFGR;          // 0x08: Clock Configuration Register
    volatile uint32_t CIR;           // 0x0C: Clock Interrupt Register
    volatile uint32_t AHB1RSTR;      // 0x10: AHB1 Peripheral Reset Register
    volatile uint32_t AHB2RSTR;      // 0x14: AHB2 Peripheral Reset Register
    volatile uint32_t AHB3RSTR;      // 0x18: AHB3 Peripheral Reset Register
    uint32_t          DUMMY0;        // 0x1C: Rezerv (Boşluk)
    volatile uint32_t APB1RSTR;      // 0x20: APB1 Peripheral Reset Register
    volatile uint32_t APB2RSTR;      // 0x24: APB2 Peripheral Reset Register
    uint32_t          DUMMY1[2];     // 0x28 - 0x2C: Rezerv (2 adet 32-bit)
    volatile uint32_t AHB1ENR;       // 0x30: AHB1 Peripheral Clock Enable Register
    volatile uint32_t AHB2ENR;       // 0x34: AHB2 Peripheral Clock Enable Register
    volatile uint32_t AHB3ENR;       // 0x38: AHB3 Peripheral Clock Enable Register
    uint32_t          DUMMY2;        // 0x3C: Rezerv
    volatile uint32_t APB1ENR;       // 0x40: APB1 Peripheral Clock Enable Register
    volatile uint32_t APB2ENR;       // 0x44: APB2 Peripheral Clock Enable Register
    uint32_t          DUMMY3[2];     // 0x48 - 0x4C: Rezerv (2 adet 32-bit)
    volatile uint32_t AHB1LPENR;     // 0x50: AHB1 Low Power Peripheral Clock Enable Register
    volatile uint32_t AHB2LPENR;     // 0x54: AHB2 Low Power Peripheral Clock Enable Register
    volatile uint32_t AHB3LPENR;     // 0x58: AHB3 Low Power Peripheral Clock Enable Register
    uint32_t          DUMMY4;        // 0x5C: Rezerv
    volatile uint32_t APB1LPENR;     // 0x60: APB1 Low Power Peripheral Clock Enable Register
    volatile uint32_t APB2LPENR;     // 0x64: APB2 Low Power Peripheral Clock Enable Register
    uint32_t          DUMMY5[2];     // 0x68 - 0x6C: Rezerv (2 adet 32-bit)
    volatile uint32_t BDCR;          // 0x70: Backup Domain Control Register
    volatile uint32_t CSR;           // 0x74: Clock Control & Status Register
    uint32_t          DUMMY6[2];     // 0x78 - 0x7C: Rezerv (2 adet 32-bit)
    volatile uint32_t SSCGR;         // 0x80: Spread Spectrum Clock Generation Register
    volatile uint32_t PLLI2SCFGR;    // 0x84: PLLI2S Configuration Register
    volatile uint32_t PLLSAICFGR;    // 0x88: PLLSAI Configuration Register
    volatile uint32_t DCKCFGR;       // 0x8C: Dedicated Clock Configuration Register
} RCC_t;

typedef struct
{
	volatile uint32_t MEMRMP; 		 // Bellek adres eslemesini yeniden duzenler
	volatile uint32_t PMC;   		 // Ethernet PHY arayuzunu secer. (MII / RMII)
	volatile uint32_t EXTICR[4]; 	 // Port pin eslesmesini EXTI hatlarini yonlendirir.
	volatile uint32_t CMPCR; 		 // Yuksek hizda I/O bozulmalarını engeller.
}SYSCFG_t;

typedef struct
{
	volatile uint32_t IMR;			 // Hatlardaki kesinti uretimini acar vada maskeler.
	volatile uint32_t EMR; 			 // Hatlardaki donanımsal olay ureitmini acar yada maskeler.
	volatile uint32_t RTSR;    		 // Interruptın yukselen kenar oalcagını soyler.
	volatile uint32_t FTSR; 		 // INterruptın dusen kenar oalcagını soyler.
	volatile uint32_t SWIER;		 // Yazılımsal interupt var yok der.
	volatile uint32_t PR; 			 // Bekleyen kesinti bayragını okur veya temizler.

}EXTI_t;

typedef struct
{
	volatile uint32_t SR;  // USART donanımın o anki calisma durumu
	volatile uint32_t DR;  // Gonderilecek veya alinan verinin tutuldugu merkez register.
	volatile uint32_t BRR; // BaudRAte Register
	volatile uint32_t CR1; // Iletisim için temel parametreleri ve kesmeleri yonetir. 
                         // UE 13.bit ---> USART modulunu acar kapatır. 
                         // M  12.bit ---> Word length 
                         // PCE / PS 10 ve 9 bit  ---> Partiy COntrol enable ve tipi even odd
                         // TE / RE 3 ve 2 bit ---> Verici veya aliciyi devreye sokar. 
	volatile uint32_t CR2; // Stop bitleirni , senkron saat sinyalini ve coklu islemci adreslemesini yapar.  
	volatile uint32_t CR3; // DMA kanallarını, hata kesmelerini  ve donanımsal el sıkışma hatlarını yonetir. 
	volatile uint32_t GTPR;
}USART_t;

typedef struct
{
      volatile uint32_t CR1;    // 0x00: Sayaci acar/kapatir (CEN), sayma yonu, ARR preload (ARPE)
      volatile uint32_t CR2;    // 0x04: Master mod / trigger cikisi
      volatile uint32_t SMCR;   // 0x08: Slave mod, harici saat secimi
      volatile uint32_t DIER;   // 0x0C: Kesme ve DMA izinleri (UIE, CCxIE ...)
      volatile uint32_t SR;     // 0x10: Durum bayraklari (UIF, CCxIF ...)
      volatile uint32_t EGR;    // 0x14: Yazilimla olay uretme (UG)
      volatile uint32_t CCMR1;  // 0x18: Kanal 1-2 capture/compare modu
      volatile uint32_t CCMR2;  // 0x1C: Kanal 3-4 capture/compare modu
      volatile uint32_t CCER;   // 0x20: Capture/compare cikis izinleri ve polarite
      volatile uint32_t CNT;    // 0x24: Sayacin anlik degeri (TIM2/TIM5 icin 32 bit)
      volatile uint32_t PSC;    // 0x28: Prescaler -> sayac saati = f_TIM / (PSC + 1)
      volatile uint32_t ARR;    // 0x2C: Auto-reload, sayac bu degere ulasinca basa doner
      uint32_t          DUMMY0; // 0x30: Rezerv (TIM1/8'de RCR)
      volatile uint32_t CCR1;   // 0x34
      volatile uint32_t CCR2;   // 0x38
      volatile uint32_t CCR3;   // 0x3C
      volatile uint32_t CCR4;   // 0x40
      uint32_t          DUMMY1; // 0x44: Rezerv (TIM1/8'de BDTR)
      volatile uint32_t DCR;    // 0x48: DMA kontrol
      volatile uint32_t DMAR;   // 0x4C: DMA burst erisim adresi
      volatile uint32_t OR;     // 0x50: Option register (TIM2/TIM5'e ozel)
}TIM_t;

enum  class PinState_t : uint8_t
{
	DISABLE = 0X0U,
	ENABLE  = 0X01U
};

enum class FunctionalState_t : uint8_t
{
	DISABLE = 0X0U,
	ENABLE  = 0X01U
};



#define  GPIOA  ((GPIO_t*)(GPIOA_BASE_ADDR))
#define  GPIOB  ((GPIO_t*)(GPIOB_BASE_ADDR))
#define  GPIOC  ((GPIO_t*)(GPIOC_BASE_ADDR))
#define  GPIOD  ((GPIO_t*)(GPIOD_BASE_ADDR))
#define  GPIOE  ((GPIO_t*)(GPIOE_BASE_ADDR))

#define  RCC    ((RCC_t*)(RCC_BASE_ADDR))
#define  SYSCFG ((SYSCFG_t*)(SYSCFG_BASE_ADDR))
#define  EXTI 	((EXTI_t*)(EXTI_BASE_ADDR))

#define  USART1 ((USART_t*)(USART1_BASE_ADDR))
#define  USART6 ((USART_t*)(USART6_BASE_ADDR))

#define  USART2 ((USART_t*)(USART2_BASE_ADDR))
#define  USART3 ((USART_t*)(USART3_BASE_ADDR))
#define  UART4  ((USART_t*)(UART4_BASE_ADDR))
#define  UART5  ((USART_t*)(UART5_BASE_ADDR))

#define  TIM2   ((TIM_t*)(TIM2_BASE_ADDR)) 



#define RCC_AHB1ENR_GPIOAEN_Pos  (0u)
#define RCC_AHB1ENR_GPIOAEN_Mask (1u << RCC_AHB1ENR_GPIOAEN_Pos)
#define RCC_AHB1ENR_GPIOAEN      (RCC_AHB1ENR_GPIOAEN_Mask)

#define RCC_AHB1ENR_GPIOBEN_Pos  (1u)
#define RCC_AHB1ENR_GPIOBEN_Mask (1u << RCC_AHB1ENR_GPIOBEN_Pos)
#define RCC_AHB1ENR_GPIOBEN      (RCC_AHB1ENR_GPIOBEN_Mask)

#define RCC_AHB1ENR_GPIOCEN_Pos  (2u)
#define RCC_AHB1ENR_GPIOCEN_Mask (1u << RCC_AHB1ENR_GPIOCEN_Pos)
#define RCC_AHB1ENR_GPIOCEN      (RCC_AHB1ENR_GPIOCEN_Mask)

#define RCC_AHB1ENR_GPIODEN_Pos  (3u)
#define RCC_AHB1ENR_GPIODEN_Mask (1u << RCC_AHB1ENR_GPIODEN_Pos)
#define RCC_AHB1ENR_GPIODEN      (RCC_AHB1ENR_GPIODEN_Mask)

#define RCC_APB2ENR_SYSCFG_Pos   (14u)
#define RCC_APB2ENR_SYSCFG_Mask  (1u << RCC_APB2ENR_SYSCFG_Pos)
#define RCC_APB2ENR_SYSCFGEN	   (RCC_APB2ENR_SYSCFG_Mask)

#define RCC_APB1EN_USART2_Pos 	 (17u)
#define RCC_APB1EN_USART2_Mask   (1u << RCC_APB1EN_USART2_Pos)
#define RCC_APB1EN_USART2EN		   (RCC_APB1EN_USART2_Mask)

#define RCC_APB1EN_TIM2_Pos 	 (0u)
#define RCC_APB1EN_TIM2_Mask   (1u << RCC_APB1EN_TIM2_Pos)
#define RCC_APB1EN_TIM2EN		   (RCC_APB1EN_TIM2_Mask)

/*
  Flag Definations 
*/

/* USART bit POZISYONLARI (maske degil) - kullanimda (0x1U << X) seklinde kaydirilir */

/* CR1 */
#define USART_CR1_OVER8  (15u)
#define USART_CR1_UE     (13u)
#define USART_CR1_M      (12u)
#define USART_CR1_PCE    (10u)
#define USART_CR1_PS     (9u)
#define USART_CR1_TXEIE  (7u)
#define USART_CR1_TCIE   (6u)
#define USART_CR1_RXNEIE (5u)
#define USART_CR1_IDLEIE (4u)
#define USART_CR1_TE     (3u)
#define USART_CR1_RE     (2u)

/* CR2 */
#define USART_CR2_STOP  (12u)

/* CR3 */
#define USART_CR3_CTSE  (9u)
#define USART_CR3_RTSE  (8u)

/* SR */
#define USART_SR_TXE    (7u)
#define USART_SR_TC     (6u)
#define USART_SR_RXNE   (5u)

#include "Rcc.h"
#include "Gpio.h"

#endif /* INC_STM32F407XX_H_ */
