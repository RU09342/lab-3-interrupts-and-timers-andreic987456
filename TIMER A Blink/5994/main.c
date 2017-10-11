#include <msp430fr5994.h>


#define LED1        BIT0
#define LED2        BIT1
#define LED_OUT     P1OUT
#define LED_DIR     P1DIR

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO

    P1SEL0 &= ~(LED1 + LED2); // Select I/O direction for P1.0 and P1.1
    P1SEL1 &= ~(LED1 + LED2);
    LED_DIR |= LED1 + LED2; // Set P1.0 and P1.1 to output direction
    LED_OUT &= ~(LED1 + LED2); // Clear the LEDs

    TB0CCTL0 |= CCIE;
    TB0CTL |= TBSSEL_2 + MC_2 + ID_3; // Set the timer B to SMCLCK/8, Continuous
    // Clear the timer and enable timer interrupt
    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // LPM0 with interrupts enabled

}


// Timer B0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0 (void)
{
    LED_OUT ^= LED1 + LED2; // Toggle the LEDs
}
