#include <Gpio.h>

namespace can::driver::gpio
{
	void GPIO_Init(GPIO_t* pGPIOx, const GPIO_Init_t& pInit)
	{
		//*1
		for(uint32_t pos = 0; pos < 16; pos++)
		{
			uint32_t tTemp  = (0X1U << pos);
			uint32_t cValue = ((uint32_t)pInit.pinNumber & tTemp);
			if(cValue == tTemp)
			{
				//MODER  *2
				uint32_t tRegValue = pGPIOx->MODER; //gecici degiskene ata
				tRegValue &= ~(0x3u << (pos * 2)); //clear
				tRegValue |= (pInit.mode << (pos * 2));
				pGPIOx->MODER = tRegValue;

				if(GPIO_Mode_t::MODE_OUTPUT == pInit.mode || GPIO_Mode_t::MODE_ALTERNATE_FUNC == pInit.mode) //output icin gerekli yapilar set edilecek
				{
					tRegValue = pGPIOx->OTYPER;
					tRegValue &= ~(0X1U << pos);
					tRegValue |= (pInit.otype << pos);
					pGPIOx->OTYPER = tRegValue;

					tRegValue = pGPIOx->OSPEEDR;
					tRegValue &= ~(0X3U << (pos * 2));
					tRegValue |= (pInit.ospeed << (pos * 2));
					pGPIOx->OSPEEDR = tRegValue;
				}

				tRegValue = pGPIOx->PUPDR;
				tRegValue &= ~(0X3U << (pos * 2));
				tRegValue |= (pInit.pupd << (pos * 2));
				pGPIOx->PUPDR = tRegValue;
			}
		}
	}

	void GPIO_Write_Pin(GPIO_t* pGPIOx, uint16_t pPinNumber, GPIO_PinState_t pPinState)
	{
		if(GPIO_Pin_Set == pPinState)
			pGPIOx->BSRR = pPinNumber;
		else if(GPIO_Pin_Reset == pPinState)
			pGPIOx->BSRR = (pPinNumber << 16U);
	}


	GPIO_PinState_t GPIO_Read_Pin(GPIO_t* pGPIOx, uint16_t pPinNumber)
	{
		GPIO_PinState_t returnValue = GPIO_Pin_Reset;

		if((pGPIOx->IDR & pPinNumber) != 0X0U)
			returnValue = GPIO_Pin_Set;

		return returnValue;
	}

	//*3
	void  GPIO_Toggle_Pin(GPIO_t* pGPIOx, uint16_t pPinNumber)
	{
		uint32_t tRegValue = pGPIOx->ODR;
		pGPIOx->BSRR = ((tRegValue & pPinNumber) << 16U)
				        | (~tRegValue & pPinNumber);
	}

}



/*
 *
 ******1 Pozisyon bilgisinin elde edilmesi. PinNumber 16 bit olarak set edildi.
 *
 * 			PIN0 -  0000 0000 0000 0001
 * 			PIN1 -  0000 0000 0000 0010 .......
 *			...
 *			PIN15 - 1000 0000 0000 0000
 *
 *			Init metodunda ilgili registerlara pinNumber verilerek set edilmesi beklenmektedir. Fakat 2^15 inanılmaz bir sayi ve register bulunmaz.
 *			Bu degerleri indirgeyerek registerdaki konumlarını bulmalıyız. Bu proplemin cozumu icin;
 *			for dongusu icerisinde 16 ya kadar gezilir. Set edilmis bitler 1 durumunda.
 *			Eger for dongusu icerisinde her biti sırayla andleyerek gezersem pozisyon bilgisini bulmus olurum.
 *
 ******2 Bazi registerlar 2 bit alan kaplar bazilari 1 bit.
 *			OR. Moder 2 bit alan kaplamaktadir. Eger pin No: 4 ise bunu x2 kadar ofsetlemem lazım cunku Pin4 un baslangic konumu 8. bitde yer almaktadırç.
 *			OTYPER registeri ise 1 bit alan kaplamaktadir. 16 - 31 reserved edilmis durumda. yani ben ilgili bit icin pos degerini kullanabilirim sadece.
 *
 ******3 Toogle anlamak icin; ODR registerini okuma yapacaz. ODR reginde set edilmisse 1 olmak zorunda;
 *		 eger set edilmis biti disable etmek istersekde. bsrr ininin 16 31 de karsıiligi olan reser pini set edilmelidir.
 *		 eger disable sa. onde ODR registerini tersine cevirirz. and kapisyla 1 olan biti buluruz. bulunan bit zaten BSRR inda ilgili kısmı set etmemiz halinde yanacaktir.
 *
 *		 OR :: pın2  baslangicta 1 iken; 0000 0000 0000 0100
 *		 0000 0000 0000 0100(reg) & 0000 0000 0000 0100 (pPİnNumber) ---> 0000 0000 0000 0100 bunu BSRR de 16 bit kaydırarak yazarsak istedigimiz 18 bite yazıyoruz buda 2. bitin reset tarafı.
 *
 *
 */
