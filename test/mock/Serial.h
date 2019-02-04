#ifndef SERIAL_H
#define SERIAL_H

class HardwareSerial {
    public:
        HardwareSerial() { }

        int available_bytes;
        char data_in;

        void begin(unsigned long) { return; }

        void println(const char*) { return; }

        void println(unsigned int) { return; }
        
        void print(char) { return; }
        
        int available() { return available_bytes; }

        void write(char) {  return; }

        char read() { return data_in; }
};

HardwareSerial Serial;

#endif // SERIAL_H