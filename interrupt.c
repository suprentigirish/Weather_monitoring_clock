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

int data,temp,FLAG;

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
	int i,j,Keyp;

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
AGAIN:	delay_ms(500); 
		GLCD_Fill_Screen(0);
		StrGLCD(PAGE0,COL_ADDR,0,(S8)"       MENU       ");
		StrGLCD(PAGE2,COL_ADDR,0,(S8)"1. Hours");
		StrGLCD(PAGE2,COL_ADDR,65,(S8)"2. Minutues");
		GLCD_Fill_Line(0,3);
		StrGLCD(PAGE4,COL_ADDR,0,(S8)"3. Seconds");
		StrGLCD(PAGE4,COL_ADDR,65,(S8)"4. Day");
		StrGLCD(PAGE6,COL_ADDR,0,(S8)"5. Month");
		StrGLCD(PAGE6,COL_ADDR,65,(S8)"6. Year");
		 Keyp=temp=data=FLAG=0;
		 Keyp=Keyscan(); 
		GLCD_Fill_Screen(0);
		ChipSelect(CHIP_1);
		CmdGLCD(PAGE4);
		CmdGLCD(COL_ADDR+22);
	switch(Keyp)
	{
		case 1 : 	StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"HH");
					ChipSelect(CHIP_2);
			for(i=0;i<2;i++)
					{
					temp=Keyscan();	
					delay_ms(100);
					for(j=0;j<6;j++)
			 		WriteGLCD(TIME[temp][j]);		    
					data=data*10+temp;
					}
					if(data>23)
					{
					delay_ms(100);
					StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"00");
					delay_ms(200);
					StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"HH");
					FLAG=1;
					}
					HOUR=data;//Initialized hour
					break;		 
		case 2 : 	StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"MM");
					ChipSelect(CHIP_2);
		for(i=0;i<2;i++)
					{
					temp=Keyscan();	
					delay_ms(100);
					for(j=0;j<6;j++)
			 		WriteGLCD(TIME[temp][j]);		    
					data=data*10+temp;
					}
					if(data>59)
		{
			delay_ms(20);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"MM");
					FLAG=1;
					}
					MIN=data;
					break;		 
		case 3 : 	StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"SS");
					ChipSelect(CHIP_2);
		for(i=0;i<2;i++)
					{
					temp=Keyscan();	
					delay_ms(100);
					for(j=0;j<6;j++)
			 		WriteGLCD(TIME[temp][j]);		    
					data=data*10+temp;
					}
					if(data>59)
		{
			delay_ms(20);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"SS");
					FLAG=1;
					}
					SEC=data;
					break;		 
		case 4 : 	StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"DD");
					ChipSelect(CHIP_2);
				for(i=0;i<2;i++)
					{
					temp=Keyscan();	
					delay_ms(100);
					for(j=0;j<6;j++)
			 		WriteGLCD(TIME[temp][j]);		    
					data=data*10+temp;
					}
					if(data>31)
		{
			delay_ms(20);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"DD");
			CmdGLCD(PAGE4);
			CmdGLCD(COL_ADDR+22);
					FLAG=1;
					}
					DOM=data;
					break;		 
		   case 5 : StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"MM");
					ChipSelect(CHIP_2);
					for(i=0;i<2;i++)
					{
					temp=Keyscan();	
					delay_ms(100);
					for(j=0;j<6;j++)
			 		WriteGLCD(TIME[temp][j]);		    
					data=data*10+temp;
					}
					if(data>12)
		{
			delay_ms(20);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"MM");
			CmdGLCD(PAGE4);
			CmdGLCD(COL_ADDR+22);
					FLAG=1;
					}
					MONTH=data;
					break;		 
		   case 6 : StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"YYYY");
					ChipSelect(CHIP_2);
					for(i=0;i<4;i++)
					{
					temp=Keyscan();
					delay_ms(100);	
					for(j=0;j<6;j++)
			 		WriteGLCD(TIME[temp][j]);		    
					data=data*10+temp;
					}
					if(data<1000)
		{
			delay_ms(20);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"00");
			delay_ms(200);
			StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"YYYY");
					FLAG=1;
					}
					YEAR=data;
					break;
			default :StrGLCD(PAGE4,COL_ADDR+22,0,(S8)"HH");
					ChipSelect(CHIP_2);
					 FLAG=1;
		}		 
		   if(FLAG)
		   {
		   	GLCD_Fill_Line(0,4);
		   	StrGLCD(PAGE4,COL_ADDR+0,0,(S8)"Provide Proper Data");
		   	goto AGAIN;
			}

		
	EXTINT=0x01;
	VICVectAddr=0;
}
#endif

