#include "group.h"
#include "../../util/utilalghoritms.h"
#include <algorithm>

Group::Group() {}

Group::~Group() {}

std::string Group::getTableName()
{
    return "groups";
}

std::string Group::getPrimaryKey()
{
    return id;
}

void Group::setPrimaryKey(std::string id)
{
    this->id = id;
}

PRIMARY_KEY_TYPE Group::getPrimaryKeyType()
{
    return PRIMARY_KEY_TYPE::INSERTED;
}

std::vector<std::string> Group::getAttributes()
{
    return { "id", "name", "group_pic", "archived" };
}

std::vector<std::string> Group::getAttributesWithoutId()
{
    return { "name", "group_pic", "archived" };
}

std::map<std::string, std::string> Group::getAttributesAndValues()
{
    return {
        {"id", id},
        {"name", name},
        {"group_pic", groupPic},
        {"archived", UtilAlghoritms::boolToString(archived)},
    };
}

std::map<std::string, std::string> Group::getAttributesAndValuesWithoutId()
{
    return {
        {"name", name},
        {"group_pic", groupPic},
        {"archived", UtilAlghoritms::boolToString(archived)},
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

bool Group::isArchived()
{
    return archived;
}

void Group::setArchived(bool archived)
{
    this->archived = archived;
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