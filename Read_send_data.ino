#include <FirebaseESP8266.h>
#include <ESP8266Wifi.h>
const char* ssid = "Quynh Tien";
const char* password = "hieu2211";
FirebaseData dulieufirebase;
String a;
int b;
float c;
int LED = 2; // GPIO16 (D0)
unsigned long startTime;
unsigned long endTime;
unsigned long dataReceived = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 1000;  // Cập nhật băng thông mỗi giây (1000ms)
float dataRateReceived;

float voltage;
float bat_percentage;
int analogInPin  = A0;    // Analog input pin
int sensorValue;

void setup() 
{
  Serial.begin(115200);
  ketnoiwifi();
  Firebase.begin("ledcontrol-122a1-default-rtdb.asia-southeast1.firebasedatabase.app","N2VrFZrTzu8UN8OMJ7uP1QNfUaWWOU64fSxijpLG");
  Firebase.setString(dulieufirebase,"kieustring","hello");
  Firebase.setInt(dulieufirebase,"kieuint",2001);
  Firebase.setFloat(dulieufirebase,"kieufloat",0);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() 
{
  startTime = millis();
  Firebase.getString(dulieufirebase,"kieustring");
  a = dulieufirebase.stringData();
  // Firebase.getInt(dulieufirebase,"kieuint");
  // b = dulieufirebase.intData();
  // Firebase.getFloat(dulieufirebase,"kieufloat");
  // c = dulieufirebase.floatData();
  endTime = millis();
  Serial.print("a = ");
  Serial.print(a);
  Serial.print("\t");
  Serial.print("time = ");
  Serial.print(endTime - startTime);
  Serial.print(" ms\t");
  dataReceived += a.length();// + sizeof(b) + sizeof(c);
  
  Firebase.setInt(dulieufirebase,"kieuint",endTime - startTime);

  sensorValue = analogRead(analogInPin);
  voltage = (((sensorValue * 3.3) / 1024) * 2); //multiply by two as voltage divider network is 100K & 100K Resistor
  bat_percentage = map(voltage, 0, 4.2, 0, 100); //2.8V as Battery Cut off Voltage & 4.2V as Maximum Voltage
  if (bat_percentage >= 100)
  {
    bat_percentage = 100;
  }
  if (bat_percentage <= 0)
  {
    bat_percentage = 1;
  }
  Serial.print("Output Voltage = ");
  Serial.print(voltage);
  Serial.print("\tBattery Percentage = ");
  Serial.print(bat_percentage);

  Firebase.setFloat(dulieufirebase,"kieufloat",bat_percentage);

  if(a == "Stop") digitalWrite(LED, HIGH); // Turn LED OFF
  else digitalWrite(LED, LOW);// Turn LED OFF

  // Tính băng thông
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    dataRateReceived = (float)dataReceived / ((currentMillis - previousMillis) / 1000);  // Băng thông nhận (KB/s)
  }
    Serial.print("\tData Rate Received: ");
    Serial.print(dataRateReceived);
    Serial.println(" B/s");
  if (currentMillis - previousMillis >= interval) {
    // Đặt lại biến cho lần tính băng thông tiếp theo
    dataReceived = 0;
    previousMillis = currentMillis;
  }
  Firebase.setInt(dulieufirebase,"rate",dataRateReceived);
}

void ketnoiwifi()
{
  Serial.print("Bat dau ket noi wifi ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while (!(WiFi.status() == WL_CONNECTED))
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Ket noi thanh cong");
}
