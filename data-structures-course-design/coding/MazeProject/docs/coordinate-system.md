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
