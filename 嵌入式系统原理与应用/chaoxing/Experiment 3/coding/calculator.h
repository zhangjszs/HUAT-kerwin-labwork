#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class Calculator : public QDialog {
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);

private slots:
    void digitClicked();
    void operatorClicked();
    void equalClicked();
    void clearClicked();

private:
    QLineEdit *display;
    QString expression;  // 存储完整表达式
    bool newNumber;      // 标记新数字输入

    QPushButton* createButton(const QString &text, const char *member);
    bool lastCharIsOperator();  // 检查最后一个字符是否是运算符
};

#endif    
