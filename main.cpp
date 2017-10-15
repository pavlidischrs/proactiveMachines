/*
 *
 *
 *  This is a Computer Vision app for detecting MNIST digits (which are inside a frame of QR Code)
 *  and then recognising it.
 *
 *  ************************************************************************************************
 *  QR Detection is implemented thanks to the repository https://github.com/bharathp666/opencv_qr
 *  ************************************************************************************************
 *
 **/






#include "imageextractorclass.h"
#include "digitclassifierclass.h"
#include "opencv2/opencv.hpp"

#include <vector>

using namespace cv;
using namespace digitClassifier;    // namespace of the DigitClassifierClass

int main(int argc, char* argv[]) {

    ImageExtractorClass ImageExtractor;


    // Add yours paths
    string model_file = "C:/Workspace/projects/proactiveMachines_0.1/proactiveMachines/resources/networkArchtecture.prototxt";
    string trained_file = "C:/Workspace/projects/proactiveMachines_0.1/proactiveMachines/resources/pretrainedModel.caffemodel";
    string labels_file = "C:/Workspace/projects/proactiveMachines_0.1/proactiveMachines/resources/labels";
    DigitClassifierClass DigitClassifierObj(model_file, trained_file, labels_file);

    VideoCapture cap(0);
    if (!cap.isOpened())
        return 0;


    Mat frame;
    cap >> frame;

    for (;;){

        cap >> frame;


        // Extract the Image inside the Markers. If any.
        Mat extractedImage;
        bool imageExtracted = ImageExtractor.extractImage(frame, &extractedImage);
        if(imageExtracted){

            std::vector<Prediction> digitPrediction =  DigitClassifierObj.classifyImage(extractedImage);
            cout << "The network output as top predicition the digit : " << digitPrediction[0].first << endl;

            namedWindow( "Extracted Image", CV_GUI_EXPANDED );
            imshow("Extracted Image", extractedImage);

            extractedImage.release();
        }


        // Display Web Cam image
        namedWindow( "Camera Image", CV_GUI_EXPANDED );
        imshow("Camera Image", frame);


        if (waitKey(10) == 27) break; // stop capturing by pressing ESC

    }

    return 0;
}
