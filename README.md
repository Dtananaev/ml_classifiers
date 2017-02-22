Machine Learning: classification
====================================================

K nearest neighbor, SVM, softmax classifiers, two layers neural network.

[![Build Status](https://travis-ci.org/Dtananaev/ml_classifiers.svg?branch=master)](https://travis-ci.org/Dtananaev/ml_classifiers)
[![BSD2 License](http://img.shields.io/badge/license-BSD2-brightgreen.svg)](https://github.com/Dtananaev/ml_classifiers/blob/master/LICENSE.md) 

Dataset
=======
To download CIFAR10 dataset use:

  ./\<classifier\>/get_datasets.sh

It will upload and extract files in CIFAR10 folder.


* CIFAR10visual - visualization of the CIFAR10 dataset
<p align="side">
  <img src="https://github.com/Dtananaev/ml_classifiers/blob/master/pictures/CIFAR10VIZ.JPG" width="150"/>
</p>
     * To install use in terminal: 
         * cd CIFAR10visual
         * mkdir build
         * cd build
         * cmake ..
         * make
     * To run: ./CIFAR10visual    
     

* two region segmentation based on variational methods allows to segment noisy images where the k-means segmentation ussualy fails because it adds noise to one of the segment. Two region segmentation approach could be easily extended to multi region segmentation
<p align="center">
  <img src="https://github.com/Dtananaev/cv_segmentation/blob/master/pictures/withWholeEasier.jpeg" width="350"/>
  <img src="https://github.com/Dtananaev/cv_segmentation/blob/master/pictures/withWholeEasier_2r.jpeg" width="350"/>
</p>
     * To install use in terminal: 
         * cd two_region_segmentation
         * make
     * To run: ./segment2r name_of_file(without .pgm)
