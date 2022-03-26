#include "gpioConfig.h"

void gpioConfig()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	for(int i = 0; i < 10; i++) {
		GPIOB->MODER |= (1U << (2*i));
	}
}
