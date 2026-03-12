#include "24c02.h"
#include "iic.h"
#include "Delay.h"


void at24c02_write_one_byte(unsigned int addr,unsigned int dat)
{				   	  	    																 
    iic_start();  
	iic_write_byte(0XA0);	//发送写命令	    	  
	iic_wait_ack();	   
    iic_write_byte(addr);	//发送写地址   
	iic_wait_ack(); 	 										  		   
	iic_write_byte(dat);	//发送字节    							   
	iic_wait_ack();  		    	   
    iic_stop();				
	Delay(10);	 
}

unsigned int at24c02_read_one_byte(unsigned int addr)
{				  
	unsigned int temp=0;		  	    																 
    iic_start();  
	iic_write_byte(0XA0);	//发送写命令	   
	iic_wait_ack(); 
    iic_write_byte(addr); 	//发送写地址  
	iic_wait_ack();	    
	iic_start();  	 	   
	iic_write_byte(0XA1); 	//进入接收模式         			   
	iic_wait_ack();	 
    temp=iic_read_byte(0);	//读取字节		   
    iic_stop();				 
	return temp;			
}

