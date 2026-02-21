#include "fileservice.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

QString FileService::getDefaultMapPath() {
    return QCoreApplication::applicationDirPath() + "/map.txt";
}

bool FileService::saveMap(const QString& filePath, const QVector<QVector<int>>& map) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << filePath << "Error:" << file.errorString();
        return false;
    }

    QTextStream out(&file);
    for (const auto& row : map) {
        for (int val : row) {
            out << val << " ";
        }
        out << "\n";
        if (out.status() != QTextStream::Ok) {
            qWarning() << "Write error occurred while saving map";
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}

bool FileService::loadMap(const QString& filePath, QVector<QVector<int>>& map) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for reading:" << filePath << "Error:" << file.errorString();
        return false;
    }

    map.clear();
    QTextStream in(&file);
    int row = 0;
    int expectedCols = -1;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(" ", Qt::SkipEmptyParts);
        if (values.isEmpty()) continue;

        // 验证每行列数一致
        if (expectedCols == -1) {
            expectedCols = values.size();
        } else if (values.size() != expectedCols) {
            qWarning() << "Inconsistent column count at row" << row
                       << "expected" << expectedCols << "but got" << values.size();
            map.clear();
            file.close();
            return false;
        }

        map.append(QVector<int>());
        for (const QString& val : values) {
            bool ok;
            int intVal = val.toInt(&ok);
            if (!ok || (intVal != 0 && intVal != 1)) {
                qWarning() << "Invalid value at row" << row << ":" << val;
                map.clear();
                file.close();
                return false;
            }
            map[row].append(intVal);
        }
        row++;
    }
    file.close();

    // 验证迷宫至少有一行
    if (map.isEmpty()) {
        qWarning() << "Map file is empty or contains no valid data";
        return false;
    }

    return true;
}
