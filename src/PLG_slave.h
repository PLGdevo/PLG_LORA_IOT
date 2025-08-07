
#include <PLG_datastring.h>
#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <string.h>
#include <hardwareSerial.h>

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
#define ID_relay "PLG_relay"
String ID_control = "0";
String ID_CB = "1";
String ID_ = "2";
String ID_master = "3";
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
float temp_setpoint = 40.0;
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
    pinMode(led_slave, OUTPUT);
    pinMode(led_master, OUTPUT);
    pinMode(led_connected, OUTPUT);
    digitalWrite(led_connected, LOW);
    digitalWrite(led_slave, HIGH);
    digitalWrite(led_master, LOW);
    DEBUG_PRINTLN("PLG RUNNING SLAVE");

    Serial2.begin(115200, SERIAL_8N1, RX, TX);
    PLG_write_4(ID_control, ID_master, ID_control, Auto_cool ? "auto" : "manual");
    sen_lora_data_4();
    PLG_write_4(ID_control, ID_master, ID_control, "reset");
    DEBUG_PRINTLN("PLG_board_relay_reset");
    sen_lora_data_4();
}
void PLG_cooling_level_0()
{
    PLG_write_4(ID_control, ID_relay, QUATHUT, "OFF");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_relay, CATNAG, "OFF");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_relay, PHUNSUONG, "OFF");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_relay, DAOKHI, "OFF");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_master, "level", "0");
    sen_lora_data_4();
    PLG_write_4(ID_control, ID_master, "PLG_relay", "update");
    DEBUG_PRINTLN("PLG_board_relay_update");
    sen_lora_data_4();
}
void PLG_cooling_level_1()
{
    PLG_write_4(ID_control, ID_relay, QUATHUT, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_master, "level", "1");
    sen_lora_data_4();
}
void PLG_cooling_level_2()
{
    PLG_write_4(ID_control, ID_relay, QUATHUT, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_relay, DAOKHI, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_master, "level", "2");
    sen_lora_data_4();
}
void PLG_cooling_level_3()
{
    PLG_write_4(ID_control, ID_relay, CATNAG, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_relay, QUATHUT, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_relay, DAOKHI, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_master, "level", "3");
    sen_lora_data_4();
}
void PLG_cooling_level_4()
{
    PLG_write_4(ID_control, ID_relay, PHUNSUONG, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_relay, CATNAG, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_relay, QUATHUT, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_relay, DAOKHI, "ON");
    SEN_PRINTLN(messages4);
    PLG_write_4(ID_control, ID_master, "level", "4");
    sen_lora_data_4();
}
void PLG_cooling_level_5()
{
    PLG_write_4(ID_control, ID_master, "level", "5");
    sen_lora_data_4();
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
        PLG_write_4(ID_control, ID_master, "level", "0");
        DEBUG_PRINTLN("PLG_FAN_LV_0");
        sen_lora_data_4();
        last_mode = 0;
        lastControlSentTime = millis();
        DEBUG_PRINTLN("THOÁT CHẾ ĐỘ AUTO");
    }

    if (Auto_cool)
    {
        // 3 phút thì tăng cấp làm mát
        if (millis() - lastModeIncreaseTime >= 30000)
        {
            if (mode < 5)
            {
                mode++;
                DEBUG_PRINTF("TĂNG CẤP ĐỘ: %d\n", mode);
                PLG_write_4(ID_master, ID_control, "level", String(mode));
                sen_lora_data_4();
            }
            lastModeIncreaseTime = millis();
        }

        // Gửi điều khiển định kỳ mỗi 15 giây hoặc nếu mode vừa thay đổi
        if (mode != last_mode || millis() - lastControlSentTime >= 2000)
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
            case 5:
                PLG_cooling_level_5();
                break;
            default:
                break;
            }

            last_mode = mode;
            lastControlSentTime = millis();
        }
    }
}

void thucthilenh()
{
    if (address_slave.startsWith(ID_control) || address_slave.startsWith("slave1")) // nhan du lieu dieu khien tu master va du lieu cam bien
    {
        // Map tên thiết bị sang tên hiển thị debug
        struct Device
        {
            const char *name;
            const char *debugName;
        };

        const Device devices[] = {
            {BOM, "bom"},
            {DAOKHI, "quat"},
            {QUATHUT, "quat hut"},
            {CATNAG, "cat nang"},
            {PHUNSUONG, "phunsuong"},
            {BOMCCON, "bomccon"},
            {DEN, "den"}};

        bool commandHandled = false;

        for (const auto &device : devices)
        {
            if (namedata.startsWith(device.name))
            {
                if (data.startsWith("ok"))
                {
                    // DEBUG_PRINTF("[ON]%s\n ", device.name);
                    PLG_write_4(ID_control, ID_relay, device.name, "ON");
                }
                else if (data.startsWith("not ok"))
                {
                    // DEBUG_PRINTF("[OFF]%s\n ", device.name);
                    PLG_write_4(ID_control, ID_relay, device.name, "OFF");
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
                PLG_write_4(ID_control, "PLG_relay", "requet", "run");
                SEN_PRINTLN(messages4);
                commandHandled = true;
            }
            else if (namedata.startsWith("all_CB"))
            {
                temp = data1.toFloat();
                commandHandled = true;
            }
        }
        if (namedata.startsWith("datarun") && data.startsWith("running"))
        {
            PLG_write_4(ID_control, ID_master, "slave1", "reset");
            DEBUG_PRINTLN("PLG_board_relay_reset");
            sen_lora_data_4();
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
        PLG_check_message(); // Check the received data
        thucthilenh();       // Execute the command
        digitalWrite(led_connected, HIGH);
        delay(20);
        digitalWrite(led_connected, LOW);
    }

    if (Serial2.available())
    {
        receivedData = Serial2.readStringUntil('\n'); // Ví dụ nhận chuỗi
        PLG_check_message();                          // Gọi xử lý chuỗi
        thucthilenh();                                // Execute the command
    }

    PLG_AIR_AUTO();

    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 2345)
    {
        if (temp >= temp_setpoint)
        {
            PLG_write_4(ID_control, ID_master, ID_control, "auto");
            sen_lora_data_4();
            DEBUG_PRINTLN(Auto_cool ? "PLG_board_relay_update-auto" : "PLG_board_relay_update_manual");
            // PLG_write_4(ID_control, ID_master, "level",String(mode,2));
            // sen_lora_data_4();
        }
        else if (temp <= temp_setpoint - 2)
        {
            PLG_write_4(ID_control, ID_master, ID_control, "manual");
            sen_lora_data_4();
            DEBUG_PRINTLN(Auto_cool ? "PLG_board_relay_update-auto" : "PLG_board_relay_update_manual");
            // PLG_write_4(ID_control, ID_master, "level",String(mode,2));
            // sen_lora_data_4();
        }

        lastTime = currentTime;
    }
}

void PLG_slave_loop()
{
    loop_slave();
}
