#ifndef _24c02_H
#define _24c02_H

void at24c02_write_one_byte(unsigned int addr,unsigned int dat);
unsigned int at24c02_read_one_byte(unsigned int addr);

#endif
