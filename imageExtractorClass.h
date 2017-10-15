/*
 * The ImageExtractorClass identifies concentric squares in an image, which we cal Markers.
 * Later, it extracts the image which is contained inside the Markers
 *
 * For our experiment, we suppose that we have 3 Markers in a layout like the QR Codes.
 *
 * In order to extract the inner image of the 3 Markers, the aglorithm has to find 3 Markers (i.e. 3 batches of concentric squares)
 *
 * [it is supposed that it works for other shapes as well but I did not try it]
 *
 *
 *
 * TODO!: If the camera monitors the Markerts with some angle it does not extract the image well, because the current implementation
 *        extracts the image with a Rectangle Region of Interest yet. Which means that does not include any rotation of the image.
 *
 * FIX:   A rotation-affineWarp should be added to take the whole image inside the Markers.
 *
 * */



#ifndef IMAGEEXTRACTOR_H
#define IMAGEEXTRACTOR_H

#include "opencv2/opencv.hpp"

#include <iterator>


enum
{
    TOP_LEFT = 0,
    TOP_RIGHT = 1,
    BOTTOM_LEFT = 2,

};

using namespace cv;
using namespace std;

class ImageExtractorClass
{
public:
    ImageExtractorClass();

    // The maing functions of the class.
    // It uses the below methods to extract the image inside the Markers.
    //
    // If the functions succeeds it write the extracted in variable output and returns True!
    //
    // You will find more comments in its definition :)
    //
    //  @arg1: the input image
    //  @arg1: the output image
    //
    bool extractImage(Mat image, Mat *output);


private:

    // This functions retrives the points of the contour which are closer to the inner image center
    // (NOTE, it is not our image center!!! but the center of the image which is inside the Markers).
    // For each point it computes the distance from the center.
    // The shortest distance would come from the point closer to the center
    void retrieveContourCorners(int contourIndex, Point innerImageCenter, Point *closerPoint);

    // This method finds the Layout of the Markers, i.e. the top left/right and bottom left.
    // It uses the mass centers of the markers to identify which is where.
    void findLayoutOfMarkers(int *topLeft, int *topRight, int *bottomLeft);

    // This method find the condours of the input image
    void detectContours(Mat srcImage);

    // This method examines the found contours so as to find any QR Squares
    void examineContoursForMarkers();

    // This recursive method counts the concentric nested contours.
    // We use them to identify concentric nested rectangles, i.e. QR Squares
    int checkForConcentric(int i);

    // This method releases all the values of the used variables, contours, etc.
    void clearVariables();

    vector<vector<Point> > contours;    // the contours
    vector<Vec4i> hierarchy;            // the hierarchy of the contours
    vector<Moments> mu;    // moments
    vector<Point2f> mc;    // and mass centers

    vector<int> theMarkersContours;

    Size imageSize;

    int massCentersThreshold = 5; // or 10??
};

#endif // IMAGEEXTRACTOR_H
