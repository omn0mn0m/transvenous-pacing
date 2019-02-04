#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "framework/catch.hpp"

// src files with code to test
#include "../include/config.h"
#include "../src/main.cpp"

// Mock files
#include "mock/Arduino.h"

// Mocks

// Tests
TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( test_function(1) == 1 );
    REQUIRE( test_function(2) == 2 );
    REQUIRE( test_function(3) == 3 );
}

TEST_CASE( "Initialises a sensor and gets an initial reading", "[ir_init") {
    set_pin_val(PORT_IR_0, 3000);

    ir_init(&ir_0);

    REQUIRE(ir_0.pin == PORT_IR_0);
    REQUIRE(ir_0.ambient_lvl == 3000);
    REQUIRE(ir_0.avg_reading == ir_0.ambient_lvl);
}

TEST_CASE( "Testing getting moving average", "[moving_average" ) {
    set_pin_val(PORT_IR_0, 3000);

    ir_init(&ir_0);

    REQUIRE( get_moving_average(&ir_0) == 3000);

    set_pin_val(ir_0.pin, 3500);
    REQUIRE( get_moving_average(&ir_0) == 3062);
    REQUIRE( get_moving_average(&ir_0) == 3116);
}