#ifndef __HAL_CONFIG_H_
#define __HAL_CONFIG_H_

#include "stm32f10x_rcc.h"

#define SPI_1
//#define SPI_2  //W5100S EVB
//#define _WIZCHIP_IO_BASE_				0x60000003
#define PRINTF_USART			USART1
#define PRINTF_USART_RCC		RCC_APB2Periph_USART1
#define PRINTF_USART_TX_PIN		GPIO_Pin_9
#define PRINTF_USART_TX_PORT	GPIOA
#define PRINTF_USART_RX_PIN		GPIO_Pin_10
#define PRINTF_USART_RX_PORT	GPIOA

#define W5100S_GPIO_RCC			RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF
#define W5100S_RESET_PIN		GPIO_Pin_7
#define W5100S_RESET_PORT		GPIOC

#if defined(SPI_1)
	#define W5100S_CS_PIN			GPIO_Pin_6
	#define W5100S_CS_PORT			GPIOB
#else
	#define W5100S_CS_PIN			GPIO_Pin_7
	#define W5100S_CS_PORT			GPIOD
#endif

#define W5100S_INT_PIN			GPIO_Pin_9
#define W5100S_INT_PORT			GPIOD

#if defined(SPI_1)
	#define W5100S_DMA_CHANNEL_RX	DMA1_Channel2
	#define W5100S_DMA_CHANNEL_TX	DMA1_Channel3
#else
	#define W5100S_DMA_CHANNEL_RX	DMA1_Channel4
	#define W5100S_DMA_CHANNEL_TX	DMA1_Channel5
#endif

#if defined(SPI_1)
/* SPI 1*/
#define W5100S_SPI				SPI1
#define W5100S_SPI_RCC			RCC_APB2Periph_SPI1
#define W5100S_SPI_PORT			GPIOA

#define W5100S_SPI_SCK_PIN		GPIO_Pin_5
#define W5100S_SPI_MOSI_PIN		GPIO_Pin_7
#define W5100S_SPI_MISO_PIN		GPIO_Pin_6

#define DMA_TX_FLAG				DMA1_FLAG_TC3
#define DMA_RX_FLAG				DMA1_FLAG_TC2

#else
/* SPI2 */
#define W5100S_SPI				SPI2
#define W5100S_SPI_RCC			RCC_APB1Periph_SPI2
#define W5100S_SPI_PORT			GPIOB

#define W5100S_SPI_SCK_PIN		GPIO_Pin_13
#define W5100S_SPI_MOSI_PIN		GPIO_Pin_15
#define W5100S_SPI_MISO_PIN		GPIO_Pin_14

#define DMA_TX_FLAG				DMA1_FLAG_TC5
#define DMA_RX_FLAG				DMA1_FLAG_TC4

#endif

#endif
