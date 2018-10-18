#ifndef INI_H
#define INI_H

#include <WString.h>

class Ini {
private:
	String text;

public:
	Ini(String const text = "");
	void parse(void parseParam(String const, String const)) const;
	void add(String const name, String const value);
	String get();	
};

#endif
