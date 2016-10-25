/*
 * File: knn.cpp
 *
 * Author: Denis Tananaev
 *
 * Date: 24.10.2016
 */

#include "knn.h"

knn::knn(QWidget *parent):  QMainWindow(parent){   
 

    // this sets up GUI
    setupUi(this);

    // File menu   
   menuBar()->setNativeMenuBar(false);// this line  is necessary for visualization otherwise menu invisible
  QAction *open = new QAction( "&Open", this);
  QAction *quit = new QAction( "&Quit", this);
  quit->setShortcut(tr("CTRL+Q"));

  QMenu *file;
  file = menuBar()->addMenu("&File");
  file->addAction(open);
  file->addSeparator();
  file->addAction(quit);
 
  connect(open, SIGNAL(triggered()), this, SLOT(open()));
  connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
  connect(numberBox, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));
}

knn::~knn(){
}

void knn::init(){
  //fill the cathegory names
  categories.push_back("airplane");
  categories.push_back("automobile");
  categories.push_back("bird");
  categories.push_back("cat");
  categories.push_back("deer");
  categories.push_back("dog");
  categories.push_back("frog");
  categories.push_back("horse");
  categories.push_back("ship");
  categories.push_back("truck");


  //show the first picture in dataset
  int index=0;
  QImage img(32, 32, QImage::Format_RGB888);
    for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
        int red=test_images[index][y*32+x];
        int green=test_images[index][1024+y*32+x];
        int blue=test_images[index][2048+y*32+x];
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }
  
    img=img.scaledToWidth(labelPicture->width(), Qt::SmoothTransformation);
   labelPicture->setPixmap(QPixmap::fromImage(img));
   labelPicture->show();
   lineEdit->setText(QString::fromUtf8(categories[test_labels[index]].c_str()));
//knn recognition
    int knn_label= knnDistance(index);
    lineEdit_2->setText(QString::fromUtf8(categories[train_labels[knn_label]].c_str()));
}

void knn::updateImage(){
    if( numberBox->value()>=test_images.size()){

        numberBox->setValue(test_images.size()-1);
    }

  int index=numberBox->value();
    
     QImage img(32, 32, QImage::Format_RGB888);
    for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
        int red=test_images[index][y*32+x];
        int green=test_images[index][1024+y*32+x];
        int blue=test_images[index][2048+y*32+x];
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }
    img=img.scaledToWidth(labelPicture->width(), Qt::SmoothTransformation);
   labelPicture->setPixmap(QPixmap::fromImage(img));
   labelPicture->show();
   lineEdit->setText(QString::fromUtf8(categories[test_labels[index]].c_str()));

//knn recognition
    int knn_label= knnDistance(index);

    lineEdit_2->setText(QString::fromUtf8(categories[train_labels[knn_label]].c_str()));
}

bool knn::trainSetread(const char* dirname){


    QDir dir(dirname);

    if(!dir.exists()) {
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

    foreach(QFileInfo mitm, dir.entryInfoList()){

        std::cout << "Reading " << mitm.absoluteFilePath().toUtf8().constData() << std::endl;

        std::ifstream file(mitm.absoluteFilePath().toUtf8().constData(), std::ios::binary);

        if (!file.is_open()){
            return false;
        }

        int number_of_images = 10000;
        int n_rows = 32;
        int n_cols = 32;
        for(int i = 0; i < number_of_images; ++i)
        {

            //read label for the image
             unsigned char tplabel = 0;
            file.read((char*) &tplabel, sizeof(tplabel));
            //push to the vector of labels
            train_labels.push_back((int)tplabel);

            std::vector<int> picture;
            for(int channel = 0; channel < 3; ++channel){
                for(int x = 0; x < n_rows; ++x){
                    for(int y = 0; y < n_cols; ++y){
                       unsigned  char temp = 0;
                       file.read((char*) &temp, sizeof(temp));
                       picture.push_back((int)temp);
                    }
                }
            }
            train_images.push_back(picture);
        }
        file.close();
    }
    
    return true;
}

//the function return the label of the closest pictures
int  knn::knnDistance(int test_picture_index){

    std::vector<int> distances;
       //go through all train set of images
        int number_of_images = train_images.size();
        int n_rows = 32;
        int n_cols = 32;

        distances.resize(number_of_images);

    for(int i = 0; i < number_of_images; ++i)
        {
            int dist=0;
                for(int x = 0; x < n_rows; ++x){
                    for(int y = 0; y < n_cols; ++y){

                       int d_red=train_images[i][y*32+x]-test_images[test_picture_index][y*32+x];
                       int d_green=train_images[i][1024+y*32+x]-test_images[test_picture_index][1024+y*32+x];
                       int d_blue=train_images[i][2048+y*32+x]-test_images[test_picture_index][2048+y*32+x];
                        //L1
                        if(L1->isChecked()){
                        dist+= abs(d_red)+abs(d_green)+abs(d_blue);

                        }                                               
                        //L2
                        if(L2->isChecked()){
                          dist+=sqrt(d_red*d_red +d_green*d_green + d_blue*d_blue);
                        }  

                    }
                }
            distances[i]=dist;
        }


  int min_index = std::min_element(distances.begin(), distances.end())- distances.begin();


return min_index;
}

bool knn::testSetread(const char* dirname){


    QDir dir(dirname);

    if(!dir.exists()) {
        std::cerr << "Folder " << dirname << " doesn't exist." << std::endl;
        return false;
    }

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QStringList filters;
    filters << "test_batch.bin";
    dir.setNameFilters(filters);

    foreach(QFileInfo mitm, dir.entryInfoList()){

        std::cout << "Reading " << mitm.absoluteFilePath().toUtf8().constData() << std::endl;

        std::ifstream file(mitm.absoluteFilePath().toUtf8().constData(), std::ios::binary);

        if (!file.is_open()){
            return false;
        }

        int number_of_images = 10000;
        int n_rows = 32;
        int n_cols = 32;
        for(int i = 0; i < number_of_images; ++i)
        {

            //read label for the image
             unsigned char tplabel = 0;
            file.read((char*) &tplabel, sizeof(tplabel));
            //push to the vector of labels
           test_labels.push_back((int)tplabel);

            std::vector<int> picture;
            for(int channel = 0; channel < 3; ++channel){
                for(int x = 0; x < n_rows; ++x){
                    for(int y = 0; y < n_cols; ++y){
                       unsigned  char temp = 0;
                       file.read((char*) &temp, sizeof(temp));
                       picture.push_back((int)temp);
                    }
                }
            }
            test_images.push_back(picture);
        }
        file.close();
    }
    
    return true;
}

void knn::open(){
    QString folder_path = QFileDialog::getExistingDirectory(this, tr("Load CIFAR dataset"), "");
   //if(images.size()!=0){
  //          std::cout<<"Dataset already uploaded"<<"\n";
  //      return;
  //      }
    if( trainSetread(folder_path.toUtf8().constData())&& testSetread(folder_path.toUtf8().constData())){      
            init();
       } 


}
