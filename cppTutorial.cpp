#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>

#define STASSID "TP-LINK_CDD650"
#define STAPSK  "2d4ux6yu"
#define mqtt_server "broker.hivemq.com"
const uint16_t mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Dang ket noi toi: ");
  Serial.println(STASSID);
  WiFi.begin(STASSID,STAPSK);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi da ket noi thanh cong");
  Serial.println("IP Adress: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic,byte* payload,unsigned int length){
  Serial.print("Co tin nhan moi toi tu topic: ");
  Serial.println(topic);
  int le = length;
  for (int i = 0; i < le; i++)
  {
    Serial.print((char)payload[i]);
  }
}

void reconnect(){
  while (!client.connected())
  {
    if(client.connect("sdfsdfsdfsdf"))
    {
      Serial.println("Da ket noi toi broker ");
      client.subscribe("helloword");
    }
    else {
      Serial.print("Loi:, rc=");
      Serial.print(client.state());
      Serial.println("thu lai trong 5 giay");
      delay(5000);
    }
  }
  
}

unsigned long t;


void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server,mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if(!client.connected())
  reconnect();
  client.loop();
}