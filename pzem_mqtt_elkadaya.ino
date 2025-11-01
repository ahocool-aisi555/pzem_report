#include <ESP8266WiFi.h>
#include <PubSubClient.h>



const char *ssid =  "WesGakUsah";   // cannot be longer than 32 characters!
const char *pass =  "1234567890";   //



const char *mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;
const char *mqtt_user = "";
const char *mqtt_pass = "";
const char *mqtt_client_name = "ahocool123"; // Client connections cant have the same connection name


#define TOPIC "/aisi555/dimmer"


#define LED 5




WiFiClient wclient;
PubSubClient client(wclient);

void setup() {
  pinMode(LED, OUTPUT);
  
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqtt_callback);
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
   

    if (client.connected()){
      Serial.println("subscribe to topic: ");
      Serial.println(TOPIC);
      client.subscribe(TOPIC); //subscribe ke topic dimmer
    }
  

}

void loop() {
 
  if (!client.connected()) 
   {
    reconnectmqtt();
   }
   else client.loop(); //cek terus kalau ada data masuk
  
  
  
}


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
 
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
   }
     
  
  int pwm = message.toInt();

  analogWrite(LED, pwm);
    
  Serial.println("---atur nyala lampu----");
  Serial.print(" Level : ");
  Serial.println(pwm);
  Serial.println("-----------------------");  


}
