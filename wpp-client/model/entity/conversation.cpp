#include "conversation.h"
#include "../../util/utilalghoritms.h"
#include <stdexcept>

Conversation::Conversation() {}

Conversation::~Conversation() {}

std::string Conversation::getTableName()
{
    return "conversations";
}

std::string Conversation::getPrimaryKey()
{
    return std::to_string(id);
}

void Conversation::setPrimaryKey(std::string id)
{
    long newId;
    if(UtilAlghoritms::isLong(id, newId)){
        this->id = newId;
    }else{
        std::runtime_error("Id needs to be long");
    }
}

PRIMARY_KEY_TYPE Conversation::getPrimaryKeyType()
{
    return PRIMARY_KEY_TYPE::SEQUENCIAL;
}

std::vector<std::string> Conversation::getAttributes()
{
    return { "id", "origin_id", "target_id", "last_message_date" };
}

std::vector<std::string> Conversation::getAttributesWithoutId()
{
    return { "origin_id", "target_id", "last_message_date" };
}

std::map<std::string, std::string> Conversation::getAttributesAndValues()
{
    return {
        {"id", std::to_string(id)},
        {"origin_id", std::to_string(originId)},
        {"target_id", std::to_string(targetId)},
        {"last_message_date", std::to_string(lastMessageDate)},
    };
}

std::map<std::string, std::string> Conversation::getAttributesAndValuesWithoutId()
{
    return {
        {"origin_id", std::to_string(originId)},
        {"target_id", std::to_string(targetId)},
        {"last_message_date", std::to_string(lastMessageDate)},
    };
}

void Conversation::fillAttributes(const std::map<std::string, std::string> &sqlValues)
{
    setPrimaryKey(sqlValues.at("id"));

    if(!UtilAlghoritms::isLong(sqlValues.at("origin_id"), originId)){
        std::runtime_error("originId needs to be long");
    }

    if(!UtilAlghoritms::isLong(sqlValues.at("target_id"), targetId)){
        std::runtime_error("targetId needs to be long");
    }

    lastMessageDate = static_cast<time_t>(std::stol(sqlValues.at("last_message_date")));
}

long Conversation::getOriginId()
{
    return originId;
}

void Conversation::setOriginId(long originId)
{
    this->originId = originId;
}

long Conversation::getTargetId()
{
    return targetId;
}

void Conversation::setTargetId(long targetId)
{
    this->targetId = targetId;
}

time_t Conversation::getLastMessageDate()
{
    return lastMessageDate;
}

void Conversation::setLastMessageDate(time_t date)
{
    this->lastMessageDate = date;
}
