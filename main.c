/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include <stm8s.h> 
#include <init.h>
#include <string.h>
#include <stdbool.h>



#define SIZE	16		/* buffer size */

unsigned char buffer[SIZE];		/* reception buffer */
unsigned char index = 0;
unsigned int counter = 0;

/* prototypes */
void InitialiseSystemClock(void);
void InitialiseUSART(void);
void InitialiseTimer2(void);
void SetupTimer2(void);
void StopTimer2(void);
void ClearBuffer(void);
void ConfirmTag(void);

void BeepOn(void);
void BeepOff(void);

void BeepOn(){BEEP = 0x2a;}
void BeepOff(){BEEP = 0x0a;}

/* This is the UART receive character interrupt */
@interrupt void receipt(void){	
	tmp = USART1_DR;							// clear interrupt
	buffer[index++] = USART1_DR;	// add character to the buffer
}

//
//  Timer 2 Overflow interrupt.
//  Interrupt every 10mS
@interrupt void tim2_overflow(void){
    TIM2_SR1 &= 0xfe;         // Reset the interrupt otherwise it will fire again straight away.
		counter += 1;							// increment the counter
		if (counter > 100){				// 100 x 10mS = 1 second
			counter = 0;						// reset the counter
			PD_ODR |= 0x01;					// turn off the green on-board led
			PD_ODR |= 0x02;					// turn off the buzzer
			PD_ODR |= 0x04;					// turn off the relay
			TIM2_IER &= 0xfe;       // disable the overflow interrupts.
			TIM2_CR1 &= 0xfe;       // disable the timer.
			BeepOff();
	}
}

/*
	ConfirmTag()
	Turn on the onboard green led
	Turn on the buzzer
*/
void ConfirmTag(){
	PD_DDR |= 0x01;		// Port D, bit 0 configured as output
	PD_CR1 &= 0xfe;		// Port D, bit 0 configured as open drain
	PD_ODR &= 0xfe;		// Port D, bit 0 reset to 0, ie led on
	
	PD_DDR |= 0x02;		// Port D, bit 1 configured as output
	PD_CR1 &= 0xfd;		// Port D, bit 1 configured as open drain	
	PD_ODR &= 0xfd;		// Port D, bit 1 reset to 0, ie buzzer on
	
	PD_DDR |= 0x04;		// Port D, bit 2 configured as output
	PD_CR1 &= 0xfb;		// Port D, bit 2 configured as open drain	
	PD_ODR &= 0xfb;		// Port D, bit 2 reset to 0, ie relay on
	BeepOn();
	
	SetupTimer2();
}

int CompareArrays(unsigned char a[], unsigned char b[]){
	int ii;
  for(ii = 0; ii < 10; ii++) {
    if (a[ii] != b[ii]) return 0;   
  }
	return 1;
}

void ClearBuffer(){
	int n;
	for (n=0; n < SIZE; n++) buffer[n] = 0;
}

// Brittas common code
// 0001567944
// 0x02, 0x0a, 0x02, 0x5c, 0x00, 0x17, 0xec, 0xc8, 0x67, 0x03
// This is the code that all tags are compared to
const unsigned char code[] = {0x02, 0x0a, 0x02, 0x5c, 0x00, 0x17, 0xec, 0xc8, 0x67, 0x03};


main(){	
	sim();											// global interrupt disable
	InitialiseTimer2();					// prepare timer 2 for 10mS interrupts
	InitialiseSystemClock();		// set system clock to 16MHz
	InitialiseUSART();					// set the uart for 9600 8 N 1
		
	index = 0;									// initialize the buffer index
	tmp = USART1_DR;						// reset any pending uart interrupts
	rim();											// global interrupt enable
		
	while (1){		
		if(index >= 10){					// full packet received from the tag?
			if (CompareArrays(code, buffer) == 1){ // check if the tag is the valid
				ConfirmTag();					// valid, so announce it with led, buzzer and relay
			}
			ClearBuffer();					// start over
			index = 0;							// start over
		}	
	};
}