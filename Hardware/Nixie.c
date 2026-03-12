#include <REGX52.H>
#include "Delay.h"

unsigned char NixieTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x40,0x00,0x79,0x71,0x50,0x54};
//                          0    1    2    3    4    5     6    7    8    9    -    Пе   e    f    r    n

void Nixie(unsigned char Location,Number)
{
	switch(Location)
	{
		case 1:P2_4=1;P2_3=1;P2_2=1;break;
		case 2:P2_4=1;P2_3=1;P2_2=0;break;
		case 3:P2_4=1;P2_3=0;P2_2=1;break;
		case 4:P2_4=1;P2_3=0;P2_2=0;break;
		
	}
	P0=NixieTable[Number];
	Delay(1);
	P0=0x00;
}

void display_true(void)
{
	Nixie(1,0);
	Nixie(2,13);
	Nixie(3,13);
}

void display_lock(void)
{
	Nixie(1,0);
	Nixie(2,15);
	
}


void display_false(unsigned int err_time)
{
	unsigned int i = 0;
	for(i;i < 50;i++)
	{
		Nixie(1,12);
		Nixie(2,14);
		Nixie(3,14);
		Nixie(4,err_time);
	}
}


