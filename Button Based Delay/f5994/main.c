#include <msp430fr5994.h>

#define LED     BIT0
#define BUTTON  BIT6

unsigned int buttonPressed;
unsigned int en = 0x00;
unsigned int count = 0;
unsigned int i = 0;

void initTIMER(int f);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO

    P1DIR |= LED; // Set P1.0 as output
    P1OUT &= ~LED; // Turn LED off

    P5DIR &= ~BUTTON; // Set P1.1 as input
    P5REN |= BUTTON; // Enable puller resistor on button-pin
    P5OUT |= BUTTON; // Make resistor pull up

    P5IES |= BUTTON; // Interrupts when you press button
    P5IE |= BUTTON; // Enable interrupt on button pin
    P5IFG &= ~BUTTON; // Clear interrupt flag


    initTIMER(10);    // initialize timer to 10Hz

    __enable_interrupt();

    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

void initTIMER(int f) // call function with desired frequency to initialize timer module
{
    int n;
    TB0CCTL0 = CCIE; // Enable interrupt in compare mode
    TB0CTL = TBSSEL_2 + MC_1 + ID_2; // SMCLK/4, Up
    n = 250000 / f; //250000 / 10 = 25000
    TB0CCR0 = n; // ( 10^6 [Hz] / 4) / (25000) = 10Hz
}

// TIMER A interrupt routine
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B0(void)
{
    if (en != 0x00)
    {
        if (P5IN & BUTTON) // Reset count and enabled when button is up
        {
            buttonPressed = 0;
            en = 0;
            count = 0;
        }

        else    // Increment i while the button is pressed on each timer interrupt
        {
            P1OUT |= LED;
            i++;
        }


    }

    else                                  // Increment until count == i
    {
        if (count == i) // Toggle every i interrupts (i * 0.1 = 5 s)
        {
            P1OUT ^= LED; //Toggle LED
            count = 0;  // reset count
        }

        else
            count++;    // increment count each interrupt until count = i
    }
}

// Port 1 interrupt routine
#pragma vector = PORT5_VECTOR
__interrupt void PORT_5(void)
{
    en = 1;
    i = 0; // Reset timing counter
    buttonPressed = 1;
    P5IFG &= ~BUTTON; // Clear interrupt flag
}
