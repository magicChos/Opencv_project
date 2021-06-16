/*****************************************************************************
*   Number Plate Recognition using SVM and Neural Networks
******************************************************************************
*   by David Mill�n Escriv�, 5th Dec 2012
*   http://blog.damiles.com
******************************************************************************
*   Ch5 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

#include "OCR.h"
#include <opencv2/opencv.hpp>
#include <opencv2/world.hpp>


const char OCR::strCharacters[] = {'0','1','2','3','4','5','6','7','8','9','B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'R', 'S', 'T', 'V', 'W', 'X', 'Y', 'Z'};
const int OCR::numCharacters=30;

CharSegment::CharSegment(){}
CharSegment::CharSegment(Mat i, Rect p){
    img=i;
    pos=p;
}

OCR::OCR(){
    DEBUG=false;
    trained=false;
    saveSegments=false;
    charSize=20;
}
OCR::OCR(string trainFile){
    DEBUG=false;
    trained=false;
    saveSegments=false;
    charSize=20;

    //Read file storage.
    cv::FileStorage fs;
    fs.open("OCR.xml", FileStorage::READ);
    cv::Mat TrainingData;
    cv::Mat Classes;
    fs["TrainingDataF15"] >> TrainingData;
    fs["classes"] >> Classes;

    train(TrainingData, Classes, 10);

}

cv::Mat OCR::preprocessChar(cv::Mat in){
    //Remap image
    int h=in.rows;
    int w=in.cols;
    cv::Mat transformMat=cv::Mat::eye(2,3,CV_32F);
    int m=max(w,h);
    transformMat.at<float>(0,2)=m/2 - w/2;
    transformMat.at<float>(1,2)=m/2 - h/2;

    cv::Mat warpImage(m,m, in.type());
    cv::warpAffine(in, warpImage, transformMat, warpImage.size(), INTER_LINEAR, BORDER_CONSTANT, cv::Scalar(0) );

    cv::Mat out;
    cv::resize(warpImage, out, cv::Size(charSize, charSize) ); 

    return out;
}

bool OCR::verifySizes(Mat r){
    //Char sizes 45x77
    float aspect=45.0f/77.0f;
    float charAspect= (float)r.cols/(float)r.rows;
    float error=0.35;
    float minHeight=15;
    float maxHeight=28;
    //We have a different aspect ratio for number 1, and it can be ~0.2
    float minAspect=0.2;
    float maxAspect=aspect+aspect*error;
    //area of pixels
    float area=countNonZero(r);
    //bb area
    float bbArea=r.cols*r.rows;
    //% of pixel in area
    float percPixels=area/bbArea;

    if(DEBUG)
        cout << "Aspect: "<< aspect << " ["<< minAspect << "," << maxAspect << "] "  << "Area "<< percPixels <<" Char aspect " << charAspect  << " Height char "<< r.rows << "\n";
    if(percPixels < 0.8 && charAspect > minAspect && charAspect < maxAspect && r.rows >= minHeight && r.rows < maxHeight)
        return true;
    else
        return false;

}

vector<CharSegment> OCR::segment(Plate plate){
    cv::Mat input=plate.plateImg;
    vector<CharSegment> output;
    //Threshold input image
    cv::Mat img_threshold;
    cv::threshold(input, img_threshold, 60, 255, CV_THRESH_BINARY_INV);
    if(DEBUG)
        imshow("Threshold plate", img_threshold);
    cv::Mat img_contours;
    img_threshold.copyTo(img_contours);
    //Find contours of possibles characters
    vector< vector< Point> > contours;
    cv::findContours(img_contours,
            contours, // a vector of contours
            CV_RETR_EXTERNAL, // retrieve the external contours
            CV_CHAIN_APPROX_NONE); // all pixels of each contours
    
    // Draw blue contours on a white image
    cv::Mat result;
    img_threshold.copyTo(result);
    cv::cvtColor(result, result, CV_GRAY2RGB);
    cv::drawContours(result,contours,
            -1, // draw all contours
            cv::Scalar(255,0,0), // in blue
            1); // with a thickness of 1

    //Start to iterate to each contour founded
    vector<vector<cv::Point> >::iterator itc= contours.begin();
    
    //Remove patch that are no inside limits of aspect ratio and area.    
    while (itc!=contours.end()) {
        
        //Create bounding rect of object
        cv::Rect mr= cv::boundingRect(cv::Mat(*itc));
        cv::rectangle(result, mr, cv::Scalar(0,255,0));
        //Crop image
        cv::Mat auxRoi(img_threshold, mr);
        if(verifySizes(auxRoi)){
            auxRoi=preprocessChar(auxRoi);
            output.push_back(CharSegment(auxRoi, mr));
            cv::rectangle(result, mr, Scalar(0,125,255));
        }
        ++itc;
    }
    if(DEBUG)
        cout << "Num chars: " << output.size() << "\n";
    


    if(DEBUG)
        cv::imshow("SEgmented Chars", result);
    return output;
}

cv::Mat OCR::ProjectedHistogram(cv::Mat img, int t)
{
    int sz=(t)?img.rows:img.cols;
    cv::Mat mhist=cv::Mat::zeros(1,sz,CV_32F);

    for(int j=0; j<sz; j++){
        cv::Mat data=(t)?img.row(j):img.col(j);
        mhist.at<float>(j)=countNonZero(data);
    }

    //Normalize histogram
    double min, max;
    cv::minMaxLoc(mhist, &min, &max);
    
    if(max>0)
        mhist.convertTo(mhist,-1 , 1.0f/max, 0);

    return mhist;
}

cv::Mat OCR::getVisualHistogram(cv::Mat *hist, int type)
{

    int size=100;
    cv::Mat imHist;


    if(type==HORIZONTAL){
        imHist.create(cv::Size(size,hist->cols), CV_8UC3);
    }else{
        imHist.create(cv::Size(hist->cols, size), CV_8UC3);
    }

    imHist=cv::Scalar(55,55,55);

    for(int i=0;i<hist->cols;i++){
        float value=hist->at<float>(i);
        int maxval=(int)(value*size);

        cv::Point pt1;
        cv::Point pt2, pt3, pt4;

        if(type==HORIZONTAL){
            pt1.x=pt3.x=0;
            pt2.x=pt4.x=maxval;
            pt1.y=pt2.y=i;
            pt3.y=pt4.y=i+1;

            cv::line(imHist, pt1, pt2, CV_RGB(220,220,220),1,8,0);
            cv::line(imHist, pt3, pt4, CV_RGB(34,34,34),1,8,0);

            pt3.y=pt4.y=i+2;
            cv::line(imHist, pt3, pt4, CV_RGB(44,44,44),1,8,0);
            pt3.y=pt4.y=i+3;
            cv::line(imHist, pt3, pt4, CV_RGB(50,50,50),1,8,0);
        }else{

                        pt1.x=pt2.x=i;
                        pt3.x=pt4.x=i+1;
                        pt1.y=pt3.y=100;
                        pt2.y=pt4.y=100-maxval;


            cv::line(imHist, pt1, pt2, CV_RGB(220,220,220),1,8,0);
            cv::line(imHist, pt3, pt4, CV_RGB(34,34,34),1,8,0);

            pt3.x=pt4.x=i+2;
            cv::line(imHist, pt3, pt4, CV_RGB(44,44,44),1,8,0);
            pt3.x=pt4.x=i+3;
            cv::line(imHist, pt3, pt4, CV_RGB(50,50,50),1,8,0);

        }




    }

    return imHist ;
}

void OCR::drawVisualFeatures(cv::Mat character, cv::Mat hhist, cv::Mat vhist, cv::Mat lowData){
    cv::Mat img(121, 121, CV_8UC3, cv::Scalar(0,0,0));
    cv::Mat ch;
    cv::Mat ld;
    
    cv::cvtColor(character, ch, CV_GRAY2RGB);

    cv::resize(lowData, ld, Size(100, 100), 0, 0, INTER_NEAREST );
    cv::cvtColor(ld,ld,CV_GRAY2RGB);

    cv::Mat hh=getVisualHistogram(&hhist, HORIZONTAL);
    cv::Mat hv=getVisualHistogram(&vhist, VERTICAL);

    cv::Mat subImg=img(cv::Rect(0,101,20,20));
    ch.copyTo(subImg);

    subImg=img(Rect(21,101,100,20));
    hh.copyTo(subImg);

    subImg=img(Rect(0,0,20,100));
    hv.copyTo(subImg);

    subImg=img(Rect(21,0,100,100));
    ld.copyTo(subImg);

    cv::line(img, Point(0,100), Point(121,100), Scalar(0,0,255));
    cv::line(img, Point(20,0), Point(20,121), Scalar(0,0,255));

    cv::imshow("Visual Features", img);

    cv::waitKey(0);
}

cv::Mat OCR::features(cv::Mat in, int sizeData){
    //Histogram features
    cv::Mat vhist=ProjectedHistogram(in,VERTICAL);
    cv::Mat hhist=ProjectedHistogram(in,HORIZONTAL);
    
    //Low data feature
    cv::Mat lowData;
    cv::resize(in, lowData, cv::Size(sizeData, sizeData) );

    if(DEBUG)
        drawVisualFeatures(in, hhist, vhist, lowData);
    

    
    //Last 10 is the number of moments components
    int numCols=vhist.cols+hhist.cols+lowData.cols*lowData.cols;
    
    cv::Mat out=Mat::zeros(1,numCols,CV_32F);
    //Asign values to feature
    int j=0;
    for(int i=0; i<vhist.cols; i++)
    {
        out.at<float>(j)=vhist.at<float>(i);
        j++;
    }
    for(int i=0; i<hhist.cols; i++)
    {
        out.at<float>(j)=hhist.at<float>(i);
        j++;
    }
    for(int x=0; x<lowData.cols; x++)
    {
        for(int y=0; y<lowData.rows; y++){
            out.at<float>(j)=(float)lowData.at<unsigned char>(x,y);
            j++;
        }
    }
    if(DEBUG)
        cout << out << "\n===========================================\n";
    return out;
}

void OCR::train(cv::Mat TrainData, cv::Mat classes, int nlayers){
    cv::Mat layers(1,3,CV_32SC1);
    layers.at<int>(0)= TrainData.cols;
    layers.at<int>(1)= nlayers;
    layers.at<int>(2)= numCharacters;
    ann->setLayerSizes(layers);
    ann->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, 0.001, 0.1);
    ann->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM, 1.0, 1.0);
    ann->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 10000,0.0001));

    //Prepare trainClases
    //Create a mat with n trained data by m classes
    Mat trainClasses;
    trainClasses.create( TrainData.rows, numCharacters, CV_32FC1 );
    for( int i = 0; i <  trainClasses.rows; i++ )
    {
        for( int k = 0; k < trainClasses.cols; k++ )
        {
            //If class of data i is same than a k class
            if( k == classes.at<int>(i) )
                trainClasses.at<float>(i,k) = 1;
            else
                trainClasses.at<float>(i,k) = 0;
        }
    }
    Mat weights( 1, TrainData.rows, CV_32FC1, Scalar::all(1) );
    cv::Ptr<cv::ml::TrainData> trainData = cv::ml::TrainData::create(TrainData , cv::ml::ROW_SAMPLE , trainClasses);
    
    //Learn classifier
    ann->train(trainData);
    trained=true;
}

int OCR::classify(Mat f){
    int result=-1;
    Mat output(1, numCharacters, CV_32FC1);
    ann->predict(f, output);
    Point maxLoc;
    double maxVal;
    cv::minMaxLoc(output, 0, &maxVal, 0, &maxLoc);
    //We need know where in output is the max val, the x (cols) is the class.

    return maxLoc.x;
}

int OCR::classifyKnn(Mat f)
{
    cv::Mat nearests(1 , K , CV_32FC1);
    cv::Mat result(f.size() , CV_32FC1);
    int response = (int)knnClassifier->findNearest( f, K , result , nearests);
    return response;
}
void OCR::trainKnn(Mat trainSamples, Mat trainClasses, int k){
    K=k;
    knnClassifier->setDefaultK(K);
    knnClassifier->setIsClassifier(true);
    cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(trainSamples , cv::ml::ROW_SAMPLE , trainClasses);
    // learn classifier
    // knnClassifier->train( trainSamples, trainClasses, Mat(), false, K );
    knnClassifier->train(tData);
}

string OCR::run(Plate *input){
    
    //Segment chars of plate
    vector<CharSegment> segments=segment(*input);

    for(int i=0; i<segments.size(); i++){
        //Preprocess each char for all images have same sizes
        Mat ch=preprocessChar(segments[i].img);
        if(saveSegments){
            stringstream ss(stringstream::in | stringstream::out);
            ss << "tmpChars/" << filename << "_" << i << ".jpg";
            imwrite(ss.str(),ch);
        }
        //For each segment Extract Features
        Mat f=features(ch,15);
        //For each segment feature Classify
        int character=classify(f);
        input->chars.push_back(strCharacters[character]);
        input->charsPos.push_back(segments[i].pos);
    }
    return "-";//input->str();
}


