#ifndef CONTACT_H
#define CONTACT_H

#include "Entity.h"

class Contact : public Entity
{
private:
    std::string id;
    std::string name;
    std::string profilePic;
public:
    Contact();
    Contact(std::string id);
    Contact(std::string id, std::string name, std::string profilePic);
    ~Contact();

    std::string getTableName() override;
    std::string getPrimaryKey() override;
    void setPrimaryKey(std::string id) override;
    PRIMARY_KEY_TYPE getPrimaryKeyType() override;
    std::vector<std::string> getAttributes() override;
    std::vector<std::string> getAttributesWithoutId() override;
    std::map<std::string, std::string> getAttributesAndValues() override;
    std::map<std::string, std::string> getAttributesAndValuesWithoutId() override;
    void fillAttributes(const std::map<std::string, std::string> &sqlValues) override;


    std::string getName();
    void setName(std::string name);

    std::string getProfilePic();
    void setProfilePic(std::string profilePic);
};

#endif // CONTACT_H
