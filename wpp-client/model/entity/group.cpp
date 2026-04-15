#include "group.h"
#include "../../util/utilalghoritms.h"
#include <stdexcept>
#include <algorithm>

Group::Group() {}

Group::~Group() {}

std::string Group::getTableName()
{
    return "groups";
}

std::string Group::getPrimaryKey()
{
    return std::to_string(id);
}

void Group::setPrimaryKey(std::string id)
{
    long newId;
    if(UtilAlghoritms::isLong(id, newId)){
        this->id = newId;
    }else{
        std::runtime_error("Id needs to be long");
    }
}

PRIMARY_KEY_TYPE Group::getPrimaryKeyType()
{
    return PRIMARY_KEY_TYPE::SEQUENCIAL;
}

std::vector<std::string> Group::getAttributes()
{
    return { "id", "name", "group_pic" };
}

std::vector<std::string> Group::getAttributesWithoutId()
{
    return { "name", "group_pic" };
}

std::map<std::string, std::string> Group::getAttributesAndValues()
{
    return {
        {"id", std::to_string(id)},
        {"name", name},
        {"group_pic", groupPic}
    };
}

std::map<std::string, std::string> Group::getAttributesAndValuesWithoutId()
{
    return {
        {"id", std::to_string(id)},
        {"name", name},
        {"group_pic", groupPic}
    };
}

void Group::fillAttributes(const std::map<std::string, std::string> &sqlValues)
{
    setPrimaryKey(sqlValues.at("id"));
    name = sqlValues.at("name");
    groupPic = sqlValues.at("group_pic");
}

std::string Group::getName()
{
    return name;
}

void Group::setName(std::string name)
{
    this->name = name;
}

std::string Group::getGroupPic()
{
    return groupPic;
}

void Group::setGroupPic(std::string groupPic)
{
    this->groupPic = groupPic;
}

std::vector<Contact *> Group::getMembers()
{
    return members;
}

void Group::insertNewMember(Contact *contact)
{
    members.push_back(contact);
}

void Group::deleteMember(Contact *contact)
{
    members.erase(
        std::remove(members.begin(), members.end(), contact),
        members.end()
    );
}