
#include <msp430.h>

double toTicks(double); //converts frequency to ticks for ccr

void main(void) {
    //WDTCTL is watchdog timer control
    //WDTPW watchdog timer + password, is an interrupt
    //WDTHOLD watchdog timer +hold, if 1 watchdog timer is stopped
    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    //PM5CTL0 = Power mode 5 control register 0
    //LOCKLPM5 = Locks I/O pin, bit is reset by a power cycle
    //~LOCKLPM5=8'b0 and by anding PM5CTL0 it clears the register
    PM5CTL0 &= ~LOCKLPM5;


    //Enables port 1.0 as output, has to use = to initialize
    P1DIR = BIT0;
    //Enables port 9.7 as output, has to use = to initialize
    P9DIR = BIT7;



    //TA0CTL = Timer A0 Control
    //TASSEL_1 Timer_A clock source select = 01 ACLK 32k
    //MC_1 Up Mode
    TA0CTL=TASSEL_1+MC_1; //UP MODE


    TA0CCTL0 = 0x10; //Timer A0 in compare mode

    TA0CCR0=toTicks(10); //Timer A0 counts up to this, making it lower increases speed led blinks



    //enter LPM4 mode and enable global interrupt
    _BIS_SR(LPM4_bits + GIE);


}

//Port 1 ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) //double __
{

    P1OUT^=BIT0;
    P9OUT^=BIT7;


}

//converts frequency to ticks
double toTicks(double frequency){
    //f=ACLK/ticks
    //f=32k/ticks
    //ticks=32k/f
    return 32000/frequency;
}
