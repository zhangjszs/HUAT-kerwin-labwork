#include "algorithms.h"
#include "utils/gridutils.h"
#include <QTimer>
#include <map>
#include <random>
#include <chrono>

// Standard visited checking pattern for all algorithms:
// 1. Pop/dequeue node from frontier
// 2. Check if already visited, skip if yes
// 3. Mark as visited
// 4. Process node and emit step
// 5. Add neighbors to frontier

AlgorithmRunner::AlgorithmRunner(QObject *parent)
    : QObject(parent)
{
}

void AlgorithmRunner::startAStar(APointPtr beginPoint, APointPtr endPoint, Grid2D& allPoints)
{
    stop();
    m_currentAlgorithm = AlgorithmType::AStar;
    m_running = true;
    m_allPoints = &allPoints;
    m_gridValid = true;
    m_startPoint = beginPoint;
    m_endPoint = endPoint;

    m_openSetAStar = std::priority_queue<APointPtr, std::vector<APointPtr>, CompareAStar>();
    m_visitedSet.clear();
    m_aStarPath.clear();
    m_aStarFinished = false;

    for (int i = 0; i < static_cast<int>(allPoints.size()); i++) {
        for (int j = 0; j < static_cast<int>(allPoints[i].size()); j++) {
            allPoints[i][j]->g_cost = INFINITY_COST;
            allPoints[i][j]->f_cost = 0;
            allPoints[i][j]->h_cost = 0;
            allPoints[i][j]->parent = nullptr;
        }
    }

    m_startPoint->g_cost = 0;
    m_startPoint->h_cost = heuristicCost(m_startPoint.get(), m_endPoint.get());
    m_startPoint->f_cost = m_startPoint->h_cost;
    m_openSetAStar.push(m_startPoint);

    emit algorithmChanged(AlgorithmType::AStar);
    executeNextStepAStar();
}

void AlgorithmRunner::executeNextStepAStar()
{
    if (!m_running || m_aStarFinished) return;

    if (m_openSetAStar.empty()) {
        m_running = false;
        emit searchFinished(false);
        return;
    }

    auto current = m_openSetAStar.top();
    m_openSetAStar.pop();

    std::pair<int,int> currPos = {current->y, current->x};
    if (m_visitedSet.count(currPos) > 0) {
        QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepAStar);
        return;
    }
    m_visitedSet.insert(currPos);

    if (current->type != AType::StartPoint && current->type != AType::EndPoint) {
        StepInfo step = {current->y, current->x, AType::Visited, false};
        emit stepExecuted(step);
    }

    if (current.get() == m_endPoint.get()) {
        QVector<QPoint> path = reconstructPathAStar(current);
        m_running = false;
        m_aStarFinished = true;
        emit pathFound(path);
        emit searchFinished(true);
        return;
    }

    auto processNeighbor = [&](const auto& dir) {
        int nx = current->x + dir.dy;
        int ny = current->y + dir.dx;

        if (!GridUtils::isInBounds(nx, ny, *m_allPoints)) return;

        auto neighbor = (*m_allPoints)[ny][nx];
        if (neighbor->type == AType::Wall) return;

        if (dir.isDiagonal) {
            int midX1 = current->x + dir.dy;
            int midY1 = current->y;
            int midX2 = current->x;
            int midY2 = current->y + dir.dx;

            bool path1Clear = GridUtils::isInBounds(midX1, midY1, *m_allPoints) &&
                             (*m_allPoints)[midY1][midX1]->type != AType::Wall;
            bool path2Clear = GridUtils::isInBounds(midX2, midY2, *m_allPoints) &&
                             (*m_allPoints)[midY2][midX2]->type != AType::Wall;

            if (!path1Clear && !path2Clear) return;
        }

        std::pair<int,int> neighborPos = {ny, nx};
        if (m_visitedSet.count(neighborPos) > 0) return;

        double tentativeG = current->g_cost + dir.cost;
        if (tentativeG < neighbor->g_cost) {
            neighbor->parent = current;
            neighbor->g_cost = tentativeG;
            neighbor->h_cost = heuristicCost(neighbor.get(), m_endPoint.get());
            neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
            m_openSetAStar.push(neighbor);
        }
    };

    if (m_movementType == MovementType::EightDirections) {
        for (const auto& dir : GridUtils::DIRECTIONS_8) processNeighbor(dir);
    } else {
        for (const auto& dir : GridUtils::DIRECTIONS_4) processNeighbor(dir);
    }

    QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepAStar);
}

QVector<QPoint> AlgorithmRunner::reconstructPathAStar(APointPtr end)
{
    QVector<QPoint> path;
    auto current = end;
    while (current) {
        path.append(QPoint(current->x, current->y));
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());

    for (int i = 1; i < path.size() - 1; i++) {
        StepInfo step = {path[i].y(), path[i].x(), AType::Path, true};
        emit stepExecuted(step);
    }

    return path;
}

void AlgorithmRunner::startBFS(const std::vector<std::vector<int>>& maze, const std::pair<int,int>& start, const std::pair<int,int>& end, Grid2D& allPoints)
{
    stop();
    m_currentAlgorithm = AlgorithmType::BFS;
    m_running = true;
    m_allPoints = &allPoints;
    m_gridValid = true;
    m_bfsStart = start;
    m_bfsEnd = end;
    m_maze = maze;

    m_queueBFS = std::queue<std::pair<int,int>>();
    m_visitedSet.clear();
    m_bfsParent.clear();
    m_queueBFS.push(start);

    emit algorithmChanged(AlgorithmType::BFS);
    executeNextStepBFS();
}

void AlgorithmRunner::executeNextStepBFS()
{
    if (!m_running) return;

    if (m_queueBFS.empty()) {
        m_running = false;
        emit searchFinished(false);
        return;
    }

    // 1. Pop node from queue
    auto current = m_queueBFS.front();
    m_queueBFS.pop();

    // 2. Check if already visited, skip if yes
    if (m_visitedSet.count(current) > 0) {
        QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepBFS);
        return;
    }

    // 3. Mark as visited
    m_visitedSet.insert(current);

    // 4. Process node and emit step
    if (current != m_bfsStart && current != m_bfsEnd) {
        StepInfo step = {current.first, current.second, AType::Visited, false};
        emit stepExecuted(step);
    }

    if (current == m_bfsEnd) {
        m_running = false;

        // Reconstruct path using parent map
        std::vector<std::pair<int,int>> path;
        auto p = current;
        while (m_bfsParent.count(p) > 0) {
            path.push_back(p);
            p = m_bfsParent[p];
        }
        path.push_back(m_bfsStart);

        // Reverse to get correct order (start to end)
        std::reverse(path.begin(), path.end());

        // Emit path cells (skip start and end)
        for (size_t i = 1; i < path.size() - 1; ++i) {
            StepInfo step = {path[i].first, path[i].second, AType::Path, true};
            emit stepExecuted(step);
        }

        emit searchFinished(true);
        return;
    }

    // 5. Add neighbors to frontier
    if (m_movementType == MovementType::EightDirections) {
        for (const auto& dir : GridUtils::DIRECTIONS_8) {
            int nx = current.second + dir.dy;
            int ny = current.first + dir.dx;

            if (!GridUtils::isInBounds(nx, ny, m_maze)) {
                continue;
            }

            if (m_maze[ny][nx] == 1) continue;
            if (m_visitedSet.count({ny, nx}) > 0) continue;

            // 对角线移动检测：确保不会"切角"穿过墙体
            if (dir.isDiagonal) {
                int midX1 = current.second + dir.dy;
                int midY1 = current.first;
                int midX2 = current.second;
                int midY2 = current.first + dir.dx;

                bool path1Clear = GridUtils::isInBounds(midX1, midY1, m_maze) &&
                                 m_maze[midY1][midX1] == 0;
                bool path2Clear = GridUtils::isInBounds(midX2, midY2, m_maze) &&
                                 m_maze[midY2][midX2] == 0;

                if (!path1Clear && !path2Clear) {
                    continue;
                }
            }

            m_queueBFS.push({ny, nx});
            m_bfsParent[{ny, nx}] = current;
        }
    } else {
        for (const auto& dir : GridUtils::DIRECTIONS_4) {
            int nx = current.second + dir.dy;
            int ny = current.first + dir.dx;

            if (!GridUtils::isInBounds(nx, ny, m_maze)) {
                continue;
            }

            if (m_maze[ny][nx] == 1) continue;
            if (m_visitedSet.count({ny, nx}) > 0) continue;

            m_queueBFS.push({ny, nx});
            m_bfsParent[{ny, nx}] = current;
        }
    }

    QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepBFS);
}

void AlgorithmRunner::startDFS(const std::vector<std::vector<int>>& maze, const std::pair<int,int>& start, const std::pair<int,int>& end, Grid2D& allPoints)
{
    stop();
    m_currentAlgorithm = AlgorithmType::DFS;
    m_running = true;
    m_allPoints = &allPoints;
    m_gridValid = true;
    m_bfsStart = start;
    m_bfsEnd = end;
    m_maze = maze;

    m_stackDFS = std::stack<std::pair<int,int>>();
    m_visitedSet.clear();
    m_dfsPath.clear();
    m_dfsParent.clear();
    m_stackDFS.push(start);

    emit algorithmChanged(AlgorithmType::DFS);
    executeNextStepDFS();
}

void AlgorithmRunner::executeNextStepDFS()
{
    if (!m_running) return;

    if (m_stackDFS.empty()) {
        m_running = false;
        emit searchFinished(false);
        return;
    }

    // 1. Pop node from stack
    auto current = m_stackDFS.top();
    m_stackDFS.pop();

    // 2. Check if already visited, skip if yes
    if (m_visitedSet.count(current) > 0) {
        QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepDFS);
        return;
    }

    // 3. Mark as visited
    m_visitedSet.insert(current);
    m_dfsPath.push_back(current);

    // 4. Process node and emit step
    if (current != m_bfsStart && current != m_bfsEnd) {
        StepInfo step = {current.first, current.second, AType::Visited, false};
        emit stepExecuted(step);
    }

    if (current == m_bfsEnd) {
        m_dfsEmittingPath = true;
        m_dfsPathIndex = 0;
        m_running = false;
        QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::emitNextDFSPathStep);
        return;
    }

    // 5. Add neighbors to frontier
    if (m_movementType == MovementType::EightDirections) {
        for (const auto& dir : GridUtils::DIRECTIONS_8) {
            int nx = current.second + dir.dy;
            int ny = current.first + dir.dx;

            if (!GridUtils::isInBounds(nx, ny, m_maze)) {
                continue;
            }

            if (m_maze[ny][nx] == 1) continue;
            if (m_visitedSet.count({ny, nx}) > 0) continue;

            // 对角线移动检测：确保不会"切角"穿过墙体
            if (dir.isDiagonal) {
                int midX1 = current.second + dir.dy;
                int midY1 = current.first;
                int midX2 = current.second;
                int midY2 = current.first + dir.dx;

                bool path1Clear = GridUtils::isInBounds(midX1, midY1, m_maze) &&
                                 m_maze[midY1][midX1] == 0;
                bool path2Clear = GridUtils::isInBounds(midX2, midY2, m_maze) &&
                                 m_maze[midY2][midX2] == 0;

                if (!path1Clear && !path2Clear) {
                    continue;
                }
            }

            m_stackDFS.push({ny, nx});
            if (m_dfsParent.count({ny, nx}) == 0) m_dfsParent[{ny, nx}] = current;
        }
    } else {
        for (const auto& dir : GridUtils::DIRECTIONS_4) {
            int nx = current.second + dir.dy;
            int ny = current.first + dir.dx;

            if (!GridUtils::isInBounds(nx, ny, m_maze)) {
                continue;
            }

            if (m_maze[ny][nx] == 1) continue;
            if (m_visitedSet.count({ny, nx}) > 0) continue;

            m_stackDFS.push({ny, nx});
            if (m_dfsParent.count({ny, nx}) == 0) m_dfsParent[{ny, nx}] = current;
        }
    }

    QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepDFS);
}

void AlgorithmRunner::emitNextDFSPathStep()
{
    // Reconstruct actual path from start to end using parent map
    std::vector<std::pair<int,int>> path;
    auto pos = m_bfsEnd;
    while (m_dfsParent.count(pos) > 0) {
        path.push_back(pos);
        pos = m_dfsParent[pos];
    }
    path.push_back(m_bfsStart);
    std::reverse(path.begin(), path.end());

    // Emit path cells (skip start and end)
    for (size_t i = 1; i < path.size() - 1; ++i) {
        StepInfo step = {path[i].first, path[i].second, AType::Path, true};
        emit stepExecuted(step);
    }

    m_dfsEmittingPath = false;
    emit searchFinished(true);
}


void AlgorithmRunner::startDijkstra(const std::vector<std::vector<int>>& maze, const std::pair<int,int>& start, const std::pair<int,int>& end, Grid2D& allPoints)
{
    stop();
    m_currentAlgorithm = AlgorithmType::Dijkstra;
    m_running = true;
    m_allPoints = &allPoints;
    m_gridValid = true;
    m_bfsStart = start;
    m_bfsEnd = end;
    m_maze = maze;

    m_dijkstraPQ = std::priority_queue<std::pair<double, std::pair<int,int>>,
                                       std::vector<std::pair<double, std::pair<int,int>>>,
                                       std::greater<std::pair<double, std::pair<int,int>>>>();
    m_dijkstraDist.clear();
    m_dijkstraParent.clear();
    m_visitedSet.clear();

    m_dijkstraDist[start] = 0.0;
    m_dijkstraPQ.push({0.0, start});

    emit algorithmChanged(AlgorithmType::Dijkstra);
    executeNextStepDijkstra();
}

void AlgorithmRunner::executeNextStepDijkstra()
{
    if (!m_running) return;

    if (m_dijkstraPQ.empty()) {
        m_running = false;
        emit searchFinished(false);
        return;
    }

    auto [currentDist, current] = m_dijkstraPQ.top();
    m_dijkstraPQ.pop();

    if (m_visitedSet.count(current) > 0) {
        QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepDijkstra);
        return;
    }

    m_visitedSet.insert(current);

    if (current != m_bfsStart && current != m_bfsEnd) {
        StepInfo step = {current.first, current.second, AType::Visited, false};
        emit stepExecuted(step);
    }

    if (current == m_bfsEnd) {
        std::vector<std::pair<int,int>> path;
        auto pos = current;
        while (m_dijkstraParent.count(pos) > 0) {
            path.push_back(pos);
            pos = m_dijkstraParent[pos];
        }
        path.push_back(m_bfsStart);
        std::reverse(path.begin(), path.end());

        for (const auto& p : path) {
            if (p != m_bfsStart && p != m_bfsEnd) {
                StepInfo step = {p.first, p.second, AType::Path, true};
                emit stepExecuted(step);
            }
        }
        m_running = false;
        emit searchFinished(true);
        return;
    }

    if (m_movementType == MovementType::EightDirections) {
        for (const auto& dir : GridUtils::DIRECTIONS_8) {
            int nx = current.second + dir.dy;
            int ny = current.first + dir.dx;

            if (!GridUtils::isInBounds(nx, ny, m_maze)) {
                continue;
            }

            if (m_maze[ny][nx] == 1) continue;
            if (m_visitedSet.count({ny, nx}) > 0) continue;

            // 对角线移动检测：确保不会"切角"穿过墙体
            if (dir.isDiagonal) {
                int midX1 = current.second + dir.dy;
                int midY1 = current.first;
                int midX2 = current.second;
                int midY2 = current.first + dir.dx;

                bool path1Clear = GridUtils::isInBounds(midX1, midY1, m_maze) &&
                                 m_maze[midY1][midX1] == 0;
                bool path2Clear = GridUtils::isInBounds(midX2, midY2, m_maze) &&
                                 m_maze[midY2][midX2] == 0;

                if (!path1Clear && !path2Clear) {
                    continue;
                }
            }

            double newDist = currentDist + dir.cost;
            std::pair<int,int> neighbor = {ny, nx};

            if (m_dijkstraDist.count(neighbor) == 0 || newDist < m_dijkstraDist[neighbor]) {
                m_dijkstraDist[neighbor] = newDist;
                m_dijkstraParent[neighbor] = current;
                m_dijkstraPQ.push({newDist, neighbor});
            }
        }
    } else {
        for (const auto& dir : GridUtils::DIRECTIONS_4) {
            int nx = current.second + dir.dy;
            int ny = current.first + dir.dx;

            if (!GridUtils::isInBounds(nx, ny, m_maze)) {
                continue;
            }

            if (m_maze[ny][nx] == 1) continue;
            if (m_visitedSet.count({ny, nx}) > 0) continue;

            double newDist = currentDist + dir.cost;
            std::pair<int,int> neighbor = {ny, nx};

            if (m_dijkstraDist.count(neighbor) == 0 || newDist < m_dijkstraDist[neighbor]) {
                m_dijkstraDist[neighbor] = newDist;
                m_dijkstraParent[neighbor] = current;
                m_dijkstraPQ.push({newDist, neighbor});
            }
        }
    }

    QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepDijkstra);
}

void AlgorithmRunner::startJPS(const std::vector<std::vector<int>>& maze, const std::pair<int,int>& start, const std::pair<int,int>& end, Grid2D& allPoints)
{
    stop();
    m_currentAlgorithm = AlgorithmType::JPS;
    m_running = true;
    m_allPoints = &allPoints;
    m_gridValid = true;
    m_bfsStart = start;
    m_bfsEnd = end;
    m_maze = maze;

    m_jpsPQ = std::priority_queue<JPSNode, std::vector<JPSNode>, std::greater<JPSNode>>();
    m_jpsGCost.clear();
    m_jpsParent.clear();
    m_visitedSet.clear();
    m_jpsFinished = false;

    JPSNode startNode;
    startNode.pos = start;
    startNode.g_cost = 0.0;
    startNode.f_cost = calculateHeuristic(start.second, start.first, end.second, end.first, HeuristicType::Octile);
    startNode.parent = {-1, -1};

    m_jpsGCost[start] = 0.0;
    m_jpsPQ.push(startNode);

    emit algorithmChanged(AlgorithmType::JPS);
    executeNextStepJPS();
}

void AlgorithmRunner::executeNextStepJPS()
{
    if (!m_running || m_jpsFinished) return;

    if (m_jpsPQ.empty()) {
        m_running = false;
        emit searchFinished(false);
        return;
    }

    JPSNode current = m_jpsPQ.top();
    m_jpsPQ.pop();

    if (m_visitedSet.count(current.pos) > 0) {
        QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepJPS);
        return;
    }

    m_visitedSet.insert(current.pos);

    if (current.pos != m_bfsStart && current.pos != m_bfsEnd) {
        StepInfo step = {current.pos.first, current.pos.second, AType::Visited, false};
        emit stepExecuted(step);
    }

    if (current.pos == m_bfsEnd) {
        std::vector<std::pair<int,int>> jumpPoints;
        auto pos = current.pos;
        while (m_jpsParent.count(pos) > 0 && m_jpsParent[pos] != std::make_pair(-1, -1)) {
            jumpPoints.push_back(pos);
            pos = m_jpsParent[pos];
        }
        jumpPoints.push_back(m_bfsStart);
        std::reverse(jumpPoints.begin(), jumpPoints.end());

        std::vector<std::pair<int,int>> fullPath;
        if (jumpPoints.size() >= 2) {
            for (size_t i = 0; i < jumpPoints.size() - 1; i++) {
                auto& from = jumpPoints[i];
                auto& to = jumpPoints[i + 1];
                
                int dx = to.first - from.first;
                int dy = to.second - from.second;
                int steps = std::max(std::abs(dx), std::abs(dy));
                
                if (steps > 0) {
                    int stepX = (dx != 0) ? dx / std::abs(dx) : 0;
                    int stepY = (dy != 0) ? dy / std::abs(dy) : 0;
                    
                    for (int s = 0; s <= steps; s++) {
                        fullPath.push_back({from.first + s * stepX, from.second + s * stepY});
                    }
                }
            }
        } else if (jumpPoints.size() == 1) {
            fullPath = jumpPoints;
        }

        std::set<std::pair<int,int>> emitted;
        for (const auto& p : fullPath) {
            if (p != m_bfsStart && p != m_bfsEnd && emitted.find(p) == emitted.end()) {
                StepInfo step = {p.first, p.second, AType::Path, true};
                emit stepExecuted(step);
                emitted.insert(p);
            }
        }

        m_jpsFinished = true;
        m_running = false;
        emit searchFinished(true);
        return;
    }

    auto successors = findSuccessors(current.pos);

    for (const auto& successor : successors) {
        if (m_visitedSet.count(successor) > 0) continue;

        double dx = successor.second - current.pos.second;
        double dy = successor.first - current.pos.first;
        double distance = std::sqrt(dx * dx + dy * dy);
        double tentativeG = current.g_cost + distance;

        if (m_jpsGCost.count(successor) == 0 || tentativeG < m_jpsGCost[successor]) {
            m_jpsGCost[successor] = tentativeG;
            m_jpsParent[successor] = current.pos;

            JPSNode newNode;
            newNode.pos = successor;
            newNode.g_cost = tentativeG;
            newNode.f_cost = tentativeG + calculateHeuristic(successor.second, successor.first,
                                                             m_bfsEnd.second, m_bfsEnd.first, HeuristicType::Octile);
            newNode.parent = current.pos;

            m_jpsPQ.push(newNode);
        }
    }

    QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepJPS);
}

void AlgorithmRunner::stop()
{
    m_running = false;
    m_gridValid = false;

    // Clear all algorithm data structures to prevent stale state
    while (!m_openSetAStar.empty()) m_openSetAStar.pop();
    while (!m_queueBFS.empty()) m_queueBFS.pop();
    while (!m_stackDFS.empty()) m_stackDFS.pop();
    m_visitedSet.clear();
    m_dfsPath.clear();
    m_bfsParent.clear();
    m_dfsParent.clear();

    while (!m_dijkstraPQ.empty()) m_dijkstraPQ.pop();
    m_dijkstraDist.clear();
    m_dijkstraParent.clear();

    while (!m_jpsPQ.empty()) m_jpsPQ.pop();
    m_jpsGCost.clear();
    m_jpsParent.clear();
}

double AlgorithmRunner::heuristicCost(const APoint* a, const APoint* b)
{
    return calculateHeuristic(a->x, a->y, b->x, b->y, m_heuristicType);
}

double AlgorithmRunner::calculateHeuristic(int x1, int y1, int x2, int y2, HeuristicType type)
{
    int dx = std::abs(x1 - x2);
    int dy = std::abs(y1 - y2);

    HeuristicType effectiveType = type;
    
    if (m_movementType == MovementType::FourDirections) {
        if (type == HeuristicType::Octile || type == HeuristicType::Chebyshev) {
            effectiveType = HeuristicType::Manhattan;
        }
    } else {
        if (type == HeuristicType::Manhattan) {
            effectiveType = HeuristicType::Octile;
        }
    }

    switch (effectiveType) {
        case HeuristicType::Manhattan:
            return static_cast<double>(dx + dy);

        case HeuristicType::Euclidean:
            return std::sqrt(dx * dx + dy * dy);

        case HeuristicType::Chebyshev:
            return static_cast<double>(std::max(dx, dy));

        case HeuristicType::Octile:
            return 1.0 * (dx + dy) + (1.414 - 2.0) * std::min(dx, dy);

        default:
            return static_cast<double>(dx + dy);
    }
}

bool AlgorithmRunner::isWalkable(int x, int y) const
{
    if (m_maze.empty() || m_maze[0].empty()) return false;
    if (y < 0 || y >= static_cast<int>(m_maze.size()) ||
        x < 0 || x >= static_cast<int>(m_maze[0].size())) {
        return false;
    }
    return m_maze[y][x] == 0;
}

std::pair<int,int> AlgorithmRunner::jump(const std::pair<int,int>& current, int dx, int dy)
{
    std::pair<int,int> curr = current;

    while (true) {
        int ny = curr.first + dx;
        int nx = curr.second + dy;

        if (!isWalkable(nx, ny)) {
            return {-1, -1};
        }

        if (dx != 0 && dy != 0) {
            if (!isWalkable(curr.second, ny) && !isWalkable(nx, curr.first)) {
                return {-1, -1};
            }
        }

        std::pair<int,int> next = {ny, nx};

        if (next == m_bfsEnd) {
            return next;
        }

        if (dx != 0 && dy != 0) {
            if ((!isWalkable(nx - dx, ny) && isWalkable(nx - dx, ny + dy)) ||
                (!isWalkable(nx, ny - dy) && isWalkable(nx + dx, ny - dy))) {
                return next;
            }

            if (jump(next, dx, 0).first != -1 || jump(next, 0, dy).first != -1) {
                return next;
            }
        }
        else {
            if (dx != 0) {
                if ((!isWalkable(nx, ny - 1) && isWalkable(nx + dx, ny - 1)) ||
                    (!isWalkable(nx, ny + 1) && isWalkable(nx + dx, ny + 1))) {
                    return next;
                }
            } else {
                if ((!isWalkable(nx - 1, ny) && isWalkable(nx - 1, ny + dy)) ||
                    (!isWalkable(nx + 1, ny) && isWalkable(nx + 1, ny + dy))) {
                    return next;
                }
            }
        }

        curr = next;
    }
}

std::vector<std::pair<int,int>> AlgorithmRunner::pruneNeighbors(const std::pair<int,int>& current, const std::pair<int,int>& parent)
{
    std::vector<std::pair<int,int>> neighbors;

    if (parent.first == -1) {
        // 起点，返回所有可行邻居
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int ny = current.first + dx;
                int nx = current.second + dy;
                if (isWalkable(nx, ny)) {
                    neighbors.push_back({ny, nx});
                }
            }
        }
        return neighbors;
    }

    int dx = (current.first - parent.first);
    int dy = (current.second - parent.second);
    if (dx != 0) dx = dx / std::abs(dx);
    if (dy != 0) dy = dy / std::abs(dy);

    if (dx != 0 && dy != 0) {
        if (isWalkable(current.second, current.first + dx)) {
            neighbors.push_back({current.first + dx, current.second});
        }
        if (isWalkable(current.second + dy, current.first)) {
            neighbors.push_back({current.first, current.second + dy});
        }
        if (isWalkable(current.second + dy, current.first + dx)) {
            neighbors.push_back({current.first + dx, current.second + dy});
        }
        if (!isWalkable(current.second, current.first - dx) &&
            isWalkable(current.second + dy, current.first - dx)) {
            neighbors.push_back({current.first - dx, current.second + dy});
        }
        if (!isWalkable(current.second - dy, current.first) &&
            isWalkable(current.second - dy, current.first + dx)) {
            neighbors.push_back({current.first + dx, current.second - dy});
        }
    }
    else {
        if (dx != 0) {
            if (isWalkable(current.second, current.first + dx)) {
                neighbors.push_back({current.first + dx, current.second});
            }
            if (!isWalkable(current.second + 1, current.first) &&
                isWalkable(current.second + 1, current.first + dx)) {
                neighbors.push_back({current.first + dx, current.second + 1});
            }
            if (!isWalkable(current.second - 1, current.first) &&
                isWalkable(current.second - 1, current.first + dx)) {
                neighbors.push_back({current.first + dx, current.second - 1});
            }
        } else {
            if (isWalkable(current.second + dy, current.first)) {
                neighbors.push_back({current.first, current.second + dy});
            }
            if (!isWalkable(current.second, current.first + 1) &&
                isWalkable(current.second + dy, current.first + 1)) {
                neighbors.push_back({current.first + 1, current.second + dy});
            }
            if (!isWalkable(current.second, current.first - 1) &&
                isWalkable(current.second + dy, current.first - 1)) {
                neighbors.push_back({current.first - 1, current.second + dy});
            }
        }
    }

    return neighbors;
}

std::vector<std::pair<int,int>> AlgorithmRunner::findSuccessors(const std::pair<int,int>& current)
{
    std::vector<std::pair<int,int>> successors;
    std::pair<int,int> parent = m_jpsParent.count(current) > 0 ? m_jpsParent[current] : std::make_pair(-1, -1);

    auto neighbors = pruneNeighbors(current, parent);

    for (const auto& neighbor : neighbors) {
        int dx = neighbor.first - current.first;
        int dy = neighbor.second - current.second;
        if (dx != 0) dx = dx / std::abs(dx);
        if (dy != 0) dy = dy / std::abs(dy);

        auto jumpPoint = jump(current, dx, dy);
        if (jumpPoint.first != -1) {
            successors.push_back(jumpPoint);
        }
    }

    return successors;
}

SearchResult MazeSolver::findPathBFS(const std::vector<std::vector<int>>& maze,
                                                           const std::pair<int, int>& start,
                                                           const std::pair<int, int>& end,
                                                           MovementType movementType)
{
    SearchResult result;
    result.found = false;
    result.visitedCount = 0;

    std::queue<std::pair<int, int>> queue;
    std::set<std::pair<int, int>> visited;
    std::map<std::pair<int, int>, std::pair<int, int>> parent;

    queue.push(start);
    visited.insert(start);

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();
        result.visitedCount++;

        if (current == end) {
            auto node = end;
            while (node != start) {
                result.path.push_back(node);
                node = parent[node];
            }
            result.path.push_back(start);
            std::reverse(result.path.begin(), result.path.end());
            result.found = true;
            return result;
        }

        if (movementType == MovementType::EightDirections) {
            for (const auto& dir : GridUtils::DIRECTIONS_8) {
                int ny = current.first + dir.dx;
                int nx = current.second + dir.dy;

                if (!GridUtils::isInBounds(nx, ny, maze)) {
                    continue;
                }

                if (maze[ny][nx] == 1) continue;
                if (visited.count({ny, nx}) > 0) continue;

                // 对角线移动检测：确保不会"切角"穿过墙体
                if (dir.isDiagonal) {
                    int midX1 = current.second + dir.dy;
                    int midY1 = current.first;
                    int midX2 = current.second;
                    int midY2 = current.first + dir.dx;

                    bool path1Clear = GridUtils::isInBounds(midX1, midY1, maze) &&
                                     maze[midY1][midX1] == 0;
                    bool path2Clear = GridUtils::isInBounds(midX2, midY2, maze) &&
                                     maze[midY2][midX2] == 0;

                    if (!path1Clear && !path2Clear) {
                        continue;
                    }
                }

                visited.insert({ny, nx});
                parent[{ny, nx}] = current;
                queue.push({ny, nx});
            }
        } else {
            for (const auto& dir : GridUtils::DIRECTIONS_4) {
                int ny = current.first + dir.dx;
                int nx = current.second + dir.dy;

                if (!GridUtils::isInBounds(nx, ny, maze)) {
                    continue;
                }

                if (maze[ny][nx] == 1) continue;
                if (visited.count({ny, nx}) > 0) continue;

                visited.insert({ny, nx});
                parent[{ny, nx}] = current;
                queue.push({ny, nx});
            }
        }
    }

    return result;
}

SearchResult MazeSolver::findPathDFS(const std::vector<std::vector<int>>& maze,
                                                           const std::pair<int, int>& start,
                                                           const std::pair<int, int>& end,
                                                           MovementType movementType)
{
    SearchResult result;
    result.found = false;
    result.visitedCount = 0;

    std::vector<std::vector<bool>> visited(maze.size(), std::vector<bool>(maze[0].size(), false));
    result.found = dfsHelper(maze, start.first, start.second, end, visited, result.path, result.visitedCount, movementType);
    std::reverse(result.path.begin(), result.path.end());
    return result;
}

bool MazeSolver::dfsHelper(const std::vector<std::vector<int>>& maze, int row, int col,
                           const std::pair<int, int>& end,
                           std::vector<std::vector<bool>>& visited,
                           std::vector<std::pair<int, int>>& path,
                           int& visitedCount,
                           MovementType movementType)
{
    if (row < 0 || row >= static_cast<int>(maze.size()) ||
        col < 0 || col >= static_cast<int>(maze[0].size())) {
        return false;
    }

    if (maze[row][col] == 1 || visited[row][col]) {
        return false;
    }

    visited[row][col] = true;
    visitedCount++;
    path.push_back({row, col});

    if (std::make_pair(row, col) == end) {
        return true;
    }

    if (movementType == MovementType::EightDirections) {
        for (const auto& dir : GridUtils::DIRECTIONS_8) {
            int newRow = row + dir.dx;
            int newCol = col + dir.dy;

            // 对角线移动检测：确保不会"切角"穿过墙体
            if (dir.isDiagonal) {
                int midRow1 = row;
                int midCol1 = col + dir.dy;
                int midRow2 = row + dir.dx;
                int midCol2 = col;

                bool path1Clear = (midRow1 >= 0 && midRow1 < static_cast<int>(maze.size()) &&
                                  midCol1 >= 0 && midCol1 < static_cast<int>(maze[0].size()) &&
                                  maze[midRow1][midCol1] == 0);
                bool path2Clear = (midRow2 >= 0 && midRow2 < static_cast<int>(maze.size()) &&
                                  midCol2 >= 0 && midCol2 < static_cast<int>(maze[0].size()) &&
                                  maze[midRow2][midCol2] == 0);

                if (!path1Clear && !path2Clear) {
                    continue;
                }
            }

            if (dfsHelper(maze, newRow, newCol, end, visited, path, visitedCount, movementType)) {
                return true;
            }
        }
    } else {
        for (const auto& dir : GridUtils::DIRECTIONS_4) {
            if (dfsHelper(maze, row + dir.dx, col + dir.dy, end, visited, path, visitedCount, movementType)) {
                return true;
            }
        }
    }

    path.pop_back();
    return false;
}

SearchResult MazeSolver::findPathDijkstra(const std::vector<std::vector<int>>& maze,
                                          const std::pair<int, int>& start,
                                          const std::pair<int, int>& end,
                                          MovementType movementType)
{
    SearchResult result;
    result.found = false;
    result.visitedCount = 0;

    int rows = maze.size();
    int cols = maze[0].size();

    std::priority_queue<std::pair<double, std::pair<int,int>>,
                        std::vector<std::pair<double, std::pair<int,int>>>,
                        std::greater<std::pair<double, std::pair<int,int>>>> pq;

    std::map<std::pair<int,int>, double> dist;
    std::map<std::pair<int,int>, std::pair<int,int>> parent;
    std::set<std::pair<int,int>> visited;

    dist[start] = 0.0;
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [currentDist, current] = pq.top();
        pq.pop();

        if (visited.count(current) > 0) {
            continue;
        }

        visited.insert(current);
        result.visitedCount++;

        if (current == end) {
            result.found = true;
            std::vector<std::pair<int,int>> path;
            auto pos = current;
            while (parent.count(pos) > 0) {
                path.push_back(pos);
                pos = parent[pos];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            result.path = path;
            return result;
        }

        if (movementType == MovementType::EightDirections) {
            for (const auto& dir : GridUtils::DIRECTIONS_8) {
                int nx = current.second + dir.dy;
                int ny = current.first + dir.dx;

                if (!GridUtils::isInBounds(nx, ny, maze)) {
                    continue;
                }

                if (maze[ny][nx] == 1) continue;
                if (visited.count({ny, nx}) > 0) continue;

                // 对角线移动检测：确保不会"切角"穿过墙体
                if (dir.isDiagonal) {
                    int midX1 = current.second;
                    int midY1 = ny;
                    int midX2 = nx;
                    int midY2 = current.first;

                    bool path1Clear = GridUtils::isInBounds(midX1, midY1, maze) && maze[midY1][midX1] == 0;
                    bool path2Clear = GridUtils::isInBounds(midX2, midY2, maze) && maze[midY2][midX2] == 0;

                    if (!path1Clear && !path2Clear) {
                        continue;
                    }
                }

                double newDist = currentDist + dir.cost;
                std::pair<int,int> neighbor = {ny, nx};

                if (dist.count(neighbor) == 0 || newDist < dist[neighbor]) {
                    dist[neighbor] = newDist;
                    parent[neighbor] = current;
                    pq.push({newDist, neighbor});
                }
            }
        } else {
            for (const auto& dir : GridUtils::DIRECTIONS_4) {
                int nx = current.second + dir.dy;
                int ny = current.first + dir.dx;

                if (!GridUtils::isInBounds(nx, ny, maze)) {
                    continue;
                }

                if (maze[ny][nx] == 1) continue;
                if (visited.count({ny, nx}) > 0) continue;

                double newDist = currentDist + dir.cost;
                std::pair<int,int> neighbor = {ny, nx};

                if (dist.count(neighbor) == 0 || newDist < dist[neighbor]) {
                    dist[neighbor] = newDist;
                    parent[neighbor] = current;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }

    return result;
}

SearchResult MazeSolver::findPathJPS(const std::vector<std::vector<int>>& maze,
                                     const std::pair<int, int>& start,
                                     const std::pair<int, int>& end)
{
    SearchResult result;
    result.found = false;
    result.visitedCount = 0;

    int rows = maze.size();
    int cols = maze[0].size();

    struct JPSNode {
        std::pair<int,int> pos;
        double g_cost;
        double f_cost;

        bool operator>(const JPSNode& other) const {
            return f_cost > other.f_cost;
        }
    };

    auto heuristic = [](int x1, int y1, int x2, int y2) -> double {
        int dx = std::abs(x1 - x2);
        int dy = std::abs(y1 - y2);
        return 1.0 * (dx + dy) + (1.414 - 2.0) * std::min(dx, dy);
    };

    auto isWalkable = [&maze, rows, cols](int x, int y) -> bool {
        if (y < 0 || y >= rows || x < 0 || x >= cols) return false;
        return maze[y][x] == 0;
    };

    std::function<std::pair<int,int>(const std::pair<int,int>&, int, int)> jump;
    jump = [&](const std::pair<int,int>& current, int dx, int dy) -> std::pair<int,int> {
        int ny = current.first + dx;
        int nx = current.second + dy;

        if (!isWalkable(nx, ny)) {
            return {-1, -1};
        }

        if (dx != 0 && dy != 0) {
            if (!isWalkable(current.second, ny) && !isWalkable(nx, current.first)) {
                return {-1, -1};
            }
        }

        std::pair<int,int> next = {ny, nx};
        if (next == end) return next;

        if (dx != 0 && dy != 0) {
            if ((!isWalkable(nx - dx, ny) && isWalkable(nx - dx, ny + dy)) ||
                (!isWalkable(nx, ny - dy) && isWalkable(nx + dx, ny - dy))) {
                return next;
            }
            if (jump(next, dx, 0).first != -1 || jump(next, 0, dy).first != -1) {
                return next;
            }
        } else {
            if (dx != 0) {
                if ((!isWalkable(nx, ny - 1) && isWalkable(nx + dx, ny - 1)) ||
                    (!isWalkable(nx, ny + 1) && isWalkable(nx + dx, ny + 1))) {
                    return next;
                }
            } else {
                if ((!isWalkable(nx - 1, ny) && isWalkable(nx - 1, ny + dy)) ||
                    (!isWalkable(nx + 1, ny) && isWalkable(nx + 1, ny + dy))) {
                    return next;
                }
            }
        }

        return jump(next, dx, dy);
    };

    std::priority_queue<JPSNode, std::vector<JPSNode>, std::greater<JPSNode>> pq;
    std::map<std::pair<int,int>, double> gCost;
    std::map<std::pair<int,int>, std::pair<int,int>> parent;
    std::set<std::pair<int,int>> visited;

    JPSNode startNode;
    startNode.pos = start;
    startNode.g_cost = 0.0;
    startNode.f_cost = heuristic(start.second, start.first, end.second, end.first);

    gCost[start] = 0.0;
    pq.push(startNode);

    while (!pq.empty()) {
        JPSNode current = pq.top();
        pq.pop();

        if (visited.count(current.pos) > 0) continue;
        visited.insert(current.pos);
        result.visitedCount++;

        if (current.pos == end) {
            std::vector<std::pair<int,int>> jumpPoints;
            auto pos = current.pos;
            while (parent.count(pos) > 0) {
                jumpPoints.push_back(pos);
                pos = parent[pos];
            }
            jumpPoints.push_back(start);
            std::reverse(jumpPoints.begin(), jumpPoints.end());

            if (jumpPoints.size() >= 2) {
                for (size_t i = 0; i < jumpPoints.size() - 1; i++) {
                    auto& from = jumpPoints[i];
                    auto& to = jumpPoints[i + 1];
                    
                    int dx = to.first - from.first;
                    int dy = to.second - from.second;
                    int steps = std::max(std::abs(dx), std::abs(dy));
                    
                    if (steps > 0) {
                        int stepX = (dx != 0) ? dx / std::abs(dx) : 0;
                        int stepY = (dy != 0) ? dy / std::abs(dy) : 0;
                        
                        for (int s = 0; s <= steps; s++) {
                            result.path.push_back({from.first + s * stepX, from.second + s * stepY});
                        }
                    }
                }
            } else if (jumpPoints.size() == 1) {
                result.path = jumpPoints;
            }

            std::set<std::pair<int,int>> seen;
            std::vector<std::pair<int,int>> uniquePath;
            for (const auto& p : result.path) {
                if (seen.find(p) == seen.end()) {
                    seen.insert(p);
                    uniquePath.push_back(p);
                }
            }
            result.path = uniquePath;
            result.found = true;
            return result;
        }

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;

                auto jumpPoint = jump(current.pos, dx, dy);
                if (jumpPoint.first == -1) continue;
                if (visited.count(jumpPoint) > 0) continue;

                double jdx = jumpPoint.first - current.pos.first;
                double jdy = jumpPoint.second - current.pos.second;
                double distance = std::sqrt(jdx * jdx + jdy * jdy);
                double tentativeG = current.g_cost + distance;

                if (gCost.count(jumpPoint) == 0 || tentativeG < gCost[jumpPoint]) {
                    gCost[jumpPoint] = tentativeG;
                    parent[jumpPoint] = current.pos;

                    JPSNode newNode;
                    newNode.pos = jumpPoint;
                    newNode.g_cost = tentativeG;
                    newNode.f_cost = tentativeG + heuristic(jumpPoint.second, jumpPoint.first,
                                                            end.second, end.first);
                    pq.push(newNode);
                }
            }
        }
    }

    return result;
}

void MazeGenerator::generateRandomMaze(Grid2D& grid, int width, int height, double obstacleRatio)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!grid[i][j]) {
                grid[i][j] = std::make_shared<APoint>();
            }
            grid[i][j]->type = AType::Unknown;
            grid[i][j]->x = j;
            grid[i][j]->y = i;
            grid[i][j]->parent = nullptr;
            grid[i][j]->g_cost = 0;
            grid[i][j]->f_cost = 0;
            grid[i][j]->h_cost = 0;
            grid[i][j]->algorithmPath = -1;
            grid[i][j]->isBestPath = false;
            grid[i][j]->pathColor = QColor();

            if ((i == 0 && j == 0) || (i == height-1 && j == width-1)) {
                continue;
            }

            static thread_local std::mt19937 gen(static_cast<unsigned>(
                std::chrono::steady_clock::now().time_since_epoch().count()));
            std::uniform_real_distribution<> dis(0.0, 1.0);
            if (dis(gen) < obstacleRatio) {
                grid[i][j]->type = AType::Wall;
            }
        }
    }

    grid[0][0]->type = AType::Unknown;
    grid[height-1][width-1]->type = AType::Unknown;
}

void MazeGenerator::generateSolvableMaze(Grid2D& grid, int width, int height, double obstacleRatio)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!grid[i][j]) {
                grid[i][j] = std::make_shared<APoint>();
            }
            grid[i][j]->type = AType::Unknown;
            grid[i][j]->x = j;
            grid[i][j]->y = i;
            grid[i][j]->parent = nullptr;
            grid[i][j]->g_cost = 0;
            grid[i][j]->f_cost = 0;
            grid[i][j]->h_cost = 0;
            grid[i][j]->algorithmPath = -1;
            grid[i][j]->isBestPath = false;
            grid[i][j]->pathColor = QColor();
        }
    }

    int totalCells = width * height;
    int targetWalls = static_cast<int>(totalCells * obstacleRatio);
    int currentWalls = 0;

    std::vector<std::pair<int,int>> candidates;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ((i == 0 && j == 0) || (i == height-1 && j == width-1)) continue;
            candidates.push_back({i, j});
        }
    }

    static thread_local std::mt19937 gen(static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<> dis;
    for (size_t i = 0; i < candidates.size(); i++) {
        int j = i + dis(gen, std::uniform_int_distribution<>::param_type(0, 
            static_cast<int>(candidates.size() - i - 1)));
        std::swap(candidates[i], candidates[j]);
    }

    for (const auto& pos : candidates) {
        if (currentWalls >= targetWalls) break;

        int row = pos.first;
        int col = pos.second;

        grid[row][col]->type = AType::Wall;

        if (!isSolvable(grid, {0, 0}, {height-1, width-1})) {
            grid[row][col]->type = AType::Unknown;
        } else {
            currentWalls++;
        }
    }

    grid[0][0]->type = AType::Unknown;
    grid[height-1][width-1]->type = AType::Unknown;
}

void MazeGenerator::generateMazeByDFS(Grid2D& grid, int width, int height)
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!grid[i][j]) {
                grid[i][j] = std::make_shared<APoint>();
            }
            grid[i][j]->type = AType::Wall;
            grid[i][j]->x = j;
            grid[i][j]->y = i;
            grid[i][j]->parent = nullptr;
            grid[i][j]->g_cost = 0;
            grid[i][j]->f_cost = 0;
            grid[i][j]->h_cost = 0;
            grid[i][j]->algorithmPath = -1;
            grid[i][j]->isBestPath = false;
            grid[i][j]->pathColor = QColor();
        }
    }

    std::vector<std::pair<int,int>> stack;
    std::set<std::pair<int,int>> visited;

    stack.push_back({0, 0});
    visited.insert({0, 0});
    grid[0][0]->type = AType::Unknown;

    int dx[] = {-2, 2, 0, 0};
    int dy[] = {0, 0, -2, 2};

    while (!stack.empty()) {
        auto current = stack.back();
        int cx = current.first;
        int cy = current.second;

        std::vector<int> directions = {0, 1, 2, 3};
        static thread_local std::mt19937 gen(static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));
        std::uniform_int_distribution<> dis;
        for (size_t i = 0; i < directions.size(); i++) {
            int j = i + dis(gen, std::uniform_int_distribution<>::param_type(0, 
                static_cast<int>(directions.size() - i - 1)));
            std::swap(directions[i], directions[j]);
        }

        bool found = false;
        for (int dir : directions) {
            int nx = cx + dx[dir];
            int ny = cy + dy[dir];

            if (nx >= 0 && nx < height && ny >= 0 && ny < width && 
                visited.find({nx, ny}) == visited.end()) {
                
                int wallX = cx + dx[dir] / 2;
                int wallY = cy + dy[dir] / 2;
                
                grid[wallX][wallY]->type = AType::Unknown;
                grid[nx][ny]->type = AType::Unknown;
                
                visited.insert({nx, ny});
                stack.push_back({nx, ny});
                found = true;
                break;
            }
        }

        if (!found) {
            stack.pop_back();
        }
    }

    grid[0][0]->type = AType::Unknown;
    grid[height-1][width-1]->type = AType::Unknown;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j]->type == AType::Wall) {
                grid[i][j]->type = AType::Wall;
            }
        }
    }
}

bool MazeGenerator::isSolvable(const Grid2D& grid, const std::pair<int,int>& start, const std::pair<int,int>& end)
{
    int rows = grid.size();
    int cols = grid[0].size();

    std::queue<std::pair<int,int>> queue;
    std::set<std::pair<int,int>> visited;
    std::vector<std::vector<int>> maze(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            maze[i][j] = (grid[i][j]->type == AType::Wall) ? 1 : 0;
        }
    }

    queue.push(start);
    visited.insert(start);

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (current == end) {
            return true;
        }

        for (const auto& dir : GridUtils::DIRECTIONS_4) {
            int ny = current.first + dir.dx;
            int nx = current.second + dir.dy;

            if (ny < 0 || ny >= rows || nx < 0 || nx >= cols) continue;
            if (maze[ny][nx] == 1) continue;
            if (visited.count({ny, nx}) > 0) continue;

            visited.insert({ny, nx});
            queue.push({ny, nx});
        }
    }

    return false;
}

AlgorithmResult MazeSolver::compareAllAlgorithms(const std::vector<std::vector<int>>& maze,
                                                const std::pair<int,int>& start,
                                                const std::pair<int,int>& end,
                                                MovementType movementType)
{
    AlgorithmResult result;
    result.type = AlgorithmType::AStar;
    result.found = false;
    result.visitedCount = 0;
    result.pathLength = 0;
    result.executionTime = 0;

    auto startTime = std::chrono::high_resolution_clock::now();

    int rows = maze.size();
    int cols = maze[0].size();
    std::set<std::pair<int,int>> visited;
    std::priority_queue<std::pair<double, std::pair<int,int>>, std::vector<std::pair<double, std::pair<int,int>>>, std::greater<std::pair<double, std::pair<int,int>>>> openSet;
    std::map<std::pair<int,int>, std::pair<int,int>> parent;
    std::map<std::pair<int,int>, double> gCost;

    auto heuristic = [&](int x, int y) {
        return std::sqrt(std::pow(x - end.second, 2) + std::pow(y - end.first, 2));
    };

    openSet.push({heuristic(start.second, start.first), start});
    parent[start] = {-1, -1};
    gCost[start] = 0;

    while (!openSet.empty()) {
        auto current = openSet.top().second;
        openSet.pop();

        if (visited.count(current) > 0) continue;
        visited.insert(current);

        if (current == end) {
            result.found = true;
            result.visitedCount = visited.size();

            std::vector<std::pair<int,int>> path;
            auto node = end;
            while (node != start) {
                path.push_back(node);
                node = parent[node];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            result.path = path;
            result.pathLength = path.size();
            break;
        }

        auto processNeighbor = [&](const auto& dir) {
            int nx = current.second + dir.dy;
            int ny = current.first + dir.dx;

            if (ny < 0 || ny >= rows || nx < 0 || nx >= cols) return;
            if (maze[ny][nx] == 1) return;
            if (visited.count({ny, nx}) > 0) return;

            if (dir.isDiagonal) {
                int midX1 = current.second + dir.dy;
                int midY1 = current.first;
                int midX2 = current.second;
                int midY2 = current.first + dir.dx;

                bool path1Clear = midY1 >= 0 && midY1 < rows && midX1 >= 0 && midX1 < cols && maze[midY1][midX1] == 0;
                bool path2Clear = midY2 >= 0 && midY2 < rows && midX2 >= 0 && midX2 < cols && maze[midY2][midX2] == 0;

                if (!path1Clear && !path2Clear) return;
            }

            double currentG = gCost[current];
            double newG = currentG + dir.cost;

            double h = heuristic(nx, ny);
            double f = newG + h;

            if (gCost.find({ny, nx}) == gCost.end() || newG < gCost[{ny, nx}]) {
                gCost[{ny, nx}] = newG;
                parent[{ny, nx}] = current;
                openSet.push({f, {ny, nx}});
            }
        };

        if (movementType == MovementType::EightDirections) {
            for (const auto& dir : GridUtils::DIRECTIONS_8) processNeighbor(dir);
        } else {
            for (const auto& dir : GridUtils::DIRECTIONS_4) processNeighbor(dir);
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    return result;
}
