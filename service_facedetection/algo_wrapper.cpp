#include <string>
#include <vector>
#include <iostream>
#include <sys/time.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include "algo_wrapper.hpp"

using namespace std;

DEFINE_string(fd_proto_path,
              "/home/lqy/workshop/thrift_demo/service_facedetection/face_detection/yufacedetectnet-open-v1.prototxt",
              "The prototxt path for face detection algorithm.");

DEFINE_string(fd_model_path,
              "/home/lqy/workshop/thrift_demo/service_facedetection/face_detection/yufacedetectnet-open-v1.caffemodel",
              "The model path for face detection algorithm.");


AlgoWrapper::AlgoWrapper() : fd_(NULL)
{
    const string prototxt = FLAGS_fd_proto_path;
    const string model = FLAGS_fd_model_path;
    vector<float> mean;
    mean.push_back(103.94);
    mean.push_back(116.78);
    mean.push_back(123.68);
    float thresh = 0.9;

    fd_ = new FaceDetection(prototxt, model, mean, thresh);
    LOG(INFO) <<__FUNCTION__<< "create a new face detection instance" << std::endl;
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
        LOG(INFO) <<__FUNCTION__<< "face detection time: " << end_ts - start_ts << std::endl;
    }
    return detections;
}
