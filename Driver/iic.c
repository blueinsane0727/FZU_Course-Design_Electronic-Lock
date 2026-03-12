#include <REGX52.H>
#include "Delay.h"

sbit IIC_SCL = P2^1;
sbit IIC_SDA = P2^0;


void iic_start(void)
{
	IIC_SDA=1;
	Delay_us(2);
	IIC_SCL=1;
	Delay_us(2);
	IIC_SDA=0;	
	Delay_us(2);
	IIC_SCL=0;
	Delay_us(2);
}


void iic_stop(void)
{	
	IIC_SDA=0;
	Delay_us(2);
	IIC_SCL=1;
	Delay_us(2);
	IIC_SDA=1;	
	Delay_us(4);		
}


void iic_ack(void)
{
	IIC_SCL=0;
	IIC_SDA=0;	
	Delay_us(2);
   	IIC_SCL=1;
	Delay_us(2);
	IIC_SCL=0;
}


void iic_nack(void)
{
	IIC_SCL=0;
	IIC_SDA=1;	
	Delay_us(2);
   	IIC_SCL=1;
	Delay_us(2);
	IIC_SCL=0;	
}


unsigned int iic_wait_ack(void)
{
	unsigned int time_temp=0;
	
	IIC_SCL=1;
	Delay_us(2);
	while(IIC_SDA)	
	{
		time_temp++;
		if(time_temp>100)
		{	
			iic_stop();
			return 1;	
		}			
	}
	IIC_SCL=0;
	return 0;	
}


void iic_write_byte(unsigned int dat)
{                        
    unsigned int i=0; 
	   	    
    IIC_SCL=0;
    for(i=0;i<8;i++)	
    {              
        if((dat&0x80)>0) 
			IIC_SDA=1;
		else
			IIC_SDA=0;
        dat<<=1; 	  
		Delay_us(2);
		IIC_SCL=1;
		Delay_us(2);
		IIC_SCL=0;	
		Delay_us(2);
    }	 
}


unsigned int iic_read_byte(unsigned int ack)
{
	unsigned int i=0,receive=0;
   	
    for(i=0;i<8;i++ )	
	{
        IIC_SCL=0; 
        Delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(IIC_SDA)receive++;   
		Delay_us(2);
    }					 
    if (!ack)
        iic_nack();
    else
        iic_ack();  
		  
    return receive;
}
