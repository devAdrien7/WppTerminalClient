#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <string>
#include <vector>
#include "OBSERVABLE_COMMAND.h"

class IObserver{
public:
    virtual ~IObserver() = default;
    virtual void update(const OBSERVABLE_COMMAND& command, std::vector<std::string>& args) = 0;
};

#endif // IOBSERVER_H
