#include "usr_main.h"


FLAG Flag;

u32 FlashDestination;

void Usr_InitHardware(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    SystemClock_Config();

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DBGMCU);	
	LL_DBGMCU_EnableDBGStopMode();								
	LL_DBGMCU_DisableDBGStopMode();

	GPIO_init();
	UART_Init();
	EXFLASH_SpiInit();
}


void Usr_InitValue(void)
{	
	FS_InitValue(); 
}

int main()
{	
	
	Usr_InitHardware();

	printf("\r\n=============W686 BOOTLOADER==================");
	printf("\r\n===Software Built Time:%s %s===",__DATE__,__TIME__);
	printf("\r\n==============================================\r\n");
	Usr_InitValue();

	if(FsUpg.UpgEnJamp==0x55)
	{
		FlashDestination=FsUpg.AppLenBuf;
		printf("Start firmware update...\r\n");
		UPG_AppUprate(FlashDestination);
		printf("Firmware update complete!\r\n");
		FsUpg.UpgEnJamp=0xaa;

		memset(FsUpg.HttpError,0,32);
		strcpy(FsUpg.HttpError,"Https Upgrade Boot Ok!");


		Flag.NeedUpdateFs=1;
		FS_UpdateValue();				
	}
	else if((FsUpg.UpgEnJamp!=0x00)&&(FsUpg.UpgEnJamp!=0xFF)&&(FsUpg.UpgEnJamp!=0xAA))
	{
		FsUpg.UpgNeedSendGprs=1;			
		memset(FsUpg.HttpError,0,32);
		strcpy(FsUpg.HttpError,"Tftp Upgrade Boot Fail!");


		Flag.NeedUpdateFs=1;
		FS_UpdateValue();				
	}	
	iap_load_app(FLASH_APP1_ADDR);
}



