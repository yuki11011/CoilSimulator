#include "Renderer.h"

#include "FieldScene.h"

void drawLine(
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

void Renderer::render(HDC deviceContext, const RECT& clientRect, const FieldScene& scene) {
    Viewport viewport{};

    const SamplingDesc samplingDesc{ scene.samplingDesc()};

    viewport.setWorldBounds(
        samplingDesc.minimumX, samplingDesc.maximumX,
        samplingDesc.minimumY, samplingDesc.maximumY
    );
    viewport.setClientSize(
        clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top
    );

    drawAxes(deviceContext, viewport);
    for (const auto& sample : scene.samples()) {
        drawVector(deviceContext, viewport, sample);
    }
}

void Renderer::drawAxes(HDC deviceContext, const Viewport& viewport) {
    const int savedState{ SaveDC(deviceContext) };

    HPEN axisPen{ CreatePen(PS_SOLID, 1, RGB(170, 170, 170)) };

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

void Renderer::drawVector(HDC deviceContext, const Viewport& viewport, const FieldSample& sample) {
    const int savedState{ SaveDC(deviceContext) };

    HPEN vectorPen{ CreatePen(PS_SOLID, 1, RGB(230, 170, 170)) };

    SelectObject(deviceContext, vectorPen);

    const ScreenPoint vectorStart{ viewport.worldToScreen(
        Vec2{ sample.pos.x, sample.pos.y }
    ) };
    const ScreenPoint vectorEnd{ viewport.worldToScreen(
        Vec2{ sample.pos.x * sample.pos.magnitude(), sample.pos.y * sample.pos.magnitude() }
    ) };
    drawLine(deviceContext, vectorStart, vectorEnd);

    RestoreDC(deviceContext, savedState);

    DeleteObject(vectorPen);
}
