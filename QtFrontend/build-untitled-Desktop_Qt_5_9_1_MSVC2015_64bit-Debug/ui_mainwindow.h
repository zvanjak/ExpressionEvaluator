/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionEvaluate;
    QWidget *centralWidget;
    QLineEdit *editExpression;
    QPushButton *cmdEvaluate;
    QLabel *txtResult;
    QMenuBar *menuBar;
    QMenu *menuCalculator;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(414, 148);
        actionEvaluate = new QAction(MainWindow);
        actionEvaluate->setObjectName(QStringLiteral("actionEvaluate"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        editExpression = new QLineEdit(centralWidget);
        editExpression->setObjectName(QStringLiteral("editExpression"));
        editExpression->setGeometry(QRect(10, 10, 281, 20));
        cmdEvaluate = new QPushButton(centralWidget);
        cmdEvaluate->setObjectName(QStringLiteral("cmdEvaluate"));
        cmdEvaluate->setGeometry(QRect(310, 10, 75, 23));
        txtResult = new QLabel(centralWidget);
        txtResult->setObjectName(QStringLiteral("txtResult"));
        txtResult->setGeometry(QRect(10, 50, 161, 21));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 414, 21));
        menuCalculator = new QMenu(menuBar);
        menuCalculator->setObjectName(QStringLiteral("menuCalculator"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuCalculator->menuAction());
        menuCalculator->addAction(actionEvaluate);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionEvaluate->setText(QApplication::translate("MainWindow", "Evaluate", Q_NULLPTR));
        cmdEvaluate->setText(QApplication::translate("MainWindow", "Evaluate", Q_NULLPTR));
        txtResult->setText(QString());
        menuCalculator->setTitle(QApplication::translate("MainWindow", "Calculator", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
