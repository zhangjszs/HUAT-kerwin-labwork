#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "mazegrid.h"
#include <QObject>
#include <QVector>
#include <QPoint>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <memory>
#include <algorithm>
#include <cmath>
#include <chrono>

enum class AlgorithmType {
    AStar,
    BFS,
    DFS,
    Dijkstra,
    JPS
};

enum class MovementType {
    FourDirections,
    EightDirections
};

enum class HeuristicType {
    Manhattan,      // 曼哈顿距离
    Euclidean,      // 欧几里得距离
    Chebyshev,      // 切比雪夫距离
    Octile          // 八方向距离
};

// Coordinate system: (row, col) format
// - row: vertical position (0 = top)
// - col: horizontal position (0 = left)
// - Grid access: grid[row][col]
struct StepInfo {
    int row, col;
    AType type;
    bool isPath;
};

struct AlgorithmResult {
    AlgorithmType type;
    bool found;
    int visitedCount;
    int pathLength;
    double executionTime;
    std::vector<std::pair<int,int>> path;
};

struct SearchResult {
    std::vector<std::pair<int, int>> path;
    int visitedCount;
    bool found;
};

struct CompareAStar {
    bool operator()(const APointPtr& a, const APointPtr& b) const {
        return a->f_cost > b->f_cost;
    }
};

class MazeGenerator {
public:
    static void generateRandomMaze(Grid2D& grid, int width, int height, double obstacleRatio = 0.3);
    static void generateSolvableMaze(Grid2D& grid, int width, int height, double obstacleRatio = 0.3);
    static void generateMazeByDFS(Grid2D& grid, int width, int height);
    static bool isSolvable(const Grid2D& grid, const std::pair<int,int>& start, const std::pair<int,int>& end);
};

class AlgorithmRunner : public QObject
{
    Q_OBJECT

signals:
    void stepExecuted(const StepInfo& step);
    void pathFound(const QVector<QPoint>& path);
    void searchFinished(bool found);
    void algorithmChanged(AlgorithmType type);

public:
    AlgorithmRunner(QObject *parent = nullptr);
    void startAStar(APointPtr beginPoint, APointPtr endPoint, Grid2D& allPoints);
    void startBFS(const std::vector<std::vector<int>>& maze, const std::pair<int,int>& start, const std::pair<int,int>& end, Grid2D& allPoints);
    void startDFS(const std::vector<std::vector<int>>& maze, const std::pair<int,int>& start, const std::pair<int,int>& end, Grid2D& allPoints);
    void startDijkstra(const std::vector<std::vector<int>>& maze, const std::pair<int,int>& start, const std::pair<int,int>& end, Grid2D& allPoints);
    void startJPS(const std::vector<std::vector<int>>& maze, const std::pair<int,int>& start, const std::pair<int,int>& end, Grid2D& allPoints);
    void stop();
    bool isRunning() const { return m_running; }
    void setSpeed(int ms) { m_stepDelay = ms; }
    void setMovementType(MovementType type) { m_movementType = type; }
    MovementType getMovementType() const { return m_movementType; }
    void setHeuristicType(HeuristicType type) { m_heuristicType = type; }
    HeuristicType getHeuristicType() const { return m_heuristicType; }

private:
    void executeNextStepAStar();
    void executeNextStepBFS();
    void executeNextStepDFS();
    void executeNextStepDijkstra();
    void executeNextStepJPS();
    void emitNextDFSPathStep();
    double heuristicCost(const APoint* a, const APoint* b);
    double calculateHeuristic(int x1, int y1, int x2, int y2, HeuristicType type);

    // JPS辅助函数
    std::pair<int,int> jump(const std::pair<int,int>& current, int dx, int dy);
    std::vector<std::pair<int,int>> findSuccessors(const std::pair<int,int>& current);
    std::vector<std::pair<int,int>> pruneNeighbors(const std::pair<int,int>& current, const std::pair<int,int>& parent);
    bool isWalkable(int x, int y) const;

    AlgorithmType m_currentAlgorithm;
    bool m_running = false;
    int m_stepDelay = 50;
    MovementType m_movementType = MovementType::FourDirections;
    HeuristicType m_heuristicType = HeuristicType::Manhattan;

    Grid2D* m_allPoints = nullptr;
    bool m_gridValid = false;
    APointPtr m_startPoint;
    APointPtr m_endPoint;

    std::priority_queue<APointPtr, std::vector<APointPtr>, CompareAStar> m_openSetAStar;
    std::queue<std::pair<int,int>> m_queueBFS;
    std::stack<std::pair<int,int>> m_stackDFS;
    std::set<std::pair<int,int>> m_visitedSet;
    std::vector<std::pair<int,int>> m_dfsPath;

    // Dijkstra算法专用数据结构
    std::priority_queue<std::pair<double, std::pair<int,int>>,
                        std::vector<std::pair<double, std::pair<int,int>>>,
                        std::greater<std::pair<double, std::pair<int,int>>>> m_dijkstraPQ;
    std::map<std::pair<int,int>, double> m_dijkstraDist;
    std::map<std::pair<int,int>, std::pair<int,int>> m_dijkstraParent;

    // BFS parent map (member to avoid static variable bug)
    std::map<std::pair<int,int>, std::pair<int,int>> m_bfsParent;

    // DFS parent map for correct path reconstruction
    std::map<std::pair<int,int>, std::pair<int,int>> m_dfsParent;

    // JPS算法专用数据结构
    struct JPSNode {
        std::pair<int,int> pos;
        double g_cost;
        double f_cost;
        std::pair<int,int> parent;

        bool operator>(const JPSNode& other) const {
            return f_cost > other.f_cost;
        }
    };
    std::priority_queue<JPSNode, std::vector<JPSNode>, std::greater<JPSNode>> m_jpsPQ;
    std::map<std::pair<int,int>, double> m_jpsGCost;
    std::map<std::pair<int,int>, std::pair<int,int>> m_jpsParent;
    bool m_jpsFinished = false;

    std::pair<int,int> m_bfsStart;
    std::pair<int,int> m_bfsEnd;
    std::vector<std::vector<int>> m_maze;

    bool m_aStarFinished = false;
    std::vector<std::pair<int,int>> m_aStarPath;

    bool m_dfsEmittingPath = false;
    size_t m_dfsPathIndex = 0;

    QVector<QPoint> reconstructPathAStar(APointPtr end);
};

class MazeSolver {
public:
    SearchResult findPathBFS(const std::vector<std::vector<int>>& maze,
                             const std::pair<int, int>& start,
                             const std::pair<int, int>& end,
                             MovementType movementType = MovementType::FourDirections);

    SearchResult findPathDFS(const std::vector<std::vector<int>>& maze,
                             const std::pair<int, int>& start,
                             const std::pair<int, int>& end,
                             MovementType movementType = MovementType::FourDirections);

    SearchResult findPathDijkstra(const std::vector<std::vector<int>>& maze,
                                  const std::pair<int, int>& start,
                                  const std::pair<int, int>& end,
                                  MovementType movementType = MovementType::FourDirections);

    SearchResult findPathJPS(const std::vector<std::vector<int>>& maze,
                             const std::pair<int, int>& start,
                             const std::pair<int, int>& end);

    AlgorithmResult compareAllAlgorithms(const std::vector<std::vector<int>>& maze,
                                         const std::pair<int,int>& start,
                                         const std::pair<int,int>& end,
                                         MovementType movementType = MovementType::EightDirections);

private:
    bool dfsHelper(const std::vector<std::vector<int>>& maze, int row, int col,
                   const std::pair<int, int>& end,
                   std::vector<std::vector<bool>>& visited,
                   std::vector<std::pair<int, int>>& path,
                   int& visitedCount,
                   MovementType movementType);
};

#endif
