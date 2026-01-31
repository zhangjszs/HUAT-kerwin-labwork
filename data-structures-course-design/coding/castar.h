/*
    F:路径评分 = g+h
    G:走一格格子的花销
    H:当前格子到目标格子的估算花销

    上下左右走一格花销为10，斜着走一格花销为14，以方便计算
    即格子宽高为10 对角线为14
 */

#ifndef CAstar_H
#define CAstar_H

#include "constants.h"
#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QDebug>
#include <qstring.h>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <vector>
#include <QTextEdit>
#include <memory>
#include <set>
using namespace std;


class APoint;
class CAstar;
class BFS;

enum class AType {
    Unknow,
    Wall,
    StartPoint,
    EndPoint,
    Path,
    Visited
};

class APoint {
public:
    ~APoint();
    int x;
    int y;
    AType type;
    double current_cost;
    double f_cost;
    double g_cost;
    double h_cost;
    APoint* parent;

    bool operator==(const APoint& po) const {
        return (x == po.x && y == po.y);
    }

    APoint() : x(0), y(0), type(AType::Unknow), g_cost(0.0), f_cost(0.0), parent(nullptr) {}
    APoint(int x, int y) : x(x), y(y), type(AType::Unknow), g_cost(0.0), f_cost(0.0), parent(nullptr) {}
};

class CAstar : public QObject {
    Q_OBJECT

signals:
    void visualizationUpdated();

public:
    APoint* findWay(APoint* beginPoint, APoint* endPoint, vector<vector<APoint*>>& allPoints);
    double heuristicCost(const APoint* a, const APoint* b);
    void setPointType(APoint* point, AType type);
    vector<APoint*> path;
};

class MazeSolver {
public:
    std::vector<std::vector<std::pair<int, int>>> findPaths(const std::vector<std::vector<int>>& maze,
                                                            const std::pair<int, int>& start,
                                                            const std::pair<int, int>& end)
    {
        int rows = maze.size();
        int cols = maze[0].size();
        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        std::vector<std::vector<std::pair<int, int>>> paths;

        std::vector<std::pair<int, int>> path;
        path.push_back(start);
        backtrack(maze, start.first, start.second, end, path, paths, visited);

        return paths;
    }

private:
    void backtrack(const std::vector<std::vector<int>>& maze, int row, int col,
                   const std::pair<int, int>& end, std::vector<std::pair<int, int>>& path,
                   std::vector<std::vector<std::pair<int, int>>>& paths,
                   std::vector<std::vector<bool>>& visited)
    {
        if (row == end.first && col == end.second)
        {
            paths.push_back(path);
            return;
        }

        visited[row][col] = true;

        // 上
        if (row > 0 && maze[row - 1][col] == 0 && !visited[row - 1][col])
        {
            path.push_back(std::make_pair(row - 1, col));
            backtrack(maze, row - 1, col, end, path, paths, visited);
            path.pop_back();
        }

        // 下
        if (row < maze.size() - 1 && maze[row + 1][col] == 0 && !visited[row + 1][col])
        {
            path.push_back(std::make_pair(row + 1, col));
            backtrack(maze, row + 1, col, end, path, paths, visited);
            path.pop_back();
        }

        // 左
        if (col > 0 && maze[row][col - 1] == 0 && !visited[row][col - 1])
        {
            path.push_back(std::make_pair(row, col - 1));
            backtrack(maze, row, col - 1, end, path, paths, visited);
            path.pop_back();
        }

        // 右
        if (col < maze[0].size() - 1 && maze[row][col + 1] == 0 && !visited[row][col + 1])
        {
            path.push_back(std::make_pair(row, col + 1));
            backtrack(maze, row, col + 1, end, path, paths, visited);
            path.pop_back();
        }

        visited[row][col] = false;
    }
};
#endif // CAstar_H

