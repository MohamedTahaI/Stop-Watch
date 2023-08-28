
// facebok name : mohamed taha ibrahem
// diploma76
/* stop watch project
 *  F cpu = 1Mhz , N =1024
 *	   Ft = 976.5hz
 *	   T = 1.024ms
 *	   T compare = 1.024ms *977 = 1.00044s timer1 interrupt at 1s
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>


void Timer1_COMPARE_CTC_Init(void);
void INT0_Init(void);
void INT1_Init(void);
void INT2_Init(void);
void stop_watch(void);

unsigned char i, Time[6] ={0};
unsigned char flag =0;


int main()
{
	DDRA  |= 0x3F; // control 7segment
	PORTA &= 0xC0; // disable all 7segment

	DDRC  |= 0x0F; // out to decoder
	PORTC &= 0xF0; // send number to decoder

	//call function
	Timer1_COMPARE_CTC_Init();
	INT0_Init();
	INT1_Init();
	INT2_Init();





	while(1)
	{
		for(i = 0; i < 6; i++)
		{

			PORTA = (PORTA & 0xC0) | (1<<i); // chose the 7 segment to enable
			PORTC = (PORTC & 0xF0) | (Time[i] & 0x0F); //  chose the time display in a enabled 7segment
			_delay_ms(1);
		}

		if(flag==1) // interrupt timer1 at 1s
		{
			stop_watch();
			flag=0;
		}

	}

}



//all ISR


ISR(TIMER1_COMPA_vect)
{
  flag=1;
}

ISR(INT0_vect)
{
	TCNT1 = 0;
	PORTC &= 0xF0;
	for(i = 0; i < 6 ; i++)
		Time[i] = 0;
}
ISR(INT1_vect)
{
	TIMSK &=~(1<<OCIE1A);
}
ISR(INT2_vect)
{
	TIMSK |= (1<<OCIE1A);
}


// All Function


void Timer1_COMPARE_CTC_Init(void)
{
	 //
	TCNT1  = 0;		// Set timer1 start to zero

	OCR1A  = 977;    // Set the Compare value to 125

	TIMSK |= (1<<OCIE1A); // Enable Timer1 Compare A Interrupt

	// Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	//  FOC1A=1 FOC1B=0
	// CTC Mode WGM10=0 WGM11=0

	TCCR1A = (1<<FOC1A);

	//  CTC Mode WGM12=1 WGM13=0
	// Prescaler = F_CPU/1024 CS10=1 CS11=0 CS12=1

	TCCR1B = (1<<WGM12) | (1<<CS10) |(1<<CS12) ;

	SREG |= (1<<7);  // Enable I-bit
}


void INT0_Init(void)
{
	DDRD  &= ~(1<<PD2);   // Configure INT0/PD2 as input pin
	PORTD |= (1<<PD2);    //internal pull-up resistor


	// The falling edge of INT0 ISC00 =0 ISC01 =1
	MCUCR &=~(1<<ISC00);
	MCUCR |=  (1<<ISC01);

	GICR  |= (1<<INT0);    // Enable INT0
	SREG  |= (1<<7);        // Enable I-bit

}


void INT1_Init(void)
{
	DDRD  &=~(1<<PD3); // Configure INT1/PD3 as input pin

	//  The rising edge of INT1 generates  ISC11 =1 ISC10 =1
	MCUCR |=  (1<<ISC10);
	MCUCR |=  (1<<ISC11);

	GICR  |= (1<<INT1);     // Enable INT1
	SREG  |= (1<<7);        // Enable I-bit

}


void INT2_Init(void)
{
	DDRB  &=~(1<<PB2);   // Configure INT2/PB2 as input pin
	PORTB |= (1<<PB2);   //internal pull-up resistor

	// The falling edge of INT0 ISC2 =0;
	MCUCSR &= ~(1<<ISC2);

	GICR  |= (1<<INT2);     // Enable INT2
	SREG  |= (1<<7);        // Enable I-bit

}


void stop_watch(void)
{

	Time[0]++;

	if(Time[0] > 9)
        {
		Time[1]++;
		Time[0] = 0;

		if(Time[1] == 6)
        {
			Time[2]++;
			Time[1] = 0;
		}
	}

	if(Time[2] > 9)
	{
		Time[3]++;
		Time[2] = 0;

		if(Time[3] == 6){
			Time[4]++;
			Time[3] = 0;
		}
	}

	if(Time[4] > 9)
	{
		Time[5]++;
		Time[4] = 0;

		if(Time[5] > 9)
		{
			for(i = 0; i < 6 ; i++)
				Time[i] = 0;
		}
	}

}




