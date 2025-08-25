                            /* lcd.c */                  
#include "types.h"
#include "delay.h"
#include "defines.h"
#include "lcd_defines.h"
#include <LPC21xx.h>

void WriteLCD(u8 dat)
{
	SCLRBIT(IOCLR0,LCD_RW);
	WRITEBYTE(IOPIN0,LCD_DATA,dat);
	SSETBIT(IOSET0,LCD_EN);
	delay_us(1);
	SCLRBIT(IOCLR0,LCD_EN);
	delay_ms(2);
}

void CmdLCD(u8 cmd)
{
	SCLRBIT(IOCLR0,LCD_RS);//select command register for writing to
	WriteLCD(cmd);//write to command register
}

void CharLCD(u8 Ascii)
{
	SSETBIT(IOSET0,LCD_RS); //select data register for writing to
	WriteLCD(Ascii);  //write to data register then to DDRAM 
	               //for display
}	

void InitLCD(void)
{
	WRITEBYTE(IODIR0,LCD_DATA,0xFF);
	SETBIT(IODIR0,LCD_RS);
	SETBIT(IODIR0,LCD_RW);
	SETBIT(IODIR0,LCD_EN);
	
	delay_ms(15);
//	delay_us(100);
	CmdLCD(0x30);
	delay_ms(5);
//	delay_us(200);
	CmdLCD(0x30);
	delay_us(100);
	CmdLCD(0x30);
	CmdLCD(0x38);   //	 CmdLCD(MODE_8BIT_2LINE)
	CmdLCD(0x0F);	//	 CmdLCD(DISP_ON_CUR_BLK)
	CmdLCD(0x01);	//	 CmdLCD(CLRLCD)
	CmdLCD(0x06);	//	 cMDLCD(SFT_CUR_RGT)
	CmdLCD(0x0C);	//	 CmdLCD(DISP_ON_CUR_OFF)
	return;
}	

void StrLCD(u8 *ptr)
{	while(*ptr)	
		CharLCD(*ptr++); 
}

void U32LCD(u32 n)
{
	u8 a[10];	
	s16 i=0;
	
	if(n==0)
		CharLCD('0');
	else
	{
		while(n>0)
		{	
		 a[i]=(n%10)+48;
		 n=n/10;
		 i++;
		}
    for(--i;i>=0;i--)
        CharLCD(a[i]);		
  }		
}
void F32LCD(f32 f,u8 nDP)
{
	u32 i; 
	s8 j;
	if(f<0.0)
	{
		CharLCD('-');
		f=-f;
	}
	i=f;
	U32LCD(i);
	CharLCD('.');
	for(j=0;j<nDP;j++)
	{
		f=(f-i)*10;
		i=f;
		CharLCD(i+48);
	}
}	
void S32LCD(s32 n)
{
	if(n<0)
	{
	  CharLCD('-');
  	  n=-n;
	}
  U32LCD(n);	
}
void BuildCGRAM(u8 *p,u8 n)
{
	u8 i;
	//point cursor to CGRAM beginning
	CmdLCD(CGRAM_START_ADDR);
	for(i=0;i<n;i++)
	{
		//write cgram via data register
		CharLCD(p[i]);
	}
	//point back cursor to DDRAM
	CmdLCD(GOTO_LINE1_POS0);
}  
void BinLCD(u32 n,u8 nBD)
{
	s32 i;
	for(i=nBD-1;i>=0;i--)
		CharLCD(((n>>i)&1)+'0');
}
 void HexLCD(u32 n)
 {
	 s8 i=0,t;
	 u8 s[8]; 
	CharLCD('0');
	CharLCD('x');

	 while(n)
	 {
	 	t=n%16;
		s[i++]=t>10 ? ((t-10)+'A') : (t+'0');
		n/=16;
	 }
	 for(i-=1;i>=0;i--)
	 	CharLCD(s[i]);			
 }
