#pragma once

#include "FieldScene.h"
#include "Viewport.h"
#include <vector>

#include <Windows.h>

class Renderer {
public:
    void render(
        HDC deviceContext,
        const Viewport& viewport,
        const RECT& clientRect,
        const FieldScene& scene
    );

private:
    void drawAxes(
        HDC deviceContext,
        const Viewport& viewport
    );

    void drawVector(
        HDC deviceContext,
        const Viewport& viewport,
        const FieldSample& sample,
        double maximumMagnitude
    );

    void drawMagneticFieldAndIntegrationPath(
        HDC deviceContext,
        const Viewport& viewport,
        const IntegrationSample& sample,
        double maximumMagnitude
    );

    void drawContributionGraph(
        HDC deviceContext,
        RECT graphRect,
        const std::vector<IntegrationSample>& samples,
        double maximumMagnitude
    );

    void showIntegrationResult(
        HDC deviceContext,
        RECT clientRect,
        double integration
    );
};