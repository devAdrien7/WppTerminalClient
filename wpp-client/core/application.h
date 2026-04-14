#ifndef APPLICATION_H
#define APPLICATION_H

#include "../service/wppconnectorservice.h"
#include <mutex>
#include <condition_variable>
#include "../view/IView.h"
#include <map>
#include "ViewType.h"

class Application
{
private:
    std::mutex mtx;
    std::condition_variable cv;

    WppConnectorService *service;

    ViewType currentView;
    bool newView;

    std::map<ViewType, IView*> views;

    void loadViews();
    void changeView();
public:
    Application();
    ~Application();
    void run();
    void changeCurrentView(const ViewType& view);
};

#endif // APPLICATION_H
