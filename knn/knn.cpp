/*
 * File: knn.cpp
 *
 * Author: Denis Tananaev
 *
 * Date: 24.10.2016
 */

#include "knn.h"
#include <limits.h>  //INT_MAX
#include <omp.h>
knn::knn(QWidget *parent) : QMainWindow(parent) {
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
  connect(kBox, SIGNAL(valueChanged(int)), this, SLOT(updateK()));
  connect(L1, SIGNAL(clicked()), this, SLOT(updateL1()));
  connect(L2, SIGNAL(clicked()), this, SLOT(updateL2()));

  connect(evalButton, SIGNAL(clicked()), this, SLOT(calculatePerformance()));
}

knn::~knn() {}

void knn::init() {
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

  // show the first picture in the dataset
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
  // knn recognition
  int knn_label = knnDistance(index);
  lineEdit_2->setText(QString::fromUtf8(categories_[knn_label].c_str()));
}

void knn::updateK() {
  k_ = kBox->value();
  // knn recognition
  int knn_label = knnDistance(currentIndex_);
  lineEdit_2->setText(QString::fromUtf8(categories_[knn_label].c_str()));
}

void knn::updateL1() {
  if (L1->isChecked()) {
    int knn_label = knnDistance(currentIndex_);
    lineEdit_2->setText(QString::fromUtf8(categories_[knn_label].c_str()));
  }
}

void knn::updateL2() {
  if (L2->isChecked()) {
    int knn_label = knnDistance(currentIndex_);
    lineEdit_2->setText(QString::fromUtf8(categories_[knn_label].c_str()));
  }
}

void knn::updateImage() {
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

  // knn recognition
  int knn_label = knnDistance(index);

  lineEdit_2->setText(QString::fromUtf8(categories_[knn_label].c_str()));
}

void knn::calculatePerformance() {
  float acc = 0;
  for (int i = 0; i < 100; i++) {
    progressBar->setValue(i);
    int prediction = knnDistance(i);

    acc += prediction == test_labels_[i];
  }
  progressBar->setValue(100);

  acc = acc / 100;
  std::cout << "final acc " << acc << "\n";

  accResult->setText(QString::number(acc));
}

bool knn::trainSetread(const char *dirname) {
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
      train_images_.push_back(picture);
    }
    file.close();
  }

  return true;
}

// the function return the label of the closest pictures
int knn::knnDistance(int test_picture_index) {
  std::vector<int> distances;
  // go through all train set of images
  int number_of_images = train_images_.size();
  int n_rows = 32;
  int n_cols = 32;

  distances.resize(number_of_images);
#pragma omp parallel for
  for (int i = 0; i < number_of_images; ++i) {
    int dist = 0;
    for (int x = 0; x < n_rows; ++x) {
      for (int y = 0; y < n_cols; ++y) {
        int d_red = train_images_[i][y * 32 + x] -
                    test_images_[test_picture_index][y * 32 + x];
        int d_green = train_images_[i][1024 + y * 32 + x] -
                      test_images_[test_picture_index][1024 + y * 32 + x];
        int d_blue = train_images_[i][2048 + y * 32 + x] -
                     test_images_[test_picture_index][2048 + y * 32 + x];
        // L1
        if (L1->isChecked()) {
          dist += abs(d_red) + abs(d_green) + abs(d_blue);
        }
        // L2
        if (L2->isChecked()) {
          dist += sqrt(d_red * d_red + d_green * d_green + d_blue * d_blue);
        }
      }
    }
    distances[i] = dist;
  }

  std::vector<int> labels;
  labels.resize(k_);

  // first k distances
  for (int d = 0; d < k_; d++) {
    int min_index = std::min_element(distances.begin(), distances.end()) -
                    distances.begin();
    labels[d] = train_labels_[min_index];
    distances[min_index] =
        INT_MAX;  // replace the minimum distance with bigest value
  }

  // find mode of labels
  std::vector<int> histogram(10, 0);
  for (int i = 0; i < labels.size(); ++i) {
    histogram[labels[i]] += 1;
  }

  return std::max_element(histogram.begin(), histogram.end()) -
         histogram.begin();
}

bool knn::testSetread(const char *dirname) {
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

void knn::open() {
  QString folder_path =
      QFileDialog::getExistingDirectory(this, tr("Load CIFAR dataset"), "");
  if (trainSetread(folder_path.toUtf8().constData()) &&
      testSetread(folder_path.toUtf8().constData())) {
    init();
  }
}
