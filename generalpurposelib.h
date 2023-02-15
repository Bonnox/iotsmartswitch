#pragma once

/**
milliseconds in a second
*/
#define SECOND_MILLIS 1000

#define SERIAL_SPEED_SLOW 9600
#define SERIAL_SPEED_RECOMMENDED 115200

#define STRING_METRIC_SIZE 200

/**
the maximum number of values that an ADC read can use
*/
#define ADC_SAMPLES (2<<11)	//4096

/*
the voltage that represents the "logic 1" in the microcontroller
*/
#define LOGIC_LEVEL_VOLTAGE 3.3


/**
* out of bounds
* we use >= instead of == to catch every possible error.
* basically, automatically fix invariants not satisfied
* that would make the process crash. also checks for < 0
*/
	// 
#define out_of_bounds_array(n, size) ( (n >= size) || (n < 0) )