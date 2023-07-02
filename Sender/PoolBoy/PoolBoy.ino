// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

/*
 * This is an Arduino-based Azure IoT Hub sample for ESPRESSIF ESP8266 board.
 * It uses our Azure Embedded SDK for C to help interact with Azure IoT.
 * For reference, please visit https://github.com/azure/azure-sdk-for-c.
 *
 * To connect and work with Azure IoT Hub you need an MQTT client, connecting, subscribing
 * and publishing to specific topics to use the messaging features of the hub.
 * Our azure-sdk-for-c is an MQTT client support library, helping to compose and parse the
 * MQTT topic names and messages exchanged with the Azure IoT Hub.
 *
 * This sample performs the following tasks:
 * - Synchronize the device clock with a NTP server;
 * - Initialize our "az_iot_hub_client" (struct for data, part of our azure-sdk-for-c);
 * - Initialize the MQTT client (here we use Nick Oleary's PubSubClient, which also handle the tcp
 * connection and TLS);
 * - Connect the MQTT client (using server-certificate validation, SAS-tokens for client
 * authentication);
 * - Periodically send telemetry data to the Azure IoT Hub.
 *
 * To properly connect to your Azure IoT Hub, please fill the information in the `iot_configs.h`
 * file.
 */

// C99 libraries
#include <cstdlib>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Libraries for MQTT client, WiFi connection and SAS-token generation.
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <base64.h>
#include <bearssl/bearssl.h>
#include <bearssl/bearssl_hmac.h>
#include <libb64/cdecode.h>

// Azure IoT SDK for C includes
#include <az_core.h>
#include <az_iot.h>
#include <azure_ca.h>

// Additional sample headers
#include "iot_configs.h"
#include "WiFi_Base.h"
#include "TimeFunctions.h"
#include "PoolBoy_MQTT.h"
#include "PoolBoy_Telemetry.h"
#include "PoolBoy_Initialize.h"




static unsigned long next_telemetry_send_time_ms = 0;
static char telemetry_topic[128];
int tempPin = D1;
OneWire oneWire(tempPin);         // setup a oneWire instance 
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library



/*
 * @brief           Gets the number of seconds since UNIX epoch until now.
 * @return uint32_t Number of seconds.
 */









static void sendTelemetry()
{
 float tempCelsius;    // temperature in Celsius
tempSensor.requestTemperatures();             // send the command to get temperatures
  tempCelsius = tempSensor.getTempCByIndex(0);  // read temperature in Celsius 

  digitalWrite(LED_PIN, HIGH);
  Serial.print(millis());
  Serial.print(" ESP8266 Sending telemetry . . . ");
  if (az_result_failed(az_iot_hub_client_telemetry_get_publish_topic(
          &client, NULL, telemetry_topic, sizeof(telemetry_topic), NULL)))
  {
    Serial.println("Failed az_iot_hub_client_telemetry_get_publish_topic");
    return;
  }
Serial.println(getTelemetryPayload(tempCelsius));
  mqtt_client.publish(telemetry_topic, getTelemetryPayload(tempCelsius), false);
  Serial.println("OK");
  delay(100);
  digitalWrite(LED_PIN, LOW);
}

// Arduino setup and loop main functions.

void setup()
{
  tempSensor.begin();    // initialize the sensor
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  establishConnection();
}

void loop()
{
  if (millis() > next_telemetry_send_time_ms)
  {
    // Check if connected, reconnect if needed.
    if (!mqtt_client.connected())
    {
      establishConnection();
    }

    sendTelemetry();
    next_telemetry_send_time_ms = millis() + TELEMETRY_FREQUENCY_MILLISECS;
  }

  // MQTT loop must be called to process Device-to-Cloud and Cloud-to-Device.
  mqtt_client.loop();
  delay(500);
}
