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
    u_int16_t  samples[SAMPLE_SIZE];
    u_int16_t  avg_reading;
    u_int16_t  ambient_lvl;
    int        sum_samples;
    // Constructor
    ir_sensor(PinName analog_pin) : sensor(analog_pin) {};
};

struct ir_sensor ir_0(PORT_IR_0);
struct ir_sensor ir_1(PORT_IR_1);
struct ir_sensor ir_2(PORT_IR_2);

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

    pc_serial.printf("Initialised\n");

    u_int8_t prev_pos = POS_UHOH;
    pc_serial.putc(POS_UHOH);

    while (1) {
        // put your main code here, to run repeatedly:
        ir_0.avg_reading = get_moving_average(&ir_0);
        ir_1.avg_reading = get_moving_average(&ir_1);
        ir_2.avg_reading = get_moving_average(&ir_2);
        
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

void 
ir_init(struct ir_sensor *ir) {
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        ir->samples[i] = ir->sensor.read_u16();
        ir->sum_samples += ir->samples[i];
    }

    ir->avg_reading = ir->sum_samples / SAMPLE_SIZE;
    ir->ambient_lvl = ir->avg_reading;

    // pc_serial.printf("%d\n", ir->ambient_lvl);
}

u_int16_t
get_moving_average(struct ir_sensor *ir) {
    int old_sample = ir->samples[0];

    for (int i = 1; i < SAMPLE_SIZE; i++) {
        ir->samples[i - 1] = ir->samples[i];
    }

    ir->samples[SAMPLE_SIZE - 1] = ir->sensor.read_u16();

    ir->sum_samples = ir->sum_samples + ir->samples[SAMPLE_SIZE - 1] - old_sample;

    // pc_serial.printf("%d\n", ir->avg_reading);

    return ir->sum_samples / SAMPLE_SIZE;
}