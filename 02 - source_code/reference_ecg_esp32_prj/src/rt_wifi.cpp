#include "rt_wifi.h"

WebServer server(80);
const char *AP_SSID = "ESP32_Config"; //热点名称
String wifi_ssid = "";
String wifi_pass = "";
String scanNetworksID = ""; //用于储存扫描到的WiFi
// 以下为OTA网页文件
String indexhtml = String("") +
                   "<!DOCTYPE html>\n" +
                   "<head>\n" +
                   "    <meta charset=\"UTF-8\">\n" +
                   "    <title>Update Test</title>\n" +
                   "</head>\n" +
                   "<body>\n" +
                   "    <form method=\'POST\' action=\'/update\' enctype=\'multipart/form-data\'>\n" +
                   "        <input type=\'file\' name=\'firmware\'>\n" +
                   "        <input type=\'submit\'>\n" +
                   "    </form>\n" +
                   "</body>\n";

bool shouldreboot = false; // 重启标志，固件升级操作完成后设置该标志准备重启设备

void handleResponse() //回调函数
{
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
}

void handleFileupload() //回调函数
{
    HTTPUpload &upload = server.upload();   // 文件上传对象
    if (upload.status == UPLOAD_FILE_START) // 文件上传开始
    {
        Serial.printf("Start uploading files %s\n", upload.filename.c_str());
        if (!Update.begin()) // 开始固件升级，检查空可用间大小，如果正在升级或空间不够则返回false
        {
            Update.printError(Serial);
        }
    }
    else if (upload.status == UPLOAD_FILE_WRITE) // 文件读写中
    {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) // 将文件数据写入到OTA区域
        {
            Update.printError(Serial);
        }
    }
    else if (upload.status == UPLOAD_FILE_END) // 文件上传完成
    {
        Serial.println("write in " + String(upload.totalSize) + " byte to OTA area");
        if (!Update.end(true)) // 完成数据写入，设置在系统重启后自动将OTA区域固件移动到Sketch区域 // Update.begin不指定大小时这里设置true
        {
            Update.printError(Serial);
        }
        Serial.println("After the upgrade operation is completed, the module will restart in 5 seconds to complete the firmware upgrade");
        shouldreboot = true;
    }
    else
    {
        Serial.printf("Firmware upload failed: status=%d\n", upload.status);
    }
}

//用于配置WiFi
void wifi_Config()
{
    Serial.println("scan start");
    // 扫描附近WiFi
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
    {
        Serial.println("no networks found");
        scanNetworksID = "no networks found";
    }
    else
    {
        
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            scanNetworksID += "<P>" + WiFi.SSID(i) + "</P>";
            delay(10);
        }
    }
    Serial.println("");

    WiFi.mode(WIFI_AP);                        //配置为AP模式
    boolean result = WiFi.softAP(AP_SSID, ""); //开启WIFI热点
    if (result)
    {
        IPAddress myIP = WiFi.softAPIP();
        //打印相关信息
        Serial.println("");
        Serial.print("Soft-AP IP address = ");
        Serial.println(myIP);
        Serial.println(String("MAC address = ") + WiFi.softAPmacAddress().c_str());
        Serial.println("waiting ...");
    }
    else
    { //开启热点失败
        Serial.println("WiFiAP Failed");
        delay(3000);
        ESP.restart(); //复位esp32
    }

    if (MDNS.begin("esp32"))
    {
        Serial.println("MDNS responder started");
    }

    //首页
    server.on("/", []()
              { server.send(200, "text/html", ROOT_HTML + scanNetworksID + "</body></html>"); });

    //连接
    server.on("/connect", []()
              {
                  server.send(200, "text/html", "<html><body><font size=\"10\">successd,wifi connecting...<br />Please close this page manually.</font></body></html>");

                  WiFi.softAPdisconnect(true);
                  //获取输入的WIFI账户和密码
                  wifi_ssid = server.arg("ssid");
                  wifi_pass = server.arg("pass");
                  server.close();
                  WiFi.softAPdisconnect();
                  Serial.println("WiFi Connect SSID:" + wifi_ssid + "  PASS:" + wifi_pass);

                  //设置为STA模式并连接WIFI
                  WiFi.mode(WIFI_STA);
                  WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());
                  uint8_t Connect_time = 0; //用于连接计时，如果长时间连接不成功，复位设备
                  while (WiFi.status() != WL_CONNECTED)
                  { //等待WIFI连接成功
                      delay(500);
                      Serial.print(".");
                      Connect_time++;
                      if (Connect_time > 80)
                      { //长时间连接不上，复位设备
                          Serial.println("Connection timeout, check input is correct or try again later!");
                          delay(3000);
                          ESP.restart();
                      }
                  }
                  Serial.println("");
                  Serial.println("WIFI Config Success");
                  Serial.printf("SSID:%s", WiFi.SSID().c_str());
                  Serial.print("  LocalIP:");
                  Serial.print(WiFi.localIP());
                  Serial.println("");
                  
              });
    server.on("/ota", HTTP_GET, []()
              {
                  server.sendHeader("Connection", "close");
                  server.send(200, "text/html", indexhtml); // 发送网页
              });

    server.on("/update", HTTP_POST, handleResponse, handleFileupload); // 绑定回调函数

    server.begin();
}

//用于上电自动连接WiFi
bool AutoConfig()
{
    // WiFi.begin();
    for (int i = 0; i < 30; i++)
    {
        int wstatus = WiFi.status();
        if (wstatus == WL_CONNECTED)
        {
            Serial.println("WIFI SmartConfig Success");
            Serial.printf("SSID:%s", WiFi.SSID().c_str());
            Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
            Serial.print("LocalIP:");
            Serial.print(WiFi.localIP());
            Serial.print(" ,GateIP:");
            Serial.println(WiFi.gatewayIP());
            return true;
        }
        else
        {
            Serial.print("WIFI AutoConfig Waiting......");
            Serial.println(wstatus);
            delay(1000);
        }
    }
    Serial.println("WIFI AutoConfig Faild!");
    return false;
}
//用于删除已存WiFi
// if (digitalRead(RESET_PIN) == LOW)
// {
//     delay(1000);
//     esp_wifi_restore();
//     delay(10);
//     ESP.restart(); //复位esp32
// }
