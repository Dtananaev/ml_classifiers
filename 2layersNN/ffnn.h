/*
 * File: ffnn.h
 *
 * Author: Denis Tananaev
 * 
 * Date: 01.11.2016
 */

/* Description of the neural network
  A two-layer fully-connected neural network. The net has an input dimension of
  N, a hidden layer dimension of H, and performs classification over C classes.
  We train the network with a softmax loss function and L2 regularization on the
  weight matrices. The network uses a ReLU nonlinearity after the first fully
  connected layer.

  In other words, the network has the following architecture:

  input - fully connected layer - ReLU - fully connected layer - softmax

  The outputs of the second fully-connected layer are the scores for each class.
*/

#ifndef FFNN_H
#define FFNN_H

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
#include "ui_ffnn.h"

#include <memory>
#include "CMatrix.h"
#include "CVector.h"



class ffnn : public QMainWindow, private Ui_ffnn
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
    void standartisation();
    void normalization();
    void stopClicked();
public:

    explicit ffnn(QWidget *parent = 0);
    ~ffnn();
    void init();

    bool trainSetread(const char* dirname);
    bool testSetread(const char* dirname);
    void updateWeights();
    void initRandomWeights();


    void weight2image(CMatrix<float> w,int label, QImage &img);
    void vizWeights();

//Neural-Network functions
    CMatrix<float> calculateScoresLayer1(int from, int until );
    CMatrix<float> ReLU(CMatrix<float> scores);
    CMatrix<float> calculateScoresLayer2(CMatrix<float> data);
    void softmaxLoss(CMatrix<float> scores,CMatrix<float> relu, int from, int until);

    
private slots:
    void open();

private:
    int L1_neurons_;
    int L2_neurons_; 
    std::vector<int> index_;


    bool stop_;
    float Loss_=0;
    int iteration_;
    int batch_size_;
    int currentIndex_;
    float lambda_; //regularization parameter
    float step_;

      CMatrix<float> W1_;//weight matrix
      CMatrix<float> W2_;//weight matrix
      CMatrix<float> dW1_;//gradient weight matrix
      CMatrix<float> dW2_;//gradient weight matrix
     // CMatrix<float> score_;  //cost matrix   
      std::vector<float> loss_; //vector of loss for each data sample
    std::vector<float> normalizer_;

    std::vector<int> train_labels;
    std::vector< std::vector<float> > train_images; 
    std::vector<int> test_labels;
    std::vector< std::vector<float> > test_images; 
    std::vector<std::string> categories;

};

#endif // FFNN_H
