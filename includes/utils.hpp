#pragma once
#include <algorithm>
#include <string>
#include <sstream>

inline std::string tolower(const std::string& str)
{
	std::string sent = str;
	transform(sent.begin(), sent.end(), sent.begin(), [](char c){ return tolower(c); });
	return sent;
}

template<typename T>
std::string to_string(const T& value)	// Generic to_string
{
    std::ostringstream out;

    out << value;
    return out.str();
}

#ifdef __CYGWIN__

#include <stdexcept>

inline int stoi(const std::string& str)
{
	std::istringstream in(str);
	int sent = 0;
	
	if (!(in >> sent))
		throw std::invalid_argument("Bad formatted number");
	return sent;
}

inline unsigned long stoul(const std::string& str)
{
	std::istringstream in(str);
	unsigned long sent = 0;
	
	if (!(in >> sent))
		throw std::invalid_argument("Bad formatted number");
	return sent;
}

#endif