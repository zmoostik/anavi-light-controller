#include "lightController.h"

#include <Arduino.h>

//from Redshift source code (https://github.com/jonls/redshift/blob/master/src/colorramp.c)
#define BLACKBODY_COLOR_LENGTH 71
const float blackbodyColor[BLACKBODY_COLOR_LENGTH][3] = {
	{1.00000000,  0.18172716,  0.00000000}, // 1000K
	{1.00000000,  0.25503671,  0.00000000}, // 1100K
	{1.00000000,  0.30942099,  0.00000000}, // 1200K
	{1.00000000,  0.35357379,  0.00000000}, // ...  
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
	{1.00000000,  1.00000000,  1.00000000}, // 6500K 
	{0.98947904,  0.99348723,  1.00000000},
	{0.97940448,  0.98722715,  1.00000000},
	{0.96975025,  0.98120637,  1.00000000},
	{0.96049223,  0.97541240,  1.00000000},
	{0.95160805,  0.96983355,  1.00000000}, // 7000K
	{0.94303638,  0.96443333,  1.00000000},
	{0.93480451,  0.95923080,  1.00000000},
	{0.92689056,  0.95421394,  1.00000000},
	{0.91927697,  0.94937330,  1.00000000},
	{0.91194747,  0.94470005,  1.00000000},
	{0.90488690,  0.94018594,  1.00000000},
	{0.89808115,  0.93582323,  1.00000000},
	{0.89151710,  0.93160469,  1.00000000},
	{0.88518247,  0.92752354,  1.00000000},
	{0.87906581,  0.92357340,  1.00000000}  // 8000K
};

void LightController::begin() {
	pinMode(pin_led, OUTPUT);
	pinMode(pin_r, OUTPUT);
	pinMode(pin_g, OUTPUT);
	pinMode(pin_b, OUTPUT);
}

void LightController::write() {
	int index = (temperature - 1000) / 100;
	if (index < 0) index = 0;
	if (index >= BLACKBODY_COLOR_LENGTH) index = BLACKBODY_COLOR_LENGTH - 1;

	analogWrite(pin_r, power ? color_r * brightness / 100.0 * blackbodyColor[index][0] * d65_r / 255.0 * 4.0 : 0);
	analogWrite(pin_g, power ? color_g * brightness / 100.0 * blackbodyColor[index][1] * d65_g / 255.0 * 4.0 : 0);
	analogWrite(pin_b, power ? color_b * brightness / 100.0 * blackbodyColor[index][2] * d65_b / 255.0 * 4.0 : 0);
	digitalWrite(pin_led, led ? LOW : HIGH);
}

void LightController::copy(LightController const &src) {
	power = src.power;
	brightness = src.brightness;
	temperature = src.temperature;
	color_r = src.color_r;
	color_g = src.color_g;
	color_b = src.color_b;
	d65_r = src.d65_r;
	d65_g = src.d65_g;
	d65_b = src.d65_b;
	led = src.led;
}

int LightController::getColorR() const {
	return color_r;
}

void LightController::setColorR(int const value) {
	color_r = value;
}

int LightController::getColorG() const {
	return color_g;
}

void LightController::setColorG(int const value) {
	color_g = value;
}

int LightController::getColorB() const {
	return color_b;
}

void LightController::setColorB(int const value) {
	color_b = value;
}

int LightController::getD65R() const {
	return d65_r;
}

void LightController::setD65R(int const value) {
	d65_r = value;
}

int LightController::getD65G() const {
	return d65_g;
}

void LightController::setD65G(int const value) {
	d65_g = value;
}

int LightController::getD65B() const {
	return d65_b;
}

void LightController::setD65B(int const value) {
	d65_b = value;
}

bool LightController::getPower() const {
	return power;
}

void LightController::setPower(bool const value) {
	power = value;
}

bool LightController::getLed() const {
	return led;
}

void LightController::setLed(bool const value) {
	led = value;
}

int LightController::getBrightness() const {
	return brightness;
}

void LightController::setBrightness(int const value) {
	brightness = value;
}

int LightController::getTemperature() const {
	return temperature;
}

void LightController::setTemperature(int const value) {
	temperature = value;
}

String LightController::getColorJSON() const {
	return "{\"r\": " + String(color_r) + ", \"g\": " + String(color_g) + ", \"b\": " + String(color_b) + "}";
}

String LightController::getD65JSON() const {
	return "{\"r\": " + String(d65_r) + ", \"g\": " + String(d65_g) + ", \"b\": " + String(d65_b) + "}";
}

String LightController::getJSON() const {
	return "{"
		"\"power\": " + String(power ? "true" : "false") + ", "
		"\"brightness\": " + String(brightness) + ", "
		"\"temperature\": " + String(temperature) + ", "
		"\"color\": " + getColorJSON() + ", "
		"\"d65\": " + getD65JSON() + ", "
		"\"led\": " + (led ? "true" : "false") + 
	"}";
}

void LightController::setProperty(String const name, String const value) {
	if (name == "power") {
		setPower(value == "true");
	}
	else if (name == "brightness") {
		setBrightness(value.toInt());
	}
	else if (name == "temperature") {
		setTemperature(value.toInt());
	}
	else if (name == "color.r") {
		setColorR(value.toInt());
	}
	else if (name == "color.g") {
		setColorG(value.toInt());
	}
	else if (name == "color.b") {
		setColorB(value.toInt());
	}
	else if (name == "d65.r") {
		setD65R(value.toInt());
	}
	else if (name == "d65.g") {
		setD65G(value.toInt());
	}
	else if (name == "d65.b") {
		setD65B(value.toInt());
	}
	else if (name == "led") {
		setLed(value == "true");
	}	
}
