#include<ESP8266WiFi.h>

#define  yfsensor1 D2


#define trig_pin D5
#define echo_pin D6


volatile int flowing_frequency;
float waterquantity_prhour;


unsigned long currentTime1;
unsigned long cloopTime1;
WiFiClient client;
WiFiServer server(80);
int duration;
int level;

const char *ssid ="your SSID";
const char *password ="your Password";

const char* host = "Your Server link";





void rpm()
{
 flowing_frequency++;
 
}



void setup()
{
  Serial.begin(9600);
  pinMode(trig_pin,OUTPUT);
  pinMode(echo_pin,INPUT);
  setup_wifi();
  pinMode(yfsensor1,INPUT);
 
  

}

 void setup_wifi()
 {
  Serial.println();
  Serial.println("connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid , password);

  if (WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");

  }

  Serial.println("");
   Serial.println ("Wifi connected");
   Serial.println("IP adress");
   Serial.println(WiFi.localIP());
 }
 
void loop()
{
  delay(5000);

  digitalWrite(trig_pin,HIGH);
  delay(1000);
  digitalWrite(trig_pin,LOW);
  duration = pulseIn(echo_pin,HIGH);
  level = (duration/2) * 0.0343;


   attachInterrupt(4, rpm, RISING); // Setup Interrupt m
   sei();
   currentTime1 = millis();
   // Every second, calculate and print litres/hour
   if(currentTime1 >= (cloopTime1 + 1000))
   { Serial.println("yes");    
      cloopTime1 = currentTime1;              // Updates cloopTime
      waterquantity_prhour = (flowing_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
      flowing_frequency = 0;                   // Reset Counter
      Serial.print(waterquantity_prhour, DEC);            // Print litres/hour
      Serial.println(" L/hour");
   }
 
  
}
 
 void upload()
{
String tsData;
tsData="/Smartwatermeter/api/update/?field1="+String (waterquantity_prhour, DEC)+"&field2="+String (level, DEC);
Serial.println(tsData);
if (client.connect("your server link", 80))
{
client.print("POST "+tsData+" HTTP/1.1\n");
client.print("Host: your server link\n");
client.print("Connection: close\n");
//client.print("X-THINGSPEAKAPIKEY: "+channelsAPIKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(tsData.length());
client.print("\n\n");
client.print(tsData);

if (client.connected())
{
Serial.println("****************************************");
Serial.println("Update channels");
Serial.println();
Serial.println("Connecting to WaterManagement.");
Serial.println();
Serial.println("Server response ->");
Serial.println();
while(client.connected() && !client.available()) delay(1); //waits for data
while (client.connected() || client.available())
{
char charIn = client.read();
Serial.print(charIn);
//client.stop();
}
Serial.println();
Serial.println();
Serial.println("...disconnected");
Serial.println();
}

client.stop();
Serial.flush();
}
}
