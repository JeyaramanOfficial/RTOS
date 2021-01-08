#include "../lcd_header.h"
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

void ext_isr(void)__irq;
void intr_init(void);
void blink_led(int);

void consumerTask(void *);

TaskHandle_t t1;
SemaphoreHandle_t sem;


int main()
{
	IODIR0 |= (1<<11);
	IODIR1 |= (0xFF<<17);
	lcd_init();
	intr_init();
	sem = xSemaphoreCreateBinary();
	xTaskCreate(consumerTask,"CTASK",100,NULL,1,&t1);
	vTaskStartScheduler();
	
	while(1)
	{
		blink_led(24);
	}
	
}

void consumerTask(void *param)
{
	unsigned int ret;
	while(1)
	{
		ret = xSemaphoreTake(sem,100000);
		if(ret)
		{
			IOSET0 = 1<<11;
			delay(100);
			IOCLR0 = 1<<11;
			delay(100);
			lcd_str("SEM Taken");
		}
		
	}

}
void ext_isr(void)__irq
{
	blink_led(20);
	xSemaphoreGiveFromISR(sem,NULL);
	EXTINT = 1<<1; //clear the intr flag bit.
	VICVectAddr = 0x00000000;
}

void intr_init(void)
{
	PINSEL0 = PINSEL0 | (1<<29); //p0.14	// 10 --> 29|28
	PINSEL0 = PINSEL0 & ~(1<<28);
	
	VICIntSelect &= ~(1<<15);   //EXT1 
	VICVectCntl2 = (1<<5)|15;
	VICVectAddr2 = (long)ext_isr;
	VICIntEnable = (1<<15);
	
	EXTMODE = 1<<1;  //p0.14//ISP --> EXT1 --> Edge Triggered
	EXTPOLAR &= ~(1<<1); //falling edge
/*	EXTPOLAR = (1<<1); //rising edge */
	
}



void blink_led(int led)
{
		IOSET1 = 1<<led;  // High
		delay(100);
		IOCLR1 = 1<<led;   // Low
		delay(100);
}
