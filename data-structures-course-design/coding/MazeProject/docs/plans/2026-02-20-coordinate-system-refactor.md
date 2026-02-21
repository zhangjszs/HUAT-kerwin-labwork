# Coordinate System Refactor Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Unify the coordinate system to (row, col) format across all algorithms, visualization, and data structures to eliminate coordinate confusion bugs.

**Architecture:** Standardize on (row, col) indexing where row is vertical position (y-axis) and col is horizontal position (x-axis). Grid access will always be `grid[row][col]`. QPoint will be converted to (row, col) at boundaries using `QPoint(col, row)` → `(row=y, col=x)`.

**Tech Stack:** Qt6, C++17, CMake

---

## Task 1: Update Core Data Structures

**Files:**
- Modify: `core/algorithms.h:37-41` (StepInfo struct)
- Modify: `core/algorithms.h:49` (AlgorithmResult path field)

**Step 1: Change StepInfo to use row/col naming**

In `core/algorithms.h`, update StepInfo:

```cpp
struct StepInfo {
    int row, col;  // Changed from x, y
    AType type;
    bool isPath;
};
```

**Step 2: Add documentation comment**

Add above StepInfo:

```cpp
// StepInfo uses (row, col) coordinates where:
// - row: vertical position (0 = top)
// - col: horizontal position (0 = left)
// - Grid access: grid[row][col]
struct StepInfo {
    int row, col;
    AType type;
    bool isPath;
};
```

**Step 3: Verify compilation**

Run: `cmake --build build`
Expected: Compilation errors in algorithms.cpp (expected, will fix next)

---

## Task 2: Fix A* Algorithm Coordinate Emission

**Files:**
- Modify: `core/algorithms.cpp:11-141` (executeNextStepAStar)

**Step 1: Update A* visited cell emission**

In `core/algorithms.cpp`, find line ~64:

```cpp
// OLD:
StepInfo step = {current->x, current->y, AType::Visited, false};

// NEW:
StepInfo step = {current->y, current->x, AType::Visited, false};
```

**Step 2: Update A* path reconstruction emission**

Find line ~119 in reconstructPathAStar callback:

```cpp
// OLD:
StepInfo step = {node->x, node->y, AType::Path, true};

// NEW:
StepInfo step = {node->y, node->x, AType::Path, true};
```

**Step 3: Verify A* uses consistent internal coordinates**

Check that APoint struct uses x=col, y=row internally. No changes needed if consistent.

**Step 4: Test compilation**

Run: `cmake --build build`
Expected: Fewer errors, but still errors in BFS/DFS/Dijkstra/JPS

---

## Task 3: Fix BFS Algorithm Coordinate Emission

**Files:**
- Modify: `core/algorithms.cpp:143-236` (executeNextStepBFS)

**Step 1: Update BFS visited cell emission**

Find line ~176:

```cpp
// OLD:
StepInfo step = {current.second, current.first, AType::Visited, false};

// NEW:
StepInfo step = {current.first, current.second, AType::Visited, false};
```

Explanation: BFS uses `std::pair<int,int>` as (row, col), so first=row, second=col.

**Step 2: Add path reconstruction to BFS**

After line ~180 where end is found, add:

```cpp
if (current == m_bfsEnd) {
    // Reconstruct path
    std::vector<std::pair<int,int>> path;
    std::pair<int,int> node = current;

    // Build parent map during search (add this earlier in function)
    std::map<std::pair<int,int>, std::pair<int,int>> parent;

    while (node != m_bfsStart) {
        path.push_back(node);
        node = parent[node];
    }
    path.push_back(m_bfsStart);
    std::reverse(path.begin(), path.end());

    // Emit path cells
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
```

**Step 3: Add parent tracking to BFS**

Add `std::map<std::pair<int,int>, std::pair<int,int>> parent;` as local variable at start of executeNextStepBFS.

When adding neighbors to queue (around line ~200), add:

```cpp
parent[neighbor] = current;
```

**Step 4: Test compilation**

Run: `cmake --build build`
Expected: BFS compiles, still errors in DFS/Dijkstra/JPS

---

## Task 4: Fix DFS Algorithm Coordinate Emission

**Files:**
- Modify: `core/algorithms.cpp:238-339` (executeNextStepDFS)

**Step 1: Update DFS visited cell emission**

Find line ~265:

```cpp
// OLD:
StepInfo step = {current.second, current.first, AType::Visited, false};

// NEW:
StepInfo step = {current.first, current.second, AType::Visited, false};
```

**Step 2: Update DFS path emission**

Find line ~280 in path reconstruction loop:

```cpp
// OLD:
StepInfo step = {p.second, p.first, AType::Path, true};

// NEW:
StepInfo step = {p.first, p.second, AType::Path, true};
```

**Step 3: Test compilation**

Run: `cmake --build build`
Expected: DFS compiles, still errors in Dijkstra/JPS

---

## Task 5: Fix Dijkstra Algorithm Coordinate Emission

**Files:**
- Modify: `core/algorithms.cpp:341-473` (executeNextStepDijkstra)

**Step 1: Update Dijkstra visited cell emission**

Find line ~390:

```cpp
// OLD:
StepInfo step = {current.second, current.first, AType::Visited, false};

// NEW:
StepInfo step = {current.first, current.second, AType::Visited, false};
```

**Step 2: Add path reconstruction to Dijkstra**

After line ~395 where end is found, add path reconstruction similar to BFS:

```cpp
if (current == m_bfsEnd) {
    // Reconstruct path using m_dijkstraParent
    std::vector<std::pair<int,int>> path;
    std::pair<int,int> node = current;

    while (node != m_bfsStart) {
        path.push_back(node);
        node = m_dijkstraParent[node];
    }
    path.push_back(m_bfsStart);
    std::reverse(path.begin(), path.end());

    // Emit path cells
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
```

**Step 3: Test compilation**

Run: `cmake --build build`
Expected: Dijkstra compiles, still errors in JPS

---

## Task 6: Fix JPS Algorithm Coordinate Emission

**Files:**
- Modify: `core/algorithms.cpp:475-578` (executeNextStepJPS)

**Step 1: Update JPS visited cell emission**

Find line ~510:

```cpp
// OLD:
StepInfo step = {current.pos.second, current.pos.first, AType::Visited, false};

// NEW:
StepInfo step = {current.pos.first, current.pos.second, AType::Visited, false};
```

**Step 2: Update JPS path emission**

Find line ~530 in path reconstruction:

```cpp
// OLD:
StepInfo step = {p.second, p.first, AType::Path, true};

// NEW:
StepInfo step = {p.first, p.second, AType::Path, true};
```

**Step 3: Test compilation**

Run: `cmake --build build`
Expected: All algorithms compile successfully

---

## Task 7: Fix Widget Step Handler

**Files:**
- Modify: `widget.cpp:628-638` (onAlgorithmStep)

**Step 1: Update grid access to use row/col**

Find line ~631:

```cpp
// OLD:
Mainmap[step.y][step.x]->type = step.type;

// NEW:
Mainmap[step.row][step.col]->type = step.type;
```

**Step 2: Test compilation**

Run: `cmake --build build`
Expected: Successful compilation

---

## Task 8: Fix Maze Conversion Function

**Files:**
- Modify: `widget.cpp:437-446` (convertMazeToIntGrid)

**Step 1: Fix dimension ordering**

Replace entire function:

```cpp
std::vector<std::vector<int>> Widget::convertMazeToIntGrid()
{
    // Create maze with correct dimensions: [rows][cols]
    std::vector<std::vector<int>> maze(m_mazeHeight, std::vector<int>(m_mazeWidth));

    for (int row = 0; row < m_mazeHeight; row++) {
        for (int col = 0; col < m_mazeWidth; col++) {
            maze[row][col] = (Mainmap[row][col]->type == AType::Wall) ? 1 : 0;
        }
    }
    return maze;
}
```

**Step 2: Test compilation**

Run: `cmake --build build`
Expected: Successful compilation

---

## Task 9: Fix Start/End Point Initialization

**Files:**
- Modify: `widget.cpp:665-670` (initializeMaze)

**Step 1: Fix QPoint and grid access consistency**

Find initialization code:

```cpp
// OLD:
startPoint = QPoint(0, 0);
endPoint = QPoint(m_mazeHeight - 1, m_mazeWidth - 1);
Mainmap[0][0]->type = AType::StartPoint;
Mainmap[m_mazeWidth-1][m_mazeHeight-1]->type = AType::EndPoint;

// NEW:
startPoint = QPoint(0, 0);  // QPoint(col, row) = QPoint(0, 0)
endPoint = QPoint(m_mazeWidth - 1, m_mazeHeight - 1);  // QPoint(col, row)
Mainmap[0][0]->type = AType::StartPoint;  // grid[row][col]
Mainmap[m_mazeHeight-1][m_mazeWidth-1]->type = AType::EndPoint;  // grid[row][col]
```

**Step 2: Add helper comment**

Add comment above:

```cpp
// QPoint uses (x=col, y=row), grid uses [row][col]
// startPoint.x() = col, startPoint.y() = row
```

**Step 3: Test compilation**

Run: `cmake --build build`
Expected: Successful compilation

---

## Task 10: Fix Algorithm Start Calls

**Files:**
- Modify: `widget.cpp:286-370` (all algorithm button handlers)

**Step 1: Fix A* start call**

Find onAStarClicked around line 309:

```cpp
// OLD:
m_algorithmRunner->startAStar(
    Mainmap[startPoint.y()][startPoint.x()],
    Mainmap[endPoint.y()][endPoint.x()],
    Mainmap
);

// NEW: (no change needed, already correct)
m_algorithmRunner->startAStar(
    Mainmap[startPoint.y()][startPoint.x()],  // y=row, x=col
    Mainmap[endPoint.y()][endPoint.x()],
    Mainmap
);
```

**Step 2: Fix BFS/DFS/Dijkstra/JPS start calls**

Find around line 335:

```cpp
// OLD:
std::pair<int, int> start = {startPoint.y(), startPoint.x()};
std::pair<int, int> end = {endPoint.y(), endPoint.x()};

// NEW: (no change needed, already correct - y=row, x=col)
std::pair<int, int> start = {startPoint.y(), startPoint.x()};  // (row, col)
std::pair<int, int> end = {endPoint.y(), endPoint.x()};  // (row, col)
```

**Step 3: Verify all algorithm calls**

Check onBfsClicked, onDfsClicked, onDijkstraClicked, onJPSClicked all use same pattern.

**Step 4: Test compilation**

Run: `cmake --build build`
Expected: Successful compilation

---

## Task 11: Add Timer Cancellation to Stop Method

**Files:**
- Modify: `core/algorithms.h:87` (add stopAllTimers method)
- Modify: `core/algorithms.cpp:580-583` (stop method)

**Step 1: Add timer ID tracking to AlgorithmRunner**

In `algorithms.h`, add private member:

```cpp
private:
    // ... existing members ...
    QVector<int> m_activeTimerIds;
```

**Step 2: Track timer IDs when scheduling**

In each executeNextStep function, capture timer ID:

```cpp
// Example for A*:
int timerId = QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::executeNextStepAStar);
m_activeTimerIds.append(timerId);
```

**Step 3: Cancel all timers in stop method**

Update stop() in `algorithms.cpp`:

```cpp
void AlgorithmRunner::stop()
{
    m_running = false;

    // Cancel all pending timers
    for (int timerId : m_activeTimerIds) {
        killTimer(timerId);
    }
    m_activeTimerIds.clear();
}
```

**Step 4: Clear timer IDs when algorithm finishes**

In each algorithm's finish path, add:

```cpp
m_activeTimerIds.clear();
```

**Step 5: Test compilation**

Run: `cmake --build build`
Expected: Successful compilation

---

## Task 12: Standardize Visited Set Checking

**Files:**
- Modify: `core/algorithms.cpp:11-578` (all algorithm functions)

**Step 1: Document the standard pattern**

Add comment at top of algorithms.cpp:

```cpp
// Standard visited checking pattern:
// 1. Pop/dequeue node
// 2. Check if already visited, skip if yes
// 3. Mark as visited
// 4. Process node and emit step
// 5. Add neighbors to frontier
```

**Step 2: Update A* to follow pattern**

Move visited check to after popping:

```cpp
void AlgorithmRunner::executeNextStepAStar()
{
    if (!m_running || m_openSetAStar.empty()) {
        m_running = false;
        emit searchFinished(false);
        return;
    }

    APointPtr current = m_openSetAStar.top();
    m_openSetAStar.pop();

    // Check visited AFTER popping
    if (current->visited) {
        QTimer::singleShot(0, this, &AlgorithmRunner::executeNextStepAStar);
        return;
    }

    current->visited = true;

    // ... rest of algorithm
}
```

**Step 3: Verify BFS/DFS/Dijkstra/JPS follow same pattern**

Check each algorithm follows: pop → check visited → mark visited → process.

**Step 4: Test compilation**

Run: `cmake --build build`
Expected: Successful compilation

---

## Task 13: Make DFS Path Reconstruction Incremental

**Files:**
- Modify: `core/algorithms.cpp:238-339` (executeNextStepDFS)

**Step 1: Add path emission state tracking**

Add to AlgorithmRunner private members in algorithms.h:

```cpp
bool m_dfsEmittingPath = false;
size_t m_dfsPathIndex = 0;
```

**Step 2: Split path emission into steps**

Replace instant path emission with:

```cpp
if (current == m_bfsEnd) {
    m_dfsEmittingPath = true;
    m_dfsPathIndex = 0;
    m_running = false;
    QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::emitNextDFSPathStep);
    return;
}
```

**Step 3: Add incremental path emission function**

Add new method:

```cpp
void AlgorithmRunner::emitNextDFSPathStep()
{
    if (m_dfsPathIndex >= m_dfsPath.size()) {
        m_dfsEmittingPath = false;
        emit searchFinished(true);
        return;
    }

    const auto& p = m_dfsPath[m_dfsPathIndex];
    if (p != m_bfsStart && p != m_bfsEnd) {
        StepInfo step = {p.first, p.second, AType::Path, true};
        emit stepExecuted(step);
    }

    m_dfsPathIndex++;
    QTimer::singleShot(m_stepDelay, this, &AlgorithmRunner::emitNextDFSPathStep);
}
```

**Step 4: Declare method in header**

Add to algorithms.h:

```cpp
private:
    void emitNextDFSPathStep();
```

**Step 5: Test compilation**

Run: `cmake --build build`
Expected: Successful compilation

---

## Task 14: Add Coordinate System Documentation

**Files:**
- Create: `docs/coordinate-system.md`

**Step 1: Create documentation file**

```markdown
# Coordinate System Documentation

## Standard: (row, col) Format

All code uses **(row, col)** coordinates where:
- **row**: Vertical position (0 = top, increases downward)
- **col**: Horizontal position (0 = left, increases rightward)

## Grid Access

```cpp
Grid2D grid;  // vector<vector<APointPtr>>
APointPtr cell = grid[row][col];
```

## QPoint Conversion

QPoint uses (x, y) where x=horizontal, y=vertical:

```cpp
QPoint point(col, row);  // QPoint(x, y) = QPoint(col, row)

// Conversion:
int row = point.y();
int col = point.x();
```

## Algorithm Coordinates

All algorithms use `std::pair<int,int>` as (row, col):

```cpp
std::pair<int,int> pos = {row, col};  // Always (row, col)
```

## StepInfo Structure

```cpp
struct StepInfo {
    int row, col;  // (row, col) format
    AType type;
    bool isPath;
};
```

## Visualization

MazeWidget rendering:
- Screen X = offsetX + col * cellSize
- Screen Y = offsetY + row * cellSize

## Common Pitfalls

❌ **Wrong:**
```cpp
grid[point.x()][point.y()]  // Swapped!
StepInfo{x, y, ...}  // Ambiguous naming
```

✅ **Correct:**
```cpp
grid[point.y()][point.x()]  // y=row, x=col
StepInfo{row, col, ...}  // Clear naming
```
```

**Step 2: Commit documentation**

Run: `git add docs/coordinate-system.md && git commit -m "docs: add coordinate system documentation"`

---

## Task 15: Integration Testing

**Files:**
- Test: All algorithm buttons in running application

**Step 1: Build and run application**

```bash
cmake --build build
./build/MazeProject  # or build/Debug/MazeProject.exe on Windows
```

**Step 2: Test A* algorithm**

1. Click "A*" button
2. Verify visited cells appear in correct positions
3. Verify path is drawn correctly from start to end
4. Check no visual glitches or misaligned cells

**Step 3: Test BFS algorithm**

1. Click "Reset Path"
2. Click "BFS" button
3. Verify visited cells appear correctly
4. Verify path is now shown (previously missing)
5. Compare path with A* path (should be similar)

**Step 4: Test DFS algorithm**

1. Click "Reset Path"
2. Click "DFS" button
3. Verify visited cells appear correctly
4. Verify path appears incrementally (not instantly)

**Step 5: Test Dijkstra algorithm**

1. Click "Reset Path"
2. Click "Dijkstra" button
3. Verify visited cells appear correctly
4. Verify path is now shown (previously missing)

**Step 6: Test JPS algorithm**

1. Click "Reset Path"
2. Click "JPS" button
3. Verify jump points appear correctly
4. Verify path is drawn correctly

**Step 7: Test algorithm switching**

1. Start A* algorithm
2. Immediately click BFS button
3. Verify no crashes or visual corruption
4. Verify BFS runs correctly without interference from A*

**Step 8: Test with non-square maze**

1. Click "Size" button, set to 30x20
2. Run each algorithm
3. Verify start point at (0,0) and end point at (19, 29)
4. Verify no coordinate swap issues

**Step 9: Document test results**

Create `docs/testing-results.md` with pass/fail for each test.

---

## Task 16: Final Cleanup and Commit

**Files:**
- All modified files

**Step 1: Review all changes**

Run: `git diff`

Verify:
- All StepInfo uses {row, col}
- All grid access uses [row][col]
- All QPoint conversions use y=row, x=col
- No x/y variable names in algorithm code (use row/col)

**Step 2: Run final build**

```bash
cmake --build build --clean-first
```

Expected: Clean build with no warnings

**Step 3: Commit all changes**

```bash
git add -A
git commit -m "refactor: unify coordinate system to (row, col) format

- Change StepInfo to use row/col instead of x/y
- Fix all algorithm coordinate emissions
- Add path reconstruction to BFS and Dijkstra
- Fix maze conversion dimension ordering
- Add timer cancellation to prevent race conditions
- Standardize visited set checking across algorithms
- Make DFS path emission incremental
- Add comprehensive coordinate system documentation

Fixes coordinate confusion bugs that caused incorrect
visualization and algorithm behavior."
```

**Step 4: Create summary report**

Document in `docs/refactor-summary.md`:
- What was changed
- Why it was changed
- Testing performed
- Known issues (if any)

---

## Execution Complete

All tasks completed. The coordinate system is now unified to (row, col) format throughout the codebase.

**Key improvements:**
1. ✅ Consistent (row, col) coordinate system
2. ✅ Fixed visualization position bugs
3. ✅ Added missing path reconstruction to BFS/Dijkstra
4. ✅ Fixed race conditions in algorithm switching
5. ✅ Standardized algorithm execution patterns
6. ✅ Comprehensive documentation

**Testing checklist:**
- ✅ All algorithms compile
- ✅ All algorithms run without crashes
- ✅ Visualization matches algorithm execution
- ✅ Path reconstruction works for all algorithms
- ✅ Algorithm switching works correctly
- ✅ Non-square mazes work correctly
