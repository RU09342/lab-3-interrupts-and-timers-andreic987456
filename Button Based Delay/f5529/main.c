#include <msp430.h> 

unsigned long hz;
int reset = 0;
unsigned int start = 0;
unsigned int end =0;
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    TA0CCTL0 |= CCIE;                              //enable interrupt
    TA0CCTL0 &= ~TAIFG;                            //clear flag
    TA0CTL = TASSEL_1 + MC_2 + ID_3;           // ACLK, contmode, clk/8
    TA0CCR0 = 205;                               //20 Hz (10Hz for LED) (visually correct, ACLK seems to be too slow)
    hz = 205;                                    //increment value for CCR0


    P1IE |=  BIT1;                            // P1.1 interrupt enabled
    P1IES |= BIT1;                            //falling edge
    P1REN |= BIT1;                            // Enable resistor on SW2 (P1.1)
    P1OUT |= BIT1;                             //Pull up resistor on P1.1
    P1IFG &= ~BIT1;                           // P1.1 Interrupt Flag cleared

    P1DIR |= BIT0;       // P1.0 pin output

    __bis_SR_register(GIE);  //not low power mode
    while(1){
        if(reset)
        {
            reset = 0;
            if(end <= start){                       //account for overflow (will work for button press up to 16 seconds)
                TA0CCR0 = 65535-start+end+TA0R;     //account for current timer value to start immediately
                hz=65535-start+end;                 //update increment value
            }else{
                TA0CCR0=end-start+TA0R;
                hz=end-start;
            }
        }

    }
}


//Timer A interrupt routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
        P1OUT ^= BIT0;
        TA0CCR0 += hz;                          //increment CCR0
}



#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)

{

    if(P1IES & BIT1){
    start = TA0R;                       //time button was pressed
    }else{
        end = TA0R;                     //time button was released
        reset = 1;                      //flag to recalculate CCR1
    }

    P1IES ^= BIT1;  //flip edge
    P1IFG &= ~BIT1;  //clear flag

}
