/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */

typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

extern void _stext();     		/* startup routine */
extern void receipt();				/* USART1_Rx */
extern void tim2_overflow();	/* Timer 2 overflow interrupt */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, 				/* reset */
	{0x82, NonHandledInterrupt}, 								/* trap  */
	{0x82, NonHandledInterrupt}, 								/* irq0 TLI */
	{0x82, NonHandledInterrupt}, 								/* irq1 AWU */
	{0x82, NonHandledInterrupt}, 								/* irq2 CLK */
	{0x82, NonHandledInterrupt}, 								/* irq3 EXTI0 */
	{0x82, NonHandledInterrupt}, 								/* irq4 EXTI1 */
	{0x82, NonHandledInterrupt}, 								/* irq5 EXTI2 */
	{0x82, NonHandledInterrupt}, 								/* irq6 EXTI3 */
	{0x82, NonHandledInterrupt}, 								/* irq7 EXTI4 */
	{0x82, NonHandledInterrupt}, 								/* irq8 RESERVED */
	{0x82, NonHandledInterrupt}, 								/* irq9 RESERVED */
	{0x82, NonHandledInterrupt}, 								/* irq10 SPI */
	{0x82, NonHandledInterrupt}, 								/* irq11 TIM1 UPDATE/OVERFLOW/TRIGGER/BREAK */
	{0x82, NonHandledInterrupt}, 								/* irq12 TIM1 CAPTURE/COMPARE*/
	{0x82, (interrupt_handler_t)tim2_overflow}, /* irq13 TIM2 UPDATE/OVERFLOW */
	{0x82, NonHandledInterrupt}, 								/* irq14 TIM2 CAPTURE/COMPARE */
	{0x82, NonHandledInterrupt}, 								/* irq15 RESERVED */
	{0x82, NonHandledInterrupt}, 								/* irq16 RESERVED */
	{0x82, NonHandledInterrupt}, 								/* irq17 UART1 TX COMPLETE*/
	{0x82, (interrupt_handler_t)receipt}, 			/* irq18 UART1_RX */
	{0x82, NonHandledInterrupt}, /* irq19 */
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, NonHandledInterrupt}, /* irq22 */
	{0x82, NonHandledInterrupt}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
