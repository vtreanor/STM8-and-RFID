unsigned char tmp;

//
//  Setup the system clock to run at 16MHz using the internal oscillator.
//	http://blog.mark-stevens.co.uk/2012/07/configuring-the-stm8s-system-clock-the-way-of-the-register/
//
void InitialiseSystemClock(){
    CLK_ICKR = 0;                       //  Reset the Internal Clock Register.
    CLK_ICKR |= 1;                 			//  Enable the HSI.
    CLK_ECKR = 0;                       //  Disable the external clock.		
		while ((CLK_ICKR & 0x02) == 0);			//  Wait for the HSI to be ready for use.
		CLK_CKDIVR = 0;											// 	clock at full speed ie 16MHz    
    CLK_PCKENR1 = 0xff;                 //  Enable all peripheral clocks.
    CLK_PCKENR2 = 0xff;                 //  Ditto.
    CLK_CCOR = 0;                       //  Turn off CCO.
    CLK_HSITRIMR = 0;                   //  Turn off any HSIU trimming.
    CLK_SWIMCCR = 0;                    //  Set SWIM to run at clock / 2.    
		CLK_SWR = 0xe1;                     //  Use HSI as the clock source.
		CLK_SWCR |= 0x02;										//  enable clock switcu
		while ((CLK_SWCR & 0x01) == 0x01);	//  Pause while the clock switch is busy.
}

//
//  Setup the USART to run at 115200 baud, no parity, one stop bit, 8 data bits.
//
//  Important: This relies upon the system clock being set to run at 16 MHz.
//
void InitialiseUSART()
{
    //
    //  Clear the Idle Line Detected bit in the status register by a read
    //  to the USART1_SR register followed by a Read to the USART1_DR register.
    //    
		tmp = USART1_SR;
    tmp = USART1_DR;
    //
    //  Reset the USART registers to the reset values.
    //
    USART1_CR1 = 0;
    USART1_CR2 = 0;
    USART1_CR4 = 0;
    USART1_CR3 = 0;
    USART1_CR5 = 0;
    USART1_GTR = 0;
    USART1_PSCR = 0;
//
//  Now setup the port to 9600,n,8,1.
//
		
		USART1_CR1 &= 0x00;		//  Disable parity and 8 Data bits..
		USART1_CR3  &= 0x0f;     //  1 stop bit.
		
/*  Set the baud rate registers to 9600 baud, 16 MHz system clock. */
    USART1_BRR2 = 0x03;
    USART1_BRR1 = 0x68;
		
/*  Set the baud rate registers to 19.2 kbaud, 16 MHz system clock. */
//  USART1_BRR2 = 0x01;
//  USART1_BRR1 = 0x34;
		
/*  Set the baud rate registers to 57.6 kbaud, 16 MHz system clock. */
//  USART1_BRR2 = 0x06;
//  USART1_BRR1 = 0x11;
		
/*  Set the baud rate registers to 115.2 kbaud, 16 MHz system clock. */
//  USART1_BRR2 = 0x0b;
//  USART1_BRR1 = 0x08;
		
//
//  Turn on the USART transmit, receive and the USART clock.
//
		USART1_CR2 |= 0x24; 			//  Enable transmit and receive.

}


//
//  Reset Timer 2 to a known state.
//

void InitialiseTimer2()
{
    TIM2_CR1 = 0;               // Turn everything TIM2 related off.
    TIM2_IER = 0;
    TIM2_SR2 = 0;
    TIM2_CCER1 = 0;
    TIM2_CCER2 = 0;    
    TIM2_CCMR1 = 0;
    TIM2_CCMR2 = 0;
    TIM2_CCMR3 = 0;
    TIM2_CNTRH = 0;
    TIM2_CNTRL = 0;
    TIM2_PSCR = 0;
    TIM2_ARRH  = 0;
    TIM2_ARRL  = 0;
    TIM2_CCR1H = 0;
    TIM2_CCR1L = 0;
    TIM2_CCR2H = 0;
    TIM2_CCR2L = 0;
    TIM2_CCR3H = 0;
    TIM2_CCR3L = 0;
    TIM2_SR1 = 0;
}

//
//  Setup Timer 2 to generate a 20 Hz interrupt based upon a 16 MHz timer.
//
void SetupTimer2(){
	TIM2_SR1 |= 0;
	TIM2_PSCR = 0x0e;       //  Prescaler = 16384 or 1.024mS
	TIM2_ARRH = 0x00;       //  Auto-reload high. High byte of 10
	TIM2_ARRL = 0x0a;       //  Auto-reload low. Low byte of 10
	TIM2_IER |= 0x01;       //  Enable the update interrupts.
	TIM2_CR1 |= 0x01;       //  Finally enable the timer.
}

void StopTimer2(){
	TIM2_IER &= 0xfe;       //  Enable the update interrupts.
	TIM2_CR1 &= 0xfe;       //  Finally enable the timer.
}