/* Code to create a webserver using ESP8266 
and turn the LED on GPI0 ON or OFF
Tested & Improvised by - Harkrishan Singh (harkrishan@steamlabs.in)
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials.
This will create a Wi-Fi SSID with below mentioned details
*/
const char *ssid = "Microcloud";
const char *password = "987654321"; //ensure that the password is atleast 8 characters long
                                    //if there is any error in configuration you will see an
                                    //open network from ESP

ESP8266WebServer server(80);
const int led = 2; //GPIO2

/* Just a little test message. Go to http://192.168.4.1 in a web browser
* connected to this access point to see it.
*/

void handleRoot() {
int size=1000;
char temp[size];
int sec = millis() / 1000;
int min = sec / 60;
int hr = min / 60;
snprintf ( temp, size,
"<html>\
<head>\ 
<title>MicroCloud</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h3>You are connected on MicroCloud!</h3>\
<p>Uptime: %02d:%02d:%02d</p>\ 
<p>Status: Light ON</h1></p>\
<p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
<p></p>\
<p><a href=\"http://192.168.4.1/off\"><h1>Turn OFF</h1></a></p>\ 
</body>\
</html>",
hr, min % 60, sec % 60
);
server.send ( 200, "text/html", temp ); 
}

void setup() {
delay(1000);
Serial.begin(9600);
Serial.println();
Serial.print("Configuring access point...");
/* You can remove the password parameter if you want the AP to be open. */
WiFi.softAP(ssid, password);
IPAddress myIP = WiFi.softAPIP();
Serial.print("AP IP address: ");
Serial.println(myIP);
pinMode(led, OUTPUT);
digitalWrite ( led, HIGH );
//URLs available to query
server.on("/", handleRoot);
server.on ( "/on", turnON ); 
server.on ( "/off", turnOFF );
server.begin();
Serial.println("HTTP server started");
}

void turnON(){
digitalWrite ( led, HIGH );
int size=1000;
char temp[size];
int sec = millis() / 1000;
int min = sec / 60;
int hr = min / 60;
snprintf ( temp, size,
"<html>\
<head>\ 
<title>MicroCloud</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h3>Request: Light ON</h3>\
<p>Uptime: %02d:%02d:%02d</p>\
<p></p>\
<p>Status: Light ON</h1></p>\
<p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
<p></p>\
<p><a href=\"http://192.168.4.1/off\"><h1>Turn OFF</h1></a></p>\ 
</body>\
</html>",
 hr, min % 60, sec % 60
);
server.send ( 200, "text/html", temp);
}

void turnOFF(){
digitalWrite ( led, LOW );
int size=1000;
char temp[size];
int sec = millis() / 1000;
int min = sec / 60;
int hr = min / 60;
snprintf ( temp, size,
"<html>\
<head>\ 
<title>MicroCloud</title>\
<style>\
body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h3>Request: Light OFF</h3>\
<p>Uptime: %02d:%02d:%02d</p>\
<p>Status: Light OFF</p>\
<p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
<p></p>\
<p><a href=\"http://192.168.4.1/off\"><h1>Turn OFF</h1></a></p>\ 
</body>\
</html>",
hr, min % 60, sec % 60
);
server.send ( 200, "text/html", temp);
}

void loop() {
server.handleClient();
}
