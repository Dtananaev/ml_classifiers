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

svm::svm(QWidget *parent):  QMainWindow(parent){   
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

svm::~svm(){
}


void svm::stopClicked(){

 stop_=true;
}

void svm::weight2image(CMatrix<float> w, int label, QImage &img){


 for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
        int red=w(label,y*32+x);
        int green=w(label,1024+y*32+x);
        int blue=w(label,2048+y*32+x);
      img.setPixel(x, y, qRgb(red, green, blue));
    }
  }

}


void svm::vizWeights(){

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

CMatrix<float> w=W_;
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

void svm::updatIterationNumber(){
    iteration_=itBox->value();
}

void svm::updatBatchNumber(){
    batch_size_=batchBox->value();
}

void svm::updateStep(){
    step_=stepBox->value();
}
void svm::updateLambda(){

    lambda_=lambdaBox->value();
}
void svm::runTraining(){
    stop_=false;
    SVMiterate(iteration_, batch_size_);
}

void svm::zeroMeanData(){
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
meanLabel->setText(QString::fromUtf8("Zero Mean dataset."));
    std::cout<<"Make dataset zero mean"<<"\n"; 
   zeroMeanButton->setEnabled(false); 
}




void svm::SVMiterate(int iter, int batch){

      progressBar->setValue(0);
  int number_of_full_batches= train_images.size()/batch;

   int last_batch= train_images.size()-number_of_full_batches*batch;

    for(int i=0; i<iter;i++){

         itResult->setText(QString::number(i+1) +"/"+QString::number(iter) );
         progressBar->setValue(0);  
        for(int j=0;j<number_of_full_batches;j++){    
                               if(stop_){return;}
                
               int progress = 100*j/number_of_full_batches;
                progressBar->setValue(progress);       

             int from=j*batch;
             int until=j*batch+batch-1;    
            Loss_=loss(train_images, train_labels,  from, until);

            lossResult->setText(QString::number(Loss_));
    vizWeights();
        } 
    

    calculatePerformance();
    }

       progressBar->setValue(100);
}


void svm::resetWeights(){
//init weight and scores matrices
   initRandomWeights();
    vizWeights();
    int result = inference(test_images[currentIndex_]);
    lineEdit_2->setText(QString::fromUtf8(categories[result].c_str()));

}



void svm::initRandomWeights(){
  W_.setSize(categories.size(),train_images[0].size()); //weight matrix

 std::default_random_engine generator;
  std::normal_distribution<double> distribution(0,0.00001);

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
    iteration_=1;
    batch_size_=100;
    lambda_=0.5;
    step_=0.00000000001;
 //init weight and scores matrices
   initRandomWeights();
    vizWeights();
    dW_.setSize(categories.size(),train_images[0].size()); //gradient update matrix
    




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

    int result = inference(test_images[currentIndex_]);
  
    lineEdit_2->setText(QString::fromUtf8(categories[result].c_str()));
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

    int result = inference(test_images[index]);
   

    lineEdit_2->setText(QString::fromUtf8(categories[result].c_str()));
}


void svm::calculatePerformance(){

     float acc=0;
 for(int i=0; i<test_labels.size();i++){


          int prediction=inference(test_images[i]);

        acc+= prediction == test_labels[i];
    }
    progressBar->setValue(0);
    acc=acc/test_labels.size();
std::cout<<"final acc "<<acc<<"\n";
accResult->setText(QString::number(acc));


}

bool svm::trainSetread(const char* dirname){
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

//all svm functions

float svm::L2W_reg(){
    float sum = 0;
    for (int x = 0; x < W_.xSize(); ++x)
        for (int y = 0; y < W_.ySize(); ++y){
            sum += W_(x,y) * W_(x,y);
        }
    return sum;
}

float svm::loss_one_image(const std::vector<int> &image, const int &y){

   // assert(image.size() == 3073);

    std::vector<float> scores(10, 0);

    // Compute scores
    // scores = W*x
    for(int c=0; c<C; ++c){
        for(int d=0; d<D; ++d){
            scores[c] += W_(c,d)*image[d];
        }
    }

    // Compute loss
    float loss = 0;
    int counter = 0;
    std::vector<float> margins(10,0);
    for (int j=0; j<C; ++j)
    {
        if(j==y) continue;
        margins[j] = std::max(0.f, scores[j] - scores[y] + 1);
        counter += (margins[j]>0);
        loss += margins[j];

    }

    // Compute gradient
    for (int j=0; j<C; ++j)
        for (int d=0; d<D; ++d){
            if(j==y){
                dW_(j,d) += -image[d]*counter;
            } else if(j!=y) {
                dW_(j,d) += (margins[j]>0)*image[d];
            }
        }


    return loss;
}

float svm::loss(const std::vector< std::vector<int> > &images, const std::vector<int> &labels, int from, int to)
{
    assert(images.size() == 50000); //only training set

    // Reset gradient
    dW_.fill(0.0);

    // Compute loss for all images
    float L = 0;
    int N = to-from; // N images in batch
    for(int i=from; i<to; ++i){
        L += loss_one_image(images[i], labels[i]);
    }
    L /= N;
    L += 0.5 * lambda_ * L2W_reg();
    std::cout << "Loss: " << L << std::endl;

    // Normalize and regularize gradient
    for (int x=0; x< dW_.xSize(); ++x){
        for (int y=0; y < dW_.ySize(); ++y){
            dW_(x,y) = dW_(x,y)/static_cast<float>(N) + lambda_*W_(x,y);
        }
    }

    // Update weights
    for (int x=0; x<W_.xSize(); ++x){
        for (int y=0; y<W_.ySize(); ++y){
            W_(x, y) += -step_*dW_(x, y);
        }
    }

    return L;
}


int svm::inference(const std::vector<int> &image){

    std::vector<float> scores(10, 0);

    // scores = W*x
    for(int c=0; c < C; ++c){
        for(int d=0; d < D; ++d){
            scores[c] += W_(c,d)*image[d];
        }
    }

    return std::max_element(scores.begin(), scores.end()) - scores.begin();
}
