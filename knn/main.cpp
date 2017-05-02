#include "knn.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    knn w;
   if(w.trainSetread("../CIFAR10") && w.testSetread("../CIFAR10")){
        w.init();
        w.show();
    }else{
       std::cerr<<"Can't open CIFAR10 dataset from ../CIFAR10 folder"<<"\n";
       return 0;
    }


    

}
