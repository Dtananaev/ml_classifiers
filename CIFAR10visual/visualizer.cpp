/*
 * File: visualizer.cpp
 *
 * Author: Denis Tananaev
 *
 * Date: 24.10.2016
 */

#include "visualizer.h"


visualizer::visualizer(QWidget *parent):  QMainWindow(parent)
    //ui(new Ui::visualizer)
{
    
        // this sets up GUI   
     setupUi(this); 
//ui->setupUi(this);

    //show picture button
    connect(showPicture, SIGNAL(clicked()), this, SLOT(showImage()));
//   QImage myImage(Data,32,32,QImage::Format_RGB16);
 //  QLabel myLabel;
 //  myLabel.setPixmap(QPixmap::fromImage(myImage));
}



visualizer::~visualizer()
{
  //  delete ui;
}
void visualizer::showImage(){
    //check the number of image in the box
      int index=numberBox->value();
       std::cout<<"index "<<index<<"\n";
   // unsigned char* Data = (unsigned char*)&images[index];

     //   for(int i=0; i<images[index].size();i++){
  //        std::cout<<"images "<<images[index][i]<<"\n";

    //   }


     QImage img(32, 32, QImage::Format_RGB888);
    for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
        int red=images[index][y*32+x];
        int green=images[index][1024+y*32+x];
        int blue=images[index][2048+y*32+x];
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }
  
   //QImage myImage(Data,32,32,QImage::Format_RGB16);
// QImage myImage((unsigned char*)&images[index],32,32,QImage::Format_RGB888);
   //QLabel labelPicture;
    img=img.scaledToWidth(labelPicture->width(), Qt::SmoothTransformation);
   labelPicture->setPixmap(QPixmap::fromImage(img));
   labelPicture->show();
}

bool visualizer::readCFAR(const char* filename){
    
    std::ifstream file(filename,std::ios::binary);

    if (file.is_open()){
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
         //   std::cout<< "vec size "<<labels.size()<<"\n";
        //    std::cout<< "label "<<(int)tplabel<<"\n";
       // std::cin.get();
            std::vector<int> picture; 
            for(int channel = 0; channel < 3; ++channel){
             
                // temporal vector 
               // std::vector<int> ch; 
                for(int x = 0; x < n_rows; ++x){
                    for(int y = 0; y < n_cols; ++y){
                       unsigned  char temp = 0;

                        file.read((char*) &temp, sizeof(temp));

                       //std::cout<< "CHAR size "<<(int)temp<<"\n";
                       picture.push_back((int)temp);
                    }
                }
               // std::cin.get();
             //  picture.push_back(ch);
            }
       images.push_back(picture);

        }

        std::cout<< "vec size final "<<labels.size()<<"\n";
       file.close();
      return true;
    }  else{ 
      return false;
    } 

}
