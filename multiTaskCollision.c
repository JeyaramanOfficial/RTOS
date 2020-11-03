#include "../lcd_header.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

void blink_led(int );
void task1(void *);
void task2(void *);

int main()
{
	IODIR1 |= (0xFF<<17);
	lcd_config();
	
	
	xTaskCreate(task1,"Task1",100,NULL,1,NULL);
	xTaskCreate(task2,"Task2",100,NULL,1,NULL);
	vTaskStartScheduler();
	
	while(1)
	{
		blink_led(24);
	}
}

void task1(void *param)
{
	while(1)
	{
		lcd_str("Task1");
		vTaskDelay(200);
	}
}

void task2(void *param)
{
	while(1)
	{
		lcd_str("Task2");
	}
}
void blink_led(int led)
{
		IOSET1 = 1<<led;  // High
		delay(300);
		IOCLR1 = 1<<led;   // Low
		delay(300);
}
