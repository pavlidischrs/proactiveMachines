#include "imageextractorclass.h"

ImageExtractorClass::ImageExtractorClass()
{

}

bool ImageExtractorClass::extractImage(Mat image, Mat *output){

    if(image.empty()){
        return false;
    }

    this->imageSize = image.size();

    bool succeed = false;

//Step 1
    // Detec Contours
    detectContours(image);

//Step 2
    // Examine each contours for possible markers
    examineContoursForMarkers();

//Step 3
    // Find the layout of the Markers
    int topLeft, topRight, bottomLeft;
    Point topLeftMC, topRightMC, bottomLeftMC;
    if(this->theMarkersContours.size()==3){



        this->findLayoutOfMarkers(&topLeft, &topRight, &bottomLeft);

        topLeftMC = mc[topLeft];
        topRightMC = mc[topRight];
        bottomLeftMC = mc[bottomLeft];

//Step 4
        // Since you spotted where the Markers are it is time to check
        // for the points of their contours which are closer to the
        // inner image, which we want to extract
        Point topLeftBottomRight, topRightBottomLeft, bottomLeftTopRight;

        Point innerImageCenter( topLeftMC.x + (topRightMC.x-topLeftMC.x)/2, topLeftMC.y + ( bottomLeftMC.y - topLeftMC.y ) / 2 );

        this->retrieveContourCorners(topLeft, innerImageCenter, &topLeftBottomRight);
        this->retrieveContourCorners(topRight, innerImageCenter, &topRightBottomLeft);
        this->retrieveContourCorners(bottomLeft, innerImageCenter, &bottomLeftTopRight);


//Step 5
        // You have already found the most interesting points.
        // Now you can extract the inner image.

        int topVertex = norm(topLeftBottomRight - topRightBottomLeft);
        int leftVertex = norm(topLeftBottomRight - bottomLeftTopRight);

        float offsetX = topVertex*0.1;
        float offsetY = leftVertex*0.1;

        Rect myROI( topLeftBottomRight.x + offsetX, topLeftBottomRight.y + offsetY, topVertex - 2*offsetX, leftVertex - 2*offsetY );

//Step 6 [FINAL]

        // Right the extracted image to output
        *output = image(myROI);
        succeed = true;


// Debug code. Displays points, contours or you can add whatever you want
        Mat debugImage = Mat::zeros( this->imageSize, CV_8UC3 );

        cv::drawMarker(debugImage, topLeftBottomRight,  cv::Scalar(0, 0, 255), MARKER_CROSS, 10, 5);
        cv::drawMarker(debugImage, topRightBottomLeft,  cv::Scalar(0, 0, 255), MARKER_CROSS, 10, 5);
        cv::drawMarker(debugImage, bottomLeftTopRight,  cv::Scalar(0, 0, 255), MARKER_CROSS, 10, 5);
        cv::drawMarker(debugImage, innerImageCenter,  cv::Scalar(255, 0, 0), MARKER_CROSS, 10, 5);

        for( int i = 0; i< this->theMarkersContours.size(); i++ ){
            Scalar color = Scalar( 0, 255, 0);
            drawContours( debugImage, this->contours, this->theMarkersContours[i], color, 2, 8, this->hierarchy, 0, Point() );
        }

        namedWindow( "Contours", CV_GUI_EXPANDED );
        imshow("Contours", debugImage);
        debugImage.release();
    }


    // Clear variables
    this->clearVariables();

    return succeed;
}

void ImageExtractorClass::retrieveContourCorners(int contourIndex, Point innerImageCenter, Point *closerPoint){


        vector<Point> markerContour  = this->contours[contourIndex]; // top left Marker contour


        // initialise the search
        Point temp = markerContour[0];
        int minDistance = norm( temp - innerImageCenter );
        *closerPoint = temp;

        // check if other Points are closer to the inner image center
        for( int i=1; i< markerContour.size(); i++){
            temp = markerContour[i];
            int tempDistance = norm( temp - innerImageCenter );

            if(tempDistance < minDistance){
               minDistance = tempDistance;
               *closerPoint = temp;
            }
        }

}

void ImageExtractorClass::findLayoutOfMarkers(int *topLeft, int *topRight, int *bottomLeft){


    // We take the 3
    Point A = mc[this->theMarkersContours[0]];
    Point B = mc[this->theMarkersContours[1]];
    Point C = mc[this->theMarkersContours[2]];

    int AB = norm(A-B);
    int BC = norm(B-C);
    int CA = norm(C-A);

    // Image the three points A,B,C to shape a triangle.
    // The biggest edge of the triangle is the one which does not contain
    // the top left Marker

    if( AB>BC && AB>CA ){ // C is the top left

        *topLeft = this->theMarkersContours[2]; //this the C point, but we actually care for the index of the contour

        // The top left square will have bigger x
        if(B.x > C.x){// B -> top right
            *topRight = this->theMarkersContours[1];
            *bottomLeft = this->theMarkersContours[0];
        }
        if(A.x > C.x){// A -> top right
            *topRight = this->theMarkersContours[0];
            *bottomLeft = this->theMarkersContours[1];
        }
    }
    else if( BC>AB && BC>CA ){ // A is the top left

        *topLeft = this->theMarkersContours[0]; //this the A point

        // The top left square will have bigger x
        if(B.x > A.x){// B -> top right
            *topRight = this->theMarkersContours[1];
            *bottomLeft = this->theMarkersContours[2];
        }
        if(C.x > A.x){// C -> top right
            *topRight = this->theMarkersContours[2];
            *bottomLeft = this->theMarkersContours[1];
        }
    }
    else if( CA>AB && CA>BC ){ // B is the top left

        *topLeft = this->theMarkersContours[1]; //this the B point

        // The top left square will have bigger x
        if(A.x > B.x){// A -> top right
            *topRight = this->theMarkersContours[0];
            *bottomLeft = this->theMarkersContours[2];
        }
        if(C.x > B.x){// C -> top right
            *topRight = this->theMarkersContours[2];
            *bottomLeft = this->theMarkersContours[0];
        }
    }

}


void ImageExtractorClass::detectContours(Mat srcImage){


    Mat srcGray;    // for grayscale image

    // From colour to graysclae image
    cvtColor( srcImage, srcGray, CV_BGR2GRAY );
    blur( srcGray, srcGray, Size(3,3) );          // Some typical blurring


    Mat edges;
    // Find the edges
    int thres = 50;
    Canny( srcGray, edges, thres, thres*2, 3 );

    // Since you found the edges now create the contours
    findContours( edges, this->contours, this->hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


    //You can uncoment the following block if you want to see the found edges. It could be used for debug to see the detected contours.
    // draw contours
//    Mat drawing = Mat::zeros( edges.size(), CV_8UC3 );
//    for( int i = 0; i< this->contours.size(); i++ ){
//        Scalar color = Scalar( 255, 255, 255);
//        drawContours( drawing, this->contours, i, color, 2, 8, this->hierarchy, 0, Point() );
//    }

//    imshow("Contours", drawing);
}



int ImageExtractorClass::checkForConcentric(int i){

    // If there is no children any more return since this is the last one
    if(this->hierarchy[i][2]==-1){
        return 1;
    }

    Point2f currentCenter = this->mc[i];   // center of examined contour
    Point2f childCenter = this->mc[i];   // center of the child of the examined contour

    float distance = norm(currentCenter-childCenter); // distance of two centers



    if( distance < this->massCentersThreshold){

        // If the child is inner square increase the concentric chidren number and check for the child's child
        return ( this->checkForConcentric(this->hierarchy[i][2] ) + 1 );
    }
    else{

        // If the child is not concentric, you are looking for inner squares in wrong region
        return 0;
    }

}



void ImageExtractorClass::examineContoursForMarkers(){

    // Initialise the moments and centers of the image
    mu = vector<Moments>(contours.size());
    mc = vector<Point2f>(contours.size());

    for( int i = 0; i < contours.size(); i++ )
    {
        mu[i] = moments( contours[i], false );
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    }


    // Examine if your contours have got children
    // If they have, check for possible Marker
    //
    // This method is looking for concentric squares so as to find the Markers
    //
    for( int i = 0; i < contours.size(); i++ ){

        int numberOfConcentricSquares=0;

        // If the contour has a child, it is a candidate for Marker
        if( this->hierarchy[i][2] != -1 ){
            numberOfConcentricSquares = this->checkForConcentric(i);
        }

        // 11 concentric contours is equal to 3 concentric squares, hence we have a Marker
        if(numberOfConcentricSquares == 10){
            this->theMarkersContours.push_back(i);
        }

    }



}


void ImageExtractorClass::clearVariables(){

    // Clear previous data
    this->theMarkersContours.clear();
    this->contours.clear();
    this->hierarchy.clear();
    // Clear moments and mass centers for next image
    mu.clear();
    mc.clear();

}



