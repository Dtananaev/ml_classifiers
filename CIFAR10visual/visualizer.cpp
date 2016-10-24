/*
 * File: visualizer.cpp
 *
 * Author: Denis Tananaev
 *
 * Date: 24.10.2016
 */

#include "visualizer.h"


visualizer::visualizer(QWidget *parent):  QMainWindow(parent){    
    // this sets up GUI
    setupUi(this);
    connect(numberBox, SIGNAL(valueChanged(int)), this, SLOT(updateImage()));
}

visualizer::~visualizer(){
}

void visualizer::init(){
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
        int red=images[index][y*32+x];
        int green=images[index][1024+y*32+x];
        int blue=images[index][2048+y*32+x];
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }
  
    img=img.scaledToWidth(labelPicture->width(), Qt::SmoothTransformation);
   labelPicture->setPixmap(QPixmap::fromImage(img));
   labelPicture->show();
   lineEdit->setText(QString::fromUtf8(categories[labels[index]].c_str()));
}

void visualizer::updateImage(){
    if( numberBox->value()>=60000){

        numberBox->setValue(59999);
    }

  int index=numberBox->value();
    
     QImage img(32, 32, QImage::Format_RGB888);
    for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
        int red=images[index][y*32+x];
        int green=images[index][1024+y*32+x];
        int blue=images[index][2048+y*32+x];
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }
    img=img.scaledToWidth(labelPicture->width(), Qt::SmoothTransformation);
   labelPicture->setPixmap(QPixmap::fromImage(img));
   labelPicture->show();
   lineEdit->setText(QString::fromUtf8(categories[labels[index]].c_str()));
}

bool visualizer::readCFAR(const char* dirname){


    QDir dir(dirname);

    if(!dir.exists()) {
        std::cerr << "Folder " << dirname << " doesn't exist." << std::endl;
        return false;
    }

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QStringList filters;
    filters << "*.bin";
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
            labels.push_back((int)tplabel);

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
            images.push_back(picture);
        }
        file.close();

    }
    

    return true;
}

void visualizer::on_actionLoad_from_folder_triggered(){
    QString folder_path = QFileDialog::getExistingDirectory(this, tr("Load CIFAR dataset"), "");
  
    if(readCFAR(folder_path.toUtf8().constData())){
        init();
    }
}
