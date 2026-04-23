#ifndef GROUPREPOSITORY_H
#define GROUPREPOSITORY_H

#include "../model/entity/group.h"
#include "../configuration/databaseconnection.h"

class GroupRepository
{
private:
    inline static GroupRepository* INSTANCE = nullptr;

    DatabaseConnection* dbConn;

    GroupRepository();
    ~GroupRepository();
public:
    static GroupRepository* getInstance();
    static void killInstance();

    Group* getGroupById(const std::string& id);
    std::vector<Group*> getAllGroups();


    void insertNewGroup(Group* group);
    void bashInsertNewGroups(std::vector<Group*> groups);
    void updateGroup(const std::string& id, Group* group);

    void deleteGroup(Group* group);
};

#endif // GROUPREPOSITORY_H
