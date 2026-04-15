#include "utilalghoritms.h"
#include <sstream>

std::vector<std::string> UtilAlghoritms::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }

    return tokens;
}

bool UtilAlghoritms::isLong(const std::string& str, long& value) {
    try {
        size_t pos;
        value = std::stol(str, &pos);

        return pos == str.size();
    } catch (...) {
        return false;
    }
}

std::string UtilAlghoritms::boolToString(bool value) {
    return value ? "true" : "false";
}

bool UtilAlghoritms::stringToBoolean(const std::string& s) {
    return s == "true" || s == "1";
}
