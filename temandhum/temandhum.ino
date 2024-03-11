

#include <WiFi.h>
#include <HTTPClient.h>

#include <DHT.h> 
#define DHTPIN 23
#define DHTTYPE DHT22 
DHT dht22(DHTPIN, DHTTYPE); 

int soilvalue;


String URL = "http://192.168.1.158/iot/connction.php";

const char* ssid = "SLT FIBRE"; 
const char* password = "rohan1964"; 

int temperature ;
int humidity ;

void setup() {
  Serial.begin(115200);

   connectWiFi();

   dht22.begin(); 
}
 
  
  


void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }
 soilsen();
  Load_DHT22_Data();

 String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity) + "&soil=" + String(soilvalue);

  
  HTTPClient http;
  http.begin(URL);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  int httpCode = http.POST(postData);
  String payload = http.getString();

  Serial.print("URL : "); Serial.println(URL); 
  Serial.print("Data: "); Serial.println(postData);
  Serial.print("httpCode: "); Serial.println(httpCode);
  Serial.print("payload : "); Serial.println(payload);
  Serial.println("--------------------------------------------------");
  delay(1000);
}



void Load_DHT22_Data() {
  //-----------------------------------------------------------
  temperature = dht22.readTemperature();
  humidity = dht22.readHumidity();
  delay(10000);
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
}

void connectWiFi() {
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    
  Serial.print("connected to : "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

void soilsen() {
    soilvalue = analogRead(19);
    Serial.print("Raw Soil Value: ");
    Serial.println(soilvalue);

   

    soilvalue = constrain(map(soilvalue, 1000, 4095, 100, 0), 0, 100);
    Serial.print("Mapped and Constrained Soil Value: ");
    Serial.println(soilvalue);
}

