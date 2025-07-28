
#include <PLG_datastring.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <string.h>
#include <hardwareSerial.h>

// #define PLG_sensor

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

unsigned long lastTime = 0;
float temp = 0.0;    // V10
float hum = 0.0;     // V11
float lux = 0.0;     // V12
float ph_nuoc = 0.0; // V20
float ec_nuoc = 0.0; // V19
float ph_dat = 0.0;  // V13
bool bom, catnag, den, funsuong, daokhi, quathut, bomcaycon;
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
void sen_lora_data_4()
{
    // Example of sending sensor data
    LoRa.beginPacket(); // Start a new packet
    LoRa.print(messages4);
    LoRa.endPacket();               // Finish the packet and send it
    digitalWrite(led_master, HIGH); // Turn off LED for slave status
    delay(20);                      // Delay to ensure the message is sent
    digitalWrite(led_master, LOW);  // Turn off LED for slave status
}
void thucthilenh()
{
    if (address_slave.startsWith("slave1") && namedata.startsWith("bom"))
    {

        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("bom ON");
            PLG_write_4("slave1", "PLG_relay", "bom", "ON");
            SEN_PRINTLN(messages4);
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("bom OFF");
            PLG_write_4("slave1", "PLG_relay", "bom", "OFF");
            SEN_PRINTLN(messages4);
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("quat"))
    {
        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("quat ON");
            PLG_write_4("slave1", "PLG_relay", "quat", "ON");
            SEN_PRINTLN(messages4);
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("quat OFF");
            PLG_write_4("slave1", "PLG_relay", "quat", "OFF");
            SEN_PRINTLN(messages4);
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("fanhut"))
    {
        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("quat hut ON");
            PLG_write_4("slave1", "PLG_relay", "fanhut", "ON");
            SEN_PRINTLN(messages4);
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("quat hut OFF");
            PLG_write_4("slave1", "PLG_relay", "fanhut", "OFF");
            SEN_PRINTLN(messages4);
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("catnag"))
    {
        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("cat nang ON");
            PLG_write_4("slave1", "PLG_relay", "catnag", "ON");
            SEN_PRINTLN(messages4);
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("cat nang OFF");
            PLG_write_4("slave1", "PLG_relay", "catnag", "OFF");
            SEN_PRINTLN(messages4);
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("phunsuong"))
    {
        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("phunsuong ON");
            PLG_write_4("slave1", "PLG_relay", "phunsuong", "ON");
            SEN_PRINTLN(messages4);
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("phunsuong OFF");
            PLG_write_4("slave1", "PLG_relay", "phunsuong", "OFF");
            SEN_PRINTLN(messages4);
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("bomccon"))
    {
        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("bomccon ON");
            PLG_write_4("slave1", "PLG_relay", "bomccon", "ON");
            SEN_PRINTLN(messages4);
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("bomcconOFF");
            PLG_write_4("slave1", "PLG_relay", "bomccon", "OFF");
            SEN_PRINTLN(messages4);
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("den"))
    {
        if (data.startsWith("ok"))
        {

            DEBUG_PRINTLN("den ON");
            PLG_write_4("slave1", "PLG_relay", "den", "ON");
            SEN_PRINTLN(messages4);
        }
        else if (data.startsWith("not ok"))
        {

            DEBUG_PRINTLN("den OFF");
            PLG_write_4("slave1", "PLG_relay", "den", "OFF");
            SEN_PRINTLN(messages4);
        }
    }
    else if (address_slave.startsWith("slave1") && namedata.startsWith("wifi"))
    {
        if (data.startsWith("connected"))
        {

            PLG_write_4("slave1", "PLG_relay", "requet", "run");
            SEN_PRINTLN(messages4)
        }
    }
    else if (address.startsWith("PLG_relay") && address_slave.startsWith("slave1"))
    {
        if (namedata.startsWith("bom"))
        {
            data=bom;
            PLG_write_4("slave1", "master", "bom", String(bom));
            sen_lora_data_4();
        }
        else if (namedata.startsWith("catnag"))
        {
            data=catnag;
            PLG_write_4("slave1", "master", "catnag", String(catnag));
            sen_lora_data_4();
        }
        else if (namedata.startsWith("den"))
        {
            data=den;
            PLG_write_4("slave1", "master", "den", String(den));
            sen_lora_data_4();
        }
        else if (namedata.startsWith("funsuong"))
        {
            data=funsuong;
            PLG_write_4("slave1", "master", "funsuong", String(funsuong));
            sen_lora_data_4();
        }
        else if (namedata.startsWith("daokhi"))
        {
            data=daokhi;
            PLG_write_4("slave1", "master", "daokhi", String(daokhi));
            sen_lora_data_4();
        }
        else if (namedata.startsWith("quathut"))
        {
            data=quathut;
            PLG_write_4("slave1", "master", "quathut", String(quathut));
            sen_lora_data_4();
        }
        else if (namedata.startsWith("bomcaycon"))
        {
            data=bomcaycon;
            PLG_write_4("slave1", "master", "bomcaycon", String(bomcaycon));
            sen_lora_data_4();
        }
        
    }
    else
    {
        DEBUG_PRINTLN("Khong co lenh nao duoc thuc hien");
    }
}
void loop_slave()
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
        // DEBUG_PRINTF("temp: %.2f   hum: %.2f   lux: %.2f   PH-dat: %.2f\n", temp, hum, lux, ph_dat);
        digitalWrite(led_connected, HIGH);
        delay(20);
        digitalWrite(led_connected, LOW);
        thucthilenh(); // Execute the command
    }
    if (Serial2.available())
    {
        receivedData = Serial2.readStringUntil('\n'); // Ví dụ nhận chuỗi
        PLG_check_message();                          // Gọi xử lý chuỗi
        thucthilenh();                                // Execute the command
    }
}
/*------------------- board cam bien---------------------------------------------*/

void sen_data_cambien()
{
    // Example of sending sensor data
    PLG_write_4("slave2", "slave1", "temp", String(temp, 2));
    sen_lora_data_4();
    PLG_write_4("slave2", "slave1", "hum", String(hum, 2));
    sen_lora_data_4();
    PLG_write_4("slave2", "slave1", "lux", String(lux, 2));
    sen_lora_data_4();
    // PLG_write_4("slave2", "slave1", "ph_nuoc", String(ph_nuoc, 2));
    // sen_lora_data_4();
    // PLG_write_4("slave2", "slave1", "ec_nuoc", String(ec_nuoc, 2));
    // sen_lora_data_4();
    PLG_write_4("slave2", "slave1", "ph_dat", String(ph_dat, 2));
    sen_lora_data_4();
    DEBUG_PRINTF("temp: %.2f   hum: %.2f   lux: %.2f   PH-dat: %.2f\n", temp, hum, lux, ph_dat);
}
void loop_cambien()
{
    unsigned long currentTime = millis();

    if (currentTime - lastTime >= 2000)
    {
        // Gửi dữ liệu cảm biến mỗi 2 giây
        temp = hum = lux = ph_dat = temp + 0.5;
        sen_data_cambien();
        lastTime = currentTime;
    }
}

void PLG_slave_loop()
{
#if defined(PLG_sensor)
    loop_cambien();
#else
    loop_slave();
#endif
}
