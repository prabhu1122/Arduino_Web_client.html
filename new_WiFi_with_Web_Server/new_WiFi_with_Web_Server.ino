#include<ESP8266WiFi.h>
WiFiServer server(80);

const char* ssid = "prabhu";
const char* password = "1nightlight1";

int data;
int val;
const int relay1 = D4;
const int relay2 = D0;
void setup() {
  // put your setup code here, to run once:
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  digitalWrite(relay1,HIGH);
  Serial.begin(115200);
  delay(10);
  
  WiFi.begin(ssid,password);

  Serial.println(" ");
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }
  Serial.println(" ");
  Serial.print("A new network found...");
  Serial.print("connected to: ");
  Serial.println(ssid);
  Serial.print("Your IP address is: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if(!client){
    return;
  }

  Serial.println("New client is available");
  Serial.print("Waiting for data");

  while(!client.available()){
    delay(1000);
    Serial.print(".");
    val=0;
  }

  val=1;
  String request = client.readStringUntil('\r');
  Serial.println(" ");
  Serial.println(request);
  client.flush();


  if(request.indexOf("/RELAY1=ON") != -1){
    digitalWrite(relay1,LOW);
    data = 0;
  }

  if(request.indexOf("/RELAY1=OFF") != -1){
    digitalWrite(relay1,HIGH);
    data = 1;
  }

  if(request.indexOf("/RELAY2=ON") != -1){
    digitalWrite(relay2,LOW);
    data = 2;
  }

  if(request.indexOf("/RELAY2=OFF") != -1){
    digitalWrite(relay2,HIGH);
    data = 3;
  }
  //HTML COding//

  client.println("<!DOCTYPE html>");
  client.println("<html>");
  client.println("<titel><h1>Control Led with WiFi<h1></title>");
  client.println("<body>");
  
  if(data==0){
    client.println("Relay1 is ON.");
  }
  if(data==1){
    client.println("Relay1 is OFF.");
  }
  client.println("<nav class=\"navbar navbar-expand-md navbar-light bg-light sticky-left\">");
  client.println("<div>");
  client.println("<br>");
  client.println("<a href=\"/RELAY1=ON\"><button>Realy1 On</button></a>");
  client.println("<a href=\"/RELAY1=OFF\"><button>Relay1 Off</button></a>");
  client.println("<br><br>");

  if(data==2){
    client.println("Relay2 is ON.");
  }
  if(data==3){
    client.println("Relay2 is OFF.");
  }
  client.println("<br>");
  client.println("<a href=\"/RELAY2=ON\"><button>Realy2 On</button></a>");
  client.println("<a href=\"/RELAY2=OFF\"><button>Relay2 Off</button></a>");
  client.println("</div>");
  client.println("</nav>");
  client.println("</body>");
  client.println("</html>");

  delay(1);
  Serial.println("CLient Disconnected");
  Serial.println(" ");
}
