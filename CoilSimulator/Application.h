#pragma once

#include "MainWindow.h"

class Application {
public:
    explicit Application(HINSTANCE hInstance);

    int run(int nCmdShow);

private:
    MainWindow m_mainWindow;
};