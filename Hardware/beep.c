#include <REGX52.H>
#include "Delay.h"
#include "timer0.h"

sbit BEEP = P2^5;

unsigned int code Freqtable[]={0,63628,63731,63835,63928,64021,64103,64185,64260,64331,
64400,64463,64528,64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,
65000,65030,65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,
65283};

unsigned char FreqSelect,MusicSelect1;

unsigned char code Music1[]={
//1
30,4,
28,2,
26,4,
28,2,
//2
30,3,
31,1,
30,2,
28,5,

0xFF};

void BeepWorking(void)
{
	Timer0Init();
    while(Music1[MusicSelect1]!=0xFF)
    {
        FreqSelect=Music1[MusicSelect1];
        MusicSelect1++;
        Delay(25*Music1[MusicSelect1]);	
        MusicSelect1++;
        TR0=0;
        Delay(5);
        TR0=1;
     }
     TR0=0;
     BEEP = 0;


}

void Timer0_Routine()	interrupt 1
{
	if(Freqtable[FreqSelect])
	{
		TL0=Freqtable[FreqSelect]%256;	//设置定时初值
		TH0=Freqtable[FreqSelect]/256;	//设置定时初值
		BEEP=!BEEP;
	}
	
}
