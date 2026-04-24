#include "conversation.h"
#include "../../util/utilalghoritms.h"
#include <algorithm>

Conversation::Conversation()
    : Conversation("", "", "", false, false, {})
{}

Conversation::Conversation(std::string id, bool archived, bool group, std::vector<Contact *> members)
    : Conversation(id, "", "", archived, group, members)
{}

Conversation::Conversation(std::string id, std::string name, std::string groupPic, bool archived, bool group, std::vector<Contact *> members)
    : id(id), name(name), groupPic(groupPic), archived(archived), group(group), members(members)
{}

Conversation::~Conversation() {}

std::string Conversation::getTableName()
{
    return "conversations";
}

std::string Conversation::getPrimaryKey()
{
    return id;
}

void Conversation::setPrimaryKey(std::string id)
{
    this->id = id;
}

PRIMARY_KEY_TYPE Conversation::getPrimaryKeyType()
{
    return PRIMARY_KEY_TYPE::INSERTED;
}

std::vector<std::string> Conversation::getAttributes()
{
    return { "id", "name", "group_pic", "archived", "group" };
}

std::vector<std::string> Conversation::getAttributesWithoutId()
{
    return { "name", "group_pic", "archived", "group" };
}

std::map<std::string, std::string> Conversation::getAttributesAndValues()
{
    return {
        {"id", id},
        {"name", name},
        {"group_pic", groupPic},
        {"archived", UtilAlghoritms::boolToString(archived)},
        {"group", UtilAlghoritms::boolToString(group)}
    };
}

std::map<std::string, std::string> Conversation::getAttributesAndValuesWithoutId()
{
    return {
        {"name", name},
        {"group_pic", groupPic},
        {"archived", UtilAlghoritms::boolToString(archived)},
        {"group", UtilAlghoritms::boolToString(group)}
    };
}

void Conversation::fillAttributes(const std::map<std::string, std::string> &sqlValues)
{
    setPrimaryKey(sqlValues.at("id"));
    name = sqlValues.at("name");
    groupPic = sqlValues.at("group_pic");
    archived = UtilAlghoritms::stringToBoolean(sqlValues.at("archived"));
    group = UtilAlghoritms::stringToBoolean(sqlValues.at("group"));
}

std::string Conversation::getName()
{
    return name;
}

void Conversation::setName(std::string name)
{
    this->name = name;
}

std::string Conversation::getGroupPic()
{
    return groupPic;
}

void Conversation::setGroupPic(std::string groupPic)
{
    this->groupPic = groupPic;
}

bool Conversation::isArchived()
{
    return archived;
}

void Conversation::setArchived(bool archived)
{
    this->archived = archived;
}

bool Conversation::isGroup()
{
    return group;
}

void Conversation::setIsGroup(bool isGroup)
{
    this->group = isGroup;
}

std::vector<Contact *> Conversation::getMembers()
{
    return members;
}

void Conversation::insertNewMember(Contact *contact)
{
    members.push_back(contact);
}

void Conversation::deleteMember(Contact *contact)
{
    members.erase(
        std::remove(members.begin(), members.end(), contact),
        members.end()
    );
}