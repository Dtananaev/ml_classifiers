#include "visualizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    visualizer w;
    if(!w.readCFAR("CIFAR10/data_batch_2.bin")){
        std::cout<<"Can't read the CIFAR10 dataset"<<"\n";
        return 0;
    }
    w.init();
    w.show();
    
    while (w.isVisible()) {

        a.processEvents();
    }

    return a.exec();
}
