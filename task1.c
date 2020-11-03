#include <lpc21xx.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

void ledTask1(void *);		//Task1 Function
void ledTask2(void *);		//Task2 Function
void ledTask3(void *);		//Task3 Function

void delay(int );
void blink_led(int );
void setupHardware(void);

int main()
{
	setupHardware();
	xTaskCreate(ledTask1,"Task1",configMINIMAL_STACK_SIZE,NULL,1,NULL);			//Task1 --> LED
	xTaskCreate(ledTask2,"Task2",configMINIMAL_STACK_SIZE,NULL,1,NULL);			//Task2 --> LED
	xTaskCreate(ledTask3,"Task3",100,NULL,1,NULL);			//Task3 --> LED	
	
	vTaskStartScheduler();
	
	/* whenever not enough HEAP memory is available...
	 Program reaches here */
	while(1)					//backGround process
	{
		blink_led(24);
	}
}

void ledTask1(void *param)
{
	while(1)					//T1 backGround process
	{
		blink_led(17);
	}
}

void ledTask2(void *param)
{
	while(1)					//T2 backGround process
	{
		blink_led(18);
	}
}

void ledTask3(void *param)
{
	while(1)					//T2 backGround process
	{
		blink_led(19);
	}
}

void setupHardware(void)
{
	IODIR1 |= (0xFF<<17);
}

void blink_led(int led)
{
		IOSET1 = 1<<led;  // High
		delay(300);
		IOCLR1 = 1<<led;   // Low
		delay(300);
}

void delay(int ct)
{
	int i,j;
	for(i=0;i<ct;++i)
	{
		for(j=0;j<6000;++j)
		{}
	}
}
