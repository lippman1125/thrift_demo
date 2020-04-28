#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "face_detection_handler.hpp"
#include "face_detection/face_detection.hpp"
#include "gen-cpp/face_detection_types.h"

using namespace std;
FaceDetectHandler::FaceDetectHandler()
{
    const string prototxt = "/home/lqy/workshop/thrift_demo/service_facedetection/face_detection/yufacedetectnet-open-v1.prototxt";
    const string model = "/home/lqy/workshop/thrift_demo/service_facedetection/face_detection/yufacedetectnet-open-v1.caffemodel";
    vector<float> mean;
    mean.push_back(103.94);
    mean.push_back(116.78);
    mean.push_back(123.68);
    float thresh = 0.9;
    fd_ptr_ = std::make_shared<FaceDetection>(prototxt, model, mean, thresh);
};

void FaceDetectHandler::Detect(DetectionResult& _return, const DetectionInput& input) {
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
}


