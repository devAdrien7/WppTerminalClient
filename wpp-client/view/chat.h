#ifndef CHAT_H
#define CHAT_H

#include "IView.h"
#include <ncurses.h>


class Chat : public IView
{
private:
    WINDOW* chatHeader;
    WINDOW* chat;
    WINDOW* chatInput;

    WINDOW* menuBar;
    WINDOW* contacts;
    WINDOW* contactSearch;

    bool isActive = false;
    bool firstTime = true;

    void loadWindows();
public:
    Chat();
    ~Chat();

    void drawView() override;
    void inactiveView() override;
};

#endif // CHAT_H
