#include "usr_main.h"

FS_UPG FsUpg;
void STMFLASH_Write_NoCheck(u32 WriteAddr, u64 *pBuffer, u16 NumToWrite);

#define STM_SECTOR_SIZE 2048						//STM32G070一个扇区2K大小

void STMFLASH_WriteFs(u32 WriteAddr,u64 *pBuffer,u16 NumToWrite)	
{
	#if 1
	u32 secpos;  							//扇区地址
	u32 offaddr; 							//去掉0X08000000后的地址
	if (WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
	{
		return; 							//非法地址
	}
	LL_Flash_Unlock();						//解锁
	offaddr = WriteAddr - STM32_FLASH_BASE; //实际偏移地址.
	secpos = offaddr / STM_SECTOR_SIZE;		//所在扇区地址  0~63 for STM32G070RB

	LL_Flash_PageErase(secpos); 			//擦除这个扇区
	STMFLASH_Write_NoCheck(WriteAddr, pBuffer, NumToWrite);		  //写已经擦除了的,直接写入扇区剩余区间.

	LL_Flash_Lock(); 							//上锁
	#endif
}


//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:字(32位)数
void STMFLASH_Write_NoCheck(u32 WriteAddr, u64 *pBuffer, u16 NumToWrite)
{
	#if 1
	u16 i;
	LL_FLASH_EnableProgram();						//使能写入（允许写入）
	while (LL_FLASH_IsActiveFlag_BSY()) {}
	for (i = 0; i < NumToWrite; i++)
	{
		LL_FLASH_Program(ProgaraType_DATA64, WriteAddr, pBuffer[i]);
		WriteAddr += 8; 							//地址增加4.
	}
	while (LL_FLASH_IsActiveFlag_BSY()) {}
	LL_FLASH_DisenableProgram();					//禁止写入
	#endif
}



u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32 *)faddr;
}

void STMFLASH_Read(u32 ReadAddr, u32 *pBuffer, u16 NumToRead)
{
	#if 1
	u16 i;
	for (i = 0; i < NumToRead; i++)
	{
		pBuffer[i] = STMFLASH_ReadWord(ReadAddr); 		//读取2个字节.
		ReadAddr += 4;								  	//偏移2个字节.
	}
	#endif
}


void FS_InitValue(void)
{
	memset(&FsUpg,0,sizeof(FsUpg));
  	EXFLASH_ReadBuffer((u8 *)&FsUpg,FLASH_UPG_ADDR,sizeof(FsUpg));
	
	printf("\r\nAppIpAdress: %s\r\n",FsUpg.AppIpAdress);
	printf("AppFilePath:     %s\r\n",FsUpg.AppFilePath);
	printf("UpgEnJamp:       %02X\r\n",FsUpg.UpgEnJamp);
	printf("AppLen:          %02X\r\n",FsUpg.AppLenBuf);
	printf("HttpError:       %s\r\n",FsUpg.HttpError);

}



void FS_UpdateValue(void)
{
	if(!Flag.NeedUpdateFs) return;
	Flag.NeedUpdateFs=0;

	EXFLASH_WriteBuffer((u8 *)&FsUpg,FLASH_UPG_ADDR,sizeof(FsUpg));
}
