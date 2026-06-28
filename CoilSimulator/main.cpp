#include <iostream>
#include "Vector.h"
#include "MagneticFieldModel.h"

int main() {
    double x{};
    double z{};

    std::cout << "Enter values of x, z of the observation point: ";
    std::cin >> x;
    std::cin >> z;
    Vec2 observationPoint{ x, z };

    CoilParameters parameters{};
    MagneticFieldModel model{parameters};

    Vec2 b{ model.fieldAt(observationPoint) };

    std::cout << "B = (" << b.x << ", " << b.y << ")\n";
}