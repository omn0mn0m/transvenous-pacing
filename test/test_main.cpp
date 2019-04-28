#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "framework/catch.hpp"

// src files with code to test
#include "../include/config.h"
#include "../src/main.cpp"

// Mock files
#include "mock/Arduino.h"
#include "mock/BluetoothSerial.h"

// Tests
TEST_CASE( "Initialises a sensor and gets an initial reading", "[ir_init]") {
    set_pin_val(PORT_IR_0, 3000);

    ir_init(&ir_0);

    REQUIRE(ir_0.pin == PORT_IR_0);
    REQUIRE(ir_0.ambient_lvl == 3000);
    REQUIRE(ir_0.avg_reading == ir_0.ambient_lvl);
}

TEST_CASE( "Testing getting moving average", "[moving_average]" ) {
    set_pin_val(PORT_IR_0, 3000);

    ir_init(&ir_0);
    ir_0.avg_reading = get_moving_average(&ir_0);
    REQUIRE( ir_0.avg_reading == 3000);

    set_pin_val(ir_0.pin, 3500);

    ir_0.avg_reading = get_moving_average(&ir_0);
    REQUIRE( ir_0.avg_reading == 3062);

    ir_0.avg_reading = get_moving_average(&ir_0);
    REQUIRE( ir_0.avg_reading == 3116);
}

TEST_CASE( "Testing setup code", "[setup]" ) {
    set_pin_val(PORT_IR_0, 3000);
    
    setup();

    REQUIRE(ir_0.pin == PORT_IR_0);
    REQUIRE(ir_0.ambient_lvl == 3000);
    REQUIRE(ir_0.avg_reading == ir_0.ambient_lvl);
}

TEST_CASE( "Testing serial write out", "[write_out]" ) {
    for (int i = 0; i < 128; i++) {
        write_out((char)i);

        REQUIRE(SerialBT.get_data_out() == i);
    }
}

TEST_CASE( "Testing serial read in", "[read_in]" ) {
    for (int i = 0; i < 128; i++) {
        SerialBT.set_data_in((char)i);

        REQUIRE(read_in() == i);
    }
}

TEST_CASE( "Testing serial available read", "[get_available_bytes]" ) {
    SerialBT.set_available_bytes(10);

    REQUIRE(get_available_bytes() == 10);
}

TEST_CASE( "Testing command functions", "[read_command]" ) {
    set_pin_val(PORT_IR_0, 1000);
    setup();

    SECTION("Testing recalibrate command") {
        int initial_ir_val = ir_0.ambient_lvl;

        set_pin_val(PORT_IR_0, 3000);
        SerialBT.set_data_in(COM_RECALIBRATE);
        read_command();

        int new_ir_val = ir_0.ambient_lvl;

        REQUIRE((initial_ir_val == 1000 && new_ir_val == 3000));
    }

    SECTION("Testing manual position reading") {
        SerialBT.set_data_in(COM_GET_POS);
        read_command();

        REQUIRE(SerialBT.get_data_out() == POS_UHOH);
    }
}