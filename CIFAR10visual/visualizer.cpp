/*
 * File: visualizer.cpp
 *
 * Author: Denis Tananaev
 *
 * Date: 24.10.2016
 */

#include "visualizer.h"
#include "ui_visualizer.h"

visualizer::visualizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::visualizer)
{
    ui->setupUi(this);


//   QImage myImage(Data,32,32,QImage::Format_RGB16);
 //  QLabel myLabel;
 //  myLabel.setPixmap(QPixmap::fromImage(myImage));
}



visualizer::~visualizer()
{
    delete ui;
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
            std::vector<uchar> picture; 
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
