#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QString>
#include <QVector>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QSpinBox>
#include <QSlider>
#include <QKeyEvent>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include "constants.h"
#include "models/mazegrid.h"
#include "core/algorithms.h"
#include "services/fileservice.h"
#include "mazewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class CompareDialog : public QDialog {
    Q_OBJECT
public:
    CompareDialog(QWidget *parent = nullptr);
    void setResults(const QVector<AlgorithmResult>& results);
};

class Widget : public QMainWindow
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    // Getters
    const Grid2D& getMainmap() const { return m_mainmap; }
    Grid2D& getMainmap() { return m_mainmap; }
    QPoint getStartPoint() const { return m_startPoint; }
    QPoint getEndPoint() const { return m_endPoint; }
    void setStartPoint(const QPoint& point) { m_startPoint = point; }
    void setEndPoint(const QPoint& point) { m_endPoint = point; }

    void applyTheme(const QString &theme);
    MazeWidget* getMazeWidget() { return mazeWidget; }

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::Widget *ui;
    QPushButton* aStarButton;
    QPushButton* bfsButton;
    QPushButton* dfsButton;
    QPushButton* dijkstraButton;
    QPushButton* jpsButton;
    QPushButton* clearButton;
    QPushButton* loadButton;
    QPushButton* saveButton;
    QPushButton* tipsButton;
    QPushButton* themeButton;
    QPushButton* sizeButton;
    QPushButton* generateButton;
    QPushButton* resetPathButton;
    QPushButton* exportButton;
    QPushButton* compareButton;
    QSpinBox* sizeSpinBox;
    QSlider* speedSlider;
    QSlider* densitySlider;
    QComboBox* movementComboBox;
    QComboBox* heuristicComboBox;
    QTextEdit * textEdit;
    MazeWidget* mazeWidget;
    AlgorithmRunner* m_algorithmRunner;
    bool m_isDarkTheme = false;
    int m_visitedCount = 0;
    AType m_startPrevType = AType::Unknown;
    AType m_endPrevType = AType::Unknown;
    int m_mazeWidth = 20;
    int m_mazeHeight = 20;
    Grid2D m_mainmap;
    QPoint m_startPoint = QPoint(-1, -1);
    QPoint m_endPoint = QPoint(-1, -1);
    Grid2D initializeMaze(int width, int height);
    void setupConnections();
    void applySize(int size);
    void generateRandomMaze();
    void resetPathOnly();
    void exportAsImage();
    void compareAlgorithms();
    void clearAlgorithmOverlay();
    std::vector<std::vector<int>> convertMazeToIntGrid();

signals:
    void updateTextEdit(const QString &text);

public slots:
    void handleUpdateTextEdit(const QString &text);
    void onAStarClicked();
    void onBfsClicked();
    void onDfsClicked();
    void onDijkstraClicked();
    void onJPSClicked();
    void clearMap();
    void loadMap();
    void saveMap();
    void showTipsDialog();
    void toggleTheme();
    void onMazeClicked(int i, int j, Qt::MouseButton button);
    void onMazeDragged(int i, int j);
    void onSizeButtonClicked();
    void onAlgorithmStep(const StepInfo& step);
    void onAlgorithmFinished(bool found);
    void onSpeedChanged(int value);
    void onGenerateClicked();
    void onResetPathClicked();
    void onExportClicked();
    void onCompareClicked();
    void onDensityChanged(int value);
    void onMovementTypeChanged(int index);
    void onHeuristicTypeChanged(int index);

};

#endif
