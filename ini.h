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
	void add(String const name, int const value);
	void add(String const name, bool const value);
	String get();	
};

#endif
