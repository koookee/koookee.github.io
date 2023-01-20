#include "msp.h"

uint8_t chosenLED = 0; // Determins whether we're dealing with the red or RGB led (0: Red, 1: RGB)
uint8_t stateCounter = 0; // The state of the RGB led

int main(){

	// Disabling the watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// Configuring switches P1.1 & P1.4
	P1SEL0 &= (uint8_t)(~((1 << 1)|(1 << 4)));
	P1SEL1 &= (uint8_t)(~((1 << 1)|(1 << 4)));
	P1DIR &= (uint8_t)(~((1 << 1)|(1 << 4)));
	P1IN |= (uint8_t)((1 << 1)|(1 << 4));
	P1REN |= (uint8_t)((1 << 1)|(1 << 4));
	P1OUT |= (uint8_t)((1 << 1)|(1 << 4));

	// Configuring LEDs P1.0, P2.0, P2.1, and P2.2
	P1SEL0 &= (uint8_t)(~(1 << 0));
	P1SEL1 &= (uint8_t)(~(1 << 0));
	P2SEL0 &= (uint8_t)(~((1 << 0)|(1 << 1)|(1 << 2)));
	P2SEL1 &= (uint8_t)(~((1 << 0)|(1 << 1)|(1 << 2)));
	P1DIR |= (uint8_t)(1 << 0);
	P2DIR |= (uint8_t)((1 << 0)|(1 << 1)|(1 << 2));
	P1OUT &= (uint8_t)(~(1 << 0));
	P2OUT &= (uint8_t)(~((1 << 0)|(1 << 1)|(1 << 2)));
	
	// Configuring switches P1.1 & P1.4 as interrupts
	
	// Configure interrupts at device level
	P1IES |= (uint8_t)((1<< 1) | ( 1<<4));
	P1IFG  &= (uint8_t)(~((1 << 1)|(1 << 4)));
	P1IE |= (uint8_t)((1<< 1) | ( 1<<4));
	
	// Timer configuration
	TA0CTL &= (uint16_t)(~((1<<5) | (1<<4))); //stop timer
	TA0CTL &= (uint16_t)(~(1<<0)); //clear interrupt flag CCIFG
	TA0CCR0 = (uint16_t)(32768);  // CCR0 value
	TA0CTL |= (uint16_t)(1<<1); // interrupt enable
	TA0CTL &= (uint16_t)(~(1<<9)); // setting it to ACLK clock source
	TA0CTL |= (uint16_t)((1<<8)); // setting it to ACLK clock source
	TA0CTL |= (uint16_t)((1<<4)); //up mode (count to CCR0)
	
	
	// Configure interrupts at NVIC level
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC->ICPR[1] |= 0x00000008; 
	NVIC_EnableIRQ(PORT1_IRQn);
	
	NVIC_SetPriority(TA0_N_IRQn,2);
	NVIC->ICPR[1] |= 0x00000008; 
	NVIC_EnableIRQ(TA0_N_IRQn);
	
	// Globally enable interrupts
	__ASM("CPSIE I");

	
	while (1) {		
		__ASM("WFI");
	}
	return 0;
}
void PORT1_IRQHandler(void) 
{
	static uint8_t isPaused = 0; 
	//Test for pin 1 interrupt flag
	if ((P1IFG & (uint8_t)0x02) != 0) { 
		//Yes, pin 1: clear flag 
		P1IFG &= (uint8_t)~0x02;
		chosenLED ^= BIT0;
	} 
	//pin 4
	else {
		//Pin 4: clear flag 
		P1IFG &= (uint8_t)~0x10;
		isPaused ^= (uint8_t)0x01; // toggle isPaused
		if((isPaused & (uint8_t)(1 << 0)) == 0){ // not paused
			TA0CTL |= (uint16_t)((1<<4)); //up mode (enabling timer)
		}
		else { // paused
			TA0CTL &= (uint16_t)(~((1<<5) | (1<<4))); //stop timer
			TA0CTL |= (uint16_t)(1<<2); //clear timer
		}
	}
}


void TA0_N_IRQHandler(void) { 
	//clear interrupt flag
	TA0CTL &= (uint16_t)(~(1<<0)); 
	if((chosenLED & (uint8_t)(1 << 0)) == 0) P1OUT ^= (uint8_t)0x01; //(toggle P1.0)
	else {
			stateCounter = (stateCounter + 1) % 8;
			P2OUT &= (uint8_t)(~((1 << 0)|(1 << 1)|(1 << 2))); // Resets LED back to 0
			switch(stateCounter){
				case 0: 
					break; // Do nothing
				case 1:
					P2OUT |= (uint8_t)(1 << 0);
					break;
				case 2:
					P2OUT |= (uint8_t)(1 << 1);	
					break;
				case 3:
					P2OUT |= (uint8_t)((1 << 0)|(1 << 1));
					break;
				case 4:
					P2OUT |= (uint8_t)(1 << 2);
					break;
				case 5:
					P2OUT |= (uint8_t)((1 << 0)|(1 << 2));
					break;
				case 6:
					P2OUT |= (uint8_t)((1 << 1)|(1 << 2));
					break;
				case 7:
					P2OUT |= (uint8_t)((1 << 0)|(1 << 1)|(1 << 2));
					break;
			}
		}
}
