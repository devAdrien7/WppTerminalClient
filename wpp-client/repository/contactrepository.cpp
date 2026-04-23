#include "contactrepository.h"

ContactRepository::ContactRepository()
{
    dbConn = DatabaseConnection::getInstance();
}

ContactRepository::~ContactRepository() {}

ContactRepository *ContactRepository::getInstance()
{
    if(INSTANCE == nullptr){
        INSTANCE = new ContactRepository();
    }

    return INSTANCE;
}

void ContactRepository::killInstance()
{
    delete INSTANCE;
}

Contact *ContactRepository::getContactById(const std::string &id)
{
    std::map<std::string, std::string> map = { {"id", id} };
    std::vector<Contact*> contacts = dbConn->getSearch<Contact>("contacts", map);

    if(contacts.empty())
        return nullptr;

    return contacts[0];
}

std::vector<Contact *> ContactRepository::getAllContacts()
{
    return dbConn->getAll<Contact>("contacts");
}

void ContactRepository::insertNewContact(Contact *contact)
{
    dbConn->insert(contact);
}

void ContactRepository::bashInsertNewContacts(std::vector<Contact *> contacts)
{
    std::vector<Entity*> entities;
    for(const auto& contact : contacts){
        entities.push_back(contact);
    }

    dbConn->bashInsert(entities);
}

void ContactRepository::updateContact(const std::string &id, Contact *contact)
{
    std::map<std::string, std::string> map = { {"id", id} };
    dbConn->update(contact, map);
}

void ContactRepository::deleteContact(Contact *contact)
{
    dbConn->deleteEntity(contact);
    delete contact;
}
