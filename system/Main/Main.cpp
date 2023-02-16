#include "Main.h"
#include "../../multitasklib.h"

#include "../MQTTManager/MQTTManager.h"


#include "../../Tasks/TaskRelay/TaskRelay.h"
#include "../../Tasks/TaskCurrentMonitor/TaskCurrentMonitor.h"
#include "../../Tasks/TaskFailureDetector/TaskFailureDetector.h"



#include "../../Tasks/Wifi/Wifi.h"
#include "../../Tasks/TaskPrometheus/TaskPrometheus.h"

#include"../PrometheusHelper/PrometheusHelper.h"


void Main_loop()
{

	HAL_cycleBegin(&machine);
	TaskMqtt_loop();

	globalLoopBusyWaitingAvoider_Data loopMeasure;
	globalLoopBusyWaitingAvoider_p1(&loopMeasure);	

	TaskCurrentMonitor_loop(&machine , &metric1);
	TaskFailureDetector_loop(&metric2);
	TaskRelay_loop(&machine, &MQTTRuntime);
	TaskPrometheus_Loop(1, compactMetrics(1, metric1, metric2, "a"));
	

	delay(50);
	globalLoopBusyWaitingAvoider_p2(&loopMeasure);
	HAL_cycleEnd(&machine);	
}
String compactMetrics(int size, String metric1, String metric2, String metric3)
{
	String ret = "";
	/*
	for (int i = 0; i < size; i++)
	{
		isStringAlmostMetric()
		//if (metrics[i].length()>0)
		if (starts)
		{
			Serial.print("metric " + i);
			Serial.println(metrics[i]);
			ret += metrics[i];
			ret += "\n";
		}
	}*/
	if (isStringAlmostMetric(metric1))
	{
		ret += metric1;
	}
	if (isStringAlmostMetric(metric2))
	{
		ret += metric2;
	}
	if (isStringAlmostMetric(metric3))
	{
		ret += metric3;
	}
	return ret;
}

void setupStrings()
{
	//metric1.reserve(sizeof(generateMetricStringGauge("a", "b", 500))+200);
	generateMetricStringGauge("a", "b", 0).toCharArray(metric1,STRING_METRIC_SIZE);
	generateMetricStringGauge("c", "d", 0).toCharArray(metric2,STRING_METRIC_SIZE);
}

void Main_setup()
{
	setupStrings();
	delay(6 * SECOND_MILLIS);
	Serial.begin(SERIAL_SPEED_RECOMMENDED);
	Serial.println("boot");
	Wifi_setup(WIFI_SSID, WIFI_PASSWORD);
	
	TaskMqtt_setup();
	TaskPrometheus_Setup();

	// tasks init
	TaskRelay_setup(&machine);
	TaskCurrentMonitor_setup(&machine);
	TaskFailureDetector_setup();
	
	//delay(2 * SECOND_MILLIS);
	Serial.println("setup completed\n\n\n\n");
}