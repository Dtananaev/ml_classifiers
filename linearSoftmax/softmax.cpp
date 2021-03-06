/*
 * File: softmax.cpp
 *
 * Author: Denis Tananaev
 *
 * Date: 29.10.2016
 */

#include "softmax.h"
#include <random>
#include <omp.h>
#include <algorithm>  // std::random_shuffle
softmax::softmax(QWidget *parent) : QMainWindow(parent) {
  stop_ = true;
  // this sets up GUI
  setupUi(this);

  // File menu
  menuBar()->setNativeMenuBar(false);  // this line  is necessary for
                                       // visualization otherwise menu invisible
  QAction *open = new QAction("&Open", this);
  QAction *quit = new QAction("&Quit", this);
  quit->setShortcut(tr("CTRL+Q"));

  QMenu *file;
  file = menuBar()->addMenu("&File");
  file->addAction(open);
  file->addSeparator();
  file->addAction(quit);

  connect(open, SIGNAL(triggered()), this, SLOT(open()));
  connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(numberBox, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));

  connect(stopButton, SIGNAL(clicked()), this, SLOT(stopClicked()));
  connect(trainButton, SIGNAL(clicked()), this, SLOT(runTraining()));
  connect(resetWeightsButton, SIGNAL(clicked()), this, SLOT(resetWeights()));
  connect(zeroMeanButton, SIGNAL(clicked()), this, SLOT(zeroMeanData()));
  connect(standardizationButton, SIGNAL(clicked()), this,
          SLOT(standartisation()));
  connect(normalizationButton, SIGNAL(clicked()), this, SLOT(normalization()));

  connect(stepBox, SIGNAL(valueChanged(double)), this, SLOT(updateStep()));
  connect(lambdaBox, SIGNAL(valueChanged(double)), this, SLOT(updateLambda()));
  connect(itBox, SIGNAL(valueChanged(int)), this, SLOT(updatIterationNumber()));
  connect(batchBox, SIGNAL(valueChanged(int)), this, SLOT(updatBatchNumber()));

  batchBox->setValue(100);
  stepBox->setRange(0.00000000000001, 10);
  stepBox->setValue(0.0000001);
  stepBox->setSingleStep(0.0000001);
  lambdaBox->setValue(0.5);
}

softmax::~softmax() {}

void softmax::stopClicked() { stop_ = true; }

void softmax::weight2image(CMatrix<float> w, int label, QImage &img) {
  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      int red = w(label, y * 32 + x);
      int green = w(label, 1024 + y * 32 + x);
      int blue = w(label, 2048 + y * 32 + x);
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }
}

void softmax::vizWeights() {
  // create 10 image for weights
  QImage img0(32, 32, QImage::Format_RGB888);
  QImage img1(32, 32, QImage::Format_RGB888);
  QImage img2(32, 32, QImage::Format_RGB888);
  QImage img3(32, 32, QImage::Format_RGB888);
  QImage img4(32, 32, QImage::Format_RGB888);
  QImage img5(32, 32, QImage::Format_RGB888);
  QImage img6(32, 32, QImage::Format_RGB888);
  QImage img7(32, 32, QImage::Format_RGB888);
  QImage img8(32, 32, QImage::Format_RGB888);
  QImage img9(32, 32, QImage::Format_RGB888);

  // CMatrix<float> w=W_;
  CMatrix<float> w = W_test_;
  w.normalize(0, 255);
  weight2image(w, 0, img0);
  weight2image(w, 1, img1);
  weight2image(w, 2, img2);
  weight2image(w, 3, img3);
  weight2image(w, 4, img4);
  weight2image(w, 5, img5);
  weight2image(w, 6, img6);
  weight2image(w, 7, img7);
  weight2image(w, 8, img8);
  weight2image(w, 9, img9);

  img0 = img0.scaledToWidth(airplaneLbl1->width(), Qt::SmoothTransformation);
  airplaneLbl1->setPixmap(QPixmap::fromImage(img0));
  airplaneLbl1->show();

  img1 = img1.scaledToWidth(automobilLbl2->width(), Qt::SmoothTransformation);
  automobilLbl2->setPixmap(QPixmap::fromImage(img1));
  automobilLbl2->show();

  img2 = img2.scaledToWidth(birdLbl3->width(), Qt::SmoothTransformation);
  birdLbl3->setPixmap(QPixmap::fromImage(img2));
  birdLbl3->show();

  img2 = img2.scaledToWidth(birdLbl3->width(), Qt::SmoothTransformation);
  birdLbl3->setPixmap(QPixmap::fromImage(img2));
  birdLbl3->show();

  img3 = img3.scaledToWidth(catLbl4->width(), Qt::SmoothTransformation);
  catLbl4->setPixmap(QPixmap::fromImage(img3));
  catLbl4->show();

  img4 = img4.scaledToWidth(deerLbl5->width(), Qt::SmoothTransformation);
  deerLbl5->setPixmap(QPixmap::fromImage(img4));
  deerLbl5->show();

  img5 = img5.scaledToWidth(dogLbl6->width(), Qt::SmoothTransformation);
  dogLbl6->setPixmap(QPixmap::fromImage(img5));
  dogLbl6->show();

  img6 = img6.scaledToWidth(frogLbl7->width(), Qt::SmoothTransformation);
  frogLbl7->setPixmap(QPixmap::fromImage(img6));
  frogLbl7->show();

  img7 = img7.scaledToWidth(horseLbl8->width(), Qt::SmoothTransformation);
  horseLbl8->setPixmap(QPixmap::fromImage(img7));
  horseLbl8->show();

  img8 = img8.scaledToWidth(shipLbl9->width(), Qt::SmoothTransformation);
  shipLbl9->setPixmap(QPixmap::fromImage(img8));
  shipLbl9->show();

  img9 = img9.scaledToWidth(truckLbl10->width(), Qt::SmoothTransformation);
  truckLbl10->setPixmap(QPixmap::fromImage(img9));
  truckLbl10->show();
}

void softmax::updatIterationNumber() { iteration_ = itBox->value(); }

void softmax::updatBatchNumber() { batch_size_ = batchBox->value(); }

void softmax::updateStep() { step_ = stepBox->value(); }
void softmax::updateLambda() { lambda_ = lambdaBox->value(); }
void softmax::runTraining() {
  stop_ = false;

  iterate(iteration_, batch_size_);
}

void softmax::zeroMeanData() {
  meanLabel->setText(QString::fromUtf8("Wait..."));
  // compute the mean image

  CMatrix<float> red(32, 32, 0);
  CMatrix<float> green(32, 32, 0);
  CMatrix<float> blue(32, 32, 0);

  for (int i = 0; i < train_images_.size(); i++) {
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        red(x, y) += train_images_[i][y * 32 + x];
        green(x, y) += train_images_[i][1024 + y * 32 + x];
        blue(x, y) += train_images_[i][2048 + y * 32 + x];
      }
    }
  }

  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      red(x, y) = red(x, y) / 50000;
      green(x, y) = green(x, y) / 50000;
      blue(x, y) = blue(x, y) / 50000;
    }
  }

  for (int i = 0; i < train_images_.size(); i++) {
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        train_images_[i][y * 32 + x] =
            (train_images_[i][y * 32 + x] - red(x, y));
        train_images_[i][1024 + y * 32 + x] =
            (train_images_[i][1024 + y * 32 + x] - green(x, y));
        train_images_[i][2048 + y * 32 + x] =
            (train_images_[i][2048 + y * 32 + x] - blue(x, y));
      }
    }
  }

  for (int i = 0; i < test_images_.size(); i++) {
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        test_images_[i][y * 32 + x] = (test_images_[i][y * 32 + x] - red(x, y));
        test_images_[i][1024 + y * 32 + x] =
            (test_images_[i][1024 + y * 32 + x] - green(x, y));
        test_images_[i][2048 + y * 32 + x] =
            (test_images_[i][2048 + y * 32 + x] - blue(x, y));
      }
    }
  }
  meanLabel->setText(QString::fromUtf8("Zero Mean dataset."));
  std::cout << "Make dataset zero mean"
            << "\n";
  zeroMeanButton->setEnabled(false);
}
void softmax::CalcMeanSigma(const std::vector<double> data, double &mean,
                            double &sigma) {
  mean = 0;
  sigma = 0;
  for (int i = 0; i < data.size(); i++) {
    mean += data[i];
  }
  mean = mean / data.size();

  for (int i = 0; i < data.size(); i++) {
    sigma += (data[i] - mean) * (data[i] - mean);
  }

  sigma = sqrt(sigma / (data.size() - 1));
}

void softmax::standartisation() {
  meanLabel->setText(QString::fromUtf8("Wait..."));
  // zero mean and sigma from  1
  // compute the mean image
  CMatrix<float> red(32, 32, 0);
  CMatrix<float> green(32, 32, 0);
  CMatrix<float> blue(32, 32, 0);

  for (int i = 0; i < train_images_.size(); i++) {
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        red(x, y) += train_images_[i][y * 32 + x];
        green(x, y) += train_images_[i][1024 + y * 32 + x];
        blue(x, y) += train_images_[i][2048 + y * 32 + x];
      }
    }
  }

  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      red(x, y) = red(x, y) / 50000;
      green(x, y) = green(x, y) / 50000;
      blue(x, y) = blue(x, y) / 50000;
    }
  }

  // compute the sigma
  CMatrix<double> redSigma(32, 32, 0);
  CMatrix<double> greenSigma(32, 32, 0);
  CMatrix<double> blueSigma(32, 32, 0);

  for (int i = 0; i < train_images_.size(); i++) {
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        redSigma(x, y) += (train_images_[i][y * 32 + x] - red(x, y)) *
                          (train_images_[i][y * 32 + x] - red(x, y));
        greenSigma(x, y) +=
            (train_images_[i][1024 + y * 32 + x] - green(x, y)) *
            (train_images_[i][1024 + y * 32 + x] - green(x, y));
        blueSigma(x, y) += (train_images_[i][2048 + y * 32 + x] - blue(x, y)) *
                           (train_images_[i][2048 + y * 32 + x] - blue(x, y));
      }
    }
  }

  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      redSigma(x, y) = sqrt(redSigma(x, y) / 49999);
      greenSigma(x, y) = sqrt(greenSigma(x, y) / 49999);
      blueSigma(x, y) = sqrt(blueSigma(x, y) / 49999);
    }
  }

  // normalization
  for (int i = 0; i < train_images_.size(); i++) {
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        train_images_[i][y * 32 + x] =
            (train_images_[i][y * 32 + x] - red(x, y)) / redSigma(x, y);
        train_images_[i][1024 + y * 32 + x] =
            (train_images_[i][1024 + y * 32 + x] - green(x, y)) /
            greenSigma(x, y);
        train_images_[i][2048 + y * 32 + x] =
            (train_images_[i][2048 + y * 32 + x] - blue(x, y)) /
            blueSigma(x, y);
      }
    }
  }

  for (int i = 0; i < test_images_.size(); i++) {
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        test_images_[i][y * 32 + x] =
            (test_images_[i][y * 32 + x] - red(x, y)) / redSigma(x, y);
        test_images_[i][1024 + y * 32 + x] =
            (test_images_[i][1024 + y * 32 + x] - green(x, y)) /
            greenSigma(x, y);
        test_images_[i][2048 + y * 32 + x] =
            (test_images_[i][2048 + y * 32 + x] - blue(x, y)) / blueSigma(x, y);
      }
    }
  }

  meanLabel->setText(QString::fromUtf8("Standardized  dataset."));
  std::cout << "Make dataset Standardized"
            << "\n";
  zeroMeanButton->setEnabled(false);
  standardizationButton->setEnabled(false);
}
// put the values zero mean and from at the intervall -1 to 1 y =
// (ymax-ymin)*(x-xmin)/(xmax-xmin) + ymin;
void softmax::normalization() {
  for (int i = 0; i < train_images_.size(); i++) {
    float minR = 0;
    float minG = 0;
    float minB = 0;
    float maxR = 0;
    float maxG = 0;
    float maxB = 0;
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        if (train_images_[i][y * 32 + x] > maxR) {
          maxR = train_images_[i][y * 32 + x];
        }
        if (train_images_[i][y * 32 + x] < minR) {
          minR = train_images_[i][y * 32 + x];
        }

        if (train_images_[i][1024 + y * 32 + x] > maxG) {
          maxG = train_images_[i][1024 + y * 32 + x];
        }
        if (train_images_[i][1024 + y * 32 + x] < minG) {
          minG = train_images_[i][1024 + y * 32 + x];
        }

        if (train_images_[i][2048 + y * 32 + x] > maxB) {
          maxB = train_images_[i][2048 + y * 32 + x];
        }
        if (train_images_[i][2048 + y * 32 + x] < minB) {
          minB = train_images_[i][2048 + y * 32 + x];
        }
      }
    }

    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        train_images_[i][y * 32 + x] =
            2 * (train_images_[i][y * 32 + x] - minR) / (maxR - minR) - 1;
        train_images_[i][1024 + y * 32 + x] =
            2 * (train_images_[i][1024 + y * 32 + x] - minG) / (maxG - minG) -
            1;
        train_images_[i][2048 + y * 32 + x] =
            2 * (train_images_[i][2048 + y * 32 + x] - minB) / (maxB - minB) -
            1;
      }
    }
  }

  for (int i = 0; i < test_images_.size(); i++) {
    float minR = 0;
    float minG = 0;
    float minB = 0;
    float maxR = 0;
    float maxG = 0;
    float maxB = 0;
    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        if (test_images_[i][y * 32 + x] > maxR) {
          maxR = test_images_[i][y * 32 + x];
        }
        if (test_images_[i][y * 32 + x] < minR) {
          minR = test_images_[i][y * 32 + x];
        }

        if (test_images_[i][1024 + y * 32 + x] > maxG) {
          maxG = test_images_[i][1024 + y * 32 + x];
        }
        if (test_images_[i][1024 + y * 32 + x] < minG) {
          minG = test_images_[i][1024 + y * 32 + x];
        }

        if (test_images_[i][2048 + y * 32 + x] > maxB) {
          maxB = test_images_[i][2048 + y * 32 + x];
        }
        if (test_images_[i][2048 + y * 32 + x] < minB) {
          minB = test_images_[i][2048 + y * 32 + x];
        }
      }
    }

    for (int x = 0; x < 32; ++x) {
      for (int y = 0; y < 32; ++y) {
        test_images_[i][y * 32 + x] =
            2 * (test_images_[i][y * 32 + x] - minR) / (maxR - minR) - 1;
        test_images_[i][1024 + y * 32 + x] =
            2 * (test_images_[i][1024 + y * 32 + x] - minG) / (maxG - minG) - 1;
        test_images_[i][2048 + y * 32 + x] =
            2 * (test_images_[i][2048 + y * 32 + x] - minB) / (maxB - minB) - 1;
      }
    }
  }

  meanLabel->setText(QString::fromUtf8("Normalized dataset."));
  std::cout << "Make dataset normalized"
            << "\n";
  // zeroMeanButton->setEnabled(false);
  normalizationButton->setEnabled(false);
}

// calculate scores by using linear score function y=W*x for training
void softmax::calculateScores(int from, int until, int batch) {
  // for each image we should have vector of the size 10 (score for each class)
  // for all train set of the size N we will have matrix Nx10 of scores (in our
  // case 50 000x10)
  // init score matrix
  score_.setSize(batch, categories_.size());  // Nx10
  score_.fill(0.0);
  normalizer_.clear();

  // for each image in train set compute the cost:
  for (int i = from; i < until; i++) {
    int max_score = 0;
    int a = i - from;
    for (int p = 0; p < train_images_[index_[i]].size(); p++) {
      // cost(50000,10)  W(10, 3073)*image(3073,1)

      for (int label = 0; label < 10; label++) {
        score_(a, label) += W_(label, p) * train_images_[index_[i]][p];
        if (score_(a, label) > max_score) {
          max_score = score_(a, label);
        }
      }
      // normalize scores in order to avoid numerical issues we need to
      // substract the max score from all scores
    }
    float normalizer = 0;
    for (int label = 0; label < 10; label++) {
      score_(a, label) = exp(score_(a, label) - max_score);
      normalizer += score_(a, label);
    }
    normalizer_.push_back(normalizer);
  }
}
void softmax::iterate(int iter, int batch) {
  progressBar->setValue(0);
  int number_of_full_batches = train_images_.size() / batch;
  int last_batch = train_images_.size() - number_of_full_batches * batch;

  for (int i = 0; i < iter; i++) {
    std::random_shuffle(index_.begin(), index_.end());  // random batch sampling

    itResult->setText(QString::number(i + 1) + "/" + QString::number(iter));
    progressBar->setValue(0);
    for (int j = 0; j < number_of_full_batches; j++) {
      if (stop_) {
        return;
      }

      int progress = 100 * j / number_of_full_batches;
      progressBar->setValue(progress);

      int from = j * batch;
      int until = j * batch + batch;
      calculateScores(from, until, batch);
      SoftmaxTraining(from, until);
      updateWeights();
      lossResult->setText(QString::number(Loss_));
      vizWeights();
    }

    calculatePerformance();
  }

  progressBar->setValue(100);
}

void softmax::resetWeights() {
  // init weight and scores matrices
  initRandomWeights();
  vizWeights();
  int result = inference(currentIndex_);
  lineEdit_2->setText(QString::fromUtf8(categories_[result].c_str()));
}
void softmax::updateWeights() {
  float weight = 0;
  float dw = 0;
  // min max
  float min = 0;
  float max = 0;
  float dmin = 0;
  float dmax = 0;

  for (int x = 0; x < dW_.xSize(); x++) {
    for (int y = 0; y < dW_.ySize(); y++) {
      if (stop_) {
        return;
      }
      // calculate the scale of weights and weight update
      weight += W_(x, y) * W_(x, y);
      dw += step_ * dW_(x, y) * step_ * dW_(x, y);
      W_(x, y) -= step_ * dW_(x, y);
      W_test_(x, y) = 0.995 * W_test_(x, y) + 0.005 * W_(x, y);

      if (W_(x, y) > max) {
        max = W_(x, y);
      }
      if (W_(x, y) < min) {
        min = W_(x, y);
      }

      if (dW_(x, y) > dmax) {
        dmax = dW_(x, y);
      }
      if (dW_(x, y) < dmin) {
        dmin = dW_(x, y);
      }
    }
  }
  float a = sqrt(dw) / sqrt(weight);
  weightRelation->setText(QString::number(a));
  // mimax weights
  wMin->setText(QString::number(min));
  wMax->setText(QString::number(max));
  dwMin->setText(QString::number(dmin));
  dwMax->setText(QString::number(dmax));
  uMin->setText(QString::number(step_ * dmin));
  uMax->setText(QString::number(step_ * dmax));
}

// SoftmaxTraining  function
void softmax::SoftmaxTraining(int from, int until) {
  dW_.fill(0);  // reset grad update matrix
  loss_.clear();  // compute loss for each image
  int N = until - from;
  for (int i = from; i < until; i++) {
    QCoreApplication::processEvents();
    if (stop_) {
      return;
    }
    int a = i - from;
    double y = score_(a, train_labels_[index_[i]]);  // right  exp(score)

    // compute  softmax loss
    double softmax = y / normalizer_[a];
    loss_.push_back(-log(softmax));

    for (int dx = 0; dx < dW_.xSize(); dx++) {
      for (int dy = 0; dy < dW_.ySize(); dy++) {
        if (train_labels_[index_[i]] ==
            dx) {  // if this is correct label row of weights

          dW_(dx, dy) += train_images_[index_[i]][dy] * (softmax - 1);

        } else if (train_labels_[index_[i]] != dx) {
          dW_(dx, dy) +=
              train_images_[index_[i]][dy] * score_(a, dx) / normalizer_[a];
        }
      }
    }
  }

  float Loss = std::accumulate(loss_.begin(), loss_.end(), 0.0);

  // dweight regularization
  for (int dx = 0; dx < dW_.xSize(); dx++) {
    for (int dy = 0; dy < dW_.ySize(); dy++) {
      dW_(dx, dy) = dW_(dx, dy) / N + lambda_ * W_(dx, dy);
    }
  }
  float regularization = 0;
  for (int y = 0; y < W_.ySize(); y++) {
    for (int x = 0; x < W_.xSize(); x++) {
      regularization += W_(x, y) * W_(x, y);
    }
  }

  regularization = 0.5 * lambda_ * regularization;
  Loss_ = Loss / loss_.size() + regularization;
}

void softmax::initRandomWeights() {
  W_.setSize(categories_.size(), train_images_[0].size());  // weight matrix
  W_test_.setSize(categories_.size(), train_images_[0].size());
  W_test_.fill(0);
  std::default_random_engine generator;
  std::normal_distribution<double> distribution(0, 0.00001);

  for (int x = 0; x < W_.xSize(); x++) {
    for (int y = 0; y < W_.ySize(); y++) {
      W_(x, y) = distribution(generator);
      if (y == W_.ySize() - 1) {  // make the weight for bias positive (better
                                  // for initialization)
        W_(x, y) = fabs(W_(x, y));
      }
    }
  }
}

void softmax::init() {
  // add indices from 0 to 49 999 to the index_ vector for random batch sampling
  for (int i = 0; i < 50000; i++) {
    index_.push_back(i);
  }

  // fill the cathegory names
  categories_.push_back("airplane");
  categories_.push_back("automobile");
  categories_.push_back("bird");
  categories_.push_back("cat");
  categories_.push_back("deer");
  categories_.push_back("dog");
  categories_.push_back("frog");
  categories_.push_back("horse");
  categories_.push_back("ship");
  categories_.push_back("truck");

  // init!!!
  iteration_ = 1;
  batch_size_ = 100;
  lambda_ = 0.5;
  step_ = 0.0000001;
  // init weight and scores matrices
  initRandomWeights();
  vizWeights();

  dW_.setSize(categories_.size(),
              train_images_[0].size());  // gradient update matrix

  // Qt init
  // show the first picture in dataset
  int index = 0;
  currentIndex_ = index;
  QImage img(32, 32, QImage::Format_RGB888);
  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      int red = test_images_[index][y * 32 + x];
      int green = test_images_[index][1024 + y * 32 + x];
      int blue = test_images_[index][2048 + y * 32 + x];
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }

  img = img.scaledToWidth(labelPicture->width(), Qt::SmoothTransformation);
  labelPicture->setPixmap(QPixmap::fromImage(img));
  labelPicture->show();
  lineEdit->setText(
      QString::fromUtf8(categories_[test_labels_[index]].c_str()));

  int result = inference(currentIndex_);

  lineEdit_2->setText(QString::fromUtf8(categories_[result].c_str()));
}

void softmax::updateImage() {
  if (numberBox->value() >= test_images_.size()) {
    numberBox->setValue(test_images_.size() - 1);
  }

  int index = numberBox->value();
  currentIndex_ = index;
  QImage img(32, 32, QImage::Format_RGB888);
  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      int red = test_images_[index][y * 32 + x];
      int green = test_images_[index][1024 + y * 32 + x];
      int blue = test_images_[index][2048 + y * 32 + x];
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }
  img = img.scaledToWidth(labelPicture->width(), Qt::SmoothTransformation);
  labelPicture->setPixmap(QPixmap::fromImage(img));
  labelPicture->show();
  lineEdit->setText(
      QString::fromUtf8(categories_[test_labels_[index]].c_str()));

  int result = inference(index);

  lineEdit_2->setText(QString::fromUtf8(categories_[result].c_str()));
}
int softmax::inference(int test_picture_index) {
  std::vector<float> score(10, 0);

  for (int p = 0; p < test_images_[test_picture_index].size(); p++) {
    score[0] += W_test_(0, p) * test_images_[test_picture_index][p];
    score[1] += W_test_(1, p) * test_images_[test_picture_index][p];
    score[2] += W_test_(2, p) * test_images_[test_picture_index][p];
    score[3] += W_test_(3, p) * test_images_[test_picture_index][p];
    score[4] += W_test_(4, p) * test_images_[test_picture_index][p];
    score[5] += W_test_(5, p) * test_images_[test_picture_index][p];
    score[6] += W_test_(6, p) * test_images_[test_picture_index][p];
    score[7] += W_test_(7, p) * test_images_[test_picture_index][p];
    score[8] += W_test_(8, p) * test_images_[test_picture_index][p];
    score[9] += W_test_(9, p) * test_images_[test_picture_index][p];
  }

  return std::max_element(score.begin(), score.end()) - score.begin();
}

void softmax::calculatePerformance() {
  float acc = 0;
  for (int i = 0; i < test_labels_.size(); i++) {
    int prediction = inference(i);

    acc += prediction == test_labels_[i];
  }
  progressBar->setValue(0);
  acc = acc / test_labels_.size();
  std::cout << "Acc: " << acc << "\n";
  accResult->setText(QString::number(acc));
}

bool softmax::trainSetread(const char *dirname) {
  train_images_.clear();
  train_labels_.clear();
  QDir dir(dirname);

  if (!dir.exists()) {
    std::cerr << "Folder " << dirname << " doesn't exist." << std::endl;
    return false;
  }

  dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
  QStringList filters;
  filters << "data_batch_1.bin";
  filters << "data_batch_2.bin";
  filters << "data_batch_3.bin";
  filters << "data_batch_4.bin";
  filters << "data_batch_5.bin";
  dir.setNameFilters(filters);

  foreach (QFileInfo mitm, dir.entryInfoList()) {
    std::cout << "Reading " << mitm.absoluteFilePath().toUtf8().constData()
              << std::endl;

    std::ifstream file(mitm.absoluteFilePath().toUtf8().constData(),
                       std::ios::binary);

    if (!file.is_open()) {
      return false;
    }

    int number_of_images = 10000;
    int n_rows = 32;
    int n_cols = 32;
    for (int i = 0; i < number_of_images; ++i) {
      // read label for the image
      unsigned char tplabel = 0;
      file.read((char *)&tplabel, sizeof(tplabel));
      // push to the vector of labels
      train_labels_.push_back((int)tplabel);

      std::vector<float> picture;
      for (int channel = 0; channel < 3; ++channel) {
        for (int x = 0; x < n_rows; ++x) {
          for (int y = 0; y < n_cols; ++y) {
            unsigned char temp = 0;
            file.read((char *)&temp, sizeof(temp));
            picture.push_back((int)temp);
          }
        }
      }
      picture.push_back(1);  // bias trick
      train_images_.push_back(picture);
    }
    file.close();
  }

  return true;
}

bool softmax::testSetread(const char *dirname) {
  test_labels_.clear();
  test_images_.clear();
  QDir dir(dirname);

  if (!dir.exists()) {
    std::cerr << "Folder " << dirname << " doesn't exist." << std::endl;
    return false;
  }

  dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
  QStringList filters;
  filters << "test_batch.bin";
  dir.setNameFilters(filters);

  foreach (QFileInfo mitm, dir.entryInfoList()) {
    std::cout << "Reading " << mitm.absoluteFilePath().toUtf8().constData()
              << std::endl;

    std::ifstream file(mitm.absoluteFilePath().toUtf8().constData(),
                       std::ios::binary);

    if (!file.is_open()) {
      return false;
    }

    int number_of_images = 10000;
    int n_rows = 32;
    int n_cols = 32;
    for (int i = 0; i < number_of_images; ++i) {
      // read label for the image
      unsigned char tplabel = 0;
      file.read((char *)&tplabel, sizeof(tplabel));
      // push to the vector of labels
      test_labels_.push_back((int)tplabel);

      std::vector<float> picture;
      for (int channel = 0; channel < 3; ++channel) {
        for (int x = 0; x < n_rows; ++x) {
          for (int y = 0; y < n_cols; ++y) {
            unsigned char temp = 0;
            file.read((char *)&temp, sizeof(temp));
            picture.push_back((int)temp);
          }
        }
      }
      test_images_.push_back(picture);
    }
    file.close();
  }

  return true;
}

void softmax::open() {
  QString folder_path =
      QFileDialog::getExistingDirectory(this, tr("Load CIFAR dataset"), "");
  // if(images.size()!=0){
  //          std::cout<<"Dataset already uploaded"<<"\n";
  //      return;
  //      }
  if (trainSetread(folder_path.toUtf8().constData()) &&
      testSetread(folder_path.toUtf8().constData())) {
    init();
  }
}
