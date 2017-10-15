# Description

This application combines **Computer Vision** and **Machine Learning (Deep Learning)** to detect and recognise a digit from the **MNIST** database. The detection module uses the OpenCV library to extract a part of the Web Camera image which is on the inside of 3 Markers (check resources/imageWithMarkers.png). The recognition module uses the Caffe framework and the **LeNet CNN** to classify the image and finally recognise the digit.

# Detection Module

The created Markers are 5 black and white concentric squares. First, the module detects the contours of the image. Note that, the contours of the Markers will have the same **mass center** because the squares are homocentric. The black and white pattern creates a significant amount of contours on this area. We expect that there is no other place in the image with so many homocentric contours. 

Since we have the contours of the Markers we **extract** the inner image.

**[It should work with other concentric shapes as well!]**

# Recognition Module

The source file of this module is based on the cpp_classification example of the Caffe framework.

In the **resousrces** folder you can find the **networkArchtecture.prototxt** and **pretrainedModel.caffemodel** files. 
The former is the network model, which is a LeNet CNN. The latter is a pre-trained model of the network on the MNIST database. **Note** that, the provided pre-trained model was trained with few iterations (in purpose) and has 84% accuracy. You can always use your own **network** architecture and super-trained **caffemodel**.

**Future Work**. One issue of the detection module is that if you monitor an image with a big angle (which means that the markers are not aligned horizontally and vertically) it extracts a part of the inner image. This issue could be managed with an affine transformation of the image. Since the transformed image will be warped we expect that the accuracy of our network will be decreased. To resolve this issue we will use the so-called **[1] Spatial Transformer Network** to retain the accuracy of our classification.

[1] Jaderberg, Max, Karen Simonyan, and Andrew Zisserman. "Spatial transformer networks." Advances in Neural Information Processing Systems. 2015.

# Prerequisites

* If you want to run only the detection module you need **OpenCV** (**2.4** or **3.1** are ok).
* If you want to run both you need **Caffe Framework**.


# Usage

1. Change the **paths** of the needed files of the Recognition Module. 
1. Print or open some **images** with the markers.
1. **Place** your Web Cam in front of the printed image or diplay.
1. Check the **console prints** to see that the digit was recognised.


# System Details

The application was build with **QtCreator 5.9.2** and **MSVC 2015** on **Windows 10**.



