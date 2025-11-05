#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <PZEM004Tv30.h>

// Gunakan Serial2 (software serial) karena Serial0 digunakan untuk debug
#include <SoftwareSerial.h>
SoftwareSerial pzemSerial(D6, D7); // RX, TX → sesuaikan pin!

PZEM004Tv30 pzem(pzemSerial);


const char *ssid =  "WesGakUsah";   // cannot be longer than 32 characters!
const char *pass =  "1234567890";   //



const char *mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char *mqtt_user = "";
const char *mqtt_pass = "";
const char *mqtt_client_name = "ahocool123"; // Client connections cant have the same connection name

// timer mqtt
unsigned long lastMsg = 0;
#define MSG_DELAY 10000
#define TOPIK "/aisi555/pzem"

WiFiClient wclient;
PubSubClient client(wclient);

void setup() {

  
  pzemSerial.begin(9600);
  // atur alamat Modbus pzem
  pzem.setAddress(0x01);
  // parameter MQTT
  client.setServer(mqtt_server, mqtt_port);
  //serial debug
  Serial.begin(9600);
  delay(10);
  
  Serial.println();
  Serial.println();
  setupwifi();
}

void setupwifi()
{
   if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to Wifi:  ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void reconnectmqtt()
{
   
    
      Serial.println("Connecting to MQTT server..");
     
      if (client.connect(mqtt_client_name,mqtt_user, mqtt_pass)) {
        Serial.println("Connected to MQTT server");
      } else {
        Serial.println("Could not connect to MQTT server");   
      }
   


}

void loop() {
 
  if (!client.connected()) 
   {
    reconnectmqtt();
   }
   else client.loop(); 
  
  
  // Kirim pzem tiap 10 detik
  unsigned long now = millis();
  if (now - lastMsg > MSG_DELAY) {
    lastMsg = now;
    kirim_pzem();
  }
}


void kirim_pzem()
{

  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  if(isnan(voltage)){
    Serial.println("Error membaca PZEM!");
  } else {
    Serial.print("Tegangan: "); Serial.print(voltage); Serial.println(" V");
    Serial.print("Arus: "); Serial.print(current); Serial.println(" A");
    Serial.print("Daya: "); Serial.print(power); Serial.println(" W");
    Serial.print("Energi: "); Serial.print(energy); Serial.println(" kWh");
    Serial.print("Frekuensi: "); Serial.print(frequency); Serial.println(" Hz");
    Serial.print("PF: "); Serial.println(pf);

    String json = "{\"volt\":";
    json += String(voltage, 2);  // 2 angka di belakang koma
    json += ",\"arus\":";
    json += String(current, 3);
    json += ",\"daya\":";
    json += String(power, 2);
    json += ",\"kwh\":";
    json += String(energy, 2);
    json += ",\"frek\":";
    json += String(frequency, 1);
    json += ",\"pf\":";
    json += String(pf, 2);
    json += "}";

    // Kirim via MQTT
    client.publish(TOPIK, json.c_str());

    Serial.println(json); //  lihat di Serial Monitor
  }

}
