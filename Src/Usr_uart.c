#include "usr_main.h"


unsigned char 	DebugMskCnt;
unsigned short 	DebugIndex;
unsigned char 	DebugRecCnt;
u16				Obd_data_len=0;

unsigned short 	Uart1Index;
unsigned char 	Uart1RecCnt;

u16 mfofftime=0;
u8 mfontime=0;


//char UartObdBuf[UART4_BUF_LEN+1];
//char RecBuf[UART4_BUF_LEN+1];
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)                           
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int ch, FILE *f)
{      
	while((USART3->ISR&0X40)==0);
	{
		USART3->TDR = (unsigned char) ch;
	}
			
	return ch;
}
#endif 



//AT指令串口使用串口2，更换串口时要修改本函数
void UART_InitUartDebug(void)
{	
	LL_USART_InitTypeDef USART_InitStruct = {0};

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};


	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

	/*  USART3 GPIO Configuration  
	PB8   ------> USART3_TX
	PB9   ------> USART3_RX 
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USART3 interrupt Init */
	NVIC_SetPriority(USART3_4_IRQn, 1);
	NVIC_EnableIRQ(USART3_4_IRQn);

	USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	USART_InitStruct.BaudRate = 115200;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART3, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USART3);
	LL_USART_EnableIT_RXNE_RXFNE(USART3);


	LL_USART_Enable(USART3);

	/* Polling USART3 initialisation */
	while((!(LL_USART_IsActiveFlag_TEACK(USART3))) || (!(LL_USART_IsActiveFlag_REACK(USART3))))
	{
	}

}




void UART_Init(void)
{
	UART_InitUartDebug();
}


void USART3_4_IRQHandler(void)
{
    uint8_t tmp;

    if(LL_USART_IsActiveFlag_IDLE(USART3))
    {
      LL_USART_ClearFlag_IDLE(USART3);
    }

    if(LL_USART_IsActiveFlag_ORE(USART3)) 
    {
      LL_USART_ClearFlag_ORE(USART3);
    }

    if(LL_USART_IsActiveFlag_RXNE(USART3)) 
    {
		tmp = LL_USART_ReceiveData8(USART3); 
	}


    if(LL_USART_IsActiveFlag_IDLE(USART4))
    {
      LL_USART_ClearFlag_IDLE(USART4);
    }

    if(LL_USART_IsActiveFlag_ORE(USART4)) 
    {
      LL_USART_ClearFlag_ORE(USART4);
    }	
    
	if(LL_USART_IsActiveFlag_RXNE(USART4)) //检测是否接收中断
    {
		tmp = LL_USART_ReceiveData8(USART4); 
	}
}

