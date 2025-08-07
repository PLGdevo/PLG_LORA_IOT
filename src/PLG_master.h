
/**************************************************************
 *                PLG END CONFIG_WIFI_SENDER
 ***************************************************************/

#include <PLG_set_era.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <PLG_datastring.h>
#include <EEPROM.h>

// Define hardware pins
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
String ID_control = "0";
String ID_CB = "1";
String ID_DD = "2";
String ID_master = "3";
int status_control = 0;
// EEPROM addresses
#define ADDR_BOM 0
#define ADDR_CATNAG 1
#define ADDR_DEN 2
#define ADDR_PHUNSUONG 3
#define ADDR_DAOKHI 4
#define ADDR_QUATHUT 5
#define ADDR_BOMCCON 6
#define BOM "V1"
#define BOMCCON "V2"
#define CATNAG "V3"
#define DEN "V4"
#define PHUNSUONG "V5"
#define DAOKHI "V6"
#define QUATHUT "V7"

unsigned long lastCheck = 0;
unsigned long lastTime = 0;
const unsigned long interval = 5000; // 5 giây
int value = 0;
float temp = 0.0;    // V10
float hum = 0.0;     // V11
float lux = 0.0;     // V12
float ph_nuoc = 0.0; // V20
float ec_nuoc = 0.0; // V19
float ph_dat = 0.0;  // V13
int level_cool = 0;
String mode = "";
struct ThietBi
{
    int bom = 0;
    int catnag = 0;
    int den = 0;
    int phunsuong = 0;
    int daokhi = 0;
    int quathut = 0;
    int bomcaycon = 0;
};

ThietBi tb;

void handleDeviceControl(String deviceName, String status, int ledPin)
{
    PLG_write_4(ID_master, ID_control, deviceName.c_str(), status.c_str());

    LoRa.beginPacket();
    LoRa.print(messages4);
    LoRa.endPacket();
    DEBUG_PRINTF("[STATUS] %-10s → %s\n", deviceName, status);
    digitalWrite(ledPin, HIGH);
    delay(20);
    digitalWrite(ledPin, LOW);
}
void PLG_WRITE_EEPROOM()
{
    EEPROM.write(ADDR_BOM, (uint8_t)tb.bom);
    EEPROM.write(ADDR_CATNAG, (uint8_t)tb.catnag);
    EEPROM.write(ADDR_DEN, (uint8_t)tb.den);
    EEPROM.write(ADDR_PHUNSUONG, (uint8_t)tb.phunsuong);
    EEPROM.write(ADDR_DAOKHI, (uint8_t)tb.daokhi);
    EEPROM.write(ADDR_QUATHUT, (uint8_t)tb.quathut);
    EEPROM.write(ADDR_BOMCCON, (uint8_t)tb.bomcaycon);
    EEPROM.commit();
    DEBUG_PRINTF("Qhut : %d  catnang : %d  funS : %d  dao khi  : %d", tb.quathut, tb.catnag, tb.daokhi, tb.phunsuong);
}
void PLG_READ_EEPROOM()
{
    tb.bom = EEPROM.read(ADDR_BOM);
    tb.catnag = EEPROM.read(ADDR_CATNAG);
    tb.den = EEPROM.read(ADDR_DEN);
    tb.phunsuong = EEPROM.read(ADDR_PHUNSUONG);
    tb.daokhi = EEPROM.read(ADDR_DAOKHI);
    tb.quathut = EEPROM.read(ADDR_QUATHUT);
    tb.bomcaycon = EEPROM.read(ADDR_BOMCCON);
}
ERA_WRITE(V0) // Bơm
{
    tb.bom = param.getInt();
    ERa.virtualWrite(V0, tb.bom);
    PLG_WRITE_EEPROOM();
    handleDeviceControl(BOM, tb.bom == HIGH ? "ok" : "not ok", led_slave);
}
ERA_WRITE(V1) // Quạt hút
{
    tb.quathut = param.getInt();
    ERa.virtualWrite(V1, tb.quathut);
    PLG_WRITE_EEPROOM();
    handleDeviceControl(QUATHUT, tb.quathut == HIGH ? "ok" : "not ok", led_slave);
}
ERA_WRITE(V2) // Cắt nâng
{
    tb.catnag = param.getInt();
    ERa.virtualWrite(V2, tb.catnag);
    PLG_WRITE_EEPROOM();
    handleDeviceControl(CATNAG, tb.catnag == HIGH ? "ok" : "not ok", led_slave);
}
ERA_WRITE(V5) // Quạt đảo khí
{
    tb.daokhi = param.getInt();
    ERa.virtualWrite(V5, tb.daokhi);
    PLG_WRITE_EEPROOM();
    handleDeviceControl(DAOKHI, tb.daokhi == HIGH ? "ok" : "not ok", led_slave);
}
ERA_WRITE(V6) // Bơm cây con
{
    tb.bomcaycon = param.getInt();
    PLG_WRITE_EEPROOM();
    handleDeviceControl(BOMCCON, tb.bomcaycon == HIGH ? "ok" : "not ok", led_slave);
}
ERA_WRITE(V7) // Đèn
{
    tb.den = param.getInt();

    PLG_WRITE_EEPROOM();
    handleDeviceControl(DEN, tb.den == HIGH ? "ok" : "not ok", led_slave);
}
ERA_WRITE(V8) // Phun sương
{
    tb.phunsuong = param.getInt();
    ERa.virtualWrite(V8, tb.phunsuong);
    PLG_WRITE_EEPROOM();
    handleDeviceControl(PHUNSUONG, tb.phunsuong == HIGH ? "ok" : "not ok", led_slave);
}
ERA_WRITE(V10) // nhiet do
{
    ERa.virtualWrite(V10, temp);
}
ERA_WRITE(V11) // do am
{
    ERa.virtualWrite(V11, hum); // Assuming 'hum' is a float variable for humidity
}
ERA_WRITE(V12) // lux
{
    ERa.virtualWrite(V12, lux);
}
ERA_WRITE(V13) // PH dat
{
    ERa.virtualWrite(V13, ph_dat);
}
ERA_WRITE(V3) // su dung dinh duong 1
{
    value = param.getInt();
}
ERA_WRITE(V4) // su dung dinh duong 2
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
    ERa.virtualWrite(V19, ec_nuoc); // Assuming 'ec_nuoc' is a float variable for EC
}
ERA_WRITE(V20) // PH_nuoc
{
    ERa.virtualWrite(V20, ph_nuoc); // Assuming 'ph_nuoc' is a float variable for pH of water
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
    EEPROM.begin(512);
    // #endif

    pinMode(led_connected, OUTPUT);   // Set LED pin as output
    pinMode(led_master, OUTPUT);      // Set LED pin as output
    pinMode(led_slave, OUTPUT);       // Set LED pin as output
    digitalWrite(led_slave, LOW);     // Turn off LED for slave status
    digitalWrite(led_master, HIGH);   // Turn on LED for master status
    digitalWrite(led_connected, LOW); // Turn off LED initially
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
    PLG_READ_EEPROOM();
    ERa.virtualWrite(V0, tb.bom);
    ERa.virtualWrite(V1, tb.quathut);
    ERa.virtualWrite(V2, tb.catnag);
    ERa.virtualWrite(V5, tb.daokhi);
    ERa.virtualWrite(V6, tb.bomcaycon);
    ERa.virtualWrite(V7, tb.den);
    ERa.virtualWrite(V8, tb.phunsuong);
    handleDeviceControl(BOM, tb.bom == HIGH ? "ok" : "not ok", led_slave);
    handleDeviceControl(QUATHUT, tb.quathut == HIGH ? "ok" : "not ok", led_slave);
    handleDeviceControl(CATNAG, tb.catnag == HIGH ? "ok" : "not ok", led_slave);
    handleDeviceControl(DAOKHI, tb.daokhi == HIGH ? "ok" : "not ok", led_slave);
    handleDeviceControl(BOMCCON, tb.bomcaycon == HIGH ? "ok" : "not ok", led_slave);
    handleDeviceControl(DEN, tb.den == HIGH ? "ok" : "not ok", led_slave);
    handleDeviceControl(PHUNSUONG, tb.phunsuong == HIGH ? "ok" : "not ok", led_slave);
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

    // if (millis() - lastCheck >= interval) // Check if 400 milliseconds have passed
    // {
    //     if (ERa.connected() == true)
    //     {
    //         lastCheck = millis();
    //         digitalWrite(led_connected, HIGH);
    //         PLG_write_4("master", "slave1", "wifi", "connected");
    //         DEBUG_PRINTLN(messages4);
    //         LoRa.beginPacket();
    //         LoRa.print(messages4);
    //         LoRa.endPacket();

    //         digitalWrite(led_slave, HIGH);
    //         delay(20);
    //         digitalWrite(led_slave, LOW);

    //     }
    //     else
    //     {
    //         lastCheck = millis();
    //         digitalWrite(led_connected, HIGH);
    //         PLG_write_4("master", "slave1", "wifi", "disconnected");
    //         DEBUG_PRINTLN(messages4);
    //         LoRa.beginPacket();
    //         LoRa.print(messages4);
    //         LoRa.endPacket();

    //         digitalWrite(led_slave, HIGH);
    //         delay(20);
    //         digitalWrite(led_slave, LOW);
    //     }
    // }
}
// Hàm xử lý cảm biến
void PLG_data_Sensor()
{
    if (namedata.startsWith("all_CB"))
    {
        temp = data1.toFloat();
        hum = data2.toFloat();
        lux = data3.toFloat();
        ph_dat = data4.toFloat();
        ERa.virtualWrite(V10, temp);
        ERa.virtualWrite(V11, hum);
        ERa.virtualWrite(V12, lux);
        ERa.virtualWrite(V13, ph_dat);
        // DEBUG_PRINTF("temp: %.2f   hum: %.2f   lux: %.2f   PH-dat: %.2f\n", temp, hum, lux, ph_dat);
    }
    else if (namedata.startsWith("ec_nuoc"))
    {
        ec_nuoc = data1.toFloat();
        ERa.virtualWrite(V19, ec_nuoc);
        // DEBUG_PRINT("EC nước: ");
        // DEBUG_PRINTLN(ec_nuoc);
    }
    else if (namedata.startsWith("ph_nuoc"))
    {
        ph_nuoc = data2.toFloat();
        ERa.virtualWrite(V20, ph_nuoc);
        // DEBUG_PRINT("pH nước: ");
        // DEBUG_PRINTLN(ph_nuoc);
    }
    else
    {
        // DEBUG_PRINT("Tên dữ liệu không hợp lệ: ");
    }
    if (status_control == 1)
    {

        PLG_READ_EEPROOM();
        handleDeviceControl("PLG", "test", led_slave);
        handleDeviceControl(BOM, tb.bom == HIGH ? "ok" : "not ok", led_slave);
        // handleDeviceControl(QUATHUT, tb.quathut == HIGH ? "ok" : "not ok", led_slave);
        // handleDeviceControl(CATNAG, tb.catnag == HIGH ? "ok" : "not ok", led_slave);
        // handleDeviceControl(DAOKHI, tb.daokhi == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(BOMCCON, tb.bomcaycon == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(DEN, tb.den == HIGH ? "ok" : "not ok", led_slave);
        // handleDeviceControl(PHUNSUONG, tb.phunsuong == HIGH ? "ok" : "not ok", led_slave);
    }
    else if (status_control == 0)
    {
        PLG_READ_EEPROOM();
        handleDeviceControl("PLG", "test", led_slave);
        handleDeviceControl(BOM, tb.bom == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(QUATHUT, tb.quathut == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(CATNAG, tb.catnag == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(DAOKHI, tb.daokhi == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(BOMCCON, tb.bomcaycon == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(DEN, tb.den == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(PHUNSUONG, tb.phunsuong == HIGH ? "ok" : "not ok", led_slave);
    }
    DEBUG_PRINTLN("--------------------------------------end");
}
void PLG_level()
{
    if (level_cool == 0)
    {
        // tb.quathut = 0;
        // tb.daokhi = 0;
        // tb.catnag = 0;
        // tb.phunsuong = 0;
        // // PLG_WRITE_EEPROOM();
        PLG_READ_EEPROOM();
        ERa.virtualWrite(V0, tb.bom);
        ERa.virtualWrite(V1, tb.quathut);
        ERa.virtualWrite(V2, tb.catnag);
        ERa.virtualWrite(V5, tb.daokhi);
        ERa.virtualWrite(V6, tb.bomcaycon);
        ERa.virtualWrite(V7, tb.den);
        ERa.virtualWrite(V8, tb.phunsuong);
        DEBUG_PRINTLN("Level 0: All devices off");
    }
    else if (level_cool == 1)
    {
        tb.quathut = 1;
        // tb.daokhi = 0;
        // tb.catnag = 0;
        // tb.phunsuong = 0;
        // PLG_WRITE_EEPROOM();
        // PLG_READ_EEPROOM();
        ERa.virtualWrite(V0, tb.bom);
        ERa.virtualWrite(V1, tb.quathut);
        ERa.virtualWrite(V2, tb.catnag);
        ERa.virtualWrite(V5, tb.daokhi);
        ERa.virtualWrite(V6, tb.bomcaycon);
        ERa.virtualWrite(V7, tb.den);
        ERa.virtualWrite(V8, tb.phunsuong);
        DEBUG_PRINTLN("Level 1: Quạt hút on, others off");
    }
    else if (level_cool == 2)
    {
        tb.quathut = 1;
        tb.daokhi = 1;
        // tb.catnag = 0;
        // tb.phunsuong = 0;
        // PLG_WRITE_EEPROOM();
        // PLG_READ_EEPROOM();
        ERa.virtualWrite(V0, tb.bom);
        ERa.virtualWrite(V1, tb.quathut);
        ERa.virtualWrite(V2, tb.catnag);
        ERa.virtualWrite(V5, tb.daokhi);
        ERa.virtualWrite(V6, tb.bomcaycon);
        ERa.virtualWrite(V7, tb.den);
        ERa.virtualWrite(V8, tb.phunsuong);
        DEBUG_PRINTLN("Level 2: Quạt hút and Quạt đảo khí on, others off");
    }
    else if (level_cool == 3)
    {
        tb.quathut = 1;
        tb.daokhi = 1;
        tb.catnag = 1;
        // tb.phunsuong = 0;
        // PLG_WRITE_EEPROOM();
        // PLG_READ_EEPROOM();
        ERa.virtualWrite(V0, tb.bom);
        ERa.virtualWrite(V1, tb.quathut);
        ERa.virtualWrite(V2, tb.catnag);
        ERa.virtualWrite(V5, tb.daokhi);
        ERa.virtualWrite(V6, tb.bomcaycon);
        ERa.virtualWrite(V7, tb.den);
        ERa.virtualWrite(V8, tb.phunsuong);
        DEBUG_PRINTLN("Level 3: Quạt hút, Quạt đảo khí, and Cắt nâng on, Phun sương off");
    }
    else if (level_cool == 4)
    {
        tb.quathut = 1;
        tb.daokhi = 1;
        tb.catnag = 1;
        tb.phunsuong = 1;
        // PLG_WRITE_EEPROOM();
        // PLG_READ_EEPROOM();
        ERa.virtualWrite(V0, tb.bom);
        ERa.virtualWrite(V1, tb.quathut);
        ERa.virtualWrite(V2, tb.catnag);
        ERa.virtualWrite(V5, tb.daokhi);
        ERa.virtualWrite(V6, tb.bomcaycon);
        ERa.virtualWrite(V7, tb.den);
        ERa.virtualWrite(V8, tb.phunsuong);
        DEBUG_PRINTLN("Level 4: All devices on");
    }
    else if (level_cool == 5)
    {
        // tb.quathut = 1;
        // tb.daokhi = 1;
        // tb.catnag = 1;
        // tb.phunsuong = 1;
        // PLG_WRITE_EEPROOM();
        // PLG_READ_EEPROOM();
        // ERa.virtualWrite(V0, tb.bom);
        // ERa.virtualWrite(V1, tb.quathut);
        // ERa.virtualWrite(V2, tb.catnag);
        // ERa.virtualWrite(V5, tb.daokhi);
        // ERa.virtualWrite(V6, tb.bomcaycon);
        // ERa.virtualWrite(V7, tb.den);
        // ERa.virtualWrite(V8, tb.phunsuong);
    }
    // DEBUG_PRINTLN("PLG_UPDATE_LEVEL");
}
void PLG_thucthilenh() // Execute the command
{
    if (address.startsWith(ID_CB) && address_slave.startsWith("slave1"))
    {
        PLG_data_Sensor(); // Process sensor data
        DEBUG_PRINTF("temp: %.2f   hum: %.2f   lux: %.2f   PH-dat: %.2f\n", temp, hum, lux, ph_dat);
    }
    if (address_slave.startsWith(ID_master) && address.startsWith(ID_DD))
    {
        PLG_data_Sensor(); // Process sensor data
    }
    // if (address.startsWith(ID_CB) &&  data.startsWith("end"))
    // {
    // }
    if (address.startsWith(ID_control) && data.startsWith("reset"))
    {
        PLG_READ_EEPROOM();
        handleDeviceControl("PLG", "test", led_slave);
        handleDeviceControl(BOM, tb.bom == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(QUATHUT, tb.quathut == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(CATNAG, tb.catnag == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(DAOKHI, tb.daokhi == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(BOMCCON, tb.bomcaycon == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(DEN, tb.den == HIGH ? "ok" : "not ok", led_slave);
        handleDeviceControl(PHUNSUONG, tb.phunsuong == HIGH ? "ok" : "not ok", led_slave);
    }

    if (address_slave.startsWith(ID_master))
    {
        if (namedata.startsWith(ID_control))
        {
            if (data.startsWith("manual"))
            {
                status_control = 0;
                // DEBUG_PRINTLN(status_cotrol);
            }
            else if (data.startsWith("auto"))
            {
                status_control = 1;
                // DEBUG_PRINTLN(status_cotrol);
            }
        }
        if (namedata.startsWith("level"))
        {
            level_cool = data.toInt();
            DEBUG_PRINTF("---------------ok_LEVEL:");
            DEBUG_PRINT(level_cool);
            DEBUG_PRINTLN("----------------");
        }
    }
    // if (namedata.startsWith("level"))
    // {
    //     level_cool = data.toInt();
    // }
}
ERA_APP_LOOP()
{

    PLG_check_mode_connect(); // Check the connection mode and update LED status
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        receivedData = "";
        while (LoRa.available())
        {
            receivedData += (char)LoRa.read();
        }
        // DEBUG_PRINTLN("Received data: ");
        // DEBUG_PRINTLN(receivedData);
        PLG_check_message();
        PLG_thucthilenh();
        digitalWrite(led_slave, HIGH);
        delay(10);
        digitalWrite(led_slave, LOW);
    }
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 1500)
    {

        PLG_level();
        lastTime = currentTime;
    }
}
void PLG_master_loop()
{
    ERa.run(); // Run ERa to handle network events
}
