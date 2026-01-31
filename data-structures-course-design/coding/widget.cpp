#include "widget.h"
#include "./ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    Mainmap = initializeMaze(MAX_X, MAX_Y);
    ui->setupUi(this);
    // 设置窗口的固定大小
    setFixedSize(800, 600);
    // 设置窗口标题
    setWindowTitle("HUAT 数据结构课设 课题一 By Kerwin Zhang");
    // 设置窗口图标
    QPixmap pixmap("D:\\coding\\Study_code\\Qt_Project\\MazeProject\\81A5F206980373F0A3F35A7F230_C305AE68_27744.png");
    QIcon icon(pixmap);
    setWindowIcon(icon);
    setMouseTracking(true);

    // 创建并连接按钮
    AStarButton = new QPushButton("A*");
    BFSButton = new QPushButton("BFS");
    DFSButton = new QPushButton("DFS");
    clearButton = new QPushButton("清理地图");
    loadButton = new QPushButton("加载地图");
    saveButton = new QPushButton("保存地图");
    label_tips = new QPushButton("提示");
    textEdit = new QTextEdit(this);

    // 设置按钮的大小
    const QSize buttonSize(120, 40);
    AStarButton->setFixedSize(buttonSize);
    BFSButton->setFixedSize(buttonSize);
    DFSButton->setFixedSize(buttonSize);
    clearButton->setFixedSize(buttonSize);
    loadButton->setFixedSize(buttonSize);
    saveButton->setFixedSize(buttonSize);
    label_tips->setFixedSize(buttonSize);
    textEdit->setPlaceholderText("等待你的选择");
    textEdit->setReadOnly(true);
    textEdit->move(550,50);
    textEdit->resize(180, 300);


    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setAlignment(Qt::AlignTop);

    // 添加按钮和标签到布局
    layout->addWidget(AStarButton);
    layout->addWidget(BFSButton);
    layout->addWidget(DFSButton);
    layout->addWidget(clearButton);
    layout->addWidget(loadButton);
    layout->addWidget(saveButton);
    layout->addWidget(label_tips);

    // 设置布局的边距
    layout->setContentsMargins(400, 50, 0, 0);

    // 设置布局到窗口
    setLayout(layout);

    // 连接按钮点击事件到槽函数
    connect(AStarButton, &QPushButton::clicked, this, &Widget::AStar);
    connect(BFSButton, &QPushButton::clicked, this, &Widget::BFS);
    connect(DFSButton, &QPushButton::clicked, this, &Widget::DFS);
    connect(clearButton, &QPushButton::clicked, this, &Widget::clearMap);
    connect(loadButton, &QPushButton::clicked, this, &Widget::loadMap);
    connect(saveButton, &QPushButton::clicked, this, &Widget::saveMap);
    connect(this, &Widget::updateTextEdit, this, &Widget::handleUpdateTextEdit);

    // 连接label_tips按钮点击事件到槽函数
    connect(label_tips, &QPushButton::clicked, this, &Widget::showTipsDialog);

}

Widget::~Widget()
{
    // for (int i = 0; i < MAX_X; i++) {
    //     for (int j = 0; j < MAX_Y; j++) {
    //         delete Mainmap[i][j];
    //     }
    // }
    delete ui;
}

// 绘制代码...
void Widget::paintEvent(QPaintEvent* event) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < Mainmap.size(); i++) {
        for (int j = 0; j < Mainmap[i].size(); j++) {
            // APoint* point = Mainmap[i][j];
            int x = startX + j * gridSize;
            int y = startY + i * gridSize;
            // point->printAPoint();
            if (Mainmap[i][j]->type == AType::Wall) {
                painter.fillRect(x, y, gridSize, gridSize, Qt::black); // 黑色用于墙壁，以便在浅色背景上有更好的对比度
            } else if (Mainmap[i][j]->type == AType::Unknow) {
                painter.fillRect(x, y, gridSize, gridSize, Qt::white); // 白色用于未知区域，保持低调且易于区分
            } else if (Mainmap[i][j]->type == AType::StartPoint) {
                painter.fillRect(x, y, gridSize, gridSize, Qt::cyan); // 浅蓝色用于起点，清晰且引人注目
            } else if (Mainmap[i][j]->type == AType::EndPoint) {
                painter.fillRect(x, y, gridSize, gridSize, Qt::red); // 红色用于终点，色彩鲜明，易于识别
            } else if (Mainmap[i][j]->type == AType::Path) {
                painter.fillRect(x, y, gridSize, gridSize, Qt::green); // 绿色用于路径，自然且直观
            } else if (Mainmap[i][j]->type == AType::Visited) {
                painter.fillRect(x, y, gridSize, gridSize, Qt::darkGray); // 深灰色用于已访问区域，醒目且不失优雅
            }

        }
    }
    // 绘制网格线
    painter.setPen(Qt::black);

    // 绘制水平线
    for (int i = 0; i <= MAX_X; i++) {
        int y = startY + i * gridSize;
        painter.drawLine(startX, y, startX + MAX_Y * gridSize, y);
    }

    // 绘制垂直线
    for (int j = 0; j <= MAX_Y; j++) {
        int x = startX + j * gridSize;
        painter.drawLine(x, startY, x, startY +MAX_X * gridSize);
    }
}

// 鼠标事件处理代码...
void Widget::mousePressEvent(QMouseEvent* event) {
    int x = event->position().x();
    int y = event->position().y();

    int i = (y - startY) / gridSize;
    int j = (x - startX) / gridSize;

    if (i >= 0 && i < Mainmap.size() && j >= 0 && j < Mainmap[i].size()) {
        if (event->button() == Qt::LeftButton) {
            Mainmap[i][j]->type = (Mainmap[i][j]->type == AType::Wall) ? AType::Unknow : AType::Wall;
        } else if (event->button() == Qt::RightButton) {
            if (startPoint == QPoint(-1, -1) && endPoint == QPoint(-1, -1)) {
                startPoint = QPoint(j, i);
                Mainmap[i][j]->type = AType::StartPoint;
            } else if (startPoint != QPoint(-1, -1) && endPoint == QPoint(-1, -1)) {
                endPoint = QPoint(j, i);
                Mainmap[i][j]->type = AType::EndPoint;
            } else if (startPoint != QPoint(-1, -1) && endPoint != QPoint(-1, -1)) {
                Mainmap[startPoint.y()][startPoint.x()]->type = AType::Unknow;
                Mainmap[endPoint.y()][endPoint.x()]->type = AType::Unknow;
                endPoint = QPoint(-1, -1);
                startPoint = QPoint(-1, -1);
            }
        }
        update();
    }
}

void Widget::mouseMoveEvent(QMouseEvent* event) {
    int x = event->position().x();
    int y = event->position().y();

    int i = (y - startY) / gridSize;
    int j = (x - startX) / gridSize;

    if (i >= 0 && i < Mainmap.size() && j >= 0 && j < Mainmap[i].size()) {
        if (event->buttons() & Qt::LeftButton) {
            Mainmap[i][j]->type = AType::Wall;
            update();
        }
    }
}

std::vector<std::vector<APoint*>> Widget::initializeMaze(int width, int height) {
    std::vector<std::vector<APoint*>> maze(width, std::vector<APoint*>(height));

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            APoint* pt = new APoint();
            pt->type = AType::Unknow;
            pt->x = i;
            pt->y = j;
            maze[i][j] = pt;
        }
    }
    return maze;
}

// A*
void Widget::AStar()
{
    emit updateTextEdit("A*");
    if (startPoint == QPoint(-1, -1) || endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请设置起点和终点！");
        return;
    }
    CAstar astar;
    APoint* pathEnd = astar.findWay(Mainmap[startPoint.y()][startPoint.x()], Mainmap[endPoint.y()][endPoint.x()], Mainmap);
    // 构建路径字符串
    QString pathString = "路径：";
    QVector<APoint*> path;  // 存储路径节点
    while (pathEnd != nullptr) {
        path.push_back(pathEnd);
        pathEnd = pathEnd->parent;
    }
    // 反转路径
    std::reverse(path.begin(), path.end());

    // 将路径添加到字符串中
    for (int i = 0; i < path.size(); ++i) {
        pathString += "(" + QString::number(path[i]->x) + ", " + QString::number(path[i]->y) + ")";
        if (i != path.size() - 1) {
            pathString += " -> ";
        }
    }
    emit updateTextEdit(pathString);
    update();
}

//BFS
void Widget::BFS()
{
    emit updateTextEdit("点击BFS");
    std::vector<std::vector<int>> maze(MAX_X, std::vector<int>(MAX_Y));
    for (int i = 0; i < MAX_X; i++) {
        for (int j = 0; j < MAX_Y; j++) {
                if (Mainmap[i][j]->type == AType::Wall)
            {
                maze[i][j] =1;
            }else{
                maze[i][j] = 0;
            }
        }
    }
    MazeSolver solver;
    std::pair<int, int> start = {startPoint.y(), startPoint.x()};
    std::pair<int, int> end = {endPoint.y(), endPoint.x()};
    std::vector<std::vector<std::pair<int, int>>> paths = solver.findPaths(maze, start, end);
    for (size_t i = 0; i < paths.size(); ++i) {
        std::stringstream ss;
        for (size_t j = 0; j < paths[i].size(); ++j) {
            ss << "(" << paths[i][j].first << ", " << paths[i][j].second << ")";
            if (j < paths[i].size() - 1) {
                ss << " -> ";
            }
        }
        std::cout << "Path " << (i + 1) << ": " << ss.str() << std::endl;
        // QString str = "Path " + QString::number(i + 1) + ": " + QString::fromStdString(ss.str());
        // emit updateTextEdit(str);
    }
}

//DFS
void Widget::DFS()
{
    emit updateTextEdit("点击DFS");
}

// 清理地图按钮点击事件处理函数
void Widget::clearMap() {
    emit updateTextEdit("点击清理地图");
    Mainmap = initializeMaze(MAX_X, MAX_Y);
    update();
}

// 加载地图按钮点击事件处理函数
void Widget::loadMap() {
    emit updateTextEdit("点击加载地图");
    std::string filePath = "D:\\coding\\Study_code\\Qt_Project\\MazeProject\\map.txt";
    std::ifstream file(filePath);
    if (!file.is_open()) {
        emit updateTextEdit("无法打开地图文件进行加载！");
        return;
    }
    Mainmap = initializeMaze(MAX_X, MAX_Y);
    std::string line;
    while (std::getline(file, line)) {
        std::vector<APoint*> row;
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            AType type = (value == 1) ? AType::Wall : AType::Unknow;
            APoint* point = new APoint();
            row.push_back(point);
        }
        Mainmap.push_back(row);
    }
    file.close();
    emit updateTextEdit("地图已成功从" + QString::fromStdString(filePath) + "加载");
    update();
}

// 保存地图按钮点击事件处理函数
void Widget::saveMap() {
    emit updateTextEdit("点击保存地图");
    std::string filePath = "D:\\coding\\Study_code\\Qt_Project\\MazeProject\\map.txt";
    std::ofstream file(filePath);
    if (!file.is_open()) {
        emit updateTextEdit("无法打开文件进行保存！");
        return;
    }
    for (int i = 0; i < Mainmap.size(); ++i) {
        for (int j = 0; j < Mainmap[i].size(); ++j) {
            if (Mainmap[i][j]->type == AType::Wall) {
                file << "1 ";
            } else {
                file << "0 ";
            }
        }
        file << std::endl;
    }
    file.close();
    emit updateTextEdit("地图已成功保存到" + QString::fromStdString(filePath));
}

// 可视化更新槽函数
void Widget::updateVisualizationSlot() {
    emit updateTextEdit("可视化更新");
    repaint();
}


void Widget::showTipsDialog() {
    QMessageBox::information(this, "使用说明",
                             "<b>右键单击</b>格子选定<b>起始点</b>，<b>左键单击</b>格子选定<b>墙壁</b>或<b>删除墙壁</b>。"
                             "<br><br>"
                             "<b>颜色说明：</b>"
                             "<ul>"
                             "  <li>蓝色：起始点</li>"
                             "  <li>红色：终点</li>"
                             "  <li>黑色：墙壁</li>"
                             "  <li>灰色：已搜索过的（closed 列表）</li>"
                             "  <li>绿色：当前搜索到的路径</li>"
                             "</ul>"
                             "<br>"
                             "<b>联系方式：</b>"
                             "zhangjszs@foxmail.com"
                             "<br>"
                             "<b>Version 1.0</b>"
                             );
}

void Widget::handleUpdateTextEdit(const QString &text)
{
    textEdit->setText(text);
}
