#ifndef GRIDUTILS_H
#define GRIDUTILS_H

#include <vector>
#include <array>

struct Direction {
    int dx;
    int dy;
    bool isDiagonal;
    double cost;
};

class GridUtils {
public:
    // 4方向（上下左右）
    static inline const std::array<Direction, 4> DIRECTIONS_4 = {{
        {-1, 0, false, 1.0},  // 上
        {1, 0, false, 1.0},   // 下
        {0, -1, false, 1.0},  // 左
        {0, 1, false, 1.0}    // 右
    }};

    // 8方向（包含对角线）
    static inline const std::array<Direction, 8> DIRECTIONS_8 = {{
        {-1, 0, false, 1.0},      // 上
        {1, 0, false, 1.0},       // 下
        {0, -1, false, 1.0},      // 左
        {0, 1, false, 1.0},       // 右
        {-1, -1, true, 1.414},    // 左上
        {-1, 1, true, 1.414},     // 右上
        {1, -1, true, 1.414},     // 左下
        {1, 1, true, 1.414}       // 右下
    }};

    // 检查坐标是否在网格范围内
    static bool isValidPosition(int row, int col, int maxRows, int maxCols) {
        return row >= 0 && row < maxRows && col >= 0 && col < maxCols;
    }

    // 检查坐标是否在网格范围内（vector版本）
    template<typename T>
    static bool isInBounds(int x, int y, const std::vector<std::vector<T>>& grid) {
        return y >= 0 && y < static_cast<int>(grid.size()) &&
               x >= 0 && x < static_cast<int>(grid[0].size());
    }
};

#endif // GRIDUTILS_H
