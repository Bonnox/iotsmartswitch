#include "TaskCurrentMonitor.h"
#include "../../generalpurposelib.h"
#include "../../system/HAL/HAL.h"
#include "../../system/HAL/board.h"


void TaskCurrentMonitor_setup(HALdata* machine)
{

	HAL_setupPin(CurrentMonitorRuntime.pin, INPUT, machine);
	
	// initialize structr
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		CurrentMonitorRuntime.measures[i] = 0;
	}

	CurrentMonitorRuntime.bufferIndex = 0;
	CurrentMonitorRuntime.voltage = 0;
	CurrentMonitorRuntime.ready = 0;

	startDelay(TASK_CURRENTMONITOR_WAIT, &(CurrentMonitorRuntime.timer));

}

void handleReadyness()
{
	if (!CurrentMonitorRuntime.ready)
	{
		CurrentMonitorRuntime.ready = 1;
	}
}

void calculateAverage()
{
	int sum = 0;
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		sum += CurrentMonitorRuntime.measures[i];
	}

	int average = sum / BUFFER_SIZE;

	float percent = ((float)average) / ((float)ADC_SAMPLES); 
	float voltage = percent * LOGIC_LEVEL_VOLTAGE;
	
	CurrentMonitorRuntime.voltage = voltage;

	Serial.println("voltage avg: " + (String)(CurrentMonitorRuntime.voltage));
	//Serial.println("voltage sum: " + (String)(sum));
}


void TaskCurrentMonitor_callBack(HALdata* machine)
{

	handleReadyness();
	
	// i found that splitting this simple function up too much 
	// actually hurt readability because your brain needs to jump
	// among pieces of the same concept and reconstruct them, while
	// presenting it in one go it was more fluid


	//handleBufferIndexResetAndCalculateAverage

	if (out_of_bounds_array(CurrentMonitorRuntime.bufferIndex, BUFFER_SIZE))
	{
		CurrentMonitorRuntime.bufferIndex = 0;
		calculateAverage();
	}

	//handleMeasureAndStore
	int read = HAL_read(false, CurrentMonitorRuntime.pin, machine);
	CurrentMonitorRuntime.measures[CurrentMonitorRuntime.bufferIndex] = read;
	CurrentMonitorRuntime.bufferIndex++;
}

void TaskCurrentMonitor_loop(HALdata* machine, char **metric)
{

		TaskCurrentMonitor_callBack(machine);
		generateMetricStringGauge("current", "measure of current", CurrentMonitorRuntime.voltage).toCharArray(*metric, STRING_METRIC_SIZE);

} 