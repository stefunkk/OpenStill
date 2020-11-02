#include "WifiServerTask.h"

WifiServerTaskClass::WifiServerTaskClass(ESP8266WiFiClass& wifi, WiFiServer& server):
_wifi(wifi), _server(server)
{
    _wifi.mode(WIFI_STA);
    _wifi.begin("", "");

	// Start the server
	_server.begin();
	Serial.println(F("Server started"));
}

void WifiServerTaskClass::exec()
{
    if (_wifi.status() != WL_CONNECTED)
    {
        Serial.println("Not connected");
    }

    Serial.println("wifi Task");
    Serial.println(_wifi.localIP());

    // Check if a client has connected
    WiFiClient client = _server.available();
    if (!client) {
        return;
    }
    Serial.println(F("new client"));

    client.setTimeout(100); // default is 1000

    // Read the first line of the request
    String req = client.readStringUntil('\r');
    Serial.println(F("request: "));
    Serial.println(req);

    // Match the request
    int val;
    if (req.indexOf(F("/gpio/0")) != -1) {
        val = 0;
    }
    else if (req.indexOf(F("/gpio/1")) != -1) {
        val = 1;
    }
    else {
        Serial.println(F("invalid request"));
        val = digitalRead(LED_BUILTIN);
    }

    // Set LED according to the request
    digitalWrite(LED_BUILTIN, val);

    // read/ignore the rest of the request
    // do not client.flush(): it is for output only, see below
    while (client.available()) {
        // byte by byte is not very efficient
        client.read();
    }

    // Send the response to the client
    // it is OK for multiple small client.print/write,
    // because nagle algorithm will group them into one single packet
    client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now "));
    client.print((val) ? F("high") : F("low"));
    client.print(F("<br><br>Click <a href='http://"));
    client.print(_wifi.localIP());
    client.print(F("/gpio/1'>here</a> to switch LED GPIO on, or <a href='http://"));
    client.print(_wifi.localIP());
    client.print(F("/gpio/0'>here</a> to switch LED GPIO off.</html>"));

    // The client will actually be *flushed* then disconnected
    // when the function returns and 'client' object is destroyed (out-of-scope)
    // flush = ensure written data are received by the other side
    Serial.println(F("Disconnecting from client"));
}

uint32_t WifiServerTaskClass::timeOfNextCheck()
{
    setTriggered(true);
	return millisToMicros(1000);
}
