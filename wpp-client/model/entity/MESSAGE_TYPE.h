#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <string>
#include <stdexcept>
#include <unordered_map>

enum MESSAGE_TYPE{
    TEXT,
    VIDEO,
    TEXT_AND_VIDEO,
    TEXT_AND_IMAGE,
    AUDIO,
    STICKER
};

static std::string messageTypeToString(MESSAGE_TYPE s) {
    switch (s) {
        case MESSAGE_TYPE::TEXT: return "TEXT";
        case MESSAGE_TYPE::VIDEO: return "VIDEO";
        case MESSAGE_TYPE::TEXT_AND_VIDEO: return "TEXT_AND_VIDEO";
        case MESSAGE_TYPE::TEXT_AND_IMAGE: return "TEXT_AND_IMAGE";
        case MESSAGE_TYPE::AUDIO: return "AUDIO";
        case MESSAGE_TYPE::STICKER: return "STICKER";
    }
    throw std::invalid_argument("Invalid Enum!");
}

static const std::unordered_map<std::string, MESSAGE_TYPE> stringToMessageType = {
    { "TEXT", MESSAGE_TYPE::TEXT },
    { "VIDEO", MESSAGE_TYPE::VIDEO },
    { "TEXT_AND_VIDEO", MESSAGE_TYPE::TEXT_AND_VIDEO },
    { "TEXT_AND_IMAGE", MESSAGE_TYPE::TEXT_AND_IMAGE },
    { "AUDIO", MESSAGE_TYPE::AUDIO },
    { "STICKER", MESSAGE_TYPE::STICKER }
};

#endif // MESSAGE_TYPE_H
