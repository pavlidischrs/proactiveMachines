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
    string model_file = "../proactiveMachines/resourceFiles/lenet_architecture.prototxt";
    string trained_file = "../proactiveMachines/resourceFiles/trained_model.caffemodel";
    string labels_file = "../proactiveMachines/resourceFiles/labels";
    DigitClassifierClass DigitClassifierObj(model_file, trained_file, labels_file);

    VideoCapture cap(0);
    if (!cap.isOpened())
        return 0;

    Mat frame;
    cap >> frame;

    int i=1;
    char c;

    vector<int> vote(10,0);

    for (;;){

        cap >> frame;


        // Extract the Image inside the Markers. If any.
        Mat extractedImage;
        bool imageExtracted = ImageExtractor.extractImage(frame, &extractedImage);
        if(imageExtracted){

            std::vector<Prediction> digitPrediction =  DigitClassifierObj.classifyImage(extractedImage);
//            cout << "Forward pass result: digit " << digitPrediction.at(0).first << endl;

            // Voting
            if(digitPrediction.at(0).first=="0 zero"){
                vote[0]++;
            }
            else if(digitPrediction.at(0).first=="1 one"){
                vote[1]++;
            }
            else if(digitPrediction.at(0).first=="2 two"){
                vote[2]++;
            }
            else if(digitPrediction.at(0).first=="3 three"){
                vote[3]++;
            }
            else if(digitPrediction.at(0).first=="4 four"){
                vote[4]++;
            }
            else if(digitPrediction.at(0).first=="5 five"){
                vote[5]++;
            }
            else if(digitPrediction.at(0).first=="6 six"){
                vote[6]++;
            }
            else if(digitPrediction.at(0).first=="7 seven"){
                vote[7]++;
            }
            else if(digitPrediction.at(0).first=="8 eight"){
                vote[8]++;
            }
            else if(digitPrediction.at(0).first=="9 nine"){
                vote[9]++;
            }

            namedWindow( "Extracted Image", CV_GUI_EXPANDED );
            imshow("Extracted Image", extractedImage);

            extractedImage.release();
        }


        for(int i=0; i<10;i++){
            if(vote[i]==10){

                Mat resultImage( Size(620,50), CV_8UC3, Scalar(255,255,255));
                putText(resultImage, "The network recognises the number " +to_string(i) , Point(0,30), CV_FONT_HERSHEY_SIMPLEX,1,Scalar(128,128,0),2,2);

                for(int k=0;k<10;k++)
                    vote[k]=0;

                namedWindow( "Result", CV_GUI_EXPANDED );
                imshow("Result", resultImage);
            }
        }

        // Display Web Cam image
        namedWindow( "Web Cam", CV_GUI_EXPANDED );
        imshow("Web Cam", frame);


        if (c == 27) break; // stop capturing by pressing ESC
        c = waitKey(10) ;

    }

    return 0;
}
