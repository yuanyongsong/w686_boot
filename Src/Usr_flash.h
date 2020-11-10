#ifndef USR_FLASH_H
#define USR_FLASH_H

#define DATABUFLEN 150
#define SCIBUFLEN  200

#define STM32_FLASH_SIZE 	128 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_BASE 	0x08000000 	//STM32 FLASH的起始地址
#define FLASH_APP1_ADDR		0x08002000  	//APP起始地址

#define FLASH_SAVE_ADDR  	0X08022000//第126K 127K为模拟EEPROM数据，根据不同芯片flash大小更改 2017-10-8		




#ifndef _FS_UPG
#define _FS_UPG
typedef struct{
	char   	Ok[3];			  			//为"OK"表示从flash读出的数据有效，不是"OK"表示此flash没有初始化
	char 	AppFilePath[50];   		 	//远程升级文件在http服务器上的文件路径
	char 	AppFileName[50];   		 	//远程升级文件在http服务器上的文件名
	char   	AppIpAdress[50];   			//远程升级用http地址
	char   	HttpError[32];				//http下载文件中出现的错误原因

	unsigned char   UpgNeedSendGprs;	//0x01 时要求GPRS回复升级结果	
	unsigned char   UpgEnJamp;			//0xaa为程序已经更新完毕，可以跳转，0x55为程序接收完毕
	u32   			AppLenBuf;			//接收数据OK,存EXFLASH长度。
}FS_UPG;

#endif

extern FS_UPG FsUpg;


void FS_InitValue(void);
void FS_UpdateValue(void);
void FS_FactroyValueFile(void);
void FS_EepromWriteOneByte(unsigned char addr,unsigned char value);
unsigned char FS_EepromReadOneByte(unsigned char addr);
void STMFLASH_WriteFs(u32 WriteAddr,u64 *pBuffer,u16 NumToWrite);	
void STMFLASH_Read(u32 ReadAddr, u32 *pBuffer, u16 NumToRead);

#endif


