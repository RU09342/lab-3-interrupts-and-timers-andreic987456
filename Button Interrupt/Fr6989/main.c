#include <msp430.h>

int count=0;                         //number of times button is pressed
int k;                               //how many times led blinks

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



    P1IE |=BIT1;                           //enable interrupt on port 1.1
    P1IES |=BIT1;                            //sets as falling edge
    P1IFG &=~(BIT1);                     //clear interrupt flag

                                        //enter LPM4 mode and enable global interrupt
    _BIS_SR(LPM4_bits + GIE);


}

                                                   //Port 1 ISR
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)                        //double __
{

                                                   //checks to see if p1.1 is low
        if (!((P1IN & BIT1) == BIT1)){
            count = count +1;

                                                   //checks to see how many times button is pressed
            switch(count){
            case 1:


            for (k =0; k<=10; k++){

            volatile unsigned int i;
                                                   //toggles p1.0 red led
            P1OUT ^= BIT0;

                                                  //red led delay slow
            i = 25000;
                                                    //will decrement i unless i = 0
            do i--;
            while(i != 0);
            }



            break;
            case 2:


            for (k =0; k<=10; k++){

            volatile unsigned int i;
                                                      //toggles p1.0 red led
            P1OUT ^= BIT0;

                                                      //red led delay medium
            i = 80000;
                                                      //will decrement i unless i = 0
            do i--;
            while(i != 0);
            }


            break;

            case 3:
            for (k =0; k<=10; k++){

            volatile unsigned int i;
                                                      //toggles p1.0 red led
            P1OUT ^= BIT0;

                                                     //red led delay fast
            i = 30000;
                                                     //will decrement i unless i = 0
            do i--;
            while(i != 0);
            }

            break;

            default:count=0;                               //resets count

        }

    }



}


