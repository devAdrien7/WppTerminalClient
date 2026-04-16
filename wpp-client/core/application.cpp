#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include <fcntl.h>
#include <ncurses.h>
#include <locale.h>
#include <pwd.h>
#include "application.h"
#include "../configuration/signalmanager.h"
#include "../view/login.h"
#include "../view/chat.h"

Application::Application()
{
    SignalManager::registerRuningCv(&this->cv);
    this->service = new WppConnectorService();
}

Application::~Application()
{
    for (const auto& [key, value] : views) {
        delete value;
    }
    delete service;
}

void Application::run()
{
    loadViews();
    pid_t pid = fork();

    struct passwd* pw = getpwuid(getuid());

    if (pid == 0) {
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        if (getppid() == 1) {
            exit(1);
        }

        //int devNull = open("/dev/null", O_WRONLY);
        //if (devNull != -1) {
        //    dup2(devNull, STDOUT_FILENO);
        //    dup2(devNull, STDERR_FILENO);
        //    close(devNull);
        //}

        int logFd = open("/tmp/wpp-node.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (logFd != -1) {
            dup2(logFd, STDOUT_FILENO);
            dup2(logFd, STDERR_FILENO);
            close(logFd);
        }

        //Melhorar mais depois
        std::string nodePath = std::string(pw->pw_dir) + "/Documentos/Projects/WppTerminal/wpp-socket/index.js";

        execlp("node", "node", nodePath.c_str(), NULL);

        perror("Error when starting node...");
    } else if (pid > 0) {
        std::cout << "Node PID: " << pid << std::endl;
        sleep(1);

        service->startWppCommunication();

        setlocale(LC_ALL, "");
        initscr();
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        currentView = ViewType::LOGIN;
        changeView();

        while(SignalManager::running.load()){
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return this->newView || !SignalManager::running.load(); });

            if(this->newView){
                changeView();
            }
        }

        endwin();
        service->kill();
        std::cout << "Wpp Service Dead..." << std::endl;
        std::cout << "Closing program..." << std::endl;
    } else {
        perror("fork failed");
    }
}

void Application::loadViews()
{
    Login* login = new Login(this);
    service->subscribe(login);

    Chat* chat = new Chat();

    views[ViewType::LOGIN] = login;
    views[ViewType::CHAT] = chat;
}

void Application::changeView()
{
    views[this->currentView]->drawView();
}


void Application::changeCurrentView(const ViewType&  view)
{
    views[this->currentView]->inactiveView();
    this->currentView = view;
    this->newView = true;
    cv.notify_all();
}
