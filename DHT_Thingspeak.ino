/* Using ESP8266 & DHT22 to send temperature & Humidity Data to Thingspeak
Tested & Improvised by - Harkrishan Singh (harkrishan@steamlabs.in)

#include <ESP8266WiFi.h>
#include "DHT.h"


/***************************
 * Begin Settings
 **************************/


const char* ssid     = "your router SSID"; //enter the SSID here
const char* password = "SSID password"; //enter the password for the SSID here

const char* host = "api.thingspeak.com";

const char* THINGSPEAK_API_KEY = "Thingspeak API Key"; //enter your Thingspeak API Key here

// DHT Settings
#define DHTPIN 2     // what digital pin we're connected to


// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

const boolean IS_METRIC = true; //IS_METRIC = true for temperature in C, false for F

// Update every 600 seconds = 10 minutes. Min with Thingspeak is ~20 seconds
const int UPDATE_INTERVAL_SECONDS = 600;

/***************************
 * End Settings
 **************************/
 
// Initialize the temperature/ humidity sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

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
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {      
    Serial.print("connecting to ");
    Serial.println(host);
    
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }

    // read values from the sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(!IS_METRIC);
    
    // We now create a URI for the request
    String url = "/update?api_key=";
    url += THINGSPEAK_API_KEY;
    url += "&field1=";
    url += String(temperature);
    url += "&field2=";
    url += String(humidity);
    
    Serial.print("Requesting URL: ");
    Serial.println(url);
    
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" + 
                 "Connection: close\r\n\r\n");
    delay(10);
    while(!client.available()){
      delay(100);
      Serial.print(".");
    }
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    
    Serial.println();
    Serial.println("closing connection");


  // Go back to sleep. If your sensor is battery powered you might
  // want to use deep sleep here
  delay(1000 * UPDATE_INTERVAL_SECONDS);
}
