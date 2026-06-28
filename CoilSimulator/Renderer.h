#pragma once

#include "FieldScene.h"
#include "Viewport.h"

#include <Windows.h>

class Renderer {
public:
    void render(
        HDC deviceContext,
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
        const FieldSample& sample
    );
};