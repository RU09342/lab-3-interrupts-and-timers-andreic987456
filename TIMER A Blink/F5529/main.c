#include <msp430.h> 


/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    TA0CCTL2 = CCIE;                             // CCR2 interrupt enabled
    TA0CCTL1 = CCIE;
    TA0CTL = TASSEL_2 + MC_2 + ID_3;           // SMCLK, contmode, clk/8
    TA0CCR2 =  40000;                           // 3.125 Hz -- 1Mhz/8/40000
    TA0CCR1 = 10000;                               //6.25 Hz

    P1DIR |= BIT0;       // P1.0 pin output
    P4DIR |= BIT7;       //P4.7 output


    __bis_SR_register(CPUOFF + GIE);  //clocks are needed so LPM0 with just CPUOFF is used
}

//Timer A interrupt routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A (void)
{
    switch( TA0IV){
    case 2:
    {
        P4OUT ^= BIT7;                      // Toggle P4.7
        TA0CCR1 += 10000;                      //Inc CCR1 to trigger again
    }
    break;
    case 4:
        {
           P1OUT ^= BIT0;                          // Toggle P1.0, IFG gets cleared automatically
           TA0CCR2 += 40000;
        }
        break;
    case 10:
    {
    }
    break;
    }
}
