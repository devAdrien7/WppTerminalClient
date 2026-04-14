#include "login.h"
#include "../util/utilalghoritms.h"
#include <string>
//#include <iostream>

Login::Login(Application* app)
{
    this-> app = app;
}

Login::~Login()
{
    delwin(qrWindow);
}

void Login::drawView()
{
    if(firstTime)
        loadWindows();

    attron(COLOR_PAIR(1));
    this->isActive = true;

    int height, width;
    getmaxyx(stdscr, height, width);

    std::string wppLogo[] = {
    "──▄▄█▀▀▀▀▀█▄▄──",
    "▄█▀░░▄▄░░░░░▀█▄",
    "█░░░███░░░░░░░█",
    "█░░░██▄░░░░░░░█",
    "█░░░░▀██▄░██░░█",
    "█░░░░░░▀███▀░░█",
    "▀█▄░░░░░░░░░▄█▀",
    "─▄█░░░▄▄▄▄█▀▀──",
    "─█░░▄█▀────────",
    "─▀▀▀▀──────────",};

    std::string wppName[] = {
        " __        ___   _    _  _____ ____    _    ____  ____  ",
        " \\ \\      / / | | |  / \\|_   _/ ___|  / \\  |  _ \\|  _ \\ ",
        "  \\ \\ /\\ / /| |_| | / _ \\ | | \\___ \\ / _ \\ | |_) | |_) |",
        "   \\ V  V / |  _  |/ ___ \\| |  ___) / ___ \\|  __/|  __/ ",
        "    \\_/\\_/  |_| |_/_/   \\_\\_| |____/_/   \\_\\_|   |_|    ",
        "                                                        "
    };

    std::string terminalName[] = {
        " _____ _____ ____  __  __ ___ _   _    _    _     ",
        "|_   _| ____|  _ \\|  \\/  |_ _| \\ | |  / \\  | |    ",
        "  | | |  _| | |_) | |\\/| || ||  \\| | / _ \\ | |    ",
        "  | | | |___|  _ <| |  | || || |\\  |/ ___ \\| |___ ",
        "  |_| |_____|_| \\_\\_|  |_|___|_| \\_/_/   \\_\\_____|",
        "                                                  "
    };

    int firstLine = 3;
    int x = (width - 84) / 2;
    for(const std::string& wppLogoPart: wppLogo){
        mvaddstr(firstLine, x, wppLogoPart.c_str());
        firstLine++;
    }

    firstLine = 2;
    x += 16;
    for(const std::string& wppNamePart: wppName){
        mvaddstr(firstLine, x, wppNamePart.c_str());
        firstLine++;
    }

    x += 4;
    for(const std::string& terminalNamePart: terminalName){
        mvaddstr(firstLine, x, terminalNamePart.c_str());
        firstLine++;
    }

    firstLine = 3;
    x += 53;
    for(const std::string& wppLogoPart: wppLogo){
        mvaddstr(firstLine, x, wppLogoPart.c_str());
        firstLine++;
    }

    refresh();

    attroff(COLOR_PAIR(1));
}

void Login::inactiveView()
{
    this->isActive = false;
    clear();
}

void Login::update(const OBSERVABLE_COMMAND &command, std::vector<std::string> &args)
{
    if(!isActive)
        return;
    switch (command) {
        case OBSERVABLE_COMMAND::SHOW_QR:
            if(args.size() > 0)
                printQr(args[0]);
            break;
    }
}

void Login::printQr(const std::string &qrCode)
{
    //std::cout << "Chegou sim" <<std::endl;
    //attron(COLOR_PAIR(1));
    werase(qrWindow);
    wattron(qrWindow, COLOR_PAIR(1));
    int firstLine = 1;
    box(qrWindow, 0, 0);

    for(const std::string& qrPart: UtilAlghoritms::split(qrCode, '\n')){
        mvwaddstr(qrWindow, firstLine, 1, qrPart.c_str());
        firstLine++;
    }

    wrefresh(qrWindow);
    wattroff(qrWindow, COLOR_PAIR(1));
}

void Login::loadWindows()
{
    int height, width;
    getmaxyx(stdscr, height, width);

    int x = (width - 59) / 2;

    this->qrWindow = newwin(31, 59, 17, x);
}
