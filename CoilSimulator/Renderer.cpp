#include "Renderer.h"

#include <numbers>
#include <algorithm>
#include <cmath>

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

    HPEN vectorPen = CreatePen(PS_SOLID, 1, RGB(red ,0 ,blue)) ;

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
