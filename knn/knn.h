/*
 * File: knn.h
 *
 * Author: Denis Tananaev
 *
 * Date: 25.10.2016
 */
#ifndef KNN_H
#define KNN_H

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
#include "ui_knn.h"


class knn : public QMainWindow, private Ui_knn
{
    Q_OBJECT
public slots:
   
    void updateImage();
    void updateK();
    void updateL1();
    void updateL2();
    void calculatePerformance();

public:
    explicit knn(QWidget *parent = 0);
    ~knn();
    void init();
    bool trainSetread(const char* dirname);
    bool testSetread(const char* dirname);
    int  knnDistance(int test_picture_index); 

private slots:
    void open();

private:
    int currentIndex_;
    int k_=1;
    std::vector<int> train_labels;
    std::vector< std::vector<int> > train_images; 
    std::vector<int> test_labels;
    std::vector< std::vector<int> > test_images; 
    std::vector<std::string> categories;

};

#endif // KNN_H
