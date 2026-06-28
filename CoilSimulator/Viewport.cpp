#include "Viewport.h"

ScreenPoint Viewport::worldToScreen(Vec2 pos) const {
    const double normalizedX{ (pos.x - m_minimumX) / (m_maximumX - m_minimumY) };
    const double normalizedY{ (pos.y - m_minimumY) / (m_maximumY - m_minimumY) };

    return ScreenPoint{
        static_cast<int>(normalizedX * m_width),
        static_cast<int>(normalizedY * (1 - m_height))
    };
}

double Viewport::pixelsPerWorldUnit() const {
    const double horizontalScale{
        m_width / (m_maximumX - m_minimumX)
    };
    const double verticalScale{
        m_height / (m_maximumY - m_minimumY)
    };

    return horizontalScale < verticalScale
        ? horizontalScale
        : verticalScale;
}
