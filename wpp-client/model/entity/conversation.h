#ifndef CONVERSATION_H
#define CONVERSATION_H

#include "Entity.h"
#include <ctime>

class Conversation : public Entity
{
private:
    long id;
    long originId;
    long targetId;
    time_t lastMessageDate;
public:
    Conversation();
    ~Conversation();

    std::string getTableName() override;
    std::string getPrimaryKey() override;
    void setPrimaryKey(std::string id) override;
    PRIMARY_KEY_TYPE getPrimaryKeyType() override;
    std::vector<std::string> getAttributes() override;
    std::vector<std::string> getAttributesWithoutId() override;
    std::map<std::string, std::string> getAttributesAndValues() override;
    std::map<std::string, std::string> getAttributesAndValuesWithoutId() override;
    void fillAttributes(const std::map<std::string, std::string> &sqlValues) override;

    long getOriginId();
    void setOriginId(long originId);

    long getTargetId();
    void setTargetId(long targetId);

    time_t getLastMessageDate();
    void setLastMessageDate(time_t date);
};

#endif // CONVERSATION_H
