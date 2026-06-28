#include "Viewport.h"

ScreenPoint Viewport::worldToScreen(Vec2 pos) const {
    const double normalizedX{ (pos.x - m_minimumX) / (m_maximumX - m_minimumX) };
    const double normalizedY{ (pos.y - m_minimumY) / (m_maximumY - m_minimumY) };

    return ScreenPoint{
        static_cast<int>(normalizedX * m_width),
        static_cast<int>((1.0 - normalizedY) * m_height)
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
