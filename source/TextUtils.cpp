#include "TextUtils.h"

#include <windows.h>

std::string utf16_to_utf8(const std::wstring& wstr)
{
    std::string str;

    size_t reqSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (reqSize > 0) {
        str.resize(reqSize);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], reqSize, NULL, NULL);
    }

    return str;
}

std::wstring utf8_to_utf16(const std::string& str)
{
    std::wstring wstr;

    size_t reqSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (reqSize > 0) {
        wstr.resize(reqSize);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], reqSize);
    }

    return wstr;
}

std::ostream& write_escaped_string(std::ostream& outStream, const std::string& toWrite)
{
    std::string converted;

    for (char c : toWrite) {
        switch (c)
        {
        case 0x07:
            converted += "\\a";
        break;
        case 0x08:
            converted += "\\b";
        break;
        case 0x09:
            converted += "\\t";
        break;
        case 0x0A:
            converted += "\\n";
        break;
        case 0x0B:
            converted += "\\v";
        break;
        case 0x0C:
            converted += "\\f";
        break;
        case 0x0D:
            converted += "\\r";
        break;
        case 0x1B:
            converted += "\\e";
        break;
        case 0x22:
            converted += "\\\"";
        break;
        case 0x5C:
            converted += "\\\\";
        break;
        default:
            converted += c;
        }
    }

    return outStream << "\"" << converted << "\"";
}

std::istream& read_escaped_string(std::istream& inStream, std::string& toRead)
{
    std::string converted;
    bool escaped = false;

    for (char c : toRead) {
        if (escaped) {
            escaped = false;
            switch (c)
            {
            case 'a':
                converted += '\x07';
            break;
            case 'b':
                converted += '\x08';
            break;
            case 't':
                converted += '\x09';
            break;
            case 'n':
                converted += '\x0A';
            break;
            case 'v':
                converted += '\x0B';
            break;
            case 'f':
                converted += '\x0C';
            break;
            case 'r':
                converted += '\x0D';
            break;
            case 'e':
                converted += '\x1B';
            break;
            case '\"':
                converted += '\x22';
            break;
            case '\\':
                converted += '\x5C';
            break;
            default:
                inStream.setstate(std::ios::failbit);
                return inStream;
            }
        } else if (c == '\\') {
            escaped = true;
        } else {
            converted += c;
        }
    }

    toRead = converted;
    return inStream;
}