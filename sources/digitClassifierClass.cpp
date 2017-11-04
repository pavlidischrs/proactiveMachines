#include "digitClassifierClass.h"

using namespace digitClassifier;


DigitClassifierClass::DigitClassifierClass(string model_file, string trained_file, string labels_file)
{

    // set the mode to CPU only
    Caffe::set_mode(Caffe::CPU);

    // Feed the net_ variable with the appropriate files.
    // Network architecture and the trained model
    net_.reset(new Net<float>(model_file, TEST));
    net_->CopyTrainedLayersFrom(trained_file);

    // Check network functionality. We need 1 input and 1 ouput
    CHECK_EQ(net_->num_inputs(), 1) << "Network should have exactly one input.";
    CHECK_EQ(net_->num_outputs(), 1) << "Network should have exactly one output.";


    // Check if the input of the network is 1 or 3 channels. (RGB or Grayscale in our case)
    Blob<float>* input_layer = net_->input_blobs()[0];
    num_channels_ = input_layer->channels();
    CHECK(num_channels_ == 3 || num_channels_ == 1)
      << "Input layer should have 1 or 3 channels.";
    input_geometry_ = cv::Size(input_layer->width(), input_layer->height());


    // Load the labels of the model (ground truth)
    std::ifstream labels(labels_file.c_str());
    CHECK(labels) << "Unable to open labels file " << labels_file;
    string line;
    while (std::getline(labels, line))
      labels_.push_back(string(line));

    // Check if the output channels complies with the labels of the network
    // In our case 10 since we recognise the digits 0-9
    Blob<float>* output_layer = net_->output_blobs()[0];
    CHECK_EQ(labels_.size(), output_layer->channels())
      << "Number of labels is different from the output layer dimension.";
}


std::vector<Prediction> DigitClassifierClass::classifyImage(cv::Mat inputImage){

    // Pass image to network
    std::vector<float> output = forwardPass(inputImage);

    // Store the results to a Prediction vector and return it
    int N = labels_.size();
    std::vector<int> maxN = argMax(output, N);
    std::vector<Prediction> predictions;
    for (int i = 0; i < N; ++i) {
    int idx = maxN[i];
        predictions.push_back(std::make_pair(labels_[idx], output[idx]));
    }

    return predictions;
}



std::vector<float> DigitClassifierClass::forwardPass(const cv::Mat& img) {

// 1. Data preparation

    // Preparing the inputs
    Blob<float>* input_layer = net_->input_blobs()[0];
    input_layer->Reshape(1, num_channels_,
                       input_geometry_.height, input_geometry_.width);
    /* Forward dimension change to all layers. */
    net_->Reshape();


    std::vector<cv::Mat> input_channels;
    wrapInputLayer(&input_channels);

    preprocessData(img, &input_channels);


// 2. Prediction
    // Make the forward pass!!!
    net_->Forward();

// 3. Output
    // Copy the output to a vector and return it
    Blob<float>* output_layer = net_->output_blobs()[0];
    const float* begin = output_layer->cpu_data();          // beginning of data
    const float* end = begin + output_layer->channels();    // ending of data = beginning + number of output channels (number of labels)
    return std::vector<float>(begin, end);
}

void DigitClassifierClass::preprocessData(const cv::Mat& inputImage, std::vector<cv::Mat>* input_channels) {

  // Check if the number of channels of the input_image and the input_channels of the network are the same.
  // If they are different converte them so as to be the same.
    cv::Mat sample;
    if (inputImage.channels() == 3 && num_channels_ == 1)
        cv::cvtColor(inputImage, sample, cv::COLOR_BGR2GRAY);
    else if (inputImage.channels() == 4 && num_channels_ == 1)
        cv::cvtColor(inputImage, sample, cv::COLOR_BGRA2GRAY);
    else if (inputImage.channels() == 4 && num_channels_ == 3)
        cv::cvtColor(inputImage, sample, cv::COLOR_BGRA2BGR);
    else if (inputImage.channels() == 1 && num_channels_ == 3)
        cv::cvtColor(inputImage, sample, cv::COLOR_GRAY2BGR);
    else
        sample = inputImage;


    // Resize the input image to fit the network input
    cv::Mat sample_resized;
    if (sample.size() != input_geometry_)
        cv::resize(sample, sample_resized, input_geometry_);
    else
        sample_resized = sample;

    // Convert image_input values to float
    cv::Mat sample_float;
    if (num_channels_ == 3)
        sample_resized.convertTo(sample_float, CV_32FC3);
    else
        sample_resized.convertTo(sample_float, CV_32FC1);


    /* This operation will write the separate BGR planes directly to the
    * input layer of the network because it is wrapped by the cv::Mat
    * objects in input_channels. */
    cv::split(sample_float, *input_channels);

    CHECK(reinterpret_cast<float*>(input_channels->at(0).data)
          == net_->input_blobs()[0]->cpu_data())
    << "Input channels are not wrapping the input layer of the network.";
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void DigitClassifierClass::wrapInputLayer(std::vector<cv::Mat>* input_channels) {
    Blob<float>* input_layer = net_->input_blobs()[0];

    int width = input_layer->width();
    int height = input_layer->height();
    float* input_data = input_layer->mutable_cpu_data();
    for (int i = 0; i < input_layer->channels(); ++i) {
        cv::Mat channel(height, width, CV_32FC1, input_data);
        input_channels->push_back(channel);
        input_data += width * height;
    }
}



std::vector<int> DigitClassifierClass::argMax(const std::vector<float>& v, int N) {
    std::vector<std::pair<float, int> > pairs;
    for (size_t i = 0; i < v.size(); ++i)
        pairs.push_back(std::make_pair(v[i], static_cast<int>(i)));
    std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), pairCompare);

    std::vector<int> result;
    for (int i = 0; i < N; ++i)
        result.push_back(pairs[i].second);
    return result;
}

bool DigitClassifierClass::pairCompare(const std::pair<float, int>& lhs, const std::pair<float, int>& rhs) {
    return lhs.first > rhs.first;
}
