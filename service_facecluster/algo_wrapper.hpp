#ifndef _ALGO_WRAPPER_H_
#define _ALGO_WRAPPER_H_


#include <string>
#include <vector>
#include "face_cluster/cluster.hpp"

using namespace std;
// A wrapper class for face detection algorithms.
class AlgoWrapper {
public:
    AlgoWrapper();
    ~AlgoWrapper();
    std::pair<unsigned long, std::vector<unsigned long>> AlgoImpl(std::vector<std::vector<float>>& descriptors);

private:
    FaceCluster* fc_;
};


#endif  /*_ALGO_WRAPPER_H_*/
