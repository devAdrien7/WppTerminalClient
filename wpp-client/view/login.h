#ifndef LOGIN_H
#define LOGIN_H

#include "IView.h"
#include "../core/application.h"
#include <ncurses.h>

class Login : public IView, public IObserver
{
private:
    Application* app;
    bool isActive = false;
    bool firstTime = true;

    WINDOW * qrWindow;

    void printQr(const std::string& qrCode);
    void loadWindows();
public:
    Login(Application* app);
    ~Login();
    void drawView() override;
    void inactiveView() override;
    virtual void update(const OBSERVABLE_COMMAND& command, std::vector<std::string>& args) override;
};

#endif // LOGIN_H
