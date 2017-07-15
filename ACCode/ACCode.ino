
#include <IRremoteESP8266.h>

#include <ESP8266WiFi.h>

//Details of the WiFi network to connect
const char* ssid = "Thejan";
const char* password = "thejan9453";

// NETWORK: Static IP details...
IPAddress ip(192, 168, 43, 244); 
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


// PIN NUMBER DEFINING
//////////////////////////////////////////////////////   

int ledPin = 13;    // indicator led - D7
int recvPin = 2;   // ir receiver pin - D4
int sendPin = 14;  // ir sender pin - D5
int successPin = 4; // ir receieved successfully - D2 

/////////////////////////////////////////////////////

WiFiServer server(80);

IRrecv irrecv(recvPin);

decode_results results;

// DEFINING MODES
///////////////////////////////////////////////////
                                                //
boolean learningMode = false;                  //
boolean controlMode = false;                  //
boolean onOffMode = false;                   //
boolean tempUpMode = false;                 //
boolean tempDownMode = false;              //
                                          //
///////////////////////////////////////////


// DEFINING 3 VARIABLES TO STORE 3 IR CODES
/////////////////////////////////////////////////

//DEFINE HERE
      String jsonMessage = "";
//      String[] ac

////////////////////////////////////////////////


void setup() {

  // START SERIAL MONITORING
  Serial.begin(9600);
  delay(10);
  

  // SETTTING PIN MODES AND WRITING LOW
  pinMode(ledPin, OUTPUT);
  pinMode(successPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(successPin, LOW);


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

  irrecv.enableIRIn(); // Start the receiver
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
    //return;
  }
  
  // WAIT UNTILL CLIENT SENDS A REQUEST
  Serial.println("new client");
//  while(!client.available())
//  {
//    delay(1);
//  }
  
  // READING AND SERIAL PRINTING THE FIRST LINE OF THE REQUEST
  String request = client.readStringUntil('\n');
  String request2 = client.readStringUntil('\n');
  String request3 = client.readStringUntil('\n');
  String request4 = client.readStringUntil('\n');
  String request5 = client.readStringUntil('\n');
  String request6 = client.readStringUntil('\n');
  Serial.println(request);
  Serial.println(request2);
  Serial.println(request3);
  Serial.println(request4);
  Serial.println(request5);
  Serial.println(request6);
  client.flush();


  // COMPARING THE REQUEST
  if(request.indexOf("/LEARN=ON") != -1)  
  {
    learningMode = true;
    controlMode = false;

    Serial.println("Learning Mode ON");
    
    successResponse(client);
  }

  if(request.indexOf("/CONTROL=ON") != -1)  
  {
    controlMode = true;
    learningMode = false;

    Serial.println("Controlling Mode ON");

    successResponse(client);
  }

  
  // IF LEARNING MODE IS ON
  if(true)
  {

    Serial.println("Learning Mode ON");
    delay(500);

    // TURNING ON DIFFERENT MODES BASED ON SELECTED BUTTON IN ANDROID APP
    if(request.indexOf("/Mode=ONOFF") != -1)  {
      onOffMode = true;
      tempUpMode = false;
      tempDownMode = false;

      Serial.println("Learning Mode ON, ONOFF Mode ON");

      successResponse(client);
 
    } else if(request.indexOf("/Mode=TEMPUP") != -1)  {
      onOffMode = false;
      tempUpMode = true;
      tempDownMode = false;

      Serial.println("Learning Mode ON, TEMPUP Mode ON");      

      successResponse(client);
  
    } else if(request.indexOf("/Mode=TEMPDOWN") != -1)  {
      onOffMode = false;
      tempUpMode = false;
      tempDownMode = true;

      Serial.println("Learning Mode ON, TEMPDOWN Mode ON");

      successResponse(client);  
    }


    if(true){

      Serial.println("ONOFF Mode ON");
      delay(500);
      //assign the raw bits values to onOff bits variables


      // THIS CAN BE USED AS A REFERENCE CODE
      if (irrecv.decode(&results)) {

        jsonMessage = readIR();

      }
      
      delay(100);
      ///////////////////////////////////////

      // ONLY /Save IS ENOUGH ???
      if(request.indexOf("/Save=ONOFF") != -1)  {
          
          // send the bits in onOff bits variables to the mobile app (to be saved in SQLite database) as the response
          

          //sendJsonResponse(client, jsonMessage);

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
    
    successResponse(client);   
    
  }

  
  // IF CONTROLLING MODE IS ON
  if(controlMode)
  {

    if((request.indexOf("/AC=ON") != -1) || (request.indexOf("/AC=OFF") != -1) || (request.indexOf("/TEMP_UP") != -1) || (request.indexOf("/TEMP_DOWN") != -1)) 
    {

      Serial.println("in control mode");
      Serial.println();

      successResponse(client);
    }
       
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

void sendJsonResponse(WiFiClient client, String message){
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println(message);
    delay(1);
    Serial.println("IR code sent");
    Serial.println("message" + message);
}

String readIR(){
    digitalWrite(successPin, LOW);       
  
    decode_results *results_ptr = &results;

    int count = results_ptr->rawlen;

    String jsonMessage = "|";
    for (int i = 1; i < count-1; i++) {
      Serial.print((unsigned long) results_ptr->rawbuf[i]*USECPERTICK, DEC);
      jsonMessage+=(String)((unsigned long) results_ptr->rawbuf[i]*USECPERTICK);
      jsonMessage+=",";
      Serial.print(" ");
    }

    jsonMessage+=(String)((unsigned long) results_ptr->rawbuf[count-1]*USECPERTICK);
    jsonMessage+="|";

    Serial.println();
    Serial.println(jsonMessage);
    
    irrecv.resume(); // Receive the next value

    Serial.println("Value loaded. IR resumed");

    delay(200);
    digitalWrite(successPin, HIGH);

    return jsonMessage;
}

