#include<LPC21xx.h>
#include "delay.h"

#define D 1<<25		 //1-WIRE DATA LINE
#define R (IOPIN0&(1<<25))


/**RESET FUNCTION DEFINATION FOR DS18B20**/
unsigned char ResetDS18b20(void)
{
	unsigned int  presence; 
    IODIR0 |= D;
	IOPIN0 |= D;	
	DelayUs(1); //////////////1Us
	IOPIN0 &= ~(D);
	DelayUs(478);//////////////486Us 
	IOPIN0 |= D;
	DelayUs(54); //////////////55Us
	presence = IOPIN0 ; 	
	DelayUs(423);//////////////431Us
	
	if(presence&R)
		return 1;
	else 
		return 0;
}

/**READING A BIT USING 1-WIRE PROTOCOL FROM DS18B20**/
unsigned char ReadBit(void)
{
	unsigned int B;	
	IOPIN0 &= ~(D);
	DelayUs(1);//////////////1Us
	IOPIN0 |= D;
	IODIR0 &= ~(D);
	DelayUs(10);//////////////11Us
	B=IOPIN0;
	IODIR0 |=D;
	if(B&R)
		return 1;
	else 
		return 0;
}
/**WRITING A BIT USING 1-WIRE PROTOCOL INTO DS18B20**/
void WriteBit(unsigned char Dbit)
{
	IOPIN0 &= ~(D);
	DelayUs(1);//////////////1Us
	if(Dbit)
		IOPIN0 |= D;
	DelayUs(58);//////////////59Us
	IOPIN0 |= D;
	DelayUs(1);
}
/**READING A BYTE USING 1-WIRE PROTOCOL FROM DS18B20**/
unsigned char ReadByte(void)
{
	unsigned char i;
	unsigned char Din = 0;
	for (i=0;i<8;i++)
	{
		Din|=ReadBit()? 0x01<<i:Din;
		DelayUs(45); //////////////46Us
	}
	return(Din);
}

/**WRITING A BIT USING 1-WIRE PROTOCOL INTO DS18B20**/
void WriteByte(unsigned char Dout)
{
	unsigned char i;
	for (i=0; i<8; i++)
	{	    
		WriteBit((Dout & 0x1)); 
		Dout = Dout >> 1;
		DelayUs(1);
	}
	DelayUs(98);//////////////100Us
}

/**READING TEMPERATURE FROM DS18B20 USING 1-WIRE PROTOCOL**/
int ReadTemp(void)
{
	unsigned char n,buff[2];
	int temp;
	ResetDS18b20();	  //RESET DS18B20
    WriteByte(0xcc);   // skip ROM
    WriteByte(0x44);   // perform temperature conversion
    while (ReadByte()==0xff); // wait for conversion complete	
	ResetDS18b20();
	WriteByte(0xcc);   // skip ROM
	WriteByte(0xbe);    // read the result
    for (n=0; n<2; n++) // read 9 bytes but, use only one byte
    {
       buff[n]=ReadByte();  // read DS1820
    }
	temp=buff[1];
	temp=temp<<8;
	temp=temp|buff[0];
	return(temp);
}
