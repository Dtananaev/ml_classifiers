/*
 * File: ffnn.cpp
 *
 * Author: Denis Tananaev
 *
 * Date: 30.10.2016
 */

#include "ffnn.h"
#include <random>
#include <omp.h>
#include <algorithm>  // std::random_shuffle
ffnn::ffnn(QWidget *parent) : QMainWindow(parent) {
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
  stepBox->setRange(0.00000000000001, 99);
  stepBox->setValue(0.00000000000001);
  stepBox->setSingleStep(0.00000000000001);
  lambdaBox->setValue(0.5);
}

ffnn::~ffnn() {}

void ffnn::stopClicked() { stop_ = true; }

void ffnn::weight2image(CMatrix<float> w, int label, QImage &img) {
  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      int red = w(label, y * 32 + x);
      int green = w(label, 1024 + y * 32 + x);
      int blue = w(label, 2048 + y * 32 + x);
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }
}

void ffnn::vizWeights() {
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

  CMatrix<float> w = W1_;
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

  // create 10 image for weights
  QImage img01(32, 32, QImage::Format_RGB888);
  QImage img11(32, 32, QImage::Format_RGB888);
  QImage img21(32, 32, QImage::Format_RGB888);
  QImage img31(32, 32, QImage::Format_RGB888);
  QImage img41(32, 32, QImage::Format_RGB888);
  QImage img51(32, 32, QImage::Format_RGB888);
  QImage img61(32, 32, QImage::Format_RGB888);
  QImage img71(32, 32, QImage::Format_RGB888);
  QImage img81(32, 32, QImage::Format_RGB888);
  QImage img91(32, 32, QImage::Format_RGB888);
  weight2image(w, 10, img01);
  weight2image(w, 11, img11);
  weight2image(w, 12, img21);
  weight2image(w, 13, img31);
  weight2image(w, 14, img41);
  weight2image(w, 15, img51);
  weight2image(w, 16, img61);
  weight2image(w, 17, img71);
  weight2image(w, 18, img81);
  weight2image(w, 19, img91);
  img01 =
      img01.scaledToWidth(airplaneLbl1_2->width(), Qt::SmoothTransformation);
  airplaneLbl1_2->setPixmap(QPixmap::fromImage(img01));
  airplaneLbl1_2->show();
  img11 =
      img11.scaledToWidth(automobilLbl2_2->width(), Qt::SmoothTransformation);
  automobilLbl2_2->setPixmap(QPixmap::fromImage(img11));
  automobilLbl2_2->show();
  img21 = img21.scaledToWidth(birdLbl3_2->width(), Qt::SmoothTransformation);
  birdLbl3_2->setPixmap(QPixmap::fromImage(img21));
  birdLbl3_2->show();
  img31 = img31.scaledToWidth(catLbl4_2->width(), Qt::SmoothTransformation);
  catLbl4_2->setPixmap(QPixmap::fromImage(img31));
  catLbl4_2->show();
  img41 = img41.scaledToWidth(deerLbl5_2->width(), Qt::SmoothTransformation);
  deerLbl5_2->setPixmap(QPixmap::fromImage(img41));
  deerLbl5_2->show();
  img51 = img51.scaledToWidth(dogLbl6_2->width(), Qt::SmoothTransformation);
  dogLbl6_2->setPixmap(QPixmap::fromImage(img51));
  dogLbl6_2->show();
  img61 = img61.scaledToWidth(frogLbl7_2->width(), Qt::SmoothTransformation);
  frogLbl7_2->setPixmap(QPixmap::fromImage(img61));
  frogLbl7_2->show();
  img71 = img71.scaledToWidth(horseLbl8_2->width(), Qt::SmoothTransformation);
  horseLbl8_2->setPixmap(QPixmap::fromImage(img71));
  horseLbl8_2->show();
  img81 = img81.scaledToWidth(shipLbl9_2->width(), Qt::SmoothTransformation);
  shipLbl9_2->setPixmap(QPixmap::fromImage(img81));
  shipLbl9_2->show();
  img91 = img91.scaledToWidth(truckLbl10_2->width(), Qt::SmoothTransformation);
  truckLbl10_2->setPixmap(QPixmap::fromImage(img91));
  truckLbl10_2->show();

  // create 10 image for weights
  QImage img02(32, 32, QImage::Format_RGB888);
  QImage img12(32, 32, QImage::Format_RGB888);
  QImage img22(32, 32, QImage::Format_RGB888);
  QImage img32(32, 32, QImage::Format_RGB888);
  QImage img42(32, 32, QImage::Format_RGB888);
  QImage img52(32, 32, QImage::Format_RGB888);
  QImage img62(32, 32, QImage::Format_RGB888);
  QImage img72(32, 32, QImage::Format_RGB888);
  QImage img82(32, 32, QImage::Format_RGB888);
  QImage img92(32, 32, QImage::Format_RGB888);
  weight2image(w, 20, img02);
  weight2image(w, 21, img12);
  weight2image(w, 22, img22);
  weight2image(w, 23, img32);
  weight2image(w, 24, img42);
  weight2image(w, 25, img52);
  weight2image(w, 26, img62);
  weight2image(w, 27, img72);
  weight2image(w, 28, img82);
  weight2image(w, 29, img92);
  img02 =
      img02.scaledToWidth(airplaneLbl1_3->width(), Qt::SmoothTransformation);
  airplaneLbl1_3->setPixmap(QPixmap::fromImage(img02));
  airplaneLbl1_3->show();
  img12 =
      img12.scaledToWidth(automobilLbl2_3->width(), Qt::SmoothTransformation);
  automobilLbl2_3->setPixmap(QPixmap::fromImage(img12));
  automobilLbl2_3->show();
  img22 = img22.scaledToWidth(birdLbl3_3->width(), Qt::SmoothTransformation);
  birdLbl3_3->setPixmap(QPixmap::fromImage(img22));
  birdLbl3_3->show();
  img32 = img32.scaledToWidth(catLbl4_3->width(), Qt::SmoothTransformation);
  catLbl4_3->setPixmap(QPixmap::fromImage(img32));
  catLbl4_3->show();
  img42 = img42.scaledToWidth(deerLbl5_3->width(), Qt::SmoothTransformation);
  deerLbl5_3->setPixmap(QPixmap::fromImage(img42));
  deerLbl5_3->show();
  img52 = img52.scaledToWidth(dogLbl6_3->width(), Qt::SmoothTransformation);
  dogLbl6_3->setPixmap(QPixmap::fromImage(img52));
  dogLbl6_3->show();
  img62 = img62.scaledToWidth(frogLbl7_3->width(), Qt::SmoothTransformation);
  frogLbl7_3->setPixmap(QPixmap::fromImage(img62));
  frogLbl7_3->show();
  img72 = img72.scaledToWidth(horseLbl8_3->width(), Qt::SmoothTransformation);
  horseLbl8_3->setPixmap(QPixmap::fromImage(img72));
  horseLbl8_3->show();
  img82 = img82.scaledToWidth(shipLbl9_3->width(), Qt::SmoothTransformation);
  shipLbl9_3->setPixmap(QPixmap::fromImage(img82));
  shipLbl9_3->show();
  img92 = img92.scaledToWidth(truckLbl10_3->width(), Qt::SmoothTransformation);
  truckLbl10_3->setPixmap(QPixmap::fromImage(img92));
  truckLbl10_3->show();

  // create 10 image for weights
  QImage img03(32, 32, QImage::Format_RGB888);
  QImage img13(32, 32, QImage::Format_RGB888);
  QImage img23(32, 32, QImage::Format_RGB888);
  QImage img33(32, 32, QImage::Format_RGB888);
  QImage img43(32, 32, QImage::Format_RGB888);
  QImage img53(32, 32, QImage::Format_RGB888);
  QImage img63(32, 32, QImage::Format_RGB888);
  QImage img73(32, 32, QImage::Format_RGB888);
  QImage img83(32, 32, QImage::Format_RGB888);
  QImage img93(32, 32, QImage::Format_RGB888);
  weight2image(w, 30, img03);
  weight2image(w, 31, img13);
  weight2image(w, 32, img23);
  weight2image(w, 33, img33);
  weight2image(w, 34, img43);
  weight2image(w, 35, img53);
  weight2image(w, 36, img63);
  weight2image(w, 37, img73);
  weight2image(w, 38, img83);
  weight2image(w, 39, img93);
  img03 =
      img03.scaledToWidth(airplaneLbl1_4->width(), Qt::SmoothTransformation);
  airplaneLbl1_4->setPixmap(QPixmap::fromImage(img03));
  airplaneLbl1_4->show();
  img13 =
      img13.scaledToWidth(automobilLbl2_4->width(), Qt::SmoothTransformation);
  automobilLbl2_4->setPixmap(QPixmap::fromImage(img13));
  automobilLbl2_4->show();
  img23 = img23.scaledToWidth(birdLbl3_4->width(), Qt::SmoothTransformation);
  birdLbl3_4->setPixmap(QPixmap::fromImage(img23));
  birdLbl3_4->show();
  img33 = img33.scaledToWidth(catLbl4_4->width(), Qt::SmoothTransformation);
  catLbl4_4->setPixmap(QPixmap::fromImage(img33));
  catLbl4_4->show();
  img43 = img43.scaledToWidth(deerLbl5_4->width(), Qt::SmoothTransformation);
  deerLbl5_4->setPixmap(QPixmap::fromImage(img43));
  deerLbl5_4->show();
  img53 = img53.scaledToWidth(dogLbl6_4->width(), Qt::SmoothTransformation);
  dogLbl6_4->setPixmap(QPixmap::fromImage(img53));
  dogLbl6_4->show();
  img63 = img63.scaledToWidth(frogLbl7_4->width(), Qt::SmoothTransformation);
  frogLbl7_4->setPixmap(QPixmap::fromImage(img63));
  frogLbl7_4->show();
  img73 = img73.scaledToWidth(horseLbl8_4->width(), Qt::SmoothTransformation);
  horseLbl8_4->setPixmap(QPixmap::fromImage(img73));
  horseLbl8_4->show();
  img83 = img83.scaledToWidth(shipLbl9_4->width(), Qt::SmoothTransformation);
  shipLbl9_4->setPixmap(QPixmap::fromImage(img83));
  shipLbl9_4->show();
  img93 = img93.scaledToWidth(truckLbl10_4->width(), Qt::SmoothTransformation);
  truckLbl10_4->setPixmap(QPixmap::fromImage(img93));
  truckLbl10_4->show();

  // create 10 image for weights
  QImage img04(32, 32, QImage::Format_RGB888);
  QImage img14(32, 32, QImage::Format_RGB888);
  QImage img24(32, 32, QImage::Format_RGB888);
  QImage img34(32, 32, QImage::Format_RGB888);
  QImage img44(32, 32, QImage::Format_RGB888);
  QImage img54(32, 32, QImage::Format_RGB888);
  QImage img64(32, 32, QImage::Format_RGB888);
  QImage img74(32, 32, QImage::Format_RGB888);
  QImage img84(32, 32, QImage::Format_RGB888);
  QImage img94(32, 32, QImage::Format_RGB888);
  weight2image(w, 40, img04);
  weight2image(w, 41, img14);
  weight2image(w, 42, img24);
  weight2image(w, 43, img34);
  weight2image(w, 44, img44);
  weight2image(w, 45, img54);
  weight2image(w, 46, img64);
  weight2image(w, 47, img74);
  weight2image(w, 48, img84);
  weight2image(w, 49, img94);
  img04 =
      img04.scaledToWidth(airplaneLbl1_5->width(), Qt::SmoothTransformation);
  airplaneLbl1_5->setPixmap(QPixmap::fromImage(img04));
  airplaneLbl1_5->show();
  img14 =
      img14.scaledToWidth(automobilLbl2_5->width(), Qt::SmoothTransformation);
  automobilLbl2_5->setPixmap(QPixmap::fromImage(img14));
  automobilLbl2_5->show();
  img24 = img24.scaledToWidth(birdLbl3_5->width(), Qt::SmoothTransformation);
  birdLbl3_5->setPixmap(QPixmap::fromImage(img24));
  birdLbl3_5->show();
  img34 = img34.scaledToWidth(catLbl4_5->width(), Qt::SmoothTransformation);
  catLbl4_5->setPixmap(QPixmap::fromImage(img34));
  catLbl4_5->show();
  img44 = img44.scaledToWidth(deerLbl5_5->width(), Qt::SmoothTransformation);
  deerLbl5_5->setPixmap(QPixmap::fromImage(img44));
  deerLbl5_5->show();
  img54 = img54.scaledToWidth(dogLbl6_5->width(), Qt::SmoothTransformation);
  dogLbl6_5->setPixmap(QPixmap::fromImage(img54));
  dogLbl6_5->show();
  img64 = img64.scaledToWidth(frogLbl7_5->width(), Qt::SmoothTransformation);
  frogLbl7_5->setPixmap(QPixmap::fromImage(img64));
  frogLbl7_5->show();
  img74 = img74.scaledToWidth(horseLbl8_5->width(), Qt::SmoothTransformation);
  horseLbl8_5->setPixmap(QPixmap::fromImage(img74));
  horseLbl8_5->show();
  img84 = img84.scaledToWidth(shipLbl9_5->width(), Qt::SmoothTransformation);
  shipLbl9_5->setPixmap(QPixmap::fromImage(img84));
  shipLbl9_5->show();
  img94 = img94.scaledToWidth(truckLbl10_5->width(), Qt::SmoothTransformation);
  truckLbl10_5->setPixmap(QPixmap::fromImage(img94));
  truckLbl10_5->show();
}

void ffnn::updatIterationNumber() { iteration_ = itBox->value(); }

void ffnn::updatBatchNumber() { batch_size_ = batchBox->value(); }

void ffnn::updateStep() { step_ = stepBox->value(); }
void ffnn::updateLambda() { lambda_ = lambdaBox->value(); }

void ffnn::iterate(int iter, int batch) {
  progressBar->setValue(0);
  int number_of_full_batches = train_images_.size() / batch;
  int last_batch = train_images_.size() - number_of_full_batches * batch;

  CMatrix<float> score1(batch, W1_.xSize(), 0);
  CMatrix<float> relu(score1.xSize(), score1.ySize() + 1, 0);
  CMatrix<float> score2(relu.xSize(), W2_.xSize(), 0);

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
      int until = j * batch + batch - 1;
      calculateScoresLayer1(from, until, score1);
      ReLU(score1, relu);
      calculateScoresLayer2(relu, score2);
      softmaxLoss(score2, relu, from, until);
      updateWeights();
      lossResult->setText(QString::number(Loss_));
      vizWeights();
    }

    calculatePerformance();
  }

  progressBar->setValue(100);
}

void ffnn::runTraining() {
  stop_ = false;

  dW1_.setSize(L1_neurons_, train_images_[0].size());  // gradient update matrix
  dW2_.setSize(L2_neurons_, L1_neurons_ + 1);
  iterate(iteration_, batch_size_);
}

void ffnn::zeroMeanData() {
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
        train_images_[i][y * 32 + x] = (train_images_[i][y * 32 + x] - red(x, y));
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

void ffnn::standartisation() {
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
        greenSigma(x, y) += (train_images_[i][1024 + y * 32 + x] - green(x, y)) *
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
            (train_images_[i][2048 + y * 32 + x] - blue(x, y)) / blueSigma(x, y);
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
void ffnn::normalization() {
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
            2 * (train_images_[i][1024 + y * 32 + x] - minG) / (maxG - minG) - 1;
        train_images_[i][2048 + y * 32 + x] =
            2 * (train_images_[i][2048 + y * 32 + x] - minB) / (maxB - minB) - 1;
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
  normalizationButton->setEnabled(false);
}

void ffnn::resetWeights() {
  initRandomWeights();
  vizWeights();
  int result = inference(currentIndex_);
  lineEdit_2->setText(QString::fromUtf8(categories_[result].c_str()));
}

void ffnn::updateWeights() {
  float weight = 0;
  float dw = 0;
  float min = 0;
  float max = 0;
  float dmin = 0;
  float dmax = 0;

  for (int x = 0; x < dW1_.xSize(); x++) {
    for (int y = 0; y < dW1_.ySize(); y++) {
      if (stop_) {
        return;
      }
      // calculate the scale of weights and weight update
      weight += W1_(x, y) * W1_(x, y);
      dw += step_ * dW1_(x, y) * step_ * dW1_(x, y);
      W1_(x, y) -= step_ * dW1_(x, y);

      if (W1_(x, y) > max) {
        max = W1_(x, y);
      }
      if (W1_(x, y) < min) {
        min = W1_(x, y);
      }
      if (dW1_(x, y) > dmax) {
        dmax = dW1_(x, y);
      }
      if (dW1_(x, y) < dmin) {
        dmin = dW1_(x, y);
      }
    }
  }

  float a = sqrt(dw) / sqrt(weight);
  weightRelation->setText(QString::number(a));
  w1Min->setText(QString::number(min));
  w1Max->setText(QString::number(max));
  dw1Min->setText(QString::number(dmin));
  dw1Max->setText(QString::number(dmax));
  uw1Min->setText(QString::number(step_ * dmin));
  uw1Max->setText(QString::number(step_ * dmax));

  min = 0;
  max = 0;
  dmin = 0;
  dmax = 0;
  weight = 0;
  dw = 0;
  for (int x = 0; x < dW2_.xSize(); x++) {
    for (int y = 0; y < dW2_.ySize(); y++) {
      if (stop_) {
        return;
      }
      // calculate the scale of weights and weight update
      weight += W2_(x, y) * W2_(x, y);
      dw += step_ * dW2_(x, y) * step_ * dW2_(x, y);
      W2_(x, y) -= step_ * dW2_(x, y);

      if (W2_(x, y) > max) {
        max = W2_(x, y);
      }
      if (W2_(x, y) < min) {
        min = W2_(x, y);
      }
      if (dW2_(x, y) > dmax) {
        dmax = dW2_(x, y);
      }
      if (dW2_(x, y) < dmin) {
        dmin = dW2_(x, y);
      }
    }
  }
  a = sqrt(dw) / sqrt(weight);
  weightRelation2->setText(QString::number(a));
  w2Min->setText(QString::number(min));
  w2Max->setText(QString::number(max));
  dw2Min->setText(QString::number(dmin));
  dw2Max->setText(QString::number(dmax));
  uw2Min->setText(QString::number(step_ * dmin));
  uw2Max->setText(QString::number(step_ * dmax));
}

void ffnn::initRandomWeights() {
  // Layer 1 weights
  W1_.setSize(L1_neurons_, train_images_[0].size());
  // Layer 2 weights
  W2_.setSize(L2_neurons_, L1_neurons_ + 1);

  std::default_random_engine generator;
  std::normal_distribution<double> distribution(0, 0.001);

  for (int x = 0; x < W1_.xSize(); x++) {
    for (int y = 0; y < W1_.ySize(); y++) {
      W1_(x, y) = distribution(generator);
      if (y == W1_.ySize() - 1) {  // make the weight for bias positive (better
                                   // for initialization with relu)
        W1_(x, y) = fabs(W1_(x, y));
      }
    }
  }

  for (int x = 0; x < W2_.xSize(); x++) {
    for (int y = 0; y < W2_.ySize(); y++) {
      W2_(x, y) = distribution(generator);
      if (y == W2_.ySize() - 1) {  // make the weight for bias positive (better
                                   // for initialization with relu)
        W2_(x, y) = fabs(W2_(x, y));
      }
    }
  }
}

void ffnn::init() {
  // number

  L1_neurons_ = 50;  // number of neurons in hidden layer
  L2_neurons_ = 10;  // number of outputs should be 10 for CIFAR 10

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
  step_ = 0.00000000001;
  // init weight and scores matrices
  initRandomWeights();
  vizWeights();

  dW1_.setSize(L1_neurons_, train_images_[0].size());  // gradient update matrix
  dW2_.setSize(L2_neurons_, L1_neurons_ + 1);

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
  lineEdit->setText(QString::fromUtf8(categories_[test_labels_[index]].c_str()));

  int result = inference(currentIndex_);

  lineEdit_2->setText(QString::fromUtf8(categories_[result].c_str()));
}

void ffnn::updateImage() {
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
  lineEdit->setText(QString::fromUtf8(categories_[test_labels_[index]].c_str()));

  int result = inference(index);

  lineEdit_2->setText(QString::fromUtf8(categories_[result].c_str()));
}

void ffnn::calculatePerformance() {
  float acc = 0;
  for (int i = 0; i < test_labels_.size(); i++) {
    int prediction = inference(i);
    acc += prediction == test_labels_[i];
  }
  acc = acc / test_labels_.size();
  std::cout << "Acc: " << acc << "\n";
  accResult->setText(QString::number(acc));
}

bool ffnn::trainSetread(const char *dirname) {
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

bool ffnn::testSetread(const char *dirname) {
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

void ffnn::open() {
  QString folder_path =
      QFileDialog::getExistingDirectory(this, tr("Load CIFAR dataset"), "");
  if (trainSetread(folder_path.toUtf8().constData()) &&
      testSetread(folder_path.toUtf8().constData())) {
    init();
  }
}

// neural network functions

void ffnn::calculateScoresLayer1(int from, int until, CMatrix<float> &result) {
  int batch = until - from;

  // init score matrix
  result.fill(0);
  // for each image in batch compute the cost:
  for (int i = from; i < until; i++) {
    int a = i - from;
    for (int p = 0; p < train_images_[index_[i]].size(); p++) {
      for (int u = 0; u < W1_.xSize(); u++) {
        result(a, u) += W1_(u, p) * train_images_[index_[i]][p];
      }
    }
  }
}
void ffnn::ReLU(CMatrix<float> scores, CMatrix<float> &result) {
  result.fill(0);
  for (int x = 0; x < scores.xSize(); x++) {  // batch
    for (int y = 0; y < scores.ySize(); y++) {  // scores
      result(x, y) = std::max(0.01f * scores(x, y), scores(x, y));
      // result(x,y)=std::max(0.0f,scores(x,y));
    }
    result(x, scores.ySize()) = 1;  // add bias
  }
}

void ffnn::calculateScoresLayer2(CMatrix<float> data, CMatrix<float> &result) {
  normalizer_.clear();
  result.fill(0);

  // compute the cost
  for (int i = 0; i < data.xSize(); i++) {  // for all batch
    int max_score = 0;  // normalization of the values in order to avoid
                        // numerical issues with softmax loss
    for (int p = 0; p < data.ySize();
         p++) {  // for each output vector from previous layer

      for (int u = 0; u < W2_.xSize(); u++) {
        result(i, u) += W2_(u, p) * data(i, p);
        if (result(i, u) > max_score) {
          max_score = result(i, u);
        }
      }
    }
    float normalizer = 0;
    for (int u = 0; u < W2_.xSize(); u++) {
      result(i, u) = exp(result(i, u) - max_score);
      normalizer += result(i, u);
    }
    normalizer_.push_back(normalizer);
  }
}

void ffnn::softmaxLoss(CMatrix<float> scores, CMatrix<float> relu, int from,
                       int until) {
  dW1_.fill(0);  // reset grad update matrix
  dW2_.fill(0);  // reset grad update matrix
  loss_.clear();  // compute loss for each image
  int N = until - from;  // number of training images in batch
  // compute the loss
  // for all batch
  for (int i = from; i < until; i++) {
    QCoreApplication::processEvents();
    if (stop_) {
      return;
    }
    int a = i - from;

    float y = scores(a, train_labels_[index_[i]]);  // right  exp(score)
    // compute  softmax loss
    float softmax = y / normalizer_[a];
    loss_.push_back(-log(softmax));

    CMatrix<float> d2_propagate(dW2_.xSize(), dW2_.ySize(), 0);

    // update weights for the layer 2
    for (int dx = 0; dx < dW2_.xSize(); dx++) {  // 10
      for (int dy = 0; dy < dW2_.ySize(); dy++) {  // 31

        if (train_labels_[index_[i]] ==
            dx) {  // if this is correct label row of weights

          dW2_(dx, dy) += relu(a, dy) * (softmax - 1) / N;
          d2_propagate(dx, dy) = (softmax - 1);
        } else if (train_labels_[index_[i]] != dx) {
          dW2_(dx, dy) +=
              relu(a, dy) * scores(a, dx) / N + lambda_ * W2_(dx, dy);
          d2_propagate(dx, dy) = scores(a, dx) / normalizer_[a];
        }
      }
    }

    // backward propagation of the dW2_  for each image separately  (not
    // sum/N!!!) until layer 1

    // gradient before ReLU
    std::vector<float> grad(dW2_.ySize() - 1, 0);
    for (int dy = 0; dy < dW2_.ySize() - 1; dy++) {  // 31

      float result = 0;
      // gradient before ReLU
      for (int dx = 0; dx < dW2_.xSize(); dx++) {  // 10
        result += d2_propagate(dx, dy) * W2_(dx, dy);
      }

      // gradient afrer ReLU
      if (relu(a, dy) > 0) {
        grad[dy] = result;
      } else {
        grad[dy] = 0.01 * result;
      }
    }

    // update weights for the layer 1
    for (int dx = 0; dx < dW1_.xSize(); dx++) {  // 30
      for (int dy = 0; dy < dW1_.ySize(); dy++) {  // 3073
        dW1_(dx, dy) +=
            train_images_[index_[i]][dy] * grad[dx] / N + lambda_ * W1_(dx, dy);
      }
    }
  }

  float Loss = std::accumulate(loss_.begin(), loss_.end(), 0.0);

  float regularization = 0;
  for (int y = 0; y < W1_.ySize(); y++) {
    for (int x = 0; x < W1_.xSize(); x++) {
      regularization += W1_(x, y) * W1_(x, y);
    }
  }

  for (int y = 0; y < W2_.ySize(); y++) {
    for (int x = 0; x < W2_.xSize(); x++) {
      regularization += W2_(x, y) * W2_(x, y);
    }
  }

  regularization = 0.5 * lambda_ * regularization;
  Loss_ = Loss / loss_.size() + regularization;

  // std::cout<<"loss "<<Loss_<<"\n";
}

int ffnn::inference(int test_picture_index) {
  std::vector<float> score(10, 0);

  // calculate scores1
  std::vector<float> score1(L1_neurons_, 0);
  //   std::cout<<"W1_sizex "<<W1_.xSize()<<"\n";
  //  std::cout<<"W1_sizey "<<W1_.ySize()<<"\n";
  for (int p = 0; p < test_images_[test_picture_index].size(); p++) {
    for (int u = 0; u < W1_.xSize(); u++) {
      score1[u] += W1_(u, p) * test_images_[test_picture_index][p];
    }
  }

  // relu
  std::vector<float> relu(L1_neurons_, 0);
  for (int x = 0; x < L1_neurons_; x++) {  // batch
    relu[x] = std::max((float)0, score1[x]);
  }
  relu.push_back(1);  // bias

  for (int p = 0; p < relu.size();
       p++) {  // for each output vector from previous layer

    for (int u = 0; u < W2_.xSize(); u++) {
      score[u] += W2_(u, p) * relu[p];
    }
  }

  return std::max_element(score.begin(), score.end()) - score.begin();
}
