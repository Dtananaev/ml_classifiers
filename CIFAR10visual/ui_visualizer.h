/********************************************************************************
** Form generated from reading UI file 'visualizer.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISUALIZER_H
#define UI_VISUALIZER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_visualizer
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *visualizer)
    {
        if (visualizer->objectName().isEmpty())
            visualizer->setObjectName(QString::fromUtf8("visualizer"));
        visualizer->resize(431, 222);
        centralWidget = new QWidget(visualizer);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(300, 60, 99, 27));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 201, 131));
        visualizer->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(visualizer);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 431, 25));
        visualizer->setMenuBar(menuBar);
        mainToolBar = new QToolBar(visualizer);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        visualizer->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(visualizer);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        visualizer->setStatusBar(statusBar);

        retranslateUi(visualizer);

        QMetaObject::connectSlotsByName(visualizer);
    } // setupUi

    void retranslateUi(QMainWindow *visualizer)
    {
        visualizer->setWindowTitle(QApplication::translate("visualizer", "visualizer", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("visualizer", "readData", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("visualizer", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class visualizer: public Ui_visualizer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISUALIZER_H
