#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include <fcntl.h>
#include <ncurses.h>
#include <locale.h>
#include "application.h"
#include "../configuration/signalmanager.h"
#include "../view/login.h"

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

    if (pid == 0) {
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        if (getppid() == 1) {
            exit(1);
        }

        int devNull = open("/dev/null", O_WRONLY);
        if (devNull != -1) {
            dup2(devNull, STDOUT_FILENO); // stdout
            dup2(devNull, STDERR_FILENO); // stderr
            close(devNull);
        }

        execlp("node", "node", "/home/islaifer/Documentos/Projects/WppTerminal/wpp-socket/index.js", NULL);

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

        //attron(COLOR_PAIR(1));
        //printw("Hello World !!!");
        //refresh();
        //getch();

        while(SignalManager::running.load()){
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return this->newView || !SignalManager::running.load(); });

            if(this->newView){

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
    views[ViewType::LOGIN] = login;
}

void Application::changeView()
{
    views[this->currentView]->drawView();
}


void Application::changeCurrentView(ViewType view)
{
    views[this->currentView]->inactiveView();
    this->currentView = view;
    this->newView = true;
    cv.notify_one();
}
