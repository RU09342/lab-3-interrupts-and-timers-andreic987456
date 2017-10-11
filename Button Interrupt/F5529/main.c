#include <msp430.h> 

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    P1DIR |= BIT0;                            // Set P1.0 to output direction
    P1IE |=  BIT1;                            // P1.1 interrupt enabled
    P1IES |= BIT1;                            // P1.1 falling edge trigger
    P1REN |= BIT1;                            // Enable resistor on SW2 (P1.1)
    P1OUT |= BIT1;                             //Pull up resistor on P1.1
    P1IFG &= ~BIT1;                           // P1.1 Interrupt Flag cleared
                                              //BIT1 on Port 1 can be used as Switch2

    __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt
    //Low Power Mode 4 shuts off three clocks and the CPU, this would have to change
    //if a timer was used
  }

  // Port 1 interrupt service routine
  #pragma vector=PORT1_VECTOR
  __interrupt void Port_1(void)

  {
    P1OUT ^= BIT0;                            // P1.0 = toggle
    P1IFG &= ~BIT1;                           // P1.1 IFG cleared
}
