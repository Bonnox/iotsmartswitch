#include "multitasklib.h"

#include <Arduino.h>
#include <stdbool.h>


/*
TODO better multitasking lib, read below

i'm sure that we can reduce these 3 lines to 1

from:

CODE
-----------------------------------------------
	if (finished(&(struct.timer)))			(1)
	{
		callBack();
		startDelay(999, &(struct.timer));	(2)
	}
	updateTimer(&(struct.timer));			(3)
-----------------------------------------------

to:

CODE
-----------------------------------------------
	if (waitTimerFinished(999, &struct))
-----------------------------------------------

name is temporary and subject to change.
the function should contain everything


 or use cooperativemultitasking.h

*/

/**
updates a timer, should be called every cycle
*/
void updateTimer(DelayData* waitTask)
{
	long now = millis();

	

	//Serial.printf("update timer. remaining %i\tlastupd %i \n", waitTask->remainingMillis, waitTask->lastUpdate);

	if (waitTask->active)
	{
		long delta = now - waitTask->lastUpdate;
		waitTask->remainingMillis -= delta;
	}
	if (finished(waitTask))
	{
		waitTask->active = 0;
	}

	waitTask->lastUpdate = now;
}

/**
starts a timer
*/
void startDelay(int millisTarget, DelayData* data)
{
	data->active = 1;
	data->remainingMillis = millisTarget;
	data->lastUpdate = millis();
}

/**
if the timer has passed
*/
bool finished(DelayData* query)
{
	int remain = query->remainingMillis;

	if (remain <= 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}
/**
this and the other function with the same name ending with 2 need to be called respectively 
at the beggining and at the end of a block where there's the need to avoid busy waiting
*/
void globalLoopBusyWaitingAvoider_p1(globalLoopBusyWaitingAvoider_Data* data )
{
	data->globalLoopBusyWaitingAvoider_m1= millis();
}

void globalLoopBusyWaitingAvoider_p2(globalLoopBusyWaitingAvoider_Data* data)
{
	data->globalLoopBusyWaitingAvoider_m2= millis();

	long thisCycleLenght = data->globalLoopBusyWaitingAvoider_m2 - data->globalLoopBusyWaitingAvoider_m1;

	long sleepTime = data->maxSleep - thisCycleLenght;
	if (sleepTime > 0)
	{
		delay(sleepTime);
	}
	
}