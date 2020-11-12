#include "Usr_main.h"



iapfun jump2app; 
u16 iapbuf[1024];  
u16 iapread[1024];
u8  UpgPacketBuf[2048];
//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 appsize)
{
	u16 t;
	u16 i=0,j;
	u16 temp,k;
	u32 fwaddr=appxaddr;//当前写入的地址
	u8 *dfu=appbuf;
	static u8 packnumb = 1;		//完成写入的数据包

	for(t=0;t<appsize;t+=2)
	{						    
		temp=(u16)dfu[1]<<8;
		temp+=(u16)dfu[0];	  
		dfu+=2;//偏移2个字节
		iapbuf[i++]=temp;	    
		if(i==1024)
		{
			i=0;
			
			STMFLASH_WriteFs(fwaddr,(u64 *)iapbuf,256);	
			memset(iapread,0,1024);
			STMFLASH_Read(fwaddr,(u32 *)iapread,512);
			for(j=0;j<=40;j++)
			{
				k=memcmp(iapbuf,iapread,2048);
				if(k)
				{	
					printf("API data write failed, write again\r\n");
					STMFLASH_WriteFs(fwaddr,(u64 *)iapbuf,1024);	
					memset(iapread,0,1024);
					LL_mDelay(20);
					STMFLASH_Read(fwaddr,(u32 *)iapread,1024);
				}
				else 
				{
					printf("API %dk data write OK\r\n",packnumb*2);
					packnumb ++;
					break;				
				}

			}
			if(j>40)
			{
        printf("Firmware update failed, write failed, restart and update again!\r\n");
			  NVIC_SystemReset(); 
			}
			fwaddr+=2048;//偏移2048   16=2*8.所以要乘以2.
			LL_mDelay(20);
		}
	}
	if(i)
		{
		STMFLASH_WriteFs(fwaddr,(u64 *)iapbuf,i);//将最后的一些内容字节写进去.  
		memset(iapread,0,1024);
		STMFLASH_Read(fwaddr,(u32 *)iapread,i);
		for(j=0;j<=40;j++)
		{
			k=memcmp(iapbuf,iapread,i);
	//		k=0;
			
			if(k)
			{		
				printf("API data write failed, write again\r\n");
				STMFLASH_WriteFs(fwaddr,(u64 *)iapbuf,i);	
				memset(iapread,0,i);
				LL_mDelay(20);
				STMFLASH_Read(fwaddr,(u32 *)iapread,i);
			}
			else break;
		}
		if(j>40)
		{
          printf("Firmware update failed, write failed, restart and update again!\r\n");
		  NVIC_SystemReset(); 
		}
		}
}

__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
}	


void UPG_AppUprate(u32 DST_ADDR)
{
	u32 len=0,exflash_addr,stm32_addr;
	if(DST_ADDR<=EXFLASH_APP1_ADDR)
		return;
	len=DST_ADDR-EXFLASH_APP1_ADDR;
	exflash_addr=EXFLASH_APP1_ADDR;
	stm32_addr=FLASH_APP1_ADDR;
	while(1)
	{
		if(len>2048)
		{
			EXFLASH_ReadBuffer(UpgPacketBuf,exflash_addr,2048);
			iap_write_appbin(stm32_addr,UpgPacketBuf,2048);
			
			exflash_addr+=2048;
			stm32_addr+=2048;
			len-=2048;
		}
		else if(len==0)
		{
			break;
		}
		else
		{
			EXFLASH_ReadBuffer(UpgPacketBuf,exflash_addr,len);
			iap_write_appbin(stm32_addr,UpgPacketBuf,len);
			break;
		}
	}
	
}

