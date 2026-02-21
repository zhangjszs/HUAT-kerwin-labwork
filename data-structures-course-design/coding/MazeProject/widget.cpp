#include "widget.h"
#include "./ui_widget.h"
#include "constants.h"
#include <QPainter>
#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>

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
    QStringList algorithms = {"A*", "BFS", "DFS", "Dijkstra", "JPS"};
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

    QStringList colors = {"#3d566e", "#465c71", "#4b6577", "#526b7f"};
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

    statusBar()->showMessage("就绪 | 左键画墙，右键设起点/终点 | 快捷�? R清理 1/2/3算法");

    m_mainmap = initializeMaze(m_mazeWidth, m_mazeHeight);

    aStarButton = ui->aStarButton;
    bfsButton = ui->bfsButton;
    dfsButton = ui->dfsButton;

    // 创建Dijkstra和JPS按钮（暂时不添加到UI，需要在UI文件中定义）
    dijkstraButton = new QPushButton("🔸 Dijkstra", this);
    dijkstraButton->setMinimumSize(0, 38);
    dijkstraButton->setStyleSheet(
        "QPushButton {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #e67e22, stop:1 #d35400);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-size: 11pt;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f39c12, stop:1 #e67e22);"
        "}"
    );

    jpsButton = new QPushButton("�?JPS", this);
    jpsButton->setMinimumSize(0, 38);
    jpsButton->setStyleSheet(
        "QPushButton {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #16a085, stop:1 #138d75);"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-size: 11pt;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1abc9c, stop:1 #16a085);"
        "}"
    );

    // 将按钮添加到右侧布局
    QVBoxLayout* rightLayout = qobject_cast<QVBoxLayout*>(ui->verticalLayout_2);
    if (rightLayout) {
        // 在DFS按钮后插�?        int dfsIndex = rightLayout->indexOf(dfsButton);
        if (dfsIndex >= 0) {
            rightLayout->insertWidget(dfsIndex + 1, dijkstraButton);
            rightLayout->insertWidget(dfsIndex + 2, jpsButton);
        }
    }

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

    movementComboBox = new QComboBox(this);
    movementComboBox->addItem("4方向移动", static_cast<int>(MovementType::FourDirections));
    movementComboBox->addItem("8方向移动", static_cast<int>(MovementType::EightDirections));
    movementComboBox->setCurrentIndex(0);

    heuristicComboBox = new QComboBox(this);
    heuristicComboBox->addItem("曼哈顿距�?, static_cast<int>(HeuristicType::Manhattan));
    heuristicComboBox->addItem("欧几里得距离", static_cast<int>(HeuristicType::Euclidean));
    heuristicComboBox->addItem("切比雪夫距离", static_cast<int>(HeuristicType::Chebyshev));
    heuristicComboBox->addItem("八方向距�?, static_cast<int>(HeuristicType::Octile));
    heuristicComboBox->setCurrentIndex(1);

    // 将下拉框添加到右侧布局
    if (rightLayout) {
        QHBoxLayout* movementLayout = new QHBoxLayout();
        movementLayout->addWidget(new QLabel("移动方式:", this));
        movementLayout->addWidget(movementComboBox);
        rightLayout->addLayout(movementLayout);

        QHBoxLayout* heuristicLayout = new QHBoxLayout();
        heuristicLayout->addWidget(new QLabel("启发�?", this));
        heuristicLayout->addWidget(heuristicComboBox);
        rightLayout->addLayout(heuristicLayout);
    }

    m_algorithmRunner = new AlgorithmRunner(this);
    m_algorithmRunner->setSpeed(201 - speedSlider->value());

    mazeWidget->setMazeSize(m_mazeWidth, m_mazeHeight);
    mazeWidget->setGridData(&m_mainmap);
    mazeWidget->setStartPoint(&m_startPoint);
    mazeWidget->setEndPoint(&m_endPoint);

    textEdit->setPlaceholderText("等待你的选择...");

    setupConnections();
    applyTheme("light");
}

void Widget::setupConnections()
{
    connect(aStarButton, &QPushButton::clicked, this, &Widget::onAStarClicked);
    connect(bfsButton, &QPushButton::clicked, this, &Widget::onBfsClicked);
    connect(dfsButton, &QPushButton::clicked, this, &Widget::onDfsClicked);
    connect(dijkstraButton, &QPushButton::clicked, this, &Widget::onDijkstraClicked);
    connect(jpsButton, &QPushButton::clicked, this, &Widget::onJPSClicked);
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
    connect(movementComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::onMovementTypeChanged);
    connect(heuristicComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Widget::onHeuristicTypeChanged);
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
            pt->type = AType::Unknown;
            pt->x = col;
            pt->y = row;
            maze[row][col] = pt;
        }
    }
    return maze;
}

void Widget::clearAlgorithmOverlay()
{
    for (int row = 0; row < static_cast<int>(m_mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(m_mainmap[row].size()); col++) {
            m_mainmap[row][col]->algorithmPath = -1;
            m_mainmap[row][col]->isBestPath = false;
            m_mainmap[row][col]->pathColor = QColor();
        }
    }
}

void Widget::onMazeClicked(int i, int j, Qt::MouseButton button)
{
    if (button == Qt::LeftButton) {
        if (QPoint(j, i) == m_startPoint || QPoint(j, i) == m_endPoint) {
            statusBar()->showMessage("起点/终点不能设为�?);
            return;
        }
        clearAlgorithmOverlay();
        m_mainmap[i][j]->type = (m_mainmap[i][j]->type == AType::Wall) ? AType::Unknown : AType::Wall;
        statusBar()->showMessage(QString("左键点击: (%1, %2)").arg(j).arg(i));
    } else if (button == Qt::RightButton) {
        clearAlgorithmOverlay();
        auto normalizeRestoreType = [](AType type) {
            return (type == AType::StartPoint || type == AType::EndPoint) ? AType::Unknown : type;
        };

        if (m_startPoint == QPoint(-1, -1) && m_endPoint == QPoint(-1, -1)) {
            m_startPoint = QPoint(j, i);
            m_startPrevType = normalizeRestoreType(m_mainmap[i][j]->type);
            m_mainmap[i][j]->type = AType::StartPoint;
            statusBar()->showMessage("起点已设�?);
        } else if (m_startPoint != QPoint(-1, -1) && m_endPoint == QPoint(-1, -1)) {
            if (QPoint(j, i) == m_startPoint) {
                statusBar()->showMessage("终点不能与起点重�?);
                return;
            }
            m_endPoint = QPoint(j, i);
            m_endPrevType = normalizeRestoreType(m_mainmap[i][j]->type);
            m_mainmap[i][j]->type = AType::EndPoint;
            statusBar()->showMessage("终点已设�?);
        } else if (m_startPoint != QPoint(-1, -1) && m_endPoint != QPoint(-1, -1)) {
            m_mainmap[m_startPoint.y()][m_startPoint.x()]->type = m_startPrevType;
            m_mainmap[m_endPoint.y()][m_endPoint.x()]->type = m_endPrevType;
            m_endPoint = QPoint(-1, -1);
            m_startPoint = QPoint(-1, -1);
            m_startPrevType = AType::Unknown;
            m_endPrevType = AType::Unknown;
            m_startPoint = QPoint(j, i);
            m_startPrevType = normalizeRestoreType(m_mainmap[i][j]->type);
            m_mainmap[i][j]->type = AType::StartPoint;
            statusBar()->showMessage("起点已重�?);
        }
    }
    mazeWidget->refresh();
}

void Widget::onMazeDragged(int i, int j)
{
    if (QPoint(j, i) == m_startPoint || QPoint(j, i) == m_endPoint) {
        return;
    }
    clearAlgorithmOverlay();
    m_mainmap[i][j]->type = AType::Wall;
    mazeWidget->refresh();
}

void Widget::onAStarClicked()
{
    emit updateTextEdit("A* 寻路�?..");
    statusBar()->showMessage("A* 算法运行�?..");
    if (m_startPoint == QPoint(-1, -1) || m_endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请设置起点和终点�?);
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    for (int row = 0; row < static_cast<int>(m_mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(m_mainmap[row].size()); col++) {
            if (m_mainmap[row][col]->type == AType::Path || m_mainmap[row][col]->type == AType::Visited) {
                m_mainmap[row][col]->type = AType::Unknown;
            }
            m_mainmap[row][col]->g_cost = 0;
            m_mainmap[row][col]->f_cost = 0;
            m_mainmap[row][col]->h_cost = 0;
            m_mainmap[row][col]->parent = nullptr;
        }
    }
    clearAlgorithmOverlay();

    // QPoint to grid: y()=row, x()=col
    m_algorithmRunner->startAStar(m_mainmap[m_startPoint.y()][m_startPoint.x()],
                                   m_mainmap[m_endPoint.y()][m_endPoint.x()],
                                   m_mainmap);
    m_visitedCount = 0;
}

void Widget::onBfsClicked()
{
    emit updateTextEdit("BFS 寻路�?..");
    statusBar()->showMessage("BFS 算法运行�?..");
    if (m_startPoint == QPoint(-1, -1) || m_endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请设置起点和终点�?);
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    for (int row = 0; row < static_cast<int>(m_mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(m_mainmap[row].size()); col++) {
            if (m_mainmap[row][col]->type == AType::Path || m_mainmap[row][col]->type == AType::Visited) {
                m_mainmap[row][col]->type = AType::Unknown;
            }
        }
    }
    clearAlgorithmOverlay();

    std::vector<std::vector<int>> maze = convertMazeToIntGrid();
    // QPoint to std::pair: {y(), x()} = {row, col}
    std::pair<int, int> start = {m_startPoint.y(), m_startPoint.x()};
    std::pair<int, int> end = {m_endPoint.y(), m_endPoint.x()};

    m_algorithmRunner->startBFS(maze, start, end, m_mainmap);
    m_visitedCount = 0;
}

void Widget::onDfsClicked()
{
    emit updateTextEdit("DFS 寻路�?..");
    statusBar()->showMessage("DFS 算法运行�?..");
    if (m_startPoint == QPoint(-1, -1) || m_endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请设置起点和终点�?);
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    for (int row = 0; row < static_cast<int>(m_mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(m_mainmap[row].size()); col++) {
            if (m_mainmap[row][col]->type == AType::Path || m_mainmap[row][col]->type == AType::Visited) {
                m_mainmap[row][col]->type = AType::Unknown;
            }
        }
    }
    clearAlgorithmOverlay();

    std::vector<std::vector<int>> maze = convertMazeToIntGrid();
    // QPoint to std::pair: {y(), x()} = {row, col}
    std::pair<int, int> start = {m_startPoint.y(), m_startPoint.x()};
    std::pair<int, int> end = {m_endPoint.y(), m_endPoint.x()};

    m_algorithmRunner->startDFS(maze, start, end, m_mainmap);
    m_visitedCount = 0;
}

void Widget::onDijkstraClicked()
{
    emit updateTextEdit("Dijkstra 寻路�?..");
    statusBar()->showMessage("Dijkstra 算法运行�?..");
    if (m_startPoint == QPoint(-1, -1) || m_endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请设置起点和终点�?);
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    for (int row = 0; row < static_cast<int>(m_mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(m_mainmap[row].size()); col++) {
            if (m_mainmap[row][col]->type == AType::Path || m_mainmap[row][col]->type == AType::Visited) {
                m_mainmap[row][col]->type = AType::Unknown;
            }
        }
    }
    clearAlgorithmOverlay();

    std::vector<std::vector<int>> maze = convertMazeToIntGrid();
    // QPoint to std::pair: {y(), x()} = {row, col}
    std::pair<int, int> start = {m_startPoint.y(), m_startPoint.x()};
    std::pair<int, int> end = {m_endPoint.y(), m_endPoint.x()};

    m_algorithmRunner->startDijkstra(maze, start, end, m_mainmap);
    m_visitedCount = 0;
}

void Widget::onJPSClicked()
{
    emit updateTextEdit("JPS 寻路�?..");
    statusBar()->showMessage("JPS 算法运行�?..");
    if (m_startPoint == QPoint(-1, -1) || m_endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请设置起点和终点�?);
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    for (int row = 0; row < static_cast<int>(m_mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(m_mainmap[row].size()); col++) {
            if (m_mainmap[row][col]->type == AType::Path || m_mainmap[row][col]->type == AType::Visited) {
                m_mainmap[row][col]->type = AType::Unknown;
            }
        }
    }
    clearAlgorithmOverlay();

    std::vector<std::vector<int>> maze = convertMazeToIntGrid();
    // QPoint to std::pair: {y(), x()} = {row, col}
    std::pair<int, int> start = {m_startPoint.y(), m_startPoint.x()};
    std::pair<int, int> end = {m_endPoint.y(), m_endPoint.x()};

    m_algorithmRunner->startJPS(maze, start, end, m_mainmap);
    m_visitedCount = 0;
}

void Widget::clearMap()
{
    m_algorithmRunner->stop();
    emit updateTextEdit("地图已清�?);
    statusBar()->showMessage("地图已清�?);
    m_startPoint = QPoint(-1, -1);
    m_endPoint = QPoint(-1, -1);
    m_startPrevType = AType::Unknown;
    m_endPrevType = AType::Unknown;
    m_mainmap = initializeMaze(m_mazeWidth, m_mazeHeight);
    clearAlgorithmOverlay();
    mazeWidget->refresh();
}

std::vector<std::vector<int>> Widget::convertMazeToIntGrid()
{
    // Create maze with correct dimensions: [rows][cols]
    std::vector<std::vector<int>> maze(m_mazeHeight, std::vector<int>(m_mazeWidth));

    for (int row = 0; row < m_mazeHeight; row++) {
        for (int col = 0; col < m_mazeWidth; col++) {
            maze[row][col] = (m_mainmap[row][col]->type == AType::Wall) ? 1 : 0;
        }
    }
    return maze;
}

void Widget::loadMap() {
    emit updateTextEdit("加载地图...");
    statusBar()->showMessage("正在加载地图...");
    QString filePath = QCoreApplication::applicationDirPath() + "/map.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit updateTextEdit("无法打开地图文件�?);
        statusBar()->showMessage("错误: 无法打开地图文件");
        return;
    }

    m_mainmap = initializeMaze(m_mazeWidth, m_mazeHeight);
    m_startPoint = QPoint(-1, -1);
    m_endPoint = QPoint(-1, -1);
    m_startPrevType = AType::Unknown;
    m_endPrevType = AType::Unknown;

    QTextStream in(&file);
    int row = 0;
    while (!in.atEnd() && row < m_mazeHeight) {
        QString line = in.readLine();
        QStringList values = line.split(" ", Qt::SkipEmptyParts);
        for (int col = 0; col < values.size() && col < m_mazeWidth; col++) {
            bool ok;
            int value = values[col].toInt(&ok);
            if (ok && value == 1) {
                m_mainmap[row][col]->type = AType::Wall;
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
        emit updateTextEdit("无法打开文件进行保存�?);
        statusBar()->showMessage("错误: 无法保存文件");
        return;
    }
    QTextStream out(&file);
    for (int i = 0; i < static_cast<int>(m_mainmap.size()); ++i) {
        for (int j = 0; j < static_cast<int>(m_mainmap[i].size()); ++j) {
            if (m_mainmap[i][j]->type == AType::Wall) {
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
                             "<b>🖱�?操作说明�?/b><br>"
                             "<ul>"
                             "<li>左键单击/拖动：绘�?擦除墙壁</li>"
                             "<li>右键单击：依次设置起点和终点</li>"
                             "</ul>"
                             "<b>🎨 颜色说明 (WCAG AA标准)�?/b><br>"
                             "<ul>"
                             "<li><span style='background-color:#FFA000; padding:2px;'>&nbsp;琥珀�?nbsp;</span> 起点 (对比�?3.2:1)</li>"
                             "<li><span style='background-color:#009688; color:white; padding:2px;'>&nbsp;翡翠�?nbsp;</span> 终点 (对比�?4.1:1)</li>"
                             "<li><span style='background-color:#263238; color:white; padding:2px;'>&nbsp;深岩�?nbsp;</span> 墙壁 (对比�?12.5:1)</li>"
                             "<li><span style='background-color:#757575; color:white; padding:2px;'>&nbsp;铅灰�?nbsp;</span> 已搜�?(对比�?4.7:1)</li>"
                             "<li><span style='background-color:#1976D2; color:white; padding:2px;'>&nbsp;宝石�?nbsp;</span> 路径 (对比�?4.5:1)</li>"
                             "<li><span style='background-color:#FAFAFA; border:1px solid #ccc; padding:2px;'>&nbsp;雪白�?nbsp;</span> 可通行区域</li>"
                             "</ul>"
                             "<b>📊 配色符合WCAG AA无障碍标�?/b><br>"
                             "<b>📧 联系方式�?/b> zhangjszs@foxmail.com<br>"
                             "<b>📅 Version 2.3</b> - 专业配色方案"
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
        themeButton->setText("☀�?亮色主题");
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
    // 输入验证
    if (size < MIN_MAZE_SIZE || size > MAX_MAZE_SIZE) {
        QMessageBox::warning(this, "输入错误",
            QString("迷宫大小必须�?%1 �?%2 之间").arg(MIN_MAZE_SIZE).arg(MAX_MAZE_SIZE));
        return;
    }

    m_algorithmRunner->stop();
    m_mazeWidth = size;
    m_mazeHeight = size;
    m_startPoint = QPoint(-1, -1);
    m_endPoint = QPoint(-1, -1);
    m_startPrevType = AType::Unknown;
    m_endPrevType = AType::Unknown;
    m_mainmap = initializeMaze(m_mazeWidth, m_mazeHeight);
    clearAlgorithmOverlay();
    mazeWidget->setGridData(&m_mainmap);
    mazeWidget->setStartPoint(&m_startPoint);
    mazeWidget->setEndPoint(&m_endPoint);
    mazeWidget->setMazeSize(m_mazeWidth, m_mazeHeight);
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
    if (m_mainmap[step.row][step.col]->type == AType::Wall) return;
    if (step.isPath) {
        m_mainmap[step.row][step.col]->type = AType::Path;
    } else {
        m_mainmap[step.row][step.col]->type = step.type;
    }
    m_visitedCount++;
    statusBar()->showMessage(QString("已访�? %1 个节�?).arg(m_visitedCount));
    mazeWidget->refresh();
}

void Widget::onAlgorithmFinished(bool found)
{
    if (found) {
        emit updateTextEdit(QString("找到路径！已访问 %1 个节�?).arg(m_visitedCount));
        statusBar()->showMessage(QString("找到路径！已访问 %1 个节�?).arg(m_visitedCount));
    } else {
        emit updateTextEdit(QString("未找到路径！已访�?%1 个节�?).arg(m_visitedCount));
        statusBar()->showMessage(QString("未找到路径！已访�?%1 个节�?).arg(m_visitedCount));
    }
}

void Widget::onSpeedChanged(int value)
{
    // speedSlider范围: 1-200, delay范围: 200-1ms
    const int MAX_SPEED_VALUE = 200;
    int delay = MAX_SPEED_VALUE + 1 - value;
    m_algorithmRunner->setSpeed(delay);
}

void Widget::onGenerateClicked()
{
    m_algorithmRunner->stop();
    // Random seed is handled internally by MazeGenerator using std::mt19937

    double ratio = densitySlider->value() / 100.0;
    MazeGenerator::generateSolvableMaze(m_mainmap, m_mazeWidth, m_mazeHeight, ratio);

    m_startPoint = QPoint(0, 0);
    m_endPoint = QPoint(m_mazeWidth - 1, m_mazeHeight - 1);
    m_startPrevType = AType::Unknown;
    m_endPrevType = AType::Unknown;
    m_mainmap[0][0]->type = AType::StartPoint;
    m_mainmap[m_mazeHeight-1][m_mazeWidth-1]->type = AType::EndPoint;
    clearAlgorithmOverlay();

    mazeWidget->refresh();
    emit updateTextEdit(QString("随机迷宫已生�?(障碍�? %1% - 保证可解)").arg(densitySlider->value()));
    statusBar()->showMessage("随机迷宫已生�?| 保证存在有效路径 | 左键画墙，右键设起点/终点");
}

void Widget::onResetPathClicked()
{
    m_algorithmRunner->stop();
    for (int row = 0; row < static_cast<int>(m_mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(m_mainmap[row].size()); col++) {
            if (m_mainmap[row][col]->type == AType::Path || m_mainmap[row][col]->type == AType::Visited) {
                m_mainmap[row][col]->type = AType::Unknown;
            }
            m_mainmap[row][col]->parent = nullptr;
            m_mainmap[row][col]->g_cost = 0;
            m_mainmap[row][col]->f_cost = 0;
            m_mainmap[row][col]->h_cost = 0;
        }
    }
    clearAlgorithmOverlay();
    mazeWidget->refresh();
    emit updateTextEdit("路径已重�?);
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
        emit updateTextEdit("图片已导�? " + fileName);
        statusBar()->showMessage("导出成功: " + fileName);
    } else {
        emit updateTextEdit("导出失败�?);
        statusBar()->showMessage("导出失败");
    }
}

void Widget::onCompareClicked()
{
    if (m_startPoint == QPoint(-1, -1) || m_endPoint == QPoint(-1, -1)) {
        emit updateTextEdit("请先设置起点和终点！");
        statusBar()->showMessage("错误: 请设置起点和终点");
        return;
    }

    emit updateTextEdit("正在比较算法...");
    statusBar()->showMessage("算法比较�?..");

    for (int row = 0; row < static_cast<int>(m_mainmap.size()); row++) {
        for (int col = 0; col < static_cast<int>(m_mainmap[row].size()); col++) {
            if (m_mainmap[row][col]->type == AType::Path || m_mainmap[row][col]->type == AType::Visited) {
                m_mainmap[row][col]->type = AType::Unknown;
            }
        }
    }
    clearAlgorithmOverlay();

    std::vector<std::vector<int>> maze = convertMazeToIntGrid();
    std::pair<int, int> start = {m_startPoint.y(), m_startPoint.x()};
    std::pair<int, int> end = {m_endPoint.y(), m_endPoint.x()};

    QVector<AlgorithmResult> results;

    MazeSolver solver;
    MovementType currentMovement = m_algorithmRunner->getMovementType();

    auto startTime = std::chrono::high_resolution_clock::now();
    auto resultBFS = solver.findPathBFS(maze, start, end, currentMovement);
    auto endTime = std::chrono::high_resolution_clock::now();
    double bfsTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    AlgorithmResult r1;
    r1.type = AlgorithmType::BFS;
    r1.found = resultBFS.found;
    r1.pathLength = resultBFS.path.size();
    r1.visitedCount = resultBFS.visitedCount;
    r1.executionTime = bfsTime;
    r1.path = resultBFS.path;
    results.append(r1);

    startTime = std::chrono::high_resolution_clock::now();
    auto resultDFS = solver.findPathDFS(maze, start, end, currentMovement);
    endTime = std::chrono::high_resolution_clock::now();
    double dfsTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    AlgorithmResult r2;
    r2.type = AlgorithmType::DFS;
    r2.found = resultDFS.found;
    r2.pathLength = resultDFS.path.size();
    r2.visitedCount = resultDFS.visitedCount;
    r2.executionTime = dfsTime;
    r2.path = resultDFS.path;
    results.append(r2);

    startTime = std::chrono::high_resolution_clock::now();
    auto resultDijkstra = solver.findPathDijkstra(maze, start, end, currentMovement);
    endTime = std::chrono::high_resolution_clock::now();
    double dijkstraTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    AlgorithmResult r3;
    r3.type = AlgorithmType::Dijkstra;
    r3.found = resultDijkstra.found;
    r3.pathLength = resultDijkstra.path.size();
    r3.visitedCount = resultDijkstra.visitedCount;
    r3.executionTime = dijkstraTime;
    r3.path = resultDijkstra.path;
    results.append(r3);

    // JPS算法测试
    startTime = std::chrono::high_resolution_clock::now();
    auto resultJPS = solver.findPathJPS(maze, start, end);
    endTime = std::chrono::high_resolution_clock::now();
    double jpsTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();

    AlgorithmResult r4;
    r4.type = AlgorithmType::JPS;
    r4.found = resultJPS.found;
    r4.pathLength = resultJPS.path.size();
    r4.visitedCount = resultJPS.visitedCount;
    r4.executionTime = jpsTime;
    r4.path = resultJPS.path;
    results.append(r4);

    startTime = std::chrono::high_resolution_clock::now();
    auto r5 = solver.compareAllAlgorithms(maze, start, end, currentMovement);
    endTime = std::chrono::high_resolution_clock::now();

    r5.executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    results.append(r5);

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
            if (m_mainmap[p.first][p.second]->type != AType::StartPoint &&
                m_mainmap[p.first][p.second]->type != AType::EndPoint) {
                m_mainmap[p.first][p.second]->algorithmPath = i;
                m_mainmap[p.first][p.second]->pathColor = pathColor;
                m_mainmap[p.first][p.second]->isBestPath = (bestIndex >= 0 && i == bestIndex);
            }
        }
    }

    mazeWidget->refresh();
    
    QString resultText;
    if (bestIndex < 0) {
        resultText = "比较完成！无可达路径";
    } else {
        QStringList algoNames = {"BFS", "DFS", "Dijkstra", "JPS", "A*"};
        QString algoName = (bestIndex >= 0 && bestIndex < algoNames.size()) ? algoNames[bestIndex] : "Unknown";
        resultText = QString("比较完成！最短路�? %1 (%2)")
            .arg(algoName)
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
    statusBar()->showMessage(QString("障碍物密�? %1%").arg(value));
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_R:
        clearMap();
        break;
    case Qt::Key_1:
        onAStarClicked();
        break;
    case Qt::Key_2:
        onBfsClicked();
        break;
    case Qt::Key_3:
        onDfsClicked();
        break;
    case Qt::Key_Space:
        if (m_algorithmRunner->isRunning()) {
            m_algorithmRunner->stop();
            emit updateTextEdit("已暂�?);
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

void Widget::onMovementTypeChanged(int index)
{
    MovementType type = static_cast<MovementType>(movementComboBox->itemData(index).toInt());
    m_algorithmRunner->setMovementType(type);

    QString typeName = (type == MovementType::FourDirections) ? "4方向" : "8方向";
    statusBar()->showMessage(QString("移动方式已切换为: %1").arg(typeName), 2000);
}

void Widget::onHeuristicTypeChanged(int index)
{
    HeuristicType type = static_cast<HeuristicType>(heuristicComboBox->itemData(index).toInt());
    m_algorithmRunner->setHeuristicType(type);

    QString typeName;
    switch (type) {
        case HeuristicType::Manhattan:
            typeName = "曼哈顿距�?;
            break;
        case HeuristicType::Euclidean:
            typeName = "欧几里得距离";
            break;
        case HeuristicType::Chebyshev:
            typeName = "切比雪夫距离";
            break;
        case HeuristicType::Octile:
            typeName = "八方向距�?;
            break;
    }
    statusBar()->showMessage(QString("启发式函数已切换�? %1").arg(typeName), 2000);
}
