/*
 * Usart.h
 *
 *  Created on: Sep 20, 2026
 *      Author: can
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#include  <stdint.h>
#include "stm32f407xx.h"


#define __USART_DIV_VALUE_8(__PCLK__, __BAUDRATE__)  (((25U * (__PCLK__)) / (2U * (__BAUDRATE__))))
#define __USART_DIV_VALUE_16(__PCLK__, __BAUDRATE__) (((25U * (__PCLK__)) / (4U * (__BAUDRATE__))))

enum class USART_FlagStatus_t : uint32_t
{
	USART_FLAG_RESET = 0X0U,
	USART_FLAG_SET   = 0X1U
};

enum class USART_MODE : uint32_t
{
	RX = (0X00000004U),
	TX = (0X00000008U),
	TX_RX = (0X0000000CU)
};

enum class USART_Interrupt_t : uint32_t
{
	TXE = USART_CR1_TXEIE,
	TC  = USART_CR1_TCIE,
	RXE = USART_CR1_RXNEIE
};

enum class USART_Sampling_t : uint32_t
{
    OVERSAMPLING_16 = 0,
    OVERSAMPLING_8  = (0X1U << 15U)
};

enum class USART_HardwareFlowControl_t : uint32_t
{
    HW_FLOW_NONE    = 0x00000000U,
    HW_FLOW_RTS     = (0x1U << 8U),
    HW_FLOW_CTS     = (0x1U << 9U),
    HW_FLOW_RTS_CTS = (0x1U << 8U) | (0x1U << 9U)
};


enum class USART_BaudRate_t : uint32_t
{
    BAUD_1200   = 1200U,
    BAUD_2400   = 2400U,
    BAUD_9600   = 9600U,
    BAUD_19200  = 19200U,
    BAUD_38400  = 38400U,
    BAUD_57600  = 57600U,
    BAUD_115200 = 115200U,
    BAUD_230400 = 230400U,
    BAUD_460800 = 460800U,
    BAUD_921600 = 921600U
};

enum class USART_WordLength_t : uint32_t
{
	WORD_LENGTH_8_BIT = (0X00000000U),
	WORD_LENGTH_9_BIT = (0x1u << 12u)
};

enum class USART_Parity_t : uint32_t
{
	NONE = (0X00000000U),
	EVEN = (0x1u << 10u),
	ODD  = (0X1U << 10U) | (0X1U << 9U)
};

enum class USART_StopBit_t : uint32_t
{
	STOP_BIT_1   = (0X0U << 12U),
	STOP_BIT_0_5 = (0x1u << 12u),
	STOP_BIT_2   = (0x2u << 12u),
	STOP_BIT_1_5 = (0x3u << 12u)
};

enum class  USART_BusState_t: uint8_t
{
	USART_BUS_FREE = 0X0u,
	USART_BUS_TX   = 0X1u,
	USART_BUS_RX   = 0X2u 
};


struct USART_Init_t
{
	USART_Sampling_t 				mOverSampling;
	USART_BaudRate_t		 		mBaudRate;
	USART_WordLength_t 		 		mWordLength;
	USART_Parity_t          		mParityBit;
	USART_StopBit_t         		mStopBit;
	USART_HardwareFlowControl_t     mHardwareFLowControl;
	USART_MODE		 				mMode;
};


struct USART_Handle
{
	USART_t*    		 mInstance = nullptr;
	USART_Init_t 		 mInit;
	uint8_t*    		 mTxBuffer;
	uint16_t	 		 mTxBufferSize;
	USART_BusState_t  	 mTxStatus;
	void 	    		 (*TxISR_Function)(struct USART_Handle* pHandle);
	uint8_t*			 mRxBuffer;
	uint16_t			 mRxBufferSize;
	USART_BusState_t	 mRxStatus;
	void				 (*RxISR_Function)(USART_Handle* pHandle);
};


namespace can::driver::usart
{
	void USART_Init(USART_Handle* pHandle);
	void USART_TransmitData(USART_Handle* pHandle, uint8_t* pData, uint16_t dataSize);

	void USART_TransmitDataIT(USART_Handle* pHandle, uint8_t* pData, uint16_t dataSize);
	void USART_ReceiveDataIT(USART_Handle* pHandle, uint8_t* pData, uint16_t dataSize);

	void USART_InterruptHandler(USART_Handle* pHandle);

	void USART_PeriphCMD(USART_Handle* pHandle, FunctionalState_t pState);
	USART_FlagStatus_t getFlagStatus(USART_Handle* pHandle, uint16_t flagName);
};

#endif /* INC_USART_H_ */
