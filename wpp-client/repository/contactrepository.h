#ifndef CONTACTREPOSITORY_H
#define CONTACTREPOSITORY_H

#include "../model/entity/contact.h"
#include "../configuration/databaseconnection.h"

class ContactRepository
{
private:
    inline static ContactRepository* INSTANCE = nullptr;

    DatabaseConnection* dbConn;

    ContactRepository();
    ~ContactRepository();
public:
    static ContactRepository* getInstance();
    static void killInstance();

    Contact* getContactById(const std::string& id);
    std::vector<Contact*> getAllContacts();

    void insertNewContact(Contact* contact);
    void bashInsertNewContacts(std::vector<Contact*> contacts);
    void updateContact(const std::string& id, Contact* contact);

    void deleteContact(Contact* contact);
};

#endif // CONTACTREPOSITORY_H
