#include "databaseconnection.h"
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sstream>
#include <filesystem>
#include <type_traits>
#include "../model/entity/PRIMARY_KEY_TYPE.h"

DatabaseConnection::DatabaseConnection()
{
    struct passwd* pw = getpwuid(getuid());

    std::string dbPath = std::string(pw->pw_dir) + "/.wppterminal/database.db";

    std::filesystem::create_directories(dbPath.substr(0, dbPath.find_last_of('/')));

    int error = sqlite3_open(dbPath.c_str(), &db);
    if(error){
        std::cerr << "Error to open database: " << sqlite3_errmsg(db) << std::endl;
    }
}

DatabaseConnection::~DatabaseConnection()
{
    sqlite3_close(db);
}

DatabaseConnection* DatabaseConnection::getInstance()
{
    if(INSTANCE == nullptr){
        INSTANCE = new DatabaseConnection();
    }

    return INSTANCE;
}

void DatabaseConnection::killInstance()
{
    delete INSTANCE;
}

template<typename T>
std::vector<T*> DatabaseConnection::getAll(const std::string& table)
{
    static_assert(std::is_base_of_v<Entity, T>,
                  "T needs to be a Entity");

    sqlite3_stmt* stmt;
    std::stringstream sql;
    sql << "SELECT * FROM " << table;

    std::unique_lock<std::mutex> lock(mutex);
    sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr);

    T* tmp = new T();
    std::vector<std::string> columns = tmp->getAttributes();
    delete tmp;

    std::vector<T*> result;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        T* entity = new T();
        std::map<std::string, std::string> attributes;
        int i = 0;
        for(const auto& column: columns){
            attributes[column] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            i++;
        }

        entity->fillAttributes(attributes);
        result.push_back(entity);
    }

    sqlite3_finalize(stmt);
    lock.unlock();

    return result;
}

template<typename T>
std::vector<T*> DatabaseConnection::getSearch(const std::string& table, const std::map<std::string, std::string>& search)
{
    static_assert(std::is_base_of_v<Entity, T>,
                  "T needs to be a Entity");

    sqlite3_stmt* stmt;
    std::string sql = buildQuerySQL(OPERATION::SELECT, table, search);

    std::unique_lock<std::mutex> lock(mutex);
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    int index = 1;
    std::vector<std::string> columns;
    for (const auto& [key, value] : search) {
        columns.push_back(key);
        sqlite3_bind_text(stmt, index++, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    std::vector<T*> result;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        T* entity = new T();
        std::map<std::string, std::string> attributes;
        int i = 0;
        for(const auto& column: columns){
            attributes[column] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
            i++;
        }

        entity->fillAttributes(attributes);
        result.push_back(entity);
    }

    sqlite3_finalize(stmt);
    lock.unlock();

    return result;
}

void DatabaseConnection::insert(Entity *entity)
{
    std::string table = entity->getTableName();
    PRIMARY_KEY_TYPE primaryKeyType = entity->getPrimaryKeyType();

    std::unique_lock<std::mutex> lock(mutex);
    if(primaryKeyType == PRIMARY_KEY_TYPE::SEQUENCIAL){
        long id = getNextId(table);
        entity->setPrimaryKey(std::to_string(id));
    }

    std::string primaryKey = entity->getPrimaryKey();
    std::map<std::string, std::string> entityValues = entity->getAttributesAndValues();

    sqlite3_stmt* stmt;

    std::string sql = buildQuerySQL(OPERATION::INSERT, table, entityValues);

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    int index = 1;
    for (const auto& [_, value] : entityValues) {
        sqlite3_bind_text(stmt, index++, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    lock.unlock();
}

void DatabaseConnection::bashInsert(const std::vector<Entity *>& entities)
{
    std::unique_lock<std::mutex> lock(mutex);
    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    sqlite3_stmt* stmt;
    bool first = true;
    for(const auto& entity: entities){
        if(!first){
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }

        std::string table = entity->getTableName();
        PRIMARY_KEY_TYPE primaryKeyType = entity->getPrimaryKeyType();

        if(primaryKeyType == PRIMARY_KEY_TYPE::SEQUENCIAL){
            long id = getNextId(table);
            entity->setPrimaryKey(std::to_string(id));
        }

        std::string primaryKey = entity->getPrimaryKey();
        std::map<std::string, std::string> entityValues = entity->getAttributesAndValues();

        if(first){
            std::string sql = buildQuerySQL(OPERATION::INSERT, table, entityValues);
            sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        }

        int index = 1;
        for (const auto& [_, value] : entityValues) {
            sqlite3_bind_text(stmt, index++, value.c_str(), -1, SQLITE_TRANSIENT);
        }

        sqlite3_step(stmt);
        first = false;
    }
    sqlite3_finalize(stmt);

    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    lock.unlock();
}

void DatabaseConnection::update(Entity *entity, const std::map<std::string, std::string>& search)
{
    std::string table = entity->getTableName();
    std::map<std::string, std::string> valuesToUpdate = entity->getAttributesAndValuesWithoutId();

    std::stringstream sql;
    sql << "UPDATE " << table << " SET ";

    bool first = true;
    for(const auto& [column, _] : valuesToUpdate){
        if (!first) {
            sql << ", ";
        }
        sql << column << " = ?";
        first = false;
    }

    sql << " WHERE ";
    first = true;
    for(const auto& [column, _] : search){
        if (!first) {
            sql << " AND ";
        }
        sql << column << " = ?";
        first = false;
    }

    sqlite3_stmt* stmt;

    std::unique_lock<std::mutex> lock(mutex);
    sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr);

    int index = 1;
    for (const auto& [_, value] : valuesToUpdate) {
        sqlite3_bind_text(stmt, index++, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    for (const auto& [_, value] : search) {
        sqlite3_bind_text(stmt, index++, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    lock.unlock();
}

void DatabaseConnection::deleteEntity(Entity *entity)
{
    std::string table = entity->getTableName();
    std::map<std::string, std::string> values = entity->getAttributesAndValues();

    std::string sql = buildQuerySQL(OPERATION::DELETE, table, values);

    sqlite3_stmt* stmt;

    std::unique_lock<std::mutex> lock(mutex);
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    int index = 1;
    for (const auto& [_, value] : values) {
        sqlite3_bind_text(stmt, index++, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    lock.unlock();
}

long DatabaseConnection::getNextId(const std::string& table)
{
    long result = 1;
    std::stringstream sql;
    sql << "SELECT * FROM next_id WHERE table_name = ?";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, table.c_str(), -1, SQLITE_TRANSIENT);

    if(sqlite3_step(stmt) == SQLITE_ROW){
        result = sqlite3_column_int64(stmt, 2);
    }

    return result;
}

std::string DatabaseConnection::buildQuerySQL(const OPERATION& operation, const std::string& table, const std::map<std::string, std::string>& data) {
    std::stringstream columns;
    std::stringstream placeholders;

    std::stringstream wheres;

    bool first = true;

    std::stringstream sql;
    switch(operation){
        case OPERATION::INSERT:
            for (const auto& [column, _] : data) {
                if (!first) {
                    columns << ", ";
                    placeholders << ", ";
                }
                columns << column;
                placeholders << "?";
                first = false;
            }

            sql << "INSERT INTO " << table
                << " (" << columns.str() << ") VALUES ("
                << placeholders.str() << ")";
            break;
        case OPERATION::SELECT:
            for (const auto& [column, _] : data) {
                if (!first) {
                    wheres << " AND ";
                }
                wheres << column << " = " << "?";
                first = false;
            }

            sql << "SELECT * FROM " << table
                << " WHERE " << wheres.str();

            break;
        case OPERATION::DELETE:
            for (const auto& [column, _] : data) {
                if (!first) {
                    wheres << " AND ";
                }
                wheres << column << " = " << "?";
                first = false;
            }

            sql << "DELETE FROM " << table
                << " WHERE " << wheres.str();
            break;
    }

    return sql.str();
}