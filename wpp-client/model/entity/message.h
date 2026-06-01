#ifndef MESSAGE_H
#define MESSAGE_H

#include "Entity.h"
#include "MESSAGE_TYPE.h"
#include <ctime>

class Message : public Entity
{
private:
    std::string id;
    MESSAGE_TYPE type;
    std::string messageText;
    std::string messageAudioVisual;
    time_t date;
    bool read;
    bool authority;
    std::string contactId;
    std::string conversationId;
public:
    Message();
    Message(std::string id, MESSAGE_TYPE type, std::string messageText, std::string messageAudioVisual, time_t date, bool read, bool authority, std::string contactId, std::string conversationId);
    ~Message();

    std::string getTableName() override;
    std::string getPrimaryKey() override;
    void setPrimaryKey(std::string id) override;
    PRIMARY_KEY_TYPE getPrimaryKeyType() override;
    std::vector<std::string> getAttributes() override;
    std::vector<std::string> getAttributesWithoutId() override;
    std::map<std::string, std::string> getAttributesAndValues() override;
    std::map<std::string, std::string> getAttributesAndValuesWithoutId() override;
    void fillAttributes(const std::map<std::string, std::string> &sqlValues) override;

    MESSAGE_TYPE getType();
    void setType(MESSAGE_TYPE type);

    std::string getMessageText();
    void setMessageText(std::string messageText);

    std::string getMessageAudioVisual();
    void setMessageAudioVisual(std::string messageAudioVisual);

    time_t getDate();
    void setDate(time_t date);

    bool isRead();
    void readMessage();

    bool isAuthority();
    void setIsAuthority(bool isAuthority);

    std::string getContactId();
    void setContactId(long contactId);

    std::string getConversationId();
    void setConversationId(long conversationId);
};

#endif // MESSAGE_H
