#ifndef UTILALGHORITMS_H
#define UTILALGHORITMS_H

#include <vector>
#include <string>

class UtilAlghoritms
{
public:
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static bool isLong(const std::string& str, long& value);
    static std::string boolToString(bool value);
    static bool stringToBoolean(const std::string& s);
};

#endif // UTILALGHORITMS_H
