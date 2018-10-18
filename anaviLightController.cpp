#include "anaviLightController.h"

#include <Arduino.h>

//from Redshift source code (https://github.com/jonls/redshift/blob/master/src/colorramp.c)
const float blackbodyColor[56][3] = {
	{1.00000000,  0.18172716,  0.00000000}, /* 1000K */
	{1.00000000,  0.25503671,  0.00000000}, /* 1100K */
	{1.00000000,  0.30942099,  0.00000000}, /* 1200K */
	{1.00000000,  0.35357379,  0.00000000}, /* ...   */
	{1.00000000,  0.39091524,  0.00000000},
	{1.00000000,  0.42322816,  0.00000000},
	{1.00000000,  0.45159884,  0.00000000},
	{1.00000000,  0.47675916,  0.00000000},
	{1.00000000,  0.49923747,  0.00000000},
	{1.00000000,  0.51943421,  0.00000000},
	{1.00000000,  0.54360078,  0.08679949},
	{1.00000000,  0.56618736,  0.14065513},
	{1.00000000,  0.58734976,  0.18362641},
	{1.00000000,  0.60724493,  0.22137978},
	{1.00000000,  0.62600248,  0.25591950},
	{1.00000000,  0.64373109,  0.28819679},
	{1.00000000,  0.66052319,  0.31873863},
	{1.00000000,  0.67645822,  0.34786758},
	{1.00000000,  0.69160518,  0.37579588},
	{1.00000000,  0.70602449,  0.40267128},
	{1.00000000,  0.71976951,  0.42860152},
	{1.00000000,  0.73288760,  0.45366838},
	{1.00000000,  0.74542112,  0.47793608},
	{1.00000000,  0.75740814,  0.50145662},
	{1.00000000,  0.76888303,  0.52427322},
	{1.00000000,  0.77987699,  0.54642268},
	{1.00000000,  0.79041843,  0.56793692},
	{1.00000000,  0.80053332,  0.58884417},
	{1.00000000,  0.81024551,  0.60916971},
	{1.00000000,  0.81957693,  0.62893653},
	{1.00000000,  0.82854786,  0.64816570},
	{1.00000000,  0.83717703,  0.66687674},
	{1.00000000,  0.84548188,  0.68508786},
	{1.00000000,  0.85347859,  0.70281616},
	{1.00000000,  0.86118227,  0.72007777},
	{1.00000000,  0.86860704,  0.73688797},
	{1.00000000,  0.87576611,  0.75326132},
	{1.00000000,  0.88267187,  0.76921169},
	{1.00000000,  0.88933596,  0.78475236},
	{1.00000000,  0.89576933,  0.79989606},
	{1.00000000,  0.90198230,  0.81465502},
	{1.00000000,  0.90963069,  0.82838210},
	{1.00000000,  0.91710889,  0.84190889},
	{1.00000000,  0.92441842,  0.85523742},
	{1.00000000,  0.93156127,  0.86836903},
	{1.00000000,  0.93853986,  0.88130458},
	{1.00000000,  0.94535695,  0.89404470},
	{1.00000000,  0.95201559,  0.90658983},
	{1.00000000,  0.95851906,  0.91894041},
	{1.00000000,  0.96487079,  0.93109690},
	{1.00000000,  0.97107439,  0.94305985},
	{1.00000000,  0.97713351,  0.95482993},
	{1.00000000,  0.98305189,  0.96640795},
	{1.00000000,  0.98883326,  0.97779486},
	{1.00000000,  0.99448139,  0.98899179},
	{1.00000000, 1.00000000, 1.00000000} /* 6500K */
};

void AnaviLightController::begin() {
	pinMode(pin_led, OUTPUT);
	pinMode(pin_r, OUTPUT);
	pinMode(pin_g, OUTPUT);
	pinMode(pin_b, OUTPUT);
}

void AnaviLightController::write() {
	int index = (temperature - 1000) / 100;
	if (index < 0) index = 0;
	if (index > 55) index = 55;

	analogWrite(pin_r, power ? color_r * brightness / 100.0 * blackbodyColor[index][0] * 4.0 * 1.000 : 0);
	analogWrite(pin_g, power ? color_g * brightness / 100.0 * blackbodyColor[index][1] * 4.0 * 0.546 : 0);
	analogWrite(pin_b, power ? color_b * brightness / 100.0 * blackbodyColor[index][2] * 4.0 * 0.187 : 0);
	digitalWrite(pin_led, led ? LOW : HIGH);
}

void AnaviLightController::copy(AnaviLightController const &src) {
	power = src.power;
	brightness = src.brightness;
	temperature = src.temperature;
	color_r = src.color_r;
	color_g = src.color_g;
	color_b = src.color_b;
	led = src.led;
}

String AnaviLightController::getColorRJSON() const {
	return String(color_r);
}

void AnaviLightController::setColorRJSON(String const json) {
	color_r = json.toInt();
}

String AnaviLightController::getColorGJSON() const {
	return String(color_g);
}

void AnaviLightController::setColorGJSON(String const json) {
	color_g = json.toInt();
}

String AnaviLightController::getColorBJSON() const {
	return String(color_b);
}

void AnaviLightController::setColorBJSON(String const json) {
	color_b = json.toInt();
}

String AnaviLightController::getColorJSON() const {
	return "{\"r\": " + String(color_r) + ", \"g\": " + String(color_g) + ", \"b\": " + String(color_b) + "}";
}

String AnaviLightController::getPowerJSON() const {
	return power ? "true" : "false";
}

void AnaviLightController::setPowerJSON(String const json) {
	power = json == "true";
}

String AnaviLightController::getLedJSON() const {
	return led ? "true" : "false";
}

void AnaviLightController::setLedJSON(String const json) {
	led = json == "true";
}

String AnaviLightController::getBrightnessJSON() const {
	return String(brightness);
}

void AnaviLightController::setBrightnessJSON(String const json) {
	brightness = json.toInt();
}

String AnaviLightController::getTemperatureJSON() const {
	return String(temperature);
}

void AnaviLightController::setTemperatureJSON(String const json) {
	temperature = json.toInt();
}

String AnaviLightController::getJSON() const {
	return "{"
		"\"power\": " + getPowerJSON() + ", "
		"\"brightness\": " + getBrightnessJSON() + ", "
		"\"temperature\": " + getTemperatureJSON() + ", "
		"\"color\": " + getColorJSON() + ", "
		"\"led\": " + getLedJSON() + 
	"}";
}

void AnaviLightController::setParamJSON(String const name, String const value) {
	if (name == "power") {
		setPowerJSON(value);
	}
	else if (name == "brightness") {
		setBrightnessJSON(value);
	}
	else if (name == "temperature") {
		setTemperatureJSON(value);
	}
	else if (name == "color.r") {
		setColorRJSON(value);
	}
	else if (name == "color.g") {
		setColorGJSON(value);
	}
	else if (name == "color.b") {
		setColorBJSON(value);
	}
	else if (name == "led") {
		setLedJSON(value);
	}	
}

