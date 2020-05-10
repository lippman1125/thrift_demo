#ifndef _ALGO_WRAPPER_H_
#define _ALGO_WRAPPER_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <string>
#include <vector>
#include "face_detection/face_detection.hpp"

using namespace std;
// A wrapper class for face detection algorithms.
class AlgoWrapper {
public:
    AlgoWrapper();
    ~AlgoWrapper();
    vector<vector<float>> AlgoImpl(const cv::Mat& img);

private:
    FaceDetection* fd_;
};


#endif  /*_ALGO_WRAPPER_H_*/
