#ifndef MAZEGRID_H
#define MAZEGRID_H

#include <memory>
#include <vector>
#include <QColor>

enum class AType {
    Unknow,
    Wall,
    StartPoint,
    EndPoint,
    Path,
    Visited
};

struct APoint {
    int x = 0;
    int y = 0;
    AType type = AType::Unknow;
    double current_cost = 0.0;
    double f_cost = 0.0;
    double g_cost = 0.0;
    double h_cost = 0.0;
    std::shared_ptr<APoint> parent;
    
    int algorithmPath = -1;
    QColor pathColor;
    bool isBestPath = false;

    bool operator==(const APoint& other) const {
        return x == other.x && y == other.y;
    }
};

using APointPtr = std::shared_ptr<APoint>;
using Grid2D = std::vector<std::vector<APointPtr>>;

class MazeGrid {
public:
    MazeGrid(int width = 10, int height = 10);
    ~MazeGrid() = default;

    void initialize();
    void clear();

    Grid2D& data() { return m_grid; }
    const Grid2D& data() const { return m_grid; }

    int width() const { return m_width; }
    int height() const { return m_height; }

    void setWidth(int w) { m_width = w; }
    void setHeight(int h) { m_height = h; }

    APointPtr at(int x, int y) const;
    bool isValid(int x, int y) const;

private:
    int m_width;
    int m_height;
    Grid2D m_grid;
};

#endif // MAZEGRID_H
