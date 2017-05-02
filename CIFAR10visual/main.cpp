#include "visualizer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    visualizer w;
    if(w.readCFAR("../CIFAR10")){
        w.init();
	w.show();
    
    }else{
    std::cerr<<"Can't open CIFAR10 dataset in folder ../CIFAR10"<<"\n";
    }
}
