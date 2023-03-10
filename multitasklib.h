#pragma once

#include <stdbool.h>

#define DEFAULT_MAX_SLEEP 50

typedef struct
{
	/**
	counter for the delay
	*/
	long remainingMillis;
	bool active;
	long lastUpdate;
} DelayData;

typedef struct
{
	/**
	 m stands for measurement
	*/
	long globalLoopBusyWaitingAvoider_m1;
	long globalLoopBusyWaitingAvoider_m2;
	int maxSleep=DEFAULT_MAX_SLEEP;

} globalLoopBusyWaitingAvoider_Data;


#include "multitasklib-autogenerated.h"