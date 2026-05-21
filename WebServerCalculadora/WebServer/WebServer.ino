#include <Arduino.h>
#include <WebServer.h>
#include <uri/UriRegex.h>
#include <WiFi.h>
#include <iostream>
#include <string>

#include "secrets.h"  // add WLAN Credentials in here.

#include <FS.h>        // File System for Web Server Files

// mark parameters not used in example
#define UNUSED __attribute__((unused))

// TRACE output simplified, can be deactivated here
#define TRACE(...) Serial.printf(__VA_ARGS__)

// name of the server. You reach it using http://webserver
#define HOSTNAME "webserver"

// local time zone definition (Berlin)
#define TIMEZONE "CET-1CEST,M3.5.0,M10.5.0/3"

// need a WebServer for http access on port 80.
WebServer server(80);

// The text of builtin files are in this header file
#include "builtinfiles.h"


// ===== Simple functions used to answer simple GET requests =====

// This function is called when the sysInfo service was requested.
void calculator() {
  String op = server.pathArg(0);
  String a = server.pathArg(1);
  String b = server.pathArg(2); 

  int an = a.toInt();
  int bn = b.toInt();

  int res = 0;
  if (op == "p") {
    res = an + bn;
  } else {
    res = an - bn;
  }

  server.send(200, "application/json", "{res:" + String(res) + "}");
}  // handleSysInfo()


// Setup everything to make the webserver work.
void setup(void) {
  delay(3000);  // wait for serial monitor to start completely.

  // Use Serial port for some trace information from the example
  Serial.begin(115200);
  Serial.setDebugOutput(false);

  TRACE("Starting WebServer example...\n");

  // allow to address the device by the given name e.g. http://webserver
  WiFi.setHostname(HOSTNAME);

  // start WiFI
  WiFi.mode(WIFI_AP);
  if (strlen(ssid) == 0) {
    WiFi.begin();
  } else {
    WiFi.begin(ssid, passPhrase);
  }

  TRACE("Connect to WiFi...\n");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    TRACE(".");
  }
  TRACE("connected.\n");

  // Ask for the current time using NTP request builtin into ESP firmware.
  TRACE("Setup ntp...\n");
  configTzTime(TIMEZONE, "pool.ntp.org");

  TRACE("Register redirect...\n");

  TRACE("Register service handlers...\n");

  // register some REST services
  server.on(UriRegex("/(p|m)/(\d+)/(\d+)"), HTTP_GET, calculator);

  TRACE("Register file system handlers...\n");

  // enable CORS header in webserver results
  server.enableCORS(true);

  TRACE("Register default (not found) answer...\n");

  // handle cases when file is not found
  server.onNotFound([]() {
    // standard not found in browser.
    server.send(404, "text/html", FPSTR(notFoundContent));
  });

  server.begin();

  TRACE("open <http://%s> or <http://%s>\n", WiFi.getHostname(), WiFi.localIP().toString().c_str());
}  // setup

// run the server...
void loop(void) {
  server.handleClient();
}  // loop()

// end.
