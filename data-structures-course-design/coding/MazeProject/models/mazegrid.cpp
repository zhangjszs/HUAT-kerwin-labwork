#include "mazegrid.h"

MazeGrid::MazeGrid(int width, int height)
    : m_width(width), m_height(height) {
    initialize();
}

void MazeGrid::initialize() {
    m_grid.resize(m_height);
    for (int row = 0; row < m_height; ++row) {
        m_grid[row].resize(m_width);
        for (int col = 0; col < m_width; ++col) {
            auto pt = std::make_shared<APoint>();
            pt->type = AType::Unknow;
            pt->x = col;
            pt->y = row;
            m_grid[row][col] = pt;
        }
    }
}

void MazeGrid::clear() {
    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            m_grid[row][col]->type = AType::Unknow;
            m_grid[row][col]->g_cost = 0;
            m_grid[row][col]->f_cost = 0;
            m_grid[row][col]->h_cost = 0;
            m_grid[row][col]->parent = nullptr;
        }
    }
}

APointPtr MazeGrid::at(int x, int y) const {
    if (isValid(x, y)) {
        return m_grid[y][x];
    }
    return nullptr;
}

bool MazeGrid::isValid(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}
