#include "widget.h"
#include "./ui_widget.h"
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>

CompareDialog::CompareDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("算法性能比较");
    setMinimumSize(500, 400);
    setStyleSheet("QDialog { background-color: #2c3e50; color: #ecf0f1; }");
}

void CompareDialog::setResults(const QVector<AlgorithmResult>& results)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    QLabel* title = new QLabel("<h2>📊 算法性能比较</h2>", this);
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QStringList headers = {"算法", "访问节点", "路径长度", "执行时间(ms)"};
    QStringList algorithms = {"A*", "BFS", "DFS"};
    QVector<QStringList> data;

    for (const auto& r : results) {
        QStringList row;
        row.append(algorithms[int(r.type)]);
        row.append(QString::number(r.visitedCount));
        row.append(QString::number(r.pathLength));
        row.append(QString::number(r.executionTime, 'f', 2));
        data.append(row);
    }

    QString html = "<table border='1' cellpadding='10' style='border-collapse:collapse; width:100%; background:#34495e; color:#ecf0f1;'>";
    html += "<tr style='background:#2980b9; color:white; font-weight:bold;'><th>" + headers.join("</th><th>") + "</th></tr>";
    
    QStringList colors = {"#3d566e", "#465c71", "#4b6577"};
    for (int i = 0; i < data.size(); i++) {
        html += "<tr style='background:" + colors[i % colors.size()] + "; color:#ecf0f1;'>";
        html += "<td style='font-weight:bold;'>" + data[i].join("</td><td>") + "</td></tr>";
    }
    html += "</table>";

    QLabel* table = new QLabel(html, this);
    layout->addWidget(table);

    QPushButton* closeBtn = new QPushButton("关闭", this);
    closeBtn->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; border: none; padding: 10px 20px; border-radius: 5px; font-size: 12pt; } QPushButton:hover { background-color: #c0392b; }");
    layout->addWidget(closeBtn, 0, Qt::AlignCenter);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}

Widget::Widget(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    setMinimumSize(800, 550);
    setWindowTitle("迷宫寻路系统 - HUAT 数据结构课设");

    statusBar()->showMessage("就绪 | 左键画墙，右键设起点/终点 | 快捷键: R清理 1/2/3算法");

    Mainmap = initializeMaze(MAX_X, MAX_Y);

    aStarButton = ui->aStarButton;
    bfsButton = ui->bfsButton;
    dfsButton = ui->dfsButton;
    clearButton = ui->clearButton;
    loadButton = ui->loadButton;
    saveButton = ui->saveButton;
    tipsButton = ui->tipsButton;
    themeButton = ui->themeButton;
    sizeButton = ui->sizeButton;
    sizeSpinBox = ui->sizeSpinBox;
    speedSlider = ui->speedSlider;
    densitySlider = ui->densitySlider;
    generateButton = ui->generateButton;
    resetPathButton = ui->resetPathButton;
    exportButton = ui->exportButton;
    compareButton = ui->compareButton;
    textEdit = ui->textEdit;
    mazeWidget = ui->mazeWidget;

    m_algorithmRunner = new AlgorithmRunner(this);
    m_algorithmRunner->setSpeed(201 - speedSlider->value());

    mazeWidget->setGridData(&Mainmap);
    mazeWidget->setStartPoint(&startPoint);
    mazeWidget->setEndPoint(&endPoint);

    textEdit->setPlaceholderText("等待你的选择...");

    setupConnections();
    applyTheme("light");
}

void Widget::setupConnections()
{
    connect(aStarButton, &QPushButton::clicked, this, &Widget::onAStarClicked);
    connect(bfsButton, &QPushButton::clicked, this, &Widget::onBfsClicked);
    connect(dfsButton, &QPushButton::clicked, this, &Widget::onDfsClicked);
    connect(clearButton, &QPushButton::clicked, this, &Widget::clearMap);
    connect(loadButton, &QPushButton::clicked, this, &Widget::loadMap);
    connect(saveButton, &QPushButton::clicked, this, &Widget::saveMap);
    connect(tipsButton, &QPushButton::clicked, this, &Widget::showTipsDialog);
    connect(themeButton, &QPushButton::clicked, this, &Widget::toggleTheme);
    connect(sizeButton, &QPushButton::clicked, this, &Widget::onSizeButtonClicked);
    connect(speedSlider, &QSlider::valueChanged, this, &Widget::onSpeedChanged);
    connect(densitySlider, &QSlider::valueChanged, this, &Widget::onDensityChanged);
    connect(generateButton, &QPushButton::clicked, this, &Widget::onGenerateClicked);
    connect(resetPathButton, &QPushButton::clicked, this, &Widget::onResetPathClicked);
    connect(exportButton, &QPushButton::clicked, this, &Widget::onExportClicked);
    connect(compareButton, &QPushButton::clicked, this, &Widget::onCompareClicked);
    connect(this, &Widget::updateTextEdit, this, &Widget::handleUpdateTextEdit);

    connect(m_algorithmRunner, &AlgorithmRunner::stepExecuted, this, &Widget::onAlgorithmStep);
    connect(m_algorithmRunner, &AlgorithmRunner::searchFinished, this, &Widget::onAlgorithmFinished);

    connect(mazeWidget, &MazeWidget::clicked, this, &Widget::onMazeClicked);
    connect(mazeWidget, &MazeWidget::dragged, this, &Widget::onMazeDragged);
}

Widget::~Widget()
{
    delete ui;
}

Grid2D Widget::initializeMaze(int rows, int cols) {
    Grid2D maze(rows, std::vector<APointPtr>(cols));

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            auto pt = std::make_shared<APoint>();
            pt->type = AType::Unknow;
            pt->x = col;
            pt->y = row;
            maze[row][col] = pt;
        }
    }
    return maze;
}

void Widget::clearAlgorithmOverlay()
{
    for (int row = 0; row < static_cast<int>(Mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(Mainmap[row].size()); col++) {
            Mainmap[row][col]->algorithmPath = -1;
            Mainmap[row][col]->isBestPath = false;
            Mainmap[row][col]->pathColor = QColor();
        }
    }
}

void Widget::onMazeClicked(int i, int j, Qt::MouseButton button)
{
    if (button == Qt::LeftButton) {
        if (QPoint(j, i) == startPoint || QPoint(j, i) == endPoint) {
            statusBar()->showMessage("起点/终点不能设为墙");
            return;
        }
        clearAlgorithmOverlay();
        Mainmap[i][j]->type = (Mainmap[i][j]->type == AType::Wall) ? AType::Unknow : AType::Wall;
        statusBar()->showMessage(QString("左键点击: (%1, %2)").arg(j).arg(i));
    } else if (button == Qt::RightButton) {
        clearAlgorithmOverlay();
        auto normalizeRestoreType = [](AType type) {
            return (type == AType::StartPoint || type == AType::EndPoint) ? AType::Unknow : type;
        };

        if (startPoint == QPoint(-1, -1) && endPoint == QPoint(-1, -1)) {
            startPoint = QPoint(j, i);
            m_startPrevType = normalizeRestoreType(Mainmap[i][j]->type);
            Mainmap[i][j]->type = AType::StartPoint;
            statusBar()->showMessage("起点已设置");
        } else if (startPoint != QPoint(-1, -1) && endPoint == QPoint(-1, -1)) {
            if (QPoint(j, i) == startPoint) {
                statusBar()->showMessage("终点不能与起点重叠");
                return;
            }
            endPoint = QPoint(j, i);
            m_endPrevType = normalizeRestoreType(Mainmap[i][j]->type);
            Mainmap[i][j]->type = AType::EndPoint;
            statusBar()->showMessage("终点已设置");
        } else if (startPoint != QPoint(-1, -1) && endPoint != QPoint(-1, -1)) {
            Mainmap[startPoint.y()][startPoint.x()]->type = m_startPrevType;
            Mainmap[endPoint.y()][endPoint.x()]->type = m_endPrevType;
            endPoint = QPoint(-1, -1);
            startPoint = QPoint(-1, -1);
            m_startPrevType = AType::Unknow;
            m_endPrevType = AType::Unknow;
            startPoint = QPoint(j, i);
            m_startPrevType = normalizeRestoreType(Mainmap[i][j]->type);
            Mainmap[i][j]->type = AType::StartPoint;
            statusBar()->showMessage("起点已重置");
        }
    }
    mazeWidget->refresh();
}

void Widget::onMazeDragged(int i, int j)
{
    if (QPoint(j, i) == startPoint || QPoint(j, i) == endPoint) {
        return;
    }
    clearAlgorithmOverlay();
    Mainmap[i][j]->type = AType::Wall;
    mazeWidget->refresh();
}

void Widget::onAStarClicked()
{
    emit updateTextEdit("A* 寻路中...");
    statusBar()->showMessage("A* 算法运行中...");
    if (startPoint == QPoint(-1, -1) || endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请设置起点和终点！");
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    for (int row = 0; row < static_cast<int>(Mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(Mainmap[row].size()); col++) {
            if (Mainmap[row][col]->type == AType::Path || Mainmap[row][col]->type == AType::Visited) {
                Mainmap[row][col]->type = AType::Unknow;
            }
            Mainmap[row][col]->g_cost = 0;
            Mainmap[row][col]->f_cost = 0;
            Mainmap[row][col]->h_cost = 0;
            Mainmap[row][col]->parent = nullptr;
        }
    }
    clearAlgorithmOverlay();

    m_algorithmRunner->startAStar(Mainmap[startPoint.y()][startPoint.x()],
                                   Mainmap[endPoint.y()][endPoint.x()],
                                   Mainmap);
    m_visitedCount = 0;
}

void Widget::onBfsClicked()
{
    emit updateTextEdit("BFS 寻路中...");
    statusBar()->showMessage("BFS 算法运行中...");
    if (startPoint == QPoint(-1, -1) || endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请设置起点和终点！");
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    for (int row = 0; row < static_cast<int>(Mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(Mainmap[row].size()); col++) {
            if (Mainmap[row][col]->type == AType::Path || Mainmap[row][col]->type == AType::Visited) {
                Mainmap[row][col]->type = AType::Unknow;
            }
        }
    }
    clearAlgorithmOverlay();

    std::vector<std::vector<int>> maze(MAX_X, std::vector<int>(MAX_Y));
    for (int row = 0; row < MAX_X; row++) {
        for (int col = 0; col < MAX_Y; col++) {
            if (Mainmap[row][col]->type == AType::Wall) {
                maze[row][col] = 1;
            } else {
                maze[row][col] = 0;
            }
        }
    }

    std::pair<int, int> start = {startPoint.y(), startPoint.x()};
    std::pair<int, int> end = {endPoint.y(), endPoint.x()};

    m_algorithmRunner->startBFS(maze, start, end, Mainmap);
    m_visitedCount = 0;
}

void Widget::onDfsClicked()
{
    emit updateTextEdit("DFS 寻路中...");
    statusBar()->showMessage("DFS 算法运行中...");
    if (startPoint == QPoint(-1, -1) || endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请设置起点和终点！");
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    for (int row = 0; row < static_cast<int>(Mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(Mainmap[row].size()); col++) {
            if (Mainmap[row][col]->type == AType::Path || Mainmap[row][col]->type == AType::Visited) {
                Mainmap[row][col]->type = AType::Unknow;
            }
        }
    }
    clearAlgorithmOverlay();

    std::vector<std::vector<int>> maze(MAX_X, std::vector<int>(MAX_Y));
    for (int row = 0; row < MAX_X; row++) {
        for (int col = 0; col < MAX_Y; col++) {
            if (Mainmap[row][col]->type == AType::Wall) {
                maze[row][col] = 1;
            } else {
                maze[row][col] = 0;
            }
        }
    }

    std::pair<int, int> start = {startPoint.y(), startPoint.x()};
    std::pair<int, int> end = {endPoint.y(), endPoint.x()};

    m_algorithmRunner->startDFS(maze, start, end, Mainmap);
    m_visitedCount = 0;
}

void Widget::clearMap()
{
    m_algorithmRunner->stop();
    emit updateTextEdit("地图已清理");
    statusBar()->showMessage("地图已清理");
    startPoint = QPoint(-1, -1);
    endPoint = QPoint(-1, -1);
    m_startPrevType = AType::Unknow;
    m_endPrevType = AType::Unknow;
    Mainmap = initializeMaze(MAX_X, MAX_Y);
    clearAlgorithmOverlay();
    mazeWidget->refresh();
}

void Widget::loadMap() {
    emit updateTextEdit("加载地图...");
    statusBar()->showMessage("正在加载地图...");
    QString filePath = QCoreApplication::applicationDirPath() + "/map.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit updateTextEdit("无法打开地图文件！");
        statusBar()->showMessage("错误: 无法打开地图文件");
        return;
    }

    Mainmap = initializeMaze(MAX_X, MAX_Y);
    startPoint = QPoint(-1, -1);
    endPoint = QPoint(-1, -1);
    m_startPrevType = AType::Unknow;
    m_endPrevType = AType::Unknow;

    QTextStream in(&file);
    int row = 0;
    while (!in.atEnd() && row < MAX_X) {
        QString line = in.readLine();
        QStringList values = line.split(" ", Qt::SkipEmptyParts);
        for (int col = 0; col < values.size() && col < MAX_Y; col++) {
            bool ok;
            int value = values[col].toInt(&ok);
            if (ok && value == 1) {
                Mainmap[row][col]->type = AType::Wall;
            }
        }
        row++;
    }
    file.close();
    emit updateTextEdit("地图加载成功");
    statusBar()->showMessage("地图加载成功");
    mazeWidget->refresh();
}

void Widget::saveMap() {
    emit updateTextEdit("保存地图...");
    statusBar()->showMessage("正在保存地图...");
    QString filePath = QCoreApplication::applicationDirPath() + "/map.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit updateTextEdit("无法打开文件进行保存！");
        statusBar()->showMessage("错误: 无法保存文件");
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < static_cast<int>(Mainmap.size()); ++i) {
        for (int j = 0; j < static_cast<int>(Mainmap[i].size()); ++j) {
            if (Mainmap[i][j]->type == AType::Wall) {
                out << "1 ";
            } else {
                out << "0 ";
            }
        }
        out << "\n";
    }
    file.close();
    emit updateTextEdit("地图保存成功");
    statusBar()->showMessage("地图保存成功");
}

void Widget::showTipsDialog() {
    QMessageBox::information(this, "使用说明",
                             "<div style='font-size:11pt;'>"
                             "<b>🖱️ 操作说明：</b><br>"
                             "<ul>"
                             "<li>左键单击/拖动：绘制/擦除墙壁</li>"
                             "<li>右键单击：依次设置起点和终点</li>"
                             "</ul>"
                             "<b>🎨 颜色说明：</b><br>"
                             "<ul>"
                             "<li><span style='background-color:#3498db; color:white; padding:2px;'>&nbsp;蓝色&nbsp;</span> 起点</li>"
                             "<li><span style='background-color:#e74c3c; color:white; padding:2px;'>&nbsp;红色&nbsp;</span> 终点</li>"
                             "<li><span style='background-color:#34495e; color:white; padding:2px;'>&nbsp;深灰&nbsp;</span> 墙壁</li>"
                             "<li><span style='background-color:#bdc3c7; padding:2px;'>&nbsp;浅灰&nbsp;</span> 已搜索</li>"
                             "<li><span style='background-color:#2ecc71; color:white; padding:2px;'>&nbsp;绿色&nbsp;</span> 路径</li>"
                             "</ul>"
                             "<b>📧 联系方式：</b> zhangjszs@foxmail.com<br>"
                             "<b>📅 Version 2.1</b> - 独立 MazeWidget"
                             "</div>");
}

void Widget::handleUpdateTextEdit(const QString &text)
{
    textEdit->setText(text);
}

void Widget::applyTheme(const QString &theme) {
    if (theme == "dark") {
        m_isDarkTheme = true;
        mazeWidget->setTheme(true);
        setStyleSheet(R"(
            QMainWindow {
                background-color: #2c3e50;
            }
            QLabel {
                color: #ecf0f1;
            }
            QLabel#titleLabel {
                color: #ecf0f1;
            }
            QTextEdit {
                background-color: #34495e;
                color: #ecf0f1;
                border: 2px solid #7f8c8d;
                border-radius: 6px;
                padding: 6px;
                font-size: 10pt;
            }
            QStatusBar {
                background-color: #1a252f;
                color: #ecf0f1;
            }
        )");
        themeButton->setText("☀️ 亮色主题");
    } else {
        m_isDarkTheme = false;
        mazeWidget->setTheme(false);
        setStyleSheet(R"(
            QMainWindow {
                background-color: #f5f6fa;
            }
            QLabel {
                color: #2c3e50;
            }
            QLabel#titleLabel {
                color: #2c3e50;
            }
            QTextEdit {
                background-color: #f8f9fa;
                color: #2c3e50;
                border: 2px solid #dee2e6;
                border-radius: 6px;
                padding: 6px;
                font-size: 10pt;
            }
            QStatusBar {
                background-color: #3498db;
                color: white;
            }
        )");
        themeButton->setText("🌙 暗色主题");
    }
    mazeWidget->refresh();
}

void Widget::toggleTheme() {
    if (m_isDarkTheme) {
        applyTheme("light");
    } else {
        applyTheme("dark");
    }
}

void Widget::applySize(int size) {
    MAX_X = size;
    MAX_Y = size;
    startPoint = QPoint(-1, -1);
    endPoint = QPoint(-1, -1);
    m_startPrevType = AType::Unknow;
    m_endPrevType = AType::Unknow;
    Mainmap = initializeMaze(MAX_X, MAX_Y);
    clearAlgorithmOverlay();
    mazeWidget->setGridData(&Mainmap);
    mazeWidget->setStartPoint(&startPoint);
    mazeWidget->setEndPoint(&endPoint);
    mazeWidget->updateSize();
    mazeWidget->refresh();
    emit updateTextEdit(QString("迷宫大小已更改为 %1x%2").arg(size).arg(size));
    statusBar()->showMessage(QString("迷宫大小: %1x%2").arg(size).arg(size));
}

void Widget::onSizeButtonClicked() {
    int size = sizeSpinBox->value();
    applySize(size);
}

void Widget::onAlgorithmStep(const StepInfo& step)
{
    if (step.isPath) {
        Mainmap[step.y][step.x]->type = AType::Path;
    } else {
        Mainmap[step.y][step.x]->type = step.type;
    }
    m_visitedCount++;
    statusBar()->showMessage(QString("已访问: %1 个节点").arg(m_visitedCount));
    mazeWidget->refresh();
}

void Widget::onAlgorithmFinished(bool found)
{
    if (found) {
        emit updateTextEdit(QString("找到路径！已访问 %1 个节点").arg(m_visitedCount));
        statusBar()->showMessage(QString("找到路径！已访问 %1 个节点").arg(m_visitedCount));
    } else {
        emit updateTextEdit(QString("未找到路径！已访问 %1 个节点").arg(m_visitedCount));
        statusBar()->showMessage(QString("未找到路径！已访问 %1 个节点").arg(m_visitedCount));
    }
}

void Widget::onSpeedChanged(int value)
{
    int delay = 201 - value;
    m_algorithmRunner->setSpeed(delay);
}

void Widget::onGenerateClicked()
{
    m_algorithmRunner->stop();
    srand(QDateTime::currentMSecsSinceEpoch() % 100000);
    
    double ratio = densitySlider->value() / 100.0;
    MazeGenerator::generateRandomMaze(Mainmap, MAX_X, MAX_Y, ratio);
    
    startPoint = QPoint(0, 0);
    endPoint = QPoint(MAX_Y - 1, MAX_X - 1);
    m_startPrevType = AType::Unknow;
    m_endPrevType = AType::Unknow;
    Mainmap[0][0]->type = AType::StartPoint;
    Mainmap[MAX_X-1][MAX_Y-1]->type = AType::EndPoint;
    clearAlgorithmOverlay();
    
    mazeWidget->refresh();
    emit updateTextEdit(QString("随机迷宫已生成 (障碍率: %1%)").arg(densitySlider->value()));
    statusBar()->showMessage("随机迷宫已生成 | 左键画墙，右键设起点/终点");
}

void Widget::onResetPathClicked()
{
    m_algorithmRunner->stop();
    for (int row = 0; row < static_cast<int>(Mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(Mainmap[row].size()); col++) {
            if (Mainmap[row][col]->type == AType::Path || Mainmap[row][col]->type == AType::Visited) {
                Mainmap[row][col]->type = AType::Unknow;
            }
            Mainmap[row][col]->parent = nullptr;
            Mainmap[row][col]->g_cost = 0;
            Mainmap[row][col]->f_cost = 0;
            Mainmap[row][col]->h_cost = 0;
        }
    }
    clearAlgorithmOverlay();
    mazeWidget->refresh();
    emit updateTextEdit("路径已重置");
    statusBar()->showMessage("路径已重置，可以重新搜索");
}

void Widget::onExportClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "导出图片", 
        QCoreApplication::applicationDirPath() + "/maze.png", 
        "PNG Files (*.png);;JPG Files (*.jpg);;All Files (*)");
    
    if (fileName.isEmpty()) return;
    
    QPixmap pixmap = mazeWidget->grab();
    if (pixmap.save(fileName)) {
        emit updateTextEdit("图片已导出: " + fileName);
        statusBar()->showMessage("导出成功: " + fileName);
    } else {
        emit updateTextEdit("导出失败！");
        statusBar()->showMessage("导出失败");
    }
}

void Widget::onCompareClicked()
{
    if (startPoint == QPoint(-1, -1) || endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请先设置起点和终点！");
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    emit updateTextEdit("正在比较算法...");
    statusBar()->showMessage("算法比较中...");

    for (int row = 0; row < static_cast<int>(Mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(Mainmap[row].size()); col++) {
            if (Mainmap[row][col]->type == AType::Path || Mainmap[row][col]->type == AType::Visited) {
                Mainmap[row][col]->type = AType::Unknow;
            }
        }
    }
    clearAlgorithmOverlay();

    std::vector<std::vector<int>> maze(MAX_X, std::vector<int>(MAX_Y));
    for (int row = 0; row < MAX_X; row++) {
        for (int col = 0; col < MAX_Y; col++) {
            maze[row][col] = (Mainmap[row][col]->type == AType::Wall) ? 1 : 0;
        }
    }
    
    std::pair<int, int> start = {startPoint.y(), startPoint.x()};
    std::pair<int, int> end = {endPoint.y(), endPoint.x()};

    QVector<AlgorithmResult> results;

    MazeSolver solver;

    int visitedBFS = 0;
    int visitedDFS = 0;

    auto startTime = std::chrono::high_resolution_clock::now();
    auto pathBFS = solver.findPathBFS(maze, start, end, &visitedBFS);
    auto endTime = std::chrono::high_resolution_clock::now();
    double bfsTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    
    AlgorithmResult r1;
    r1.type = AlgorithmType::BFS;
    r1.found = !pathBFS.empty();
    r1.pathLength = pathBFS.size();
    r1.visitedCount = visitedBFS;
    r1.executionTime = bfsTime;
    r1.path = pathBFS;
    results.append(r1);

    startTime = std::chrono::high_resolution_clock::now();
    auto pathDFS = solver.findPathDFS(maze, start, end, &visitedDFS);
    endTime = std::chrono::high_resolution_clock::now();
    double dfsTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    
    AlgorithmResult r2;
    r2.type = AlgorithmType::DFS;
    r2.found = !pathDFS.empty();
    r2.pathLength = pathDFS.size();
    r2.visitedCount = visitedDFS;
    r2.executionTime = dfsTime;
    r2.path = pathDFS;
    results.append(r2);

    startTime = std::chrono::high_resolution_clock::now();
    auto r3 = solver.compareAllAlgorithms(maze, start, end);
    endTime = std::chrono::high_resolution_clock::now();
    
    results.append(r3);

    int bestPathLength = INT_MAX;
    int bestIndex = -1;
    for (int i = 0; i < results.size(); i++) {
        if (results[i].found && results[i].pathLength > 0 && results[i].pathLength < bestPathLength) {
            bestPathLength = results[i].pathLength;
            bestIndex = i;
        }
    }

    for (int i = 0; i < results.size(); i++) {
        if (!results[i].found || results[i].path.empty()) continue;
        
        QColor pathColor;
        
        if (i == bestIndex) {
            pathColor = QColor(46, 204, 113);
        } else if (i == 0) {
            pathColor = QColor(52, 152, 219);
        } else if (i == 1) {
            pathColor = QColor(155, 89, 182);
        } else {
            pathColor = QColor(241, 196, 15);
        }
        
        for (size_t j = 0; j < results[i].path.size(); j++) {
            auto& p = results[i].path[j];
            if (Mainmap[p.first][p.second]->type != AType::StartPoint &&
                Mainmap[p.first][p.second]->type != AType::EndPoint) {
                Mainmap[p.first][p.second]->algorithmPath = i;
                Mainmap[p.first][p.second]->pathColor = pathColor;
                Mainmap[p.first][p.second]->isBestPath = (bestIndex >= 0 && i == bestIndex);
            }
        }
    }

    mazeWidget->refresh();
    
    QString resultText;
    if (bestIndex < 0) {
        resultText = "比较完成！无可达路径";
    } else {
        resultText = QString("比较完成！最短路径: %1 (%2)")
            .arg(bestIndex == 0 ? "BFS" : (bestIndex == 1 ? "DFS" : "A*"))
            .arg(bestPathLength);
    }
    emit updateTextEdit(resultText);
    statusBar()->showMessage(resultText);

    CompareDialog dialog(this);
    dialog.setResults(results);
    dialog.exec();
}

void Widget::onDensityChanged(int value)
{
    statusBar()->showMessage(QString("障碍物密度: %1%").arg(value));
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_R:
        clearMap();
        break;
    case Qt::Key_1:
        m_currentAlgorithm = 0;
        onAStarClicked();
        break;
    case Qt::Key_2:
        m_currentAlgorithm = 1;
        onBfsClicked();
        break;
    case Qt::Key_3:
        m_currentAlgorithm = 2;
        onDfsClicked();
        break;
    case Qt::Key_Space:
        if (m_algorithmRunner->isRunning()) {
            m_algorithmRunner->stop();
            emit updateTextEdit("已暂停");
        }
        break;
    case Qt::Key_Plus:
    case Qt::Key_Equal:
        speedSlider->setValue(qMin(speedSlider->value() + 10, 200));
        break;
    case Qt::Key_Minus:
        speedSlider->setValue(qMax(speedSlider->value() - 10, 10));
        break;
    case Qt::Key_G:
        onGenerateClicked();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
}
