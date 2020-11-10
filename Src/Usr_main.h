#ifndef USR_MAIN_H
#define USR_MAIN_H


#include "stm32g0xx_ll_adc.h"
#include "stm32g0xx.h"
#include "stm32g0xx_ll_i2c.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_spi.h"
#include "stm32g0xx_ll_tim.h"
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_rtc.h"
#include "stm32g0xx_ll_flash.h"

#include "SEGGER.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Usr_flash.h"
#include "Usr_gpio.h"
#include "Usr_timer.h"
#include "Usr_uart.h"
#include "Usr_exflash.h"
#include "Usr_upgrade.h"


#ifndef _FLAG_
#define _FLAG_
typedef struct FLAG_
{
	unsigned char NeedUpdateFs:1;     //需要跟新Fs内容

	
}FLAG;
#endif

extern FLAG Flag;

#endif

