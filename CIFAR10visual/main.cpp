#include "visualizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    visualizer w;
    if(w.readCFAR("./CIFAR10")){
        w.init();
    }

    w.show();
    
    return a.exec();
}
