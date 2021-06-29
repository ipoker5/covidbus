#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <DHT_U.h>

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#include <Ticker.h>

#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"
#include "viola.h"

#include <Servo.h>


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// VARIABLES GENERALES //////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// variables para la página html
String temperatura_;
String humedad_;
String nivel_gas_;

/////////////////////////////// SENSOR GPS ///////////////////////////////////////
static const int RXPin = 3, TXPin = 1; static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);
char buffer[100];

int idtipo_gps = 490; String tipo_gps = "gps"; String nombre_gps = "NEO-M6"; int idsensor_gps = 490; //variables para los metodos REST
float latitude , longitude; String lat_str , lng_str;  // variables para la funcion printData()

/////////////////////////////// SENSOR DHT_11 //////////////////////////////////// 
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int id_dht11 = 12; String tipo_dht11 = "humedad_temperatura"; String nombre_dth11 = "DHT_11"; // variables para los metodos REST

/////////////////////////////// SENSOR MQ-2 //////////////////////////////////////
int id_mq2 = 2; String tipo_mq2 = "humo"; String nombre_mq2 = "MQ_2"; // variables para los metodos REST

/////////////////////////////// ACTUADOR ALTAVOZ /////////////////////////////////
AudioGeneratorWAV *wav;
AudioFileSourcePROGMEM *file;
AudioOutputI2SNoDAC *out;

/////////////////////////////// ACTUADOR SERVO ///////////////////////////////////
Servo myservo1;
Servo myservo2;

WiFiServer server(80);

/////////////////////////////// CONEXIONES ///////////////////////////////////////
IPAddress serverAddress(192, 168, 0, 16); // ip del pc

//variables para conexión ESP8266/wifi
WiFiClient wifi_http; int port_http = 8080;
HttpClient client_http = HttpClient(wifi_http, serverAddress, port_http); //cliente HTTP

//variables para conexión MQTT
WiFiClient wifi_mqtt; int port_mqtt = 1883; const char usser_pass[7] = "admin";
PubSubClient client_mqtt(serverAddress,port_mqtt,wifi_mqtt); //cliente MQTT

long last_msg = 0; char msg_mqtt[100];


//______________________________________________________________________________//
//______________________________________________________________________________//


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// FUNCIONES AUXILIARES /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//FUNCIÓN AUXILAR PARA EL SERVO

void ventana_open(float temp, float hum, float gas){ // funcion para comprobar los niveles de temperatura, humedad y gas para activar el servo
  if(temp > 24 || hum > 90 || gas > 60){
    for (int angulo = 0; angulo <= 90; angulo++){  // se activa el servo a 90 grados (ventana)
      myservo1.write(angulo);     
      myservo2.write(angulo);              
      delay(10); 
    }
  }
  if(temp>0){
    temperatura_ = temp;
  }
  if(hum>0){
    humedad_ = hum;
  }
  if(gas>0){
    nivel_gas_ = gas;
  }
}


/////////////////////////////// CONEXIÓN WIFI ////////////////////////////////////

void setup_wifi(){
  WiFi.begin("vodafone7638", "N5ZXFUJGH5AK4Y");
  Serial.print("\nConectando Wifi:");
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(50);
  }
  Serial.print("  --> Wifi Conectado: IP address -> ");
  Serial.print(WiFi.localIP());
}


/////////////////////////////// CONEXIÓN MQTT ////////////////////////////////////

void callback(char* topic, byte* payload, unsigned int length) {
  digitalWrite(LED_BUILTIN, LOW);

  Serial.print("\nMessage arrived [");
  Serial.print(topic);
  Serial.print("]\n ");

  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  String topicStr(topic);
  if (topicStr.compareTo("control_puerta")==0){ //si el topic es control_puerta

    if ((char)payload[0] == '1') { //1 para entrar al bus, se activa el servo a 180 grados (puerta)
      //SERVOS ON
      for (int angulo = 0; angulo <= 180; angulo++){  
        myservo1.write(angulo);     
        myservo2.write(180-angulo);              
        delay(20); 
      }
      delay(3000); 
      for (int angulo = 180; angulo>= 0; angulo--){  
        myservo1.write(angulo);     
        myservo2.write(180-angulo);              
        delay(20); 
      }    
    }
    if ((char)payload[0] == '0') { //0 para salir del bus, se activa el servo a 180 grados (puerta) y suena el altavoz para indicar al conductor que alguien se quiere bajar
      //ALTAVOZ ON
      audioLogger = &Serial;
      file = new AudioFileSourcePROGMEM( viola, sizeof(viola) );
      out = new AudioOutputI2SNoDAC();
      wav = new AudioGeneratorWAV();
      wav->begin(file, out);

      //SERVOS ON
      for (int angulo = 0; angulo <= 180; angulo++){  
        myservo1.write(angulo);     
        myservo2.write(180-angulo);              
        delay(20); 
      } 
      delay(3000);   
      for (int angulo = 180; angulo>= 0; angulo--){  
        myservo1.write(angulo);     
        myservo2.write(180-angulo);              
        delay(20); 
      }     
    }
  }

  if (topicStr.compareTo("control_ventana")==0){ //si el topic es control_ventana

    if ((char)payload[0] == '1') { //1 para servo D3, se activa el servo a 90 grados (ventana nº1)
      //SERVO D3 ON
      for (int angulo = 0; angulo <= 90; angulo++){  
        myservo1.write(angulo);     
        delay(10); 
      }      
    }
    if ((char)payload[0] == '0') {  //0 para servo D0, se activa el servo a 90 grados (ventana nº2)
      //SERVO D0 ON
      for (int angulo = 0; angulo <= 90; angulo++){  
        myservo2.write(angulo);     
        delay(10); 
      }    
    }
  }

  if (topicStr.compareTo("control_sensores")==0){ //si el topic es control_sensores

    if ((char)payload[0] == '1') { //1 para para sensor temperatura

      float h = dht.readHumidity();
      float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
      if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT sensor!");
      }
      ventana_open(t,h,0);
      Serial.print(("  Temperature: "));
      Serial.print(t);
      Serial.print((" grados ||"));
      Serial.print(("  Humededity: "));
      Serial.print(h);
      Serial.print("% ");
    }
    if ((char)payload[0] == '0') {  //0 para sensor gas
      Serial.print("\n\nMQ-2 test -> ");
      float h = analogRead(A0);
      if(isnan(h)){
        Serial.println("ERROR NO DETECTA SENSOR MQ-2");
      }
      ventana_open(0,0,h/1023*100); 
      Serial.print("Nivel de gas: ");
      Serial.print(h/1023*100); 
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
}

void mqtt_reconnect() { 

  String client_Id = "ESP8266Client";  //nombre de cliente
  client_Id += String(random(0xffff), HEX); 
  while (!client_mqtt.connected()) {
    Serial.print("\nEsperando a la Conexion MQTT...");
    if (client_mqtt.connect(client_Id.c_str(),usser_pass,usser_pass)) { 
      Serial.print("\nMQTT Conectado --> ");
      Serial.print("ID Cliente --> ");
      Serial.print(client_Id);

      //client_mqtt.publish("casa/despacho/temperatura", "Enviando el primer mensaje");  //subscripcion o subscripciones
      client_mqtt.subscribe("control_puerta");
      client_mqtt.subscribe("control_ventana");
      client_mqtt.subscribe("control_sensores");

      
    } else {
        Serial.print("\nFailed, rc=");
        Serial.print(client_mqtt.state());
        Serial.println("Try again in 5 seconds: ");
        for(int i=5;i>0;i--){
            delay(1000);
            Serial.print(i);
            Serial.print(", ");
      }
    }
  }
}

void mqtt_setup(){
    Serial.print("\n\nMQTT Iniciado:");
    delay(10);
    pinMode(LED_BUILTIN,OUTPUT);
    client_mqtt.setServer(serverAddress,port_mqtt);
    client_mqtt.setCallback(callback);
    mqtt_reconnect();
}

void mqtt_loop() {
  if (!client_mqtt.connected()) {
    mqtt_reconnect();
  }
  client_mqtt.loop();
  long now = millis();
  if (now - last_msg > 2000) {
    last_msg = now;
    //Serial.print("Publish message: ");
    Serial.println(msg_mqtt);
    //client_mqtt.publish("casa/despacho/temperatura", msg_mqtt);
  }
}


/////////////////////////////// SERIALIZE //////////////////////////////////////

String serialize_GPS_Info(int idtipo_gps, float last_value_x, float last_value_y, int idsensor){

  StaticJsonDocument<200> doc;
  doc["idtipo_gps"] = idtipo_gps;
  doc["x"] = last_value_x;
  doc["y"] = last_value_y;
  doc["idsensor"] = idsensor;

  String output;
  serializeJson(doc,output);
  return output;
}

String serialize_Sensor_Info(int idsensor, String tipo, String nombre, float last_value1, float last_value2, int iddispositivo){

  StaticJsonDocument<200> doc;
  doc["idsensor"] = idsensor;
  doc["tipo"] = tipo;
  doc["nombre"] = nombre;
  doc["last_value1"] = last_value1;
  doc["last_value2"] = last_value2;
  doc["iddispositivo"] = iddispositivo;

  String output;
  serializeJson(doc,output);
  return output;
}

String serialize_Sensor_Data(String timestamp,float valor1, float valor2, int idsensor){

  DynamicJsonDocument doc(200);
  doc["timestamp"] = timestamp;
  doc["valor1"] = valor1;
  doc["valor2"] = valor2;
  doc["idsensor"] = idsensor;

  String output;
  serializeJson(doc,output);
  return output;
}


/////////////////////////////// FUNCIONES DE SENSORES ///////////////

//SENSOR GPS
void init_GPS(){ //INICIALIZACIÓN EN SETUP: introduzco con un post del sensor gps correspondiente a la base de datos -->  Info_Sensor y en Tipo_GPS

  String contentType = "application/json";

  //POST EN INFO_SENSOR
  Serial.print("\n\n  --  POST_1 GPS (setup) --> INFO_SENSOR --");
  String body_info_sensor = serialize_Sensor_Info(idsensor_gps, tipo_gps, nombre_gps, 00.00, 00.00, 1);

  client_http.beginRequest();
  client_http.post("/api/Post_Info_Sensor/", contentType, body_info_sensor.c_str());
  int code_gps_info = client_http.responseStatusCode();
  if(code_gps_info==200 || code_gps_info==201){
    Serial.print("\nCode: ");
    Serial.print(code_gps_info);
    Serial.print("\nBody: ");
    Serial.print(client_http.responseBody());
  }else if(code_gps_info == 401){
    Serial.print("\nCode: ");
    Serial.print(code_gps_info);
    Serial.print(" --> Sensor ya instalado");
  }else{
    Serial.print("\nCódigo de error: ");
    Serial.print(code_gps_info);
  }
  client_http.endRequest();


  //POST Tipo_gps ---> idtipo_gps  /  x  /  y  /  idsensor  
  Serial.print("\n\n  --  POST_2 GPS (setup) --> TIPO_GPS --");
  String body_info_gps = serialize_GPS_Info(idtipo_gps, 00.00, 00.00, idsensor_gps);

  client_http.beginRequest();
  client_http.post("/api/PostGPS/", contentType, body_info_gps.c_str());
  int code_gps = client_http.responseStatusCode();
  if(code_gps==200 || code_gps==201){
    Serial.print("\nCode: ");
    Serial.print(code_gps);
    Serial.print("\nBody: ");
    Serial.print(client_http.responseBody());
  }else if(code_gps == 401){
    Serial.print("\nCode: ");
    Serial.print(code_gps);
    Serial.print(" --> Sensor ya instalado");
  }else{
    Serial.print("\nCódigo de error: ");
    Serial.print(code_gps);
  }
  client_http.endRequest();
}

void sensor_GPS(){ //FUNCION EN LOOP: para actualizar los valores del sensor gps con un POST en Data_Sensor y dos PUTS en Info_Sensor y Tipo_gps

  float x = random(100000);
  float y = random(100000);

  //post datasensor --> timestamp  /  valor_X  /  valor_Y  /  idsensor 
  Serial.println(("\n POST PERIÓDICO GPS --> DATA_SENSOR:"));
  String contentType = "application/json";
  String body_data_sensor = serialize_Sensor_Data("null", x, y, idsensor_gps);

  client_http.beginRequest();
  client_http.post("/api/Post_Data_Sensor/", contentType, body_data_sensor.c_str());
  int code = client_http.responseStatusCode();
  if(code==200 || code==201){
    Serial.print("\nCode: ");
    Serial.print(code);
    Serial.print("\nBody: ");
    Serial.print(client_http.responseBody());
   }else if(code == 401){
    Serial.print("\nCode: ");
    Serial.print(code);
    Serial.print(" --> Fecha ya existente");
  }else{
    Serial.print("\nCódigo de error: ");
    Serial.print(code);
  }
  client_http.endRequest();


  //PUT Info_Sensor ---> idsensor  /  tipo  /  nombre  /  last_value1  /  last_value2  /  iddispositivo
  Serial.println(("\n PUT PERIÓDICO GPS --> INFO_SENSOR:"));
  String body_info_sensor = serialize_Sensor_Info(idsensor_gps, tipo_gps, nombre_gps, x, y, 1);

  client_http.beginRequest();
  client_http.put("/api/PutInfoSensor/490",contentType,body_info_sensor.c_str());
  Serial.print("\nCode: ");
  Serial.print(client_http.responseStatusCode());
  Serial.print("\nBody: ");
  Serial.print(client_http.responseBody());
  client_http.endRequest();
  

  //PUT Tipo_gps ---> idtipo_gps  /  x  /  y  /  idsensor  
  Serial.print("\n\n  --  PUT PERIÓDICO GPS --> TIPO_GPS --");
  String body_info_gps = serialize_GPS_Info(idtipo_gps, x, y, idsensor_gps);

  client_http.beginRequest();
  client_http.put("/api/PutSensorGPS/490", contentType, body_info_gps.c_str());
  Serial.print("\nCode: ");
  Serial.print(client_http.responseStatusCode());
  Serial.print("\nBody: ");
  Serial.print(client_http.responseBody());
  client_http.endRequest();
}

/*void displayInfo() //OPCION_1
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat());
    Serial.print(F(","));
    Serial.print(gps.location.lng());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
void printData() //OPCION_2
{
    if (gps.location.isUpdated()) {
        double lat = gps.location.lat();
        double lng = gps.location.lng();
 
        double altitude = gps.altitude.meters();
 
        int year = gps.date.year();
        int month = gps.date.month();
        int day = gps.date.day();
 
        int hour = gps.time.hour();
        int minute = gps.time.minute();
        int second = gps.time.second();
 
        snprintf(buffer, sizeof(buffer),
                 "Latitude: %.8f, Longitude: %.8f, Altitude: %.2f m, "
                 "Date/Time: %d-%02d-%02d %02d:%02d:%02d",
                 lat, lng, altitude, year, month, day, hour, minute, second);
 
        Serial.println(buffer);
    }
}*/

//SENSOR DHT11
void init_DTH_11(){ //INICIALIZACIÓN EN SETUP: introduzco con un post el sensor correspondiente a la base de datos --> Info_Sensor

  Serial.print("\n\n  --  POST DHT_11 (setup) --> INFO_SENSOR --");

  String contentType = "application/json";
  dht.begin();
  String body_info = serialize_Sensor_Info(id_dht11, tipo_dht11, nombre_dth11, 00.00, 00.00, 1);

  client_http.beginRequest();
  client_http.post("/api/Post_Info_Sensor/", contentType, body_info.c_str());
  int code_dht = client_http.responseStatusCode();
  if(code_dht==200 || code_dht==201){
    Serial.print("\nCode: ");
    Serial.print(code_dht);
    Serial.print("\nBody: ");
    Serial.print(client_http.responseBody());
  }else if(code_dht == 401){
    Serial.print("\nCode: ");
    Serial.print(code_dht);
    Serial.print(" --> Sensor ya instalado");
  }else{
    Serial.print("\nCódigo de error: ");
    Serial.print(code_dht);
  }
  client_http.endRequest();
}

void sensor_DHT11(){ //FUNCION EN LOOP: para actualizar los valores del sensor con un put en Info_Sensor y un post en Data_Sensor

  //lectura de temperatura y humedad:
  Serial.print(("\nDHT11 test --> "));
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  ventana_open(t,h,0);

  Serial.print(("  Temperature: "));
  Serial.print(t);
  Serial.print((" grados ||"));
  delay(1500);
  Serial.print(("  Humededity: "));
  Serial.print(h);
  Serial.print("% ");
  delay(1500);


  //métodos rest
  String contentType = "application/json";
  
  //put infosensor ---> idsensor  /  tipo  /  nombre  /  last_value1  /  last_value2  /  iddispositivo
  Serial.print("\n\n  --  PUT PERIÓDICO DHT_11 --> INFO_SENSOR --");
  String body_info = serialize_Sensor_Info(id_dht11, tipo_dht11, nombre_dth11, t, h, 1);
  client_http.beginRequest();
  client_http.put("/api/PutInfoSensor/12",contentType,body_info.c_str());
  Serial.print("\nCode: ");
  Serial.print(client_http.responseStatusCode());
  Serial.print("\nBody: ");
  Serial.print(client_http.responseBody());
  client_http.endRequest();
  
  delay(1500);

  //post datasensor --> timestamp  /  valor1  /  valor2  /  idsensor 

  Serial.print("\n\n  --  POST PERIÓDICO DHT_11 --> DATA_SENSOR --");
  String body_data = serialize_Sensor_Data("null", t, h, id_dht11);

  client_http.beginRequest();
  client_http.post("/api/Post_Data_Sensor/", contentType, body_data);
  int code = client_http.responseStatusCode();
  if(code==200 || code==201){
    Serial.print("\nCode: ");
    Serial.print(code);
    Serial.print("\nBody: ");
    Serial.print(client_http.responseBody());
   }else if(code == 401){
    Serial.print("\nCode: ");
    Serial.print(code);
    Serial.print(" --> Fecha ya existente");
  }else{
    Serial.print("\nCódigo de error: ");
    Serial.print(code);
  }
  client_http.endRequest();
}

//SENSOR MQ2
void init_MQ_2(){ //INICIALIZACIÓN EN SETUP: introduzco con un post el sensor correspondiente a la base de datos --> Info_Sensor

  Serial.print("\n\n  --  POST MQ-2 (setup) --> INFO_SENSOR --");

  String contentType = "application/json";
  String body_info = serialize_Sensor_Info(id_mq2, tipo_mq2, nombre_mq2, 00.00, 00.00, 1);

  client_http.beginRequest();
  client_http.post("/api/Post_Info_Sensor/", contentType, body_info.c_str());
  int code_mq2 = client_http.responseStatusCode();
  if(code_mq2==200 || code_mq2==201){
    Serial.print("\nCode: ");
    Serial.print(code_mq2);
    Serial.print("\nBody: ");
    Serial.print(client_http.responseBody());
  }else if(code_mq2 == 401){
    Serial.print("\nCode: ");
    Serial.print(code_mq2);
    Serial.print(" --> Sensor ya instalado");
  }else{
    Serial.print("\nCódigo de error: ");
    Serial.print(code_mq2);
  }
  client_http.endRequest();
}

void sensor_MQ_2(){ //FUNCION EN LOOP: para actualizar los valores del sensor con un put en Info_Sensor y un post en Data_Sensor

  //lectura mq-2
  Serial.print("\n\nMQ-2 test -> ");
  float h = analogRead(A0);
    if(isnan(h)){
      Serial.println("ERROR NO DETECTA SENSOR MQ-2");
      return;
    }

  ventana_open(0,0,h/1023*100); 

  Serial.print("Nivel de gas: ");
  Serial.print(h/1023*100);
  delay(1500);

  //métodos rest
  String contentType = "application/json";

  //put infosensor ---> idsensor  /  tipo  /  nombre  /  last_value1  /  last_value2  /  iddispositivo 
  Serial.print("\n\n  --  PUT PERIÓDICO MQ-2 --> INFO_SENSOR --");
  String body_info = serialize_Sensor_Info(id_mq2, tipo_mq2, nombre_mq2, h/1023*100, 00.00, 1);
  
  client_http.beginRequest();
  client_http.put("/api/PutInfoSensor/2", contentType, body_info);
  Serial.print("\nCode: ");
  Serial.print(client_http.responseStatusCode());
  Serial.print("\nBody: ");
  Serial.print(client_http.responseBody());
  client_http.endRequest();
  
  delay(1500);

  //post datasensor --> timestamp  /  valor1  /  valor2  /  idsensor 
  
  Serial.print("\n\n  --  POST PERIÓDICO MQ-2 --> DATA_SENSOR --");
  String body_data = serialize_Sensor_Data("null", h/1023*100, 00.00, id_mq2);

  client_http.beginRequest();
  client_http.post("/api/Post_Data_Sensor/", contentType, body_data);
  int code = client_http.responseStatusCode();
  if(code==200 || code==201){
    Serial.print("\nCode: ");
    Serial.print(code);
    Serial.print("\nBody: ");
    Serial.print(client_http.responseBody());
  }else if(code == 401){
    Serial.print("\nCode: ");
    Serial.print(code);
    Serial.print(" --> Fecha ya existente");
  }else{
    Serial.print("\nCódigo de error: ");
    Serial.print(code);
  }
  client_http.endRequest();
}


//______________________________________________________________________________//
//______________________________________________________________________________//
/////////////////////////////// TICKERS //////////////////////////////////////////
Ticker timer_gps(sensor_GPS,60000); //1 MIN
Ticker timer_dht11(sensor_DHT11, 240000); // 4 MIN
Ticker timer_mq2(sensor_MQ_2, 120000); //2 MIN
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// SETUP ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void setup(){
  delay(3000);
  Serial.begin(9600);
  Serial.println("\n\nDispositivo arrancado.");

  //INICIO CONEXIÓN WIFI
    setup_wifi();

  //INICIO CONEXIÓN MQTT
    mqtt_setup();

  //SERVIDOR HTTP PARA ESP8266NODE
    server.begin(); //Iniciamos el servidor
    Serial.print("\n\nServidor ESP Iniciado -->");
    Serial.print("Ingrese desde un navegador web usando la siguiente IP --> ");
    Serial.print(WiFi.localIP()); //Obtenemos la IP

  //INICIALIZACIÓN GPS  --> POST1: idsensor  /  tipo  /  nombre  /  last_value1  /  last_value2  /  iddispositivo || POST2: idtipo_gps  /  x  /  y  /  idsensor 
    init_GPS();
    //gpsSerial.begin(GPSBaud);

  //INICIALIZACIÓN DE DHT11 --> POST: idsensor  /  tipo  /  nombre  /  last_value1  /  last_value2  /  iddispositivo
    init_DTH_11();

  //INICIALIZACIÓN DE MQ-2 --> POST: idsensor  /  tipo  /  nombre  /  last_value1  /  last_value2  /  iddispositivo
    init_MQ_2();

  //INICIALIZACIÓN DE ALTAVOZ
    audioLogger = &Serial;
    file = new AudioFileSourcePROGMEM( viola, sizeof(viola) );
    out = new AudioOutputI2SNoDAC();
    wav = new AudioGeneratorWAV();
    //wav->begin(file, out);

  //INICIALIZACION SERVO
    myservo1.attach(D2); 
    myservo2.attach(D0); 

  //Start tickers
    timer_gps.start();
    timer_dht11.start();
    timer_mq2.start();
}

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// LOOP /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void loop(){
  mqtt_loop();
  
  //COMPROBACIÓN DE ALTAVOZ
  if (wav->isRunning()) {
    if (!wav->loop()) wav->stop();
  }
  
  //ACTUALIZACIÓN DE SENSORES
  timer_gps.update();
  timer_dht11.update();
  timer_mq2.update();
  
//GPS
/* OPCION_1
  while (gpsSerial.available())
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
*/ 
/* OPCION_1.2
  while (gpsSerial.available() > 0)
    if (gps.encode(ss.read()))
    {
      displayInfo();
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        Serial.println(lat_str + lng_str);
      }
    }
*/
/* OPCION_2 
   while (gpsSerial.available() > 0) {
        if (gps.encode(gpsSerial.read())) {
            printData();
        }
    }
 */

WiFiClient client = server.available();
  if (client) //Si hay un cliente presente
  { 
    Serial.println("Nuevo Cliente");
    

    //esperamos hasta que hayan datos disponibles
    while(!client.available()&&client.connected()){
    delay(1);
    }
    
    // Leemos la primera línea de la petición del cliente.
    String linea1 = client.readStringUntil('r');
    Serial.println(linea1);

    if (linea1.indexOf("CALOR=ON")>0){ //Buscamos un CALOR=ON en la 1°Linea
      for (int angulo = 0; angulo <= 90; angulo += 1){ 
        myservo1.write(angulo);      
        myservo2.write(angulo);                      
        delay(10); 
      }
    }
    if (linea1.indexOf("FRIO=OFF")>0){//Buscamos un FRIO=OFF en la 1°Linea
      for (int angulo = 90; angulo >= 0; angulo -= 1){ 
        myservo1.write(angulo);  
        myservo2.write(angulo);              
        delay(10);  
      }
    }
    if (linea1.indexOf("BAJARSE")>0){
      for (int angulo = 0; angulo <= 180; angulo += 1){ 
        myservo1.write(angulo);      
        myservo2.write(180-angulo);              
        delay(20); 
      }
      delay(3000); 
      for (int angulo = 180; angulo >= 0; angulo -= 1){ 
        myservo1.write(180-angulo);
        myservo2.write(angulo);                
        delay(20);  
      }
    }
    if (linea1.indexOf("SUBIRSE")>0){
      for (int angulo = 0; angulo <= 180; angulo += 1){ 
        myservo1.write(180-angulo); 
        myservo2.write(angulo);                         
        delay(10); 
      }
      delay(3000); 
      for (int angulo = 180; angulo >= 0; angulo -= 1){ 
        myservo1.write(angulo);
        myservo2.write(180-angulo);                
        delay(10);  
      }
    }
    client.flush(); 
                
    Serial.println("Enviando respuesta...");   
    //Encabesado http    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");// La conexión se cierra después de finalizar de la respuesta
    client.println();
    //Pagina html  para en el navegador
    String s="<!DOCTYPE HTML>";
    s+="<html>";
    s+="<head><title>COVIDBUS</title>";
    s+="<body>";
    s+="<h1 align='center'> COVIDBUS </h1>";
    s+="<div style='text-align:center;'>";
    s+="Temperatura de hoy: "+temperatura_+" grados ||";
    s+=" Humedad de hoy: "+humedad_ +" % ||";
    s+=" Nivel de GAS de hoy: "+nivel_gas_;
    s+="<br/><br/>";
    s+="<button onClick=location.href='./?CALOR=ON'>CALOR</button>";
    s+="<button onClick=location.href='./?FRIO=OFF'>FRIO</button>";
    s+="<br/><br/>";
    s+="<div><button onClick=location.href='./?BAJARSE'>PULSA PARA BAJARTE</button></div>";
    s+="<div><button onClick=location.href='./?SUBIRSE'>PULSA PARA SUBIRTE</button></div>";
    s+="<br/><br/>";
    s+= "</body> </html> \n";
    client.println(s);
    delay(1);
    Serial.println("respuesta enviada");
    Serial.println();
  }
}
