
/**************************************************************
 *                PLG END CONFIG_WIFI_SENDER
 ***************************************************************/
#include <PLG_debug.hpp>
#include <PLG_set_era.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <PLG_datastring.h>

// define the pins used by the transceiver module
#define ss 05            // Slave Select pin
#define rst 04           // Reset pin
#define dio0 14          // DIO0 pin
                         // mosi, miso, sck are defined in the SPI library
                         // for ESP32, the default pins are: mosi=23, miso=19, sck=18
#define led_connected 26 // LED pin for connection status
#define led_master 25    // LED pin for master status
#define led_slave 33     // LED pin for slave status
#define TX 17            // TX pin for RS485 module
#define RX 16            // RX pin for RS485 module

int value = 0;
// void PLG_data(string address_slave, string tendulieu, string dulieu)
// {
//     mes = address_slave + "|" + tendulieu + "|" + dulieu;
//     DEBUG_PRINT("Sending packet: ");
//     DEBUG_PRINTLN(mes);
//     LoRa.beginPacket(); // Start a new packet
//     LoRa.print(mes);
//     LoRa.endPacket();              // Finish the packet and send it
//     digitalWrite(led_slave, HIGH); // Turn off LED for slave status
//     delay(20);                     // Delay to ensure the message is sent
//     digitalWrite(led_slave, LOW);  // Turn off LED for slave status
// }
// ERA_WRITE(V0)
// {
//     value = param.getInt();
//     if (value == HIGH)
//     {

//         // ERa.virtualWrite(V0, value = 0);
//         address_slave = "slave1";
//         tendulieu = "bom";
//         dulieu = "ok";
//         mes = address_slave + "|" + tendulieu + "|" + dulieu;
//         DEBUG_PRINT("Sending packet: ");
//         DEBUG_PRINTLN(mes);
//         LoRa.beginPacket(); // Start a new packet
//         LoRa.print(mes);
//         // LoRa.write(1); // Write a byte to the packet
//         LoRa.endPacket();              // Finish the packet and send it
//         digitalWrite(led_slave, HIGH); // Turn off LED for slave status
//         delay(20);                     // Delay to ensure the message is sent
//         digitalWrite(led_slave, LOW);  // Turn off LED for slave status
//     }
//     else
//     {

//         // ERa.virtualWrite(V0, value = 1);
//         address_slave = "slave1";
//         tendulieu = "bom";
//         dulieu = "not ok";
//         mes = address_slave + "|" + tendulieu + "|" + dulieu;
//         DEBUG_PRINT("Sending packet: ");
//         DEBUG_PRINTLN(mes);
//         LoRa.beginPacket(); // Start a new packet
//         LoRa.print(mes);
//         LoRa.endPacket();              // Finish the packet and send it
//         digitalWrite(led_slave, HIGH); // Turn on LED for slave status
//         delay(20);                     // Delay to ensure the message is sent
//         digitalWrite(led_slave, LOW);  // Turn off LED for slave status
//     }
// }
// ERA_WRITE(V5)
// {
//     value = param.getInt();
//     if (value == HIGH)
//     {
//         // ERa.virtualWrite(V1, value = 0);
//         address_slave = "slave1";
//         tendulieu = "fandao";
//         dulieu = "ok";
//         mes = address_slave + "|" + tendulieu + "|" + dulieu;
//         DEBUG_PRINT("Sending packet: ");
//         DEBUG_PRINTLN(mes);
//         LoRa.beginPacket(); // Start a new packet
//         LoRa.print(mes);
//         LoRa.endPacket();              // Finish the packet and send it
//         digitalWrite(led_slave, HIGH); // Turn off LED for slave status
//         delay(20);                     // Delay to ensure the message is sent
//         digitalWrite(led_slave, LOW);  // Turn off LED for slave status
//     }
//     else
//     {
//         // ERa.virtualWrite(V1, value = 1);
//         address_slave = "slave1";
//         tendulieu = "fandao";
//         dulieu = "not ok";
//         mes = address_slave + "|" + tendulieu + "|" + dulieu;
//         DEBUG_PRINT("Sending packet: ");
//         DEBUG_PRINTLN(mes);
//         LoRa.beginPacket(); // Start a new packet
//         LoRa.print(mes);
//         LoRa.endPacket();              // Finish the packet and send it
//         digitalWrite(led_slave, HIGH); // Turn on LED for slave status
//         delay(20);                     // Delay to ensure the message is sent
//         digitalWrite(led_slave, LOW);  // Turn off LED for slave status
//     }
// }
// ERA_WRITE(V1)
// {
//     value = param.getInt();
//     if (value == HIGH)
//     {
//         // ERa.virtualWrite(V2, value = 0);
//         address_slave = "slave1";
//         tendulieu = "fanhut";
//         dulieu = "ok";
//         mes = address_slave + "|" + tendulieu + "|" + dulieu;
//         DEBUG_PRINT("Sending packet: ");
//         DEBUG_PRINTLN(mes);
//         LoRa.beginPacket(); // Start a new packet
//         LoRa.print(mes);
//         LoRa.endPacket();              // Finish the packet and send it
//         digitalWrite(led_slave, HIGH); // Turn off LED for slave status
//         delay(20);                     // Delay to ensure the message is sent
//         digitalWrite(led_slave, LOW);  // Turn off LED for slave status
//     }
//     else
//     {
//         // ERa.virtualWrite(V2, value = 1);
//         address_slave = "slave1";
//         tendulieu = "fanhut";
//         dulieu = "not ok";
//         mes = address_slave + "|" + tendulieu + "|" + dulieu;
//         DEBUG_PRINT("Sending packet: ");
//         DEBUG_PRINTLN(mes);
//         LoRa.beginPacket(); // Start a new packet
//         LoRa.print(mes);
//         LoRa.endPacket();              // Finish the packet and send it
//         digitalWrite(led_slave, HIGH); // Turn on LED for slave status
//         delay(20);                     // Delay to ensure the message is sent
//         digitalWrite(led_slave, LOW);  // Turn off LED for slave status
//     }
// }
// ERA_WRITE(V2)
// {
//     value = param.getInt();
//     if (value == HIGH)
//     {
//         // ERa.virtualWrite(V3, value = 0);
//         address_slave = "slave1";
//         tendulieu = "catnang";
//         dulieu = "ok";
//         mes = address_slave + "|" + tendulieu + "|" + dulieu;
//         DEBUG_PRINT("Sending packet: ");
//         DEBUG_PRINTLN(mes);
//         LoRa.beginPacket(); // Start a new packet
//         LoRa.print(mes);
//         LoRa.endPacket();              // Finish the packet and send it
//         digitalWrite(led_slave, HIGH); // Turn off LED for slave status
//         delay(20);                     // Delay to ensure the message is sent
//         digitalWrite(led_slave, LOW);  // Turn off LED for slave status
//     }
//     else
//     {
//         // ERa.virtualWrite(V3, value = 1);
//         address_slave = "slave1";
//         tendulieu = "catnang";
//         dulieu = "not ok";
//         mes = address_slave + "|" + tendulieu + "|" + dulieu;
//         DEBUG_PRINT("Sending packet: ");
//         DEBUG_PRINTLN(mes);
//         LoRa.beginPacket(); // Start a new packet
//         LoRa.print(mes);
//         LoRa.endPacket();              // Finish the packet and send it
//         digitalWrite(led_slave, HIGH); // Turn on LED for slave status
//         delay(20);                     // Delay to ensure the message is sent
//         digitalWrite(led_slave, LOW);  // Turn off LED for slave status
//     }
// }
// ERA_WRITE(V3)
// {
//     value = param.getInt();

//     address_slave = "slave1";
//     tendulieu = "dinhduong1";
//     dulieu = String(value); // Use the value as the data
//     mes = address_slave + "|" + tendulieu + "|" + dulieu;
//     DEBUG_PRINT("Sending packet: ");
//     DEBUG_PRINTLN(mes);
//     LoRa.beginPacket(); // Start a new packet
//     LoRa.print(mes);
//     LoRa.endPacket();              // Finish the packet and send it
//     digitalWrite(led_slave, HIGH); // Turn off LED for slave status
//     delay(20);                     // Delay to ensure the message is sent
//     digitalWrite(led_slave, LOW);  // Turn off LED for slave status
// }
void Setup_master()
{

    // #if defined(ERA_DEBUG)
    Serial.begin(115200);
    // #endif
    pinMode(led_connected, OUTPUT);   // Set LED pin as output
    pinMode(led_master, OUTPUT);      // Set LED pin as output
    pinMode(led_slave, OUTPUT);       // Set LED pin as output
    digitalWrite(led_slave, LOW);     // Turn off LED for slave status
    digitalWrite(led_master, HIGH);   // Turn on LED for master status
    digitalWrite(led_connected, LOW); // Turn off LED initially

#if defined(BUTTON_PIN)
    /* Initializing button. */
    initButton();
    /* Enable read/write WiFi credentials */
    ERa.setPersistent(true);
#endif

    ERa.setModbusClient(mbTcpClient);
    ERa.setScanWiFi(true);
    ERa.begin(ssid, pass);

    //     /* Setup timer called function every second */
    // ERa.addInterval(1000L, timerEvent);
    DEBUG_PRINT("-----------------PLG_start----------\n\r");
    // initialize Serial Monitor
    Serial.begin(115200);
    while (!Serial)
        ;
    DEBUG_PRINTLN("LoRa Sender");

    // setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);

    // replace the LoRa.begin(---E-) argument with your location's frequency
    // 433E6 for Asia
    // 868E6 for Europe
    // 915E6 for North America
    // LoRa.begin(433E6); // Initialize LoRa with the specified frequency
    while (!LoRa.begin(433E6))
    {
        DEBUG_PRINT(".");
        delay(500);
    }
}
void PLG_check_mode_connect()
{

    // This function is called when the WiFi AP is running
    // You can add your code here to handle the AP mode
    if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA)
    {
        static unsigned long lastTime = 0;
        static bool ledState = false;
        if (millis() - lastTime >= 400) // Check if 400 milliseconds have passed
        {
            ledState = !ledState; // Toggle the LED state
            digitalWrite(led_connected, ledState ? HIGH : LOW);
            lastTime = millis();
        }
    }
    else
    {
        ERa.connected() ? digitalWrite(led_connected, HIGH) : digitalWrite(led_connected, LOW);
    }
}
ERA_APP_LOOP()
{

    PLG_check_mode_connect(); // Check the connection mode and update LED status
    
}
void PLG_master_loop()
{

    ERa.run(); // Run ERa to handle network events
}
