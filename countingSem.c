#include "../lcd_header.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

void blink_led(int );
void producerTask(void *);
void consumerTask(void *);

SemaphoreHandle_t sem_key;
int main()
{
	IODIR1 |= (0xFF<<17);
	lcd_config();
	
	sem_key = xSemaphoreCreateCounting(0,3);			//(init_val,MAX_LIMIT)
	
	xTaskCreate(producerTask,"PTASK",100,NULL,1,NULL);
	xTaskCreate(consumerTask,"CTASK",100,NULL,2,NULL);
	
	vTaskStartScheduler();
	
	while(1)
	{
		blink_led(24);
	}
}

void producerTask(void *param)
{
//	while(1)
	{
		blink_led(17);
//		lcd_cmd(0x80);
		lcd_str("PT");
		xSemaphoreGive(sem_key);
	}
}

void consumerTask(void *param)
{
	BaseType_t status;
	while(1)
	{
		status = xSemaphoreTake(sem_key,100000);
		if(status)
		{
			blink_led(18);
//			lcd_cmd(0xc0);
			lcd_str("CT");
		}
	}
}
void blink_led(int led)
{
		IOSET1 = 1<<led;  // High
		delay(300);
		IOCLR1 = 1<<led;   // Low
		delay(300);
}
