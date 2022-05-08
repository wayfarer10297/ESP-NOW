/*===================================================================================
Filename:	ESP322now.ino
Version :	Draft A
Date	: 	8-May-2022
Author	:	Roger Thompson
====================================================================================*/

/*************************************************************************************
DESCRIPTION:

This is a sketch to test inter-ESP32 communication using ESP32-NOW.

The code is largely drawan from the Dronebot YouTube tutorial on ESP32-NOW.

*************************************************************************************/

#include <esp_now.h>
#include "WiFi.h"


// ESP32 MAC address
#define MAC_DATA_LOGGER		58:BF:25:83:1D:F0
#define MAC_AUDIONAV3		7C:9E:BD:39:E1:54


// MAC Address of the receiving ESP32 (the Data Logger)
uint8_t broadcastAddress[] = {0x58, 0xBF, 0x25, 0x83, 0x1D, 0xF0};


// Data structure to be sent to the logger (max 256 bytes)
struct Log_record{
	char prefix[16];
	long n;
	float f;
	char suffix[16];
} logRecord;

// Peer info
esp_now_peer_info_t peerInfo;


// Callback function called when data is SENT
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


// Callback function executed when data is RECEIVED
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&logRecord, incomingData, sizeof(logRecord));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print(logRecord.prefix);
  Serial.println(logRecord.n);
  Serial.print("Float Value: ");
  Serial.println(logRecord.f);
  Serial.println(logRecord.suffix);
  Serial.println();
}



void setup() {
  Serial.begin(115200);
  Serial.println("\n\nESP32-NOW test sketch begins...\n");

  WiFi.mode(WIFI_MODE_STA);	// Put ESP32 into Station mode

  // Print MAC Address to Serial monitor
  Serial.print("MAC Address of this ESP32 is: ");
  Serial.println(WiFi.macAddress());

  // Initialise ESP-NOW
   if (esp_now_init() != ESP_OK) {
     Serial.println("Error initialising ESP-NOW");
     return;
   }

   // Register the SEND callback
   esp_now_register_send_cb(OnDataSent);

   // Register RECEIVE callback function
     esp_now_register_recv_cb(OnDataRecv);

   // Register peer
   memcpy(peerInfo.peer_addr, broadcastAddress, 6);
   peerInfo.channel = 0;
   peerInfo.encrypt = false;

   // Add peer
   if (esp_now_add_peer(&peerInfo) != ESP_OK){
     Serial.println("Failed to add peer");
     return;
   }
Serial.println("End of Setup\n\n");
}

void loop() {

	/*// Code for SENDER ESP32>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	Serial.println("START OF LOOP1");
	static long int_value;
	static float float_value;
	Serial.println("START OF LOOP2");


	int_value = millis();
	float_value = 1.3 * int_value;// Use integer to make a float

	Serial.println("START populate");
	// Populate the record to be sent
	strcpy(logRecord.prefix, "Time: ");
	logRecord.n = int_value;
	logRecord.f = float_value;
	strcpy(logRecord.suffix, "End record\n");

	Serial.print("Call to esp-now-send");

	// Send message via ESP-NOW
	esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &logRecord, sizeof(logRecord));

	// After call to esp-now-send");

	if (result == ESP_OK) {
		Serial.println("Sending confirmed");
	}
	else {
		Serial.println("Sending error");
	}
	delay(2000);
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

	// Code for RECEIVER ESP32
	// = Nothing!   It's all done by the receive Callback function

}

