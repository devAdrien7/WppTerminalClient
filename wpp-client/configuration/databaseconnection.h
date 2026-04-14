#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <vector>
#include <map>
#include <sqlite3.h>
#include "../model/entity/Entity.h"

class DatabaseConnection
{
private:
    inline static DatabaseConnection* INSTANCE = nullptr;

    enum OPERATION{
        SELECT,
        INSERT,
        DELETE
    };

    sqlite3* db;

    DatabaseConnection();
    ~DatabaseConnection();

    long getNextId(const std::string& table);
    std::string buildQuerySQL(const OPERATION& operation,const std::string& table, const std::map<std::string, std::string>& data);
public:
    static DatabaseConnection* getInstance();
    static void killInstance();

    template<typename T>
    std::vector<T*> getAll(const std::string& table);
    template<typename T>
    std::vector<T*> getSearch(const std::string& table, const std::map<std::string, std::string>& search);

    void insert(Entity* entity);
    void bashInsert(const std::vector<Entity*>& entities);

    void update(Entity* entity, const std::map<std::string, std::string>& search);

    void deleteEntity(Entity* entity);
};

#endif // DATABASECONNECTION_H
