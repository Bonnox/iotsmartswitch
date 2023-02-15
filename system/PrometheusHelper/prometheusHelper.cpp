#include "PrometheusHelper.h"


#include <stdbool.h>

#include <Arduino.h>
#include <WiFi.h>
#include <iomanip>
#include <sstream>
#include <map>
#include <AsyncTCP.h>

// this was taken from  https://github.com/ToMe25/ESP-WiFi-Thermometer
// and rearranged. the "core" (the on connect lambda) is -mostly-
// to be treated as a blackbox. this means that improvements are 
// definitely possible, but a bit hard


//namespace prom
//{

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

	String generateMetricStringGauge(String name, String description, /*String type,*/ float value)
	{
		String metrics("");
		metrics = metrics + "# HELP" + " " + name + " " + description + "\n";
		metrics = metrics + "# TYPE " + name + " gauge" + "\n";
		metrics = metrics + name + " ";

		metrics = metrics + value;

		metrics = metrics +"\n";
		
		return metrics;
	}


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

	void writeMetric(AsyncClient* cli, String metrics)
	{
		cli->write("POST ");
		cli->write(push_url.c_str());
		cli->write(" HTTP/1.0\r\nHost: ");
		cli->write(TASK_PROMETHEUS_SERVER);
		cli->write("\r\n");
		cli->write("Content-Type: application/x-www-form-urlencoded\r\n");
		cli->write("Content-Length: ");
		std::ostringstream converter;
		converter << metrics.length();
		cli->write(converter.str().c_str());
		cli->write("\r\n\r\n");
		cli->write(metrics.c_str());
		cli->write("\r\n\r\n");
	}
	bool initConnection()
	{
		tcpClient = new AsyncClient();

		if (!tcpClient)
		{
			Serial.println("Failed to allocate Async TCP Client!");
			return 0;
		}
		tcpClient->setAckTimeout(PROMETHEUS_PUSH_INTERVAL * 0.75);
		tcpClient->setRxTimeout(PROMETHEUS_PUSH_INTERVAL * 0.75);

		return 1;
	}

	void handleNotConnected()
	{
		if (!tcpClient->connect(TASK_PROMETHEUS_SERVER, PROMETHEUS_PUSH_PORT))
		{
			Serial.println("Connecting to the metrics server failed!");
			if (tcpClient != NULL) {
				AsyncClient* cli = tcpClient;
				tcpClient = NULL;
				delete cli;
			}
		}
	}

	void setupOnError()
	{
		tcpClient->onError([](void* arg, AsyncClient * cli, int error)
			{
				Serial.println("Connecting to the metrics server failed!");
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

	void setupOnDisconnect(AsyncClient* cli)
	{
		cli->onDisconnect([](void* arg, AsyncClient * c)
			{
				if (tcpClient != NULL) {
					tcpClient = NULL;
					Serial.println("Connection to prometheus pushgateway server was closed while reading or writing.");
					delay(10);
					delete c;
				}
			}, NULL);
	}

	void timerStuffPrint()
	{
		uint64_t now = millis();

		if (now - last_push >= (PROMETHEUS_PUSH_INTERVAL + 10) * 1000)
		{
			Serial.print("Successfully pushed again after ");
			Serial.print(now - last_push);
			Serial.println("ms.");
			
		}

		last_push = now;
	}


	void sthPrint(uint32_t code)
	{
		if (code == 200)
		{
			timerStuffPrint();
		}
		else
		{
			Serial.print("Received http status code ");
			Serial.print(code);
			Serial.println(" when trying to push metrics.");
		}
		delay(10);
	}

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

	void pushMetrics(String metrics)
	{
		if (!preliminaryChecks())
		{
			Serial.println("checks failed");
			delay(10);
			return;
		}

		if (!initConnection())
		{
			Serial.println("checks failed 2");
			delay(10);
			return; 
		}

		setupOnError();

		tcpClient->onConnect([metrics](void* arg, AsyncClient * cli)
			{
				Serial.println("onconnect");
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

				Serial.println("pushing metrics now");
				writeMetric(cli, metrics);

			}, NULL);

		handleNotConnected();
	}




//}