#pragma once

#include "../../multitasklib.h"

#include <stdbool.h>

/**
number of values to average
*/
#define BUFFER_SIZE 50
/*
the wait between each measurement
*/
#define TASK_CURRENTMONITOR_WAIT 100

typedef struct
{	
	/**
	The real voltage read from the circuit after an average over a short period of time
	*/
	float voltage=0;
	/**
	Raw reads from the circuit
	*/
	int measures[BUFFER_SIZE];
	/**
	Needed to use the buffer. points to the last written record
	*/
	int bufferIndex =0;
	/**
	Physical pin on the development board used to sense the value
	*/
	const int pin = 36;
	/**
	Set during the first successful measure. Indicates that the values are meaningful
	*/
	bool ready=false;

	DelayData timer;

} CurrentMonitorTaskData;

CurrentMonitorTaskData CurrentMonitorRuntime;

#include "TaskCurrentMonitor-autogenerated.h"