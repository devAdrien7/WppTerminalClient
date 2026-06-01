#include "message.h"
#include "../../util/utilalghoritms.h"

Message::Message()
    : Message("", MESSAGE_TYPE::TEXT, "", "", 0, false, false, "", "")
{}

Message::Message(std::string id, MESSAGE_TYPE type, std::string messageText, std::string messageAudioVisual, time_t date, bool read, bool authority, std::string contactId, std::string conversationId)
{
    this->id = id;
    this->type = type;
    this->messageText = messageText;
    this->messageAudioVisual = messageAudioVisual;
    this->date = date;
    this->read = read;
    this->authority = authority;
    this->contactId = contactId;
    this->conversationId = conversationId;
}

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
        {"contact_id", contactId},
        {"conversation_id", conversationId}
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
        {"contact_id", contactId},
        {"conversation_id", conversationId}
    };
}

void Message::fillAttributes(const std::map<std::string, std::string> &sqlValues)
{
    id = sqlValues.at("id");
    type = getStringToMessageType().at(sqlValues.at("type"));
    messageText = sqlValues.at("message_text");
    messageAudioVisual = sqlValues.at("message_audio_visual");
    date = static_cast<time_t>(std::stol(sqlValues.at("date")));
    read = UtilAlghoritms::stringToBoolean(sqlValues.at("read"));
    authority = UtilAlghoritms::stringToBoolean(sqlValues.at("authority"));
    contactId = sqlValues.at("contact_id");
    conversationId = sqlValues.at("conversation_id");
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
    this->authority = isAuthority;
}

std::string Message::getContactId()
{
    return contactId;
}

void Message::setContactId(long contactId)
{
    this->contactId = contactId;
}

std::string Message::getConversationId()
{
    return conversationId;
}

void Message::setConversationId(long conversationId)
{
    this->conversationId = conversationId;
}