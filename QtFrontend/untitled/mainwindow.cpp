#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../../ExpressionEvaluatorLib/ExpressionEvaluatorSingleHeader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_cmdEvaluate_clicked()
{
    QString expression = ui->editExpression->text();

    ExpressionEvaluator calculator;

    string result = calculator.driver(expression.toStdString());
    QString qstr = QString::fromStdString(result);

    ui->txtResult->setText(qstr);
}

void MainWindow::on_editExpression_returnPressed()
{
    on_cmdEvaluate_clicked();

    ui->editExpression->setText("");
}
