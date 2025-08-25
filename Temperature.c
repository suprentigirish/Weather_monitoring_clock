#include "headers.h"

void Init_Temp(void)
{
	Init_ADC();
}

f32 Temp(void)
{
	float temp;
	//code
	/*  For Test  
	temp=rand()%100;  */
	
	temp=Read_ADC(CH0);
	
	return (temp*100);
}
