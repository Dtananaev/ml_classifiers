/*
 * File: visualizer.h
 *
 * Authors: Denis Tananaev, Vladislav Tananaev
 *
 * Date: 24.10.2016
 */
#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QKeyEvent>
#include <QString>
#include <QScrollBar>
#include <QFileDialog>
#include <QDir>

#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
#include <fstream>
#include <iostream>
#include <vector>
#include "ui_visualizer.h"


class visualizer : public QMainWindow, private Ui_visualizer
{
    Q_OBJECT
public slots:
   
    void updateImage();

public:
    explicit visualizer(QWidget *parent = 0);
    ~visualizer();
    void init();
    bool readCFAR(const char* dirname);

private slots:
    void open();

private:

    std::vector<int> labels;
    std::vector< std::vector<int> > images; 
    std::vector<std::string> categories;

};

#endif // VISUALIZER_H
