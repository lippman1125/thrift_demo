#ifndef _FACE_DETECTION_HANDLER_H_
#define _FACE_DETECTION_HANDLER_H_

#include "gen-cpp/FaceDetect.h"
#include "gen-cpp/face_detection_types.h"
#include "face_detection/face_detection.hpp"

using namespace std;

class FaceDetectHandler : virtual public FaceDetectIf {
public:
    FaceDetectHandler();
    void Detect(DetectionResult& _return, const DetectionInput& input);
private:
    std::shared_ptr<FaceDetection> fd_ptr_;

};


#endif /*_FACE_DETECTION_HANDLER_H_*/
