
#include <IRremoteESP8266.h>

#include <ESP8266WiFi.h>

//Details of the WiFi network to connect
const char* ssid = "Thejan";
const char* password = "thejan9453";

// NETWORK: Static IP details...
IPAddress ip(192, 168, 43, 74); 
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


// PIN NUMBER DEFINING
//////////////////////////////////////////////////////   

int ledPin = 13;    // indicator led - D7
int recvPin = 2;   // ir receiver pin - D4

/////////////////////////////////////////////////////

WiFiServer server(80);

IRrecv irrecv(recvPin);

decode_results results;

// DEFINING MODES
///////////////////////////////////////////////////

boolean learningMode = false;
boolean controlMode = false;
boolean onOffMode = false;
boolean tempUpMode = false;
boolean tempDownMode = false;

//////////////////////////////////////////////////


// DEFINING 3 VARIABLES TO STORE 3 IR CODES
/////////////////////////////////////////////////

DEFINE HERE

////////////////////////////////////////////////


void setup() {

  // START SERIAL MONITORING
  Serial.begin(9600);
  delay(10);
  

  // SETTTING PIN MODES AND WRITING LOW
  pinMode(ledPin, OUTPUT);

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
  if(request.indexOf("/LEARN=ON") != -1)  
  {
    learningMode = true;
    controlMode = false;

    Serial.println("Learning Mode ON");

    irrecv.enableIRIn(); // Start the receiver
    
    // SENDING THE CLIENT A EMPTY STRING RESPONSE ON SUCCESSFUL EXECUTION
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    delay(1);
    Serial.println("Client disonnected");
    Serial.println("");
  }

  if(request.indexOf("/CONTROL=ON") != -1)  
  {
    controlMode = true;
    learningMode = false;

    Serial.println("Controlling Mode ON");

    // SENDING THE CLIENT A EMPTY STRING RESPONSE ON SUCCESSFUL EXECUTION
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    delay(1);
    Serial.println("Client disonnected");
    Serial.println("");
  }

  
  // IF LEARNING MODE IS ON
  if(learningMode)
  {

    // TURNING ON DIFFERENT MODES BASED ON SELECTED BUTTON IN ANDROID APP
    if(request.indexOf("/Mode=ONOFF") != -1)  {
      onOffMode = true;
      tempUpMode = false;
      tempDownMode = false;

      Serial.println("Learning Mode ON, ONOFF Mode ON");

      // SENDING THE CLIENT A EMPTY STRING RESPONSE ON SUCCESSFUL EXECUTION
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      delay(1);
      Serial.println("Client disonnected");
      Serial.println("");
 
    } else if(request.indexOf("/Mode=TEMPUP") != -1)  {
      onOffMode = false;
      tempUpMode = true;
      tempDownMode = false;

      Serial.println("Learning Mode ON, TEMPUP Mode ON");      

      // SENDING THE CLIENT A EMPTY STRING RESPONSE ON SUCCESSFUL EXECUTION
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      delay(1);
      Serial.println("Client disonnected");
      Serial.println("");
  
    } else if(request.indexOf("/Mode=TEMPDOWN") != -1)  {
      onOffMode = false;
      tempUpMode = false;
      tempDownMode = true;

      Serial.println("Learning Mode ON, TEMPDOWN Mode ON");
      
      // SENDING THE CLIENT A EMPTY STRING RESPONSE ON SUCCESSFUL EXECUTION
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      delay(1);
      Serial.println("Client disonnected");
      Serial.println("");
  
    }


    if(onOffMode){

      //assign the raw bits values to onOff bits variables

      // THIS CAN BE USED AS A REFERENCE CODE
      if (irrecv.decode(&results)) {
        Serial.println(results.value, HEX);
  
        decode_results *results_ptr = &results;
  
        int count = results_ptr->rawlen;
        
        for (int i = 1; i < count; i++) {
          if (i & 1) {
            Serial.print(results_ptr->rawbuf[i]*USECPERTICK, DEC);
          }
          else {
            Serial.write('-');
            Serial.print((unsigned long) results_ptr->rawbuf[i]*USECPERTICK, DEC);
          }
          Serial.print(" ");
        }
        
        irrecv.resume(); // Receive the next value
      }
      delay(100);
      ///////////////////////////////////////

      // ONLY /Save IS ENOUGH ???
      if(request.indexOf("/Save=ONOFF") != -1)  {
          
          // send the bits in onOff bits variables to the mobile app (to be saved in SQLite database) as the response

      }
      
    } else if (tempUpMode){
      
      //assign the raw bits values to tempUp bits variables

      if(request.indexOf("/Save=TEMPUP") != -1)  {
          
          // send the bits in tempUp bits variables to the mobile app (to be saved in SQLite database) as the response

      }
      
    } else if (tempDownMode){

      //assign the raw bits values to tempDown bits variables

      if(request.indexOf("/Save=TEMPDOWN") != -1)  {
          
          // send the bits in tempDown bits variables to the mobile app (to be saved in SQLite database) as the response

      }
      
    }
    
    
    // SENDING THE CLIENT A EMPTY STRING RESPONSE ON SUCCESSFUL EXECUTION
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    delay(1);
    Serial.println("Client disonnected");
    Serial.println("");   
    
  }

  
  // IF CONTROLLING MODE IS ON
  if(controlMode)
  {

    if((request.indexOf("/AC=ON") != -1) || (request.indexOf("/AC=OFF") != -1) || (request.indexOf("/TEMP_UP") != -1) || (request.indexOf("/TEMP_DOWN") != -1)) 
    {

      Serial.println("in control mode");
      
      // SENDING THE CLIENT A EMPTY STRING RESPONSE ON SUCCESSFUL EXECUTION
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      delay(1);
      Serial.println("Client disonnected");
      Serial.println("");
    }
    
    
  }

  
}
