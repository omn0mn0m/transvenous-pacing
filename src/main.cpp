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
#define POS_PUL_VEIN    0x37 // 7
#define POS_IVC         0x38 // 8

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
// struct ir_sensor ir_1(PORT_IR_1);
// struct ir_sensor ir_2(PORT_IR_2);
// struct ir_sensor ir_3(PORT_IR_3);
// struct ir_sensor ir_4(PORT_IR_4);
// struct ir_sensor ir_5(PORT_IR_5);
// struct ir_sensor ir_6(PORT_IR_6);
// struct ir_sensor ir_7(PORT_IR_7);
// struct ir_sensor ir_8(PORT_IR_8);

uint8_t prev_pos = POS_UHOH;

// Global Functions
void ir_init(struct ir_sensor *ir);
uint16_t get_moving_average(struct ir_sensor *ir);

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
  // ir_init(&ir_1);
  // ir_init(&ir_2);
  // ir_init(&ir_3);
  // ir_init(&ir_4);
  // ir_init(&ir_5);
  // ir_init(&ir_6);
  // ir_init(&ir_7);
  // ir_init(&ir_8);

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
void 
loop() {
  // put your main code here, to run repeatedly:
  int bytes_incoming;

  if (USE_BT) {
    bytes_incoming = SerialBT.available();
  } else {
    bytes_incoming = Serial.available();
  }

  if (bytes_incoming > 0) {
    uint8_t command_char;

    if (USE_BT) {
      command_char = SerialBT.read();
    } else {
      command_char = Serial.read();
    }
    
    switch (command_char) {
      case COM_RECALIBRATE:
        ir_init(&ir_0);
        // ir_init(&ir_1);
        // ir_init(&ir_2);
        // ir_init(&ir_3);
        // ir_init(&ir_4);
        // ir_init(&ir_5);
        // ir_init(&ir_6);
        // ir_init(&ir_7);
        // ir_init(&ir_8);
        break;
      case COM_GET_POS:
        if (USE_BT) {
          SerialBT.write(prev_pos);
        } else {
          Serial.write(prev_pos);
        }
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

  ir_0.avg_reading = get_moving_average(&ir_0);
  // ir_1.avg_reading = get_moving_average(&ir_1);
  // ir_2.avg_reading = get_moving_average(&ir_2);
  // ir_3.avg_reading = get_moving_average(&ir_3);
  // ir_4.avg_reading = get_moving_average(&ir_4);
  // ir_5.avg_reading = get_moving_average(&ir_5);
  // ir_6.avg_reading = get_moving_average(&ir_6);
  // ir_7.avg_reading = get_moving_average(&ir_7);
  // ir_8.avg_reading = get_moving_average(&ir_8);
  
  // if (ir_6.ambient_lvl - ir_6.avg_reading >= THRESHOLD) {
  //   if (prev_pos != POS_PUL_VEIN) {
  //     prev_pos = POS_PUL_VEIN;
  //     pc_serial.putc(POS_PUL_VEIN);
  //   }
  // } else if (ir_5.ambient_lvl - ir_5.avg_reading >= THRESHOLD) {
  //   if (prev_pos != POS_VEN_WALL) {
  //     prev_pos = POS_VEN_WALL;
  //     pc_serial.putc(POS_VEN_WALL);
  //   }
  // } else if (ir_4.ambient_lvl - ir_4.avg_reading >= THRESHOLD) {
  //   if (prev_pos != POS_VEN) {
  //     prev_pos = POS_VEN;
  //     pc_serial.putc(POS_VEN);
  //   }
  // } else if (ir_3.ambient_lvl - ir_3.avg_reading >= THRESHOLD) {
  //   if (prev_pos != POS_LOW_ATR) {
  //     prev_pos = POS_LOW_ATR;
  //     pc_serial.putc(POS_LOW_ATR);
  //   }
  // } else if (ir_2.ambient_lvl - ir_2.avg_reading >= THRESHOLD) {
  //   if (prev_pos != POS_MID_ATR) {
  //     prev_pos = POS_MID_ATR;
  //     pc_serial.putc(POS_MID_ATR);
  //   }
  // } else if (ir_1.ambient_lvl - ir_1.avg_reading >= THRESHOLD) {
  //   if (prev_pos != POS_UP_ATR) {
  //     prev_pos = POS_UP_ATR;
  //     pc_serial.putc(POS_UP_ATR);
  //   }
  // } else 
  if (ir_0.ambient_lvl - ir_0.avg_reading >= THRESHOLD) {
    if (prev_pos != POS_SVC) {
      prev_pos = POS_SVC;
      if (USE_BT) {
        SerialBT.write(POS_SVC);
      } else {
        Serial.write(POS_SVC);
      }
    }
  } else {
    if (prev_pos != POS_UHOH) {
      prev_pos = POS_UHOH;
      if (USE_BT) {
        SerialBT.write(POS_UHOH);
      } else {
        Serial.write(POS_UHOH);
      }
    }
  }
}

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
