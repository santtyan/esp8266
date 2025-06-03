#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>
#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA Sensores de Tensão
#include "DHT.h"     //INCLUSÃO DE BIBLIOTECA DE TEMPERATURA DHT11

#include "config.hpp" // INCLUI O ARQUIVO DE CONFIGURAÇÃO

//Pinos Utilizados
const int pintensao0 = 1; //PINO ANALÓGICO UTILIZADO PELO SENSOR
const int pintensao1 = 2; //PINO ANALÓGICO UTILIZADO PELO SENSOR
const int pintemp = A4;    //PINO ANALÓGICO UTILIZADO PELO SENSOR

int   Htime; //inteiro para armazenar tempo alto para o calculo de frequência
int   Ltime; //inteiro para armazenar tempo baixo para o calculo de frequência
float Ttime; //inteiro para armazenar o tempo total de um ciclo para o calculo de frequência

#define VOLT_CAL 215.3 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO)

#define DHTTYPE DHT11 // DHT 22 (AM2302)
EnergyMonitor emon1; //CRIA UMA INSTÂNCIA
EnergyMonitor emon2; //CRIA UMA INSTÂNCIA
DHT dht(pintemp, DHTTYPE);

byte mac[] = {MAC1, MAC2, MAC3, MAC4, MAC5, MAC6}; // Valores definidos no config.hpp
IPAddress zabbix(ZABBIX_IP1, ZABBIX_IP2, ZABBIX_IP3, ZABBIX_IP4);
int zabbixPort = 10051;

char host[] = "arduino1"; // hostname zabbix
char key[][11] = {"temperatura", "umidade"}; // DEFINA AQUI O NOME DAS VARIAVEIS (tamanho máximo definido pelo 11)

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    //Configuração da Ethernet
    Serial.println("Obtaining IP address using DHCP...");
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        for (;;)
            ;
    }
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());

    //SENSORES DE TENSÃO
    emon1.voltage(pintensao0, VOLT_CAL, 1.7); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO / MUDANÇA DE FASE)
    emon2.voltage(pintensao1, VOLT_CAL, 1.7); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO / MUDANÇA DE FASE)
}

void loop()
{
    //sensores de tensão
    emon1.calcVI(17, 500); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS, TEMPO LIMITE PARA FAZER A MEDIÇÃO)
    emon2.calcVI(17, 500); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS, TEMPO LIMITE PARA FAZER A MEDIÇÃO)
    float t0 = emon1.Vrms; //VARIÁVEL RECEBE O VALOR DE TENSÃO RMS OBTIDO
    float t1 = emon2.Vrms; //VARIÁVEL RECEBE O VALOR DE TENSÃO RMS OBTIDO

    //sensor de temperatura
    float temperatura = dht.readTemperature(); //VARIÁVEL RECEBE A TEMPERATURA MEDIDA
    float umidade = dht.readHumidity();        //VARIÁVEL RECEBE A UMIDADE MEDIDA

    // Envio para Zabbix
    zabbix_send_trap();

    delay(2000);
}

void zabbix_send_trap()
{
    EthernetClient zbx_client;
    char packet_header[] = "ZBXD\1"; //followed by unsigned long long content_len
    unsigned long long content_len;
    unsigned int payload_len;
    char packet_content[256];

    for (int i = 0; i < sizeof(key) / sizeof(key[0]); i++)
    {
        content_len = prepare_content(packet_content, key[i]);
        payload_len = sizeof(content_header) + content_len + sizeof(content_footer);
        if (zbx_client.connect(zabbix, zabbixPort))
        {
            Serial.println("connected to zabbix");
            Serial.println("sending data");
            zbx_client.write(packet_header, sizeof(packet_header) - 1);
            zbx_client.write((char *)&content_len, sizeof(content_len));
            zbx_client.write(packet_content, content_len);
            delay(1);
            zbx_client.stop();
            Serial.println("disconnected");
        }
    }
}

const char PROGMEM content_header[] = "{\"request\":\"sender data\",\"data\":[";
const char PROGMEM content_item[] = "{\"host\":\"%s\",\"key\":\"%s\",\"value\":\"%d\"}";
const char PROGMEM content_footer[] = "]}";

unsigned int prepare_content(char *dst, char keyAtual[])
{
    unsigned int len = 0;

    memcpy(&dst[len], content_header, sizeof(content_header));
    len += sizeof(content_header) - 1;
    len += sprintf_P(&dst[len], content_item, host, keyAtual, counter);
    memcpy(&dst[len], content_footer, sizeof(content_footer));
    len += sizeof(content_footer) - 1;
    return len;
}
