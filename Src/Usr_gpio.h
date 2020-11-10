#ifndef USR_GPIO_H
#define USR_GPIO_H


#define RED_ON    		        LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_12)					
#define RED_OFF   		        LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_12)			
#define RED_NEG   		        LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_12)? LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_12):LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_12)

#define GREEN_ON    	        LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11)					
#define GREEN_OFF   	        LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_11)		
#define GREEN_NEG   	        LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_11)? LL_GPIO_ResetOutputPin(GPIOA,LL_GPIO_PIN_11):LL_GPIO_SetOutputPin(GPIOA,LL_GPIO_PIN_11)

void GPIO_init(void);

#endif
