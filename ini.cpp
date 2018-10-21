#include "ini.h"

Ini::Ini(String const text_) {
	text = text_;
}

void Ini::parse(void parseParam(String const, String const)) const {
	int from = 0;
	for (int offset = 0; offset < text.length(); offset++) {
		if (text.charAt(offset) == '\n') {
			String line = text.substring(from, offset);

			int pos = line.indexOf('=');
			if (pos != -1) {
				String key = line.substring(0, pos);
				key.trim();
				key.toLowerCase();
				
				String value = line.substring(pos + 1, line.length());
				value.trim();
				
				parseParam(key, value);
			}
	
			from = offset + 1;
		}
	}
}

void Ini::add(String const name, String const value) {
	text += name + "=" + value + "\n";
}

void Ini::add(String const name, int const value) {
	add(name, String(value));
}

void Ini::add(String const name, bool const value) {
	add(name, value ? "true" : "false");
}

String Ini::get() {
	return text;
}
