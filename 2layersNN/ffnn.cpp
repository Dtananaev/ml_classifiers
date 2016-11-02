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
#include <algorithm>    // std::random_shuffle
ffnn::ffnn(QWidget *parent):  QMainWindow(parent){   
    stop_=true;
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

  connect(stopButton, SIGNAL(clicked()), this, SLOT(stopClicked()));
  connect(trainButton, SIGNAL(clicked()), this, SLOT(runTraining()));
  connect(resetWeightsButton, SIGNAL(clicked()), this, SLOT(resetWeights()));
  connect(zeroMeanButton, SIGNAL(clicked()), this, SLOT(zeroMeanData()));
  connect(standardizationButton, SIGNAL(clicked()), this, SLOT(standartisation()));
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


ffnn::~ffnn(){
}



void ffnn::stopClicked(){
 stop_=true;
}


void ffnn::weight2image(CMatrix<float> w, int label, QImage &img){
 for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
        int red=w(label,y*32+x);
        int green=w(label,1024+y*32+x);
        int blue=w(label,2048+y*32+x);
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }

}


void ffnn::vizWeights(){

   //create 10 image for weights
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

CMatrix<float> w=W1_;
    w.normalize(0,255);
   weight2image(w,0, img0);
   weight2image(w,1, img1); 
   weight2image(w,2, img2); 
   weight2image(w,3, img3);
   weight2image(w,4, img4);
   weight2image(w,5, img5);
   weight2image(w,6, img6);
   weight2image(w,7, img7);
   weight2image(w,8, img8);
   weight2image(w,9, img9);

   img0=img0.scaledToWidth(airplaneLbl1->width(), Qt::SmoothTransformation);
   airplaneLbl1->setPixmap(QPixmap::fromImage(img0));
   airplaneLbl1->show();


   img1=img1.scaledToWidth(automobilLbl2->width(), Qt::SmoothTransformation);
   automobilLbl2->setPixmap(QPixmap::fromImage(img1));
   automobilLbl2->show();

   img2=img2.scaledToWidth(birdLbl3->width(), Qt::SmoothTransformation);
   birdLbl3->setPixmap(QPixmap::fromImage(img2));
   birdLbl3->show();

   img2=img2.scaledToWidth(birdLbl3->width(), Qt::SmoothTransformation);
   birdLbl3->setPixmap(QPixmap::fromImage(img2));
   birdLbl3->show();

   img3=img3.scaledToWidth(catLbl4->width(), Qt::SmoothTransformation);
   catLbl4->setPixmap(QPixmap::fromImage(img3));
   catLbl4->show();

   img4=img4.scaledToWidth(deerLbl5->width(), Qt::SmoothTransformation);
   deerLbl5->setPixmap(QPixmap::fromImage(img4));
   deerLbl5->show();

   img5=img5.scaledToWidth(dogLbl6->width(), Qt::SmoothTransformation);
   dogLbl6->setPixmap(QPixmap::fromImage(img5));
   dogLbl6->show();

   img6=img6.scaledToWidth(frogLbl7->width(), Qt::SmoothTransformation);
   frogLbl7->setPixmap(QPixmap::fromImage(img6));
   frogLbl7->show();


   img7=img7.scaledToWidth(horseLbl8->width(), Qt::SmoothTransformation);
   horseLbl8->setPixmap(QPixmap::fromImage(img7));
   horseLbl8->show();

   img8=img8.scaledToWidth(shipLbl9->width(), Qt::SmoothTransformation);
   shipLbl9->setPixmap(QPixmap::fromImage(img8));
   shipLbl9->show();


   img9=img9.scaledToWidth(truckLbl10->width(), Qt::SmoothTransformation);
   truckLbl10->setPixmap(QPixmap::fromImage(img9));
   truckLbl10->show();


}

void ffnn::updatIterationNumber(){
    iteration_=itBox->value();
}

void ffnn::updatBatchNumber(){
    batch_size_=batchBox->value();
}

void ffnn::updateStep(){
    step_=stepBox->value();
}
void ffnn::updateLambda(){

    lambda_=lambdaBox->value();
}


void ffnn::iterate(int iter, int batch){

      progressBar->setValue(0);
  int number_of_full_batches= train_images.size()/batch;
   int last_batch= train_images.size()-number_of_full_batches*batch;


    CMatrix<float> score1(batch, W1_.xSize(),0);
    CMatrix<float> relu(score1.xSize(),score1.ySize()+1,0);
    CMatrix<float> score2(relu.xSize(), W2_.xSize(),0);

    for(int i=0; i<iter;i++){

     std::random_shuffle ( index_.begin(), index_.end() );//random batch sampling

         itResult->setText(QString::number(i+1) +"/"+QString::number(iter) );
         progressBar->setValue(0);  
        for(int j=0;j<number_of_full_batches;j++){    
                               if(stop_){return;}
                
               int progress = 100*j/number_of_full_batches;
                progressBar->setValue(progress);       

             int from=j*batch;
             int until=j*batch+batch-1;    
             calculateScoresLayer1( from,  until, score1 );
             ReLU(score1, relu);
             calculateScoresLayer2(relu,score2);
             softmaxLoss(score2,relu,  from,  until);
             updateWeights();
            lossResult->setText(QString::number(Loss_));
    vizWeights();
        } 
    

    calculatePerformance();
    }

       progressBar->setValue(100);
}


void ffnn::runTraining(){
    stop_=false;
/*
    int from = 0; 
    int until =1000;
    int batch= until-from;
   
    for(int i=0;i<iteration_;i++){
     QCoreApplication::processEvents();
           if(stop_){return;}
    calculateScoresLayer1( from,  until, score1 );
    ReLU(score1, relu);
    calculateScoresLayer2(relu,score2);
    softmaxLoss(score2,relu,  from,  until);
    updateWeights();
}
*/
    iterate(iteration_, batch_size_);
}

void ffnn::zeroMeanData(){
meanLabel->setText(QString::fromUtf8("Wait..."));
//compute the mean image

CMatrix<float> red(32,32,0);
CMatrix<float> green(32,32,0);
CMatrix<float> blue(32,32,0);

for(int i=0; i<train_images.size();i++){
for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
         red(x,y)+=train_images[i][y*32+x];
         green(x,y)+=train_images[i][1024+y*32+x];
         blue(x,y)+=train_images[i][2048+y*32+x];
    }
  }
    
}

   for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
         red(x,y)=red(x,y)/50000;
         green(x,y)=green(x,y)/50000;
         blue(x,y)=blue(x,y)/50000;
    }
  }


for(int i=0; i<train_images.size();i++){
for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
         train_images[i][y*32+x]=(train_images[i][y*32+x]-red(x,y));
        train_images[i][1024+y*32+x]=(train_images[i][1024+y*32+x]-green(x,y));
        train_images[i][2048+y*32+x]=(train_images[i][2048+y*32+x]-blue(x,y));
    }
  }


}

for(int i=0; i<test_images.size();i++){
for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
         test_images[i][y*32+x]=(test_images[i][y*32+x]-red(x,y));
        test_images[i][1024+y*32+x]=(test_images[i][1024+y*32+x]-green(x,y));
        test_images[i][2048+y*32+x]=(test_images[i][2048+y*32+x]-blue(x,y));
    }
  }


}
meanLabel->setText(QString::fromUtf8("Zero Mean dataset."));
    std::cout<<"Make dataset zero mean"<<"\n"; 
   zeroMeanButton->setEnabled(false); 
}


void ffnn::standartisation(){
meanLabel->setText(QString::fromUtf8("Wait..."));
//zero mean and sigma from  1
//compute the mean image
CMatrix<float> red(32,32,0);
CMatrix<float> green(32,32,0);
CMatrix<float> blue(32,32,0);

for(int i=0; i<train_images.size();i++){
for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
         red(x,y)+=train_images[i][y*32+x];
         green(x,y)+=train_images[i][1024+y*32+x];
         blue(x,y)+=train_images[i][2048+y*32+x];
    }
  }
    
}

   for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
         red(x,y)=red(x,y)/50000;
         green(x,y)=green(x,y)/50000;
         blue(x,y)=blue(x,y)/50000;
    }
  }

//compute the sigma 
CMatrix<double> redSigma(32,32,0);
CMatrix<double> greenSigma(32,32,0);
CMatrix<double> blueSigma(32,32,0);

for(int i=0; i<train_images.size();i++){
for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
         redSigma(x,y)+=(train_images[i][y*32+x]- red(x,y))*(train_images[i][y*32+x]- red(x,y));
         greenSigma(x,y)+=(train_images[i][1024+y*32+x]-green(x,y))*(train_images[i][1024+y*32+x]-green(x,y));
         blueSigma(x,y)+=(train_images[i][2048+y*32+x]-blue(x,y))*(train_images[i][2048+y*32+x]-blue(x,y));
    }
  }
    
}

   for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
         redSigma(x,y)=sqrt( redSigma(x,y)/49999);
         greenSigma(x,y)=sqrt( greenSigma(x,y)/49999);
         blueSigma(x,y)=sqrt( blueSigma(x,y)/49999);
    }
  }


//normalization
for(int i=0; i<train_images.size();i++){
for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
        train_images[i][y*32+x]=(train_images[i][y*32+x]-red(x,y))/ redSigma(x,y);
        train_images[i][1024+y*32+x]=(train_images[i][1024+y*32+x]-green(x,y))/ greenSigma(x,y);
        train_images[i][2048+y*32+x]=(train_images[i][2048+y*32+x]-blue(x,y))/  blueSigma(x,y);
    }
  }


}

for(int i=0; i<test_images.size();i++){
for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
        test_images[i][y*32+x]=(test_images[i][y*32+x]-red(x,y))/ redSigma(x,y);
        test_images[i][1024+y*32+x]=(test_images[i][1024+y*32+x]-green(x,y))/greenSigma(x,y);
        test_images[i][2048+y*32+x]=(test_images[i][2048+y*32+x]-blue(x,y))/  blueSigma(x,y);
    }
  }


}

meanLabel->setText(QString::fromUtf8("Standardized  dataset."));
    std::cout<<"Make dataset Standardized"<<"\n"; 
   zeroMeanButton->setEnabled(false); 
   standardizationButton->setEnabled(false); 

}

//put the values zero mean and from at the intervall -1 to 1 y = (ymax-ymin)*(x-xmin)/(xmax-xmin) + ymin;
void ffnn::normalization(){


for(int i=0; i<train_images.size();i++){
    float minR=0;
    float minG=0;
    float minB=0;
    float maxR=0;
    float maxG=0;
    float maxB=0;
for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {

         if( train_images[i][y*32+x]>maxR ){ maxR=train_images[i][y*32+x]; }
         if(train_images[i][y*32+x]<minR){minR=train_images[i][y*32+x];}

         if(train_images[i][1024+y*32+x]>maxG){maxG=train_images[i][1024+y*32+x];}
         if(train_images[i][1024+y*32+x]<minG){minG=train_images[i][1024+y*32+x];}

         if(train_images[i][2048+y*32+x]>maxB){maxB=train_images[i][2048+y*32+x];}
         if(train_images[i][2048+y*32+x]<minB){minB=train_images[i][2048+y*32+x];}
    }
  }
    


        for (int x = 0; x < 32; ++x) {
            for (int y = 0; y < 32; ++y) {
        train_images[i][y*32+x]=2*(train_images[i][y*32+x] -minR)/(maxR-minR)-1;
        train_images[i][1024+y*32+x]=2*(train_images[i][1024+y*32+x] -minG)/(maxG-minG)-1;
        train_images[i][2048+y*32+x]=2*(train_images[i][2048+y*32+x] -minB)/(maxB-minB)-1;    
    }   

  }
}


for(int i=0; i<test_images.size();i++){
    float minR=0;
    float minG=0;
    float minB=0;
    float maxR=0;
    float maxG=0;
    float maxB=0;
for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {

         if(test_images[i][y*32+x]>maxR){maxR=test_images[i][y*32+x];}
         if(test_images[i][y*32+x]<minR){minR=test_images[i][y*32+x];}

         if(test_images[i][1024+y*32+x]>maxG){maxG=test_images[i][1024+y*32+x];}
         if(test_images[i][1024+y*32+x]<minG){minG=test_images[i][1024+y*32+x];}

         if(test_images[i][2048+y*32+x]>maxB){maxB=test_images[i][2048+y*32+x];}
         if(test_images[i][2048+y*32+x]<minB){minB=test_images[i][2048+y*32+x];}
    }
  }
    


        for (int x = 0; x < 32; ++x) {
            for (int y = 0; y < 32; ++y) {
        test_images[i][y*32+x]=2*(test_images[i][y*32+x] -minR)/(maxR-minR)-1;
        test_images[i][1024+y*32+x]=2*(test_images[i][1024+y*32+x] -minG)/(maxG-minG)-1;
        test_images[i][2048+y*32+x]=2*(test_images[i][2048+y*32+x] -minB)/(maxB-minB)-1;  
    }   

  }
}
 



meanLabel->setText(QString::fromUtf8("Normalized dataset."));
    std::cout<<"Make dataset normalized"<<"\n"; 
  // zeroMeanButton->setEnabled(false); 
   normalizationButton->setEnabled(false); 
}





void ffnn::resetWeights(){

  initRandomWeights();
    vizWeights();
    int result = inference(currentIndex_);
    lineEdit_2->setText(QString::fromUtf8(categories[result].c_str()));

}

void ffnn::updateWeights(){
float weight;
float dw;   
float scale1=0;
float scale2=0;

 for(int x=0; x<dW1_.xSize();x++){
       weight=0;
       dw=0;
     for(int y=0; y<dW1_.ySize();y++){
                   if(stop_){return;}
        //calculate the scale of weights and weight update
        weight+=W1_(x,y)*W1_(x,y);
         dw+= step_*dW1_(x,y)*step_*dW1_(x,y); 
       W1_(x,y)-= step_*dW1_(x,y);
       //  std::cout<<"dW1["<<x<<"]["<<y<<"]="<<dW1_(x,y)<<"  W1["<<x<<"]["<<y<<"]="<<W1_(x,y)<<"\n";
 
        }
    
    }
      // std::cin.get();

    float a=sqrt(dw)/sqrt(weight);
            weightRelation->setText(QString::number(a));
 for(int x=0; x<dW2_.xSize();x++){
      // weight=0;
       //dw=0;
     for(int y=0; y<dW2_.ySize();y++){
                   if(stop_){return;}
        //calculate the scale of weights and weight update
      //  weight+=W2_(x,y)*W2_(x,y);
        // dw+= step_*dW1_(x,y)*step_*dW1_(x,y); 
       W2_(x,y)-= step_*dW2_(x,y);
       // std::cout<<"dW2["<<x<<"]["<<y<<"]="<<dW2_(x,y)<<"  W2["<<x<<"]["<<y<<"]="<<W2_(x,y)<<"\n";
 
        }
       // scale1
    
    }
    //  std::cin.get();

}



void ffnn::initRandomWeights(){
//Layer 1 weights
  //weight matrix for the first layer size:  L1_neurons_ x 32x32x3 +1 
  //should be  30x3073 if L1_neurons_=30
     W1_.setSize( L1_neurons_,train_images[0].size());
//Layer 2 weights
  //weight matrix for the second layer size:  L2_neurons_ x L1_neurons_ +1 
  //should be  10x31 if L1_neurons_=30
     W2_.setSize( L2_neurons_,L1_neurons_+1);//weight matrix


 std::default_random_engine generator;
  std::normal_distribution<double> distribution(0,0.001);

    for(int x=0; x<W1_.xSize();x++){
      for(int y=0;y<W1_.ySize();y++){         
            W1_(x,y)=distribution(generator);
            if(y==W1_.ySize()-1){ //make the weight for bias positive (better for initialization with relu)
                W1_(x,y)=fabs( W1_(x,y));
            }                   
        }
    }

    for(int x=0; x<W2_.xSize();x++){
      for(int y=0;y<W2_.ySize();y++){         
            W2_(x,y)=distribution(generator);
            if(y==W2_.ySize()-1){ //make the weight for bias positive (better for initialization with relu)
                W2_(x,y)=fabs( W2_(x,y));
            }                   
        }
    }

}

void ffnn::init(){
//number 

   L1_neurons_ = 70;//number of neurons in hidden layer
   L2_neurons_ = 10; //number of outputs should be 10 for CIFAR 10


//add indices from 0 to 49 999 to the index_ vector for random batch sampling
for(int i=0;i<50000;i++){
    index_.push_back(i);
}
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
// init!!!
    iteration_=1;
    batch_size_=100;
    lambda_=0.5;
    step_=0.00000000001;
 //init weight and scores matrices
   initRandomWeights();
    vizWeights();

    dW1_.setSize( L1_neurons_,train_images[0].size()); //gradient update matrix
    dW2_.setSize(L2_neurons_,L1_neurons_+1);

//Qt init
  //show the first picture in dataset
  int index=0;
  currentIndex_=index;
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

    int result = inference(currentIndex_);
  
   lineEdit_2->setText(QString::fromUtf8(categories[result].c_str()));
}


void ffnn::updateImage(){
    if( numberBox->value()>=test_images.size()){

        numberBox->setValue(test_images.size()-1);
    }

  int index=numberBox->value();
  currentIndex_=index;
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

    int result = inference(index);
   

    lineEdit_2->setText(QString::fromUtf8(categories[result].c_str()));
}

void ffnn::calculatePerformance(){

     float acc=0;
 for(int i=0; i<test_labels.size();i++){
          int prediction=inference(i);
          acc+= prediction == test_labels[i];
    }
    acc=acc/test_labels.size();
std::cout<<"Acc: "<<acc<<"\n";
accResult->setText(QString::number(acc));


}

bool ffnn::trainSetread(const char* dirname){
train_images.clear();
train_labels.clear();
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

            std::vector<float> picture;
            for(int channel = 0; channel < 3; ++channel){
                for(int x = 0; x < n_rows; ++x){
                    for(int y = 0; y < n_cols; ++y){
                       unsigned  char temp = 0;
                       file.read((char*) &temp, sizeof(temp));
                       picture.push_back((int)temp);
                    }
                }
            }
            picture.push_back(1); //bias trick
            train_images.push_back(picture);
        }
        file.close();
    }
    
    return true;
}


bool ffnn::testSetread(const char* dirname){
test_labels.clear();
test_images.clear();
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

            std::vector<float> picture;
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

void ffnn::open(){
    QString folder_path = QFileDialog::getExistingDirectory(this, tr("Load CIFAR dataset"), "");

    if( trainSetread(folder_path.toUtf8().constData())&& testSetread(folder_path.toUtf8().constData())){      
            init();
       } 


}




//neural network functions

void ffnn::calculateScoresLayer1(int from, int until, CMatrix<float>  &result){
    int batch=until - from;

  //init score matrix
    result.fill(0);
    //CMatrix<float> result(batch, W1_.xSize(),0);

    //for each image in batch compute the cost:
    for(int i=from; i<until; i++){
            int a=i-from; 
        for(int p=0;p<train_images[index_[i]].size();p++){
             
            for (int u=0; u<W1_.xSize();u++){            
                result(a,u)+= W1_(u,p)*train_images[index_[i]][p];

            }

         
        }
//for (int u=0; u<W1_.xSize();u++){            
              //    std::cout<<"score1 "<< result(a,u)<<"\n";
             //   std::cin.get(); 
   //         }
                    
    }
 // return result;
}
void ffnn::ReLU(CMatrix<float> scores, CMatrix<float> &result){

  //CMatrix<float> result(scores.xSize(),scores.ySize()+1);
result.fill(0);
    for(int x=0;x<scores.xSize();x++){//batch
        for(int y=0;y<scores.ySize();y++){//scores
           result(x,y)=std::max((float)0,scores(x,y));

        }
        result(x,scores.ySize())=1;//add bias
    }

  //  return result;
}

void ffnn::calculateScoresLayer2(CMatrix<float> data, CMatrix<float> &result){
 normalizer_.clear();
    result.fill(0);

    //compute the cost
        for(int i=0;i<data.xSize();i++){//for all batch
                 int max_score=0;//normalization of the values in order to avoid numerical issues with softmax loss
            for(int p=0;p<data.ySize();p++){ //for each output vector from previous layer
                            
                 for(int u=0; u<W2_.xSize(); u++){

                    result(i,u)+=W2_(u,p)* data(i,p);   
                     if(result(i,u)>max_score){
                    max_score=result(i,u);
                }
            }
 
        }
            float normalizer=0;
              for (int u=0; u<W2_.xSize(); u++){            
            result(i,u) = exp(result(i,u)- max_score);


              normalizer+=result(i,u);
            }


            normalizer_.push_back(normalizer);  


    }

    //return result;

}

void ffnn::softmaxLoss(CMatrix<float> scores,CMatrix<float> relu, int from, int until){
    dW1_.fill(0); //reset grad update matrix
    dW2_.fill(0); //reset grad update matrix
    loss_.clear(); //compute loss for each image
    int N=until-from; //number of training images in batch
   //compute the loss
     //for all batch   
    for(int i=from; i<until; i++){
         QCoreApplication::processEvents();
           if(stop_){return;}
                  int a=i-from;

         float y=scores(a,train_labels[index_[i]]);//right  exp(score)
           //compute  softmax loss
            float softmax=y/normalizer_[a];
            loss_.push_back(-log(softmax));


    CMatrix<float> d2_propagate(dW2_.xSize(),dW2_.ySize(),0);
        //update weights for the layer 2

         for(int dx=0; dx<dW2_.xSize();dx++){//10
                for(int dy=0; dy<dW2_.ySize();dy++){//31
                               
                 if(train_labels[index_[i]]==dx){ //if this is correct label row of weights
                          
                  dW2_(dx,dy)+=relu(a,dy)*(softmax-1)/N;
                  d2_propagate(dx,dy)=  (softmax-1);     
                }else if(train_labels[index_[i]]!=dx){    

                     dW2_(dx,dy)+= relu(a,dy)*scores(a,dx)/N + lambda_*W2_(dx,dy); 
                     d2_propagate(dx,dy)= scores(a,dx)/normalizer_[a];             
                }
          //             std::cout<<"dW2["<<dx<<"]["<<dy<<"]="<<dW2_(dx,dy)<<"\n";
          //  std::cin.get();                  
            }

     }

     //backward propagation of the dW2_  for each image until layer 1

     //gradient before ReLU   
    std::vector<float> grad(dW2_.ySize()-1,0);
     for(int dy=0; dy<dW2_.ySize()-1;dy++){//31
                      //  std::cout<<"dy "<<dy<<"\n";
                      //  std::cout<<"dx "<<dW2_.xSize()<<"\n";
            float result=0;
            //gradient before ReLU   
            for(int dx=0; dx<dW2_.xSize();dx++){//10
                       //result+=  d2_propagate(dx,dy); 

                        result+=  d2_propagate(dx,dy)*W2_(dx,dy); 

                }
               // std::cout<<"result "<<result<<"\n";
               // std::cin.get();
             //gradient afrer ReLU 
               if(relu(a,dy)>0){ 
                    grad[dy]=result;
            }
        }

  
   //update weights for the layer 1
         for(int dx=0; dx<dW1_.xSize();dx++){//30
                for(int dy=0; dy<dW1_.ySize();dy++){//3073                                           
                         
                dW1_(dx,dy)+=train_images[index_[i]][dy]*grad[dx]/N +lambda_*W1_(dx,dy);
           //                                std::cout<<"dW1["<<dx<<"]["<<dy<<"]="<<dW1_(dx,dy)<<"\n";
           // std::cin.get();
            }
 
     }  
        

}


   float Loss = std::accumulate(loss_.begin(), loss_.end(), 0.0);

   float regularization=0;
        for(int y=0;y<W1_.ySize();y++){
            for(int x=0;x<W1_.xSize();x++){
               regularization+= W1_(x,y)*W1_(x,y);

            }           

        }

        for(int y=0;y<W2_.ySize();y++){
            for(int x=0;x<W2_.xSize();x++){
               regularization+= W2_(x,y)*W2_(x,y);

            }           

        }

        regularization=0.5*lambda_*regularization;
        Loss_=Loss/ loss_.size() +regularization;

   // std::cout<<"loss "<<Loss_<<"\n";
}


int ffnn::inference(int test_picture_index){

  std::vector<float> score(10,0);  

    //calculate scores1  
    std::vector<float> score1(L1_neurons_,0);
 //   std::cout<<"W1_sizex "<<W1_.xSize()<<"\n";
 //  std::cout<<"W1_sizey "<<W1_.ySize()<<"\n";
        for(int p=0;p<test_images[test_picture_index].size();p++){           
            for (int u=0; u<W1_.xSize();u++){            
                score1[u]+= W1_(u,p)*test_images[test_picture_index][p];
            }         
        }


    //relu
    std::vector<float> relu(L1_neurons_,0);
    for(int x=0;x<L1_neurons_;x++){//batch
           relu[x]=std::max((float)0,score1[x]);
        }
        relu.push_back(1); //bias

  for(int p=0;p<relu.size();p++){ //for each output vector from previous layer
                            
                 for(int u=0; u<W2_.xSize(); u++){

                 score[u]+=W2_(u,p)* relu[p];   
        
                }
            }
      


 return  std::max_element(score.begin(), score.end())- score.begin();
}



