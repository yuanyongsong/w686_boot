#include "usr_main.h"

unsigned short Breakpointleng;

void FLASH_WriteEnable(void);
uint8_t FLASH_SendByte(uint8_t byte);
void FLASH_WaitForWriteEnd(void);


void FLASH_LowLevel_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	
 	GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void EXFLASH_SpiInit(void)
{

    LL_SPI_InitTypeDef  LL_SPI_InitStructure;

    FLASH_LowLevel_Init();

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);
    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();

    LL_SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV4;
    LL_SPI_InitStructure.BitOrder = LL_SPI_MSB_FIRST;
    LL_SPI_InitStructure.ClockPhase = LL_SPI_PHASE_2EDGE;
    LL_SPI_InitStructure.ClockPolarity = LL_SPI_POLARITY_HIGH;
    LL_SPI_InitStructure.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    LL_SPI_InitStructure.CRCPoly = 7;
    LL_SPI_InitStructure.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    LL_SPI_InitStructure.Mode = LL_SPI_MODE_MASTER;
    LL_SPI_InitStructure.NSS = LL_SPI_NSS_SOFT;
    LL_SPI_InitStructure.TransferDirection = LL_SPI_FULL_DUPLEX;

    LL_SPI_SetRxFIFOThreshold(SPI1,LL_SPI_RX_FIFO_TH_QUARTER);
    LL_SPI_Init(SPI1,&LL_SPI_InitStructure);

    LL_SPI_Enable(SPI1);
}

//檫除一个扇区内容（一个扇区大小为64k，即0x10000）
void FLASH_EraseSector(uint32_t SectorAddr)
{
    /*!< Send write enable instruction */
    FLASH_WriteEnable();

    /*!< Sector Erase */
    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /*!< Send Sector Erase instruction */
    FLASH_SendByte(FLASH_CMD_SE);
    /*!< Send SectorAddr high nibble address byte */
    FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /*!< Send SectorAddr medium nibble address byte */
    FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /*!< Send SectorAddr low nibble address byte */
    FLASH_SendByte(SectorAddr & 0xFF);
    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    FLASH_WaitForWriteEnd();
}

/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
void FLASH_EraseBulk(void)
{
    /*!< Send write enable instruction */
    FLASH_WriteEnable();

    /*!< Bulk Erase */
    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /*!< Send Bulk Erase instruction  */
    FLASH_SendByte(FLASH_CMD_BE);
    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    FLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle 
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "FLASH_PAGESIZE" value.
  * @retval None
  */
void FLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    /*!< Enable the write access to the FLASH */
    FLASH_WriteEnable();

    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /*!< Send "Write to Memory " instruction */
    FLASH_SendByte(FLASH_CMD_WRITE);
    /*!< Send WriteAddr high nibble address byte to write to */
    FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /*!< Send WriteAddr medium nibble address byte to write to */
    FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /*!< Send WriteAddr low nibble address byte to write to */
    FLASH_SendByte(WriteAddr & 0xFF);

    /*!< while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
      /*!< Send the current byte */
      FLASH_SendByte(*pBuffer);
      /*!< Point on the next byte to be written */
      pBuffer++;
    }

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();

    /*!< Wait the end of Flash writing */
    FLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
void EXFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % FLASH_SPI_PAGESIZE;
    count = FLASH_SPI_PAGESIZE - Addr;
    NumOfPage =  NumByteToWrite / FLASH_SPI_PAGESIZE;
    NumOfSingle = NumByteToWrite % FLASH_SPI_PAGESIZE;

    if (Addr == 0) /*!< WriteAddr is FLASH_PAGESIZE aligned  */
    {
        if (NumOfPage == 0) /*!< NumByteToWrite < FLASH_PAGESIZE */
        {
            FLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /*!< NumByteToWrite > FLASH_PAGESIZE */
        {
            while (NumOfPage--)
            {
                FLASH_WritePage(pBuffer, WriteAddr, FLASH_SPI_PAGESIZE);
                WriteAddr +=  FLASH_SPI_PAGESIZE;
                pBuffer += FLASH_SPI_PAGESIZE;
            }

        FLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else /*!< WriteAddr is not FLASH_PAGESIZE aligned  */
    {
        if (NumOfPage == 0) /*!< NumByteToWrite < FLASH_PAGESIZE */
        {
            if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > FLASH_PAGESIZE */
            {
                temp = NumOfSingle - count;

                FLASH_WritePage(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;

                FLASH_WritePage(pBuffer, WriteAddr, temp);
            }
            else
            {
                FLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /*!< NumByteToWrite > FLASH_PAGESIZE */
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / FLASH_SPI_PAGESIZE;
            NumOfSingle = NumByteToWrite % FLASH_SPI_PAGESIZE;

            FLASH_WritePage(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;

            while (NumOfPage--)
            {
                FLASH_WritePage(pBuffer, WriteAddr, FLASH_SPI_PAGESIZE);
                WriteAddr +=  FLASH_SPI_PAGESIZE;
                pBuffer += FLASH_SPI_PAGESIZE;
            }

            if (NumOfSingle != 0)
            {
                FLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void EXFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();

    /*!< Send "Read from Memory " instruction */
    FLASH_SendByte(FLASH_CMD_READ);

    /*!< Send ReadAddr high nibble address byte to read from */
    FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /*!< Send ReadAddr medium nibble address byte to read from */
    FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /*!< Send ReadAddr low nibble address byte to read from */
    FLASH_SendByte(ReadAddr & 0xFF);

    while (NumByteToRead--) /*!< while there is data to be read */
    {
        /*!< Read a byte from the FLASH */
        *pBuffer = FLASH_SendByte(FLASH_DUMMY_BYTE);
        /*!< Point to the next location where the byte read will be saved */
        pBuffer++;
    }

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

/**
  * @brief  Reads FLASH identification.
  * @param  None
  * @retval FLASH identification
  */
uint32_t FLASH_ReadID(void)
{
    uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();

    /*!< Send "RDID " instruction */
    FLASH_SendByte(0x9F);

    /*!< Read a byte from the FLASH */
    Temp0 = FLASH_SendByte(FLASH_DUMMY_BYTE);

    /*!< Read a byte from the FLASH */
    Temp1 = FLASH_SendByte(FLASH_DUMMY_BYTE);

    /*!< Read a byte from the FLASH */
    Temp2 = FLASH_SendByte(FLASH_DUMMY_BYTE);

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();

    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}

/**
  * @brief  Initiates a read data byte (READ) sequence from the Flash.
  *   This is done by driving the /CS line low to select the device, then the READ
  *   instruction is transmitted followed by 3 bytes address. This function exit
  *   and keep the /CS line low, so the Flash still being selected. With this
  *   technique the whole content of the Flash is read with a single READ instruction.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @retval None
  */
void FLASH_StartReadSequence(uint32_t ReadAddr)
{
    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();

    /*!< Send "Read from Memory " instruction */
    FLASH_SendByte(FLASH_CMD_READ);

    /*!< Send the 24-bit address of the address to read from -------------------*/
    /*!< Send ReadAddr high nibble address byte */
    FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /*!< Send ReadAddr medium nibble address byte */
    FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /*!< Send ReadAddr low nibble address byte */
    FLASH_SendByte(ReadAddr & 0xFF);
}


/**
  * @brief  Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t FLASH_SendByte(uint8_t byte)
{
    /*!< Loop while DR register in not emplty */
    while (LL_SPI_IsActiveFlag_TXE(SPI1) == 0);

    /*!< Send byte through the SPI1 peripheral */
    LL_SPI_TransmitData8(FLASH_SPI, byte);

    /*!< Wait to receive a byte */
    while (LL_SPI_IsActiveFlag_RXNE(FLASH_SPI) == 0);

    /*!< Return the byte read from the SPI bus */
    return LL_SPI_ReceiveData8(FLASH_SPI);
}

/**
  * @brief  Sends a Half Word through the SPI interface and return the Half Word
  *         received from the SPI bus.
  * @param  HalfWord: Half Word to send.
  * @retval The value of the received Half Word.
  */
uint16_t FLASH_SendHalfWord(uint16_t HalfWord)
{
    /*!< Loop while DR register in not emplty */
    while (LL_SPI_IsActiveFlag_TXE(SPI1) == 0);

    /*!< Send Half Word through the sFLASH peripheral */
    LL_SPI_TransmitData16(FLASH_SPI, HalfWord);

    /*!< Wait to receive a Half Word */
    while (LL_SPI_IsActiveFlag_RXNE(FLASH_SPI) == 0);

    /*!< Return the Half Word read from the SPI bus */
    return LL_SPI_ReceiveData16(FLASH_SPI);
}

/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void FLASH_WriteEnable(void)
{
    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();

    /*!< Send "Write Enable" instruction */
    FLASH_SendByte(FLASH_CMD_WREN);

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void FLASH_WaitForWriteEnd(void)
{
    uint8_t flashstatus = 0;

    /*!< Select the FLASH: Chip Select low */
    FLASH_CS_LOW();

    /*!< Send "Read Status Register" instruction */
    FLASH_SendByte(FLASH_CMD_RDSR);

    /*!< Loop as long as the memory is busy with a write cycle */
    do
    {
        /*!< Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        flashstatus = FLASH_SendByte(FLASH_DUMMY_BYTE);

    }
    while ((flashstatus & FLASH_WIP_FLAG) == SET); /* Write in progress */

    /*!< Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

void EXFLASH_Init(void)
{
    FLASH_EraseSector(BREAK_PIONT_ADDR_0);       //擦除flash
    FLASH_EraseSector(BREAK_PIONT_ADDR_1);       //擦除flash
}






