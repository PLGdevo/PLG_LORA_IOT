
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

#define BOM "V1"
#define BOMCCON "V2"
#define CATNAG "V3"
#define DEN "V4"
#define PHUNSUONG "V5"
#define DAOKHI "V6"
#define QUATHUT "V7"

unsigned long lastTime = 0;
float temp = 0.0;          // V10
float hum = 0.0;           // V11
float lux = 0.0;           // V12
float ph_nuoc = 0.0;       // V20
float ec_nuoc = 0.0;       // V19
float ph_dat = 0.0;        // V13
bool start_sensor = false; // Flag to indicate if sensor data should be processed

// String receivedData = "";
String cmd = "";
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
    PLG_write_4("slave1", "master", "slave1", "reset");
    DEBUG_PRINTLN("PLG_board_relay_reset");
    sen_lora_data_4();
}
void sen_data_cambien()
{
    // Example of sending sensor data
    struct SensorData {
    const char* name;
    float value;
};

SensorData sensorList[] = {
    { "temp",     temp },
    { "hum",      hum },
    { "lux",      lux },
    { "ph_nuoc",  ph_nuoc },
    { "ec_nuoc",  ec_nuoc },
    { "ph_dat",   ph_dat }
};

for (const auto& sensor : sensorList) {
    PLG_write_4("slave1", "master", sensor.name, String(sensor.value, 2));
    sen_lora_data_4();
}
    DEBUG_PRINTF("temp: %.2f   hum: %.2f   lux: %.2f   PH-dat: %.2f\n", temp, hum, lux, ph_dat);
}
void thucthilenh()
{
    if (address_slave.startsWith("slave1"))
{
    // Map tên thiết bị sang tên hiển thị debug
    struct Device {
        const char* name;
        const char* debugName;
    };

    const Device devices[] = {
        { BOM,        "bom" },
        { DAOKHI,     "quat" },
        { QUATHUT,    "quat hut" },
        { CATNAG,     "cat nang" },
        { PHUNSUONG,  "phunsuong" },
        { BOMCCON,    "bomccon" },
        { DEN,        "den" }
    };

    bool commandHandled = false;

    for (const auto& device : devices)
    {
        if (namedata.startsWith(device.name))
        {
            if (data.startsWith("ok"))
            {
                DEBUG_PRINT("[ON] ", device.debugName);
                PLG_write_4("slave1", "PLG_relay", device.name, "ON");
            }
            else if (data.startsWith("not ok"))
            {
                DEBUG_PRINT("[OFF] ", device.debugName);
                PLG_write_4("slave1", "PLG_relay", device.name, "OFF");
            }
            SEN_PRINTLN(messages4);
            commandHandled = true;
            break;
        }
    }

    // Xử lý các lệnh đặc biệt khác không dùng chung mô-típ
    if (!commandHandled)
    {
        if (namedata.startsWith("wifi") && data.startsWith("connected"))
        {
            PLG_write_4("slave1", "PLG_relay", "requet", "run");
            SEN_PRINTLN(messages4);
            commandHandled = true;
        }
        else if (namedata.startsWith("temp"))
        {
            temp = data.toFloat();
            // DEBUG_PRINT("Nhiệt độ: ", temp);
            commandHandled = true;
        }
        else if (namedata.startsWith("hum"))
        {
            hum = data.toFloat();
            // DEBUG_PRINT("Độ ẩm: ", hum);
            commandHandled = true;
        }
        else if (namedata.startsWith("lux"))
        {
            lux = data.toFloat();
            // DEBUG_PRINT("Lux: ", lux);
            commandHandled = true;
        }
        else if (namedata.startsWith("ph_dat"))
        {
            ph_dat = data.toFloat();
            // DEBUG_PRINT("PH đất: ", ph_dat);
            commandHandled = true;
        }
        else if (namedata.startsWith("sensor") && data.startsWith("end"))
        {
            start_sensor = true;
            commandHandled = true;
        }
    }
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

    if (start_sensor == true) // Check if it's time to send sensor data
    {
        sen_data_cambien();
        start_sensor = false; // Reset the flag after sending data
        PLG_write_4("slave1", "master", "PLG_relay", "update");
        DEBUG_PRINTLN("PLG_board_relay_update");
        sen_lora_data_4();
    }
}

void PLG_slave_loop()
{
    loop_slave();
}
