/*
 * File: svm.cpp
 *
 * Author: Denis Tananaev
 *
 * Date: 25.10.2016
 */

#include "svm.h"
#include <random>
#include <limits.h> //INT_MAX
#include <omp.h>//!!!!uncomment in case if you have installed omp libraries!!!
svm::svm(QWidget *parent):  QMainWindow(parent){   

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


  connect(evalButton, SIGNAL(clicked()), this, SLOT(calculatePerformance()));
}

svm::~svm(){
}

//calculate scores by using linear score function y=W*x
void svm::calculateScores(){

//for each image we should have vector of the size 10 (score for each class)
//for all train set of the size N we will have matrix Nx10 of scores (in our case 50 000x10)
    score_.fill(0.0);

    //for each image in train set compute the cost:
    for(int i=0; i<train_images.size(); i++){
        for(int p=0;p<train_images[i].size();p++){
            //cost(50000,10)  W(10, 3073)*image(3073,1)                     
            score_(i,0)+= W_(0,p)*train_images[i][p];
            score_(i,1)+= W_(1,p)*train_images[i][p];
            score_(i,2)+= W_(2,p)*train_images[i][p];
            score_(i,3)+= W_(3,p)*train_images[i][p];
            score_(i,4)+= W_(4,p)*train_images[i][p];
            score_(i,5)+= W_(5,p)*train_images[i][p];
            score_(i,6)+= W_(6,p)*train_images[i][p];
            score_(i,7)+= W_(7,p)*train_images[i][p];
            score_(i,8)+= W_(8,p)*train_images[i][p];
            score_(i,9)+= W_(9,p)*train_images[i][p];


        }
/*
            std::cout<<"score(0) "<<score_(i,0)<<"\n";
            std::cout<<"score(1) "<<score_(i,1)<<"\n";    
            std::cout<<"score(2) "<<score_(i,2)<<"\n";   
            std::cout<<"score(3) "<<score_(i,3)<<"\n";  
            std::cout<<"score(4) "<<score_(i,4)<<"\n";
            std::cout<<"score(5) "<<score_(i,5)<<"\n";
            std::cout<<"score(6) "<<score_(i,6)<<"\n";
            std::cout<<"score(7) "<<score_(i,7)<<"\n";
            std::cout<<"score(8) "<<score_(i,8)<<"\n";
            std::cout<<"score(9) "<<score_(i,9)<<"\n";

        std::cin.get();
*/
    }


}
void svm::SVMiterate(int iter){

    for(int i=0; i<iter;i++){
        calculateScores();
        SVMtraining();
        updateWeights();
    }

}



void svm::updateWeights(){
 for(int x=0; x<dW_.xSize();x++){
     for(int y=0; y<dW_.ySize();y++){
      //  std::cout<<"weight before update "<<W_(x,y)<<"\n";
        W_(x,y)-= step_*dW_(x,y);
      // std::cout<<"weight after update "<<W_(x,y)<<"\n";
      //  std::cin.get();
        }
    }
}
//SVM training function
void svm::SVMtraining(){

    dW_.fill(0); //reset grad update matrix
    loss_.clear(); //compute loss for each image

    for(int i=0; i<train_images.size();i++){

          float y=score_(i,train_labels[i]);//right score
          std::vector<float> score(10,0);
          //for grad update compute how many times loss more than 0
          int counter=0; 
          
        for(int j=0;j<score_.ySize();j++){                

                if(train_labels[i]!=j){

                     float temp=std::max(0.0, score_(i,j)-y +1.0) ;
            //std::cout<<"temp "<<temp<<"\n";
           
                     score[j]= temp;
                     if(temp>0){
                       counter+=1;
                    }
            }                       
          }

          //  std::cout<<"counter "<<counter<<"\n";
            //W(10, 3073)
          //  std::cin.get();

           // std::cout<<"x_size "<<dW_.xSize()<<"\n"; 
           // std::cout<<"x_size "<<dW_.ySize()<<"\n"; 
          //  std::cout<<"Wx_size "<<W_.xSize()<<"\n"; 
          //  std::cout<<"Wx_size "<<W_.ySize()<<"\n"; 
            for(int dx=0; dx<dW_.xSize();dx++){
                for(int dy=0; dy<dW_.ySize();dy++){
            
                      // std::cout<<"Wx_size "<<W_.ySize()<<"\n"; 
                 if(train_labels[i]==dx){ //if this is correct label row of weights
            
                    dW_(dx,dy)+=-1*counter*train_images[i][dy] ;//second term is regularizer 
                    //    std::cout<<"dW_(x,y) "<<dW_(x,y)<<"\n";
                   //       std::cout<<"tr image "<<train_images[i][y]<<"\n" ;
                    // std::cin.get();
                }else if(train_labels[i]!=dx){
                   // std::cout<<"score x "<<score[x]<<"\n";
                   // std::cout<<"indicator "<<(score[x]>0)<<"\n";
                    dW_(dx,dy)+= (score[dx]>0) *train_images[i][dy] ;
                   //  std::cout<<"dW_(x,y) "<<dW_(x,y)<<"\n";
                   //       std::cout<<"tr image "<<train_images[i][y]<<"\n" ;
                   //  std::cin.get();
                }
                   
                  //std::cout<<"Dw "<< dW_(x,y)<<"\n";
            //  std::cout<<"dW_["<<dx<<"]"<<"["<<dy<<"]="<<dW_(dx,dy)<<"\n";
           // std::cin.get();
            }


        }


           loss_.push_back(std::accumulate(score.begin(), score.end(), 0.0));

      

    }


   float Loss = std::accumulate(loss_.begin(), loss_.end(), 0.0);
/*
   float regularization=0;
        for(int y=0;y<W_.ySize();y++){
            for(int x=0;x<W_.xSize();x++){
               regularization+= W_(x,y)*W_(x,y);

            }           

        }
*/
       // regularization=0.5*lambda_*regularization;
    Loss=Loss/ 50000;// + regularization;
    
         //std::cout<<"regularization "<<regularization<<"\n";
        std::cout<<"Loss "<<Loss<<"\n";

}

void svm::initRandomWeights(){
  W_.setSize(categories.size(),train_images[0].size()); //weight matrix

 std::default_random_engine generator;
  std::normal_distribution<double> distribution(0,0.01);

    for(int x=0; x<W_.xSize();x++){
      for(int y=0;y<W_.ySize();y++){  
        
            W_(x,y)=distribution(generator);
            if(y==W_.ySize()-1){ //make the weight for bias positive (better for initialization)
                W_(x,y)=fabs( W_(x,y));
            }
           
           
        }
    }

}

void svm::init(){

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

//SVM init!!!
    lambda_=1;
    step_=0.00000000001;
 //init weight and scores matrices
   initRandomWeights();
    dW_.setSize(categories.size(),train_images[0].size()); //gradient update matrix
    
  //init score matrix
   score_.setSize(train_images.size(),categories.size());  // Nx10



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


  //  lineEdit_2->setText(QString::fromUtf8(categories[knn_label].c_str()));
}


void svm::updateImage(){
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


   

    //lineEdit_2->setText(QString::fromUtf8(categories[knn_label].c_str()));
}

void svm::calculatePerformance(){
SVMiterate(iteration_);
    float acc=0;

 for(int i=0; i<100;i++){

    progressBar->setValue(i);



    }
    progressBar->setValue(100);


accResult->setText(QString::number(acc));
}

bool svm::trainSetread(const char* dirname){


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
            picture.push_back(1); //bias trick
            train_images.push_back(picture);
        }
        file.close();
    }
    
    return true;
}


bool svm::testSetread(const char* dirname){


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

void svm::open(){
    QString folder_path = QFileDialog::getExistingDirectory(this, tr("Load CIFAR dataset"), "");
   //if(images.size()!=0){
  //          std::cout<<"Dataset already uploaded"<<"\n";
  //      return;
  //      }
    if( trainSetread(folder_path.toUtf8().constData())&& testSetread(folder_path.toUtf8().constData())){      
            init();
       } 


}
