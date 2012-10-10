#include <SPI.h>
#include <Ethernet.h>
#include <WebServer.h>

const int statusPin =  2;
const int switchPin =  3;

/* CHANGE THIS TO YOUR OWN UNIQUE VALUE.  The MAC number should be
 * different from any other devices on your network or you'll have
 * problems receiving packets. */
static uint8_t mac[] = {  0x90, 0xA2, 0xDA, 0x0D, 0x6F, 0x1C  };


/* CHANGE THIS TO MATCH YOUR HOST NETWORK.  Most home networks are in
 * the 192.168.0.XXX or 192.168.1.XXX subrange.  Pick an address
 * that's not in use and isn't going to be automatically allocated by
 * DHCP from your router. */
static uint8_t ip[] = { 192, 168, 2, 42 };

/* This creates an instance of the webserver.  By specifying a prefix
 * of "", all pages will be at the root of the server. */
#define PREFIX ""
WebServer webserver(PREFIX, 8080);

/* commands are functions that get called by the webserver framework
 * they can read any posted data from client, and they output to the
 * server to send data back to the web browser. */
void helloCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  /* this line sends the standard "we're all OK" headers back to the
     browser */
  server.httpSuccess();


  /* if we're handling a GET or POST, we can output our data here.
     For a HEAD request, we just stop after outputting headers. */
  if (type != WebServer::HEAD)
  {
    /* this defines some HTML text in read-only memory aka PROGMEM.
     * This is needed to avoid having the string copied to our limited
     * amount of RAM. */
     P(onoff) ="'> Anschalten";
     if (digitalRead(statusPin) == HIGH) {
        P(onoff) = "on'> Ausschalten";
          digitalWrite(switchPin, HIGH);
     } else {
        P(onoff) ="'> Anschalten";
          digitalWrite(switchPin, LOW);
     }
     
    P(head) = "<!DOCTYPE HTML><html><head><title>SaunaControl</title>"
          "<style>body {background-color: #f5e4d1;background-position:center center;background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/bg.png)}#panel {width: 638px; height: 378px; margin: 100px auto; padding-top: 180px; background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/panel.png);}a:link, a:visited {display: block;width: 131px;height: 179px;margin: auto;background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/off.png);background-repeat: no-repeat;color: transparent;}a.on {background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/on.png);}a:hover, a:active {background-image: url(https://raw.github.com/maebert/SaunaControl/master/img/touch.png);}</style>"
          "<link rel='shortcut icon' href='https://raw.github.com/maebert/SaunaControl/master/img/apple-touch-icon-72x72.png' />"
          "<link rel='apple-touch-icon' sizes='57x57' href='https://raw.github.com/maebert/SaunaControl/master/img/apple-touch-icon-57x57.png' />"
          "<link rel='apple-touch-icon' sizes='72x72' href='https://raw.github.com/maebert/SaunaControl/master/img/apple-touch-icon-72x72.png' />"
          "<link rel='apple-touch-icon' sizes='144x144' href='https://raw.github.com/maebert/SaunaControl/master/img/apple-touch-icon-144x144.png' />"
          "</head><body><div id='panel'><a href='"
          "#"
          "' class='";
    P(foot) = "</a></div></body></html>";
    /* this is a special form of print that outputs from PROGMEM */
    server.printP(head);
    server.printP(onoff);
    server.printP(foot);
  }
}

void setup()
{
  pinMode(statusPin, INPUT);      
  pinMode(switchPin, OUTPUT);      

  /* initialize the Ethernet adapter */
  Ethernet.begin(mac, ip);

  /* setup our default command that will be run when the user accesses
   * the root page on the server */
  webserver.setDefaultCommand(&helloCmd);

  /* run the same command if you try to load /index.html, a common
   * default page name */
  webserver.addCommand("index.html", &helloCmd);

  /* start the webserver */
  webserver.begin();
}

void loop()
{
  char buff[64];
  int len = 64;

  /* process incoming connections one at a time forever */
  webserver.processConnection(buff, &len);
}


