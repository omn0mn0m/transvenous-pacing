/**
 * know when sensor is going from one to another
 * track if the position has gone forward or backwards
 *
 * know distance
 * know it works
 * know failures in communications
 */

#include <Arduino.h>
#include <BluetoothSerial.h>

#include "config.h"

// Positions
#define POS_UHOH        0x30 // 0
#define POS_SVC         0x31 // 1
#define POS_UP_ATR      0x32 // 2
#define POS_MID_ATR     0x33 // 3
#define POS_LOW_ATR     0x34 // 4
#define POS_VEN         0x35 // 5
#define POS_VEN_WALL    0x36 // 6

// Serial Commands
#define COM_RECALIBRATE 0x43 // C
#define COM_GET_POS 0x47 // G

// Global Variables
BluetoothSerial SerialBT;

struct ir_sensor {
    int     pin;
    uint16_t avg_reading;
    uint16_t ambient_lvl;
};

struct ir_sensor ir_0 = {PORT_IR_0, 0, 0};
struct ir_sensor ir_1 = {PORT_IR_1, 0, 0};
struct ir_sensor ir_2 = {PORT_IR_2, 0, 0};
struct ir_sensor ir_3 = {PORT_IR_3, 0, 0};
struct ir_sensor ir_4 = {PORT_IR_4, 0, 0};
struct ir_sensor ir_5 = {PORT_IR_5, 0, 0};

uint8_t prev_pos = POS_UHOH;

// Global Functions
void ir_init(struct ir_sensor *ir);
uint16_t get_moving_average(struct ir_sensor *ir);
void write_out(char data);
int get_available_bytes();
char read_in();
void read_command();
void update_position();

/**
 *  Initialisation code
 */
void 
setup() {
  // put your setup code here, to run once:
  if (USE_BT) {
    SerialBT.begin("Phantom");
  } else {
    Serial.begin(BAUD_RATE);
  }

  ir_init(&ir_0);
  ir_init(&ir_1);
  ir_init(&ir_2);
  ir_init(&ir_3);
  ir_init(&ir_4);
  ir_init(&ir_5);

  if (USE_BT) {
    SerialBT.println("Initialised");
    SerialBT.print(POS_UHOH);
  } else {
    Serial.println("Initialised");
    Serial.print(POS_UHOH);
  }
}

/**
 *  Main loop for the microcontroller
 */
// LCOV_EXCL_START
void 
loop() {
  // put your main code here, to run repeatedly:
  read_command();
  update_position();
}

void
read_command() {
  int bytes_incoming = get_available_bytes();

  if (bytes_incoming > 0) {
    uint8_t command_char = read_in();
    
    switch (command_char) {
      case COM_RECALIBRATE:
        ir_init(&ir_0);
        ir_init(&ir_1);
        ir_init(&ir_2);
        ir_init(&ir_3);
        ir_init(&ir_4);
        ir_init(&ir_5);
        break;
      case COM_GET_POS:
        write_out(prev_pos);
        break;
      default:
        if (USE_BT) {
          SerialBT.println("Err: " + command_char);
        } else {
          Serial.println("Err: " + command_char);
        }
        break;
    }
  }
}

void
update_position() {
  ir_0.avg_reading = get_moving_average(&ir_0);
  ir_1.avg_reading = get_moving_average(&ir_1);
  ir_2.avg_reading = get_moving_average(&ir_2);
  ir_3.avg_reading = get_moving_average(&ir_3);
  ir_4.avg_reading = get_moving_average(&ir_4);
  ir_5.avg_reading = get_moving_average(&ir_5);
  
  if (ir_5.ambient_lvl - ir_5.avg_reading >= THRESHOLD) {
    if (prev_pos != POS_VEN_WALL) {
      prev_pos = POS_VEN_WALL;
      write_out(POS_VEN_WALL);
    }
  } else if (ir_4.ambient_lvl - ir_4.avg_reading >= THRESHOLD) {
    if (prev_pos != POS_VEN) {
      prev_pos = POS_VEN;
      write_out(POS_VEN);
    }
  } else if (ir_3.ambient_lvl - ir_3.avg_reading >= THRESHOLD) {
    if (prev_pos != POS_LOW_ATR) {
      prev_pos = POS_LOW_ATR;
      write_out(POS_LOW_ATR);
    }
  } else if (ir_2.ambient_lvl - ir_2.avg_reading >= THRESHOLD) {
    if (prev_pos != POS_MID_ATR) {
      prev_pos = POS_MID_ATR;
      write_out(POS_MID_ATR);
    }
  } else if (ir_1.ambient_lvl - ir_1.avg_reading >= THRESHOLD) {
    if (prev_pos != POS_UP_ATR) {
      prev_pos = POS_UP_ATR;
      write_out(POS_UP_ATR);
    }
  } else if (ir_0.ambient_lvl - ir_0.avg_reading >= THRESHOLD) {
    if (prev_pos != POS_SVC) {
      prev_pos = POS_SVC;
      write_out(POS_SVC);
    }
  } else {
    if (prev_pos != POS_UHOH) {
      prev_pos = POS_UHOH;
      write_out(POS_UHOH);
    }
  }
}
// LCOV_EXCL_STOP 

/**
 * Calculates the first average and ambient level for the sensor
 * 
 * @param ir Pointer to a sensor struct
 */
void 
ir_init(struct ir_sensor *ir) {
    uint32_t sum_samples = 0;

    for (uint8_t i = 0; i < SAMPLE_SIZE; i++) {
        sum_samples += analogRead(ir->pin);
        if (USE_BT) {
          SerialBT.println(sum_samples);
        } else {
          Serial.println(sum_samples);
        }
        
        delay(50);
    }

    ir->avg_reading = sum_samples / SAMPLE_SIZE;
    ir->ambient_lvl = ir->avg_reading;

    if (USE_BT) {
      SerialBT.println("Calibrated!");
    } else {
      Serial.println("Calibrated!");
    }
}

/**
 * Returns an exponential moving average
 * (more like an IIR filter than anything else though)
 * 
 * @param ir_sensor The sensor to read from
 * 
 * @return Moving Average 
 */
uint16_t
get_moving_average(struct ir_sensor *ir) {
    uint16_t moving_avg = ((ir->avg_reading * (SAMPLE_SIZE - 1)) + analogRead(ir->pin)) / SAMPLE_SIZE;

    return moving_avg;
}

void
write_out(char data) {
  if (USE_BT) {
    SerialBT.write(data);
  } else {
    Serial.write(data);
  }
}

int
get_available_bytes() {
  if (USE_BT) {
    return SerialBT.available();
  } else {
    return Serial.available();
  }
}

char
read_in() {
  if (USE_BT) {
      return SerialBT.read();
    } else {
      return Serial.read();
    }
}
