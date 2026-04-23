#include "grouprepository.h"

GroupRepository::GroupRepository()
{
    dbConn = DatabaseConnection::getInstance();
}

GroupRepository::~GroupRepository() {}

GroupRepository *GroupRepository::getInstance()
{
    if(INSTANCE == nullptr){
        INSTANCE = new GroupRepository();
    }

    return INSTANCE;
}

void GroupRepository::killInstance()
{
    delete INSTANCE;
}

Group *GroupRepository::getGroupById(const std::string &id)
{
    std::map<std::string, std::string> map = { {"id", id} };
    std::vector<Group*> groups = dbConn->getSearch<Group>("groups", map);

    if(groups.empty())
        return nullptr;

    return groups[0];
}

std::vector<Group *> GroupRepository::getAllGroups()
{
    return dbConn->getAll<Group>("groups");
}

void GroupRepository::insertNewGroup(Group *group)
{
    dbConn->insert(group);
}

void GroupRepository::bashInsertNewGroups(std::vector<Group *> groups)
{
    std::vector<Entity*> entities;
    for(const auto& group : groups){
        entities.push_back(group);
    }

    dbConn->bashInsert(entities);
}

void GroupRepository::updateGroup(const std::string &id, Group *group)
{
    std::map<std::string, std::string> map = { {"id", id} };
    dbConn->update(group, map);
}

void GroupRepository::deleteGroup(Group *group)
{
    dbConn->deleteEntity(group);
    delete group;
}


