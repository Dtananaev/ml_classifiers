/*
 * File: svm.h
 *
 * Author: Denis Tananaev
 *
 * Date: 26.10.2016
 */
#ifndef SVM_H
#define SVM_H
#include "linearsvm.h"
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
    void zeroMeanData();
    void stopClicked();
public:

    explicit svm(QWidget *parent = 0);
    ~svm();
    void init();
    void calculatePerformance();
    bool trainSetread(const char* dirname);
    bool testSetread(const char* dirname);



    void initRandomWeights();
    void SVMiterate(int iter, int batch);

    void weight2image(CMatrix<float> w,int label, QImage &img);
    void vizWeights();



    //svm 
    float L2W_reg();

    float loss_one_image(const std::vector<int> &image, const int &y);
    float loss(const std::vector< std::vector<int> > &images, const std::vector<int> &labels, int from, int to);

    int inference(const std::vector<int> &image);

private slots:
    void open();

private:
    int C;
    int D; // data dimentionality



    bool stop_;
    void CalcMeanSigma(const std::vector<double> v, double &mean, double &sigma);
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
