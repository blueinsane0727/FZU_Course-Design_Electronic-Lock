#include <REGX52.H>
#include "Delay.h"
#include "Nixie.h"

sbit DC_Motor = P1^0;
#define DC_MOTOR_RUN_TIME	50

void Open(void)
{
	unsigned int i = 0;
	DC_Motor = 1;
	for(i;i < DC_MOTOR_RUN_TIME;i++)
	{
		display_true();
		Delay(1);
	}
	DC_Motor = 0;
}

void Close(void)
{
	unsigned int i = 0;
	DC_Motor = 1;
	for(i;i < DC_MOTOR_RUN_TIME;i++)
	{
		display_lock();
		Delay(1);
	}
	DC_Motor = 0;
}