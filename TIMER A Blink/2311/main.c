#include <msp430.h> 
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
                               //needed to reset device
    P1SEL0 = 0x00; //sets P1 to GPIO
    P1SEL1 = 0x00; //sets P1 to GPIO
    P1DIR = BIT0; //sets LED at P1.0 to output

    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(GIE); //global interupt enable (enables all maskable interupts)

    TB0CCTL0 = CCIE; // CCR0 interrupt enabled
    TB0CCR0 = 60000; //register that timer will compare itself too
    TB0CTL = TBSSEL_2 + MC_1 + TBCLR; // SMCLK, upmode, clear TAR

    for(;;){}
    return 0;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void){
            P1OUT ^= BIT0; //toggles LED when interrupt triggered
}
