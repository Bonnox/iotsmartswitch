#pragma once

/**
milliseconds in a second
*/
#define SECOND_MILLIS 1000

#define SERIAL_SPEED_SLOW 9600
#define SERIAL_SPEED_RECOMMENDED 115200

#define STRING_METRIC_SIZE 200



#define STRING_SPACE " "


/**
* out of bounds
* we use >= instead of == to catch every possible error.
* basically, automatically fix invariants not satisfied
* that would make the process crash. also checks for < 0
*/
	// 
#define out_of_bounds_array(n, size) ( (n >= size) || (n < 0) )