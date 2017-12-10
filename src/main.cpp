#include <Arduino.h>
#define Num_Of_Word 2

#include "Pacman1.h"
#include "Pacman2.h"
#include "Pacman3.h"
#include "Pacman4.h"

#define LEDARRAY_D 2
#define LEDARRAY_C 3
#define LEDARRAY_B 4
#define LEDARRAY_A 5
#define LEDARRAY_G 6
#define LEDARRAY_DI 7
#define LEDARRAY_CLK 8
#define LEDARRAY_LAT 9

#define Num_Word 1

void Clear_Display();
void Calc_Shift();
void Display(const unsigned char dat[][16]);
void Send(unsigned char dat);
void Scan_Line( unsigned char m);

unsigned char Display_Swap_Buffer[Num_Word][32]={0};

unsigned char temp = 0x80;
unsigned char Shift_Count = 0;
unsigned char Display_Word_Count = 0;

void setup()
{
	pinMode(LEDARRAY_D, OUTPUT);
	pinMode(LEDARRAY_C, OUTPUT);
	pinMode(LEDARRAY_B, OUTPUT);
	pinMode(LEDARRAY_A, OUTPUT);
	pinMode(LEDARRAY_G, OUTPUT);
	pinMode(LEDARRAY_DI, OUTPUT);
	pinMode(LEDARRAY_CLK, OUTPUT);
	pinMode(LEDARRAY_LAT, OUTPUT);

	Clear_Display();
}

void displayAndHold(const unsigned char image[][16], unsigned int count) {
	for (unsigned int i = 0; i < count; i++){
		Display(image);
	}
}

void loop() {
	displayAndHold(Pacman1,10);
	displayAndHold(Pacman2,10);
	displayAndHold(Pacman3,10);
	displayAndHold(Pacman4,10);
}

//************************************************************
void Clear_Display()
{
	unsigned char i,j;
	for(j = 0 ; j < Num_Word; j++)
	{
		for(i = 0 ; i < 32 ;i++)
		{
			Display_Swap_Buffer[j][i] = 0xff;
		}
	}
}

//*************************************************************
void Display(const unsigned char dat[][16])
{
	unsigned char Display_Buffer[2];
	unsigned char i;

	for( i = 0 ; i < 32 ; i+=2 )
	{
		digitalWrite(LEDARRAY_G, HIGH);

		Display_Buffer[0] = dat[0][i];
		Display_Buffer[1] = dat[0][i+1];

		Send(~Display_Buffer[1]);
		Send(~Display_Buffer[0]);

		digitalWrite(LEDARRAY_LAT, HIGH);
		delayMicroseconds(1);

		digitalWrite(LEDARRAY_LAT, LOW);
		delayMicroseconds(1);

		Scan_Line(i/2);

		digitalWrite(LEDARRAY_G, LOW);

		delayMicroseconds(300);
	}
}

//****************************************************
void Scan_Line( unsigned char m)
{
	digitalWrite(LEDARRAY_D, (m & 0x08) ? HIGH : LOW);
	digitalWrite(LEDARRAY_C, (m & 0x04) ? HIGH : LOW);
	digitalWrite(LEDARRAY_B, (m & 0x02) ? HIGH : LOW);
	digitalWrite(LEDARRAY_A, (m & 0x01) ? HIGH : LOW);
}

//****************************************************
void Send( unsigned char dat)
{
	unsigned char i;
	digitalWrite(LEDARRAY_CLK, LOW);
	delayMicroseconds(1);;
	digitalWrite(LEDARRAY_LAT, LOW);
	delayMicroseconds(1);;

	for( i = 0 ; i < 8 ; i++ )
	{
		if( dat & 0x01 )
		{
			digitalWrite(LEDARRAY_DI, HIGH);
		}
		else
		{
			digitalWrite(LEDARRAY_DI, LOW);
		}

		digitalWrite(LEDARRAY_CLK, HIGH);
		delayMicroseconds(1);
		digitalWrite(LEDARRAY_CLK, LOW);
		delayMicroseconds(1);

		dat >>= 1;
	}
}
