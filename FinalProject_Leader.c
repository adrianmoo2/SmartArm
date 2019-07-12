/*
 * 122A Final Project.c
 *
 * Created: 11/27/2018 8:40:02 PM
 * Author : ucrcse
 LEADER CODE
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "usart_ATmega1284.h"
#include "io.c"
#include "scheduler.h"
#include "keypad.h"


int FollowerBoolean = 0;
int KeypadBoolean = 0;
int DefuseBoolean = 0;
int send = 0;
char *enterCode = "Enter Code:      ";


char code[] = "122A";
char userCodeInput[6];

enum LCD_States { LCD_WaitForInput, LCD_BombBeingDetonated, LCD_Exploded, LCD_Defused };
enum Follower_States {Follower_Wait, Follower_BombDetonated, Follower_BombDefused};

void StringSend(char s[], char num)
{
	char *strptr;
	
	for (strptr = &s[0]; *strptr != '\0'; strptr++)
	{
			USART_Send(*strptr, num);
	}
	USART_Flush(0);
}

void append(char* s, char c) {
	int len = strlen(s);
	s[len] = c;
	s[len+1] = '\0';
}

enum Keypad_States {Keypad_Wait, Keypad_Input};
int keypadTick(int keypad_state)
{
	char char_in;
	
	switch (keypad_state)
	{
		case -1:
			keypad_state = Keypad_Wait;
			break;
		case Keypad_Wait:
			keypad_state = (KeypadBoolean == 1) ? Keypad_Input : keypad_state;
			break;
		case Keypad_Input:
			keypad_state = (KeypadBoolean == 0) ? Keypad_Wait : keypad_state;
			break;
		default:
			keypad_state = -1;
	}
	
	switch (keypad_state)
	{
		case -1:
			break;
		case Keypad_Wait:
			//LCD_DisplayString(1, "Waiting");
			break;
		case Keypad_Input:
			//LCD_DisplayString(1, "Inputting");
			if (GetKeypadKey() != '\0' && GetKeypadKey() != '#')
			{
				char_in = GetKeypadKey();
				append(userCodeInput, char_in);
				LCD_WriteData(char_in);
				//LCD_DisplayString(1, char_in);
				//free(displayString);
			}
			else if (GetKeypadKey() == '#')
			{
				if (strcmp(userCodeInput, code) == 0)
				{
					DefuseBoolean = 1;
				}
				else
				{
					LCD_DisplayString(1,"Try again:       ");
					userCodeInput[0] = '\0';
				}
			}
			break;
		default:
			break;
	}
	
	return keypad_state;
}

//enum LCD_States { LCD_WaitForInput, LCD_BombBeingDetonated, LCD, Exploded, LCD_Defused };
int lcdTick(int lcd_state)
{
	char Data_in;
	char Data_in_Follower;
	
	switch(lcd_state) { // Transitions
		case -1: // Initial transition
			lcd_state = LCD_WaitForInput;
			break;
		case LCD_WaitForInput:
			if (USART_HasReceived(0))
			{
				Data_in = USART_Receive(0);
				lcd_state = (Data_in == '0') ? LCD_BombBeingDetonated : lcd_state;
				if (Data_in == '0')
				{
					LCD_DisplayString(1, enterCode);
				}
			}
			break;
		case LCD_BombBeingDetonated:
			if (USART_HasReceived(1))
			{
				//PORTA = 0x01;
				Data_in_Follower = USART_Receive(1);
				lcd_state = (Data_in_Follower == '0') ? LCD_Exploded : lcd_state;
			}
			lcd_state = (DefuseBoolean == 1) ? LCD_Defused : lcd_state;
			//state = ;
			break;
		case LCD_Exploded:
			break;
		case LCD_Defused:
			//state = ;
			break;
		default:
			lcd_state = -1;
	}

	switch(lcd_state) { // Actions
		case -1: 
			lcd_state = LCD_WaitForInput;
			break;
		case LCD_WaitForInput:
			LCD_DisplayString(1, "Waiting for input...");
			break;
		case LCD_BombBeingDetonated:
			FollowerBoolean = 1;
			KeypadBoolean = 1;
			//LCD_DisplayString(1, enterCode);
			break;
		case LCD_Exploded:
			LCD_DisplayString(1, "Bomb Exploded.   :(");
			break;
		case LCD_Defused:
			LCD_DisplayString(1, "Bomb Defused! Congrats!");
			break;
		default:
			break;
	}

return lcd_state;

}

//enum Follower_States {Follower_Wait, Follower_BombDetonated, Follower_BombDefused};
int FollowerTick(Follower_state)
{
	switch (Follower_state)
	{
		case -1:
			Follower_state = Follower_Wait;
			break;
		case Follower_Wait:
			Follower_state = (FollowerBoolean == 1) ? Follower_BombDetonated : Follower_state;
			Follower_state = (DefuseBoolean == 1) ? Follower_BombDefused : Follower_state;
			break;
		case Follower_BombDetonated:
			Follower_state = Follower_Wait;
			break;
		case Follower_BombDefused:
			break;
		default:
			Follower_state = -1;
	}
	
		switch (Follower_state)
		{
			case -1:
				break;
			case Follower_Wait:
				break;
			case Follower_BombDetonated:
				if (USART_IsSendReady(1))
				{
					//PORTA = 0xFF;
					USART_Send(0x01, 1);
				}
				FollowerBoolean = 0;
				break;
			case Follower_BombDefused:
				if (USART_IsSendReady(0) && send == 0)
				{
					USART_Send('3', 0);
					++send;
				}
				if (USART_IsSendReady(1))
				{
					//PORTA = 0xFF;
					USART_Send(0x02, 1);
				}
				break;
			default:
				break;
		}
		
	return Follower_state;
}

int main(void)
{
	DDRA = 0xFF, PORTA = 0x00;		/* make PORT as output port */
	DDRB = 0xF0; PORTB = 0x0F;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF, PORTD = 0x00;
	
	initUSART(0);	/* initialize USART with 9600 baud rate */
	initUSART(1);
	LCD_init();
	
	userCodeInput[0] = '\0';
	
	tasksNum = 3;
	task tsks[3];
	tasks = tsks;
	
	//define tasks
	unsigned char i = 0;
	tasks[i].state = -1;
	tasks[i].period = 2;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &FollowerTick;
	
	++i;
	
	tasks[i].state = -1;
	tasks[i].period = 20;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &lcdTick;
	
	++i;
	
	tasks[i].state = -1;
	tasks[i].period = 500;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &keypadTick;
	
	const unsigned int period_sm_1 = 2;
	TimerSet(period_sm_1);
	TimerOn();
	
	while(1)
	{
	}
}

