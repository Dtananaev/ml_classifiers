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
     

* knn - k nearest neighbor clasifier with posibility of choosing L1 or L2 distance.
<p align="side">
  <img src="https://github.com/Dtananaev/ml_classifiers/blob/master/pictures/knn.JPG" width="250"/>
</p>
     * To install use in terminal: 
         * cd knn
         * mkdir build
         * cd build
         * cmake ..
         * make
     * To run: ./knn  
