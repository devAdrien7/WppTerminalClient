#include "contact.h"
#include "../../util/utilalghoritms.h"
#include <stdexcept>

Contact::Contact() {}

Contact::~Contact() {}

std::string Contact::getTableName()
{
    return "contacts";
}

std::string Contact::getPrimaryKey()
{
    return std::to_string(id);
}

void Contact::setPrimaryKey(std::string id)
{
    long newId;
    if(UtilAlghoritms::isLong(id, newId)){
        this->id = newId;
    }else{
        std::runtime_error("Id needs to be long");
    }
}

PRIMARY_KEY_TYPE Contact::getPrimaryKeyType()
{
    return PRIMARY_KEY_TYPE::SEQUENCIAL;
}

std::vector<std::string> Contact::getAttributes()
{
    return { "id", "name", "profile_pic" };
}

std::vector<std::string> Contact::getAttributesWithoutId()
{
    return { "name", "profile_pic" };
}

std::map<std::string, std::string> Contact::getAttributesAndValues()
{
    return {
        {"id", std::to_string(id)},
        {"name", name},
        {"profile_pic", profilePic}
    };
}

std::map<std::string, std::string> Contact::getAttributesAndValuesWithoutId()
{
    return {
        {"name", name},
        {"profile_pic", profilePic}
    };
}

void Contact::fillAttributes(const std::map<std::string, std::string> &sqlValues)
{
    setPrimaryKey(sqlValues.at("id"));
    name = sqlValues.at("name");
    profilePic = sqlValues.at("profile_pic");
}

std::string Contact::getName()
{
    return name;
}

void Contact::setName(std::string name)
{
    this->name = name;
}

std::string Contact::getProfilePic()
{
    return profilePic;
}

void Contact::setProfilePic(std::string profilePic)
{
    this->profilePic = profilePic;
}
