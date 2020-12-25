#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

//SSID dados da rede
const char* ssid = "Inacio"; //nome da rede que a placa ira se conectar
const char* ssidPassword = "5938inacio"; //senha da rede

//host
#define FIREBASE_HOST "https://esp8266-webserver-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "bGM6h2PiFO5fPdPcYD2LWQMKGJUVjvcYM9WIrZqf"

//define objetos para base de dados
FirebaseData firebaseData1;

const int ledPin = 5;
String path = "/Nodes";
String nodeID = "Node1";

void setup() {
  //Serial
  Serial.begin(115200);//frequencia da porta serial
  pinMode(ledPin,OUTPUT);
  
  delay(10);
  //printa no monitor serial que esta se conectando a rede wifi
  Serial.println();
  Serial.println();
  Serial.print("Conectando a rede: ");
  Serial.print(ssid);

  WiFi.begin(ssid, ssidPassword); // se conecta a rede wifi local

  while(WiFi.status() != WL_CONNECTED){
    delay(500); // espera ate se conectar a rede wifi
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado a rede");
  Serial.println("IP address local: ");
  Serial.print(WiFi.localIP());
  Serial.println();
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH); //se conecta ao servidor
  Firebase.reconnectWiFi(true);

  if (!Firebase.setInt(firebaseData1, path + "/" + nodeID,2)) //setDouble salva o valor 2 no caminho: path + "/" + nodeID
    {
        Serial.println("Could not begin stream");
        Serial.println("REASON: " + firebaseData1.errorReason());
        Serial.println();
    }
  delay(5000);
  Serial.println("Conectado ao servidor: ");
  Serial.print(FIREBASE_HOST);
}

void loop() {
  delay(2000);
  
  Firebase.getInt(firebaseData1,path + "/" + nodeID); // acessa o valor inteiro no caminho
  Serial.println(firebaseData1.intData()); //printa valor inteiro que esta gravado em firebaseData1
  
  if (firebaseData1.intData() == 2)
    {
        digitalWrite(ledPin,HIGH);
    }else{
        digitalWrite(ledPin,LOW);
    }

    if (firebaseData1.streamTimeout()) // caso nao obtenha mais resposta do servidor
    {
        Serial.println();
        Serial.println("Stream timeout, resume streaming...");
        Serial.println();
    }
}
