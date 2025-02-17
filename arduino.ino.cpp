#include <ESP8266WiFi.h>
//#include <NewPing.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
float lat = 28.5458,lon = 77.1703;
SoftwareSerial gpsSerial(3,4);
TinyGPS gps;
String apiWritekey = "PXXGISH432AP4DQC"; // replace with your THINGSPEAK
WRITEAPI key here
const char* ssid = "Vyshnavi"; // your wifi SSID name
const char* password = "1234567890" ;// wifi pasword
const char* server = "api.thingspeak.com";
const int trigPin = D1;
const int echoPin = D2;
long duration;
int distance;
int safetyDistance;
WiFiClient client;
//NewPing sonar(trigPin, echoPin, distance);
void setup() {
 Serial.begin(9600);
 gpsSerial.begin(9600); // connect gps sensor
 WiFi.disconnect();
 delay(10);
 WiFi.begin(ssid, password);
 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.print(" ESP8266WiFi ( BIJEN ADHIKARI ) connected to wifi... ");
 Serial.println(ssid);
 Serial.println();
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input

}
void loop() {

// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
 if (client.connect(server,80))
 {
 String tsData = apiWritekey;
 tsData +="&field1=";
 tsData += String(distance);
 tsData += "\r\n\r\n";
 client.print("POST /update HTTP/1.1\n");
 client.print("Host: api.thingspeak.com\n");
 client.print("Connection: close\n");
 client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
 client.print("Content-Type: application/x-www-form-urlencoded\n");
 client.print("Content-Length: ");
 client.print(tsData.length());
 client.print("\n\n"); // the 2 carriage returns indicate closing of Header fields & starting of data
 client.print(tsData);
 Serial.print(" Amount of Garbage wastage in dustbin : ");
 Serial.print(distance);
 //Serial.print(",");
 //Serial.print(distance);
 Serial.println(" uploaded to Thingspeak server....");
 while(gpsSerial.available()){ // check for gps data
 if(gps.encode(gpsSerial.read()))// encode gps data
 {
 gps.f_get_position(&lat,&lon);
 Serial.print("Position: ");
 Serial.print("Latitude:");
 Serial.print(lat,6);
 Serial.print(";");
 Serial.print("Longitude:");
 Serial.println(lon,6);
 Serial.print(lat);
 Serial.print(" ");
 }
 client.stop();
 Serial.println(" Waiting to upload next reading...");
 Serial.println();
 // thingspeak needs minimum 15 sec delay between updates .... NOw lets upload the program
to wifi module....
 // IN this connection i have used arduino board to give 5V to ultrasonic sensor. and echo and trigger pin from ultrasonic sensor are connected to the wifi module .and ultrasonic sensor and wifi module are
 // grounded repectively, COM5 is for wifi module...
 delay(15);

}
 }
 String latitude = String(lat,6);
 String longitude = String(lon,6);
Serial.println(latitude+";"+longitude);
delay(1000);
}