
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
String ID_NHA_4 = "5";
int status_control = 0;
// EEPROM addresses
#define ADDR_BOM 0
#define ADDR_CATNAG 1
#define ADDR_DEN 2
#define ADDR_PHUNSUONG 3
#define ADDR_DAOKHI 4
#define ADDR_QUATHUT 5
#define ADDR_BOMCCON 6
#define ADDR_BONNUOC 7
#define ADDR_RUN_DD 14

#define ADDR_BOM_NHA_4 8
#define ADDR_VAN_1 9
#define ADDR_VAN_2 10
#define ADDR_VAN_3 11
#define ADDR_VAN_4 12

#define ADDR_AUTO 13
//---------------- nhà số 2
#define BOM "V1"
#define BOMCCON "V2"
#define CATNAG "V3"
#define DEN "V4"
#define PHUNSUONG "V5"
#define DAOKHI "V6"
#define QUATHUT "V7"
#define BONNUOC "V8"
#define run_dd "V13"
//---------------nhà số 4
#define BOM_NHA_4 "V9"
#define VAN_1 "V10"
#define VAN_2 "V11"
#define VAN_3 "V12"

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
struct thietbi_nha4 // nha so 4
{
    int bom = 0;
    int van_1 = 0;
    int van_2 = 0;
    int van_3 = 0;
};
thietbi_nha4 tb_nha4;
struct ThietBi // nha so 2
{
    int bom = 0;
    int catnag = 0;
    int den = 0;
    int phunsuong = 0;
    int daokhi = 0;
    int quathut = 0;
    int bomcaycon = 0;
    int bonnuoc = 0;
    int Run_dd = 1;
};

ThietBi tb;

void handleDeviceControl(String deviceName, String status, int ledPin) // gui data nha so 2
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
void handleDeviceControl_s4(String deviceName, String status, int ledPin) // gui data nha so 4
{
    PLG_write_4(ID_master, ID_NHA_4, deviceName.c_str(), status.c_str());

    LoRa.beginPacket();
    LoRa.print(messages4);
    LoRa.endPacket();
    DEBUG_PRINTF("[STATUS] %-10s → %s\n", deviceName, status);
    digitalWrite(ledPin, HIGH);
    delay(20);
    digitalWrite(ledPin, LOW);
}
void handleDevicecontrol_nha2(int ledPin)
{
    LoRa.beginPacket();
    LoRa.print(messages_nha2);
    LoRa.endPacket();
    DEBUG_PRINTLN(messages_nha2);
    digitalWrite(ledPin, HIGH);
    delay(20);
    digitalWrite(ledPin, LOW);
}
void handleDeviceControl_s7(int ledPin) // gui data nha so 4
{

    LoRa.beginPacket();
    LoRa.print(messages_7);
    LoRa.endPacket();
    digitalWrite(ledPin, HIGH);
    delay(20);
    digitalWrite(ledPin, LOW);
}
void PLG_WRITE_EEPROOM()
{
    EEPROM.write(ADDR_RUN_DD, (uint8_t)tb.Run_dd);
    EEPROM.write(ADDR_BOM, (uint8_t)tb.bom);
    EEPROM.write(ADDR_CATNAG, (uint8_t)tb.catnag);
    EEPROM.write(ADDR_DEN, (uint8_t)tb.den);
    EEPROM.write(ADDR_PHUNSUONG, (uint8_t)tb.phunsuong);
    EEPROM.write(ADDR_DAOKHI, (uint8_t)tb.daokhi);
    EEPROM.write(ADDR_QUATHUT, (uint8_t)tb.quathut);
    EEPROM.write(ADDR_BOMCCON, (uint8_t)tb.bomcaycon);
    EEPROM.write(ADDR_BONNUOC, (uint8_t)tb.bonnuoc);

    EEPROM.write(ADDR_BOM_NHA_4, (uint8_t)tb_nha4.bom);
    EEPROM.write(ADDR_VAN_1, (uint8_t)tb_nha4.van_1);
    EEPROM.write(ADDR_VAN_2, (uint8_t)tb_nha4.van_2);
    EEPROM.write(ADDR_VAN_3, (uint8_t)tb_nha4.van_3);

    EEPROM.commit();
}
void PLG_READ_EEPROOM()
{
    tb.Run_dd = EEPROM.read(ADDR_RUN_DD);
    tb.bom = EEPROM.read(ADDR_BOM);
    tb.catnag = EEPROM.read(ADDR_CATNAG);
    tb.den = EEPROM.read(ADDR_DEN);
    tb.phunsuong = EEPROM.read(ADDR_PHUNSUONG);
    tb.daokhi = EEPROM.read(ADDR_DAOKHI);
    tb.quathut = EEPROM.read(ADDR_QUATHUT);
    tb.bomcaycon = EEPROM.read(ADDR_BOMCCON);
    tb.bonnuoc = EEPROM.read(ADDR_BONNUOC);
    tb_nha4.bom = EEPROM.read(ADDR_BOM_NHA_4);
    tb_nha4.van_1 = EEPROM.read(ADDR_VAN_1);
    tb_nha4.van_2 = EEPROM.read(ADDR_VAN_2);
    tb_nha4.van_3 = EEPROM.read(ADDR_VAN_3);
}

ERA_WRITE(V3) // auto || manual
{
    tb.Run_dd = param.getInt();
    ERa.virtualWrite(V3, tb.Run_dd);
    PLG_WRITE_EEPROOM();
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
}
ERA_WRITE(V0) // Bơm
{
    tb.bom = param.getInt();
    ERa.virtualWrite(V0, tb.bom);
    PLG_WRITE_EEPROOM();
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
}
ERA_WRITE(V26) // bon nuoc
{
    tb.bonnuoc = param.getInt();
    ERa.virtualWrite(V26, tb.bonnuoc);
    PLG_WRITE_EEPROOM();
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
}
ERA_WRITE(V1) // Quạt hút
{
    tb.quathut = param.getInt();
    ERa.virtualWrite(V1, tb.quathut);
    PLG_WRITE_EEPROOM();
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
}
ERA_WRITE(V2) // Cắt nâng
{
    tb.catnag = param.getInt();
    ERa.virtualWrite(V2, tb.catnag);
    PLG_WRITE_EEPROOM();
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
}
ERA_WRITE(V5) // Quạt đảo khí
{
    tb.daokhi = param.getInt();
    ERa.virtualWrite(V5, tb.daokhi);
    PLG_WRITE_EEPROOM();
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
}
ERA_WRITE(V6) // Bơm cây con
{
    tb.bomcaycon = param.getInt();
    PLG_WRITE_EEPROOM();
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
}
ERA_WRITE(V7) // Đèn
{
    tb.den = param.getInt();

    PLG_WRITE_EEPROOM();
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
}
ERA_WRITE(V8) // Phun sương
{
    tb.phunsuong = param.getInt();
    ERa.virtualWrite(V8, tb.phunsuong);
    PLG_WRITE_EEPROOM();
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
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

//------------nha so 4
ERA_WRITE(V14) // bom nha 4
{
    tb_nha4.bom = param.getInt();
    ERa.virtualWrite(V14, tb_nha4.bom);
    PLG_WRITE_EEPROOM();
    PLG_write_7(ID_master, ID_NHA_4, "4", tb_nha4.bom ? "1" : "0", tb_nha4.van_1 ? "1" : "0", tb_nha4.van_2 ? "1" : "0", tb_nha4.van_3 ? "1" : "0");
    handleDeviceControl_s7(led_slave);
}
ERA_WRITE(V15) // van 1
{
    tb_nha4.van_1 = param.getInt();
    ERa.virtualWrite(V15, tb_nha4.van_1);
    PLG_WRITE_EEPROOM();
    PLG_write_7(ID_master, ID_NHA_4, "4", tb_nha4.bom ? "1" : "0", tb_nha4.van_1 ? "1" : "0", tb_nha4.van_2 ? "1" : "0", tb_nha4.van_3 ? "1" : "0");
    handleDeviceControl_s7(led_slave);
}
ERA_WRITE(V16) // van 2
{
    tb_nha4.van_2 = param.getInt();
    ERa.virtualWrite(V16, tb_nha4.van_2);
    PLG_WRITE_EEPROOM();
    PLG_write_7(ID_master, ID_NHA_4, "4", tb_nha4.bom ? "1" : "0", tb_nha4.van_1 ? "1" : "0", tb_nha4.van_2 ? "1" : "0", tb_nha4.van_3 ? "1" : "0");
    handleDeviceControl_s7(led_slave);
}
ERA_WRITE(V17) // van 3
{
    tb_nha4.van_3 = param.getInt();
    ERa.virtualWrite(V17, tb_nha4.van_3);
    PLG_WRITE_EEPROOM();
    PLG_write_7(ID_master, ID_NHA_4, "4", tb_nha4.bom ? "1" : "0", tb_nha4.van_1 ? "1" : "0", tb_nha4.van_2 ? "1" : "0", tb_nha4.van_3 ? "1" : "0");
    handleDeviceControl_s7(led_slave);
}
//-------------------------------

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
    LoRa.begin(433E6); // Initialize LoRa with the specified frequency
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
    ERa.virtualWrite(V26, tb.bonnuoc);
    ERa.virtualWrite(V14, tb_nha4.bom);
    ERa.virtualWrite(V15, tb_nha4.van_1);
    ERa.virtualWrite(V16, tb_nha4.van_2);
    ERa.virtualWrite(V17, tb_nha4.van_3);
    ERa.virtualWrite(V3, tb.Run_dd);
    // hut ,dao, cat,phun,bom,bomcc,den,auto
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
    handleDevicecontrol_nha2(led_slave);
    //----------------nha so 4

    PLG_write_7(ID_master, ID_NHA_4, "4", tb_nha4.bom ? "1" : "0", tb_nha4.van_1 ? "1" : "0", tb_nha4.van_2 ? "1" : "0", tb_nha4.van_3 ? "1" : "0");
    handleDeviceControl_s7(led_slave);
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
    else
    {
        DEBUG_PRINT("Tên dữ liệu cam bien không hợp lệ: ");
    }
}
void PLG_data_DD()
{
    if (namedata.startsWith("ec_nuoc"))
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
        PLG_data_Sensor();
        // DEBUG_PRINTF("temp: %.2f   hum: %.2f   lux: %.2f   PH-dat: %.2f\n", temp, hum, lux, ph_dat);
    }
    if (address.startsWith(ID_control) && address_slave.startsWith(ID_master))
    {
        // doc ph+EC bo dd tu nha 2
        ph_nuoc = data_PH_nuoc.toFloat();
        ec_nuoc = data_EC_nuoc.toFloat();
        ERa.virtualWrite(V19, ec_nuoc);        
        ERa.virtualWrite(V20, ph_nuoc);
    }

    if (address.startsWith(ID_control) && data.startsWith("reset"))
    {
        PLG_READ_EEPROOM();
        PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "", "");
        handleDevicecontrol_nha2(led_slave);
    }
    if (address.startsWith(ID_NHA_4) && data.startsWith("reset"))
    {
        PLG_READ_EEPROOM();
        PLG_write_7(ID_master, ID_NHA_4, "4", tb_nha4.bom ? "1" : "0", tb_nha4.van_1 ? "1" : "0", tb_nha4.van_2 ? "1" : "0", tb_nha4.van_3 ? "1" : "0");
        handleDeviceControl_s7(led_slave);
    }
    PLG_READ_EEPROOM();

    PLG_write_7(ID_master, ID_NHA_4, "4", tb_nha4.bom ? "1" : "0", tb_nha4.van_1 ? "1" : "0", tb_nha4.van_2 ? "1" : "0", tb_nha4.van_3 ? "1" : "0");
    handleDeviceControl_s7(led_slave);
    PLG_write_nha_2(ID_master, ID_control, tb.quathut ? "1" : "0", tb.daokhi ? "1" : "0", tb.catnag ? "1" : "0", tb.phunsuong ? "1" : "0", tb.bom ? "1" : "0", tb.bomcaycon ? "1" : "0", tb.den ? "1" : "0", tb.bonnuoc ? "1" : "0", tb.Run_dd ? "1" : "0", "a", "b");
    handleDevicecontrol_nha2(led_slave);
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
}
void PLG_master_loop()
{
    ERa.run(); // Run ERa to handle network events
}
