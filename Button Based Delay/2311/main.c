#include <msp430.h> 
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
                               //needed to reset device
    P1SEL0 = 0x00; //sets P1 to GPIO
    P1SEL1 = 0x00; //sets P1 to GPIO
    P1DIR = BIT0; //sets LED at P1.0 to output
    P1REN = BIT1; //Resistor enabled for button P1.1
    P1OUT = BIT1; //Sets resistor to pull up for button 1 when not pressed

    PM5CTL0 &= ~LOCKLPM5;

    P1IE |= BIT1; //interupt enable for button P1.1
    P1IES |= BIT1; //sets interupt to trigger on falling edge (button press)
    P1IFG &= ~BIT1; //Clears interupt flag register

    __bis_SR_register(GIE); //global interupt enable (enables all maskable interupts)

    TB0CCTL0 = CCIE; // CCR0 interrupt enabled
    TB0CCR0 = 6000; //register that timer will compare itself too
    TB0CTL = TBSSEL_2 + ID_3 + MC_1 + TBCLR; // SMCLK, divide clock by 8, upmode, clear TAR

    for(;;){}
    return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    if ((P1IN&BIT1)==0x0000) //if button pressed
    {
    P1IES &= ~BIT1; //sets interupt to trigger on rising edge (button release)
    TB0CTL = TBSSEL_2 + ID_3 + MC_2 + TBCLR; // SMCLK, divide clock by 8, continous mode, clear TAR
    P1IFG &= ~BIT1; //clear interupt flag
    }
    else //if button released
    {
    P1IES |= BIT1; //sets interupt to trigger on falling edge (button press)
    TB0CCR0 = TB0R; //changes ccr0 to value of timer
    TB0CTL = TBSSEL_2 + ID_3 + MC_1 + TBCLR; // SMCLK, divide clock by 8, upmode, clear TAR
    P1IFG &= ~BIT1; //clears interupt flag
    }
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void){
            P1OUT ^= BIT0; //toggles LED when interrupt triggered
}
