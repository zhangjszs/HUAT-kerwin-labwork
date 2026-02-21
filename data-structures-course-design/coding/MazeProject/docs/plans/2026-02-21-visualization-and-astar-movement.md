# Visualization Clarity & A* Movement Type Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Fix StartPoint color collision with BFS path overlay, and make A* respect the `m_movementType` setting.

**Architecture:** Two independent changes — (1) color constant swap in `mazewidget.cpp` + tips dialog update in `widget.cpp`, (2) add movement-type branch in `executeNextStepAStar` in `algorithms.cpp`.

**Tech Stack:** Qt 6, C++17, QColor, QTimer-based step execution

---

### Task 1: Fix StartPoint color collision

**Problem:** `StartPoint` renders as `QColor(52, 152, 219)` — identical to the BFS path overlay (`algorithmPath == 0`). During or after a compare run, the start cell is indistinguishable from BFS path cells.

**Files:**
- Modify: `mazewidget.cpp` — change StartPoint fill color
- Modify: `widget.cpp` — update tips dialog color description

**Step 1: Change StartPoint color in paintEvent**

In `mazewidget.cpp`, line 60, change:
```cpp
// BEFORE
} else if (type == AType::StartPoint) {
    painter.fillRect(cellRect, QColor(52, 152, 219));
```
to:
```cpp
// AFTER — orange, distinct from all other colors
} else if (type == AType::StartPoint) {
    painter.fillRect(cellRect, QColor(243, 156, 18));
```

**Step 2: Update tips dialog to match**

In `widget.cpp`, inside `showTipsDialog()`, find the StartPoint color description line:
```cpp
"<li><span style='background-color:#3498db; color:white; padding:2px;'>&nbsp;蓝色&nbsp;</span> 起点</li>"
```
Change to:
```cpp
"<li><span style='background-color:#f39c12; color:white; padding:2px;'>&nbsp;橙色&nbsp;</span> 起点</li>"
```

**Step 3: Verify color palette is now distinct**

After the change, the full palette is:
| Cell type       | Color          | Hex       |
|-----------------|----------------|-----------|
| Wall            | Dark blue-gray | `#34495e` |
| StartPoint      | Orange         | `#f39c12` |
| EndPoint        | Red            | `#e74c3c` |
| Path            | Green          | `#2ecc71` |
| Visited         | Light gray     | `#bdc3c7` |
| BFS overlay     | Blue           | `#3498db` |
| Purple overlay  | Purple         | `#9b59b6` |
| Yellow overlay  | Yellow         | `#f1c40f` |
| Best path       | Green          | `#2ecc71` |

No two active states share the same color.

**Step 4: Commit**
```bash
git add mazewidget.cpp widget.cpp
git commit -m "fix: change StartPoint color to orange to avoid BFS overlay collision"
```

---

### Task 2: Make A* respect m_movementType

**Problem:** `executeNextStepAStar` always iterates `GridUtils::DIRECTIONS_8`, ignoring `m_movementType`. Selecting "4方向移动" in the UI has no effect on A*.

**Files:**
- Modify: `core/algorithms.cpp` — add movement-type branch in `executeNextStepAStar`

**Step 1: Replace the hardcoded DIRECTIONS_8 loop**

In `algorithms.cpp`, inside `executeNextStepAStar()`, find the neighbor loop (around line 84):
```cpp
for (const auto& dir : GridUtils::DIRECTIONS_8) {
    int nx = current->x + dir.dy;
    int ny = current->y + dir.dx;

    if (!GridUtils::isInBounds(nx, ny, *m_allPoints)) {
        continue;
    }

    auto neighbor = (*m_allPoints)[ny][nx];
    if (neighbor->type == AType::Wall) continue;

    // 对角线移动检测：确保不会"切角"穿过墙体
    if (dir.isDiagonal) {
        int midX1 = current->x + dir.dy;
        int midY1 = current->y;
        int midX2 = current->x;
        int midY2 = current->y + dir.dx;

        bool path1Clear = GridUtils::isInBounds(midX1, midY1, *m_allPoints) &&
                         (*m_allPoints)[midY1][midX1]->type != AType::Wall;
        bool path2Clear = GridUtils::isInBounds(midX2, midY2, *m_allPoints) &&
                         (*m_allPoints)[midY2][midX2]->type != AType::Wall;

        if (!path1Clear && !path2Clear) {
            continue;
        }
    }

    std::pair<int,int> neighborPos = {ny, nx};
    if (m_visitedSet.count(neighborPos) > 0) continue;

    double moveCost = dir.cost;
    double tentativeG = current->g_cost + moveCost;

    if (tentativeG < neighbor->g_cost) {
        neighbor->parent = current;
        neighbor->g_cost = tentativeG;
        neighbor->h_cost = heuristicCost(neighbor.get(), m_endPoint.get());
        neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
        m_openSetAStar.push(neighbor);
    }
}
```

Replace with:
```cpp
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
```

**Step 2: Commit**
```bash
git add core/algorithms.cpp
git commit -m "fix: make A* respect m_movementType (4 vs 8 directions)"
```

---

### Summary of changes

| File | Change |
|------|--------|
| `mazewidget.cpp` | StartPoint: `QColor(52,152,219)` → `QColor(243,156,18)` |
| `widget.cpp` | Tips dialog: blue → orange label for 起点 |
| `core/algorithms.cpp` | `executeNextStepAStar`: hardcoded `DIRECTIONS_8` → conditional on `m_movementType` |
