#include "MainWindow.h"

bool MainWindow::create(int nCmdShow) {
    constexpr wchar_t className[]{ L"coil sim" };

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = windowProcedure;
    wc.hInstance = m_instance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassExW(&wc)) {
        return false;
    };

    m_hwnd = CreateWindowEx(
        0,
        className,
        L"電界シミュレータ",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr, m_instance, this
    );

    if (m_hwnd == nullptr) {
        return false;
    }

    initializeScene();

    ShowWindow(m_hwnd, nCmdShow);

    return true;
}

LRESULT CALLBACK MainWindow::windowProcedure(HWND hwnd, UINT uMsg, WPARAM wPatam, LPARAM lParam) {
    MainWindow* window{};
    if (uMsg == WM_CREATE) {
        const auto* pCreate{ reinterpret_cast<CREATESTRUCT*>(lParam) };

        window = static_cast<MainWindow*>(pCreate->lpCreateParams);

        window->m_hwnd = hwnd;

        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    if (window != nullptr) {
        return window->handleMessage(uMsg, wPatam, lParam);
    }

    return DefWindowProcW(hwnd, uMsg, wPatam, lParam);
}

LRESULT MainWindow::handleMessage(UINT uMsg, WPARAM wPatam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT:
        handlePaint();
        return 0;

    case WM_SIZE:
        InvalidateRect(m_hwnd, nullptr, TRUE);
        m_viewport.setClientSize(LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(m_hwnd, uMsg, wPatam, lParam);
}

void MainWindow::initializeScene() {
    CoilParameters coilParameters{
        1.0,   // コイルの半径
        1.0,   // 電流の大きさ
        1,     // コイルの巻き数
        1024,   // 分割数
        0.03   // ワイヤ半径
    };

    SamplingDesc samplingDesc{
        -2.0,
         2.0,
        -2.0,
         2.0,
        100,
        100
    };

    m_scene.setCoilParameters(coilParameters);
    m_scene.setSamplingDesc(samplingDesc);

    m_scene.rebuild();

    m_viewport.setWorldBounds(
        samplingDesc.minimumX,
        samplingDesc.maximumX,
        samplingDesc.minimumY,
        samplingDesc.maximumY
    );
}

void MainWindow::handlePaint() {
    PAINTSTRUCT ps{};
    HDC deviceContext{ BeginPaint(m_hwnd, &ps) };
    RECT clientRect{};
    GetClientRect(m_hwnd, &clientRect);
    m_viewport.setClientSize(
        clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top
    );

    m_renderer.render(deviceContext, m_viewport, clientRect, m_scene);

    EndPaint(m_hwnd, &ps);
}
