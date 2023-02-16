#include "TaskPrometheus.h"
#include "prometheusCfg.h"

#include "../TaskCurrentMonitor/TaskCurrentMonitor.h"
#include "../TaskFailureDetector/TaskFailureDetector.h"

#include "../../system/PrometheusHelper/PrometheusHelper.h"

void TaskPrometheus_Setup()
{
	generatePushURL(PROM_NAME_JOB, PROM_NAME_INSTANCE, PROM_NAME_NAMESPACE);
}
/*
an optimistic check about whether a string is a metric.
not syntactically perfect.
*/
bool isStringAlmostMetric(String str)
{
	bool starts = str.startsWith("#");
	starts = starts || (str.charAt(0) > 'a' && str.charAt(0) < 'z');
	starts = starts || (str.charAt(0) > 'A' && str.charAt(0) < 'Z');
	return starts;
}

//void TaskPrometheus_callBack(int size, String metrics, ...)
void TaskPrometheus_callBack(int size, String metricsToPush)
{	
	// we add the lifetime of the process for test purposes
	String alive = generateMetricStringGauge(PROM_METRIC_NAME_ALIVE, PROM_METRIC_DESC_ALIVE, (millis() / 1000));
	metricsToPush += alive;
	
	Serial.println("metrics to be pushed:");
	Serial.println(metricsToPush);
	
	pushMetrics(metricsToPush,(char*) TASK_PROMETHEUS_SERVER);
}


void TaskPrometheus_Loop(int size, String metricsToPush)
{	
	if (finished(&(PrometheusRuntime.timer)))
	{
		TaskPrometheus_callBack(size, metricsToPush);
		startDelay(TASK_PROMETHEUS_WAIT, &(PrometheusRuntime.timer));
	}
	updateTimer(&(PrometheusRuntime.timer));
}