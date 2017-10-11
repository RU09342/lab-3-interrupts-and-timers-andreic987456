#include <msp430.h>
#define BUTTON BIT1                 //Define "BUTTON" as bit 1
#define LED0 BIT0                   //Define "LED0" as bit 0

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       //Stop watchdog timer

    P1DIR |= LED0;                  //Set P1.0 (LED) as an output
    P1OUT &= ~LED0;                 //Set the initial LED condition to off

    P1DIR &= ~BUTTON;               //Set P1.1 (Button) as an input
    P1REN |= BUTTON;                //Enable the pull resistor on P1.1
    P1OUT |= BUTTON;                //Tell the pull resistor to pull up
    P1IE |= BUTTON;                 //Enable interrupt on P1.1
    P1IES |= BUTTON;                //Set the P1.1 interrupt to trigger on a high->low edge.
    P1IFG &= ~BUTTON;               //Clear the interrupt flag register on P1.1

    PM5CTL0 &= ~LOCKLPM5;            //Disable high impedance mode.

    __bis_SR_register(LPM4_bits + GIE); //Enter into low power mode to idle until the interrupt occurs and enable global interrupts

}

#pragma vector=PORT1_VECTOR          //Set the port 1 interrupt routine
__interrupt void Port_1(void) {
    P1OUT ^= LED0;                   //Toggle P1.0 (LED)
    P1IFG &= ~BUTTON;                //P1.1 IFG cleared
}
