#include "mazewidget.h"
#include "constants.h"
#include <QDateTime>

MazeWidget::MazeWidget(QWidget *parent) : QWidget(parent)
{
    m_lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
    updateSize();
}

void MazeWidget::updateSize()
{
    int cellSize = qMin(width() - 20, height() - 20) / qMax(m_mazeWidth, m_mazeHeight);
    cellSize = qMax(cellSize, 15);
    m_cellSize = cellSize;
    m_offsetX = (width() - m_mazeWidth * m_cellSize) / 2;
    m_offsetY = (height() - m_mazeHeight * m_cellSize) / 2;
    setMinimumSize(m_mazeWidth * m_cellSize + 40, m_mazeHeight * m_cellSize + 40);
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

    QColor bgColor = m_isDark ? MazeColors::Dark::Background : MazeColors::Light::Background;
    painter.fillRect(rect(), bgColor);

    if (!m_grid) return;

    for (int i = 0; i < static_cast<int>(m_grid->size()); i++) {
        for (int j = 0; j < static_cast<int>((*m_grid)[i].size()); j++) {
            int x = m_offsetX + j * m_cellSize;
            int y = m_offsetY + i * m_cellSize;

            QRect cellRect(x, y, m_cellSize, m_cellSize);

            AType type = (*m_grid)[i][j]->type;
            
            QColor fillColor;
            
            if (type == AType::Wall) {
                fillColor = MazeColors::Wall;
            } else if ((*m_grid)[i][j]->algorithmPath >= 0 && (*m_grid)[i][j]->isBestPath) {
                fillColor = MazeColors::Path;
            } else if ((*m_grid)[i][j]->algorithmPath == 0) {
                fillColor = MazeColors::PathCompare0;
            } else if ((*m_grid)[i][j]->algorithmPath == 1) {
                fillColor = MazeColors::PathCompare1;
            } else if ((*m_grid)[i][j]->algorithmPath == 2) {
                fillColor = MazeColors::PathCompare2;
            } else if (type == AType::StartPoint) {
                fillColor = MazeColors::StartPoint;
            } else if (type == AType::EndPoint) {
                fillColor = MazeColors::EndPoint;
            } else if (type == AType::Path) {
                fillColor = MazeColors::Path;
            } else if (type == AType::Visited) {
                fillColor = MazeColors::Visited;
            } else if (type == AType::Unknown) {
                fillColor = m_isDark ? MazeColors::Dark::CellUnknown : MazeColors::Light::CellUnknown;
            }

            painter.fillRect(cellRect, fillColor);

            QColor borderColor = m_isDark ? MazeColors::Dark::CellBorder : MazeColors::Light::CellBorder;
            painter.setPen(QPen(borderColor, 1));
            painter.drawRect(cellRect);
        }
    }
}

void MazeWidget::mousePressEvent(QMouseEvent *event)
{
    int x = event->position().x();
    int y = event->position().y();

    // i = 行索引, j = 列索引
    int i = (y - m_offsetY) / m_cellSize;
    int j = (x - m_offsetX) / m_cellSize;

    // 边界检查：i是行(应检查height)，j是列(应检查width)
    if (i >= 0 && i < m_mazeHeight && j >= 0 && j < m_mazeWidth) {
        emit clicked(i, j, event->button());
    }
}

void MazeWidget::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->position().x();
    int y = event->position().y();

    int i = (y - m_offsetY) / m_cellSize;
    int j = (x - m_offsetX) / m_cellSize;

    if (i >= 0 && i < m_mazeHeight && j >= 0 && j < m_mazeWidth) {
        if (event->buttons() & Qt::LeftButton) {
            emit dragged(i, j);
            qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
            if (currentTime - m_lastUpdateTime > MOUSE_MOVE_THROTTLE_MS) {
                update();
                m_lastUpdateTime = currentTime;
            }
        }
    }
}
