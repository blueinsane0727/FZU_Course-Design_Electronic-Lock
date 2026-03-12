#ifndef _iic_H
#define _iic_H
				 
void iic_start(void);			
void iic_stop(void);	  		
void iic_write_byte(unsigned int txd);	
unsigned int iic_read_byte(unsigned int ack);		
unsigned int iic_wait_ack(void); 			
void iic_ack(void);				
void iic_nack(void);			

#endif
