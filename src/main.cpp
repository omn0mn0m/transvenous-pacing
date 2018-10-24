#include <mbed.h>
#include "config.h"

// Global Pin Assignments
AnalogIn ir_0(PORT_IR_0);
AnalogIn ir_1(PORT_IR_1);
AnalogIn ir_2(PORT_IR_2);

// Global Class Assignments
Serial pc_serial(PC_SERIAL_TX, PC_SERIAL_RX);

// Global Variables
u_int16_t samples[SAMPLE_SIZE];
int sum_samples = 0;
u_int16_t avg_reading;
u_int16_t ambient_lvl;

/**
 *  Main code for the microcontroller
 */
int main() {
    // put your setup code here, to run once:
    pc_serial.printf("Phantom Initialised\n");

    for (int i = 0; i < SAMPLE_SIZE; i++) {
        samples[i] = ir_0.read_u16();
        sum_samples += samples[i];
    }

    avg_reading = sum_samples / SAMPLE_SIZE;
    ambient_lvl = avg_reading;

    pc_serial.printf("%d\n", avg_reading);

    while (1) {
        // put your main code here, to run repeatedly:
        int old_sample = samples[0];

        for (int i = 1; i < SAMPLE_SIZE; i++) {
            samples[i - 1] = samples[i];
        }

        samples[SAMPLE_SIZE - 1] = ir_0.read_u16();

        sum_samples = sum_samples + samples[SAMPLE_SIZE - 1] - old_sample;
        avg_reading = sum_samples / SAMPLE_SIZE;

        if (abs(ambient_lvl - avg_reading) >= THRESHOLD) {
            pc_serial.putc('0');
        } 
        // else if (ir_1 <= THRESHOLD) {
        //     pc_serial.putc('1');
        // }

        wait_ms(10);
    }
}