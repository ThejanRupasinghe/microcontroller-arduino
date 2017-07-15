#include <ESP8266WiFi.h>

//Details of the WiFi network to connect
const char* ssid = "Thejan";
const char* password = "thejan9453";

// NETWORK: Static IP details...
IPAddress ip(192, 168, 43, 76); 
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

 // DEFINING WIFI SERVER IN NODE MCU BOARD
WiFiServer server(80);

// PIN NUMBER DEFINING
//////////////////////////////////////////////////////   

int bulbPin = 12;   // main light - D6
int ledPin = 13;    // indicator led - D7

/////////////////////////////////////////////////////


void setup() {

  // START SERIAL MONITORING
  Serial.begin(9600);
  delay(10);
  

  // SETTTING PIN MODES AND WRITING LOW
  pinMode(bulbPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(bulbPin, LOW);
  digitalWrite(ledPin, LOW);
  
  // SERIAL PRINT ABOUT WIFI DETAILS
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  Serial.println();
  Serial.println();
  Serial.print("Setting static ip to : ");
  Serial.println(ip);


  // CONFIGURING STATIC IP OF THE BOARD
  WiFi.config(ip, gateway, subnet);
  
  // START WIFI TO CONNECT TO THE DEFINE NETWORK
  WiFi.begin(ssid, password);

  // DELAY AND SERIAL PRINTING WHILE WIFI CONNECTS
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // STARTING THE SERVER
  server.begin();
  Serial.println("Server started");
 
  // PRINTING THE IP DETAILS
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  // INDICATOR ON FOR SUCCESSFUL STEUP
  digitalWrite(ledPin,HIGH);
  
}
 
void loop() {

  // IF WIFI DISCONNETED OFF THE INDICATOR
  if (WiFi.status() != WL_CONNECTED) {
     digitalWrite(ledPin,LOW);
     return;
  }
  if (!(WiFi.status() != WL_CONNECTED)) {
     digitalWrite(ledPin,HIGH);
  }

  // CHECK IF THERE IS A CLIENT CONNECTION. CLIENT REQUEST
  WiFiClient client = server.available();
  if (!client) 
  {  
    return;
  }
  
  // WAIT UNTILL CLIENT SENDS A REQUEST
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }
  
  // READING AND SERIAL PRINTING THE FIRST LINE OF THE REQUEST
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
  // COMPARING THE REQUEST
  if(request.indexOf("/BULB=ON") != -1)  
  {
    digitalWrite(bulbPin, HIGH);

    // SENDS A SUCCESS RESPONSE TO THE ANDROID APP
    successResponse(client);
  }
  
  if(request.indexOf("/BULB=OFF") != -1)
  {
    digitalWrite(bulbPin, LOW);

    // SENDS A SUCCESS RESPONSE TO THE ANDROID APP
    successResponse(client);
  }
 
}

void successResponse(WiFiClient client){
    // SENDING THE CLIENT A EMPTY STRING RESPONSE ON SUCCESSFUL EXECUTION
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    delay(1);
    Serial.println("Client disonnected");
    Serial.println("");
}
