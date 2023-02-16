#include "TaskFailureDetector.h"

#include "../TaskCurrentMonitor/TaskCurrentMonitor.h"

#include "../TaskRelay/TaskRelay.h"

#include "../../multitasklib.h"


#include "../../system/PrometheusHelper/PrometheusHelper.h"

#include <stdbool.h>

void TaskFailureDetector_setup()
{
	startDelay(FAILURE_MULTITASK_TIMER, &(FailureDetectorRuntime.timer));
}




void TaskFailureDetector_loop(char** metric)
{
	
	if (finished(&(FailureDetectorRuntime.timer)))
	{	
		
		if (CurrentMonitorRuntime.ready && relaisRuntime.relayState) // no false positives
		{
			FailureDetectorRuntime.copyMeasure = CurrentMonitorRuntime.voltage;

			//Serial.print("verity " + (String)FailureDetectorRuntime.copyMeasure);
			//	Serial.println(" " + (String)(FailureDetectorRuntime.copyMeasure < FAILURE_ALERT_THRESHOLD));

			if (FailureDetectorRuntime.copyMeasure < FAILURE_ALERT_THRESHOLD)
			{
				FailureDetectorRuntime.failure = true;
			}
			else
			{
				FailureDetectorRuntime.failure = false;
			}
		}
		generateMetricStringGauge(FAILUREDETECTOR_METRIC_NAME_FAIL, FAILUREDETECTOR_METRIC_DESC_FAIL, (FailureDetectorRuntime.failure)).toCharArray(*metric, STRING_METRIC_SIZE);
		startDelay(FAILURE_MULTITASK_TIMER, &(FailureDetectorRuntime.timer));
	}

	updateTimer(&(FailureDetectorRuntime.timer));
}