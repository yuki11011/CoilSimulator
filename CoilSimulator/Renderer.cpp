#include "Renderer.h"

#include <numbers>
#include <algorithm>
#include <cmath>
#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "FieldScene.h"

inline static void drawLine(
    HDC deviceContext,
    ScreenPoint start, ScreenPoint end
) {
    MoveToEx(
        deviceContext,
        start.x, start.y,
        nullptr
    );
    LineTo(
        deviceContext,
        end.x, end.y
    );
}

inline static void drawBox(
    HDC deviceContext,
    ScreenPoint center,
    int size
) {
    MoveToEx(
        deviceContext,
        center.x - size / 2, center.y - size / 2,
        nullptr
    );
    LineTo(
        deviceContext,
        center.x + size / 2, center.y - size / 2
    );
    LineTo(
        deviceContext,
        center.x + size / 2, center.y + size / 2
    );
    LineTo(
        deviceContext,
        center.x - size / 2, center.y + size / 2
    ); 
    LineTo(
        deviceContext,
        center.x - size / 2, center.y - size / 2
    );
}

inline static void drawArrowHead(
    HDC deviceContext,
    ScreenPoint start, ScreenPoint end
) {
    const double deltaX{ static_cast<double>( end.x - start.x ) };
    const double deltaY{static_cast<double>( end.y - start.y) };
    const double angle{std::atan2(deltaY, deltaX) };

    constexpr double headLength{ 6.0 };
    constexpr double headAngle{ std::numbers::pi_v<double> / 6.0 };

    const ScreenPoint first{
        static_cast<int>(std::lround(end.x - headLength * std::cos(angle - headAngle))),
        static_cast<int>(std::lround(end.y - headLength * std::sin(angle - headAngle)))
    };
    const ScreenPoint second{
        static_cast<int>(std::lround(end.x - headLength * std::cos(angle + headAngle))),
        static_cast<int>(std::lround(end.y - headLength * std::sin(angle + headAngle)))
    };

    drawLine(deviceContext, end, first);
    drawLine(deviceContext, end, second);
}

void Renderer::render(HDC deviceContext, const Viewport& viewport, const RECT& clientRect, const FieldScene& scene) {
    const SamplingDesc samplingDesc{ scene.samplingDesc() };

    drawAxes(deviceContext, viewport);

    double maximumMagnitude{};
    for (const auto& sample : scene.samples()) {
        if (sample.magnitude > maximumMagnitude) {
            maximumMagnitude = sample.magnitude;
        }
    }

    for (const auto& sample : scene.samples()) {
        drawVector(deviceContext, viewport, sample, maximumMagnitude);
    }

    maximumMagnitude = 0.0;
    for (const auto& sample : scene.integrationSamples()) {
        if (std::abs(sample.contribution) > maximumMagnitude) {
            maximumMagnitude = std::abs(sample.contribution);
        }
    }

    for (const auto& sample : scene.integrationSamples()) {
        drawMagneticFieldAndIntegrationPath(deviceContext, viewport, sample, maximumMagnitude);
    }

    int graphWidth{ 400 };
    int graphHeight{ 100 };
    int graphHorizontalPadding{ 20 };
    int graphVerticalPadding{ 20 };

    int clientWidth{ clientRect.right - clientRect.left };
    int clientHeight{ clientRect.bottom - clientRect.top };

    RECT graphRect{
        clientRect.right - graphWidth - graphHorizontalPadding,
        clientRect.top + graphVerticalPadding,
        clientRect.right - graphHorizontalPadding,
        clientRect.top + graphHeight + graphVerticalPadding
    };
    drawContributionGraph(deviceContext, graphRect, scene.integrationSamples(), maximumMagnitude);

    showIntegrationResult(deviceContext, clientRect, scene.integrationValue());
}

void Renderer::drawAxes(HDC deviceContext, const Viewport& viewport) {
    const int savedState{ SaveDC(deviceContext) };

    HPEN axisPen{ CreatePen(PS_SOLID, 2, RGB(0, 0, 0)) };

    SelectObject(deviceContext, axisPen);

    const ScreenPoint horizontalStart{ viewport.worldToScreen(
        Vec2{ viewport.minimumX(), 0.0 }
    ) };
    const ScreenPoint horizontalEnd{ viewport.worldToScreen(
        Vec2{ viewport.maximumX(), 0.0 }
    ) };
    drawLine(deviceContext, horizontalStart, horizontalEnd);

    const ScreenPoint verticalStart{ viewport.worldToScreen(
        Vec2{ 0.0, viewport.minimumY() }
    ) };
    const ScreenPoint verticalEnd{ viewport.worldToScreen(
          Vec2{ 0.0, viewport.maximumY() }
    ) };
    drawLine(deviceContext, verticalStart, verticalEnd);

    RestoreDC(deviceContext, savedState);

    DeleteObject(axisPen);
}

void Renderer::drawVector(HDC deviceContext, const Viewport& viewport, const FieldSample& sample, double maximumMagnitude) {
    const int savedState{ SaveDC(deviceContext) };

    const double magnitudeRatio{ std::clamp(sample.magnitude / maximumMagnitude, 0.0, 1.0) };

    constexpr double logarithmicStrength{ 99.0 };

    const double colorScale{ std::log1p(logarithmicStrength * magnitudeRatio) / std::log1p( logarithmicStrength) };

    auto lerp = [](int a, int b, double t) { return static_cast<int>(std::lround(a + (b - a) * t)); };
    const int red{ lerp(0, 255, colorScale) };
    const int blue{ lerp(255, 0, colorScale) };

    HPEN vectorPen{ CreatePen(PS_SOLID, 1, RGB(red, 0, blue)) };

    SelectObject(deviceContext, vectorPen);

    const ScreenPoint vectorStart{ viewport.worldToScreen(sample.pos) };

    const Vec2 direction{ sample.field.x / sample.magnitude, sample.field.y / sample.magnitude };
    const double strength{ std::clamp(sample.magnitude / maximumMagnitude, 0.0, 1.0) };
    constexpr double maximumArrowLength{ 18.0 };
    const double arrowLength{ maximumArrowLength * strength };
    const ScreenPoint vectorEnd{
        static_cast<int>(std::lround(vectorStart.x + direction.x * maximumArrowLength)),
        static_cast<int>(std::lround(vectorStart.y - direction.y * maximumArrowLength))
    };

    drawLine(deviceContext, vectorStart, vectorEnd);
    drawArrowHead(deviceContext, vectorStart, vectorEnd);

    RestoreDC(deviceContext, savedState);

    DeleteObject(vectorPen);
}

void Renderer::drawMagneticFieldAndIntegrationPath(HDC deviceContext, const Viewport& viewport, const IntegrationSample& sample, double maximumMagnitude) {
    const int savedState{ SaveDC(deviceContext) };

    const HPEN vectorPen{ CreatePen(PS_SOLID, 1, RGB(0, 255, 0)) };

    SelectObject(deviceContext, vectorPen);

    double length{ 15.0 };
    ScreenPoint vectorStart{ viewport.worldToScreen(sample.pos) };
    Vec2 direction{ sample.magneticField.x / sample.magneticField.magnitude(), sample.magneticField.y / sample.magneticField.magnitude() };
    ScreenPoint vectorEnd{ 
        static_cast<int>(std::lround(vectorStart.x + direction.x * length)),
        static_cast<int>(std::lround(vectorStart.y - direction.y * length)) 
    };

    drawLine(deviceContext, vectorStart, vectorEnd);
    drawArrowHead(deviceContext, vectorStart, vectorEnd);

    DeleteObject(vectorPen);

    const HPEN pathPen{ CreatePen(PS_SOLID, 1, RGB(50, 50, 50)) };

    SelectObject(deviceContext, pathPen);

    const ScreenPoint pathPoint{ viewport.worldToScreen(sample.pos) };
    drawBox(deviceContext, pathPoint, 2);

    DeleteObject(pathPen);

    RestoreDC(deviceContext, savedState);
}

void Renderer::drawContributionGraph(HDC deviceContext, RECT graphRect, const std::vector<IntegrationSample>& samples, double maximumMagnitude) {
    const int savedState{ SaveDC(deviceContext) };

    int horizontalPadding{ 30 };
    int verticalPadding{ 30 };
    int graphWidth{ graphRect.right - graphRect.left - 2 * horizontalPadding };
    int graphHeight{ graphRect.bottom - graphRect.top - 2 * verticalPadding };

    // ”wŒi‚ð•`‰æ
    HPEN graphFramePen{ CreatePen(PS_SOLID, 1, RGB(0, 0, 0)) };
    HBRUSH graphBackgroundBrush{ CreateSolidBrush(RGB(255, 255, 255)) };

    SelectObject(deviceContext, graphFramePen);
    SelectObject(deviceContext, graphBackgroundBrush);

    Rectangle(deviceContext, graphRect.left, graphRect.top, graphRect.right, graphRect.bottom);

    DeleteObject(graphFramePen);
    DeleteObject(graphBackgroundBrush);

    // ‰¡Ž²‚ð•`‰æ
    HPEN graphAxisPen{ CreatePen(PS_DOT, 1, RGB(0, 0, 0)) };

    SelectObject(deviceContext, graphAxisPen);

    MoveToEx(deviceContext, graphRect.left + horizontalPadding, graphRect.top + graphHeight / 2 + verticalPadding, nullptr);
    LineTo(deviceContext, graphRect.right - horizontalPadding, graphRect.top + graphHeight / 2 + verticalPadding);

    DeleteObject(graphAxisPen);

    // ƒOƒ‰ƒt‚ð•`‰æ
    double deltaX{ static_cast<double>(graphWidth) / samples.size() };

    for (int i{ 0 }; i < samples.size(); ++i) {
        double normalizedContribution{ samples[i].contribution / maximumMagnitude };
        SetPixelV(
            deviceContext,
            static_cast<int>(graphRect.left + horizontalPadding + deltaX * i),
            static_cast<int>(graphRect.top + static_cast<double>(graphHeight) / 2 + verticalPadding - normalizedContribution * graphHeight),
            RGB(255, 0, 0)
        );
    }

    // ƒOƒ‰ƒt‚ÌŽ²ƒ‰ƒxƒ‹‚ð•\Ž¦
    std::wostringstream oss;
    oss << std::scientific << std::setprecision(4) << maximumMagnitude;

    TextOutW(deviceContext, graphRect.left, graphRect.top, oss.str().c_str(), static_cast<int>(oss.str().size()));

    TEXTMETRIC tm{};
    GetTextMetricsW(deviceContext, &tm);
    TextOutW(deviceContext, graphRect.left, graphRect.bottom - tm.tmHeight, (L"-" + oss.str()).c_str(), static_cast<int>(oss.str().size() + 1));

    RestoreDC(deviceContext, savedState);
}

void Renderer::showIntegrationResult(HDC deviceContext, RECT clientRect, double integration) {
    std::wostringstream oss;
    oss << std::scientific << std::setprecision(10) << integration;

    std::wstring label{ L"Integraion result: " };
    TextOutW(deviceContext, clientRect.left, clientRect.top, (label + oss.str()).c_str(), static_cast<int>(label.size() + oss.str().size()));
    
    constexpr double vacuumPermeability{ 4 * std::numbers::pi * 1.0e-7 };
    oss.str(L"");
    oss.clear();
    oss << std::scientific << std::setprecision(14) << integration / vacuumPermeability;

    label = L"result / ƒÊ_0: ";

    TEXTMETRIC tm{};
    GetTextMetricsW(deviceContext, &tm);
    TextOutW(deviceContext, clientRect.left, clientRect.top + tm.tmHeight, (label + oss.str()).c_str(), static_cast<int>(label.size() + oss.str().size()));
}
