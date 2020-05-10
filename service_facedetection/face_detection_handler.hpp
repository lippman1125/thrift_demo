#ifndef _FACE_DETECTION_HANDLER_H_
#define _FACE_DETECTION_HANDLER_H_

#include "gen-cpp/FaceDetect.h"
#include "gen-cpp/face_detection_types.h"
#include "algo_wrapper.hpp"
#include "semaphore.hpp"

#include <memory>
#include <vector>
#include <mutex>

using namespace std;

class AlgoWrapper;
class Semaphore;

class FaceDetectHandler : virtual public FaceDetectIf {
public:
    FaceDetectHandler();
    virtual ~FaceDetectHandler();
    void Detect(DetectionResult& _return, const DetectionInput& input);
private:
    // Gets an AlgoWrapper instance from algo_wrapper_pool_.
    // If no instance is available in the pool, this function makes a new
    // instance and returns it.
    AlgoWrapper* GetAlgoWrapper(bool &busy);
    // Puts the AlgoWrapper instance back into algo_wrapper_pool_.
    void RecycleAlgoWrapper(AlgoWrapper* algo_wrapper);

    std::vector<AlgoWrapper*> algo_wrapper_pool_;
    int algo_wrapper_count_;
    std::mutex algo_wrapper_pool_mutex_;
    std::unique_ptr<Semaphore> algo_wrapper_pool_semaphore_;
};


#endif /*_FACE_DETECTION_HANDLER_H_*/
