#include "../lcd_header.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void blink_led(int );
void buzz(void);
void senderTask(void *);
void receiverTask(void *);
TaskHandle_t stask,rtask;
QueueHandle_t Queue;

int main()
{
	IODIR1 |= (0xFF<<17);
	IODIR0 |= (1<<11);
	lcd_config();
	Queue = xQueueCreate(3,5);
	xTaskCreate(senderTask,"STask",100,NULL,2,&stask);
	xTaskCreate(receiverTask,"RTask",100,NULL,1,&rtask);
	
	vTaskStartScheduler();
	
	while(1)
	{
		blink_led(24);
	}
}

void senderTask(void *param)
{
	xQueueSendToBack(Queue, "M1", 1000000);
	xQueueSendToFront(Queue, "M2", 1000000);
	xQueueSendToBack(Queue, "M3", 1000000);
	while(1)
	{
		for(int i=0;i<5;i++)
		{
		blink_led(17);
		}
//		vTaskDelay(1000);
		vTaskSuspend(NULL);
	}
		
}

void receiverTask(void *param)
{
	char buff[5];
	BaseType_t status;
	while(1)
	{
		status  = xQueueReceive(Queue,buff,1000000);
		if(status)
		{
			buzz();
			lcd_str(buff);
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

void buzz(void)
{
	IOSET0 = 1<<11;
	delay(100);
	IOCLR0 = 1<<11;
	delay(100);
}
