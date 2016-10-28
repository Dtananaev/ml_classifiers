/*
 * File: svm.h
 *
 * Author: Denis Tananaev
 *
 * Date: 26.10.2016
 */
#ifndef SVM_H
#define SVM_H

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
#include "ui_svm.h"

#include <memory>
#include "CMatrix.h"
#include "CVector.h"



class svm : public QMainWindow, private Ui_svm
{
    Q_OBJECT
public slots:
   
    void updateImage();

    void runTraining();
    void updateStep();
    void updateLambda();
    void updatIterationNumber();
    void updatBatchNumber();
    void resetWeights();
public:

    explicit svm(QWidget *parent = 0);
    ~svm();
    void init();
    void calculatePerformance();
    bool trainSetread(const char* dirname);
    bool testSetread(const char* dirname);
    void calculateScores(int from, int until,int batch);
    void SVMtraining(int from, int until);
    void updateWeights();
    void initRandomWeights();
    void SVMiterate(int iter, int batch);

    int inference(int test_picture_index);
    void weight2image(CMatrix<float> w,int label, QImage &img);
    void vizWeights();

private slots:
    void open();

private:
    float Loss_=0;
    int iteration_;
    int batch_size_;
    int currentIndex_;
    float lambda_; //regularization parameter
    float step_;

      CMatrix<float> W_;//weight matrix
      CMatrix<float> dW_;//gradient weight matrix
      CMatrix<float> score_;  //cost matrix   
      std::vector<float> loss_; //vector of loss for each data sample
 
    std::vector<int> train_labels;
    std::vector< std::vector<int> > train_images; 
    std::vector<int> test_labels;
    std::vector< std::vector<int> > test_images; 
    std::vector<std::string> categories;

};

#endif // SVM_H
