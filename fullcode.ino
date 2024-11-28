#define gassensor 34
#define smokesensor 35
#include <DHT.h> 
#define DHTPIN 32
#define DHTTYPE DHT22 
DHT dht22(DHTPIN, DHTTYPE); 
int temperature ;
int humidity ;
#include <HTTPClient.h>

#define RELAY_PIN    17 // ESP32 pin GPIO17 that connects to relay
#define MOISTURE_PIN 36 // ESP32 pin GPIO36 (ADC0) that connects to AOUT pin of moisture sensor
#define THRESHOLD 2800   // => CHANGE YOUR THRESHOLD HERE

#include <WiFi.h>
const char* ssid = "Nokia C32"; 
const char* password = "12345678"; 

String BUTTONPAGE="http://192.168.126.190//iot2/button1.php";

String temppage="http://192.168.126.190//iot2/temp.php";

String humpage="http://192.168.126.190//iot2/hum.php";

String gaspage="http://192.168.126.190//iot2/gas.php";

String smokepage="http://192.168.126.190//iot2/smokepage.php";

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
 connectWiFi();
 dht22.begin(); 
}

void loop() {
if(WiFi.status() != WL_CONNECTED) { 
   connectWiFi();
 }


gas();
smoke();
buttonpro();
Load_DHT22_Data();
soil();

delay(3000);
}


void gas()
{
int gas1 = analogRead(gassensor);
Serial.print("Gas -");
Serial.println(gas1);
if(gas1>2100)
{
String gas = String(gas1);
String postData2 =  "gas=" + gas;
  

  HTTPClient http2;
  http2.begin(gaspage);
  http2.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode2 = http2.POST(postData2); 
  String payload2 = http2.getString(); 


  Serial.print("httpCode: "); Serial.println(httpCode2); 
}
}

void smoke(){
int digi=analogRead(smokesensor);
Serial.print("Smoke -");
Serial.println(digi);
if(digi>3500)
{
String smoke = String(digi);
String postData3 =  "smoke=" + smoke;
  
  HTTPClient http3;
  http3.begin(smokepage);
 
  int httpCode3 = http3.POST(postData3); 
  String payload3 = http3.getString(); 


  Serial.print("httpCode: "); Serial.println(httpCode3); 
}
}

void Load_DHT22_Data() {
  //-----------------------------------------------------------
  temperature = dht22.readTemperature();
  humidity = dht22.readHumidity();
  delay(1000);
  //-----------------------------------------------------------
  // Check if any reads failed.
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = 0;
    humidity = 0;
  }
  //-----------------------------------------------------------
  Serial.printf("Temperature: %d °C\n", temperature);
  Serial.printf("Humidity: %d %%\n", humidity);

String temp = String(temperature);
String postData =  "temp=" + temp;
  
  HTTPClient http;
  http.begin(temppage);

  int httpCode = http.POST(postData); 
  String payload = http.getString(); 

  Serial.print("httpCode: "); Serial.println(httpCode); 

String hum = String(humidity);
String postData1 =  "hum=" + hum;
  
  HTTPClient http1;
  http1.begin(humpage);
 
  int httpCode1 = http1.POST(postData1); 
  String payload1 = http1.getString(); 

  Serial.print("httpCode: "); Serial.println(httpCode1); 

}


void buttonpro(){
int buttonState = digitalRead(15);
if(buttonState==0)
{

String buttsat = String(buttonState);
String postData = "butstat=" + buttsat;
  
  HTTPClient http;
  http.begin(BUTTONPAGE);
 

  int httpCode = http.POST(postData); 
  String payload = http.getString(); 


  Serial.print("httpCode: "); Serial.println(httpCode); 

}
}

//wifi connecting function
void connectWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  //This line hides the viewing of ESP as wifi hotspot
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

void soil()
{
  int value = analogRead(MOISTURE_PIN); // read the analog value from soild moisture sensor

  if (value > THRESHOLD) {
     Serial.print("The soil moisture is WET=> deactivate the pump");
    digitalWrite(RELAY_PIN, HIGH);
  } else {
  Serial.print("The soil moisture is DRY => activate pump");
    digitalWrite(RELAY_PIN, LOW);
}

  Serial.print(" (");
  Serial.print(value);
  Serial.println(")");

  delay(1000);

}
