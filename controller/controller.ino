/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

const int statusPin =  2;
const int switchPin =  3;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x6F, 0x1C };
IPAddress ip(192,168,2,42);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(8080);

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  pinMode(statusPin, INPUT);      
  pinMode(switchPin, OUTPUT);      

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html><head><title>SaunaControl</title>");
          client.println("<style>body {background-color: #f5e4d1;background-position:center center;background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/bg.png)}#panel {width: 638px; height: 378px; margin: 100px auto; padding-top: 180px; background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/panel.png);}a:link, a:visited {display: block;width: 131px;height: 179px;margin: auto;background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/off.png);background-repeat: no-repeat;color: transparent;}a.on {background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/on.png);}a:hover, a:active {background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/touch.png);}</style>");

          client.println("</head><body><div id='panel'><a href='");
          client.println("#");
          client.println("' class='");
          if (digitalRead(statusPin) == HIGH) {
            client.println("on'> Ausschalten");
          } else {
            client.println("'> Anschalten");
          }
          client.println("</a></div></body></html>");

          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

