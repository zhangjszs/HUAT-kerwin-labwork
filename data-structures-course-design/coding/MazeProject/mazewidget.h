#ifndef MAZEWIDGET_H
#define MAZEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QResizeEvent>
#include "models/mazegrid.h"
#include "constants.h"

class MazeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MazeWidget(QWidget *parent = nullptr);
    void setGridData(Grid2D *grid) { m_grid = grid; }
    void setStartPoint(QPoint *sp) { m_startPoint = sp; }
    void setEndPoint(QPoint *ep) { m_endPoint = ep; }
    void setTheme(bool isDark) { m_isDark = isDark; }
    void refresh() { update(); }
    void updateSize();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

signals:
    void clicked(int i, int j, Qt::MouseButton button);
    void dragged(int i, int j);

private:
    Grid2D *m_grid = nullptr;
    QPoint *m_startPoint = nullptr;
    QPoint *m_endPoint = nullptr;
    bool m_isDark = false;
    int m_cellSize;
    int m_offsetX;
    int m_offsetY;
};

#endif
