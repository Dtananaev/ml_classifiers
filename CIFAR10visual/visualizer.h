/*
 * File: visualizer.h
 *
 * Author: Denis Tananaev
 *
 * Date: 24.10.2016
 */
#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QMainWindow>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atof */
#include <fstream>
#include <iostream>
#include <vector>

namespace Ui {
class visualizer;
}

class visualizer : public QMainWindow
{
    Q_OBJECT

public:
    explicit visualizer(QWidget *parent = 0);
    ~visualizer();

    bool readCFAR(const char* filename);

private:    
    Ui::visualizer *ui;
    std::vector<int> labels;
    std::vector< std::vector<unsigned char> > images; 

};

#endif // VISUALIZER_H
