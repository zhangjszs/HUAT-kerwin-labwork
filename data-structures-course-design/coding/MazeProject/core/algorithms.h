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
#include <map>
#include <memory>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <QTimer>

enum class AlgorithmType {
    AStar,
    BFS,
    DFS
};

struct StepInfo {
    int x, y;
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

struct CompareAStar {
    bool operator()(const APointPtr& a, const APointPtr& b) const {
        return a->f_cost > b->f_cost;
    }
};

class MazeGenerator {
public:
    static void generateRandomMaze(Grid2D& grid, int rows, int cols, double obstacleRatio = 0.3);
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
    void stop();
    bool isRunning() const { return m_running; }
    void setSpeed(int ms) { m_stepDelay = ms; }

private:
    void executeNextStepAStar();
    void executeNextStepBFS();
    void executeNextStepDFS();
    void scheduleNextStep();
    void onTimerTimeout();
    double heuristicCost(const APoint* a, const APoint* b);

    AlgorithmType m_currentAlgorithm;
    bool m_running = false;
    int m_stepDelay = 50;
    QTimer m_timer;

    Grid2D* m_allPoints = nullptr;
    APointPtr m_startPoint;
    APointPtr m_endPoint;

    std::priority_queue<APointPtr, std::vector<APointPtr>, CompareAStar> m_openSetAStar;
    std::queue<std::pair<int,int>> m_queueBFS;
    std::stack<std::pair<int,int>> m_stackDFS;
    std::set<std::pair<int,int>> m_visitedSet;
    std::map<std::pair<int,int>, std::pair<int,int>> m_dfsParent;

    std::pair<int,int> m_bfsStart;
    std::pair<int,int> m_bfsEnd;
    std::vector<std::vector<int>> m_maze;
    std::map<std::pair<int,int>, std::pair<int,int>> m_bfsParent;

    bool m_aStarFinished = false;
    std::vector<std::pair<int,int>> m_aStarPath;

    QVector<QPoint> reconstructPathAStar(APointPtr end);
};

class MazeSolver {
public:
    std::vector<std::pair<int, int>> findPathBFS(const std::vector<std::vector<int>>& maze,
                                                 const std::pair<int, int>& start,
                                                 const std::pair<int, int>& end,
                                                 int* visitedCount = nullptr);

    std::vector<std::pair<int, int>> findPathDFS(const std::vector<std::vector<int>>& maze,
                                                 const std::pair<int, int>& start,
                                                 const std::pair<int, int>& end,
                                                 int* visitedCount = nullptr);

    AlgorithmResult compareAllAlgorithms(const std::vector<std::vector<int>>& maze,
                                         const std::pair<int,int>& start,
                                         const std::pair<int,int>& end);

private:
    bool dfsHelper(const std::vector<std::vector<int>>& maze, int row, int col,
                   const std::pair<int, int>& end,
                   std::vector<std::vector<bool>>& visited,
                   std::vector<std::pair<int, int>>& path,
                   int* visitedCount);
};

#endif
