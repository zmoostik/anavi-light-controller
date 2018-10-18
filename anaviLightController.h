#ifndef ANAVI_H
#define ANAVI_H

#include <WString.h>

int const pin_led = 2;
int const pin_r = 12;
int const pin_g = 13;
int const pin_b = 14;

class AnaviLightController {
public:
	int color_r = 0;
	int color_g = 0;
	int color_b = 0;
	int brightness = 0;
	int temperature = 6500;
	bool power = false;
	bool led = false;

public:
	void begin();
	void write();
	void copy(AnaviLightController const &src);
	String getColorJSON() const;
	String getColorRJSON() const;
	void setColorRJSON(String const json);
	String getColorGJSON() const;
	void setColorGJSON(String const json);
	String getColorBJSON() const;
	void setColorBJSON(String const json);
	String getBrightnessJSON() const;
	void setBrightnessJSON(String const json);
	String getTemperatureJSON() const;
	void setTemperatureJSON(String const json);
	String getPowerJSON() const;
	void setPowerJSON(String const json);
	String getLedJSON() const;
	void setLedJSON(String const json);
	String getJSON() const;
	void setParamJSON(String const name, String const value);
};

#endif
