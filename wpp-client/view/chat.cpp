#include "chat.h"

Chat::Chat()
{

}

Chat::~Chat()
{
    delwin(chat);
    delwin(chatInput);
    delwin(contacts);
    delwin(contactSearch);
    delwin(chatHeader);
    delwin(menuBar);
}

void Chat::drawView()
{
    this->isActive = true;
    if(this->firstTime)
        loadWindows();

    //mvaddstr(4, 4, "chat is working");
    //refresh();
}

void Chat::inactiveView()
{
    this->isActive = false;
    clear();
}

void Chat::loadWindows()
{
    this->firstTime = false;

    int height, width;
    getmaxyx(stdscr, height, width);

    int heightChatHeader = (height * 0.1) - 1;
    int widthChatHeader = (width * 0.7) - 1;

    int heightChat = (height * 0.8) - 1;
    int widthChat = (width * 0.7) - 1;

    int heightChatInput = (height * 0.1) - 1;
    int widthChatInput = (width * 0.7) - 1;

    int heightSendButton = (height * 0.1) - 1;
    int widthSendButton = (width * 0.1) - 1;

    int heightContacts = (height * 0.8) - 1;
    int widthContacts = (width * 0.25) - 1;

    int heightContactSearch = (height * 0.2) - 1;
    int widthContactSearch = (width * 0.25) - 1;

    int heightMenu = height - 2;
    int widthMenu = (width * 0.05) - 1;

    int x = 1, y = 1;
    menuBar = newwin(heightMenu, widthMenu, x, y);

    y += widthMenu;
    contactSearch = newwin(heightContactSearch, widthContactSearch, x, y);

    x += heightContactSearch;
    contacts = newwin(heightContacts, widthContacts, x, y);

    x = 1;
    y += widthContacts + 1;
    chatHeader = newwin(heightChatHeader, widthChatHeader, x, y);

    x += heightChatHeader;
    chat = newwin(heightChat, widthChat, x, y);

    x += heightChat;
    chatInput = newwin(heightChatInput, widthChatInput, x, y);

    //Teste:
    werase(menuBar);
    box(menuBar, 0, 0);
    mvwaddstr(menuBar, 0, 0, "Menu Bar aqui");
    wrefresh(menuBar);

    werase(contacts);
    box(contacts, 0, 0);
    mvwaddstr(contacts, 0, 0, "Contacts aqui");
    wrefresh(contacts);

    werase(contactSearch);
    box(contactSearch, 0, 0);
    mvwaddstr(contactSearch, 0, 0, "Contact Search aqui");
    wrefresh(contactSearch);

    werase(chat);
    box(chat, 0, 0);
    mvwaddstr(chat, 0, 0, "Chat aqui");
    wrefresh(chat);

    werase(chatInput);
    box(chatInput, 0, 0);
    mvwaddstr(chatInput, 0, 0, "Chat Input aqui");
    wrefresh(chatInput);

    werase(chatHeader);
    box(chatHeader, 0, 0);
    mvwaddstr(chatHeader, 0, 0, "Chat Header aqui");
    wrefresh(chatHeader);
}
