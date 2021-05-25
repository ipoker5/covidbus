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

#include <Time.h>
#include "TimeLib.h"
#include <DS1307RTC.h>
#include <Wire.h>

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// VARIABLES GENERALES //////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// SENSOR DHT_11 //////////////////////////////////// 
#define DHTPIN D1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int id_dht11 = 77; String tipo_dht11 = "humedad_temperatura"; String nombre_dth11 = "DHT_11"; int ii = 0;

/////////////////////////////// SENSOR MQ-2 //////////////////////////////////// 
int id_mq2 = 90; String tipo_mq2 = "humo"; String nombre_mq2 = "MQ_2"; int jj = 100;

/////////////////////////////// SENSOR GPS ///////////////////////////////////////
/*
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
*/

/////////////////////////////// LIBRERIA TIME //////////////////////////////////////////////
/*const int timeZone = 1;    // Central European Time
time_t getNtpTime();
time_t prevDisplay = 0;
*/

/////////////////////////////// CONEXIONES //////////////////////////////////////////////
//variables para la conexión ESP8266/wifi
WiFiClient wifi_http;
IPAddress serverAddress(192, 168, 0, 15); 
int port_http = 8080;

HttpClient client_http = HttpClient(wifi_http, serverAddress, port_http); //cliente HTTP

//variables para MQTT
WiFiClient wifi_mqtt;
//IPAddress serverAddress(192, 168, 0, 15);
int port_mqtt = 1883;
const char usser_pass[7] = "admin";
long last_msg = 0;
char msg_mqtt[100];

PubSubClient client_mqtt(serverAddress,port_mqtt,wifi_mqtt); //cliente MQTT



//______________________________________________________________________________//
//______________________________________________________________________________//


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// FUNCIONES AUXILIARES /////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// CONEXIÓN WIFI ////////////////////////////////////

void setup_wifi(){
  WiFi.begin("vodafone7638", "N5ZXFUJGH5AK4Y");
  Serial.println("Connecting Wifi ...");
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}


/////////////////////////////// CONEXIÓN MQTT ////////////////////////////////////

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("\nMessage arrived [");
  Serial.print(topic);
  Serial.print("]\n ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
 
  if (strcmp(topic,"parpadea_led")){
    if ((char)payload[0] == '1') {
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  }
}
void mqtt_reconnect() { //procesa los msjs que recibimos de MQTT
  while (!client_mqtt.connected()) {
    Serial.println("Attempting MQTT connection...");
    String client_Id = "ESP8266Client";  //nombre de cliente
    client_Id += String(random(0xffff), HEX); 
    if (client_mqtt.connect(client_Id.c_str(),usser_pass,usser_pass)) { 
      Serial.print("\nMQTT connected:  ");
      Serial.print("ID Cliente: ");
      Serial.print(client_Id);
      Serial.println("");

      //client_mqtt.publish("casa/despacho/temperatura", "Enviando el primer mensaje");  //subscripcion o subscripciones
      client_mqtt.subscribe("parpadea_led");
    } else {
        Serial.print("Failed, rc=");
        Serial.println(client_mqtt.state());
        Serial.println("Try again in 5 seconds");
        Serial.println("");
        for(int i=5;i>0;i--){
            delay(1000);
            Serial.print(i);
            Serial.print(" ");
      }
    }
  }
}
void mqtt_setup(){
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


/////////////////////////////// TIMER ////////////////////////////////////
/*
void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}
void loop_timer()
{
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
    }
  }
}
*/


/////////////////////////////// DESERIALIZE ////////////////////////////////////

void deserialize_to_User(String response){
  if(response != ""){
      StaticJsonDocument<100> doc;
      deserializeJson(doc,response);
      DeserializationError error = deserializeJson(doc,response); //detecta si hay algun error en la deserializacion
      if(error){
            Serial.print("deserializeJson() --> failed");
            Serial.println(error.f_str());
      }else{
        String idusuario = doc["idusuario"];
        String nombre = doc["nombre"];
        String contrasena = doc["contraseña"];
        String ciudad = doc["ciudad"];
        Serial.println(idusuario);
        Serial.println(nombre);
        Serial.println(contrasena);
        Serial.println(ciudad);

      }
  }else{
    Serial.print("cuerpo vacio");
  }
}
void deserialize_to_Sensor(String response){
  if(response != ""){

      Serial.print("response: ");
      Serial.print(response);

      DynamicJsonDocument doc(200);
      deserializeJson(doc,response);
      Serial.print("doc: ");
      Serial.println(doc.as<String>());
      //Serial.printf(doc.getMember("valor"));
      DeserializationError error = deserializeJson(doc,response); //detecta si hay algun error en la deserializacion
      if(error){
            Serial.print("deserializeJson() --> failed");
            Serial.println(error.f_str());
      }else{
        int idtipo_sensor__ = doc["idtipo_sensor"].as<int>();
        float valor__ = doc["valor"].as<float>();
        int idsensor__ = doc["idsensor"].as<int>();
        Serial.println("doc: -------------------------------");
        Serial.println("idtipo_sensor: ");
        Serial.println(idtipo_sensor__);
        Serial.println("valor: ");
        Serial.println(valor__);
        Serial.println("idsensor: ");
        Serial.println(idsensor__);
        JsonArray array = doc.as<JsonArray>();
        Serial.println("array: ------------------------------");
        Serial.println(array);

        int idtipo_sensor = array[0]["idtipo_sensor"].as<int>();
        float valor = array[1]["valor"].as<float>();
        int idsensor = array[2]["idsensor"].as<int>();

        Serial.println("idtipo_sensor: ");
        Serial.println(idtipo_sensor);
        Serial.println("valor: ");
        Serial.println(valor);
        Serial.println("idsensor: ");
        Serial.println(idsensor);
      }
  }else{
    Serial.print("Cuerpo vacio");
  }
}
void deserialize_to_GPS(String response){
  if(response != ""){
      StaticJsonDocument<100> doc;
      DeserializationError error = deserializeJson(doc,response); //detecta si hay algun error en la deserializacion
      if(error){
            Serial.print("deserializeJson() --> failed");
            Serial.println(error.f_str());
      }else{
        int id =  doc["id"];
        float x = doc["x"];
        float y = doc["y"];
        long timestamp = doc["timestamp"];
        Serial.println(id);
        Serial.println(x);
        Serial.println(y);
        Serial.println(timestamp);
      }
  }else{
    Serial.print("cuerpo vacio");
  }
}
void deserialize_to_Actuador(String response){
  if(response != ""){
      StaticJsonDocument<100> doc;
      DeserializationError error = deserializeJson(doc,response); //detecta si hay algun error en la deserializacion
      if(error){
            Serial.print("deserializeJson() --> failed");
            Serial.println(error.f_str());
      }else{
        int idtipo_actuador = doc["idtipo_actuador"];
        int valor = doc["valor"];
        int modo = doc["modo"];
        int idactuador = doc["idactuador"];
        Serial.println(idtipo_actuador);
        Serial.println(valor);
        Serial.println(modo);
        Serial.println(idactuador);
      }
  }else{
    Serial.print("cuerpo vacio");
  }
}


/////////////////////////////// SERIALIZE //////////////////////////////////////

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
  //Serial.println(output);

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
  //Serial.println(output);
  return output;
}


/////////////////////////////// MÉTODOS REST PRUEBAS ///////////////////////////////////

void GET_tests(){
  //client_http.get("/api/usuario/8");
  client_http.get("/api/tipoSensor/1");

  // read the status code and body of the response
  int statusCode = client_http.responseStatusCode();
  String response = client_http.responseBody();

  Serial.println("GET con respuesta: ");
  Serial.println(statusCode);
  //Serial.println("Respuesta 1: ");
  Serial.println(response);
  //Serial.println("\n");
  //Serial.println("Respuesta 2: ");
  //deserialize_to_Sensor(response);
}
void POST_tests(){
  //para usar con post un string en forma de json, con un parametro variable, hay que concatenar los strings
  //forma manual:
  int temperatura_actual = 0;
  String postBody_t = " 'valor' : " + temperatura_actual;
  postBody_t = "{ 'idtipo_sensor' : 1234," + postBody_t + ", 'idsensor' : 1}";
  //------------------------------------------------------------------------------------------------------------------------------------
  //crear un string en forma de json, sin variables
  String postBody = "{'idusuario' : 7, 'nombre' : 'visualStudio', 'contraseña' : 'code', 'ciudad' : 'tusm'}";
  String value = "{\"idusuario\" : 7,\"nombre\" : \"visualStudio\",\"contraseña\" : \"code\",\"ciudad\" : \"tusm\"}";
  Serial.println(value);
  String contentType = "application/json";

  client_http.post("/api/PostUsuario/", contentType, value);
  int statusCode = client_http.responseStatusCode();
  String response = client_http.responseBody();
  Serial.println(statusCode);
  Serial.println(response);
  //------------------------------------------------------------------------------------------------------------------------------------
  //prueba1 --> te devuelve unicamente el codigo de estado de la peticion
  //client_http.post("/sensors", postBody.c_str())); //introducimos el cuerpo

  //prueba2 --> post con respuesta
  //client_http.post("/sensors", postBody.c_str(), &response); //introducimos el cuerpo, y escribimos en response la respuesta

  //prueba3 --> post con cabecera
  //client_http.setHeader("Content-Type: application/json");
  //client_http.post("/sensors", postBody.c_str()); //introducimos el cuerpo

  //prueba4 --> post cabecera Y respuesta
  //client_http.setHeader("Content-Type: application/json");
  //client_http.post("/sensors", postBody.c_str(), &response);
}
void PUT_tests(){

  //para usar con put un string en forma de json, con un parametro variable, hay que concatenar los strings
  //forma manual:
  int temperatura_actual = 0;
  String putBody_t = " 'valor' : " + temperatura_actual;
  putBody_t = "{ 'idtipo_sensor' : 1234,"+ putBody_t + ", 'idsensor' : 1}";
  //------------------------------------------------------------------------------------------------------------------------------------
  //crear un string en forma de json, sin variables
  String putBody = "{'idsensor' : 1234, 'tipo' : 'termico', 'nombre' : 'DHT11', 'iddispositivo' : 1}";
  //------------------------------------------------------------------------------------------------------------------------------------
  String contentType = "application/json";
  String value = "{\"idusuario\": 7,\"nombre\": \"visualStudio\",\"contraseña\": \"code\",\"ciudad\": \"tusm\"}";



  client_http.put("/api/putTipo_Sensor/1",contentType,"{\"idtipo_sensor\": 1,\"valor\": 60,\"idsensor\": 1}");
  int statusCode = client_http.responseStatusCode();
  String response = client_http.responseBody();
  Serial.println(statusCode);
  Serial.println(response);
  //prueba1 --> te devuelve unicamente el codigo de estado de la peticion
  //client_http.put("/sensors", putBody.c_str()); //introducimos el cuerpo

  //prueba2 --> put con respuesta
  //client_http.put("/sensors", putBody.c_str(), &response); //introducimos el cuerpo, y escribimos en response la respuesta

  //prueba3 --> put con cabecera
  //client_http.setHeader("Content-Type: application/json"); 
  //client_http.put("/sensors", putBody.c_str()); //introducimos el cuerpo

  //prueba4 --> put cabecera Y respuesta
  //client_http.setHeader("Content-Type: application/json");
  //client_http.put("/sensors", putBody.c_str(), &response); 
}


/////////////////////////////// FUNCIONES DE SENSORES ///////////////////////////

//SENSOR DHT11
void sensor_DHT11(){
  Serial.println("");
  Serial.print(("DHT11 test:"));
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
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
  Serial.println("");
  Serial.print("PUT DHT_11..............");
  String body_info = serialize_Sensor_Info(id_dht11, tipo_dht11, nombre_dth11, t, h, 1);
  client_http.beginRequest();
  client_http.put("/api/PutInfoSensor/333",contentType,body_info.c_str());
  Serial.println("");
  Serial.print("Code: ");
  Serial.print(client_http.responseStatusCode());
  Serial.println("");
  Serial.print("Body: ");
  Serial.print(client_http.responseBody());
  client_http.endRequest();
  
  delay(1500);

  //post datasensor --> timestamp  /  valor1  /  valor2  /  idsensor 
  //String time_ =  __TIME__; String date_ = __DATE__; String fecha = time_+"_"+date_;
  ii = ii + 1; String i = ""; i = i+ii;

  Serial.println("");
  Serial.print("POST DHT_11..............");
  String body_data = serialize_Sensor_Data(i, t, h, id_dht11);

  client_http.beginRequest();
  client_http.post("/api/Post_Data_Sensor/", contentType, body_data);
  int code = client_http.responseStatusCode();
  if(code==200 || code==201){
    Serial.println("");
    Serial.print("Code: ");
    Serial.print(code);
    Serial.println("");
    Serial.print("Body: ");
    Serial.print(client_http.responseBody());
   }else if(code == 401){
    Serial.println("");
    Serial.print("Code: ");
    Serial.print(code);
    Serial.print(" --> Fecha ya existente");
  }else{
    Serial.println("");
    Serial.print("Código de error: ");
    Serial.print(code);
  }
  client_http.endRequest();
  
}

//SENSOR MQ2
void sensor_MQ_2(){
  Serial.println("");
  Serial.print("MQ-2 test:");
  float h = analogRead(A0);
    if(isnan(h)){
      Serial.println("");
      Serial.print("ERROR NO DETECTA SENSOR MQ-2");
      return;
    }
  Serial.print("Nivel de gas: ");
  Serial.print(h/1023*100);
  delay(1500);

  //métodos rest
  String contentType = "application/json";

  //put infosensor ---> idsensor  /  tipo  /  nombre  /  last_value1  /  last_value2  /  iddispositivo 
  
  Serial.println("");
  Serial.print("PUT MQ-2.............. ");
  String body_info = serialize_Sensor_Info(id_mq2, tipo_mq2, nombre_mq2, h/1023*100, 00.00, 1);
  
  client_http.beginRequest();
  client_http.put("/api/PutInfoSensor/90", contentType, body_info.c_str());
  Serial.println("");
  Serial.print("Code: ");
  Serial.print(client_http.responseStatusCode());

  Serial.println("");
  Serial.print("Body: ");
  Serial.print(client_http.responseBody());
  client_http.endRequest();
  
  delay(1500);

  //post datasensor --> timestamp  /  valor1  /  valor2  /  idsensor 
  //String time_ =  __TIME__; String date_ = __DATE__; String fecha = time_+"__"+date_;
  jj = jj + 1; String j = ""; j = j + jj;
  
  Serial.println("");
  Serial.print("POST MQ-2.............."); 
  String body_data = serialize_Sensor_Data(j, h/1023*100, 00.00, id_mq2);

  client_http.beginRequest();
  client_http.post("/api/Post_Data_Sensor/", contentType, body_data);
  int code = client_http.responseStatusCode();
  if(code==200 || code==201){
    Serial.println("");
    Serial.print("Code: ");
    Serial.print(code);
    Serial.println("");
    Serial.print("Body: ");
    Serial.print(client_http.responseBody());
  }else if(code == 401){
    Serial.println("");
    Serial.print("Code: ");
    Serial.print(code);
    Serial.print(" --> Fecha ya existente");
  }else{
    Serial.println("");
    Serial.print("Código de error: ");
    Serial.print(code);
  }
  client_http.endRequest();
}

//SENSOR GPS
/*
void displayInfo()
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
*/

//______________________________________________________________________________//
//______________________________________________________________________________//


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// SETUP ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////// TICKERS //////////////////////////////////////////////
Ticker timer_dht11(sensor_DHT11, 7000);
Ticker timer_mq2(sensor_MQ_2, 10000);

void setup(){
  delay(3000);
  Serial.begin(9600);

  //INICIO CONEXIÓN WIFI
    Serial.println("Device started...");
    delay(1000);
    setup_wifi();

  //INICIO CONEXIÓN MQTT
    Serial.println("MQTT Started...");
    delay(1000);
    mqtt_setup();
  
  //INICIALIZACIÓN DE PERIFÉRICOS:
    String contentType = "application/json";
    dht.begin();
    delay(1000);

      //INICIALIZACIÓN DE DHT11 // post: idsensor  /  tipo  /  nombre  /  last_value1  /  last_value2  /  iddispositivo
      Serial.println("");
      Serial.print("POST DHT11 setup..............");
      String value_dht_11 = serialize_Sensor_Info(id_dht11, tipo_dht11, nombre_dth11, 00.00, 00.00, 1);

      client_http.beginRequest();
      client_http.post("/api/Post_Info_Sensor/", contentType, value_dht_11.c_str());
      int code_dht = client_http.responseStatusCode();
      if(code_dht==200 || code_dht==201){
        Serial.println("");
        Serial.print("Code: ");
        Serial.print(code_dht);
        Serial.println("");
        Serial.print("Body: ");
        Serial.print(client_http.responseBody());

      }else if(code_dht == 401){
        Serial.println("");
        Serial.print("Code: ");
        Serial.print(code_dht);
        Serial.print(" --> Sensor ya instalado");
      }else{
        Serial.println("");
        Serial.print("Código de error: ");
        Serial.print(code_dht);
      }
      client_http.endRequest();
      delay(1500);

      //INICIALIZACIÓN DE MQ-2 // post: idsensor  /  tipo  /  nombre  /  last_value1  /  last_value2  /  iddispositivo
      Serial.println("");
      Serial.print("POST MQ-2 setup..............");
      String body_info = serialize_Sensor_Info(id_mq2, tipo_mq2, nombre_mq2, 00.00, 00.00, 1);

      client_http.beginRequest();
      client_http.post("/api/Post_Info_Sensor/", contentType, body_info.c_str());
      int code_mq2 = client_http.responseStatusCode();
      if(code_mq2==200 || code_mq2==201){
        Serial.println("");
        Serial.print("Code: ");
        Serial.print(code_mq2);
        Serial.println("");
        Serial.print("Body: ");
        Serial.print(client_http.responseBody());
      }else if(code_mq2 == 401){
        Serial.println("");
        Serial.print("Code: ");
        Serial.print(code_mq2);
        Serial.print(" --> Sensor ya instalado");
      }else{
        Serial.println("");
        Serial.print("Código de error: ");
        Serial.print(code_mq2);
      }
      client_http.endRequest();
      delay(1500);

      //INICIALIZACIÓN GPS
      //ss.begin(GPSBaud);

  //Start tickers
  timer_dht11.start();
  timer_mq2.start();
}

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// LOOP /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


void loop()
{
  //GET_tests();
  //POST_tests();
  //PUT_tests();
  
  mqtt_loop();

  //ACTUALIZACIÓN DE SENSORES
  timer_dht11.update();
  timer_mq2.update();

      /*
 while (ss.available())
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
  */
  //loop_timer();


  /*
  tmElements_t tm;
  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);
  */
}
