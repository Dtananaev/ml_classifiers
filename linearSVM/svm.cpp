/*
 * File: svm.cpp
 *
 * Author: Denis Tananaev
 *
 * Date: 25.10.2016
 */

#include "svm.h"
#include <random>
#include <omp.h>

svm::svm(QWidget *parent) : QMainWindow(parent) {
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
  connect(normalizationButton, SIGNAL(clicked()), this,
          SLOT(fullNormalization()));

  connect(stepBox, SIGNAL(valueChanged(double)), this, SLOT(updateStep()));
  connect(lambdaBox, SIGNAL(valueChanged(double)), this, SLOT(updateLambda()));
  connect(itBox, SIGNAL(valueChanged(int)), this, SLOT(updatIterationNumber()));
  connect(batchBox, SIGNAL(valueChanged(int)), this, SLOT(updatBatchNumber()));

  batchBox->setValue(100);
  stepBox->setRange(0.00000000000001, 1);
  stepBox->setValue(0.00000000000001);
  stepBox->setSingleStep(0.00000000000001);
  lambdaBox->setValue(0.5);
}

svm::~svm() {}

void svm::stopClicked() { stop_ = true; }

void svm::weight2image(CMatrix<float> w, int label, QImage &img) {
  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      int red = w(label, y * 32 + x);
      int green = w(label, 1024 + y * 32 + x);
      int blue = w(label, 2048 + y * 32 + x);
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }
}

void svm::vizWeights() {
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

  CMatrix<float> w = W_;
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

void svm::updatIterationNumber() { iteration_ = itBox->value(); }

void svm::updatBatchNumber() { batch_size_ = batchBox->value(); }

void svm::updateStep() { step_ = stepBox->value(); }
void svm::updateLambda() { lambda_ = lambdaBox->value(); }
void svm::runTraining() {
  stop_ = false;
  SVMiterate(iteration_, batch_size_);
}

void svm::zeroMeanData() {
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
  meanLabel->setText(QString::fromUtf8("Zero Mean dataset."));
  std::cout << "Make dataset zero mean"
            << "\n";
  zeroMeanButton->setEnabled(false);
}
void svm::CalcMeanSigma(const std::vector<double> data, double &mean,
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

void svm::fullNormalization() {
  meanLabel->setText(QString::fromUtf8("Wait..."));
  // zero mean and dispersion from -1 to 1
  // compute the mean image

  // for each image and each channel calculate the mean value
  std::vector<double> red;
  std::vector<double> green;
  std::vector<double> blue;

  for (int i = 0; i < train_images_.size(); i++) {
    red.clear();
    green.clear();
    blue.clear();

    for (int y = 0; y < 32; ++y) {
      for (int x = 0; x < 32; ++x) {
        red.push_back(train_images_[i][y * 32 + x]);
        green.push_back(train_images_[i][1024 + y * 32 + x]);
        blue.push_back(train_images_[i][2048 + y * 32 + x]);
      }
    }

    // finding the meat and covariance
    // for red
    double meanRed;
    double sigmaRed;
    CalcMeanSigma(red, meanRed, sigmaRed);
    double meanGreen;
    double sigmaGreen;
    CalcMeanSigma(green, meanGreen, sigmaGreen);
    double meanBlue;
    double sigmaBlue;
    CalcMeanSigma(blue, meanBlue, sigmaBlue);

    // Normalization (make z tranform )

    for (int i = 0; i < red.size(); i++) {
      red[i] = (red[i] - meanRed) / sigmaRed;
    }

    for (int i = 0; i < green.size(); i++) {
      green[i] = (green[i] - meanGreen) / sigmaGreen;
    }

    for (int i = 0; i < blue.size(); i++) {
      blue[i] = (blue[i] - meanBlue) / sigmaBlue;
    }

    std::vector<double> result = red;
    result.insert(result.end(), green.begin(), green.end());
    result.insert(result.end(), blue.begin(), blue.end());

    std::copy(result.begin(), result.end(), train_images_[i].begin());
  }

  meanLabel->setText(QString::fromUtf8("Normalized dataset."));
  std::cout << "Make dataset normalized"
            << "\n";
  zeroMeanButton->setEnabled(false);
  normalizationButton->setEnabled(false);
}
// calculate scores by using linear score function y=W*x for training
void svm::calculateScores(int from, int until, int batch) {
  // for each image we should have vector of the size 10 (score for each class)
  // for all train set of the size N we will have matrix Nx10 of scores (in our
  // case 50 000x10)
  // init score matrix
  score_.setSize(batch, categories_.size());  // Nx10
  score_.fill(0.0);

  // for each image in train set compute the cost:
  for (int i = from; i < until; i++) {
    for (int p = 0; p < train_images_[i].size(); p++) {
      int a = i - from;
      score_(a, 0) += W_(0, p) * train_images_[i][p];
      score_(a, 1) += W_(1, p) * train_images_[i][p];
      score_(a, 2) += W_(2, p) * train_images_[i][p];
      score_(a, 3) += W_(3, p) * train_images_[i][p];
      score_(a, 4) += W_(4, p) * train_images_[i][p];
      score_(a, 5) += W_(5, p) * train_images_[i][p];
      score_(a, 6) += W_(6, p) * train_images_[i][p];
      score_(a, 7) += W_(7, p) * train_images_[i][p];
      score_(a, 8) += W_(8, p) * train_images_[i][p];
      score_(a, 9) += W_(9, p) * train_images_[i][p];
    }
  }
}
void svm::SVMiterate(int iter, int batch) {
  progressBar->setValue(0);
  int number_of_full_batches = train_images_.size() / batch;

  int last_batch = train_images_.size() - number_of_full_batches * batch;

  for (int i = 0; i < iter; i++) {
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
      calculateScores(from, until, batch);
      SVMtraining(from, until);
      updateWeights();
      lossResult->setText(QString::number(Loss_));
      vizWeights();
    }

    calculatePerformance();
  }

  progressBar->setValue(100);
}

void svm::resetWeights() {
  // init weight and scores matrices
  initRandomWeights();
  vizWeights();
  int result = inference(currentIndex_);
  lineEdit_2->setText(QString::fromUtf8(categories_[result].c_str()));
}
void svm::updateWeights() {
  for (int x = 0; x < dW_.xSize(); x++) {
    for (int y = 0; y < dW_.ySize(); y++) {
      if (stop_) {
        return;
      }
      W_(x, y) -= step_ * dW_(x, y);
    }
  }
}
// SVM training function
void svm::SVMtraining(int from, int until) {
  dW_.fill(0);  // reset grad update matrix
  loss_.clear();  // compute loss for each image
  int N = until - from;
  for (int i = from; i < until; i++) {
    QCoreApplication::processEvents();
    if (stop_) {
      return;
    }
    int a = i - from;
    float y = score_(a, train_labels_[i]);  // right score
    std::vector<float> SVMloss(10, 0);
    // for grad update compute how many times loss more than 0
    int counter = 0;

    for (int j = 0; j < score_.ySize(); j++) {
      if (train_labels_[i] != j) {
        SVMloss[j] = std::max(0.0, score_(a, j) - y + 1.0);
        counter += (SVMloss[j] > 0);
      }
    }
    for (int dx = 0; dx < dW_.xSize(); dx++) {
      for (int dy = 0; dy < dW_.ySize(); dy++) {
        if (train_labels_[i] == dx) {  // if this is correct label row of weights

          dW_(dx, dy) += -1 * counter *
                         train_images_[i][dy];  //+lambda_*W_(dx,dy) ;//second
                                               //term is regularizer
        } else if (train_labels_[i] != dx) {
          dW_(dx, dy) +=
              (SVMloss[dx] > 0) * train_images_[i][dy];  // +lambda_*W_(dx,dy);
        }
      }
    }

    loss_.push_back(std::accumulate(SVMloss.begin(), SVMloss.end(), 0.0));
  }

  // add regularization to the gradient weights and normalize them

  for (int dx = 0; dx < dW_.xSize(); dx++) {
    for (int dy = 0; dy < dW_.ySize(); dy++) {
      dW_(dx, dy) = dW_(dx, dy) / N + lambda_ * W_(dx, dy);
    }
  }

  float Loss = std::accumulate(loss_.begin(), loss_.end(), 0.0);
  float regularization = 0;
  for (int y = 0; y < W_.ySize(); y++) {
    for (int x = 0; x < W_.xSize(); x++) {
      regularization += W_(x, y) * W_(x, y);
    }
  }

  regularization = 0.5 * lambda_ * regularization;
  Loss_ = Loss / loss_.size() + regularization;
}

void svm::initRandomWeights() {
  W_.setSize(categories_.size(), train_images_[0].size());  // weight matrix

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

void svm::init() {
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

  // SVM init!!!
  iteration_ = 1;
  batch_size_ = 100;
  lambda_ = 0.5;
  step_ = 0.00000000001;
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
  lineEdit->setText(QString::fromUtf8(categories_[test_labels_[index]].c_str()));

  int result = inference(currentIndex_);

  lineEdit_2->setText(QString::fromUtf8(categories_[result].c_str()));
}

void svm::updateImage() {
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
int svm::inference(int test_picture_index) {
  std::vector<float> score(10, 0);

  for (int p = 0; p < test_images_[test_picture_index].size(); p++) {
    score[0] += W_(0, p) * test_images_[test_picture_index][p];
    score[1] += W_(1, p) * test_images_[test_picture_index][p];
    score[2] += W_(2, p) * test_images_[test_picture_index][p];
    score[3] += W_(3, p) * test_images_[test_picture_index][p];
    score[4] += W_(4, p) * test_images_[test_picture_index][p];
    score[5] += W_(5, p) * test_images_[test_picture_index][p];
    score[6] += W_(6, p) * test_images_[test_picture_index][p];
    score[7] += W_(7, p) * test_images_[test_picture_index][p];
    score[8] += W_(8, p) * test_images_[test_picture_index][p];
    score[9] += W_(9, p) * test_images_[test_picture_index][p];
  }

  return std::max_element(score.begin(), score.end()) - score.begin();
}

void svm::calculatePerformance() {
  float acc = 0;
  for (int i = 0; i < test_labels_.size(); i++) {
    int prediction = inference(i);

    acc += prediction == test_labels_[i];
  }
  progressBar->setValue(0);
  acc = acc / test_labels_.size();
  std::cout << "final acc " << acc << "\n";
  accResult->setText(QString::number(acc));
}

bool svm::trainSetread(const char *dirname) {
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

      std::vector<int> picture;
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

bool svm::testSetread(const char *dirname) {
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

      std::vector<int> picture;
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

void svm::open() {
  QString folder_path =
      QFileDialog::getExistingDirectory(this, tr("Load CIFAR dataset"), "");
  if (trainSetread(folder_path.toUtf8().constData()) &&
      testSetread(folder_path.toUtf8().constData())) {
    init();
  }
}
