#ifndef BLUETOOTH_SERIAL_H
#define BLUETOOTH_SERIAL_H

#include "Serial.h"

class BluetoothSerial : public HardwareSerial {
    public:
        BluetoothSerial() { };

        void begin(const char*) { };
};

#endif // BLUETOOTH_SERIAL_H