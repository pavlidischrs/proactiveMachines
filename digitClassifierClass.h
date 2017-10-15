/*
 *
 * This class uses the Caffe framework to classify an image.
 *
 * After you set the newtowrk and the pretrained model, it makes forward passes to the network
 * and classifies the input image.
 *
 * */


#ifndef DIGITCLASSIFIERCLASS_H
#define DIGITCLASSIFIERCLASS_H


// This definition was taken from the Caffe example
// to compile and use the code succesfully
// ***  Since I do not run on linux I do not
// ***  know if these definitions are required.
#define BOOST_ALL_NO_LIB
#define CPU_ONLY
#define USE_OPENCV
#define CMAKE_WINDOWS_BUILD





#include <caffe/caffe.hpp>

#include "opencv2/opencv.hpp"

#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <string>



using namespace caffe;
using std::string;



// This pair is the (label, confidence) of the net output
// The biggest confidence is the predicition of the network
namespace digitClassifier{

typedef std::pair<string, float> Prediction;

class DigitClassifierClass
{
public:

    // The constructor of the class sets up the network.
    // Specifies the network architecture, pretrained model, input/output, labels
    // Then it will be ready for a Forward Pass with classifyImage method
    //
    // @arg1: the network model
    // @arg2: the trained network
    // @arg3: the labels of the output
    DigitClassifierClass(string model_file, string trained_file, string labels_file);

//    string model_file_ = "C:/Workspace/projects/caffeExample/caffeExample/myNetData/lenet_train_test_without_ST.prototxt";
//    string trained_file_ = "C:/Workspace/projects/caffeExample/caffeExample/myNetData/partial_without_ST__iter_2000_accuracy_084.caffemodel";
//    string label_file_ = "C:/Workspace/projects/caffeExample/caffeExample/myNetData/labels";


    // This is the main function of the Class
    //
    // @arg1: we have the input image which we'll pass to our network
    //
    // Inside the Result of classification is printed.
    //
    std::vector<Prediction> classifyImage(cv::Mat inputImage);

private:

    // Takes as input an image and gives it as an input to the network.
    // In other words, it makes a forward pass (or test) of the network
    //
    // It returns the output of the network
    //
    std::vector<float> forwardPass(const cv::Mat& img) ;

    // This is the last step before feeding the network.
    // It ptorcess the data to obey the network input
    void preprocessData(const cv::Mat& inputImage, std::vector<cv::Mat>* input_channels);


    /* Caffe-Comment:
     * Wrap the input layer of the network in separate cv::Mat objects
     * (one per channel). This way we save one memcpy operation and we
     * don't need to rely on cudaMemcpy2D. The last preprocessing
     * operation will write the separate channels directly to the input
     * layer. */
    void wrapInputLayer(std::vector<cv::Mat>* input_channels);



    // Return the indices of the top N values of vector
    static std::vector<int> argMax(const std::vector<float>& v, int N);

    // Compare the pairs of the parameters in terms of the first variable of the pair
    static bool DigitClassifierClass::pairCompare(const std::pair<float, int>& lhs, const std::pair<float, int>& rhs);


    boost::shared_ptr<Net<float> > net_;   // The network of the model

    std::vector<string> labels_;

    int num_channels_;

    cv::Size input_geometry_;
};

}

#endif // DIGITCLASSIFIERCLASS_H
