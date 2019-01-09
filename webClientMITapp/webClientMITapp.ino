#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>

WiFiServer server(80);

#define FIREBASE_HOST "homeautomation-237ac.firebaseio.com"                    //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "wcBF7LRWpPJdAf9XxlKtyBx9n0Zchppo5mkbUIX0"       //Your Firebase Database Secret goes here

const char* ssid ="Yadav ji ka wifi";
const char* password ="937i18ig";

const int Light = D1;
const int Fan = D2;
const int Tv = D5;
const int Ac = D6;
String data = "";
String Status = "";
int val1 = 0;
int val2 = 0;
int val3 = 0;
int val4 = 0;

void setup(){
  Serial.begin(115200);
  WiFi.disconnect();
  delay(300);
  pinMode(Light,OUTPUT);
  pinMode(Fan,OUTPUT);
  pinMode(Tv,OUTPUT);
  pinMode(Ac,OUTPUT);

  Serial.println("\n");
  Serial.print("Connecting to ");
  Serial.print(ssid);

 WiFi.begin(ssid,password);
 while(WiFi.status()!=WL_CONNECTED){
    delay(300);
    Serial.print(".");
 }
 Serial.println("\n");
 Serial.print("Connected to ");
 Serial.println(ssid);
 Serial.print("Your IP address is: ");
 Serial.println(WiFi.localIP());
 server.begin();
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.setInt("S1",0);                     //Here the varialbe"S1","S2","S3" and "S4" needs to be the one which is used in our Firebase and MIT App Inventor
  Firebase.setInt("S2",0); 
  Firebase.setInt("S3",0); 
  Firebase.setInt("S4",0);  
}

void loop(){
  
  WiFiClient client = server.available();
  while(!client){
    if (Firebase.failed()){
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());
      Serial.print("Trying to reconnect");
      Serial.print(".");
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
   }
   val1=Firebase.getString("S1").toInt();                                        //Reading the value of the varialble Status from the firebase
   val2=Firebase.getString("S2").toInt();                                        //Reading the value of the varialble Status from the firebase
   val3=Firebase.getString("S3").toInt();                                        //Reading the value of the varialble Status from the firebase
   val4=Firebase.getString("S4").toInt();                                        //Reading the value of the varialble Status from the firebase
   Serial.print(val1);
   Serial.print(val2);
   Serial.print(val3);
   Serial.println(val4);

      if (val1==1) {
          digitalWrite(Light,HIGH);
          Status = "lighton";
      } 
      if (val1==0) {
          digitalWrite(Light,LOW);
          Status = "lightoff";
      }
      if (val2==1) {
          digitalWrite(Fan,HIGH);
          Status = "fanon";
      }
      if (val2==0) {
          digitalWrite(Fan,LOW);
          Status = "fanoff";
      }
      if (val3==1) {
          digitalWrite(Tv,HIGH);
          Status = "tvon";
      }
      if (val3==0) {
          digitalWrite(Tv,LOW);
          Status = "tvoff";
      }
      if (val4==1) {
          digitalWrite(Ac,HIGH);
          Status = "acon";
      }
      if (val4==0) {
          digitalWrite(Ac,LOW);
          Status = "acoff";
      }
  }
  while(!client.available()){
    delay(1);
  }
  data=client.readStringUntil('\r');
  data.remove(0,5);
  data.remove(data.length()-9);
  Serial.println(data);
  Serial.println("data");
  client.flush();
  Serial.println("flush");
  
  if (data == "onLight") {
      digitalWrite(Light,HIGH);
      Status = "lighton";
  } 
  else if (data == "offLight") {
      digitalWrite(Light,LOW);
      Status = "lightoff";
  }
  else if (data == "onFan") {
      digitalWrite(Fan,HIGH);
      Status = "fanon";
  }
  else if (data == "offFan") {
      digitalWrite(Fan,LOW);
      Status = "fanoff";
  }
  else if (data == "onTv") {
      digitalWrite(Tv,HIGH);
      Status = "tvon";
  }
  else if (data == "offTv") {
      digitalWrite(Tv,LOW);
      Status = "tvoff";
  }
  else if (data == "onAc") {
      digitalWrite(Ac,HIGH);
      Status = "acon";
  }
  else if (data == "offAc") {
      digitalWrite(Ac,LOW);
      Status = "acoff";
  }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<body>");
    client.println("<h1>");
      if(Status=="lighton"){
        client.println("Light On");
      }
      else if(Status=="lightoff"){
        client.println("Light Off");
      }
      else if(Status=="fanon"){
        client.println("Fan On");
      }
      else if(Status=="fanoff"){
        client.println("Fan Off");
      }
      else if(Status=="tvon"){
        client.println("Tv On");
      }
      else if(Status=="tvoff"){
        client.println("TV Off");
      }
      else if(Status=="acon"){
        client.println("Ac On");
      }
      else if(Status=="acoff"){
        client.println("Ac Off");
      }
    client.println("</h1>");
    client.println("</body>");
    client.println("</html>");
    delay(1);
}

