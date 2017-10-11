#include <msp430.h> 
void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    P1DIR |= BIT0;                            // Set P1.0 to output direction
    P1IE |=  BIT3;                            // P1.3 interrupt enabled
    P1IES |= BIT3;                            // P1.3 falling edge trigger
    P1REN |= BIT3;                            // Enable resistor on SW2 (P1.3)
    P1OUT |= BIT3;                             //Pull up resistor on P1.3
    P1IFG &= ~BIT3;                           // P1.3 Interrupt Flag cleared
                                              //BIT3 on Port 1 can be used as Switch2

    __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4 w/interrupt
    //Low Power Mode 4 shuts off three clocks and the CPU, this would have to change
    //if a timer was used
  }

  // Port 1 interrupt service routine
  #pragma vector=PORT1_VECTOR
  __interrupt void Port_1(void)

  {
    P1OUT ^= BIT0;                            // P1.0 = toggle
    P1IFG &= ~BIT3;                           // P1.3 IFG cleared
}
