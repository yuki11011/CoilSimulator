#pragma once

#include <vector>
#include "Vector.h"

struct PathDescriptor {
    double radius{ 1.0 };
    Vec2 center{ 0.0, 1.0 };

    int integrationSegments{ 512 }; // Ï•ª‚ğ”’lŒvZ‚·‚éÛ‚Ì•ªŠ„”
};

class ClosedPath {
public:
    explicit ClosedPath(PathDescriptor desc);

    const PathDescriptor& desc() const { return m_desc; };
    const std::vector<Vec2>& points() const { return m_points; };

private:
    void makePath();

    PathDescriptor m_desc{};
    std::vector<Vec2> m_points{};
};