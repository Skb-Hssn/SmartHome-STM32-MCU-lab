#include "gpioConfig.h"

void gpioConfig()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	for(int i = 0; i < 10; i++) {
		if(i == 3 || i == 4) continue;
		
		GPIOB->MODER |= (1U << (2*i));
	}
}
