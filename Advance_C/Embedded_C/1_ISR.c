// #include <avr/io.h>
// #include <avr/interrupt.h>

// volatile uint8_t button_pressed = 0;

// ISR(INT0_vect) {
//     button_pressed = 1;  // Set flag
// }

// int main(void) {
//     DDRD &= ~(1 << PD2);    // Set PD2 (INT0) as input
//     EIMSK |= (1 << INT0);   // Enable external interrupt INT0
//     EICRA |= (1 << ISC01);  // Trigger INT0 on falling edge
//     sei();                  // Enable global interrupts

//     while (1) {
//         if (button_pressed) {
//             // Handle button press
//             button_pressed = 0;
//         }
//     }
// }
