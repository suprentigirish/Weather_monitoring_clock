#include "headers.h"

#define FLAG 1 //0-->off KIT KEYPAD      1-->On KIT KEYPAD

u8 KPMLUT[4][4]={1,2,3,'A',4,5,6,'B',7,8,9,'C','*',0,'#','D'};
u8 KPMLUT1[4][4]={1,2,3,4,5,6,7,8,9,0,'*','#','A','B','C','D'};
u8 codes[4]={14,13,11,7};
//u32 temp1;

void Initkpm(void)
{
	WRITENIBBLE(IODIR1,ROW0,15);
}
u32 Colscan(void)
{
	u32 t;
	t=(READNIBBLE(IOPIN1,COL0))<15?0:1;
	return t;
}
u32	Keyscan(void)
{
	u8 i;
	u32 row,col,temp;
	while(Colscan());
	for(i=0;i<4;i++)
	{
		WRITENIBBLE(IOPIN1,ROW0,codes[i]);
		if(!Colscan())
		{
			row=i;
			break;
		}
	}
	WRITENIBBLE(IOPIN1,ROW0,0);
	for(i=0;i<4;i++)
	{
		if(READBIT(IOPIN1,(COL0+i))==0)
		{
			col=i;
			break;
		}
	}
	if(FLAG==1)
		temp=KPMLUT[row][col];
	else if(FLAG==0)
			temp=KPMLUT1[row][col];
	while(!Colscan());
	delay_ms(500);
	return temp;
}



					
