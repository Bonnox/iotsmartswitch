#include "Wifi.h"
/**
	Wrapper around the WIFI library to connect to a wireless network. Leave the password field empty if the WLAN does not have a password set
*/
void connectToWLANSilent(char* ssid, char* password)
{

	if (strlen(password) == 0)
	{
		WiFi.begin(ssid);
	}
	else
	{
		WiFi.begin(ssid, password);
	}
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1 * SECOND_MILLIS);
	}
}
/*
connects to wifi and prints a message when done
*/
void connectToWLANwithSerialMessage(char* ssid, char* password)
{
	Serial.print("Connecting to WIFI...\n");
	connectToWLANSilent(ssid, password);
	Serial.println("WiFi connected");
}


void Wifi_setup(char* ssid, char* password)
{
	delay(WIFI_SAFE_DELAY);
	connectToWLANwithSerialMessage(ssid, password);
}