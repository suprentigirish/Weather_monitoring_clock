
#include "headers.h"
#include "images.c"
#include "Animation.c"


void InitGLCD(void)
{
    WRITEBYTE(IODIR1,GLCD_DATA,0xFF);
    SETBIT(IODIR0,GLCD_RS);
    SETBIT(IODIR0,GLCD_RW);
    SETBIT(IODIR0,GLCD_EN);
    SETBIT(IODIR0,GLCD_RST);
    SETBIT(IODIR0,GLCD_CS1);
    SETBIT(IODIR0,GLCD_CS2);
    
    SSETBIT(IOCLR0,GLCD_RST);
    delay_ms(10);
    SSETBIT(IOSET0,GLCD_RST);
    
    ChipSelect(CHIP_1_2);
    CmdGLCD(GLCD_DISPLAY_OFF); // 0x3E turn it Off
    CmdGLCD(GLCD_DISPLAY_ON); // 0x3F Display ON
    CmdGLCD(COL_ADDR); // display data in horizontal way(0-63) 
    CmdGLCD(PAGE0); // display data in vertical way(0-7)
    CmdGLCD(GLCD_DISPLAY_START_LINE); // indicates the display data RAM displayed on the top of the screen                  
}

void CmdGLCD(u8 cmd)
{
    WRITEBYTE(IOPIN1,GLCD_DATA,cmd);
    SSETBIT(IOCLR0,GLCD_RW);
    SSETBIT(IOCLR0,GLCD_RS);    
    SSETBIT(IOSET0,GLCD_EN);
	delay_us(5);
    SSETBIT(IOCLR0,GLCD_EN);
    delay_us(20);
}

void CharGLCD(u8 page, u8 hposi,u8 ch,u8 v)
{
	unsigned char i,j;
	if(v==1)
		CmdGLCD(CHIP_1);
	else if(v==2)
		CmdGLCD(CHIP_2);
	CmdGLCD(hposi);
	CmdGLCD(page);
	j=ch-32;    //index 0 starting from character space (32)
	for(i=0;i<6;i++)
	{
		WriteGLCD(font[j][i]);
	}  
}

void WriteGLCD(u8 val)
{
    WRITEBYTE(IOPIN1,GLCD_DATA,val);
    SSETBIT(IOCLR0,GLCD_RW);
    SSETBIT(IOSET0,GLCD_RS);    
    SSETBIT(IOSET0,GLCD_EN);
	delay_us(5);
    SSETBIT(IOCLR0,GLCD_EN);
    delay_us(20);
}

void StrGLCD(u8 page, u8 hposi,int c,s8 *str)      //here c value is 65 then its goes to 2nd page ig c val is 0 then its is 1st page
{
    char i,l=0;                                       // here we give only either 0 or 65 as c val;
	int j,k;
//	char c=0;
    
    ChipSelect(CHIP_1_2);
	CmdGLCD(page);
    CmdGLCD(hposi);

	for(i=0;str[i];i++)
	{
		 j=str[i]-32;
		 for(k=0;k<6;k++)
		 {
		 	if(c<64)
			{
				ChipSelect(CHIP_1);
			 	WriteGLCD(font[j][k]);
				c++;
			}
			else
			{
			    ChipSelect(CHIP_2);
				WriteGLCD(font[j][k]);
				c++;
			}
			if(c>125)
			{
				l++;
        ChipSelect(CHIP_1_2);
				CmdGLCD(page+l);
				CmdGLCD(hposi);
				c=0;
			}	
		 }
	}    
}

void ChipSelect(u8 chip_sel)
{
   switch(chip_sel)
   {
     case 0: SSETBIT(IOCLR0,GLCD_CS1);
             SSETBIT(IOCLR0,GLCD_CS2); 
             break;
     case 1: SSETBIT(IOSET0,GLCD_CS1); 
	         SSETBIT(IOCLR0,GLCD_CS2); 
			 break;
     case 2: SSETBIT(IOCLR0,GLCD_CS1); 
	         SSETBIT(IOSET0,GLCD_CS2); 
			 break;
     case 3: SSETBIT(IOSET0,GLCD_CS1);
             SSETBIT(IOSET0,GLCD_CS2);
	         break;
     default: break;
   } 
    
}

void GLCD_Fill_Screen(u8 val)
{
   u8 i,y;  
   for(i=PAGE0;i<=PAGE7;i++)          
   {        
       ChipSelect(CHIP_1_2);
       CmdGLCD(i);
	   CmdGLCD(COL_ADDR);                      
       for(y=0;y<64;y++)
	       WriteGLCD(val); // Clear data in the LCD.
   }     
}

void IntGLCD(u8 page, u8 hposi,int c,s32 num)
{
    StrGLCD(page,hposi,c,itoa(num));
}

void FloatGLCD(u8 page, u8 hposi,int c,f32 val,u8 pos)
{
    int num,j;
    s8 *ptr;
    s8 f_arr[5]={'\0'};
    num=val;
    ptr = itoa(num);
    
    for(j=0;j<pos;j++)
	{
		val=(val-num)*10;
		num=val;
		f_arr[j] = (num+48);
	}
    f_arr[j]='\0';
    strcat((c8)ptr,(cc8)".");
    strcat((c8)ptr,(cc8)f_arr);
    StrGLCD(page,hposi,c,ptr);   
}

s8 *itoa(u32 num)
{
    static char arr[10];
    char i,len,temp,index=0;
    if(num==0)
    {
        arr[0] = '0';
        arr[1] = '\0';
        
    }
    else
    {
        while(num)
        {
            arr[index++] = (num%10) + 48;
            num/=10;
        }
        arr[index] = '\0';
        len=0;
        while(arr[len++]!='\0');
        len--;
        for (i = 0; i < len/2; i++)  
        {  
            temp = arr[i];  
            arr[i] = arr[len - i - 1];  
            arr[len - i - 1] = temp;  
        } 
        
    }
		return (s8 *)arr;
}

void ShowImage_PSPK(void)
{	
    int i,j;
    
    for(i=2;i<8;i++)
    {
        ChipSelect(CHIP_1_2);
        CmdGLCD(0xB8+i-2);
	    CmdGLCD(0x40);	
        ChipSelect(CHIP_1);  
        for(j=0;j<128;j++)
        {
            if(j==64)
                ChipSelect(CHIP_2); 
          WriteGLCD(PSPK[i][j]);
        }          
    }  
}

void ShowImage(u32 choice)
{	
    int i,j;
    
    for(i=0;i<8;i++)
    {
        ChipSelect(CHIP_1_2);
        CmdGLCD(0xB8+i);
	    CmdGLCD(0x40);	
        ChipSelect(CHIP_1);  
        for(j=0;j<128;j++)
        {
            if(j==64)
                ChipSelect(CHIP_2); 
						if(choice==1)
							WriteGLCD(stars[i][j]);
						else if(choice==2)
							WriteGLCD(sun[i][j]);
						else if(choice==3)
							WriteGLCD(APJ[i][j]);

        }          
    }  
}


void GLCD_Fill_Line(u8 val,int line)
{
		u8 y;
     ChipSelect(CHIP_1_2);
     CmdGLCD(0xb8+line);
	   CmdGLCD(COL_ADDR);                      
       for(y=0;y<64;y++)
	       WriteGLCD(val); // Clear data in the LCD.
}
