#include "Rcc.h"


static const uint8_t AHBPrescalerContainer[] = {0,0,0,0,0,0,0,0,1, 2, 3, 4, 6, 7 , 8 ,9};
static const uint8_t APBPrescalerContainer[] = {0,0,0,0,1,2,3,4};

namespace can::driver::rcc
{
    void setBit(volatile uint32_t &reg, uint32_t bitMask)
    {
    	SET_BIT(reg, bitMask);
    	volatile uint32_t dump = READ_BIT(reg,bitMask);
    	(void)dump;
    }

    void clearBit(volatile uint32_t &reg, uint32_t bitMask)
    {
    	CLEAR_BIT(reg, bitMask);
    }
    
    uint32_t getSystemClk()
    {
        uint32_t tSystemCoreClk   = 0;
        uint8_t  tSystemSourceClk = 0;
        
        tSystemSourceClk = (RCC->CFGR >> 2u) & (0x3u); //3 ve 4 bitleri alindi. 
        /*
        * PLL islemleri ele alinamdi !!! 
        */
        switch (tSystemSourceClk)
        {
        case 0x0u:
            tSystemCoreClk = 16000000;
            break;
        case 0x1u:
            tSystemCoreClk = 8000000;
            break;      
        default:
            tSystemCoreClk = 16000000;
            break;
        }

        return tSystemCoreClk;
    }
    
    uint32_t getHClock()
    {
        uint32_t tAHB_PeriphClk  = 0;
        uint32_t tSystemCoreClk = 0;

        tSystemCoreClk = getSystemClk();
        
        // RCC CFGR reginde yer alan 4 7 arasındaki bitleri kaydedecez. divided degeridir.  
        tAHB_PeriphClk = tSystemCoreClk >> AHBPrescalerContainer[(RCC->CFGR >> 4) & (0XFU)];
        return tAHB_PeriphClk;
    }

    uint32_t getAPB1Clock() //cfgr reginde yer alan PPRE1 regi low speed prescaler degerleini verecektir. 
    {
        uint32_t tAPB1Clk = 0;
        uint32_t tAHB_Clk;
        tAHB_Clk = getHClock();

        tAPB1Clk = tAHB_Clk >> APBPrescalerContainer[(RCC->CFGR >> 10) & (0x7u)];
        return tAPB1Clk;
    }

	uint32_t getAPB2Clock()
    {
        uint32_t tAPB2Clk = 0;
        uint32_t tAHB_Clk;
        tAHB_Clk = getHClock();

        tAPB2Clk = tAHB_Clk >> APBPrescalerContainer[(RCC->CFGR >> 13) & (0x7u)];
        return tAPB2Clk;
    }
}
