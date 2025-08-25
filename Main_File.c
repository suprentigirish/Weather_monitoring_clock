#include "headers.h"

#define FOSC      12000000
#define CCCLK  	  5*FOSC
#define PPCLK  	  CCCLK/4
#define PREINT_VAL ((PPCLK/32768)-1)
#define PREFRAC_VAL (PPCLK - ((PREINT_VAL + 1) * 32768))

u8 Temp_i;
u8 Temp_f;
u32 Temp_V;
u8 Smoke_v;

int main()
{
		int i;
		InitRTC();
		InitGLCD();
		Initkpm();
		Init_Smoke();
	//	Init_Temp();
		PREINT=PREINT_VAL;
		PREFRAC=PREFRAC_VAL;
		CCR=0x01; 
	
    GLCD_Fill_Screen(CLEAR);
    CmdGLCD(COL_ADDR); 
    ChipSelect(CHIP_1);
	
	 	
	ShowImage(3);
	StrGLCD(PAGE2,COL_ADDR,0,(S8)"  MISSILE");
	StrGLCD(PAGE3,COL_ADDR,0,(S8)"     MAN");

	delay_ms(500);
	GLCD_Fill_Screen(0);

/*	ShowImage_PSPK();
	StrGLCD(PAGE6,COL_ADDR,0,(S8)"         PSPK     "); 
	StrGLCD(PAGE7,COL_ADDR,0,(S8)"      NITHIN RAJ  "); 
	delay_ms(250);
*/	GLCD_Fill_Screen(0);
	for(i=1;i<13;i++)
	{
		Animation(i);
		delay_ms(250);
		GLCD_Fill_Screen(0);
	}
	
	ShowImage(1);
	delay_ms(500);
	GLCD_Fill_Screen(0);

	
	GLCD_Fill_Screen(0);
	StrGLCD(PAGE3,COL_ADDR,0,(S8)"     NITHIN RAJ "); 
  StrGLCD(PAGE4,COL_ADDR,0,(S8)"     V22HE10-D5 "); 
	delay_ms(500);
	GLCD_Fill_Screen(0);
	StrGLCD(PAGE3,COL_ADDR,0,(S8)"  WEATHER MONITORING  "); 
  StrGLCD(PAGE4,COL_ADDR,0,(S8)"  * * *  CLOCK * * *  "); 
	delay_ms(500);
	GLCD_Fill_Screen(0);
	Interrupt_Enable();
	
	while(1)
	{
    StrGLCD(PAGE0,COL_ADDR,0,(S8)"  WEATHER MONITORING  "); 
    StrGLCD(PAGE1,COL_ADDR,0,(S8)"  * * *  CLOCK * * *  "); 
		
		
			StrGLCD(PAGE4,COL_ADDR,0,(S8)"   TEMP "); 
			IntGLCD(PAGE5,COL_ADDR+7,0,Temp_i);
			CharGLCD(PAGE5,COL_ADDR+18,'.',1);
			CharGLCD(PAGE5,COL_ADDR+23,Temp_f,1);
			CharGLCD(PAGE5,COL_ADDR+28,'c',1);
			Temp_Symbol(); 
	
		if(!Smoke())
		{
			StrGLCD(PAGE7,COL_ADDR,0,(S8)"SMOKE : ");
			StrGLCD(PAGE7,COL_ADDR,0,(S8)"SMOKE : DANGER  ");   
		}
		else
		{
			StrGLCD(PAGE7,COL_ADDR,0,(S8)"SMOKE : ");
			StrGLCD(PAGE7,COL_ADDR,0,(S8)"SMOKE : NO SMOKE");  
		}
		
		TIME_RTC();
		Temp_V=ReadTemp();                      //READING TEMPERATURE FROM DS18B20 USING 1-WIRE PROTOCOL
		Temp_i  = Temp_V>>4;	                  //GETTING INTEGER PART
		Temp_f=Temp_V&0x08?0x35:0x30;						//GETTING FRACTIONAL PART
	
	
	}
}
