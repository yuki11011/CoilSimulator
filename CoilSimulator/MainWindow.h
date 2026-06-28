#pragma once

#include <Windows.h>

class MainWindow {
public:
    explicit MainWindow(HINSTANCE hInstance);

    bool create(int nCmdShow);

    HWND handle() const;

private:
    static LRESULT CALLBACK windowProcedure(
        HWND hwnd,
        UINT uMsg,
        WPARAM wPatam,
        LPARAM lParam
    );

    LRESULT handleMessage(
        UINT uMsg,
        WPARAM wPatam,
        LPARAM lParam
    );

    HINSTANCE m_instance{};
    HWND m_hwnd{};
};