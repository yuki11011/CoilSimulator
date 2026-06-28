#include <iostream>
#include "Application.h"
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    Application application{ hInstance };

    return application.run(nCmdShow);
}