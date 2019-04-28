#ifndef SERIAL_H
#define SERIAL_H

class HardwareSerial {
    public:
        HardwareSerial() { }

        int available_bytes;
        char data_in;
        char data_out;

        // Class Mock Functions
        void begin(unsigned long) { return; }

        void println(const char*) { return; }

        void println(unsigned int) { return; }
        
        void print(char) { return; }
        
        int available() { return available_bytes; }

        void write(char data) { data_out = data; }

        char read() { return data_in; }

        // Helper Functions
        void set_data_in(char data) { data_in = data; };

        void set_available_bytes(int num) { available_bytes = num; };

        char get_data_out() { return data_out; };
};

HardwareSerial Serial;

#endif // SERIAL_H