#include "svm.h"
#include "CMatrix.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    svm w;
   if(w.trainSetread("../CIFAR10") && w.testSetread("../CIFAR10")){
        w.init();
    }


   w.show();

    while ( w.isVisible()) {
        a.processEvents();
}    
    return 0;
}
