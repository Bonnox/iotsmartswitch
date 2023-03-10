#pragma once


#include <stdbool.h>

#include <Arduino.h>
#include <WiFi.h>
#include <iomanip>
#include <sstream>
#include <map>
#include <AsyncTCP.h>


uint32_t used_heap;
uint64_t last_push = 0;

AsyncClient* tcpClient = NULL;
String push_url;

#define ERRORS "safety checks failed"

#define METRIC_CRAFT_P1 "# HELP"
#define METRIC_CRAFT_P2 "# TYPE"
#define METRIC_CRAFT_P3 "gauge"


#define HTTP_REQUEST_CRAFT_P1_POST "POST "
#define HTTP_REQUEST_CRAFT_P2_HTTP " HTTP/1.0\r\nHost: "
#define HTTP_REQUEST_CRAFT_P3_NEWLINE "\r\n"
#define HTTP_REQUEST_CRAFT_P4_CONTENT_TYPE "Content-Type: application/x-www-form-urlencoded\r\n"
#define HTTP_REQUEST_CRAFT_P5_CONTENT_LENGTH "Content-Length: "
#define HTTP_REQUEST_CRAFT_P6_DOUBLE_NEWLINE "\r\n\r\n"

#define TRACE_PUSHING "pushing metrics now"
#define TRACE_FAILED_TO_ALLOCATE "Failed to allocate Async TCP Client!"
#define TRACE_CONNECTING_FAILED "Connecting to the metrics server failed!"
#define TRACE_CONNECTING_FAILED_PG "Connection to prometheus pushgateway server was closed while reading or writing."
#define TRACE_TIME1 "Successfully pushed again after "
#define TRACE_TIME2 "Received http status code "

#include "PrometheusHelper-autogenerated.h"
