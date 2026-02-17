#ifndef FILESERVICE_H
#define FILESERVICE_H

#include <QString>
#include <QVector>
#include <QPoint>

class FileService {
public:
    static bool saveMap(const QString& filePath, const QVector<QVector<int>>& map);
    static bool loadMap(const QString& filePath, QVector<QVector<int>>& map);
    static QString getDefaultMapPath();
};

#endif // FILESERVICE_H
