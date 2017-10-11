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


        //resistor enabled
        P1REN |= BIT1;

        //Sets Resistor to pullup, 1.0 is low, has to use = to initialize
        P1OUT = BIT1;

        //Enables port 1.0 as output, has to use = to initialize
        P1DIR = BIT0;



        P1IE |=BIT1; //enable interrupt on port 1.1
        P1IES |=BIT1; //sets as falling edge
        P1IFG &=~(BIT1);//clear interrupt flag


        //TA0CTL = Timer A0 Control
          //TASSEL_1 Timer_A clock source select = 01 ACLK 32k
          //MC_1 Up Mode

          TA0CTL = TASSEL_1 + MC_1;

          //Timer A0 in compare mode
          TA0CCTL0 = 0x0010;

          //initial rate the led blinks, 10Hz
          TA0CCR0 = toTicks(10);


        //enter LPM4 mode and enable global interrupt
        _BIS_SR(LPM4_bits + GIE);


}

//timer A interrupt
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void) //double __
{

    P1OUT^=BIT0;

}

//button interrupt, ta0ccr0 will count from 0 to the final value when button is released
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

    TA0CCR0 = 0; //initializes

    //checks for button input
    while(!((P1IN & BIT1) == BIT1)){
        TA0CCR0 = TA0CCR0 + 1; //increments the ccr
    }

    //clears interrupt flag to exit interrupt
    P1IFG &= ~BIT1;
}

//converts frequency to ticks
double toTicks(double frequency){
    //f=ACLK/ticks
    //f=32k/ticks
    //ticks=32k/f
    return 32000/frequency;
}
