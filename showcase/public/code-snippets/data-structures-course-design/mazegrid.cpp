#include "mazegrid.h"

MazeGrid::MazeGrid(int width, int height)
    : m_width(width), m_height(height) {
    initialize();
}

void MazeGrid::initialize() {
    // 使用 [row][col] = [y][x] 的坐标系统
    m_grid.resize(m_height);
    for (int row = 0; row < m_height; ++row) {
        m_grid[row].resize(m_width);
        for (int col = 0; col < m_width; ++col) {
            auto pt = std::make_shared<APoint>();
            pt->type = AType::Unknown;
            pt->x = col;  // x 对应列 (水平方向)
            pt->y = row;  // y 对应行 (垂直方向)
            m_grid[row][col] = pt;
        }
    }
}

void MazeGrid::clear() {
    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            m_grid[row][col]->type = AType::Unknown;
            m_grid[row][col]->g_cost = 0;
            m_grid[row][col]->f_cost = 0;
            m_grid[row][col]->h_cost = 0;
            m_grid[row][col]->parent = nullptr;
        }
    }
}

APointPtr MazeGrid::at(int col, int row) const {
    // at(col, row) = at(x, y)
    if (isValid(col, row)) {
        return m_grid[row][col];
    }
    return nullptr;
}

bool MazeGrid::isValid(int col, int row) const {
    // isValid(col, row) = isValid(x, y)
    return col >= 0 && col < m_width && row >= 0 && row < m_height;
}
