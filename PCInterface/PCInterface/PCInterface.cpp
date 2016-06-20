/*
 * PCInterface.cpp
 *
 * Created: 26-05-2016 13:38:50
 *  Author: Tonni
 */ 


#include <avr/io.h>
#include "Uart/uartDriver.h"
#include "SD/sdCard.h"
#include "UnitHandler/UnitHandler.h"
#include <avr/interrupt.h>
#include "PCinterface/PCinterface.h"
#include "X10/X10.h"
//#define F_CPU 16000000

UART uart_obj(57600, 8, 'E');
sdCard SD_obj(4000);
rtc RTC_obj(0xD0, 400);
UnitHandler handler_obj(&SD_obj);
X10 X10_obj(0x00, 0x00);
PCinterface PCIF_obj(&handler_obj ,&uart_obj, &RTC_obj, &X10_obj);


int main(void)
{
	RTC_obj.setDate(21, 6, 16, 2);
	RTC_obj.setTime(9, 0, 0);
	unsigned char Array[512] = { 0 };
	for (int i = 0; i < 101; i++){
		SD_obj.writeBlock(i, Array);
	}	
	sei();
    while(1)
    {
    }
}

ISR (USART0_RX_vect) // interrupt based uart
{
	PCIF_obj.handleCMD();
	//uart_obj.sendChar(0xAB);
}

// interrupts used by x10.h
ISR(INT2_vect) // used for X10.h
{
	X10_obj.reciveSendHighInterupt();
}
ISR(INT3_vect) // used for x10.h 
{
	X10_obj.reciveSendLowInterupt();
}
ISR(TIMER0_OVF_vect)
{
	X10_obj.stop120Interupt();
}
ISR(TIMER3_OVF_vect)
{
	X10_obj.resetReciverInterupt();
}
ISR(TIMER2_OVF_vect)
{
	X10_obj.inputCompleteInterupt();
}