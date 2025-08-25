#include <LPC21xx.h>
#include "glcd.h"
#include "rtc.h"
#include "dht11.h"
#include "kpm.h"
//#include "interrupts.h"
#include "delay.h"
//#include "i2c.h"
//#include "i2c_eeprom.h"
#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768))


char *days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

int main() {
    u8 hour, min, sec,day,date,month;
	  u16 year;
    u8 humidity_int,humidity_dec,temp_int,temp_dec,checksum;
	 	Initkpm();
    InitGLCD();
    //rtc_init();
	InitRTC();
	Interrupt_Enable();

    //rtc_set_time(12, 0, 0,1,1,1,2025);
	GLCD_Fill_Screen(CLEAR);
    CmdGLCD(COL_ADDR); 
    ChipSelect(CHIP_1);
    ShowImage_PSPK();
    delay_s(2);
    GLCD_Fill_Screen(CLEAR);
    CmdGLCD(COL_ADDR); 
    ChipSelect(CHIP_1);
    StrGLCD(PAGE0,COL_ADDR,0,"  WEATHER MONITORING  "); 
    StrGLCD(PAGE1,COL_ADDR,0,"       CLOCK          "); 
	delay_s(2);
    GLCD_Fill_Screen(CLEAR);
	 CCR=0X11;
//     dht11_request();
  //      dht11_response();
	while (1) 
	{
        //rtc_get_time(&hour, &min, &sec,&day,&date,&month,&year);

        dht11_request();
        dht11_response();
        humidity_int = dht11_read_byte();
        humidity_dec = dht11_read_byte();
        temp_int = dht11_read_byte();
        temp_dec = dht11_read_byte();
        checksum = dht11_read_byte();
		  
        GLCD_Fill_Screen(CLEAR);
		 TIME_RTC();
         // Display Temperature
        StrGLCD(PAGE1, COL_ADDR,0, "Temp: ");
		IntGLCD(PAGE1,COL_ADDR+30,0,temp_int);
		CharGLCD(PAGE2, COL_ADDR+42,223,1);

         // Display Humidity
        StrGLCD(PAGE2, COL_ADDR, 0,"Humi: ");
		IntGLCD(PAGE2,COL_ADDR+30,0,humidity_int);
       	CharGLCD(PAGE2, COL_ADDR+42,'%',1);
	   //delay_ms(1000);
    } 
}
