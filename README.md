# Description

This application combines **Computer Vision** and **Machine Learning (Deep Learning)** to detect and recognise a digit from the **MNIST** database. The detection module uses the OpenCV library to extract a part of the Web Camera image which is on the inside of 3 Markers (check [files/imageWithMarkers.png](files/imageWithMarkers.png)). The recognition module uses the Caffe framework and the **LeNet CNN** to classify the image and finally recognise the digit.


![1](README_Image.png "Image")



# Gettin Started

**Detection Module**. Exploits the property that the concentric squares (Markers) have Contours with the same Mass Center. Using OpenCV, we **detect** the Markers and **extract** the inner image. 
*[It should work with other concentric shapes as well!]*

**Recognition Module**. It uses the pretrained-model ([files/pretrainedModel.caffemodel](files/pretrainedModel.caffemodel)) and the network model ([files/networkArchtecture.prototxt](networkArchtecture.prototxt)) to recognise the digit in the image.
*[**Note**, the provided pre-trained model was trained with few iterations (in purpose) and has 84% accuracy.]*
*[You can use **yours** network architecture and pre-trained model.]*

## Prerequisites

* If you want to run only the detection module you need **OpenCV** (**2.4** or **3.1** are ok).
* If you want to run both you need **Caffe Framework**.


# Usage

1. Change the **paths** of the needed files of the Recognition Module. 
1. Print or open some **images** with the markers.
1. **Place** your Web Cam in front of the printed image or diplay.
1. Check the **console prints** to see that the digit was recognised.


# Built With
* OpenCV 3.1
* Caffe framewrok
* QtCreator IDE 5.9.2 with the MSVC 2015 Kit (on Windows 10)


# Future Work

One issue of the detection module is that if you monitor an image with a big angle (which means that the markers are not aligned horizontally and vertically), it extracts a part of the inner image. This issue could be managed with an affine transformation of the image. Since the transformed image will be warped we expect that network performance will be decreased. To resolve this issue we will use the so-called **[1] Spatial Transformer Network** to retain the accuracy of our classification.

[1] Jaderberg, Max, Karen Simonyan, and Andrew Zisserman. "Spatial transformer networks." Advances in Neural Information Processing Systems. 2015.


