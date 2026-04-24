#ifndef GROUP_H
#define GROUP_H

#include "Entity.h"
#include "contact.h"


class Group : public Entity
{
private:
    std::string id;
    std::string name;
    std::string groupPic;
    bool archived;
    std::vector<Contact*> members;
public:
    Group();
    ~Group();

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

    std::string getGroupPic();
    void setGroupPic(std::string groupPic);

    bool isArchived();
    void setArchived(bool archived);

    std::vector<Contact*> getMembers();
    void insertNewMember(Contact* contact);
    void deleteMember(Contact* contact);
};

#endif // GROUP_H
