#pragma once

struct CoilParameters {
    double radius{ 1.0 };
    double current{ 1.0 };
    int turns{ 1 };

    int integrationSegments{ 512 }; // 積分を数値計算する際の分割数
    double wireRadius{ 0.03 }; // 特異点を回避するためのワイヤの半径
};