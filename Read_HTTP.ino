/* To read data from an HTTP Website
Tested & Improvised by - Harkrishan Singh (harkrishan@steamlabs.in)
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
 
// WiFi information
const char WIFI_SSID[] = "your router SSID"; //enter your router's SSID here
const char WIFI_PSK[] = "your SSID password"; // enter the password for the SSID here
 
// Remote site information
const char http_site[] = "www.example.com";
const int http_port = 80;
 
// Global variables
WiFiClient client;
 
void setup() {
  
  // Set up serial console to read web page
  Serial.begin(115200);
  Serial.print("Thing GET Example");
  
  // Connect to WiFi
  connectWiFi();
  
  // Attempt to connect to website
  if ( !getPage() ) {
    Serial.println("GET request failed");
  }
}
 
void loop() {
  
  // If there are incoming bytes, print them
  if ( client.available() ) {
    char c = client.read();
    Serial.print(c);
  }
  
  // If the server has disconnected, stop the client and WiFi
  if ( !client.connected() ) {
    Serial.println("Server Disconnected");
    
    // Close socket and wait for disconnect from WiFi
    client.stop();
    if ( WiFi.status() != WL_DISCONNECTED ) {
      WiFi.disconnect();
    }

    // Do nothing
    Serial.println("Finished Thing GET test");
    while(true){
      delay(1000);
    }
  }
}
 
// Attempt to connect to WiFi
void connectWiFi() {
  
  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);
  
  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PSK);
  
  while ( WiFi.status() != WL_CONNECTED ) {
    Serial.println("Connecting...");
    delay(1000);
  }

}
 
// Perform an HTTP GET request to a remote page
bool getPage() {
  
  // Attempt to make a connection to the remote server
  if ( !client.connect(http_site, http_port) ) {
    return false;
  }
  
  // Make an HTTP GET request
  client.println("GET /index.html HTTP/1.1"); //if fetching a sub-url replace index.html with the sub-url
  client.print("Host: ");
  client.println(http_site);
  client.println("Connection: close");
  client.println();
  
  return true;
}
