#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include <string>
#include <iostream>

std::string utf16_to_utf8(const std::wstring& wstr);
std::wstring utf8_to_utf16(const std::string& str);

std::ostream& write_escaped_string(std::ostream& outStream, const std::string& toWrite);
std::istream& read_escaped_string(std::istream& inStream, std::string& toRead);


#endif // TEXT_UTILS_H