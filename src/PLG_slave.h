
#include <PLG_datastring.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <string.h>
#include <hardwareSerial.h>

// define the pins used by the transceiver module
#define ss 05            // Slave Select pin
#define rst 04           // Reset pin
#define dio0 14          // DIO0 pin
                         // mosi, miso, sck are defined in the SPI library
                         // for ESP32, the default pins are: mosi=23, miso=19, sck=18
#define led_connected 26 // LED pin for connection status
#define led_slave 33     // LED pin for master status
#define led_master 25    // LED pin for slave status
#define TX 17            // TX pin for RS485 module
#define RX 16            // RX pin for RS485 module

// String receivedData = "";
String cmd = "";

void Setup_slave()
{

    Serial.begin(115200);
    DEBUG_PRINTLN("LoRa Sender");

    // setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);

    // 433E6 for Asia
    while (!LoRa.begin(433E6))
    {
        Serial.print(".");
        delay(500);
    }
    DEBUG_PRINTLN("LoRa Initialized");
    pinMode(led_slave, OUTPUT);       // Set LED pin as output
    pinMode(led_master, OUTPUT);      // Set LED pin for master status
    pinMode(led_connected, OUTPUT);   // Set LED pin for connection status
    digitalWrite(led_connected, LOW); // Turn off LED for slave status
    digitalWrite(led_slave, HIGH);    // Turn on LED for slave status
    digitalWrite(led_master, LOW);    // Turn off LED for master status
    DEBUG_PRINTLN("PLG RUNNING SLAVE");
    // Initialize RS485 Modbus Master
    Serial2.begin(115200, SERIAL_8N1, RX, TX); // Initialize Serial2 for RS485 communication
}

void thucthilenh()
{
    if (address_slave.startsWith("slave1") && namedata.startsWith("bom"))
    {

        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("bom ON");
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("bom OFF");
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("quat"))
    {
        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("quat ON");
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("quat OFF");
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("fanhut"))
    {
        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("quat hut ON");
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("quat hut OFF");
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("catnag"))
    {
        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("cat nang ON");
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("cat nang OFF");
        }
    }
    else
    {
        DEBUG_PRINTLN("Khong co lenh nao duoc thuc hien");
    }
}
void PLG_slave_loop()
{

    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        // Received a packet
        DEBUG_PRINT("Received packet of size: ");
        DEBUG_PRINTLN(packetSize);

        // Read the packet
        receivedData = "";
        while (LoRa.available())
        {
            receivedData += (char)LoRa.read();
        }
        PLG_check_message(); // Check the received data
        thucthilenh();       // Execute the command
    }
}
