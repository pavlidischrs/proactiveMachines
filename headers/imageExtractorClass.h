/*
 * The ImageExtractorClass identifies concentric squares in an image, which we call Markers and
 * then extracts the image included into these Markers
 *
 * We suppose that we have 3 Markers in a layout like the QR Codes.
 *
 * In order to extract the inner image of the 3 Markers, the aglorithm has to find 3 Markers (i.e. 3 batches of concentric squares)
 *
 * [it is supposed that it works for other shapes as well but I did not try it]
 *
 * */



#ifndef IMAGEEXTRACTOR_H
#define IMAGEEXTRACTOR_H

#include "opencv2/opencv.hpp"

#include <iterator>

#include <algorithm>

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

    // The main functions of the class. It extracts an image include into 3 Markers and writes the result to the output!
    //
    // If the function succeeds returns True, otherwise False!
    //
    // Go to definition and see more commnets how it figures out to extract the inner image :)
    //
    //  @arg1: the input image
    //  @arg1: the output image
    //
    bool extractImage(Mat image, Mat *output);


    // **********Under construction....
    // Sometimes the extracted image contains white areas near the boarder of the image.
    // With this method we will color these areas black
    void fillMarginWithColor(Mat image);

private:

    // Given 3 points we can compute the 4th point which helps as
    Point createBottomRightPoint(Point topLeftMC, Point topRightMC, Point bottomLeftMC);


    // Returns the Point of interection of the two lines P1P2 and Q1Q2
    Point findCenterOfMarkers(Point P1, Point P2, Point Q1, Point Q2);


    // This method returns an affine transformation which maps a polygon (formed from the 4 points, arguments 1,2,3,4)
    // to an orthogonal rectangle
    Mat takeAffineTransformation(Point2f TL, Point2f TR, Point2f BL, Point2f BR);

    // Compute the perspective transformation of the markers subject to the markers position
    Mat takePerspectiveTransformation(Point2f TL, Point2f TR, Point2f BL, Point2f BR);


    // This functions retrives the points of the contour which are closer to the center of the inner image (or the center of the square formed by the 3 Markers)
    // For each point it computes the distance from the center.
    // The shortest distance would come from the point closer to the center and we write it to the closerPoint argument
    void retrieveContourCorners(int contourIndex, Point innerImageCenter, Point *closerPoint);

    // This method finds the Layout of the Markers, i.e. the top left/right and bottom left.
    //
    // 1. It finds out the bigger edge of the triangle formed from the three markers.
    // 2. The Top Right Marker is the one that does not take part in the line of 1.
    // 3. Then we use the slope and distance of the line from 1. to identify the Top Right and Bottom Left Marker (with findSlopeAndDistance() method)
    //
    void findLayoutOfMarkers(int *topLeft, int *topRight, int *bottomLeft);

    // This method is used from the findLayoutOfMarkers to compute the Top Right and Bottom Left
    // It finds the Slope of the bigger edge of a triangle and then computes the Distance from the Top Left Marker
    // to the bigger edgge.
    //
    //  If slope and dist are either negative or positive the assumption was wrong and returns -1
    //  It returns 1 otherwise. It returns 0 when slope==0, which means that the bigger edge is vertical
    //
    // **!!** (for the step 3. you can read more on the following link http://dsynflo.blogspot.ch/2014/10/opencv-qr-code-detection-and-extraction.html)
    //
    int findSlopeAndDistance(Point topLeft, Point topRight, Point bottomLeft);


    //     *********     DEPRECATED Not used any more
    //
    // This method takes as input the mass center of the top left Marker (argument: topLeft)
    // and the indexes of mass centers of the other Markers (arguments: index1, index2)
    //
    // It makes an assumption of which contours correspond to the topRight and bottomLeft Markers
    // and outputs the result at argumentsL *topRight and *bottomLeft
    void findTRandBL(Point topLeft, int index1, int index2, int *topRight, int *bottomLeft);

    // This method find the condours of the input image. You can draw these contours by passing one @arg Debug
    void detectContours(Mat srcImage);

    // This method examines the contours of the input image so as to find any Markers
    //
    void examineContoursForMarkers();

    // Finds out the number of concentric contours (if any).
    // It does so recurisively by examining the hierarchy of the contours
    // to find out if the children of contours are concentric
    int checkConcentrics(int i);

    // This method releases all the values of the used variables, contours, etc.
    void clearVariables();



    vector<vector<Point> > contours_;    // the contours
    vector<Vec4i> hierarchy_;            // the hierarchy of the contours
    vector<Moments> mu_;    // moments
    vector<Point2f> mc_;    // and mass centers

    vector<int> indexesOfTheMarkerContours_;
    vector<int> theMarkersContours_;

    Size imageSize_;

    int massCentersThreshold_ = 5; // or 10??

    Mat debugImage;


};

#endif // IMAGEEXTRACTOR_H
