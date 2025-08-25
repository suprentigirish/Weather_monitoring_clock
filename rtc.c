#include "headers.h"

void InitRTC()
{
	SEC=20; //Initialized seconds
  MIN=32; //Initialized minutes
  HOUR=11;//Initialized hour
	DOM=29;
	MONTH=3;
	YEAR=2025;
}

		u32 HR,M,S,D,MT,Y,A,B;
		char time[20];
		char date[20];

void TIME_RTC(void)
{
	  HR=HOUR,M=MIN,S=SEC;
		D=DOM,MT=MONTH,Y=YEAR,A=0,B=0;
		A=Y/100;
		B=Y%100;
	//  StrGLCD(PAGE6,COL_ADDR,0,(S8)"TIME");
		time[0]=(HR/10)+'0';
		time[1]=(HR%10)+'0';
		time[2]=':';
		time[3]=(M/10)+'0';
		time[4]=(M%10)+'0';
		time[5]=':';
		time[6]=(S/10)+'0';
		time[7]=(S%10)+'0';
		StrGLCD(PAGE4,COL_ADDR+4,65,(S8)time);
		//StrGLCD(PAGE7,COL_ADDR,0,(S8)"DATE");
		date[0]=D/10+'0';
		date[1]=D%10+'0';
		date[2]='/';
		date[3]=MT/10+'0';
		date[4]=MT%10+'0';
		date[5]='/';
		date[6]=(Y/1000)+'0';
		date[7]=(A%10+'0');
		date[8]=(B/10+'0');
		date[9]=(Y%10+'0');
		StrGLCD(PAGE5,COL_ADDR+4,65,(S8)date);
}

