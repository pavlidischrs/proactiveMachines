#include "imageExtractorClass.h"

ImageExtractorClass::ImageExtractorClass()
{

}

bool ImageExtractorClass::extractImage(Mat image, Mat *output){

    if(image.empty()){
        return false;
    }

    Point bottomRightMC;
    Point innerImageCenter;
    Point topLeftBottomRight, topRightBottomLeft, bottomLeftTopRight, bottomRightTopLeft;

    imageSize_ = image.size();

    bool succeed = false;

//Step 1
    // Detect Contours of the input image
    detectContours(image);

//Step 2
    // Examine each contours for possible markers
    examineContoursForMarkers();

//Step 3
    // Find the positions of the Markers
    int topLeft, topRight, bottomLeft;
    Point topLeftMC, topRightMC, bottomLeftMC;


    // if you have found 3 markers, go for the inner image
    if(indexesOfTheMarkerContours_.size()==3){

//Step 4
// Find the layout of the Markers, i.e. which one is the Top Left, Top Right and Bottom Left
        findLayoutOfMarkers(&topLeft, &topRight, &bottomLeft);

        topLeftMC = mc_[topLeft];
        topRightMC = mc_[topRight];
        bottomLeftMC = mc_[bottomLeft];


        // Create the Imginary(!!) Bottom Right Marker (It is imaginary but it will help out to extract the image)
        bottomRightMC = createBottomRightPoint(topLeftMC, topRightMC, bottomLeftMC);



//Step 5
// Up to here, we have the mass centers of the Markers
// Now, we want to find the Points of the Markers, which are closer to the inner image
// e.g. for the Top Left we want its bottom right corner (This point will be on the Marker's contour, right?)


        // First, we have to find the center of the square that is formed by the Markers.
        // The center will be the intersection Point of the lines formed by the diagonal Markers
        //
        innerImageCenter= findCenterOfMarkers(topLeftMC, bottomRightMC, topRightMC, bottomLeftMC);


        // Then we will find the points of the contours of each Marker which are closer to the innerImageCenter
        //
        retrieveContourCorners(topLeft, innerImageCenter, &topLeftBottomRight);
        retrieveContourCorners(topRight, innerImageCenter, &topRightBottomLeft);
        retrieveContourCorners(bottomLeft, innerImageCenter, &bottomLeftTopRight);
        bottomRightTopLeft = createBottomRightPoint(topLeftBottomRight, topRightBottomLeft, bottomLeftTopRight);


//Step 6
// Extract the inner image from whole image

//        Mat extractedImage = Mat::zeros(100, 100, CV_8UC3 );
        Mat extractedImage = Mat(100, 100, CV_8UC3 , Scalar(0,0,0));
        int offset = 10;

        // Find the affine transformation that maps the region of intereste (inisde the markers) into an image (extracted image or ouput)
        // The input will be the corners of the Markers, which are closer to the inner image. Actually, we take into account an offset of these corners to go a bit more closer to the inner image
        Mat warpMatrix = takeAffineTransformation(topLeftBottomRight + Point(offset,offset), topRightBottomLeft + Point(-offset,offset), bottomLeftTopRight + Point(offset,-offset), bottomRightTopLeft + Point(-offset,-offset));

        // Apply the transformation to the original image and take the extracted image
        warpAffine(image, extractedImage, warpMatrix, Size(100,100),INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));


//Step 7 [FINAL]
        // Right the extracted image to output
        *output = extractedImage;
        if (output->size>0){
            succeed = true;
        }
        else{
            succeed = false;
        }


// Debug block
// Debug block
// Debug block
// Debug block
// Change condition to false if you don't need it
        if(true){

            debugImage = Mat::zeros( imageSize_, CV_8UC3 );
debugImage=image;

            drawMarker(debugImage, Point(10,15),  Scalar(255, 0, 0), MARKER_CROSS, 3, 5);
            putText(debugImage, ": Created Points ", Point(20,20), CV_FONT_HERSHEY_SIMPLEX,0.6, Scalar(255,255,255),1,8);
            drawMarker(debugImage, Point(10,35),  Scalar(0, 0, 255), MARKER_CROSS, 3, 5);
            putText(debugImage, ": Closer points to the image center ", Point(20,40), CV_FONT_HERSHEY_SIMPLEX,0.6, Scalar(255,255,255),1,8);

            rectangle(debugImage, Point(12,debugImage.rows - 10), Point(28,debugImage.rows - 25), Scalar(0,0,255), 1, 8, 0);
            putText(debugImage, ": Part of Extraction", Point(+30,debugImage.rows - 15), CV_FONT_HERSHEY_SIMPLEX,0.6, Scalar(255,255,255),1,8);
            rectangle(debugImage, Point(12,debugImage.rows - 40), Point(28,debugImage.rows - 55), Scalar(0,255,0), 1, 8, 0);
            putText(debugImage, ": Outestmost Contour of Marker", Point(+30,debugImage.rows - 45), CV_FONT_HERSHEY_SIMPLEX,0.6, Scalar(255,255,255),1,8);



            drawMarker(debugImage, topLeftBottomRight,  Scalar(0, 0, 255), MARKER_CROSS, 10, 5);
            drawMarker(debugImage, topRightBottomLeft,  Scalar(0, 0, 255), MARKER_CROSS, 10, 5);
            drawMarker(debugImage, bottomLeftTopRight,  Scalar(0, 0, 255), MARKER_CROSS, 10, 5);
            drawMarker(debugImage, bottomRightTopLeft,  Scalar(0, 0, 255), MARKER_CROSS, 10, 5);

            putText(debugImage, "TopLeft", topLeftMC + Point(-50,-25), CV_FONT_HERSHEY_SIMPLEX,0.6,Scalar(180,180,180),0.1,8);
            putText(debugImage, "TopRight", topRightMC + Point(30,0), CV_FONT_HERSHEY_SIMPLEX,0.6, Scalar(180,180,180),1,8);
            putText(debugImage, "BottomLeft", bottomLeftMC + Point(-50,50), CV_FONT_HERSHEY_SIMPLEX,0.6, Scalar(180,180,180),1,8);


            for( int i = 0; i< indexesOfTheMarkerContours_.size(); i++ ){
                Scalar color = Scalar( 0, 255, 0);
                drawContours( debugImage, contours_, indexesOfTheMarkerContours_[i], color, 2, 8, hierarchy_, 0, Point() );
            }


            Point polyPoints[1][4];
            polyPoints[0][0] = topLeftBottomRight + Point(+5,+5) ;
            polyPoints[0][1] = topRightBottomLeft + Point(-5,+5) ;
            polyPoints[0][2] = bottomRightTopLeft + Point(-5,-5) ;
            polyPoints[0][3] = bottomLeftTopRight + Point(+5,-5) ;

            const Point* pts[1] = {polyPoints[0]};
            const int npts[] = {4};
            polylines(debugImage, pts, npts, 1, 1, Scalar(0,0,255), 2,8, 0);


            drawMarker(debugImage, bottomRightMC,  Scalar(255, 0, 0), MARKER_CROSS, 10, 5);
            drawMarker(debugImage, innerImageCenter,  Scalar(255, 0, 0), MARKER_CROSS, 10, 5);

            namedWindow( "Debug Image", CV_GUI_EXPANDED );
            imshow("Debug Image", debugImage);
            debugImage.release();
        }


    }

    // Clear variables
    clearVariables();

    return succeed;
}

struct myclass {
    bool operator() (Point pt1, Point pt2) { return (pt1.y < pt2.y);}
} sortingObject;

Point ImageExtractorClass::createBottomRightPoint(Point topLeftMC, Point topRightMC, Point bottomLeftMC){

    int slopeInY = - topLeftMC.y + topRightMC.y;
    int slopeInX = - topLeftMC.x + topRightMC.x;

    return Point(bottomLeftMC.x + slopeInX, bottomLeftMC.y + slopeInY);
}

Point ImageExtractorClass::findCenterOfMarkers(Point P1, Point P2, Point Q1, Point Q2)
{
    int x1 = P1.x;
    int y1 = P1.y;
    int x2 = P2.x;
    int y2 = P2.y;
    int x3 = Q1.x;
    int y3 = Q1.y;
    int x4 = Q2.x;
    int y4 = Q2.y;

    Point intersectionPoint;

    // This is the formula to calculate the intersection point of two lines
    // (check the link https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection for more details)
    //
    intersectionPoint.x = ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4)) / ( (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4) );
    intersectionPoint.y = ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4)) / ( (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4) );

    return intersectionPoint;
}


Mat ImageExtractorClass::takeAffineTransformation(Point2f TL, Point2f TR, Point2f BL, Point2f BR)
{
    Point2f src[3], dst[3];

    src[0]=TL;
    src[1]=TR;
    src[2]=BL;

    dst[0]=Point(0,0);
    dst[1]=Point(100,0);
    dst[2]=Point(0,100);

    Mat matrix = getAffineTransform(src,dst);

    return matrix;
}

Mat ImageExtractorClass::takePerspectiveTransformation(Point2f TL, Point2f TR, Point2f BL, Point2f BR)
{

    Point2f src[4], dst[4];

    // We have the points of the Markers
    src[0] = (TL);
    src[1] = (TR);
    src[2] = (BR);
    src[3] = (BL);

    // We want the "src" points to map to a rectangle of width and height 100
    dst[0] = (Point2f(0,0));
    dst[1] = (Point2f(100,0));
    dst[2] = (Point2f(100, 100));
    dst[3] = (Point2f(0, 100));

    // Find the matrix which maps the points from "src" to "dst"
    // i.e. the Perspective Transformation Matrix
    Mat matrix = getPerspectiveTransform(src, dst);

    return matrix;
}

void ImageExtractorClass::retrieveContourCorners(int contourIndex, Point innerImageCenter, Point *closerPoint){


        vector<Point> markerContour  = contours_[contourIndex]; // top left Marker contour


        // initialise the search
        Point temp = markerContour[0];
        int minDistance = norm( temp - innerImageCenter );
        *closerPoint = temp;

        // Find the point of the contour which is closer to the Center of the Inner Image
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
    Point A = mc_[indexesOfTheMarkerContours_[0]];
    Point B = mc_[indexesOfTheMarkerContours_[1]];
    Point C = mc_[indexesOfTheMarkerContours_[2]];

    int AB = norm(A-B);
    int BC = norm(B-C);
    int CA = norm(C-A);

    // Image the three points A,B,C to shape a triangle.
    // The biggest edge of the triangle is the one which does not include
    // the Top Left Marker point

    if( AB>BC && AB>CA ){ // C is the top left

        *topLeft = indexesOfTheMarkerContours_[2]; //this is the C point, but we actually care for the index of the contour, which is 2

        // make an assumption of the Top Right and Bottom Left
        *topRight = indexesOfTheMarkerContours_[0];
        *bottomLeft = indexesOfTheMarkerContours_[1];
    }
    else if( BC>AB && BC>CA ){ // A is the top left

        *topLeft = indexesOfTheMarkerContours_[0]; //this the A point

        // make an assumption of the Top Right and Bottom Left
        *topRight = indexesOfTheMarkerContours_[1];
        *bottomLeft = indexesOfTheMarkerContours_[2];
    }
    else if( CA>AB && CA>BC ){ // B is the top left

        *topLeft = indexesOfTheMarkerContours_[1]; //this the B point

        // make an assumption of the Top Right and Bottom Left
        *topRight = indexesOfTheMarkerContours_[0];
        *bottomLeft = indexesOfTheMarkerContours_[2];
    }

    // Refine and take final Layout
    int result = findSlopeAndDistance(mc_[*topLeft], mc_[*topRight], mc_[*bottomLeft]);

    // If the function has returned 1 means that the assumption was reverse
    if(result == -1){
        int temp = *bottomLeft;
        *bottomLeft = *topRight;
        *topRight = temp;
    }
}

int ImageExtractorClass::findSlopeAndDistance(Point topLeft, Point topRight, Point bottomLeft)
{
    // find the slope of the bigger line slope
    float slope;

    float dx = topRight.x-bottomLeft.x;
    float dy = topRight.y-bottomLeft.y;

    if(dx!=0){
        slope = dy/dx;
    }
    else{
        return 0;
    }


    // find the distance from the topLeft Marker to the edge formed by the other two points
    float a = -slope;
    float b = 1.0;
    float c = (slope * topRight.x) - topRight.y;


    float dist = (a*topLeft.x + b*topLeft.y + c) / sqrt( (a*a)+(b*b) );


    // if both are positive or negative the caller's assumption was wrong and needs to reverse the Top Right and Bottom Left
    if( (slope>0 && dist>0) ){
        return -1;
    }
    else if((slope<0 && dist<0) ){
        return -1;
    }
    else if((slope<0 && dist>0) ){
        return 1;
    }
    else if((slope>0 && dist<0) ){
        return 1;
    }
    else{
        return 0;
    }

}

void ImageExtractorClass::findTRandBL(Point topLeft, int index1, int index2, int *topRight, int *bottomLeft){

    Point C1 = mc_[indexesOfTheMarkerContours_[index1]];
    Point C2 = mc_[indexesOfTheMarkerContours_[index2]];


    // The top right Marker will have bigger x than the top left Marker
    if( abs(C1.x - topLeft.x) > 25 ){// C1 -> top right
        *topRight = indexesOfTheMarkerContours_[index1];
        *bottomLeft = indexesOfTheMarkerContours_[index2];
    }
    if( abs(C2.x - topLeft.x) > 25  ){// C2 -> top right
        *topRight = indexesOfTheMarkerContours_[index2];
        *bottomLeft = indexesOfTheMarkerContours_[index1];
    }
}

void ImageExtractorClass::detectContours(Mat srcImage){

    contours_.clear();
    hierarchy_.clear();

    Mat srcGray;    // for grayscale image

    // From colour to graysclae image
    cvtColor( srcImage, srcGray, CV_BGR2GRAY );
    blur( srcGray, srcGray, Size(3,3) );          // Some typical blurring


    Mat edges;
    // Find the edges
    int thres = 50;
    Canny( srcGray, edges, thres, thres*2, 3 );

    // Since you found the edges now create the contours
    findContours( edges, contours_, hierarchy_, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

}

void ImageExtractorClass::examineContoursForMarkers(){

    // Initialise the moments and centers of the image
    mu_ = vector<Moments>(contours_.size());
    mc_ = vector<Point2f>(contours_.size());

    for( int i = 0; i < contours_.size(); i++ )
    {
        mu_[i] = moments( contours_[i], false );
        mc_[i] = Point2f( mu_[i].m10/mu_[i].m00 , mu_[i].m01/mu_[i].m00 );
    }


    // Examine if the contours have got children
    // If they have, check for possible Marker (You have a marker when you have eleven concetntric contours)
    //
    for( int i = 0; i < contours_.size(); i++ ){

        int numberOfConcentricSquares=0;

        // If current contour has a child, it is a candidate for Marker
        if( hierarchy_[i][2] != -1 ){
            numberOfConcentricSquares = checkConcentrics(i);     // Examine it
        }

        // 11 concentric contours is equal to 3 concentric squares, hence we have a Marker
        if(numberOfConcentricSquares == 10){
            indexesOfTheMarkerContours_.push_back(i);
        }

    }



}

int ImageExtractorClass::checkConcentrics(int i){

    // If there is no children any more return since this is the last one
    if(hierarchy_[i][2]==-1){
        return 1;
    }

    Point2f currentCenter = mc_[i];   // center of examined contour
    Point2f childCenter = mc_[i];   // center of the child of the examined contour

    float distance = norm(currentCenter-childCenter); // distance of two centers

    if( distance < massCentersThreshold_){

        // If the child is inner square increase the concentric chidren number and examine the child's child
        return ( checkConcentrics(hierarchy_[i][2] ) + 1 );
    }
    else{
        // If the child is not concentric, you are looking for inner squares in wrong region, return zero
        return 0;
    }

}






void ImageExtractorClass::clearVariables(){

    // Clear previous data
    indexesOfTheMarkerContours_.clear();

    // Clear moments and mass centers for next image
    mu_.clear();
    mc_.clear();

}



