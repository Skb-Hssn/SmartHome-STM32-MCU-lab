#include "timerConfig.h"

void timerConfig(void) 
{
	
	RCC->APB1ENR |= (1 << 5);
	
	TIM7->PSC = (90 - 1);
	
	TIM7->ARR = 1000;
	
	TIM7->CR1 |= TIM_CR1_CEN;
	
	TIM7->DIER |= (1);
	
	while(!(TIM7->SR & (1<<0)));
	
	NVIC_EnableIRQ(TIM7_IRQn);
}
