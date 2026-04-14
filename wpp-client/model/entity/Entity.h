#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <vector>
#include <map>
#include "PRIMARY_KEY_TYPE.h"

class Entity{
public:
    Entity() = default;

    virtual std::string getTableName() = 0;
    virtual std::string getPrimaryKey() = 0;
    virtual void setPrimaryKey(std::string id);
    virtual PRIMARY_KEY_TYPE getPrimaryKeyType() = 0;

    virtual std::vector<std::string> getAttributes() = 0;
    virtual std::vector<std::string> getAttributesWithoutId() = 0;
    virtual std::map<std::string, std::string> getAttributesAndValues() = 0;
    virtual std::map<std::string, std::string> getAttributesAndValuesWithoutId() = 0;
    virtual void fillAttributes(const std::map<std::string, std::string>& sqlValues) = 0;
};

#endif // ENTITY_H
