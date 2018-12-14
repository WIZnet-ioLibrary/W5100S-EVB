#include "HAL_Config.h"
#include "W5100SRelFunctions.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"

DMA_InitTypeDef		DMA_RX_InitStructure, DMA_TX_InitStructure;

void W5100SInitialze(void)
{
	intr_kind temp;
	unsigned char W5100S_AdrSet[2][4] = {{8,0,0,0},{8,0,0,0}};
	/*
	 */
	temp = IK_DEST_UNREACH;

	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)W5100S_AdrSet) == -1)
	{
		printf("W5100S initialized fail.\r\n");
	}

	if(ctlwizchip(CW_SET_INTRMASK,&temp) == -1)
	{
		printf("W5100S interrupt\r\n");
	}

}

uint8_t spiReadByte(void)
{
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(W5100S_SPI, 0xff);
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(W5100S_SPI);
}

void spiWriteByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(W5100S_SPI, byte);
	while (SPI_I2S_GetFlagStatus(W5100S_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(W5100S_SPI);
}

uint8_t spiReadBurst(uint8_t* pBuf, uint16_t len)
{
	DMA_TX_InitStructure.DMA_BufferSize = len;
	DMA_TX_InitStructure.DMA_MemoryBaseAddr = pBuf;
	DMA_Init(W5100S_DMA_CHANNEL_TX, &DMA_TX_InitStructure);

	DMA_RX_InitStructure.DMA_BufferSize = len;
	DMA_RX_InitStructure.DMA_MemoryBaseAddr = pBuf;
	DMA_Init(W5100S_DMA_CHANNEL_RX, &DMA_RX_InitStructure);
	/* Enable SPI Rx/Tx DMA Request*/
	DMA_Cmd(W5100S_DMA_CHANNEL_RX, ENABLE);
	DMA_Cmd(W5100S_DMA_CHANNEL_TX, ENABLE);
	/* Waiting for the end of Data Transfer */
	while(DMA_GetFlagStatus(DMA_TX_FLAG) == RESET);
	while(DMA_GetFlagStatus(DMA_RX_FLAG) == RESET);


	DMA_ClearFlag(DMA_TX_FLAG | DMA_RX_FLAG);

	DMA_Cmd(W5100S_DMA_CHANNEL_TX, DISABLE);
	DMA_Cmd(W5100S_DMA_CHANNEL_RX, DISABLE);

}

void spiWriteBurst(uint8_t* pBuf, uint16_t len)
{
	DMA_TX_InitStructure.DMA_BufferSize = len;
	DMA_TX_InitStructure.DMA_MemoryBaseAddr = pBuf;
	DMA_Init(W5100S_DMA_CHANNEL_TX, &DMA_TX_InitStructure);

	DMA_RX_InitStructure.DMA_BufferSize = len;
	DMA_RX_InitStructure.DMA_MemoryBaseAddr = pBuf;
	DMA_Init(W5100S_DMA_CHANNEL_RX, &DMA_RX_InitStructure);

	DMA_Cmd(W5100S_DMA_CHANNEL_RX, ENABLE);
	DMA_Cmd(W5100S_DMA_CHANNEL_TX, ENABLE);

	/* Enable SPI Rx/Tx DMA Request*/



	/* Waiting for the end of Data Transfer */
	while(DMA_GetFlagStatus(DMA_TX_FLAG) == RESET);
	while(DMA_GetFlagStatus(DMA_RX_FLAG) == RESET);

	DMA_ClearFlag(DMA_TX_FLAG | DMA_RX_FLAG);

	DMA_Cmd(W5100S_DMA_CHANNEL_TX, DISABLE);
	DMA_Cmd(W5100S_DMA_CHANNEL_RX, DISABLE);

}


void busWriteByte(uint32_t addr, iodata_t data)
{
//	(*((volatile uint8_t*)(W5100SAddress+1))) = (uint8_t)((addr &0xFF00)>>8);
//	(*((volatile uint8_t*)(W5100SAddress+2))) = (uint8_t)((addr) & 0x00FF);
//	(*((volatile uint8_t*)(W5100SAddress+3))) = data;
//
	(*(volatile uint8_t*)(addr)) = data;

}


iodata_t busReadByte(uint32_t addr)
{
//	(*((volatile uint8_t*)(W5100SAddress+1))) = (uint8_t)((addr &0xFF00)>>8);
//	(*((volatile uint8_t*)(W5100SAddress+2))) = (uint8_t)((addr) & 0x00FF);
//	return  (*((volatile uint8_t*)(W5100SAddress+3)));
	return (*((volatile uint8_t*)(addr)));

}

void busWriteBurst(uint32_t addr, uint8_t* pBuf ,uint32_t len)
{

	DMA_TX_InitStructure.DMA_BufferSize = len;
	DMA_TX_InitStructure.DMA_MemoryBaseAddr = addr;
	DMA_TX_InitStructure.DMA_PeripheralBaseAddr = pBuf;


	DMA_Init(W5100S_DMA_CHANNEL_TX, &DMA_TX_InitStructure);

	DMA_Cmd(W5100S_DMA_CHANNEL_TX, ENABLE);

	/* Enable SPI Rx/Tx DMA Request*/



	/* Waiting for the end of Data Transfer */
	while(DMA_GetFlagStatus(DMA_TX_FLAG) == RESET);


	DMA_ClearFlag(DMA_TX_FLAG);

	DMA_Cmd(W5100S_DMA_CHANNEL_TX, DISABLE);



}


void busReadBurst(uint32_t addr,uint8_t* pBuf, uint32_t len)
{

		DMA_RX_InitStructure.DMA_BufferSize = len;
		DMA_RX_InitStructure.DMA_MemoryBaseAddr =pBuf;
		DMA_RX_InitStructure.DMA_PeripheralBaseAddr =addr;

		DMA_Init(W5100S_DMA_CHANNEL_RX, &DMA_RX_InitStructure);

		DMA_Cmd(W5100S_DMA_CHANNEL_RX, ENABLE);
		/* Waiting for the end of Data Transfer */
		while(DMA_GetFlagStatus(DMA_RX_FLAG) == RESET);


		DMA_ClearFlag(DMA_RX_FLAG);


		DMA_Cmd(W5100S_DMA_CHANNEL_RX, DISABLE);

}





inline void csEnable(void)
{
	GPIO_ResetBits(W5100S_CS_PORT, W5100S_CS_PIN);
}

inline void csDisable(void)
{
	GPIO_SetBits(W5100S_CS_PORT, W5100S_CS_PIN);
}

inline void resetAssert(void)
{
	GPIO_ResetBits(W5100S_RESET_PORT, W5100S_RESET_PIN);
}

inline void resetDeassert(void)
{
	GPIO_SetBits(W5100S_RESET_PORT, W5100S_RESET_PIN);
}

void W5100SReset(void)
{
	int i,j,k;
	k=0;
	GPIO_ResetBits(W5100S_RESET_PORT,W5100S_RESET_PIN);
	CoTickDelay(10);
	GPIO_SetBits(W5100S_RESET_PORT,W5100S_RESET_PIN);
}
