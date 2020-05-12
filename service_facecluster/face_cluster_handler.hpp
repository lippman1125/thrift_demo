#ifndef _FACE_CLUSTER_HANDLER_H_
#define _FACE_CLUSTER_HANDLER_H_

#include "gen-cpp/FaceCluster.h"
#include "gen-cpp/face_cluster_types.h"
#include "algo_wrapper.hpp"
#include "semaphore.hpp"

#include <memory>
#include <vector>
#include <mutex>

using namespace std;

class AlgoWrapper;
class Semaphore;

class FaceClusterHandler : virtual public FaceClusterIf {
public:
    FaceClusterHandler();
    virtual ~FaceClusterHandler();
    void Cluster(ClusterResult& _return, const ClusterInput& input);
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


#endif /*_FACE_CLUSTER_HANDLER_H_*/

