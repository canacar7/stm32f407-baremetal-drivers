/*
 * Usart.cpp
 *
 *  Created on: Sep 20, 2026
 *      Author: can
 */

#include <Usart.h>


namespace can::driver::usart
{

    static void CLOSE_USART_ISR(USART_Handle* pHandle, bool isTx)
    {
        if(isTx)
        {
            pHandle->mTxBufferSize  = 0;
            pHandle->mTxBuffer      = nullptr;
            pHandle->mTxStatus      = USART_BusState_t::USART_BUS_FREE;
            pHandle->mInstance->CR1 &= ~(0X1U << USART_CR1_TXEIE);  
        }
        else
        {
            pHandle->mRxBufferSize  = 0;
            pHandle->mRxBuffer      = nullptr;
            pHandle->mRxStatus      = USART_BusState_t::USART_BUS_FREE;
            pHandle->mInstance->CR1 &= ~(0X1U << USART_CR1_RXNEIE);  
        }
    }

    static void USART_SEND_WITH_IT(USART_Handle* pHandle)
    {
        if((USART_WordLength_t::WORD_LENGTH_9_BIT ==  pHandle->mInit.mWordLength) && (USART_Parity_t::NONE == pHandle->mInit.mParityBit))
        {
            uint16_t*  p16BitData = (uint16_t*)(pHandle->mTxBuffer);

            pHandle->mInstance->DR = (uint16_t)(*p16BitData & (uint16_t)(0x1ffu));
            pHandle->mTxBuffer += sizeof(uint16_t);
            pHandle->mTxBufferSize -= 2;
        }
        else
        {
            pHandle->mInstance->DR = (uint8_t)(*pHandle->mTxBuffer & (uint8_t)0XFFU);
            pHandle->mTxBuffer++;
            pHandle->mTxBufferSize -= 1;
        }

        if(pHandle->mTxBufferSize == 0)
        {
            CLOSE_USART_ISR(pHandle, true); //tx icin true
        }
    }



    static void USART_READ_WITH_IT(USART_Handle* pHandle)
    {
        uint16_t* t16BitData;
        uint8_t*  t8BitData;
        if((USART_WordLength_t::WORD_LENGTH_9_BIT ==  pHandle->mInit.mWordLength) && (USART_Parity_t::NONE == pHandle->mInit.mParityBit))
        {
            t16BitData = (uint16_t*)(pHandle->mRxBuffer);
            t8BitData  = nullptr;
        }
        else
        {
            t16BitData = nullptr;
            t8BitData = (uint8_t*)(pHandle->mRxBuffer); 
        }

        if(nullptr == t8BitData) //16 bite gore okuma yapcaz
        {
            *t16BitData = (uint16_t)(pHandle->mInstance->DR & 0x1ffu);
            pHandle->mRxBuffer += sizeof(uint16_t);
            pHandle->mRxBufferSize -= 2;
        }
        else
        {
            //9 bit ve parity var
            if((USART_WordLength_t::WORD_LENGTH_9_BIT ==  pHandle->mInit.mWordLength) && (USART_Parity_t::NONE != pHandle->mInit.mParityBit))
            {   
                *t8BitData = (uint8_t)(pHandle->mInstance->DR & 0XFFu);
                pHandle->mRxBuffer++;
                pHandle->mRxBufferSize--;                    
            }
            else if((USART_WordLength_t::WORD_LENGTH_8_BIT ==  pHandle->mInit.mWordLength) && (USART_Parity_t::NONE == pHandle->mInit.mParityBit))
            {
                *t8BitData = (uint8_t)(pHandle->mInstance->DR & 0XFFu);
                pHandle->mRxBuffer++;
                pHandle->mRxBufferSize--;  
            }
            else
            {
                *t8BitData = (uint8_t)(pHandle->mInstance->DR & 0X7Fu);
                pHandle->mRxBuffer++;
                pHandle->mRxBufferSize--;  
            }

        }

        if(pHandle->mTxBufferSize == 0)
        {
            //CLOSE_USART_ISR(pHandle, false); //rx iicn false
        }
    }

    void USART_Init(USART_Handle* pHandle)
    {
        uint32_t tAPBClk;
        uint32_t tValue = 0;
        uint32_t tUsartDivValue = 0;
        uint32_t tMantissaPart = 0;
        uint32_t tFractionPart = 0;
        uint32_t tVal = 0;

        tValue = pHandle->mInstance->CR1;
        // Kullanicinin "kapali" degerleri 0 oldugu icin OR ile bit temizlenemez.
        // Once ilgili alanlari sifirla, sonra istenen degeri yaz.
        tValue &= ~((0X1U << USART_CR1_OVER8) |   // oversampling
                    (0X1U << USART_CR1_M)     |   // word length
                    (0X3U << USART_CR1_PS)    |   // parity (PCE + PS)
                    (0X3U << USART_CR1_RE));      // mode (TE + RE)
        tValue |= static_cast<uint32_t>(pHandle->mInit.mOverSampling);
        tValue |= static_cast<uint32_t>(pHandle->mInit.mWordLength);
        tValue |= static_cast<uint32_t>(pHandle->mInit.mParityBit);
        tValue |= static_cast<uint32_t>(pHandle->mInit.mMode);
        pHandle->mInstance->CR1 = tValue;

        tValue = pHandle->mInstance->CR2;
        tValue &= ~(0X3U << USART_CR2_STOP);
        tValue |= static_cast<uint32_t>(pHandle->mInit.mStopBit);
        pHandle->mInstance->CR2 = tValue;
    
        tValue = pHandle->mInstance->CR3;
        tValue &= ~(0X3U << USART_CR3_RTSE);      // RTSE + CTSE
        tValue |= static_cast<uint32_t>(pHandle->mInit.mHardwareFLowControl);
        pHandle->mInstance->CR3 = tValue;
    
        //Bagli oldugu hatta gore baudrate hesaplamları farklılasacaktır. 
        //RCC reglerinde yer alan; CFGR reginde SW ve SWS alanları var. SW alani kullanici degistirerek clockt turunu degistirebilir.
        // SWS den ise kullancıı osilatorun durumunu alabilir. 
        if(USART6 == pHandle->mInstance || USART1 == pHandle->mInstance) //usart1 ve usart6 APB2 y ebagli 
        {
            tAPBClk = can::driver::rcc::getAPB2Clock();
        }
        else //diger usart hatlari apb1 e bagli. 
        {
            tAPBClk = can::driver::rcc::getAPB1Clock();
        }

        // reference manuel de oversampling 8 veya 16 ya gore 
        // farkli hesaplama yontemi ele alinmistir.
        if(USART_Sampling_t::OVERSAMPLING_8 == pHandle->mInit.mOverSampling)
        {
            tUsartDivValue = __USART_DIV_VALUE_8(tAPBClk, static_cast<uint32_t>(pHandle->mInit.mBaudRate)); //800.4
            tMantissaPart  = (tUsartDivValue / 100U); //8
            tFractionPart  = (tUsartDivValue) - (tMantissaPart * 100U);
            tFractionPart  = ((tFractionPart * 8U) + 50U) / 100U;
            // Yuvarlama 8'e tasarsa carry mantissa'ya aktarilmali,
            // maskeleyip atmak bir tam birim kaybettirirdi.
            if (tFractionPart >= 8U) { tMantissaPart++; tFractionPart = 0U; }
        }
        else
        {
            tUsartDivValue = __USART_DIV_VALUE_16(tAPBClk, static_cast<uint32_t>(pHandle->mInit.mBaudRate));
            tMantissaPart  = (tUsartDivValue / 100U);
            tFractionPart  = (tUsartDivValue) - (tMantissaPart * 100U);
            tFractionPart  = ((tFractionPart * 16U) + 50U) / 100U;
            if (tFractionPart >= 16U) { tMantissaPart++; tFractionPart = 0U; }

        }

        tVal |= (tMantissaPart << 4u);
        tVal |= (tFractionPart << 0u);
        pHandle->mInstance->BRR = tVal;

    }
 	
    void USART_TransmitData(USART_Handle* pHandle, uint8_t* pData, uint16_t dataSize)
    {
        uint16_t* tData16 = nullptr;
        if((USART_WordLength_t::WORD_LENGTH_9_BIT == pHandle->mInit.mWordLength) && 
            USART_Parity_t::NONE == pHandle->mInit.mParityBit)
        {
            tData16 = (uint16_t*)(pData);
        }
        else
        {
            tData16 = nullptr;
        }
        
        while (dataSize > 0)
        {
            // TXE=1 "veri register'i bos, siradakini verebilirsin" demek.
            // Dolayisiyla TXE RESET oldugu surece beklenir.
            while (USART_FlagStatus_t::USART_FLAG_RESET == getFlagStatus(pHandle, USART_SR_TXE))
            {
                /* bos - bekle */
            }
            if(nullptr == tData16)  
            {
                pHandle->mInstance->DR = (uint8_t)(*pData & 0xffu);
                dataSize--;
                pData++;
            }
            else //16 bitlik atama yaodık cunku 9 bit lik data degerimiz olabilir. 
            {
                // data gonderiminde ornegin 0000 0001 1000 0000 
                // ilk 9(1 1111 1111) biti 1 olan sayi ile andlersem ilk 9 bit gonderilcektir. dataSize 2 byte artırlacaktır. 
                pHandle->mInstance->DR = (uint16_t)(*tData16 & (0x1ffu));
                tData16++;
                // dataSize tek sayiysa 2 cikarmak uint16_t alt tasmasina yol acar.
                dataSize = (dataSize >= 2U) ? (uint16_t)(dataSize - 2U) : 0U;
            }            
        }

        // Son baytin kaydirma yazmacindan da cikmasini bekle.
        // TXE yeterli degil: TXE'de bayt henuz hatta cikmamis olabilir.
        while (USART_FlagStatus_t::USART_FLAG_RESET == getFlagStatus(pHandle, USART_SR_TC))
        {
            /* bos - bekle */
        }
    }

    USART_FlagStatus_t getFlagStatus(USART_Handle* pHandle, uint16_t flagName)
    {
        return (pHandle->mInstance->SR & (0x1u << flagName)) ? USART_FlagStatus_t::USART_FLAG_SET 
        : USART_FlagStatus_t::USART_FLAG_RESET;
    }

    void USART_PeriphCMD(USART_Handle* pHandle, FunctionalState_t pState)
    {
        if(FunctionalState_t::ENABLE == pState)
        {
               pHandle->mInstance->CR1 |= (0x1u << USART_CR1_UE);
        }
        else
        {
            pHandle->mInstance->CR1 &= ~(0x1u << USART_CR1_UE);
        }
    }

    void USART_TransmitDataIT(USART_Handle* pHandle, uint8_t* pData, uint16_t dataSize)
    {
        USART_BusState_t tUsartBusState = pHandle->mTxStatus;

        if(USART_BusState_t::USART_BUS_TX != tUsartBusState)
        {
            pHandle->mTxBuffer      = (uint8_t*)pData;
            pHandle->mTxBufferSize  = (uint16_t)dataSize;
            pHandle->mTxStatus      = USART_BusState_t::USART_BUS_TX;
            pHandle->TxISR_Function = USART_SEND_WITH_IT;

            pHandle->mInstance->CR1 |= (0x1u  << USART_CR1_TXEIE);
        }
    }

    void USART_ReceiveDataIT(USART_Handle* pHandle, uint8_t* pData, uint16_t dataSize)
    {
        USART_BusState_t tUsartBusState = pHandle->mRxStatus;
        if(USART_BusState_t::USART_BUS_RX != tUsartBusState)
        {
            pHandle->mRxBuffer      = (uint8_t*)pData;
            pHandle->mRxBufferSize  = (uint16_t)dataSize;
            pHandle->mRxStatus      = USART_BusState_t::USART_BUS_RX;
            pHandle->RxISR_Function = USART_READ_WITH_IT;

            pHandle->mInstance->CR1 |= (0x1u  << USART_CR1_RXNEIE);
        }
    }

	void USART_InterruptHandler(USART_Handle* pHandle)
    {
        // tx flage bak 
        // interrupt enable controlu
        uint8_t tFlagVal     = 0;
        uint8_t tInteruptVal = 0;

        tFlagVal      = static_cast<uint8_t>(pHandle->mInstance->SR >> 7u  & 0x1u); 
        tInteruptVal  = static_cast<uint8_t>(pHandle->mInstance->CR1 >> 7U & 0X1U);
        if(tFlagVal && tInteruptVal) //ikiside varsa interrupt gelmistir.
        {
            pHandle->TxISR_Function(pHandle);
            can::driver::gpio::GPIO_Toggle_Pin(GPIOD, GPIO_PIN_12);
        }

        //rx flag ve interrrup enable kontrolu yapilir. 
        tFlagVal     = static_cast<uint8_t>(pHandle->mInstance->SR >> 5U & 0X1U);
        tInteruptVal = static_cast<uint8_t>(pHandle->mInstance->CR1 >> 5U & 0X1U);
        if(tFlagVal && tInteruptVal)
        {
            pHandle->RxISR_Function(pHandle);
            can::driver::gpio::GPIO_Toggle_Pin(GPIOD, GPIO_PIN_15);
        }
    }

} // namespace can::driver::usart






























