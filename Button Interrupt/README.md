buttons innterupts will turn the board to low power mode and go to higher power mode on button press to continue the code.
in this case 
P1DIR |= LED0 sets the led as output P1.0
p1OUT &= ~LED0  starts on off
low power mode is the start until interupt occurs toggling LED0