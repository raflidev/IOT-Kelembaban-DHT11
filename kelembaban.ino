#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid      = "wifi";
const char* password  = "sirkelrapli101";

int wifiStatus;

const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp8266/test";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);


#include "DHT.h"
#define DHTPIN D7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

bool unit = true;

void MQTT_connect();


void setup(){
  Serial.begin(9600);
  delay(200);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println();
  Serial.println();
  Serial.print("Terkoneksi dengan Wifi : ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  //ketika WiFI.status nilainya TDK sama dg WL_CONNECTED
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }        
  Serial.println();

  Serial.println("WiFi connected");
    //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
//  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  dht.begin();
}

void loop(){ 
  wifiStatus = WiFi.status();

  if(wifiStatus == WL_CONNECTED){
    char buffer[15];
    String myString;
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    Serial.print("Suhu: ");
    Serial.print(t);
    Serial.print("... ");
    client.publish("/topik/rapli/iot/suhu", dtostrf(t,5,2,buffer));
    
    Serial.print("\nKelembaban: ");
    Serial.print(h);
    Serial.print("...");
    
    client.publish("/topik/rapli/iot/lembab", dtostrf(h,5,2,buffer));
  }else{
    Serial.println("");
    Serial.println("WiFi tdk terkoneksi");
  }
  
  delay(1000);
}
