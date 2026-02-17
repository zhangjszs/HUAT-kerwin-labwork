#include "fileservice.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>

QString FileService::getDefaultMapPath() {
    return QCoreApplication::applicationDirPath() + "/map.txt";
}

bool FileService::saveMap(const QString& filePath, const QVector<QVector<int>>& map) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    for (const auto& row : map) {
        for (int val : row) {
            out << val << " ";
        }
        out << "\n";
    }
    file.close();
    return true;
}

bool FileService::loadMap(const QString& filePath, QVector<QVector<int>>& map) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    map.clear();
    QTextStream in(&file);
    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(" ", Qt::SkipEmptyParts);
        if (values.isEmpty()) continue;

        map.append(QVector<int>());
        for (const QString& val : values) {
            bool ok;
            map[row].append(val.toInt(&ok));
        }
        row++;
    }
    file.close();
    return true;
}
