#ifndef USR_UPGRADE_H
#define USR_UPGRADE_H


typedef  void (*iapfun)(void);				//定义一个函数类型的参数.


void iap_load_app(u32 appxaddr);			//执行flash里面的app程序
void iap_load_appsram(u32 appxaddr);		//执行sram里面的app程序
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
void UPG_AppUprate(u32 DST_ADDR);

#endif
