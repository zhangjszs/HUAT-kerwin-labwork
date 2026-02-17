#include "algorithms.h"
#include <map>

AlgorithmRunner::AlgorithmRunner(QObject *parent)
    : QObject(parent)
{
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &AlgorithmRunner::onTimerTimeout);
}

void AlgorithmRunner::startAStar(APointPtr beginPoint, APointPtr endPoint, Grid2D& allPoints)
{
    stop();
    m_currentAlgorithm = AlgorithmType::AStar;
    m_running = true;
    m_allPoints = &allPoints;
    m_startPoint = beginPoint;
    m_endPoint = endPoint;

    m_openSetAStar = std::priority_queue<APointPtr, std::vector<APointPtr>, CompareAStar>();
    m_visitedSet.clear();
    m_aStarPath.clear();
    m_aStarFinished = false;

    for (int i = 0; i < static_cast<int>(allPoints.size()); i++) {
        for (int j = 0; j < static_cast<int>(allPoints[i].size()); j++) {
            allPoints[i][j]->g_cost = 0;
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

void AlgorithmRunner::scheduleNextStep()
{
    if (m_running) {
        m_timer.start(m_stepDelay);
    }
}

void AlgorithmRunner::onTimerTimeout()
{
    if (!m_running) return;

    switch (m_currentAlgorithm) {
    case AlgorithmType::AStar:
        executeNextStepAStar();
        break;
    case AlgorithmType::BFS:
        executeNextStepBFS();
        break;
    case AlgorithmType::DFS:
        executeNextStepDFS();
        break;
    }
}

void AlgorithmRunner::executeNextStepAStar()
{
    if (!m_running || m_aStarFinished) return;

    if (m_openSetAStar.empty()) {
        m_running = false;
        m_timer.stop();
        emit searchFinished(false);
        return;
    }

    auto current = m_openSetAStar.top();
    m_openSetAStar.pop();

    std::pair<int,int> currPos = {current->y, current->x};
    if (m_visitedSet.count(currPos) > 0) {
        scheduleNextStep();
        return;
    }
    m_visitedSet.insert(currPos);

    if (current->type != AType::StartPoint) {
        StepInfo step = {current->x, current->y, AType::Visited, false};
        emit stepExecuted(step);
    }

    if (current.get() == m_endPoint.get()) {
        QVector<QPoint> path = reconstructPathAStar(current);
        m_running = false;
        m_aStarFinished = true;
        m_timer.stop();
        emit pathFound(path);
        emit searchFinished(true);
        return;
    }

    static const int directions[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};
    for (const auto& dir : directions) {
        int nx = current->x + dir[0];
        int ny = current->y + dir[1];

        if (ny < 0 || ny >= static_cast<int>(m_allPoints->size()) ||
            nx < 0 || nx >= static_cast<int>((*m_allPoints)[0].size())) {
            continue;
        }

        auto neighbor = (*m_allPoints)[ny][nx];
        if (neighbor->type == AType::Wall) continue;
        if (m_visitedSet.count({ny, nx}) > 0) continue;

        bool isDiagonal = (dir[0] != 0 && dir[1] != 0);
        double moveCost = isDiagonal ? 1.414 : 1.0;
        double tentativeG = current->g_cost + moveCost;

        if (neighbor->parent == nullptr || tentativeG < neighbor->g_cost) {
            neighbor->parent = current;
            neighbor->g_cost = tentativeG;
            neighbor->h_cost = heuristicCost(neighbor.get(), m_endPoint.get());
            neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
            m_openSetAStar.push(neighbor);
        }
    }

    scheduleNextStep();
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
        StepInfo step = {path[i].x(), path[i].y(), AType::Path, true};
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
    m_bfsStart = start;
    m_bfsEnd = end;
    m_maze = maze;
    m_bfsParent.clear();

    m_queueBFS = std::queue<std::pair<int,int>>();
    m_visitedSet.clear();
    m_queueBFS.push(start);
    m_visitedSet.insert(start);

    emit algorithmChanged(AlgorithmType::BFS);
    executeNextStepBFS();
}

void AlgorithmRunner::executeNextStepBFS()
{
    if (!m_running) return;

    if (m_queueBFS.empty()) {
        m_running = false;
        m_timer.stop();
        emit searchFinished(false);
        return;
    }

    auto current = m_queueBFS.front();
    m_queueBFS.pop();

    if (current != m_bfsStart && current != m_bfsEnd) {
        StepInfo step = {current.second, current.first, AType::Visited, false};
        emit stepExecuted(step);
    }

    if (current == m_bfsEnd) {
        std::vector<std::pair<int,int>> path;
        auto node = m_bfsEnd;
        while (node != m_bfsStart) {
            path.push_back(node);
            node = m_bfsParent[node];
        }
        path.push_back(m_bfsStart);
        std::reverse(path.begin(), path.end());
        
        for (size_t i = 1; i < path.size() - 1; i++) {
            StepInfo step = {path[i].second, path[i].first, AType::Path, true};
            emit stepExecuted(step);
        }
        
        m_running = false;
        m_timer.stop();
        emit searchFinished(true);
        return;
    }

    static const int directions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (const auto& dir : directions) {
        int nx = current.second + dir[1];
        int ny = current.first + dir[0];

        if (ny < 0 || ny >= static_cast<int>(m_maze.size()) ||
            nx < 0 || nx >= static_cast<int>(m_maze[0].size())) {
            continue;
        }

        if (m_maze[ny][nx] == 1) continue;
        if (m_visitedSet.count({ny, nx}) > 0) continue;

        m_visitedSet.insert({ny, nx});
        m_bfsParent[{ny, nx}] = current;
        m_queueBFS.push({ny, nx});
    }

    scheduleNextStep();
}

void AlgorithmRunner::startDFS(const std::vector<std::vector<int>>& maze, const std::pair<int,int>& start, const std::pair<int,int>& end, Grid2D& allPoints)
{
    stop();
    m_currentAlgorithm = AlgorithmType::DFS;
    m_running = true;
    m_allPoints = &allPoints;
    m_bfsStart = start;
    m_bfsEnd = end;
    m_maze = maze;

    m_stackDFS = std::stack<std::pair<int,int>>();
    m_visitedSet.clear();
    m_dfsParent.clear();
    m_stackDFS.push(start);
    m_visitedSet.insert(start);
    m_dfsParent[start] = {-1, -1};

    emit algorithmChanged(AlgorithmType::DFS);
    executeNextStepDFS();
}

void AlgorithmRunner::executeNextStepDFS()
{
    if (!m_running) return;

    if (m_stackDFS.empty()) {
        m_running = false;
        m_timer.stop();
        emit searchFinished(false);
        return;
    }

    auto current = m_stackDFS.top();
    m_stackDFS.pop();

    if (current != m_bfsStart && current != m_bfsEnd) {
        StepInfo step = {current.second, current.first, AType::Visited, false};
        emit stepExecuted(step);
    }

    if (current == m_bfsEnd) {
        std::vector<std::pair<int,int>> path;
        auto node = m_bfsEnd;
        while (node != m_bfsStart) {
            path.push_back(node);
            auto it = m_dfsParent.find(node);
            if (it == m_dfsParent.end()) break;
            node = it->second;
        }
        path.push_back(m_bfsStart);
        std::reverse(path.begin(), path.end());
        for (size_t i = 1; i < path.size() - 1; i++) {
            StepInfo step = {path[i].second, path[i].first, AType::Path, true};
            emit stepExecuted(step);
        }
        m_running = false;
        m_timer.stop();
        emit searchFinished(true);
        return;
    }

    static const int directions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (const auto& dir : directions) {
        int nx = current.second + dir[1];
        int ny = current.first + dir[0];

        if (ny < 0 || ny >= static_cast<int>(m_maze.size()) ||
            nx < 0 || nx >= static_cast<int>(m_maze[0].size())) {
            continue;
        }

        if (m_maze[ny][nx] == 1) continue;
        if (m_visitedSet.count({ny, nx}) > 0) continue;

        m_visitedSet.insert({ny, nx});
        m_dfsParent[{ny, nx}] = current;
        m_stackDFS.push({ny, nx});
    }

    scheduleNextStep();
}

void AlgorithmRunner::stop()
{
    m_running = false;
    m_timer.stop();
}

double AlgorithmRunner::heuristicCost(const APoint* a, const APoint* b)
{
    return std::sqrt(std::pow(a->x - b->x, 2) + std::pow(a->y - b->y, 2));
}

std::vector<std::pair<int, int>> MazeSolver::findPathBFS(const std::vector<std::vector<int>>& maze,
                                                         const std::pair<int, int>& start,
                                                         const std::pair<int, int>& end,
                                                         int* visitedCount)
{
    std::vector<std::pair<int, int>> path;
    std::queue<std::pair<int, int>> queue;
    std::set<std::pair<int, int>> visited;
    std::map<std::pair<int, int>, std::pair<int, int>> parent;

    queue.push(start);
    visited.insert(start);

    static const int directions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (current == end) {
            auto node = end;
            while (node != start) {
                path.push_back(node);
                node = parent[node];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            if (visitedCount) *visitedCount = static_cast<int>(visited.size());
            return path;
        }

        for (const auto& dir : directions) {
            int ny = current.first + dir[0];
            int nx = current.second + dir[1];

            if (ny < 0 || ny >= static_cast<int>(maze.size()) ||
                nx < 0 || nx >= static_cast<int>(maze[0].size())) {
                continue;
            }

            if (maze[ny][nx] == 1) continue;
            if (visited.count({ny, nx}) > 0) continue;

            visited.insert({ny, nx});
            parent[{ny, nx}] = current;
            queue.push({ny, nx});
        }
    }

    if (visitedCount) *visitedCount = static_cast<int>(visited.size());
    return path;
}

std::vector<std::pair<int, int>> MazeSolver::findPathDFS(const std::vector<std::vector<int>>& maze,
                                                         const std::pair<int, int>& start,
                                                         const std::pair<int, int>& end,
                                                         int* visitedCount)
{
    std::vector<std::vector<bool>> visited(maze.size(), std::vector<bool>(maze[0].size(), false));
    std::vector<std::pair<int, int>> path;
    int count = 0;
    dfsHelper(maze, start.first, start.second, end, visited, path, &count);
    std::reverse(path.begin(), path.end());
    if (visitedCount) *visitedCount = count;
    return path;
}

bool MazeSolver::dfsHelper(const std::vector<std::vector<int>>& maze, int row, int col,
                           const std::pair<int, int>& end,
                           std::vector<std::vector<bool>>& visited,
                           std::vector<std::pair<int, int>>& path,
                           int* visitedCount)
{
    if (row < 0 || row >= static_cast<int>(maze.size()) ||
        col < 0 || col >= static_cast<int>(maze[0].size())) {
        return false;
    }

    if (maze[row][col] == 1 || visited[row][col]) {
        return false;
    }

    visited[row][col] = true;
    if (visitedCount) {
        (*visitedCount)++;
    }
    path.push_back({row, col});

    if (std::make_pair(row, col) == end) {
        return true;
    }

    static const int directions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (const auto& dir : directions) {
        if (dfsHelper(maze, row + dir[0], col + dir[1], end, visited, path, visitedCount)) {
            return true;
        }
    }

    path.pop_back();
    return false;
}

void MazeGenerator::generateRandomMaze(Grid2D& grid, int rows, int cols, double obstacleRatio)
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (!grid[row][col]) {
                grid[row][col] = std::make_shared<APoint>();
            }
            grid[row][col]->type = AType::Unknow;
            grid[row][col]->x = col;
            grid[row][col]->y = row;
            grid[row][col]->parent = nullptr;
            grid[row][col]->g_cost = 0;
            grid[row][col]->f_cost = 0;
            grid[row][col]->h_cost = 0;

            if ((row == 0 && col == 0) || (row == rows - 1 && col == cols - 1)) {
                continue;
            }

            if (rand() / static_cast<double>(RAND_MAX) < obstacleRatio) {
                grid[row][col]->type = AType::Wall;
            }
        }
    }

    grid[0][0]->type = AType::Unknow;
    grid[rows - 1][cols - 1]->type = AType::Unknow;
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

    static const int directions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (current == end) {
            return true;
        }

        for (const auto& dir : directions) {
            int ny = current.first + dir[0];
            int nx = current.second + dir[1];

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
                                                const std::pair<int,int>& end)
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
    std::map<std::pair<int,int>, std::pair<double, std::pair<int,int>>> parent;

    auto heuristic = [&](int x, int y) {
        return std::sqrt(std::pow(x - end.second, 2) + std::pow(y - end.first, 2));
    };

    openSet.push({heuristic(start.second, start.first), start});
    parent[start] = {0.0, {-1, -1}};

    static const int directions[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,-1},{-1,1},{1,-1},{1,1}};

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
                node = parent[node].second;
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            result.path = path;
            result.pathLength = path.size();
            break;
        }

        for (const auto& dir : directions) {
            int nx = current.second + dir[1];
            int ny = current.first + dir[0];

            if (ny < 0 || ny >= rows || nx < 0 || nx >= cols) continue;
            if (maze[ny][nx] == 1) continue;
            if (visited.count({ny, nx}) > 0) continue;

            bool isDiagonal = (dir[0] != 0 && dir[1] != 0);
            double moveCost = isDiagonal ? 1.414 : 1.0;
            
            double currentG = 0;
            auto it = parent.find(current);
            if (it != parent.end() && it->second.second != std::pair<int,int>(-1,-1)) {
                currentG = it->second.first;
            }
            double newG = currentG + moveCost;

            double h = heuristic(nx, ny);
            double f = newG + h;
            
            if (parent.find({ny, nx}) == parent.end() || newG < parent[{ny, nx}].first) {
                parent[{ny, nx}] = {newG, current};
                openSet.push({f, {ny, nx}});
            }
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    return result;
}
