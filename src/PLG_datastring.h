/* ---------------------------------------------------------------------
 * PLG define cac mode doc chuoi du lieu\--------------------------*/
#include <Arduino.h>
#include <PLG_debug.hpp>
#include <string.h>

String address = "";
String address_slave = ""; // Address of the slave device
String address_remus = "";
String namedata = "";
String data = ""; 
String data1 = "";//temp
String data2 = "";//hum
String data3 = "";//lux
String data4 = "";//ph_dat

String data_Qhut = "";
String data_Qdao = "";
String data_Ct_NG = "";
String data_PS = "";
String data_B = "";
String data_BCC = "";
String data_DEN = "";
String data_BON_N = "";
String data_Run_DD = "";
String data_EC_nuoc = "";
String data_PH_nuoc = "";
String data_end = "E";
String receivedData = "";
// mes_nha 2 = // hut , dao, cat,phun,  bom,    bomcc,  den,    bon nuoc ,  run_dd , ec, ph
String messages_nha2 = address + "|" + address_slave + "|" + data_Qhut + "|" + data_Qdao + "|" + data_Ct_NG + "|" + data_PS + "|" + data_B + "|" + data_BCC + "|" + data_DEN +"|" + data_BON_N + "|" + data_Run_DD + "|" + data_EC_nuoc + "|" + data_PH_nuoc + "|" + data_end;
String messages4 = address + "|" + address_slave + "|" + namedata + "|" + data;
String messages5 = address + "|" + address_slave + "|" + address_remus + "|" + namedata + "|" + data;
String messages_sensor = address + "|" + address_slave + "|" + namedata + "|" + data1 + "|" + data2 + "|" + data3 + "|" + data4;
String messages_7 = address + "|" + address_slave + "|" + namedata + "|" + data1 + "|" + data2 + "|" + data3 + "|" + data4;
/*                  dia chi gui | dia chi nhan | ten du lieu | du lieu
                    dc su dung cho 2 module                                 */
void PLG_write_4(String address, String address_slave, String namedata, String data)
{
    messages4 = address + "|" + address_slave + "|" + namedata + "|" + data;
}
void PLG_write_board_sensor(String address, String address_slave, String namedata, String data1, String data2, String data3, String data4)
{
    messages_sensor = address + "|" + address_slave + "|" + namedata + "|" + data1 + "|" + data2 + "|" + data3 + "|" + data4;
}
void PLG_write_7(String address, String address_slave, String namedata, String data1, String data2, String data3, String data4)
{
    messages_7 = address + "|" + address_slave + "|" + namedata + "|" + data1 + "|" + data2 + "|" + data3 + "|" + data4;
}
/*         dia chi gui | dia chi trung gian |dia chi cuoi | ten du lieu | du lieu
                        su dung cho 3 module                                */
void PLG_write_5(String address, String address_slave, String address_remus, String namedata, String data)
{
    messages5 = address + "|" + address_slave + "|" + address_remus + "|" + namedata + "|" + data;
    // DEBUG_PRINT("Sending packet: ");
    // DEBUG_PRINTLN(messages5);
}
void PLG_write_11(String address, String address_slave, String data1, String data2, String data3, String data4, String data5, String data6, String data7, String data8)
{
    messages_nha2 = address + "|" + address_slave + "|" + data1 + "|" + data2 + "|" + data3 + "|" + data4 + "|" + data5 + "|" + data6 + "|" + data7 + "|" + data8 + "|" + data_end;
    // DEBUG_PRINT("Sending packet: ");
    // DEBUG_PRINTLN(messages11);
}
void PLG_write_nha_2(String address, String address_slave, String data_Qhut, String data_Qdao, String data_Ct_NG, String data_PS, String data_B, String data_BCC, String data_DEN,String data_BON_N, String data_Run_DD, String data_EC_nuoc, String data_PH_nuoc)
{
    messages_nha2 = address + "|" + address_slave + "|" + data_Qhut + "|" + data_Qdao + "|" + data_Ct_NG + "|" + data_PS + "|" + data_B + "|" + data_BCC + "|" + data_DEN + "|" + data_BON_N + "|" + data_Run_DD + "|" + data_EC_nuoc + "|" + data_PH_nuoc + "|" + data_end;
}
void PLG_read_4()
{
    int index1 = receivedData.indexOf('|');
    int index2 = receivedData.indexOf('|', index1 + 1);
    int index3 = receivedData.indexOf('|', index2 + 1);

    if (index1 == -1 || index2 == -1 || index3 == -1)
    {
        DEBUG_PRINTLN("Chuỗi không hợp lệ!");
        return;
    }

    address = receivedData.substring(0, index1);
    address_slave = receivedData.substring(index1 + 1, index2);
    namedata = receivedData.substring(index2 + 1, index3);
    data = receivedData.substring(index3 + 1);
    messages4 = address + "|" + address_slave + "|" + namedata + "|" + data;
    // DEBUG_PRINT("Sending packet: ");
    // DEBUG_PRINTLN(messages4);
}
void PLG_read_5()
{
    int i1 = receivedData.indexOf('|');
    int i2 = receivedData.indexOf('|', i1 + 1);
    int i3 = receivedData.indexOf('|', i2 + 1);
    int i4 = receivedData.indexOf('|', i3 + 1);

    if (i1 == -1 || i2 == -1 || i3 == -1 || i4 == -1)
    {
        Serial.println("Chuỗi không hợp lệ!");
        return;
    }
    address = receivedData.substring(0, i1);
    address_slave = receivedData.substring(i1 + 1, i2);
    address_remus = receivedData.substring(i2 + 1, i3);
    namedata = receivedData.substring(i3 + 1, i4);
    data = receivedData.substring(i4 + 1);
    messages5 = address + "|" + address_slave + "|" + address_remus + "|" + namedata + "|" + data;
    // DEBUG_PRINT("Sending packet: ");
    // DEBUG_PRINTLN(messages5);
}
void PLG_read_7()
{
    int i1 = receivedData.indexOf('|');
    int i2 = receivedData.indexOf('|', i1 + 1);
    int i3 = receivedData.indexOf('|', i2 + 1);
    int i4 = receivedData.indexOf('|', i3 + 1);
    int i5 = receivedData.indexOf('|', i4 + 1);
    int i6 = receivedData.indexOf('|', i5 + 1);

    if (i1 == -1 || i2 == -1 || i3 == -1 || i4 == -1 || i5 == -1 || i6 == -1)
    {
        Serial.println("Chuỗi không hợp lệ!");
        return;
    }

    address = receivedData.substring(0, i1);
    address_slave = receivedData.substring(i1 + 1, i2);
    namedata = receivedData.substring(i2 + 1, i3);
    data1 = receivedData.substring(i3 + 1, i4);
    data2 = receivedData.substring(i4 + 1, i5);
    data3 = receivedData.substring(i5 + 1, i6);
    data4 = receivedData.substring(i6 + 1);
    messages_sensor = address + "|" + address_slave + "|" + namedata + "|" + data1 + "|" + data2 + "|" + data3 + "|" + data4;
    // DEBUG_PRINTLN(messages_sensor);
}
void PLG_read_nha_2()
{
    int i1 = receivedData.indexOf('|');
    int i2 = receivedData.indexOf('|', i1 + 1);
    int i3 = receivedData.indexOf('|', i2 + 1);
    int i4 = receivedData.indexOf('|', i3 + 1);
    int i5 = receivedData.indexOf('|', i4 + 1);
    int i6 = receivedData.indexOf('|', i5 + 1);
    int i7 = receivedData.indexOf('|', i6 + 1);
    int i8 = receivedData.indexOf('|', i7 + 1);
    int i9 = receivedData.indexOf('|', i8 + 1);
    int i10 = receivedData.indexOf('|', i9 + 1);
    int i11 = receivedData.indexOf('|', i10 + 1);
    int i12 = receivedData.indexOf('|', i11 + 1);
    int i13 = receivedData.indexOf('|', i12 + 1);
    int i14 = receivedData.indexOf('|', i13 + 1);
    if (i1 == -1 || i2 == -1 || i3 == -1 || i4 == -1 || i5 == -1 || i6 == -1 || i7 == -1 || i8 == -1 || i9 == -1 || i10 == -1 || i11 == -1 || i12 == -1||i13==-1)
    {
        Serial.println("Chuỗi không hợp lệ!");
        return;
    }

    address = receivedData.substring(0, i1);
    address_slave = receivedData.substring(i1 + 1, i2);    
    data_Qhut = receivedData.substring(i2 + 1, i3);
    data_Qdao = receivedData.substring(i3 + 1, i4);
    data_Ct_NG = receivedData.substring(i4 + 1, i5);
    data_PS = receivedData.substring(i5 + 1, i6);
    data_B = receivedData.substring(i6 + 1, i7);
    data_BCC = receivedData.substring(i7 + 1, i8);
    data_DEN = receivedData.substring(i8 + 1, i9);
    data_BON_N = receivedData.substring(i9 + 1, i10);
    data_Run_DD = receivedData.substring(i10 + 1,i11);
    data_EC_nuoc = receivedData.substring(i11 + 1,i12);
    data_PH_nuoc = receivedData.substring(i12 + 1,i13);
    data_end= receivedData.substring(i13+1);

     messages_nha2 = address + "|" + address_slave + "|" + data_Qhut + "|" + data_Qdao + "|" + data_Ct_NG + "|" + data_PS + "|" + data_B + "|" + data_BCC + "|" + data_DEN +"|" + data_BON_N + "|" + data_Run_DD + "|" + data_EC_nuoc + "|" + data_PH_nuoc + "|" + data_end;
    
}
// Hàm đếm số dấu '|' trong chuỗi
int PLG_count_separator(String str)
{
    int count = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (str.charAt(i) == '|')
        {
            count++;
        }
    }
    return count;
}

// Hàm kiểm tra chuỗi 4 hay 5 trường
void PLG_check_message()
{
    int soDauPhanCach = PLG_count_separator(receivedData);
    // DEBUG_PRINTLN(soDauPhanCach);
    if (soDauPhanCach == 3)
    {
        // DEBUG_PRINTLN("Chuỗi có 4 trường dữ liệu.");
        PLG_read_4();
    }
    else if (soDauPhanCach == 4)
    {
        // DEBUG_PRINTLN("Chuỗi có 5 trường dữ liệu.");
        PLG_read_5();
    }
    else if (soDauPhanCach == 6)
    {
        // DEBUG_PRINTLN("Chuỗi có 7 trường dữ liệu.");
        PLG_read_7();
    }
    else if (soDauPhanCach == 13)
    {
        // DEBUG_PRINTLN("chuoi co 11 du lieu");
        PLG_read_nha_2();
    }

    else
    {
        DEBUG_PRINT("Lỗi: Chuỗi không hợp lệ. Dấu '|': ");
        DEBUG_PRINTLN(soDauPhanCach);
    }
}

/* ban chi can goi ham nay ra la moi viec se dc giai quyet
if (Serial2.available())
    {
        receivedData = Serial2.readStringUntil('\n'); // Ví dụ nhận chuỗi
        PLG_check_message();                          // Gọi xử lý chuỗi
        thucthilenh(); // Execute the command
    }
*/
// các hàm trên dùng dể đọc chuỗi dữ liệu từ LoRa hoặc RS485
