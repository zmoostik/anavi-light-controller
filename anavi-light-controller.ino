#include <Esp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include "lightController.h"
#include "ini.h"

String mdns_domain = "anavi-light-controller";
String const ap_ssid = "anavi-light-controller";
String const ap_password = "leonplovdiv";

LightController controller;
LightController default_settings;

ESP8266WebServer server(80);

void onNotFoundCb() {
	server.send(404, "text/plain", "Not Found");
}

void onLedCb() {
	if (server.method() == HTTP_PUT) { 
		controller.setLed(server.arg("value") == "true");
		controller.write();
	}
	server.send(200, "text/json", controller.getLed() ? "true" : "false");
}

void onPowerCb() {
	if (server.method() == HTTP_PUT) { 
		controller.power = server.arg("value") == "true";
		controller.write();
	}
	server.send(200, "text/json", controller.getPower() ? "true" : "false");
}

void onColorCb() {
	if (server.method() == HTTP_PUT) { 
		controller.setColorR(server.arg("r").toInt());
		controller.setColorG(server.arg("g").toInt());
		controller.setColorB(server.arg("b").toInt());
		controller.write();
	}
	server.send(200, "text/json", controller.getColorJSON());
}

void onD65Cb() {
	if (server.method() == HTTP_PUT) { 
		controller.setD65R(server.arg("r").toInt());
		controller.setD65G(server.arg("g").toInt());
		controller.setD65B(server.arg("b").toInt());
		controller.write();
	}
	server.send(200, "text/json", controller.getD65JSON());
}

void onBrightnessCb() {
	if (server.method() == HTTP_PUT) { 
		controller.setBrightness(server.arg("value").toInt());
		controller.write();
	}
	server.send(200, "text/json", String(controller.getBrightness()));
}

void onTemperatureCb() {
	if (server.method() == HTTP_PUT) { 
		controller.setTemperature(server.arg("value").toInt());
		controller.write();
	}
	server.send(200, "text/json", String(controller.getTemperature()));
}

void onDefaultCb() {
	if (server.method() == HTTP_PUT) {
		for (int i = 0; i < server.args(); i++) {
			String name = server.argName(i);
			String value = server.arg(i);
			default_settings.setProperty(name, value);
			configWrite(default_settings);			
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
			"\"status\": \"" + status + "\", "
			"\"ip\": \"" + WiFi.localIP().toString() + "\""
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

void configWrite(LightController const controller) {
	Ini ini;
	ini.add("color.r", controller.getColorR());
	ini.add("color.g", controller.getColorG());
	ini.add("color.b", controller.getColorB());
	ini.add("d65.r", controller.getD65R());
	ini.add("d65.g", controller.getD65G());
	ini.add("d65.b", controller.getD65B());
	ini.add("brightness", controller.getBrightness());
	ini.add("temperature", controller.getTemperature());
	ini.add("power", controller.getPower());
	ini.add("led", controller.getLed());
	
	File f = SPIFFS.open("/settings.ini", "w");
	if (!f) {
		Serial.println("Could not open file");
		return;
	}
	f.println(ini.get());
	f.close();
}

void configRead() {
	File f = SPIFFS.open("/settings.ini", "r");
	if (!f) {
	    	Serial.println("Could not open setup file");
	    	return;
	}

	Ini ini(f.readString());
	ini.parse([] (String const name, String const value) {
		default_settings.setProperty(name, value);
	});
}

void setup() {
	Serial.begin(115200);

	SPIFFS.begin();
	configRead();

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
	server.on("/d65", onD65Cb);
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
