#include <string>
#include <vector>
#include <iostream>
#include <sys/time.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include "algo_wrapper.hpp"

using namespace std;

DEFINE_double(cluster_thresh, 0.35, "Face cluster threshold.");

AlgoWrapper::AlgoWrapper() : fc_(NULL)
{
    fc_ = new FaceCluster((float)FLAGS_cluster_thresh);
    LOG(INFO) <<__FUNCTION__<< "create a new face cluster instance" << std::endl;
}

AlgoWrapper::~AlgoWrapper()
{
    if (fc_ != NULL) {
        delete fc_;
        fc_ = NULL;
    }
}

long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

std::pair<unsigned long, std::vector<unsigned long>> AlgoWrapper::AlgoImpl(std::vector<std::vector<float>>& descriptors)
{
    std::pair<unsigned long, std::vector<unsigned long>> cluster;
    if (fc_) {
        long start_ts = getCurrentTime();
        cluster = fc_->Cluster(descriptors);
        long end_ts = getCurrentTime();
        LOG(INFO) <<__FUNCTION__<< "face cluster time: " << end_ts - start_ts << std::endl;
    }

    return cluster;
}

