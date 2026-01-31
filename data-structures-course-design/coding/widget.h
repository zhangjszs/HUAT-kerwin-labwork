#ifndef WIDGET_H
#define WIDGET_H

#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#include <QIcon>
#include <qstring.h>
#include <QPixmap>
#include <QComboBox>
#include <QVBoxLayout>
#include <QApplication>
#include <QComboBox>
#include <QMessageBox>
#include <QTextEdit>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <iostream>
#include "constants.h"
#include "castar.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    vector<vector<APoint*>> Mainmap;
    // vector<vector<APoint*>> AStarmap;
    // vector<vector<APoint*>> DFSmap;
    // vector<vector<APoint*>> BFSmap;
    QPoint startPoint = QPoint(-1, -1); //初始 起点位置
    QPoint endPoint = QPoint(-1, -1); // 初始 终点坐标

private:
    Ui::Widget *ui;
    QPushButton* AStarButton;
    QPushButton* DFSButton;
    QPushButton* BFSButton;
    QPushButton* clearButton;
    QPushButton* loadButton;
    QPushButton* saveButton;
    QPushButton* label_tips;
    QTextEdit * textEdit;
    QTimer* updateTimer; // 添加 updateTimer 成员变量
    bool isLeftButtonPressed = false; // 左键是否被按下
    // 初始化迷宫
    std::vector<std::vector<APoint*>> initializeMaze(int width, int height);
    // std::vector<std::vector<std::unique_ptr<APoint>>> initializeMaze(int width, int height);
signals:
    void updateTextEdit(const QString &text);

public:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

public slots:
    void updateVisualizationSlot();
    void handleUpdateTextEdit(const QString &text);
    void AStar();
    void BFS();
    void DFS();
    void clearMap();
    void loadMap();
    void saveMap();
    void showTipsDialog();

};
#endif // WIDGET_H
