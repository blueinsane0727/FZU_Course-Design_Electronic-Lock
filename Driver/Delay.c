#include <intrins.h>

void Delay(unsigned int x)		
{
	while(x)
	{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
	x--;
	}
}

void Delay_us(unsigned int x)		
{
	while(x)
	{
		_nop_();
		_nop_();
		_nop_();
		x--;
	}
}
