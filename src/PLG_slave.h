
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
void PLG_execmd()
{
    int idx1 = receivedData.indexOf('|');
    int idx2 = receivedData.indexOf('|', idx1 + 1);

    String slave = receivedData.substring(0, idx1);
    String name = receivedData.substring(idx1 + 1, idx2);
    String status = receivedData.substring(idx2 + 1);

    // DEBUG_PRINTLN("Slave: " + slave);
    // DEBUG_PRINTLN("Name: " + name);
    // DEBUG_PRINTLN("Status: " + status);
    digitalWrite(led_master, HIGH); // Turn on LED for slave status
    delay(15);                      // Delay to ensure the message is sent
    digitalWrite(led_master, LOW);  // Turn off LED for slave status
    if (slave.startsWith("slave1"))
    {
        if (name.startsWith("bom"))
        {
            if (status == "ok")
            {
                DEBUG_PRINTLN("bom_run");
            }
            else if (status == "not ok")
            {
                DEBUG_PRINTLN("bom_not_run");
            }
        }
        else if (name.startsWith("catnang"))
        {
            if (status == "ok")
            {
                DEBUG_PRINTLN("catnang_run");
            }
            else if (status == "not ok")
            {
                DEBUG_PRINTLN("catnang_not_run");
            }
        }
        else if (name.startsWith("fanhut"))
        {
            if (status == "ok")
            {
                DEBUG_PRINTLN("fanhut_run");
            }
            else if (status == "not ok")
            {
                DEBUG_PRINTLN("fanhut_not_run");
            }
        }
        else if (name.startsWith("dinhduong1"))
        {
            if (status < "12")
            {
                DEBUG_PRINTLN("the tich bom: " + status);
                digitalWrite(led_connected, HIGH); // Turn on LED for connection status
            }
            else if (status >= "12")
            {
                DEBUG_PRINTLN("the tich bom: " + status);
                digitalWrite(led_connected, LOW); // Turn off LED for connection status
            }
        }
    }
    else
    {
        DEBUG_PRINTLN("Unknown slave");
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
        PLG_execmd();
    }
}
void thucthilenh()
{
    if (address_slave.startsWith("slave1"))
    {
        if (namedata.startsWith("bom"))
        {
            if (data.startsWith("ok"))
            {
                digitalWrite(led_slave, HIGH); // Turn on LED for slave status
            }
            else if (data.startsWith("not ok"))
            {
                digitalWrite(led_slave, LOW); // Turn off LED for slave status
            }
        }
    }
}
void PLG_slave_rs485_loop()
{
    // PLG_write_4("master", "slave1", "bom", "ok");
    // Serial2.println(messages4);
    // delay(1000); // Delay to ensure the message is sent
    // PLG_write_5("slave1", "bom", "remus", "ok", "12");
    // Serial2.println(messages5);
    // delay(1000);
    // PLG_write_4("master", "slave1", "bom", "not ok");
    // Serial2.println(messages4);
    // delay(1000); // Delay to ensure the message is sent

    
    if (Serial2.available())
    {
        receivedData = Serial2.readStringUntil('\n'); // Ví dụ nhận chuỗi
        PLG_check_message();                          // Gọi xử lý chuỗi
        thucthilenh(); // Execute the command
    }
    
}
