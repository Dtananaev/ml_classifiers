#include "visualizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    visualizer w;
    w.readCFAR("CIFAR10/data_batch_2.bin");
    w.show();
    
    return a.exec();
}
