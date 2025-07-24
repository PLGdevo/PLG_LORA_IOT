
/**************************************************************
 *                PLG END CONFIG_WIFI_SENDER
 ***************************************************************/

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
float temp = 0.0;    // V10
float hum = 0.0;     // V11
float lux = 0.0;     // V12
float ph_nuoc = 0.0; // V20
float ec_nuoc = 0.0; // V19
float ph_dat = 0.0;  // V13
// LoRa.beginPacket(); // Start a new packet
// LoRa.print(mes);
// LoRa.endPacket();              // Finish the packet and send it
// digitalWrite(led_slave, HIGH); // Turn off LED for slave status
// delay(20);                     // Delay to ensure the message is sent
// digitalWrite(led_slave, LOW);  // Turn off LED for slave status

ERA_WRITE(V0) // su dung bom
{
    value = param.getInt();
    if (value == HIGH)
    {
        PLG_write_4("master", "slave1", "bom", "ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn off LED for slave status
    }
    else if (value == LOW)
    {
        PLG_write_4("master", "slave1", "bom", "not ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn on LED for slave status
    }
    else
    {
        DEBUG_PRINTLN("Invalid value for V0");
    }
}
ERA_WRITE(V1) // su dung quat hut
{
    value = param.getInt();
    if (value == HIGH)
    {
        PLG_write_4("master", "slave1", "fanhut", "ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn off LED for slave status
    }
    else if (value == LOW)
    {
        PLG_write_4("master", "slave1", "fanhut", "not ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn on LED for slave status
    }
    else
    {
        DEBUG_PRINTLN("Invalid value for V1");
    }
}
ERA_WRITE(V2) // su dung cat nang
{
    value = param.getInt();
    if (value == HIGH)
    {
        PLG_write_4("master", "slave1", "catnag", "ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn off LED for slave status
    }
    else if (value == LOW)
    {
        PLG_write_4("master", "slave1", "catnag", "not ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn on LED for slave status
    }
    else
    {
        DEBUG_PRINTLN("Invalid value for V1");
    }
}
ERA_WRITE(V3) // su dung dinh duong 1
{
    value = param.getInt();
}
ERA_WRITE(V4) // su dung dinh duong 2
{
    value = param.getInt();
}
ERA_WRITE(V5) // su dung quat dao khi
{
    value = param.getInt();
    if (value == HIGH)
    {
        PLG_write_4("master", "slave1", "quat", "ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn off LED for slave status
    }
    else if (value == LOW)
    {
        PLG_write_4("master", "slave1", "quat", "not ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn on LED for slave status
    }
    else
    {
        DEBUG_PRINTLN("Invalid value for V5");
    }
}
ERA_WRITE(V6) // su dung bom cay con
{
    value = param.getInt();
    if (value == HIGH)
    {
        PLG_write_4("master", "slave1", "bom cay con", "ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn off LED for slave status
    }
    else if (value == LOW)
    {
        PLG_write_4("master", "slave1", "bom cay con", "not ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn on LED for slave status
    }
    else
    {
        DEBUG_PRINTLN("Invalid value for V6");
    }
}
ERA_WRITE(V7) // su dung Den
{
    value = param.getInt();
    if (value == HIGH)
    {
        PLG_write_4("master", "slave1", "den", "ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn off LED for slave status}
    }
    else if (value == LOW)
    {
        PLG_write_4("master", "slave1", "den", "not ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn on LED for slave status
    }
    else
    {
        DEBUG_PRINTLN("Invalid value for V7");
    }
}
ERA_WRITE(V8) // su dung phun suong
{
    value = param.getInt();
    if (value == HIGH)
    {
        PLG_write_4("master", "slave1", "phunsuong", "ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn off LED for slave status
    }
    else if (value == LOW)
    {
        PLG_write_4("master", "slave1", "phunsuong", "not ok");
        LoRa.beginPacket(); // Start a new packet
        LoRa.print(messages4);
        LoRa.endPacket();              // Finish the packet and send it
        digitalWrite(led_slave, HIGH); // Turn off LED for slave status
        delay(20);                     // Delay to ensure the message is sent
        digitalWrite(led_slave, LOW);  // Turn on LED for slave status
    }
    else
    {
        DEBUG_PRINTLN("Invalid value for V8");
    }
}
ERA_WRITE(V10) // nhiet do
{
    value = param.getInt();
}
ERA_WRITE(V11) // do am
{
    ERa.virtualWrite(V11, hum); // Assuming 'hum' is a float variable for humidity
}
ERA_WRITE(V12) // lux
{
    value = param.getInt();
}
ERA_WRITE(V13) // PH dat
{
    value = param.getInt();
}
ERA_WRITE(V14) // dinh duong 3
{
    value = param.getInt();
}
ERA_WRITE(V15) // dinh duong 4
{
    value = param.getInt();
}
ERA_WRITE(V16) // bom_1
{
    value = param.getInt();
}
ERA_WRITE(V17) // bom_2
{
    value = param.getInt();
}
ERA_WRITE(V9) // bom_3
{
    value = param.getInt();
}
ERA_WRITE(V18) // bom_4
{
    value = param.getInt();
}
ERA_WRITE(V19) // Do dan dien EC
{
    value = param.getInt();
}
ERA_WRITE(V20) // PH_nuoc
{
    value = param.getInt();
}
ERA_WRITE(V21) // en bom_1
{
    value = param.getInt();
}
ERA_WRITE(V22) // en bom_2
{
    value = param.getInt();
}
ERA_WRITE(V23) // en bom_3
{
    value = param.getInt();
}
ERA_WRITE(V24) // en bom_4
{
    value = param.getInt();
}
ERA_WRITE(V25) // run_BOM_DD
{
    value = param.getInt();
}

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

    /* Setup timer called function every second */
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
void PLG_check_mode_connect() // Check the connection mode and update LED status
{
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
// Hàm xử lý cảm biến
void PLG_data_Sensor(String name, String value)
{
    float val = value.toFloat();
    if (isnan(val))
    {
        DEBUG_PRINT("Lỗi: Không chuyển được ");
        DEBUG_PRINT(name);
        DEBUG_PRINT(" sang float: ");
        DEBUG_PRINTLN(value);
        return;
    }

    if (name == "temp")
    {
        temp = val;
        ERa.virtualWrite(V10, temp);
        DEBUG_PRINT("Nhiệt độ: ");
        DEBUG_PRINTLN(temp);
    }
    else if (name == "hum")
    {
        hum = val;
        ERa.virtualWrite(V11, hum);
        DEBUG_PRINT("Độ ẩm: ");
        DEBUG_PRINTLN(hum);
    }
    else if (name == "lux")
    {
        lux = val;
        ERa.virtualWrite(V12, lux);
        DEBUG_PRINT("Ánh sáng: ");
        DEBUG_PRINTLN(lux);
    }
    else if (name == "ph_dat")
    {
        ph_dat = val;
        ERa.virtualWrite(V13, ph_dat);
        DEBUG_PRINT("pH đất: ");
        DEBUG_PRINTLN(ph_dat);
    }
    else if (name == "ec_nuoc")
    {
        ec_nuoc = val;
        ERa.virtualWrite(V19, ec_nuoc);
        DEBUG_PRINT("EC nước: ");
        DEBUG_PRINTLN(ec_nuoc);
    }
    else if (name == "ph_nuoc")
    {
        ph_nuoc = val;
        ERa.virtualWrite(V20, ph_nuoc);
        DEBUG_PRINT("pH nước: ");
        DEBUG_PRINTLN(ph_nuoc);
    }
    else
    {
        DEBUG_PRINT("Tên dữ liệu không hợp lệ: ");
        DEBUG_PRINTLN(name);
    }
}

void PLG_thucthilenh() // Execute the command
{
    if (address_slave.startsWith("master"))
    {
        PLG_data_Sensor(namedata, data); // Process sensor data
    }
}
ERA_APP_LOOP()
{

    PLG_check_mode_connect(); // Check the connection mode and update LED status
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
        DEBUG_PRINTLN("Received data: ");
        DEBUG_PRINTLN(receivedData); // Print the received data to the Serial Monitor
        PLG_check_message();         // Check the received data
        PLG_thucthilenh();
    }
}
void PLG_master_loop()
{

    ERa.run(); // Run ERa to handle network events
}
