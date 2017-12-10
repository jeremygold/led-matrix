#include <Arduino.h>

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
void Display(unsigned char dat[][16]);
void Send(unsigned char dat);
void Scan_Line( unsigned char m);

unsigned char Display_Swap_Buffer[Num_Word][32]={0};

bool Shift_Bit = 0;
bool Flag_Shift = 0;

unsigned char Timer0_Count = 0;
unsigned char temp = 0x80;
unsigned char Shift_Count = 0;
unsigned char Display_Word_Count = 0;

#define Num_Of_Word 2

const unsigned char Word[Num_Of_Word][16] =
{
	0x1F, 0xE0, // 0001111111100000
	0x30, 0x30, // 0011000000110000
	0x20, 0x1C, // 0010000000011100
	0x60, 0xC6, // 0110000011000110
	0x46, 0x63, // 0100011001100011
	0x86, 0x21, // 1000011000100001
	0x80, 0x31, // 1000000000110001
	0x80, 0x11, // 1000000000010001
	0x83, 0x13, // 1000001100010011
	0xC3, 0x12, // 1100001100010010
	0x60, 0x32, // 0110000000110010
	0x20, 0x66, // 0010000001100110
	0x30, 0x04, // 0011000000000100
	0x18, 0x04, // 0001100000000100
	0x0C, 0x0C, // 0000110000001100
	0x03, 0xF8  // 0000001111111000
};

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

/*
void loop()
{
	unsigned int i;
	for(i = 0 ; i < 30; i ++)
	{
		Display(Display_Swap_Buffer);
	}
	Display_Word_Count = Shift_Count/16;
	Calc_Shift();
	Shift_Count++;
	if(Shift_Count == (Num_Of_Word+1)*16 )
	{
		Shift_Count = 0;
	}

}
*/

void loop() {
	Display(Word);
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

//************************************************************
void Calc_Shift()
{
	unsigned char i;

	for(i = 0;i < 16;i++)
	{
		if((Display_Swap_Buffer[0][16+i]&0x80) == 0)
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)&0xfe;
		}
		else
		{
			Display_Swap_Buffer[0][i] = (Display_Swap_Buffer[0][i] << 1)|0x01;
		}

		if(Shift_Count%16 < 8 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word[Display_Word_Count][i]&temp;
		}
		else if(Shift_Count%16 < 16 && Display_Word_Count < Num_Of_Word)
		{
			Shift_Bit = Word[Display_Word_Count][16+i]&temp;
		}
		else
		{
			Shift_Bit = 1;
		}

		if( Shift_Bit == 0)
		{
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)&0xfe;
		}
		else
		{
			Shift_Bit =	1;
			Display_Swap_Buffer[0][16+i] = (Display_Swap_Buffer[0][16+i] << 1)|0x01;
		}

	}
	temp = (temp>>1)&0x7f;
	if(temp == 0x00)
	{
		temp = 0x80;
	}
}

//*************************************************************
void Display(unsigned char dat[][16])
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
