#include "usartConfig.h"

void usartConfig(void)
{
	RCC->APB1ENR |= (1<<17);
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	GPIOA->MODER |= (2<<4) | (2<<6);
	
	GPIOA->OSPEEDR |= (3<<4) | (3<<6);
	
	GPIOA->AFR[0] |= (7<<8);
	GPIOA->AFR[0] |= (7<<12);
	
	USART2->CR1 = 0x00;
	
	USART2->CR1 |= (1<<13);
	
	USART2->CR1 &= ~(1U << 12);
	
	USART2->CR1 |= (1<<2)|(1<<3);
	
	USART2->CR2 &= ~(3U<<12);
	
	USART2->BRR |= (7 << 0) | (24 << 4);

	USART2->CR1 |= (1<<5);
	
	NVIC_EnableIRQ(USART2_IRQn);
}
