#ifndef ___i_
#define ___i_

#include "headers.h"

const u8 TIME[][6]={
	{0x3E,0x51,0x49,0x45,0x3E,0},  		//  '0'
   {0x00,0x42,0x7F,0x40,0x00,0},    	//  '1'
   {0x42,0x61,0x51,0x49,0x46,0},   	    //  '2'
   {0x21,0x41,0x45,0x4B,0x31,0},   	    //  '3'
   {0x18,0x14,0x12,0x7F,0x10,0},   	    //  '4'
   {0x27,0x45,0x45,0x45,0x39,0} ,  	    //  '5'
   {0x3C,0x4A,0x49,0x49,0x30,0},   	    //  '6'
   {0x01,0x71,0x09,0x05,0x03,0},       	//  '7'
   {0x36,0x49,0x49,0x49,0x36,0},   	    //  '8'
   {0x06,0x49,0x49,0x49,0x36,0},   	    //  '9' 
   {0x00,0x36,0x36,0x00,0x00,0},      	//  ':'
	 {0x20,0x10,0x08,0x04,0x02,0}         //   '/'
};

const u8 FORMAT[][6]={
	
	{0x7F,0x08,0x08,0x08,0x7F,0},      	//  'H'
   	{0x7F,0x08,0x08,0x08,0x7F,0},      	//  'H'
   	{0x7F,0x02,0x0C,0x02,0x7F,0},       	//  'M'
   	{0x7F,0x02,0x0C,0x02,0x7F,0},       	//  'M'
   	{0x46,0x49,0x49,0x49,0x31,0},     	//  'S'
    {0x46,0x49,0x49,0x49,0x31,0},     	//  'S'
		{0x7F,0x41,0x41,0x22,0x1C,0},   	    //  'D'
   	{0x7F,0x41,0x41,0x22,0x1C,0},   	    //  'D'
   	{0x7F,0x02,0x0C,0x02,0x7F,0},       	//  'M'
   	{0x7F,0x02,0x0C,0x02,0x7F,0},       	//  'M'
   	{0x07,0x08,0x70,0x08,0x07,0},        	//  'Y'
  	{0x07,0x08,0x70,0x08,0x07,0},           //  'Y'
		{0x07,0x08,0x70,0x08,0x07,0},        	//  'Y'
  	{0x07,0x08,0x70,0x08,0x07,0},            //  'Y'
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}			//::
};

#define Switch 1
extern char time[20];
extern char date[20];

int data,temp;

void Interrupt_Enable(void)
{
	SETBIT(IOPIN0,Switch);
	CFGPIN(PINSEL0,Switch,FUN4);   //p0.1 as interrupt raise 
	SSETBIT(VICIntEnable,14);
	SSETBIT(VICIntSelect,14);
	VICVectAddr=(unsigned)KPM_Scan;
	EXTINT=0x00;
	SETBIT(EXTMODE,0);
	//SETBIT(EXTPOLAR,0);  //for Protesus
	CLRBIT(EXTPOLAR,0);  //On kit
}

void KPM_Scan(void) __irq
{
	int k=3,i,j,FLAG=0,FLAG2=0;

	/**** Display the format --Set the cusour at same position where it present****/
		ChipSelect(CHIP_2);
		CmdGLCD(PAGE4);
    CmdGLCD(COL_ADDR+4); 
    
	for(i=0;i<6;i++)
	{
		for(j=0;j<6;j++)					  /****  HH:MM:SS   **/  
			 WriteGLCD(FORMAT[i][j]);
		if(i==1 || i== 3)
		{
		   for(j=0;j<6;j++)					  /****  :   **/  
			 WriteGLCD(TIME[10][j]);
		}
	}
	
		ChipSelect(CHIP_2);
		CmdGLCD(PAGE5);
    CmdGLCD(COL_ADDR+4); 
    for(i=6;i<14;i++)
		{
		for(j=0;j<6;j++)					  /****  DD/MM/YYYY   **/  
			 WriteGLCD(FORMAT[i][j]);
		if(i==7 || i== 9)
		{
		   for(j=0;j<6;j++)					  /****  /   **/  
			 WriteGLCD(TIME[11][j]);
		}
		}
		delay_ms(500); 
		StrGLCD(PAGE4,COL_ADDR+4,65,(S8)time);
		StrGLCD(PAGE5,COL_ADDR+4,65,(S8)date);

		ChipSelect(CHIP_2);
		CmdGLCD(PAGE4);
    CmdGLCD(COL_ADDR+4); 	
	while(k--)
	{
N:	temp=0;
		data=0;
		FLAG=1;
		FLAG2=0;
		for(i=0;i<2;i++)
		{
		if(k==2)
		{
		if(FLAG)
		{
			CmdGLCD(COL_ADDR+4);
			for(j=0;j<6;j++)
			 WriteGLCD(FORMAT[14][j]);
		if(k==2 && FLAG==0)
		{
			CmdGLCD(COL_ADDR+4);
			for(j=0;j<6;j++)
			 WriteGLCD(time[0][j]);
			}
		}
		else if(k==2 && FLAG2==1)
		{
			CmdGLCD(COL_ADDR+4);
			for(j=0;j<6;j++)
			 WriteGLCD(time[temp][j]);
		}
		else if(data && k==2 && FLAG)
		{
		CmdGLCD(COL_ADDR+9);
	``		for(j=0;j<6;j++)
			 WriteGLCD(FORMAT[14][j]);
			CmdGLCD(COL_ADDR+9);
			for(j=0;j<6;j++)
			 WriteGLCD(time[1][j]);
		}
		else if(data && k==2 && FLAG2==1)
		{
			CmdGLCD(COL_ADDR+9);
	``		for(j=0;j<6;j++)
			 WriteGLCD(FORMAT[14][j]);
			CmdGLCD(COL_ADDR+9);
			for(j=0;j<6;j++)
			 WriteGLCD(time[temp][j]);
		}
		}
		if(k==1)
		{
		if(FLAG)
		{
			CmdGLCD(COL_ADDR+19);
			for(j=0;j<6;j++)
			 WriteGLCD(FORMAT[14][j]);
		if(FLAG==0)
		{
			CmdGLCD(COL_ADDR+19);
			for(j=0;j<6;j++)
			 WriteGLCD(time[3][j]);
			}
		}
		else if(FLAG2==1)
		{
			CmdGLCD(COL_ADDR+19);
			for(j=0;j<6;j++)
			 WriteGLCD(time[temp][j]);
		}
		else if(k==2 && FLAG)
		{
		CmdGLCD(COL_ADDR+24);
	``		for(j=0;j<6;j++)
			 WriteGLCD(FORMAT[14][j]);
			CmdGLCD(COL_ADDR+24);
			for(j=0;j<6;j++)
			 WriteGLCD(time[4][j]);
		}
		else if(k==2 && FLAG2==1)
		{
			CmdGLCD(COL_ADDR+24);
	``		for(j=0;j<6;j++)
			 WriteGLCD(FORMAT[14][j]);
			CmdGLCD(COL_ADDR+24);
			for(j=0;j<6;j++)
			 WriteGLCD(time[temp][j]);
		}
		if(k==0)
		{
		 if(FLAG)
		{
			CmdGLCD(COL_ADDR+29);
			for(j=0;j<6;j++)
			 WriteGLCD(FORMAT[14][j]);
		if(FLAG==0)
		{
			CmdGLCD(COL_ADDR+29);
			for(j=0;j<6;j++)
			 WriteGLCD(time[6][j]);
			}
		}
		else if(FLAG2==1)
		{
			CmdGLCD(COL_ADDR+29);
			for(j=0;j<6;j++)
			 WriteGLCD(time[temp][j]);
		}
		else if(data && FLAG)
		{
		CmdGLCD(COL_ADDR+29);
	``		for(j=0;j<6;j++)
			 WriteGLCD(FORMAT[14][j]);
			CmdGLCD(COL_ADDR+29);
			for(j=0;j<6;j++)
			 WriteGLCD(time[6][j]);
		}
		else if(data && k==2 && FLAG2==1)
		{
			CmdGLCD(COL_ADDR+29);
	``		for(j=0;j<6;j++)
			 WriteGLCD(FORMAT[14][j]);
			CmdGLCD(COL_ADDR+29);
			for(j=0;j<6;j++)
			 WriteGLCD(time[temp][j]);
		}
		 }
			  
			if(!Colscan())
			{
				temp=Keyscan();
				if(temp!='D')
				{
					
					FLAG=1;
				}
				else
				{
					FLAG2=1;
					FLAG=0;
				}

			}
			for(j=0;j<6;j++)
			 WriteGLCD(TIME[temp][j]);		    
			data=data*10+temp;
		FLAG=1;
		FLAG2=0;
		}
		
	if(k==2)
	{
		if(data>23)
		{
			delay_ms(100);
			StrGLCD(PAGE4,COL_ADDR+4,65,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE4,COL_ADDR+4,65,(S8)"HH");
			CmdGLCD(PAGE4);
			CmdGLCD(COL_ADDR+4);
			goto N;
		}
		HOUR=data;//Initialized hour
	}
	else if(k==1)
	{
		if(data>59)
		{
			delay_ms(20);
			StrGLCD(PAGE4,COL_ADDR+22,65,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE4,COL_ADDR+22,65,(S8)"MM");
			CmdGLCD(PAGE4);
			CmdGLCD(COL_ADDR+22);
			goto N;
		}
		MIN=data;
	}
	else
	{
		if(data>59)
		{
			delay_ms(20);
			StrGLCD(PAGE4,COL_ADDR+40,65,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE4,COL_ADDR+40,65,(S8)"SS");
			CmdGLCD(PAGE4);
			CmdGLCD(COL_ADDR+40);
			goto N;
		}
		SEC=data;
	}
		if(k!=0)
		   for(j=0;j<6;j++)					  /****  :   **/  
				WriteGLCD(TIME[10][j]);
	}  


		ChipSelect(CHIP_2);
		CmdGLCD(PAGE5);
    CmdGLCD(COL_ADDR+4);
		
	k=3;
	while(k--)
	{
R:	temp=0;
		data=0;
		i=0;
	if(k==0)
				i-=2;
	for(;i<2;i++)
	{
		temp=Keyscan();
		for(j=0;j<6;j++)
			 WriteGLCD(TIME[temp][j]);
		data=data*10+temp;
	}
	if(k==2)
	{
		if(data>31)
		{
			delay_ms(20);
			StrGLCD(PAGE5,COL_ADDR+4,65,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE5,COL_ADDR+4,65,(S8)"DD");
			CmdGLCD(PAGE5);
			CmdGLCD(COL_ADDR+4);
			goto R;
		}
		DOM=data;
	}
	else if(k==1)
		{
		if(data>12)
		{
			delay_ms(20);
			StrGLCD(PAGE5,COL_ADDR+22,65,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE5,COL_ADDR+22,65,(S8)"MM");
			CmdGLCD(PAGE5);
			CmdGLCD(COL_ADDR+22);
			goto R;
		}
		MONTH=data;
	}
	else
		{
		if(data>3000)
		{
			delay_ms(20);
			StrGLCD(PAGE5,COL_ADDR+40,65,(S8)"YYYY");
			delay_ms(200);
			StrGLCD(PAGE5,COL_ADDR+40,65,(S8)"YYYY");
			CmdGLCD(PAGE5);
			CmdGLCD(COL_ADDR+40);
			goto R;
		}
		YEAR=data;
	}
	if(k==2 || k==1)
	for(j=0;j<6;j++)
		WriteGLCD(TIME[11][j]);
	}
		
	EXTINT=0x01;
	VICVectAddr=0;
}
#endif

