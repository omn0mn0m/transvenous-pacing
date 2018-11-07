#include <mbed.h>
#include "config.h"

// Positions
#define POS_UHOH     '0'
#define POS_SVC      '1'
#define POS_UP_ATR   '2'
#define POS_MID_ATR  '3'
#define POS_LOW_ATR  '4'
#define POS_VEN      '5'
#define POS_VEN_WALL '6'
#define POS_PUL_VEIN '7'
#define POS_IVC      '8'

// Global Class Assignments
Serial pc_serial(PC_SERIAL_TX, PC_SERIAL_RX);

// Global Variables
struct ir_sensor {
    AnalogIn   sensor;
    u_int16_t  avg_reading;
    u_int16_t  ambient_lvl;
    // Constructor
    ir_sensor(PinName analog_pin) : sensor(analog_pin) {};
};

struct ir_sensor ir_0(PORT_IR_0);
struct ir_sensor ir_1(PORT_IR_1);
struct ir_sensor ir_2(PORT_IR_2);
// struct ir_sensor ir_3(PORT_IR_3);
// struct ir_sensor ir_4(PORT_IR_4);
// struct ir_sensor ir_5(PORT_IR_5);
// struct ir_sensor ir_6(PORT_IR_6);
// struct ir_sensor ir_7(PORT_IR_7);
// struct ir_sensor ir_8(PORT_IR_8);

// Global Functions
void ir_init(struct ir_sensor *ir);
u_int16_t get_moving_average(struct ir_sensor *ir);

/**
 *  Main code for the microcontroller
 */
int 
main(void) {
    // put your setup code here, to run once:
    ir_init(&ir_0);
    ir_init(&ir_1);
    ir_init(&ir_2);
    // ir_init(&ir_3);
    // ir_init(&ir_4);
    // ir_init(&ir_5);
    // ir_init(&ir_6);
    // ir_init(&ir_7);
    // ir_init(&ir_8);

    pc_serial.printf("Initialised\n");

    u_int8_t prev_pos = POS_UHOH;
    pc_serial.putc(POS_UHOH);

    while (1) {
        // put your main code here, to run repeatedly:
        ir_0.avg_reading = get_moving_average(&ir_0);
        ir_1.avg_reading = get_moving_average(&ir_1);
        ir_2.avg_reading = get_moving_average(&ir_2);
        // ir_3.avg_reading = get_moving_average(&ir_3);
        // ir_4.avg_reading = get_moving_average(&ir_4);
        // ir_5.avg_reading = get_moving_average(&ir_5);
        // ir_6.avg_reading = get_moving_average(&ir_6);
        // ir_7.avg_reading = get_moving_average(&ir_7);
        // ir_8.avg_reading = get_moving_average(&ir_8);
        
        if (abs(ir_2.ambient_lvl - ir_2.avg_reading) >= THRESHOLD) {
            if (prev_pos != POS_MID_ATR) {
                prev_pos = POS_MID_ATR;
                pc_serial.putc(POS_MID_ATR);
            }
        } else if (abs(ir_1.ambient_lvl - ir_1.avg_reading) >= THRESHOLD) {
            if (prev_pos != POS_UP_ATR) {
                prev_pos = POS_UP_ATR;
                pc_serial.putc(POS_UP_ATR);
            }
        } else if (abs(ir_0.ambient_lvl - ir_0.avg_reading) >= THRESHOLD) {
            if (prev_pos != POS_SVC) {
                prev_pos = POS_SVC;
                pc_serial.putc(POS_SVC);
            }
        } else {
            if (prev_pos != POS_UHOH) {
                prev_pos = POS_UHOH;
                pc_serial.putc(POS_UHOH);
            }
        }
        
        wait_ms(10);
    }
}

/**
 * Calculates the first average and ambient level for the sensor
 * 
 * @param ir Pointer to a sensor struct
 */
void 
ir_init(struct ir_sensor *ir) {
    int sum_samples;

    for (int i = 0; i < SAMPLE_SIZE; i++) {
        sum_samples += ir->sensor.read_u16();
    }

    ir->avg_reading = sum_samples / SAMPLE_SIZE;
    ir->ambient_lvl = ir->avg_reading;
}

/**
 * Returns an exponential moving average
 * (more like an IIR filter than anything else though)
 * 
 * @param ir_sensor The sensor to read from
 * 
 * @return Moving Average 
 */
u_int16_t
get_moving_average(struct ir_sensor *ir) {
    return (ir->avg_reading * (SAMPLE_SIZE - 1) + ir->sensor.read_u16()) / SAMPLE_SIZE;
}