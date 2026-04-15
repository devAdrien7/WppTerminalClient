#include "message.h"
#include "../../util/utilalghoritms.h"
#include <stdexcept>

Message::Message() {}

Message::~Message() {}

std::string Message::getTableName()
{
    return "messages";
}

std::string Message::getPrimaryKey()
{
    return id;
}

void Message::setPrimaryKey(std::string id)
{
    this->id = id;
}

PRIMARY_KEY_TYPE Message::getPrimaryKeyType()
{
    return PRIMARY_KEY_TYPE::INSERTED;
}

std::vector<std::string> Message::getAttributes()
{
    return { "id", "type", "message_text", "message_audio_visual", "date", "read", "authority", "contact_id", "conversation_id" };
}

std::vector<std::string> Message::getAttributesWithoutId()
{
    return { "type", "message_text", "message_audio_visual", "date", "read", "authority", "contact_id", "conversation_id" };
}

std::map<std::string, std::string> Message::getAttributesAndValues()
{
    return {
        {"id", id},
        {"type", messageTypeToString(type)},
        {"message_text", messageText},
        {"message_audio_visual", messageAudioVisual},
        {"date", std::to_string(date)},
        {"read", UtilAlghoritms::boolToString(read)},
        {"authority", UtilAlghoritms::boolToString(authority)},
        {"contact_id", std::to_string(contactId)},
        {"conversation_id", std::to_string(conversationId)}
    };
}

std::map<std::string, std::string> Message::getAttributesAndValuesWithoutId()
{
    return {
        {"type", messageTypeToString(type)},
        {"message_text", messageText},
        {"message_audio_visual", messageAudioVisual},
        {"date", std::to_string(date)},
        {"read", UtilAlghoritms::boolToString(read)},
        {"authority", UtilAlghoritms::boolToString(authority)},
        {"contact_id", std::to_string(contactId)},
        {"conversation_id", std::to_string(conversationId)}
    };
}

void Message::fillAttributes(const std::map<std::string, std::string> &sqlValues)
{
    id = sqlValues.at("id");
    type = stringToMessageType.at(sqlValues.at("type"));
    messageText = sqlValues.at("message_text");
    messageAudioVisual = sqlValues.at("message_audio_visual");
    date = static_cast<time_t>(std::stol(sqlValues.at("date")));
    read = UtilAlghoritms::stringToBoolean(sqlValues.at("read"));
    authority = UtilAlghoritms::stringToBoolean(sqlValues.at("authority"));

    if(!UtilAlghoritms::isLong(sqlValues.at("contact_id"), contactId)){
        std::runtime_error("contactId needs to be long");
    }

    if(!UtilAlghoritms::isLong(sqlValues.at("conversation_id"), conversationId)){
        std::runtime_error("conversationId needs to be long");
    }
}

MESSAGE_TYPE Message::getType()
{
    return type;
}

void Message::setType(MESSAGE_TYPE type)
{
    this-> type = type;
}

std::string Message::getMessageText()
{
    return messageText;
}

void Message::setMessageText(std::string messageText)
{
    this->messageText = messageText;
}

std::string Message::getMessageAudioVisual()
{
    return messageAudioVisual;
}

void Message::setMessageAudioVisual(std::string messageAudioVisual)
{
    this->messageAudioVisual = messageAudioVisual;
}

time_t Message::getDate()
{
    return date;
}

void Message::setDate(time_t date)
{
    this->date = date;
}

bool Message::isRead()
{
    return read;
}

void Message::readMessage()
{
    read = true;
}

bool Message::isAuthority()
{
    return authority;
}

void Message::setIsAuthority(bool isAuthority)
{
    this->authority = authority;
}

long Message::getContactId()
{
    return contactId;
}

void Message::setContactId(long contactId)
{
    this->contactId = contactId;
}

long Message::getConversationId()
{
    return conversationId;
}

void Message::setConversationId(long conversationId)
{
    this->conversationId = conversationId;
}