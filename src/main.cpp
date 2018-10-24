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
u_int16_t ir_avg_0;
u_int16_t ambient_lvl;

// Global Functions
void ir_init(AnalogIn ir_sensor);
u_int16_t get_moving_average();

/**
 *  Main code for the microcontroller
 */
int 
main(void) {
    // put your setup code here, to run once:
    ir_init(ir_0);

    pc_serial.printf("Initialised\n");

    while (1) {
        // put your main code here, to run repeatedly:
        ir_avg_0 = get_moving_average();

        if (abs(ambient_lvl - ir_avg_0) >= THRESHOLD) {
            pc_serial.putc('0');
        } 
        // else if (ir_1 <= THRESHOLD) {
        //     pc_serial.putc('1');
        // }

        wait_ms(10);
    }
}

void 
ir_init(AnalogIn ir_sensor) {
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        samples[i] = ir_sensor.read_u16();
        sum_samples += samples[i];
    }

    ir_avg_0 = sum_samples / SAMPLE_SIZE;
    ambient_lvl = ir_avg_0;

    // pc_serial.printf("%d\n", avg_reading);
}

u_int16_t
get_moving_average() {
    int old_sample = samples[0];

    for (int i = 1; i < SAMPLE_SIZE; i++) {
        samples[i - 1] = samples[i];
    }

    samples[SAMPLE_SIZE - 1] = ir_0.read_u16();

    sum_samples = sum_samples + samples[SAMPLE_SIZE - 1] - old_sample;

    return sum_samples / SAMPLE_SIZE;
}