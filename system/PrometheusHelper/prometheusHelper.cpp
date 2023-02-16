#include "PrometheusHelper.h"


#include <stdbool.h>

#include <Arduino.h>
#include <WiFi.h>
#include <iomanip>
#include <sstream>
#include <map>
#include <AsyncTCP.h>

#include "../../generalpurposelib.h"


// this was taken from  https://github.com/ToMe25/ESP-WiFi-Thermometer
// and rearranged. the "core" (the on connect lambda) is -mostly-
// to be treated as a blackbox. this means that improvements are 
// definitely possible, but a bit hard to do more than what it is right now.



	/*
	An initialization step
	*/
	void generatePushURL(String pushJob, String pushInstance, String pushNamespace)
	{
		String stream;
		stream = stream + "/metrics/job/";
		stream += pushJob;
		stream += "/instance/";
		stream += pushInstance;
		stream += "/namespace/";
		stream += pushNamespace;
		push_url = stream;//.str();
	}

	/*
	returns a perfectly formatted metric string for use in any task that needs to
	push metrics. the type is gauge (prometheus type).
	*/
	String generateMetricStringGauge(String name, String description, /*String type,*/ float value)
	{
		String metrics("");
		metrics = metrics + METRIC_CRAFT_P1 + STRING_SPACE + name + STRING_SPACE + description + "\n";
		metrics = metrics + METRIC_CRAFT_P2 + STRING_SPACE + name + STRING_SPACE + METRIC_CRAFT_P3 + "\n";
		metrics = metrics + name + STRING_SPACE;

		metrics = metrics + value;

		metrics = metrics +"\n";
		
		return metrics;
	}

	/**
	Error handling. Internal use.
	*/
	bool preliminaryChecks()
	{
		if (!WiFi.isConnected())
		{
			return 0;
		}

		if (tcpClient) // Don't try pushing if tcpClient isn't NULL.
		{
			if (tcpClient->connected() || tcpClient->connecting())
			{
				tcpClient->close(true);
			}
			return 0;
		}

		return 1;
	}

	/*
	Part of the push process. Internal use.
	*/
	void writeMetricWithHttpRequest(AsyncClient* cli, String metrics, char* server)
	{
		cli->write(HTTP_REQUEST_CRAFT_P1_POST);
		cli->write(push_url.c_str());
		cli->write(HTTP_REQUEST_CRAFT_P2_HTTP);
		cli->write(server);
		cli->write(HTTP_REQUEST_CRAFT_P3_NEWLINE);
		cli->write(HTTP_REQUEST_CRAFT_P4_CONTENT_TYPE);
		cli->write(HTTP_REQUEST_CRAFT_P5_CONTENT_LENGTH);
		std::ostringstream converter;
		converter << metrics.length();
		cli->write(converter.str().c_str());
		cli->write(HTTP_REQUEST_CRAFT_P6_DOUBLE_NEWLINE);
		cli->write(metrics.c_str());
		cli->write(HTTP_REQUEST_CRAFT_P6_DOUBLE_NEWLINE);
	}
	/**
	Part of the push process. Internal use.
	*/
	bool initConnection()
	{
		tcpClient = new AsyncClient();

		if (!tcpClient)
		{
			Serial.println(TRACE_FAILED_TO_ALLOCATE);
			return 0;
		}
		tcpClient->setAckTimeout(PROMETHEUS_PUSH_INTERVAL * 0.75);
		tcpClient->setRxTimeout(PROMETHEUS_PUSH_INTERVAL * 0.75);

		return 1;
	}

	/**
	Error handling. Internal use.
	*/
	void handleNotConnected()
	{
		if (!tcpClient->connect(TASK_PROMETHEUS_SERVER, PROMETHEUS_PUSH_PORT))
		{
			Serial.println(TRACE_CONNECTING_FAILED);
			if (tcpClient != NULL) {
				AsyncClient* cli = tcpClient;
				tcpClient = NULL;
				delete cli;
			}
		}
	}
	
	/**
	Error handling. Internal use.
	*/
	void setupOnError()
	{
		tcpClient->onError([](void* arg, AsyncClient * cli, int error)
			{
				Serial.println(TRACE_CONNECTING_FAILED);
				Serial.print("Connection Error: ");
				Serial.println(error);
				delay(10);
				if (tcpClient != NULL)
				{
					tcpClient = NULL;
					delete cli;
				}
			}, NULL);
	}
	
	/**
	Error handling. Internal use.
	*/
	void setupOnDisconnect(AsyncClient* cli)
	{
		cli->onDisconnect([](void* arg, AsyncClient * c)
			{
				if (tcpClient != NULL) {
					tcpClient = NULL;
					Serial.println(TRACE_CONNECTING_FAILED_PG);
					delay(10);
					delete c;
				}
			}, NULL);
	}

	/*
	Prints some informations about the last push job. Internal use.
	*/
	void timerStuffPrint()
	{
		uint64_t now = millis();

		if (now - last_push >= (PROMETHEUS_PUSH_INTERVAL + 10) * 1000)
		{
			Serial.print(TRACE_TIME1);
			Serial.print(now - last_push);
			Serial.println("ms.");
			
		}

		last_push = now;
	}

	/*
	Prints some informations about the last push job. Internal use.
	*/
	void sthPrint(uint32_t code)
	{
		if (code == 200)
		{
			timerStuffPrint();
		}
		else
		{
			Serial.print(TRACE_TIME2);
			Serial.print(code);
			Serial.println(TRACE_TIME3);
		}
		delay(10);
	}


	/*
	Part of the push process. Internal use.
	*/
	void closeAndDisconnect(AsyncClient * c)
	{
		if (tcpClient != NULL)
		{
			tcpClient = NULL;
			if (c->connected())
			{
				c->close(true);
			}
			delete c;
		}
	}


	/*
	Initiates a task to push metrics to the server.
	Do not call again until finished.
	*/
	void pushMetrics(String metrics, char* server)
	{
		if (!preliminaryChecks())
		{
			Serial.println(ERRORS);
			delay(10);
			return;
		}

		if (!initConnection())
		{
			Serial.println(ERRORS);
			delay(10);
			return; 
		}

		setupOnError();

		tcpClient->onConnect([metrics, server](void* arg, AsyncClient * cli)
			{
				setupOnDisconnect(cli);
				std::shared_ptr<size_t> read = std::make_shared<size_t>(0);
				cli->onData([read](void* arg, AsyncClient * c, void* data, size_t len) mutable
					{
						uint8_t* d = (uint8_t*)data;
						for (size_t i = 0; i < len; i++)
						{
							if (*read > 8 && isDigit(d[i]) && len > i + 2)
							{
								char status_code[4]{ 0 };
								for (uint8_t j = 0; j < 3; j++)
								{
									status_code[j] = d[i + j];
								}
								uint32_t code = atoi(status_code);
								sthPrint(code);
								closeAndDisconnect(c);
								return;
							}
							(*read)++;
						}
					}, NULL);

				Serial.println(TRACE_PUSHING);
				writeMetricWithHttpRequest(cli, metrics, server);

			}, NULL);

		handleNotConnected();
	}

