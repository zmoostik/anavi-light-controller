#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include <WString.h>

int const pin_led = 2;
int const pin_r = 12;
int const pin_g = 13;
int const pin_b = 14;

class LightController {
public:
	int color_r = 0;
	int color_g = 0;
	int color_b = 0;
	int d65_r = 255;
	int d65_g = 140;
	int d65_b = 35;
	int brightness = 0;
	int temperature = 6500;
	bool power = false;
	bool led = false;

public:
	void begin();
	void write();
	void copy(LightController const &src);

	int getD65R() const;
	void setD65R(int const value);

	int getD65G() const;
	void setD65G(int const value);

	int getD65B() const;
	void setD65B(int const value);

	int getColorR() const;
	void setColorR(int const value);

	int getColorG() const;
	void setColorG(int const value);

	int getColorB() const;
	void setColorB(int const value);

	int getBrightness() const;
	void setBrightness(int const value);

	int getTemperature() const;
	void setTemperature(int const value);

	bool getPower() const;
	void setPower(bool const json);

	bool getLed() const;
	void setLed(bool const value);

	String getColorJSON() const;
	String getD65JSON() const;
	String getJSON() const;

	void setProperty(String const name, String const value);
};

#endif
