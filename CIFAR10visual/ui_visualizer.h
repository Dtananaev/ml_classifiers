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
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_visualizer
{
public:
    QWidget *centralWidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSpinBox *numberBox;
    QLabel *labelPicture;
    QLineEdit *lineEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *visualizer)
    {
        if (visualizer->objectName().isEmpty())
            visualizer->setObjectName(QString::fromUtf8("visualizer"));
        visualizer->resize(162, 265);
        centralWidget = new QWidget(visualizer);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 0, 148, 202));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        numberBox = new QSpinBox(widget);
        numberBox->setObjectName(QString::fromUtf8("numberBox"));
        numberBox->setMaximum(60000);

        verticalLayout->addWidget(numberBox);

        labelPicture = new QLabel(widget);
        labelPicture->setObjectName(QString::fromUtf8("labelPicture"));
        labelPicture->setMinimumSize(QSize(141, 111));

        verticalLayout->addWidget(labelPicture);

        lineEdit = new QLineEdit(widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);

        visualizer->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(visualizer);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 162, 25));
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
        label->setText(QApplication::translate("visualizer", "CIFAR10 visualizer", 0, QApplication::UnicodeUTF8));
        labelPicture->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class visualizer: public Ui_visualizer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISUALIZER_H
