#include <Esp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include "anaviLightController.h"
#include "ini.h"

String mdns_domain = "anavi-light-controller";
String const ap_ssid = "anavi-light-controller";
String const ap_password = "leonplovdiv";

AnaviLightController controller;
AnaviLightController default_settings;

ESP8266WebServer server(80);

void onNotFoundCb() {
	server.send(404, "text/plain", "Not Found");
}

void onLedCb() {
	if (server.method() == HTTP_PUT) { 
		controller.setLedJSON(server.arg("value"));
		controller.write();
	}
	server.send(200, "text/json", controller.getLedJSON());
}

void onPowerCb() {
	if (server.method() == HTTP_PUT) { 
		controller.power = server.arg("value") == "true";
		controller.write();
	}
	server.send(200, "text/json", controller.getPowerJSON());
}

void onColorCb() {
	if (server.method() == HTTP_PUT) { 
		controller.setColorRJSON(server.arg("r"));
		controller.setColorGJSON(server.arg("g"));
		controller.setColorBJSON(server.arg("b"));
		controller.write();
	}
	server.send(200, "text/json", controller.getColorJSON());
}

void onBrightnessCb() {
	if (server.method() == HTTP_PUT) { 
		controller.setBrightnessJSON(server.arg("value"));
		controller.write();
	}
	server.send(200, "text/json", controller.getBrightnessJSON());
}

void onTemperatureCb() {
	if (server.method() == HTTP_PUT) { 
		controller.setTemperatureJSON(server.arg("value"));
		controller.write();
	}
	server.send(200, "text/json", controller.getTemperatureJSON());
}

void onDefaultCb() {
	if (server.method() == HTTP_PUT) {
		for (int i = 0; i < server.args(); i++) {
			String name = server.argName(i);
			String value = server.arg(i);
			default_settings.setParamJSON(name, value);
			anaviConfigWrite(default_settings);			
		}
	}
	server.send(200, "text/json", default_settings.getJSON());
}

void onWifiCb() {
	if (server.method() == HTTP_PUT) {
		String ssid = server.arg("ssid");
		String password = server.arg("password");
		server.send(200, "text/json", "true");
		delay(1000);

		setdownWifi();
		WiFi.begin(ssid.c_str(), password.c_str());
		setupWifi();
	}
	else {
		String status = [](wl_status_t const status) {
			switch (status) {
				case WL_NO_SHIELD: return "NO_SHIELD";
				case WL_IDLE_STATUS: return "IDLE_STATUS";
				case WL_NO_SSID_AVAIL: return "NO_SSID_AVAIL";
				case WL_SCAN_COMPLETED: return "SCAN_COMPLETED";
				case WL_CONNECTED: return "CONNECTED";
				case WL_CONNECT_FAILED: return "CONNECT_FAILED";
				case WL_CONNECTION_LOST: return "CONNECTION_LOST";
				case WL_DISCONNECTED: return "DISCONNECTED";
			}
			return "";
		} (WiFi.status());
		server.send(200, "text/json", "{"
			"\"ssid\": \"" + WiFi.SSID() + "\", "
			"\"ether\": \"" + WiFi.macAddress() + "\", "
			"\"status\": \"" + status + "\""
		"}");
	}
}

bool setupAP() {
	WiFi.mode(WIFI_AP);
	
	Serial.print("Setting soft-AP: \"" + ap_ssid + "\" with password \"" + ap_password + "\"… ");
	bool result = WiFi.softAP(ap_ssid.c_str(), ap_password.c_str());
	Serial.println(result ? "Ok" : "Failed");
	if (!result)
		return false;
	
	Serial.print("Ap IP address: ");
	Serial.println(WiFi.softAPIP());
	return true;
}

bool setupSTA() {
	WiFi.mode(WIFI_STA);

	bool connected = false;
	for (int i = 0; i < 60; i++) {
		connected= WiFi.status() == WL_CONNECTED;
		if (connected)
			break;
	
		delay(500);
		Serial.print(".");
	}
	
	Serial.println(connected ? "Ok" : "Failed");
	if (!connected)
		return false;
		
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	Serial.println("Setting mDNS…");
	if (MDNS.begin(mdns_domain.c_str())) {
		Serial.println("MDNS responder started: " + mdns_domain);
	}

  	return true;
}

bool setupWifi() {
	if (!setupSTA()) {
		setupAP();
		//setLed(true);
		return false;
	}
	//setLed(false);
	return true;
}

void setdownWifi() {
	WiFi.disconnect();
	while (WiFi.status() == WL_CONNECTED) {
		delay(100);
	}
}

void anaviConfigWrite(AnaviLightController const controller) {
	Ini ini;
	ini.add("color.r", controller.getColorRJSON());
	ini.add("color.g", controller.getColorGJSON());
	ini.add("color.b", controller.getColorBJSON());
	ini.add("brightness", controller.getBrightnessJSON());
	ini.add("temperature", controller.getTemperatureJSON());
	ini.add("power", controller.getPowerJSON());
	ini.add("led", controller.getLedJSON());

	File f = SPIFFS.open("/settings.ini", "w");
	if (!f) {
		Serial.println("Could not open file");
		return;
	}
	f.println(ini.get());
	f.close();
}

void anaviConfigRead() {
	File f = SPIFFS.open("/settings.ini", "r");
	if (!f) {
    	Serial.println("Could not open setup file");
    	return;
	}

	Ini ini(f.readString());
	ini.parse([] (String const name, String const value) {
		default_settings.setParamJSON(name, value);
	});
}

void setup() {
	Serial.begin(115200);

	SPIFFS.begin();
	anaviConfigRead();

	controller.begin();
	controller.copy(default_settings);
	controller.write();
	
	setupWifi();

	Serial.print("Starting HTTP Server…");
	Serial.flush();
	
	server.on("/power", onPowerCb);
	server.on("/brightness", onBrightnessCb);
	server.on("/temperature", onTemperatureCb);
	server.on("/color", onColorCb);
	server.on("/led", onLedCb);
	server.on("/default", onDefaultCb);
	server.on("/wifi", onWifiCb);
	server.onNotFound(onNotFoundCb);
	server.serveStatic("/", SPIFFS, "/index.html");
	server.serveStatic("/css/style.css", SPIFFS, "/css/style.css");
	server.serveStatic("/js/client.js", SPIFFS, "/js/client.js");
	server.serveStatic("/js/toggleWidget.js", SPIFFS, "/js/toggleWidget.js");
	server.serveStatic("/js/colorWidget.js", SPIFFS, "/js/colorWidget.js");
	server.serveStatic("/js/brightnessWidget.js", SPIFFS, "/js/brightnessWidget.js");
	server.serveStatic("/js/temperatureWidget.js", SPIFFS, "/js/temperatureWidget.js");
	server.serveStatic("/js/defaultWidget.js", SPIFFS, "/js/defaultWidget.js");
	server.serveStatic("/js/setupWidget.js", SPIFFS, "/js/setupWidget.js");
	server.serveStatic("/js/wifiWidget.js", SPIFFS, "/js/wifiWidget.js");
	server.serveStatic("/img/settings_24px.png", SPIFFS, "/img/settings_24px.png");
	server.begin();
	Serial.println("Ok");
}

void loop() {
	server.handleClient();
}
