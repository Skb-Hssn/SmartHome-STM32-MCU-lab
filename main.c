/*
 * Author : Sakib Hassan
 * CSEDU25, University of Dhaka.
 *
 */


#include "Configuration.h"
#include "gpioConfig.h"

void TIM7_IRQHandler(void);

void USART2_IRQHandler(void);

void sendChar(uint8_t x);

uint8_t recieveChar(void);

void configureHome(void);

void setPin(GPIO_TypeDef* gpio, int pin);

void resetPin(GPIO_TypeDef* gpio, int pin);

static char configuration[150];

static int length = 0;

static struct Configuration config;

static int heaterRunning = 0;
static int coolerRunning = 0;

void runCooler(void);
void stopCooler(void);
void runHeater(void);
void stopHeater(void);


static int pumpRunning = 0;
static int humidifierRunnin = 0;

void runPump(void);
void stopPump(void);
void runHumidifier(void);
void stopHumidifier(void);


static int lightOn[4] = {0, 0, 0, 0};

void runLight(void);
void stopLight(void);


// static int day = 1;


void setPin(GPIO_TypeDef* gpio, int pin)
{
	gpio->BSRR |= (1U << pin);
}

void resetPin(GPIO_TypeDef* gpio, int pin)
{
	gpio->BSRR |= ((1U << pin) << 16);
}

void sendChar(uint8_t data){
	USART2->DR = data;
	while(!(USART2->SR & (1<<6)));
}


uint8_t recieveChar(){
	while(!(USART2->SR & (1 << 5)));
	return (uint8_t)USART2->DR;
}

void USART2_IRQHandler(void){
	if(USART2->SR & (1<<5))
  {
    configuration[length] = (uint8_t) USART2->DR;
		length++;
		USART2->SR &= ~(1U<<5);
  }
	
}

static int timerCounter = 0;

void TIM7_IRQHandler(void) {
	
	timerCounter++;
	if(TIM7->SR & (1)) {
		if(timerCounter >= 10000) {
			
			if(coolerRunning) {
				stopCooler();
				config.temp = 25;
			}
			
			if(heaterRunning) {
				stopHeater();
				config.temp = 25;
			}
			
			timerCounter = 0;
		}
		TIM7->SR &= ~(1U);
	}
}


/* -----------------TEMPARATURE------------------*/
/* PB0 */

void runCooler()
{
	setPin(GPIOB, 0);
	
	TIM7->CR1 |= TIM_CR1_CEN;
	TIM7->SR &= ~(1U);
	timerCounter = 0;
	coolerRunning = 1;
}


void stopCooler()
{
	coolerRunning = 0;
	resetPin(GPIOB, 0);
}

/* PB1 */
void runHeater()
{
	setPin(GPIOB, 1);
	
	TIM7->CR1 |= TIM_CR1_CEN;
	TIM7->SR &= ~(1U);
	timerCounter = 0;
	heaterRunning = 1;
}

void stopHeater()
{
	heaterRunning = 0;
	resetPin(GPIOB, 1);
}


/*----------------PUMP------------*/
/* PB2 */
void runPump()
{
	if(!pumpRunning) {
		pumpRunning = 1;
		setPin(GPIOB, 2);
	}
}

void stopPump(void)
{
	if(pumpRunning) {
		pumpRunning = 0;
		resetPin(GPIOB, 2);
	}
}


/*-------------Humidifier--------------*/
/* PB8 */
void runHumidifier()
{
	if(!humidifierRunnin) {
		humidifierRunnin = 1;
		setPin(GPIOB, 8);
	}
}

void stopHumidifier(void)
{
	if(humidifierRunnin) {
		humidifierRunnin = 0;
		resetPin(GPIOB, 8);
	}
}


/*--------------LIGHT--------------------*/
/* PB9 */
void runLight()
{
	for(int i = 0; i < 4; i++) {
		if(config.light[i] == 1 && !lightOn[i]) {
			setPin(GPIOB, 4+i);
			lightOn[i] = 1;
			if(i == 0) {
				setPin(GPIOB, 9);
			}
		}
		
	}
}

void stopLight()
{
	for(int i = 0; i < 4; i++) {
		if(config.light[i] == 0 && lightOn[i]) {
			resetPin(GPIOB, 4+i);
			lightOn[i] = 0;
			if(i == 0) {
				resetPin(GPIOB, 9);
			}
		}
	}
}


void configureHome(void)
{
	
	/* TEMPARATURE */
	if(config.temp > 27) {
		
		if(heaterRunning) {
			stopHeater();
		}
		
		if(!coolerRunning) {
			runCooler();
		}
		
	} else if(config.temp < 23) {
		if(coolerRunning) {
			stopCooler();
		}
		
		if(!heaterRunning) {
			runHeater();
		}
	} else {
		if(coolerRunning) {
			stopCooler();
		}
		if(heaterRunning) {
			stopHeater();
		}
	}
	
	
	if(config.water < 45) {
		if(!pumpRunning) {
			runPump();
		}
	} else if(config.water > 55) {
		if(pumpRunning) {
			stopPump();
		}
	}
	
	if(config.humidity > 55 || config.humidity < 45) {
		runHumidifier();
	} else {
		stopHumidifier();
	}
	
	runLight();
	
	stopLight();
}


int main()
{
	clockConfig();
	
	timerConfig();
	
	usartConfig();
	
	gpioConfig();
		
	config.temp = 0;
	config.water = 0;
	config.humidity = 0;
	config.light[0] = 0;
	config.light[1] = 0;
	config.light[2] = 0;
	config.light[3] = 0;
	config.dayNight = 0;
	
	static char retConfig[150];
	
	
	while(1) {
		if(length > 0 && configuration[length-1] == '.') {
			retConfig[0] = 0;
			retConfig[0] = 0;
			parseConfiguration(configuration, &config, retConfig);
			
			if(retConfig[0] != 0) {
				int j = 0;
				while(retConfig[j] != '.') sendChar(retConfig[j]), j++;
			}
			
			length = 0;
			configureHome();
		}
	}
	
}
