struct ClusterInput {
    1: required list<list<double>> featurelist;
    2: optional i64 uid;
    3: optional string did;
}
struct ClusterResult {
    1: required i32 cluster_num;
    2: required list<i32> labels;
    3: optional bool busy;
}
service FaceCluster {
    ClusterResult Cluster(1: ClusterInput input)
}
