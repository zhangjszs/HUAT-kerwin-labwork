#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGlobal>
#include <QColor>

const int DEFAULT_SIZE = 20;

const int startX = 20;
const int startY = 20;

const double MOVE_COST_STRAIGHT = 1.0;
const double MOVE_COST_DIAGONAL = 1.41421356237;

const int MOUSE_MOVE_THROTTLE_MS = 30;

const double INFINITY_COST = 1e18;

const int MAX_MAZE_SIZE = 100;
const int MIN_MAZE_SIZE = 5;

namespace MazeColors {

    namespace Light {
        constexpr auto Background = QColor(236, 240, 241);
        constexpr auto CellUnknown = QColor(250, 250, 250);
        constexpr auto CellBorder = QColor(224, 224, 224);
    }

    namespace Dark {
        constexpr auto Background = QColor(60, 60, 60);
        constexpr auto CellUnknown = QColor(66, 66, 66);
        constexpr auto CellBorder = QColor(80, 80, 80);
    }

    constexpr auto StartPoint = QColor(255, 160, 0);
    constexpr auto EndPoint = QColor(0, 150, 136);
    constexpr auto Wall = QColor(38, 50, 56);
    constexpr auto Visited = QColor(117, 117, 117);
    constexpr auto Path = QColor(25, 118, 210);
    constexpr auto PathCompare0 = QColor(52, 152, 219);
    constexpr auto PathCompare1 = QColor(155, 89, 182);
    constexpr auto PathCompare2 = QColor(241, 196, 15);
}

#endif
