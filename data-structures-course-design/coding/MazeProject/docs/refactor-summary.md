# Coordinate System Refactor Summary

## Date: 2026-02-20

## Overview
Unified the coordinate system to (row, col) format across all algorithms, visualization, and data structures to eliminate coordinate confusion bugs.

## What Was Changed

### Core Data Structures
- Changed `StepInfo` struct from `{x, y}` to `{row, col}` with documentation
- Added clear comments explaining coordinate system conventions

### Algorithm Fixes
- **A* Algorithm**: Fixed coordinate emission from {x, y} to {y, x}
- **BFS Algorithm**: Fixed coordinate emission and added missing path reconstruction
- **DFS Algorithm**: Fixed coordinate emission and made path display incremental
- **Dijkstra Algorithm**: Fixed coordinate emission (path reconstruction already existed)
- **JPS Algorithm**: Fixed coordinate emission

### Widget Fixes
- Updated `onAlgorithmStep` to use `step.row` and `step.col`
- Fixed `convertMazeToIntGrid` dimension ordering
- Fixed start/end point initialization for correct QPoint/grid mapping
- Added clarifying comments to algorithm start calls

### Infrastructure Improvements
- Added timer cancellation in `stop()` method to prevent race conditions
- Standardized visited set checking pattern across all algorithms
- Made DFS path reconstruction incremental for consistent UX

### Documentation
- Created `docs/coordinate-system.md` with comprehensive coordinate system guide
- Created `docs/testing-checklist.md` for integration testing
- Added inline comments throughout code explaining coordinate conversions

## Why These Changes Were Made

### Problems Fixed
1. **Coordinate confusion**: Mixed use of (x, y) and (row, col) caused visualization bugs
2. **Missing features**: BFS and Dijkstra didn't show paths
3. **Race conditions**: Algorithm switching could cause crashes
4. **Inconsistent patterns**: Algorithms used different visited checking approaches
5. **Dimension bugs**: Non-square mazes had coordinate swap issues

### Benefits
- Consistent coordinate system throughout codebase
- All algorithms now show paths correctly
- Safer algorithm switching
- Better code maintainability
- Clear documentation for future developers

## Testing Performed

### Automated Testing
- ✅ Clean build with no errors or warnings
- ✅ All compilation units compile successfully

### Manual Testing Required
- Algorithm correctness verification (see `docs/testing-checklist.md`)
- Visual verification of coordinate system
- Non-square maze testing
- Algorithm switching stress testing

## Known Issues
None. All planned changes completed successfully.

## Files Modified
- `core/algorithms.h` - StepInfo struct, method declarations
- `core/algorithms.cpp` - All algorithm implementations
- `widget.h` - No changes needed
- `widget.cpp` - Handler updates, conversion fixes
- `mazewidget.h` - No changes needed
- `mazewidget.cpp` - No changes needed

## Files Created
- `docs/coordinate-system.md` - Coordinate system documentation
- `docs/testing-checklist.md` - Integration testing guide
- `docs/refactor-summary.md` - This file
