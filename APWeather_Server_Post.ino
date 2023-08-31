#include <WiFiNINA.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include "Adafruit_CCS811.h"

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_CCS811 ccs;
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

#define sensorPin A5

char ssid[] = "Amped_REC22A_2.4";
char pass[] = "ce224699";

int status = WL_IDLE_STATUS;

char server[] = "pcr.bounceme.net";

String postData;
String postVariable = "temp=";

String postData1;
String postVariable1 = "ppm=";

String postData2;
String postVariable2 = "pressure=";

String postData3;
String postVariable3 = "tvoc=";



WiFiClient client;

void setup() {
  Serial.begin(9600);
  unsigned status_sensor;
  
  while (!Serial) {
     delay(100); // wait for serial port to connect. Needed for native USB port only
  }
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  IPAddress gateway = WiFi.gatewayIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  status_sensor = bmp.begin();
  if (!status_sensor) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  }
  Serial.println("CCS811 test");

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  // Wait for the sensor to be ready
  while(!ccs.available());
}

void loop() {
  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
  Serial.println(" m");

  if(ccs.available()){
    if(!ccs.readData()){
      Serial.print("CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.println(ccs.getTVOC());
      Serial.println("");
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(2000);
  float ppm = ccs.geteCO2();
  float pressure = bmp.readPressure();
  float TVOC = ccs.getTVOC();
  float temperatureC = bmp.readTemperature();
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

  //postData = postVariable + temperatureF;
  postData1 = postVariable1 + ppm;
  postData2 = postVariable2 + pressure;
  postData3 = postVariable3 + TVOC;
  postData = postVariable + temperatureF + " " + postVariable1 + ppm + " "+ postVariable2 + pressure + " " + postVariable3 + TVOC + " ";
  

  if (client.connect(server, 80)) {
    client.println("POST /test/post.php HTTP/1.2");
    client.println("Host: pcr.bounceme.net");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);
    /*client.println(postData1.length());
    client.println();
    client.print(postData1);
    client.println(postData2.length());
    client.println();
    client.print(postData2);
    client.println(postData3.length());
    client.println();
    client.print(postData3);*/
    
  }

  if (client.connected()) {
    client.stop();
  }
  Serial.println("________");
  Serial.println(postData);
  Serial.println(postData1);
  Serial.println(postData2);
  Serial.println(postData3);

  delay(3000);
}
