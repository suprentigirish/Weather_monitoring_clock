#include "headers.h"



void Init_Smoke(void)
{
	CLRBIT(IODIR0,MQ);
}

u32 Smoke(void)
{
	return READBIT(IOPIN0,MQ);
}

