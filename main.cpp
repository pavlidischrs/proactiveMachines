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






#include "imageExtractorClass.h"
#include "digitClassifierClass.h"
#include "opencv2/opencv.hpp"

#include <vector>

using namespace cv;
using namespace digitClassifier;    // namespace of the DigitClassifierClass

int main(int argc, char* argv[]) {

    ImageExtractorClass ImageExtractor;


    // Add yours paths
    string model_file = "C:/Workspace/projects/proactiveMachines_0.1/proactiveMachines/files/networkArchtecture.prototxt";
    string trained_file = "C:/Workspace/projects/proactiveMachines_0.1/proactiveMachines/files/pretrainedModel.caffemodel";
    string labels_file = "C:/Workspace/projects/proactiveMachines_0.1/proactiveMachines/files/labels";
    DigitClassifierClass DigitClassifierObj(model_file, trained_file, labels_file);

    VideoCapture cap(0);
    if (!cap.isOpened())
        return 0;


    Mat frame;
    cap >> frame;

    int i=1;
    char c;

    for (;;){

        cap >> frame;
//        frame = imread("dbg2.png");

//        ImageExtractor.fillMarginWithColor(frame);

        // Extract the Image inside the Markers. If any.
        Mat extractedImage;
        bool imageExtracted = ImageExtractor.extractImage(frame, &extractedImage);
        if(imageExtracted){

            std::vector<Prediction> digitPrediction =  DigitClassifierObj.classifyImage(extractedImage);
            cout << "Forward pass result: digit " << digitPrediction.at(0).first << endl;

            namedWindow( "Extracted Image", CV_GUI_EXPANDED );
            imshow("Extracted Image", extractedImage);

            extractedImage.release();
        }


        // Display Web Cam image
        namedWindow( "Camera Image", CV_GUI_EXPANDED );
        imshow("Camera Image", frame);


        if(c==32){
            imwrite("dbg3.png", frame);
        }

        if (c == 27) break; // stop capturing by pressing ESC
        c = waitKey(10) ;

    }

    return 0;
}
