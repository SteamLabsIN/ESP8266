/* Using DHT with ESP8266, creating a Wi-Fi Access point from the ESP8266
Tested & Improvised by - Harkrishan Singh(harkrishan@steamlabs.in)
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <DHT.h>
#define DHTTYPE DHT22
#define DHTPIN  2

float humidity, temp_f;  // Values read from sensor
String webString="";     // String to display

// Generally, you should use "unsigned long" for variables that hold time

unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

// Initialize DHT sensor 
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01 

DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

/* Set these to your desired credentials.
This will create a Wi-Fi SSID with below mentioned details
*/

const char *ssid = "Microcloud_2";
const char *password = "987654321"; //ensure that the password is atleast 8 characters long
                                    //if there is any error in configuration you will see an
                                    //open network from ESP

ESP8266WebServer server(80);

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
<h3>You are connected on MicroCloud!Hello from the weather esp8266, read from /temp or /humidity</h3>\
<p>Uptime: %02d:%02d:%02d</p>\
<p><a href=\"http://192.168.4.1/temp\"><h1>Temperature</h1></a></p>\
<p></p>\
<p><a href=\"http://192.168.4.1/humidity\"><h1>Humidity</h1></a></p>\ 
</body>\
</html>",
hr, min % 60, sec % 60
);
server.send ( 200, "text/html", temp);
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
//URLs available to query
server.on("/", handleRoot);
server.on("/temp", [](){  // if you add this subdirectory to your webserver call, you get text below :)
    gettemperature();       // read sensor
    webString="Temperature: "+String((int)temp_f)+" C";   // Arduino has a hard time with float to string
    server.send(200, "text/plain", webString);            // send to someones browser when asked
  });
 
  server.on("/humidity", [](){  // if you add this subdirectory to your webserver call, you get text below :)
    gettemperature();           // read sensor
    webString="Humidity: "+String((int)humidity)+"%";
    server.send(200, "text/plain", webString);               // send to someones browser when asked
  });
server.begin();
Serial.println("HTTP server started");
}
void loop() {
server.handleClient();
}

void gettemperature() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   
 
    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp_f = dht.readTemperature();     // Read temperature as Fahrenheit
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}

//If you get avr/pgmspace.h not found error
//Copy the pgmspace.h & pgmspace.cpp file and
//paste at the following path
///Users/User/Library/Arduino15/packages/esp8266/hardware/esp8266/2.3.0/cores/esp8266/avr
//inside avr folder (will have to create a new folder)

//For DHT related override errors - install DHT library from Library Manager
//and look for DHT_sensor_library in your Arduino's Library folder
//Delete the DHT_U.h and DHT_U.cpp file
