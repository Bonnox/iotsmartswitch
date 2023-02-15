#pragma once



#define TASK_PROMETHEUS_WAIT 40*1000


// The address of the prometheus pushgateway to push the data to.
// Can be an IP address, a hostname, or a domain name.
static constexpr char TASK_PROMETHEUS_SERVER[] = "192.168.0.110";
// The port of the prometheus pushgateway.
// The default prometheus pushgateway port is 9091.
static const uint16_t PROMETHEUS_PUSH_PORT = 9091;
// The time between two HTTP post requests sent to the pushgateway.
// Specified in seconds.
// Ignored in deep sleep mode.
// Prometheus default scrape interval is every 30 seconds.
static const uint16_t PROMETHEUS_PUSH_INTERVAL = 20;

#define PROM_NAME_JOB "myjob"
#define PROM_NAME_INSTANCE "myinstance"
#define PROM_NAME_NAMESPACE "mynamespace"


#define PROM_METRIC_NAME_ALIVE "alive_seconds"
#define PROM_METRIC_DESC_ALIVE "used to determine if the arduino reboots after a crash during tests"