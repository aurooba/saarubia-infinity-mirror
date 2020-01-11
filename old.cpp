// // /**
// //  * Blink
// //  *
// //  * Turns on an LED on for one second,
// //  * then off for one second, repeatedly.
// //  */
// // #include "Arduino.h"
// // #include "setupwifi.cpp"

// // // Set LED_BUILTIN if it is not defined by Arduino framework
// // // #define LED_BUILTIN 13

// // void setup()
// // {
// //   // initialize LED digital pin as an output.
// //   pinMode(LED_BUILTIN, OUTPUT);
// // }

// // void loop()
// // {
// //   // turn the LED on (HIGH is the voltage level)
// //   digitalWrite(LED_BUILTIN, LOW);

// //   // wait for a second
// //   delay(300);

// //   // turn the LED off by making the voltage LOW
// //   digitalWrite(LED_BUILTIN, HIGH);

// //     // wait for a second
// //   delay(300);

// // }

// /*
//  * Sketch: ESP8266_LED_Control_02
//  * Control an LED from a web browser
//  * Intended to be run on an ESP8266
//  * 
//  * connect to the ESP8266 AP then
//  * use web broswer to go to 192.168.4.1
//  * 
//  */
 
 
// #include <ESP8266WiFi.h>
// #include <ArduinoJson.h>
// #include <ConfigManager.h>

// const char WiFiPassword[] = "anewkingdom0804";
// const char AP_NameChar[] = "ControlPoint";

// struct Config {
//     char name[20];
//     bool enabled;
//     int8_t hour;
//     char password[20];
// } config;

// struct Metadata {
//     int8_t version;
// } meta;

// ConfigManager configManager;

 
// WiFiServer server(80);
 
// String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
// String html_1 = "<!DOCTYPE html><html><head><title>LED Control</title></head><body><div id='main'><h2>LED Control</h2>";
// String html_2 = "<form id='F1' action='LEDON'><input class='button' type='submit' value='LED ON' ></form><br>";
// String html_3 = "<form id='F2' action='LEDOFF'><input class='button' type='submit' value='LED OFF' ></form><br>";
// String html_4 = "</div></body></html>";
 
// String request = "";
// // #define LED_BUILTIN 13
 
// void setup() 
// {
//     pinMode(LED_BUILTIN, OUTPUT); 
 
//     WiFi.softAP(AP_NameChar, WiFiPassword);
//     server.begin();

//   configManager.setAPName("Demo");
//   configManager.setAPFilename("/index.html");
//   configManager.addParameter("name", config.name, 20);
//   configManager.addParameter("enabled", &config.enabled);
//   configManager.addParameter("hour", &config.hour);
//   configManager.addParameter("password", config.password, 20, set);
//   configManager.addParameter("version", &meta.version, get);
//   configManager.begin(config);
 
// } // void setup()
 
 
// void loop() 
// {
//     configManager.loop();
//     // Check if a client has connected
//     WiFiClient client = server.available();
//     if (!client)  {  return;  }
 
//     // Read the first line of the request
//     request = client.readStringUntil('\r');
 
//     if       ( request.indexOf("LEDON") > 0 )  { digitalWrite(LED_BUILTIN, LOW);  }
//     else if  ( request.indexOf("LEDOFF") > 0 ) { digitalWrite(LED_BUILTIN, HIGH);   }
 
//     client.flush();
 
//     client.print( header );
//     client.print( html_1 );
//     client.print( html_2 );
//     client.print( html_3 );
//     client.print( html_4);

//     delay(5);
//   // The client will actually be disconnected when the function returns and 'client' object is detroyed
 
// } // void loop()

// #include "ESP8266mDNS.h"
#include <Bounce2.h>
#include <PubSubClient.h>
#include "ConfigManager.h"
#include "WiFiClient.h"

// WiFiServer server(80);

const char *settingsHTML = (char *)"/wifi.html";

// MQTT
// Make sure to update this for your own MQTT Broker!
const char* mqtt_server = "192.168.95.255";
const char* mqtt_topic = "test";
const char* mqtt_username = "saarubia";
const char* mqtt_password = "anewkingdom0804";
// The client id identifies the ESP8266 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* clientID = "Client ID";

// Initialise the Pushbutton Bouncer object
Bounce bouncer = Bounce();

PubSubClient client(mqtt_server, 1883);

struct Config {
    char ssid[20];
    bool enabled;
    char password[20];
} config;

struct Metadata {
    int8_t version;
} meta;

ConfigManager configManager;

void createCustomRoute(WebServer *server) {
    server->on("/wifi.html", HTTPMethod::HTTP_GET, [server](){
        configManager.streamFile("/wifi.html", mimeHTML);
    });
    server->on("/disconnect", HTTPMethod::HTTP_GET, [server](){
      configManager.clearWifiSettings(false);
    });
    // server->on("/custom", HTTPMethod::HTTP_GET, [server](){
    //     server->send(200, "text/plain", "HI LOVES!");
    // });
 
    // Read the first line of the request
    // request = server.readStringUntil('\r');
 
    // if       ( server.indexOf("LEDON") > 0 )  { digitalWrite(LED_BUILTIN, LOW);  }
    // else if  ( server.indexOf("LEDOFF") > 0 ) { digitalWrite(LED_BUILTIN, HIGH);   }

    // delay(5);
  // The client will actua
}

void setUpWifi(WebServer *server) {
    server->on("/", HTTPMethod::HTTP_GET, [server](){
        configManager.streamFile("/wifi.html", mimeHTML);
    });
    // server->on("/custom", HTTPMethod::HTTP_GET, [server](){
    //     server->send(200, "text/plain", "HI LOVES!");
    // });
 
    // Read the first line of the request
    // request = server.readStringUntil('\r');
 
    // if       ( server.indexOf("LEDON") > 0 )  { digitalWrite(LED_BUILTIN, LOW);  }
    // else if  ( server.indexOf("LEDOFF") > 0 ) { digitalWrite(LED_BUILTIN, HIGH);   }

    // delay(5);
  // The client will actua
}

void setup() {
    DEBUG_MODE = true; // will enable debugging and log to serial monitor
    // pinMode(LED_BUILTIN, OUTPUT); 
    Serial.begin(9600);
    

    meta.version = 3;

    // Setup config manager
    configManager.setAPName("Connect to ESP8266");
    configManager.setAPFilename("/index.html");
    configManager.addParameter("ssid", config.ssid, 20);
    configManager.addParameter("password", config.password, 20, set);

    configManager.setAPCallback(createCustomRoute);
    configManager.setAPICallback(createCustomRoute);

    if (client.connect(clientID, mqtt_username, mqtt_password)) {
        Serial.println("Connected to MQTT Broker!");
    }
    else {
        Serial.println("Connection to MQTT Broker failed...");
    }
  
    
    // DebugPrintln("config.ssid");
    // DebugPrintln("starting stuff");  
    // Serial.println(WiFi.localIP());
    // if (!MDNS.begin("esp8266")) {             // Start the mDNS responder for esp8266.local
    //     Serial.println("Error setting up MDNS responder!");
    // }
    // Serial.println("mDNS responder started");
    // // server.beginx/();
    // // Add service to MDNS-SD
    // MDNS.addService("http", "tcp", 80);
    configManager.begin(config);
    
}

void loop() {
    configManager.loop();
    
    // DebugPrintln(config.password);
    // configManager.save(config);
    // Add your loop code here
}