#pragma once

#include "FieldScene.h"
#include "Renderer.h"
#include <Windows.h>

class MainWindow {
public:
    explicit MainWindow(HINSTANCE hInstance)
        : m_instance{ hInstance } {
    };

    bool create(int nCmdShow);

    HWND handle() const { return m_hwnd; };

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

    void initializeScene();

    void handlePaint();

    HINSTANCE m_instance{};
    HWND m_hwnd{};

    FieldScene m_scene{};
    Viewport m_viewport{};
    Renderer m_renderer{};
};