/*Programa para Ler sensores e enviar dados para ScadaBR remoto
 * Sensor de temperatura DHT11
 * Utilizado um Esp8266, uma base para Esp8266,  Três Sensores de Detector de tensão
 * um sensor de fumaça mq2, um sensor de temperatura.
 * Adaptado Danilo Braz - danilobrazsilva@gmail.com
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include "DHT.h" //Biblioteca sensor de temperatura

#define DHTPIN 14 // pino D5 do Esp
#define DHTTYPE DHT11 // DHT 11 ou DHT22
#define S0 16 // Porta D0 do Esp8266
#define S1 5  // Porta D1 do Esp8266
#define S2 4  // Porta D2 do Esp8266
#define S3 0  // Porta D3 do Esp8266
#define pinA0 A0 //Porta Analógica

#define SERVER_IP "XXX.XXX.XXX.XXX:XX" //Servidor do ScadaBr, colocar o enderedo ScadaBr, ex: 187.15.212.85:8080

//Cria um instância DHT sensor de tensão
DHT dht(DHTPIN, DHTTYPE);

int c = 0 ; //Várialvel contadora;
String datasource; //Variável que armazena qual datasource
float valor; //valor  do datasouce, Receber o valor do datasource

void setup() {

  pinMode(pinA0, INPUT); //utilizado para Multipliex
  pinMode(DHTPIN, INPUT); //utilizado para Sendor de temperatura
  
  pinMode(S0,OUTPUT);//utilizado para Multipliex
  pinMode(S1,OUTPUT);//utilizado para Multipliex
  pinMode(S2,OUTPUT);//utilizado para Multipliex
  pinMode(S3,OUTPUT);  //utilizado para Multipliex

  dht.begin();
  Serial.begin(115200);
  
  //Conectar no Wifi
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    
    // WiFi.mode(WiFi_STA); // it is a good practice to make sure your code sets wifi mode how you want it.

    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    //reseta a configuração - wipe credentials for testing
    //wm.resetSettings();

    // Conectar-se automaticamente usando credenciais salvas,
    // se a conexão falhar, ele inicia um ponto de acesso com o nome especificado ( "AutoConnectAP"),

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    //CRIA um SSID com a rede Monitoramento e a senha   
    res = wm.autoConnect("Monitoramento","0123456789"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //Conectado
        Serial.println("connected... :)");
    }

}
  
void loop() {
 
  //LEITURA DE PORTAS ANALÓGICAS A0 COM MULTIPLEX PARA 16 ENTRADAS ANALÓGICAS
  //a cada loop ele irá ler uma porta analógica ou digital e enviar a valor do sensor na variável valor para o ScadaBR remoto,
  //evitando que envie várias requisições para o servidor de uma só vez
  
  
  if (c >17 ){
      delay(10000);
      c = 0;
  }


  //**sensores de detector de tensão, foi utilizado um multiplicador 0.2655 para se aproximar do valor da tensão medida,
  // logo esse valor depende da tensão de você está alimentando o sensor e esp, onde o sensor de dectação de tensão não da grande precisão em valor de tensão.
  
  
 //Porta Analógica C0 Sensor de Tensão AC
  
  if (c == 0){
      digitalWrite(S0,LOW);
      digitalWrite(S1,LOW);
      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);
      
      valor = analogRead(pinA0)*0.2655;//**
      Serial.print("Sensor de Tensão 1:  ");Serial.println(valor);
      datasource = "c0";
   }


  //Porta Analógica C1 Sensor de Tensão AC
  if (c == 1){
      digitalWrite(S0,HIGH);
      digitalWrite(S1,LOW);
      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);
      
      valor = analogRead(pinA0)*0.2655;//**
      //valor = analogRead(pinA0)*0.21484375;
      Serial.print("Sensor de Tensão 2:  ");Serial.println(valor);
      datasource = "c31";

   }
  
  if (c == 2){
      //Porta Analógica C2 Sensor de Tensão AC
      digitalWrite(S0,LOW);
      digitalWrite(S1,HIGH);
      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);
      valor = analogRead(pinA0)*0.2655;//**
      Serial.print("Sensor de Tensão 3:  ");Serial.println(valor);
      datasource = "c32";

  }

  //Porta Analógica C3
  if (c == 3){
      digitalWrite(S0,HIGH);
      digitalWrite(S1,HIGH);
      digitalWrite(S2,LOW);
      digitalWrite(S3,LOW);
      Serial.print("Sensor 4 ");Serial.println(analogRead(pinA0));
      datasource = "c33";
  }

  if (c == 4){
      //Porta Analógica C4  
      digitalWrite(S0,LOW);
      digitalWrite(S1,LOW);
      digitalWrite(S2,HIGH);
      digitalWrite(S3,LOW);
      Serial.print("Sensor 5 ");Serial.println(analogRead(pinA0));
    datasource = "c34";
  }

  //Porta Analógica C5   
  if (c == 5){
      digitalWrite(S0,HIGH);
      digitalWrite(S1,LOW);
      digitalWrite(S2,HIGH);
      digitalWrite(S3,LOW);
      Serial.print("Sensor 6 ");Serial.println(analogRead(pinA0));
      datasource = "35";
    }
  
  //Porta Analógica C6
  if (c == 6){
      digitalWrite(S0,LOW);
      digitalWrite(S1,HIGH);
      digitalWrite(S2,HIGH);
      digitalWrite(S3,LOW);
      Serial.print("Sensor 7 ");Serial.println(analogRead(pinA0));
      datasource = "36";
  }
   
  //Porta Analógica C7  
  if (c == 7){
      digitalWrite(S0,HIGH);
      digitalWrite(S1,HIGH);
      digitalWrite(S2,HIGH);
      digitalWrite(S3,LOW);
      Serial.print("Sensor 8 ");Serial.println(analogRead(pinA0));
      datasource = "c37";
  }


  //Porta Analógica C8  
   if (c == 8){
      digitalWrite(S0,LOW);
      digitalWrite(S1,LOW);
      digitalWrite(S2,LOW);
      digitalWrite(S3,HIGH);
      Serial.print("Sensor 9 ");Serial.println(analogRead(pinA0));
      datasource = "c38";
   }

  //Porta Analógica C9
  if (c == 9){
      digitalWrite(S0,HIGH);
      digitalWrite(S1,LOW);
      digitalWrite(S2,LOW);
      digitalWrite(S3,HIGH);
      Serial.print("Sensor 10 ");Serial.println(analogRead(pinA0));
      datasource = "c39";
  }

  
  //Porta Analógica C10 
  if (c == 10){ 
      digitalWrite(S0,LOW);
      digitalWrite(S1,HIGH);
      digitalWrite(S2,LOW);
      digitalWrite(S3,HIGH);
      Serial.print("Sensor 11 ");Serial.println(analogRead(pinA0));
      datasource = "c40";
  }
  
  //Porta Analógica C11 
  if (c == 11){ 
      digitalWrite(S0,HIGH);
      digitalWrite(S1,HIGH);
      digitalWrite(S2,LOW);
      digitalWrite(S3,HIGH);
      Serial.print("Sensor 12 ");Serial.println(analogRead(pinA0));
      datasource = "c41";
  }
  
  //Porta Analógica C12
    if (c == 12){  
      digitalWrite(S0,LOW);
      digitalWrite(S1,LOW);
      digitalWrite(S2,HIGH);
      digitalWrite(S3,HIGH);
      Serial.print("Sensor 13 ");Serial.println(analogRead(pinA0));
      datasource = "c42";
    }
  //Porta Analógica C13   
  if (c == 13){
      digitalWrite(S0,HIGH);
      digitalWrite(S1,LOW);
      digitalWrite(S2,HIGH);
      digitalWrite(S3,HIGH);
      Serial.print("Sensor 14 ");Serial.println(analogRead(pinA0));
      datasource = "c43";
  }
  
  //Porta Analógica C14 Sensor de Fumaça
  if (c == 14){  
      digitalWrite(S0,LOW);
      digitalWrite(S1,HIGH);
      digitalWrite(S2,HIGH);
      digitalWrite(S3,HIGH);
      
      if(analogRead(pinA0) > 420)
      {
        valor =  analogRead(pinA0);
        datasource = "c44";
      }
      else
      {
        c++;  
      }
      
      Serial.print("Sensor 15 - Sensor de Fumaça: ");Serial.println(analogRead(pinA0));
      
    
  }
  //Porta Analógica C15 Sensor de Temperatura
  if (c == 15){   
      digitalWrite(S0,HIGH);
      digitalWrite(S1,HIGH);
      digitalWrite(S2,HIGH);
      digitalWrite(S3,HIGH);
      Serial.print("Sensor 16: ");Serial.println(pinA0);
      datasource = "c15";

  }

    //ENTRADAS DIGITAIS

  if(c == 16){
      //Porta digital D5 pin 14
    
      //LEITURA DA TEMPERATURA
      // A leitura da temperatura e umidade pode levar 250ms!
      // O atraso do sensor pode chegar a 2 segundos.
      //float h = dht.readHumidity();
      float t = dht.readTemperature();
      valor = t;
      // testa se retorno é valido, caso contrário algo está errado.
      if (isnan(valor))// || isnan(h)) 
      {
        Serial.println("Failed to read from DHT");
      } 
      else 
      {
        //Serial.print("Umidade: ");
        //Serial.print(valor);
        //Serial.print(" %t");
        
        Serial.print("Sensor - Temperatura: ");
        Serial.print(valor);
        Serial.println(" *C");
      }
 
    datasource = "c20";
  }

  if (c==17){
    datasource = "c21";
    valor = h;
  }

  
 c++; //Variável de contagem
  
  String valorString = String(valor); //Converte a Variavél para string
  
  // Verifica a conexão com wifi
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    // Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    
    // Envia o valor do Sensor para o Servidor ScadaBR
     http.begin(client, "http://" SERVER_IP "/ScadaBR/httpds?" + datasource +"=" + valorString); //HTTP
     http.addHeader("Content-Type", "application/json");
    
    //Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
     Serial.println("http://" SERVER_IP "/ScadaBR/httpds?" + datasource +"=" + valorString);
     
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      //Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        //Serial.println("received payload:\n<<");
        //Serial.println(payload);
        //Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] .. failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(1000); //configuração o tempo para envio de cada envio de requisição.

}
