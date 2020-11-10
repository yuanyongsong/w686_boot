#ifndef USR_UPGRADE_H
#define USR_UPGRADE_H


typedef  void (*iapfun)(void);				//定义一个函数类型的参数.

#define FLASH_APP1_ADDR		0x08002000  	//第一个应用程序起始地址(存放在FLASH)
											//保留0X08000000~0X0800FFFF的空间为IAP使用

void iap_load_app(u32 appxaddr);			//执行flash里面的app程序
void iap_load_appsram(u32 appxaddr);		//执行sram里面的app程序
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
void UPG_AppUprate(u32 DST_ADDR);

#endif
