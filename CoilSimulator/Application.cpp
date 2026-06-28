#include "Application.h"

#include <Windows.h>

Application::Application(HINSTANCE hInstance) 
    : m_mainWindow{ hInstance } {
}

int Application::run(int nCmdShow) {
    if (!m_mainWindow.create(nCmdShow)) {
        return -1;
    };

    MSG msg{};
    BOOL bRet{};
    
    while ((bRet = GetMessageW(&msg, nullptr, 0, 0)) != 0) {
        if (bRet == -1) {
            return -1;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
