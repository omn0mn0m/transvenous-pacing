#include <mbed.h>

// Software Constants
#define THRESHOLD 300

// Hardware Constants
#define PORT_IR_0 A0
#define PORT_IR_1 A1

#define PC_SERIAL_TX USBTX
#define PC_SERIAL_RX USBRX

// Global Pin Assignments
AnalogIn ir_0(PORT_IR_0);
AnalogIn ir_1(PORT_IR_1);

// Global Class Assignments
Serial pc_serial(PC_SERIAL_TX, PC_SERIAL_RX);

/**
 *  Main code for the microcontroller
 */
int main() {
    // put your setup code here, to run once:
    pc_serial.printf("Phantom Initialised\n");

    while(1) {
        // put your main code here, to run repeatedly:
        if (ir_0 <= THRESHOLD) {
            pc_serial.putc('0');
        } else if (ir_1 <= THRESHOLD) {
            pc_serial.putc('1');
        }
    }
}