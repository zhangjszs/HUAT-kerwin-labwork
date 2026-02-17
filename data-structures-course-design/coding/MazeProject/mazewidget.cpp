#include "mazewidget.h"
#include <QTime>

MazeWidget::MazeWidget(QWidget *parent) : QWidget(parent)
{
    updateSize();
}

void MazeWidget::updateSize()
{
    int cellSize = qMin(width() - 20, height() - 20) / qMax(MAX_X, MAX_Y);
    cellSize = qMax(cellSize, 15);
    m_cellSize = cellSize;
    m_offsetX = (width() - MAX_Y * m_cellSize) / 2;
    m_offsetY = (height() - MAX_X * m_cellSize) / 2;
    setMinimumSize(MAX_Y * m_cellSize + 40, MAX_X * m_cellSize + 40);
}

void MazeWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateSize();
    update();
}

void MazeWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor bgColor = m_isDark ? QColor(60, 60, 60) : QColor(236, 240, 241);
    painter.fillRect(rect(), bgColor);

    if (!m_grid) return;

    for (int i = 0; i < static_cast<int>(m_grid->size()); i++) {
        for (int j = 0; j < static_cast<int>((*m_grid)[i].size()); j++) {
            int x = m_offsetX + j * m_cellSize;
            int y = m_offsetY + i * m_cellSize;

            QRect cellRect(x, y, m_cellSize, m_cellSize);

            AType type = (*m_grid)[i][j]->type;
            
            bool hasOverlay = ((*m_grid)[i][j]->algorithmPath >= 0) && (type == AType::Unknow);
            if (hasOverlay && (*m_grid)[i][j]->isBestPath) {
                painter.fillRect(cellRect, QColor(46, 204, 113));
            } else if (hasOverlay && (*m_grid)[i][j]->algorithmPath == 0) {
                painter.fillRect(cellRect, QColor(52, 152, 219));
            } else if (hasOverlay && (*m_grid)[i][j]->algorithmPath == 1) {
                painter.fillRect(cellRect, QColor(155, 89, 182));
            } else if (hasOverlay && (*m_grid)[i][j]->algorithmPath == 2) {
                painter.fillRect(cellRect, QColor(241, 196, 15));
            } else if (type == AType::Wall) {
                painter.fillRect(cellRect, QColor(52, 73, 94));
            } else if (type == AType::Unknow) {
                painter.fillRect(cellRect, m_isDark ? QColor(80, 80, 80) : QColor(255, 255, 255));
            } else if (type == AType::StartPoint) {
                painter.fillRect(cellRect, QColor(52, 152, 219));
            } else if (type == AType::EndPoint) {
                painter.fillRect(cellRect, QColor(231, 76, 60));
            } else if (type == AType::Path) {
                painter.fillRect(cellRect, QColor(46, 204, 113));
            } else if (type == AType::Visited) {
                painter.fillRect(cellRect, m_isDark ? QColor(127, 140, 141) : QColor(189, 195, 199));
            }

            painter.setPen(QPen(m_isDark ? QColor(100, 100, 100) : QColor(189, 195, 199), 1));
            painter.drawRect(cellRect);
        }
    }
}

void MazeWidget::mousePressEvent(QMouseEvent *event)
{
    int x = event->position().x();
    int y = event->position().y();

    int i = (y - m_offsetY) / m_cellSize;
    int j = (x - m_offsetX) / m_cellSize;

    if (i >= 0 && i < MAX_X && j >= 0 && j < MAX_Y) {
        emit clicked(i, j, event->button());
    }
}

void MazeWidget::mouseMoveEvent(QMouseEvent *event)
{
    static QTime lastUpdateTime = QTime::currentTime();

    int x = event->position().x();
    int y = event->position().y();

    int i = (y - m_offsetY) / m_cellSize;
    int j = (x - m_offsetX) / m_cellSize;

    if (i >= 0 && i < MAX_X && j >= 0 && j < MAX_Y) {
        if (event->buttons() & Qt::LeftButton) {
            emit dragged(i, j);
            if (lastUpdateTime.msecsTo(QTime::currentTime()) > MOUSE_MOVE_THROTTLE_MS) {
                update();
                lastUpdateTime = QTime::currentTime();
            }
        }
    }
}
