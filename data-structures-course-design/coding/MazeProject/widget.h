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

    Grid2D Mainmap;
    QPoint startPoint = QPoint(-1, -1);
    QPoint endPoint = QPoint(-1, -1);

    void applyTheme(const QString &theme);
    MazeWidget* getMazeWidget() { return mazeWidget; }

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::Widget *ui;
    QPushButton* aStarButton;
    QPushButton* bfsButton;
    QPushButton* dfsButton;
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
    QTextEdit * textEdit;
    MazeWidget* mazeWidget;
    AlgorithmRunner* m_algorithmRunner;
    bool m_isDarkTheme = false;
    int m_visitedCount = 0;
    int m_currentAlgorithm = 0;
    AType m_startPrevType = AType::Unknow;
    AType m_endPrevType = AType::Unknow;
    Grid2D initializeMaze(int rows, int cols);
    void clearAlgorithmOverlay();
    void setupConnections();
    void applySize(int size);
    void generateRandomMaze();
    void resetPathOnly();
    void exportAsImage();
    void compareAlgorithms();

signals:
    void updateTextEdit(const QString &text);

public slots:
    void handleUpdateTextEdit(const QString &text);
    void onAStarClicked();
    void onBfsClicked();
    void onDfsClicked();
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

};

#endif
