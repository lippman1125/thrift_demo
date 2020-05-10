#include <string>
#include <vector>
#include <iostream>
#include <sys/time.h>
#include "algo_wrapper.hpp"

using namespace std;

AlgoWrapper::AlgoWrapper() : fd_(NULL)
{
    const string prototxt = "/home/lqy/workshop/thrift_demo/service_facedetection/face_detection/yufacedetectnet-open-v1.prototxt";
    const string model = "/home/lqy/workshop/thrift_demo/service_facedetection/face_detection/yufacedetectnet-open-v1.caffemodel";
    vector<float> mean;
    mean.push_back(103.94);
    mean.push_back(116.78);
    mean.push_back(123.68);
    float thresh = 0.9;

    fd_ = new FaceDetection(prototxt, model, mean, thresh);
}

AlgoWrapper::~AlgoWrapper()
{
    if (fd_ != NULL) {
        delete fd_;
        fd_ = NULL;
    }
}

long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

vector<vector<float>> AlgoWrapper::AlgoImpl(const cv::Mat &img)
{
    vector<vector<float>> detections;
    if (fd_) {
        long start_ts = getCurrentTime();
        detections = fd_->Detect(img);
        long end_ts = getCurrentTime();
        std::cout <<__FUNCTION__<< "face detection time: " << end_ts - start_ts << std::endl;
    }
    return detections;
}
