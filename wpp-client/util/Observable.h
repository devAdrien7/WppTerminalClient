#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include "IObserver.h"
#include <vector>
#include <algorithm>
#include <string>
#include <array>
#include <sstream>
#include "OBSERVABLE_COMMAND.h"

class Observable
{
private:
    std::vector<IObserver*> observers;
public:

    void subscribe(IObserver* obs) {
        observers.push_back(obs);
    }

    void unsubscribe(IObserver* obs) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), obs),
            observers.end()
            );
    }

protected:
    template<typename T>
    std::string toString(T&& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    template<typename... Args>
    void notify(const OBSERVABLE_COMMAND& command, Args&&... __args) {
        auto args = std::vector{
            toString(std::forward<Args>(__args))...
        };

        for (auto* obs : observers) {
            obs->update(command, args);
        }
    }
};

#endif // OBSERVABLE_H
