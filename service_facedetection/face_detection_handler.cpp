#include <gflags/gflags.h>
#include <glog/logging.h>
#include "face_detection_handler.hpp"
#include "face_detection/face_detection.hpp"
#include "gen-cpp/face_detection_types.h"


// Currently one sense-time algorithm instance needs about 1GB memory.
// This pool size is used to control the total memory.
DEFINE_int32(algo_instance_pool_size,
                     30,
                     "The size of motion analyse algorithm instance pool.");


using namespace std;
FaceDetectHandler::FaceDetectHandler()
{
    algo_wrapper_count_ = 0;
    algo_wrapper_pool_semaphore_.reset(new Semaphore(FLAGS_algo_instance_pool_size));
};

FaceDetectHandler::~FaceDetectHandler()
{

}

void FaceDetectHandler::Detect(DetectionResult& _return, const DetectionInput& input) {
    #if 0
    // Your implementation goes here
    printf("Detect\n");
    vector<vector<float> > detections;
    printf("Image size = %lu\n", input.img.size());
    vector<uchar> data(input.img.begin(), input.img.end());
    cv::Mat img = cv::imdecode(data, CV_LOAD_IMAGE_COLOR);
    detections = fd_ptr_->Detect(img);
    for (unsigned int i = 0; i < detections.size(); i++) {
        vector<double> detection;
        for (unsigned int j = 0; j < detections[i].size(); j++) {
            detection.push_back(detections[i][j]);
        }
        _return.detections.push_back(detection);
    }
    #endif

    printf("Detect\n");
    vector<vector<float> > detections;
    printf("Image size = %lu\n", input.img.size());
    vector<uchar> data(input.img.begin(), input.img.end());
    cv::Mat img = cv::imdecode(data, CV_LOAD_IMAGE_COLOR);

    // Gets an AlgoWrapper instance from the pool.
    bool busy = false;
    AlgoWrapper* algo_wrapper = GetAlgoWrapper(busy);
    LOG(INFO) << "algo busy:" << busy;
    if (busy) {
        return;
    }
    CHECK_NOTNULL(algo_wrapper);

    detections = algo_wrapper->AlgoImpl(img);
    // just convert float type to double type
    for (unsigned int i = 0; i < detections.size(); i++) {
        vector<double> detection;
        for (unsigned int j = 0; j < detections[i].size(); j++) {
            detection.push_back(detections[i][j]);
        }
        _return.detections.push_back(detection);
    }
    LOG(INFO) << "Successfully run face detection algorithm. detection num:" << detections.size();

    // Must return the AlgoWrapper instance to the pool.
    RecycleAlgoWrapper(algo_wrapper);

}

AlgoWrapper* FaceDetectHandler::GetAlgoWrapper(bool &busy)
{
    AlgoWrapper* algo_wrapper = NULL;

    LOG(INFO) << "algo_wrapper_pool_.size():" << algo_wrapper_pool_.size();
    if (algo_wrapper_count_ == FLAGS_algo_instance_pool_size && algo_wrapper_pool_.size() <= 1) {
        busy = true;
        return NULL;
    }
    // Can't exceed the limit.
    algo_wrapper_pool_semaphore_->Wait();
    {
        // Locks the pool.
        std::lock_guard<std::mutex> lock(algo_wrapper_pool_mutex_);
        // if algo wrapper pool is not empty, get an algo wrapper
        if (!algo_wrapper_pool_.empty()) {
            algo_wrapper = algo_wrapper_pool_[algo_wrapper_pool_.size() - 1];
            algo_wrapper_pool_.pop_back();
        }
    }

    if (algo_wrapper != NULL) {
        LOG(INFO) << "Got an AlgoWrapper instance from the pool.";
        return algo_wrapper;
    } else {
        // Instantiates a new AlgoWrapper.
        algo_wrapper = new AlgoWrapper();
        {
          std::lock_guard<std::mutex> lock(algo_wrapper_pool_mutex_);
          ++algo_wrapper_count_;
          LOG(INFO) << "Successfully initialized an AlgoWrapper instance. Total count:"
                     << algo_wrapper_count_;
        }
        return algo_wrapper;
    }
}

void FaceDetectHandler::RecycleAlgoWrapper(AlgoWrapper* algo_wrapper)
{
    CHECK_NOTNULL(algo_wrapper);
    {
        std::lock_guard<std::mutex> lock(algo_wrapper_pool_mutex_);
        algo_wrapper_pool_.push_back(algo_wrapper);
    }
    algo_wrapper_pool_semaphore_->Signal();
    LOG(INFO) << "Recycled an AlgoWrapper instance.";
}

