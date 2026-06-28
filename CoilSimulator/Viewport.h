#pragma once

#include "Vector.h"

struct ScreenPoint {
    int x{};
    int y{};
};

class Viewport {
public:
    void setWorldBounds(
        double minimumX,
        double maximumX,
        double minimumY,
        double maximumY
    ) {
        m_minimumX = minimumX;
        m_maximumX = maximumX;
        m_minimumY = minimumY;
        m_maximumY = maximumY;
    };

    void setClientSize(
        int width,
        int height
    ) {
        if (width <= 0 || height <= 0) return;

        m_width = width;
        m_height = height;
    };

    ScreenPoint worldToScreen(Vec2 pos) const;

    double pixelsPerWorldUnit() const;

    double minimumX() const { return m_minimumX; };
    double maximumX() const { return m_maximumX; };
    double minimumY() const { return m_minimumY; };
    double maximumY() const { return m_maximumY; };

private:
    double m_minimumX{ 2.0 };
    double m_maximumX{ 2.0 };
    double m_minimumY{ 2.0 };
    double m_maximumY{ 2.0 };

    int m_width{ 1 };
    int m_height{ 1 };
};