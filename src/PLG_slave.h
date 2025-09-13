
#include <PLG_datastring.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <string.h>
#include <ModbusMaster.h>
#include <hardwareSerial.h>
#include <esp_task_wdt.h>
#define WDT_time 10

// define the pins used by the transceiver module
#define ss 05
#define rst 04
#define dio0 14
#define led_connected 26
#define led_slave 33
#define led_master 25
#define TX 17
#define RX 16

#define BOM "V1"
#define BOMCCON "V2"
#define CATNAG "V3"
#define DEN "V4"
#define PHUNSUONG "V5"
#define DAOKHI "V6"
#define QUATHUT "V7"
#define BONNUOC "V8"
#define auto "V13"

ModbusMaster relay_modbus;
#define ID_relay_modbus 1
// thanh ghi output
#define RELAY_BOM 0
#define RELAY_BOMCCON 8
#define RELAY_CATNAG 3
#define RELAY_DEN 9
#define RELAY_PHUNSUONG 5
#define RELAY_DAOKHI 2
#define RELAY_QUATHUT 1
#define RELAY_BONNUOC 4

bool last_quathut = 0;
bool last_daokhi = 0;
bool last_catnag = 0;
bool last_phunsuong = 0;
bool last_bom = 0;
bool last_bomccon = 0;
bool last_den = 0;
bool last_bonnuoc = 0;

int TT_RELAY_QUATHUT = 0;
int TT_RELAY_DAOKHI = 0;
int TT_RELAY_CATNAG = 0;
int TT_RELAY_PHUNSUONG = 0;
int TT_RELAY_BOM = 0;
int TT_RELAY_BOMCCON = 0;
int TT_RELAY_DEN = 0;
int TT_RELAY_BONNUOC = 0;
//////////////////////////
#define ID_relay "PLG_relay"
String ID_control = "0";
String ID_CB = "1";
String ID_DD = "2";
String ID_master = "3";
String ID_control_DD = "4";
unsigned long lastLoRaReceiveTime = 0; // thời điểm cuối nhận LoRa
enum control_soure
{
    dk_master = 0,
    dk_control = 1
};
unsigned long lastTime = 0;
float temp = 0.0;          // V10
float hum = 0.0;           // V11
float lux = 0.0;           // V12
float ph_nuoc = 0.0;       // V20
float ec_nuoc = 0.0;       // V19
float ph_dat = 0.0;        // V13
bool start_sensor = false; // Flag to indicate if sensor data should be processed
bool Auto_cool = false;
int couter = 0, mode = 0;
float temp_setpoint = 38.0;
int chaytudong = 0;
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
    esp_task_wdt_reset();
}
void Setup_slave()
{

    Serial.begin(115200);
    DEBUG_PRINTLN("LoRa Sender");

    // setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);
    esp_task_wdt_init(WDT_time, true);
    esp_task_wdt_add(NULL);
    // 433E6 for Asia
    while (!LoRa.begin(433E6))
    {
        Serial.print(".");
        delay(500);
    }
    DEBUG_PRINTLN("LoRa Initialized");
    pinMode(led_slave, OUTPUT);
    pinMode(led_master, OUTPUT);
    pinMode(led_connected, OUTPUT);
    digitalWrite(led_connected, LOW);
    digitalWrite(led_slave, HIGH);
    digitalWrite(led_master, LOW);
    DEBUG_PRINTLN("PLG RUNNING SLAVE");

    Serial2.begin(9600, SERIAL_8N1, RX, TX);
    relay_modbus.begin(ID_relay_modbus, Serial2); // Cảm biến địa chỉ 2

    PLG_write_4(ID_control, ID_master, ID_control, Auto_cool ? "auto" : "manual");
    sen_lora_data_4();
    PLG_write_4(ID_control, ID_master, ID_control, "reset");
    DEBUG_PRINTLN("PLG_board_relay_reset");
    // relay_modbus.writeMultipleCoils(12,0x00);
    sen_lora_data_4();
    esp_task_wdt_reset();
}
void PLG_cooling_level_0()
{
    // PLG_write_4(ID_control, ID_relay, QUATHUT, "OFF");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_relay, CATNAG, "OFF");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_relay, PHUNSUONG, "OFF");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_relay, DAOKHI, "OFF");
    // SEN_PRINTLN(messages4);
    relay_modbus.writeSingleCoil(RELAY_PHUNSUONG, 0);
    relay_modbus.writeSingleCoil(RELAY_CATNAG, 0);
    relay_modbus.writeSingleCoil(RELAY_QUATHUT, 0);
    relay_modbus.writeSingleCoil(RELAY_DAOKHI, 0);
    PLG_write_4(ID_control, ID_master, "level", "0");
    sen_lora_data_4();
    PLG_write_4(ID_control, ID_master, "PLG_relay", "update");
    DEBUG_PRINTLN("PLG_board_relay_update");
    sen_lora_data_4();
    esp_task_wdt_reset();
}
void PLG_cooling_level_1()
{
    relay_modbus.writeSingleCoil(RELAY_QUATHUT, 1);

    esp_task_wdt_reset();
}
void PLG_cooling_level_2()
{
    relay_modbus.writeSingleCoil(RELAY_QUATHUT, 1);
    relay_modbus.writeSingleCoil(RELAY_DAOKHI, 1);
    esp_task_wdt_reset();
}
void PLG_cooling_level_3()
{
    relay_modbus.writeSingleCoil(RELAY_CATNAG, 1);
    relay_modbus.writeSingleCoil(RELAY_QUATHUT, 1);
    relay_modbus.writeSingleCoil(RELAY_DAOKHI, 1);
    esp_task_wdt_reset();
}
void PLG_cooling_level_4()
{
    relay_modbus.writeSingleCoil(RELAY_PHUNSUONG, 1);
    relay_modbus.writeSingleCoil(RELAY_CATNAG, 1);
    relay_modbus.writeSingleCoil(RELAY_QUATHUT, 1);
    relay_modbus.writeSingleCoil(RELAY_DAOKHI, 1);
    sen_lora_data_4();
    esp_task_wdt_reset();
}

void PLG_AIR_AUTO()
{
    static bool lastAutoCool = false;
    static unsigned long lastModeIncreaseTime = 0;
    static unsigned long lastControlSentTime = 0;
    static int last_mode = -1;

    // Vào chế độ auto nếu nhiệt độ vượt ngưỡng
    if (!Auto_cool && temp > temp_setpoint)
    {
        Auto_cool = true;
        mode = 1;
        couter = 0;
        lastModeIncreaseTime = millis();
        lastControlSentTime = 0;
        PLG_write_4(ID_control, ID_master, "level", "1");
        DEBUG_PRINTLN("PLG_FAN_LV_1");
        sen_lora_data_4();
        DEBUG_PRINTLN("BẮT ĐẦU CHẾ ĐỘ AUTO");
    }
    // Thoát auto nếu nhiệt độ hạ thấp hơn ngưỡng - hysteresis
    else if (Auto_cool && temp < temp_setpoint - 2.0)
    {
        Auto_cool = false;
        mode = 0;
        couter = 0;
        PLG_cooling_level_0(); // Tắt hết thiết bị
        DEBUG_PRINTLN("PLG_FAN_LV_0");
        last_mode = 0;
        lastControlSentTime = millis();
        DEBUG_PRINTLN("THOÁT CHẾ ĐỘ AUTO");
    }

    if (Auto_cool)
    {
        // 5 phút thì tăng cấp làm mát
        if (millis() - lastModeIncreaseTime >= 300000UL)
        {
            if (mode < 4)
            {
                mode++;
                DEBUG_PRINTF("TĂNG CẤP ĐỘ: %d\n", mode);
            }
            lastModeIncreaseTime = millis();
            esp_task_wdt_reset();
        }

        // Gửi điều khiển định kỳ mỗi 15 giây hoặc nếu mode vừa thay đổi
        if (mode != last_mode || millis() - lastControlSentTime >= 10000UL)
        {
            switch (mode)
            {
            case 0:
                PLG_cooling_level_0();
                break;
            case 1:
                PLG_cooling_level_1();
                break;
            case 2:
                PLG_cooling_level_2();
                break;
            case 3:
                PLG_cooling_level_3();
                break;
            case 4:
                PLG_cooling_level_4();
                break;
            default:
                break;
            }

            last_mode = mode;
            lastControlSentTime = millis();
            esp_task_wdt_reset();
        }
        esp_task_wdt_reset();
    }
}

void thucthilenh()
{
    if (address.startsWith(ID_master) && data_end.startsWith("E")&& address_slave.startsWith(ID_control))
    {
        TT_RELAY_QUATHUT = data1.toInt();
        TT_RELAY_DAOKHI = data2.toInt();
        TT_RELAY_CATNAG = data3.toInt();
        TT_RELAY_PHUNSUONG = data4.toInt();
        TT_RELAY_BOM = data5.toInt();
        TT_RELAY_BOMCCON = data6.toInt();
        TT_RELAY_DEN = data7.toInt();
        TT_RELAY_BONNUOC = data8.toInt();
        data_end="KHAC";
        Serial.println("----- TRANG THAI RELAY -----");
        Serial.print("QUAT HUT   : ");
        Serial.println(TT_RELAY_QUATHUT);
        Serial.print("DAO KHI    : ");
        Serial.println(TT_RELAY_DAOKHI);
        Serial.print("CAT NANG   : ");
        Serial.println(TT_RELAY_CATNAG);
        Serial.print("PHUN SUONG : ");
        Serial.println(TT_RELAY_PHUNSUONG);
        Serial.print("BOM        : ");
        Serial.println(TT_RELAY_BOM);
        Serial.print("BOM CON    : ");
        Serial.println(TT_RELAY_BOMCCON);
        Serial.print("DEN        : ");
        Serial.println(TT_RELAY_DEN);
        Serial.print("BON NUOC   : ");
        Serial.println(TT_RELAY_BONNUOC);
        Serial.println("----------------------------");

        // ----------- QUẠT HÚT -----------
        if (TT_RELAY_QUATHUT != last_quathut)
        {
            relay_modbus.writeSingleCoil(RELAY_QUATHUT, TT_RELAY_QUATHUT);
            last_quathut = TT_RELAY_QUATHUT;
            esp_task_wdt_reset();
        }

        // ----------- ĐẢO KHÍ -----------
        if (TT_RELAY_DAOKHI != last_daokhi)
        {
            relay_modbus.writeSingleCoil(RELAY_DAOKHI, TT_RELAY_DAOKHI);
            last_daokhi = TT_RELAY_DAOKHI;
            esp_task_wdt_reset();
        }

        // ----------- CẮT NẮNG -----------
        if (TT_RELAY_CATNAG != last_catnag)
        {
            relay_modbus.writeSingleCoil(RELAY_CATNAG, TT_RELAY_CATNAG);
            last_catnag = TT_RELAY_CATNAG;
            esp_task_wdt_reset();
        }

        // ----------- PHUN SƯƠNG -----------
        if (TT_RELAY_PHUNSUONG != last_phunsuong)
        {
            relay_modbus.writeSingleCoil(RELAY_PHUNSUONG, TT_RELAY_PHUNSUONG);
            last_phunsuong = TT_RELAY_PHUNSUONG;
            esp_task_wdt_reset();
        }

        // ----------- BƠM -----------
        if (TT_RELAY_BOM != last_bom)
        {
            relay_modbus.writeSingleCoil(RELAY_BOM, TT_RELAY_BOM);
            last_bom = TT_RELAY_BOM;
            esp_task_wdt_reset();
        }

        // ----------- BƠM CON -----------
        if (TT_RELAY_BOMCCON != last_bomccon)
        {
            relay_modbus.writeSingleCoil(RELAY_BOMCCON, TT_RELAY_BOMCCON);
            last_bomccon = TT_RELAY_BOMCCON;
            esp_task_wdt_reset();
        }

        // ----------- ĐÈN -----------
        if (TT_RELAY_DEN != last_den)
        {
            relay_modbus.writeSingleCoil(RELAY_DEN, TT_RELAY_DEN);
            last_den = TT_RELAY_DEN;
            esp_task_wdt_reset();
        }

        // ----------- BỒN NƯỚC -----------
        if (TT_RELAY_BONNUOC != last_bonnuoc)
        {
            relay_modbus.writeSingleCoil(RELAY_BONNUOC, TT_RELAY_BONNUOC);
            last_bonnuoc = TT_RELAY_BONNUOC;
            esp_task_wdt_reset();
        } // hut ,dao, cat,phun,bom,bomcc,den,auto
    }
    if (address_slave.startsWith(ID_control) || address_slave.startsWith("slave1")) // nhan du lieu dieu khien tu master va du lieu cam bien
    {

        if (address.startsWith(ID_control_DD))
        {
            if (namedata.startsWith("EC"))
            {
                ec_nuoc = data.toFloat();

                DEBUG_PRINT("EC nước: ");
                DEBUG_PRINTLN(ec_nuoc);
            }
            if (namedata.startsWith("PH"))
            {
                ph_nuoc = data.toFloat();

                // DEBUG_PRINT("pH nước: ");
                // DEBUG_PRINTLN(ph_nuoc);
            }
            PLG_write_board_sensor(ID_DD, ID_master, "ec_nuoc", String(ec_nuoc, 2), String(ph_nuoc, 2), "0", "0");
            LoRa.beginPacket(); // Start a new packet
            LoRa.print(messages_sensor);
            LoRa.endPacket();               // Finish the packet and send it
            digitalWrite(led_master, HIGH); // Turn off LED for slave status
            delay(20);                      // Delay to ensure the message is sent
            digitalWrite(led_master, LOW);  // Turn off LED for slave statusF
            PLG_write_board_sensor(ID_DD, ID_master, "ph_nuoc", String(ec_nuoc, 2), String(ph_nuoc, 2), "0", "0");
            LoRa.beginPacket(); // Start a new packet
            LoRa.print(messages_sensor);
            LoRa.endPacket();               // Finish the packet and send it
            digitalWrite(led_master, HIGH); // Turn off LED for slave status
            delay(20);                      // Delay to ensure the message is sent
            digitalWrite(led_master, LOW);  // Turn off LED for slave statusF
            esp_task_wdt_reset();
        }
    }
}
void loop_slave()
{
    int packetSize = LoRa.parsePacket();
    if (packetSize)
    {
        receivedData = "";
        while (LoRa.available())
        {
            receivedData += (char)LoRa.read();
        }
        DEBUG_PRINTLN(receivedData);
        PLG_check_message(); // Check the received data
        thucthilenh();       // Execute the command

        digitalWrite(led_connected, HIGH);
        delay(20);
        digitalWrite(led_connected, LOW);

        lastLoRaReceiveTime = millis(); // cập nhật thời gian nhận LoRa
        esp_task_wdt_reset();
    }

    if (Serial2.available())
    {
        receivedData = Serial2.readStringUntil('\n'); // Ví dụ nhận chuỗi
        PLG_check_message();                          // Gọi xử lý chuỗi
        thucthilenh();
        esp_task_wdt_reset(); // Execute the command
    }

    // Kiểm tra 5 giây không nhận LoRa thì reset
    if (millis() - lastLoRaReceiveTime > 5000)
    {
        ESP.restart();
    }
    esp_task_wdt_reset();
}

void PLG_slave_loop()
{
    loop_slave();
}
