
#include <PLG_datastring.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <string.h>
#include <ModbusMaster.h>
#include <hardwareSerial.h>
#include <esp_task_wdt.h>
#define WDT_time 1

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
    // PLG_write_4(ID_control, ID_relay, QUATHUT, "ON");
    // SEN_PRINTLN(messages4);
    // relay_modbus.writeSingleCoil(RELAY_PHUNSUONG,1);
    // relay_modbus.writeSingleCoil(RELAY_CATNAG,1);
    relay_modbus.writeSingleCoil(RELAY_QUATHUT, 1);
    // relay_modbus.writeSingleCoil(RELAY_DAOKHI,1);
    PLG_write_4(ID_control, ID_master, "level", "1");
    sen_lora_data_4();
    esp_task_wdt_reset();
}
void PLG_cooling_level_2()
{
    // PLG_write_4(ID_control, ID_relay, QUATHUT, "ON");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_relay, DAOKHI, "ON");
    // SEN_PRINTLN(messages4);
    // relay_modbus.writeSingleCoil(RELAY_PHUNSUONG,1);
    // relay_modbus.writeSingleCoil(RELAY_CATNAG,1);
    relay_modbus.writeSingleCoil(RELAY_QUATHUT, 1);
    relay_modbus.writeSingleCoil(RELAY_DAOKHI, 1);
    PLG_write_4(ID_control, ID_master, "level", "2");
    sen_lora_data_4();
    esp_task_wdt_reset();
}
void PLG_cooling_level_3()
{
    // PLG_write_4(ID_control, ID_relay, CATNAG, "ON");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_relay, QUATHUT, "ON");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_relay, DAOKHI, "ON");
    // SEN_PRINTLN(messages4);
    // relay_modbus.writeSingleCoil(RELAY_PHUNSUONG,1);
    relay_modbus.writeSingleCoil(RELAY_CATNAG, 1);
    relay_modbus.writeSingleCoil(RELAY_QUATHUT, 1);
    relay_modbus.writeSingleCoil(RELAY_DAOKHI, 1);
    PLG_write_4(ID_control, ID_master, "level", "3");
    sen_lora_data_4();
    esp_task_wdt_reset();
}
void PLG_cooling_level_4()
{
    // PLG_write_4(ID_control, ID_relay, PHUNSUONG, "ON");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_relay, CATNAG, "ON");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_relay, QUATHUT, "ON");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_relay, DAOKHI, "ON");
    // SEN_PRINTLN(messages4);
    // PLG_write_4(ID_control, ID_master, "level", "4");
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
    /*// if (address_slave.startsWith(ID_control) || address_slave.startsWith("slave1")) // nhan du lieu dieu khien tu master va du lieu cam bien
     // {
     //     // Map tên thiết bị sang tên hiển thị debug
     //     struct Device
     //     {
     //         const char *name;
     //         const char *debugName;
     //         int thanhghi;
     //     };

     //     const Device devices[] = {
     //         {BOM, "bom", RELAY_BOM},
     //         {DAOKHI, "quat", RELAY_DAOKHI},
     //         {QUATHUT, "quat hut", RELAY_QUATHUT},
     //         {CATNAG, "cat nang", RELAY_CATNAG},
     //         {PHUNSUONG, "phunsuong", RELAY_PHUNSUONG},
     //         {BOMCCON, "bomccon", RELAY_BOMCCON},
     //         {DEN, "den", RELAY_DEN},
     //         {BONNUOC, "bonnuoc", RELAY_BONNUOC}};

     //     bool commandHandled = false;

     //     for (const auto &device : devices)
     //     {
     //         if (namedata.startsWith(device.name))
     //         {
     //             if (data.startsWith("ok"))
     //             {
     //                 // DEBUG_PRINTF("[ON]%s\n ", device.name);
     //                 // PLG_write_4(ID_control, ID_relay, device.name, "ON");
     //                 relay_modbus.writeSingleCoil(device.thanhghi, 1);
     //             }
     //             else if (data.startsWith("not ok"))
     //             {
     //                 // DEBUG_PRINTF("[OFF]%s\n ", device.name);
     //                 // PLG_write_4(ID_control, ID_relay, device.name, "OFF");
     //                 relay_modbus.writeSingleCoil(device.thanhghi, 0);
     //             }
     //             SEN_PRINTLN(messages4);
     //             commandHandled = true;
     //             esp_task_wdt_reset();
     //             break;
     //         }
     // }

     // Xử lý các lệnh đặc biệt khác không dùng chung mô-típ
     // if (!commandHandled)
     // {
     //     if (namedata.startsWith("wifi") && data.startsWith("connected"))
     //     {
     //         PLG_write_4(ID_control, "PLG_relay", "requet", "run");
     //         SEN_PRINTLN(messages4);
     //         commandHandled = true;
     //     }*/
    if (namedata.startsWith("all_CB"))
    {
        temp = data1.toFloat();
    }
    if (address.startsWith(ID_master) && namedata.startsWith(auto))

    {
        if (data.startsWith("ok"))
        {
            chaytudong = 1;
        }
        else if (data.startsWith("not ok"))
        {
            chaytudong = 0;
        }
    }

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
    if (chaytudong)
    {
        PLG_AIR_AUTO();
    }
    esp_task_wdt_reset();
}

void PLG_slave_loop()
{
    loop_slave();
}
