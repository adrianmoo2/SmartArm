/*
 * atran_atruo_lab2_section1.c
 *
 * Created: 10/4/2018 9:05:34 AM
 * Author : ucrcse
 FOLLOWER CODE
 */ 

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "scheduler.h"

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#define LED_Direction DDRA		/* define LED Direction */
#define LED_PORT PORTA			/* define LED port */


volatile unsigned char PORTA_TEMP = 0x00;
unsigned char temp;
double freq = 0;

int SevenSegBoolean = -1;
int index = 0;
char array[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};	

enum USART_States {Wait, Bomb_Detonated, Bomb_Defused};
enum SevenSeg_States {SevenSeg_Wait, SevenSeg_CountDown, SevenSegBombDetonated, SevenSegBombDefused};


//enum USART_States {Wait, Bomb_Detonated, Bomb_Defused};
int TickFct_Receive(int state)
{
	switch (state)
	{
		case -1:
			PORTA_TEMP = 0x00;
			state = Wait;
		break;
		case Wait:
			state = (USART_HasReceived(1) && USART_Receive(1) == 0x01) ? Bomb_Detonated : state;
			break;
		case Bomb_Detonated:
			state = (USART_HasReceived(1) && USART_Receive(1) == 0x02) ? Bomb_Defused : state;
			break;
		case Bomb_Defused:			
			break;
		default:
			state = -1;
	}
	
	switch(state)
	{
		case -1:
		break;
		case Wait:
			//USART_Flush(0);
		break;
		case Bomb_Detonated:
			temp = USART_Receive(1);
			SevenSegBoolean = 1;
			//PORTA = 0xFF;
		break;
		case Bomb_Defused:
			//PORTB = 0x02;
			SevenSegBoolean = 2;
			//PORTA_TEMP = 0x00;
		break;
		default:
			state = -1;
	}
	
	//PORTA = PORTA_TEMP;
	return state;
}

//enum SevenSeg_States {SevenSeg_Wait, SevenSeg_CountDown, SevenSegBombDetonated, SevenSegBombDefused};
int SevenSegTick(int sevenSegState)
{
	switch (sevenSegState)
	{
		case -1:
			sevenSegState = Wait;
			break;
		case Wait:
			sevenSegState = (SevenSegBoolean == 1) ? SevenSeg_CountDown : sevenSegState;
			break;
		case SevenSeg_CountDown:
			sevenSegState = (index == 0 && PORTC == 0x00) ? SevenSegBombDetonated : sevenSegState;
			sevenSegState = (SevenSegBoolean == 2) ? SevenSegBombDefused : sevenSegState;
			break;
		case SevenSegBombDetonated:
			break;
		case SevenSegBombDefused:
			break;
		default:
			sevenSegState = -1;
	}
	
		switch (sevenSegState)
		{
			case -1:
				break;
			case Wait:
				break;
			case SevenSeg_CountDown:
				if (index == 9)
				{
					PORTC = PORTC << 1;
				}
				PORTA = array[index];
				if (index == 0 && PORTC == 0x80)
				{
					PORTB = 0x01;
				}
				if (index > 0)
				{
					index--;
				}
				else {
					index = 9;
				}
				break;
			case SevenSegBombDetonated:
				index = 0;
				PORTA = array[index];
				if (USART_IsSendReady(1))
				{
					//PORTB = 0x02;
					USART_Send('0', 1);
				}
				break;
			case SevenSegBombDefused:
				break;
			default:
				break;
		}
	
	return sevenSegState;
}


int main(void)
{
	//initialize ports
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	//initialize uSART and variables
    initUSART(1);
	//PWM_on();
	PORTC = 0xE0;
	
	tasksNum = 2;
	task tsks[2];
	tasks = tsks;
	
	//define tasks
	unsigned char i = 0;
	tasks[i].state = -1;
	tasks[i].period = 1;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &TickFct_Receive;
	
	++i;
	tasks[i].state = -1;
	tasks[i].period = 83;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &SevenSegTick;
	
	const unsigned int period_sm_1 = 1;
	TimerSet(period_sm_1);
	TimerOn();
	
	
    while (1) 
    {
    }
}

