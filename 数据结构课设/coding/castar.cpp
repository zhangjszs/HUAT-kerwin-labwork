//  上下左右走一格花销为10，斜着走一格花销为14，以方便计算
//  即格子宽高为10 对角线为14

#include "castar.h"

double CAstar::heuristicCost(const APoint* a, const APoint* b) {
    return abs(a->x - b->x) + abs(a->y - b->y);
}

void CAstar::setPointType(APoint* point, AType type) {
    if (point->type == AType::StartPoint || point->type == AType::EndPoint) {
        return; // Don't override start and end points
    }
    point->type = type;
}

APoint* CAstar::findWay(APoint* beginPoint, APoint* endPoint, vector<vector<APoint*>>& allPoints) {
    vector<APoint*> openSet;
    vector<APoint*> closedSet;
    openSet.push_back(beginPoint);

    while (!openSet.empty()) {
        APoint* current = openSet[0];
        for (auto point : openSet) {
            if (point->f_cost < current->f_cost) {
                current = point;
            }
        }
        if (*current == *endPoint) {
            APoint* pathEnd = current;
            // 回溯路径并标记
            while (pathEnd != nullptr) {
                // 恢复起点和终点的原始类型
                beginPoint->type = AType::StartPoint;
                endPoint->type = AType::EndPoint;
                setPointType(pathEnd, AType::Path);
                pathEnd = pathEnd->parent;
            }
            emit visualizationUpdated();
            return current; // 返回路径的起点
        }

        openSet.erase(remove(openSet.begin(), openSet.end(), current), openSet.end());
        closedSet.push_back(current);

        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                int neighborX = current->x + i;
                int neighborY = current->y + j;

                if (neighborX >= 0 && neighborX < allPoints.size() && neighborY >= 0 && neighborY < allPoints[0].size()) {
                    APoint* neighbor = allPoints[neighborX][neighborY];
                    double cost = 1.0; // 默认移动代价为1

                    if ((i == 0 && abs(j) == 1) || (abs(i) == 1 && j == 0)) {
                        // 如果是上下左右移动，则移动代价为1
                        cost = 1.0;
                    } else if (abs(i) == 1 && abs(j) == 1) {
                        // 如果是对角线移动，则移动代价为1.4
                        cost = 14;
                    }

                    if (neighbor->type != AType::Wall && find(closedSet.begin(), closedSet.end(), neighbor) == closedSet.end()) {
                        double tentative_g_cost = current->g_cost + cost;

                        if (find(openSet.begin(), openSet.end(), neighbor) == openSet.end()) {
                            openSet.push_back(neighbor);
                        } else if (tentative_g_cost >= neighbor->g_cost) {
                            continue;
                        }

                        neighbor->parent = current;
                        neighbor->g_cost = tentative_g_cost;
                        neighbor->h_cost = heuristicCost(neighbor, endPoint);
                        neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;

                        // Set neighbor's type to Visited
                        setPointType(neighbor, AType::Visited);
                    }
                }
            }
        }
    }

    return nullptr; // 如果找不到路径，则返回空指针
}




