#include "FaceCluster.h"
#include "face_cluster_types.h"

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include <random>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "face_feature/feature.hpp"



using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::stdcxx;

void split(std::string& s, std::string& delim,std::vector< std::string >& ret) {
    size_t last = 0;
    size_t index=s.find_first_of(delim,last);
    while (index!=std::string::npos) {
        //skip continous space
        if (index > last) {
            ret.push_back(s.substr(last,index-last));
        }
        last=index+1;
        index=s.find_first_of(delim,last);
    }

    if (index-last>0) {
        ret.push_back(s.substr(last,index-last));
    }
}

void search_files(string root, vector<string> &files){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (root.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
                continue;
            files.push_back(root + "/" + string(ent->d_name));

        }
        closedir (dir);
    }
}

int get_images(string &root, std::ifstream &facedir_file, vector<string>& imgs_path) {
    string line;
    string delim=" ";
    vector<string> dirs;
    imgs_path.clear();
    if (std::getline(facedir_file, line)) {
        split(line, delim, dirs);
        // cout <<"dirs size: "<<dirs.size()<<endl;
        for (vector<string>::iterator iter = dirs.begin(); iter != dirs.end(); iter++) {
            // cout<<"sub dir path: " << root + "/" + *iter << endl;
            search_files(root + "/" + *iter, imgs_path);
        }
        return true;
    } else {
        return false;
    }
}

int get_images(string &root, std::ifstream &facedir_file, pair<unsigned long, vector<unsigned long>>&labels, vector<string>& imgs_path) {
    string line;
    string delim=" ";
    vector<string> dirs;
    int prev_size = 0;
    int idx = 0;
    labels.second.clear();
    imgs_path.clear();
    if (std::getline(facedir_file, line)) {
        split(line, delim, dirs);
        // cout <<"dirs size: "<<dirs.size()<<endl;
        labels.first = dirs.size();
        for (vector<string>::iterator iter = dirs.begin(); iter != dirs.end(); iter++) {
            // cout<<"sub dir path: " << root + "/" + *iter << endl;
            search_files(root + "/" + *iter, imgs_path);
            for (size_t i = 0; i < imgs_path.size() - prev_size; i++) {
                labels.second.push_back(idx);
            }
            idx++;
            prev_size = imgs_path.size();
        }
        return true;
    } else {
        return false;
    }
}

void montage(vector<string>& imgs_path, string& output_dir, vector<int>& labels, string sub_dir, int cluster_num)
{
    // make sub dir
    string full_path = output_dir + "/" + sub_dir;
    cout << "sub dir: "<< full_path << std::endl;
    if (opendir(full_path.c_str()) == NULL) {
        mkdir(full_path.c_str(), 0766);
    }

    vector<int> imgs_num;
    // count the number of images of each label
    for (int i = 0; i < cluster_num; i++) {
        int num = 0;
        for(size_t j = 0; j < labels.size(); j++) {
            if (labels[j] == i) {
                num++;
            }
        }
        imgs_num.push_back(num);
    }

    int sub_h = 128;
    int sub_w = 128;
    int sub_c = 3;
    int cols = 8;
    int rows = 0;
    int m = 0;
    string path;

    rows = imgs_path.size() / cols;
    if (imgs_path.size() % cols) {
        rows += 1;
    }
    cout <<"rows: " << rows << "cols: " << cols << std::endl;
    // create a black canvas
    cv::Mat canvas(sub_h * rows, sub_w * cols, CV_8UC3, cv::Scalar(0,0,0));
    for (size_t i = 0; i < imgs_path.size(); i++) {
        path = imgs_path[i];
        cv::Mat img = cv::imread(path);
        cv::Mat img_resize;
        cv::resize(img, img_resize, cv::Size(sub_w, sub_h));
        int row = (i / cols) * sub_h;
        int col = (i % cols) * sub_w;
        cout <<"row: " << row << "col: " << col << std::endl;
        cv::Mat roi = canvas(cv::Rect(col, row, sub_w, sub_h));
        img_resize.copyTo(roi);
    }
    cout << "save montage" << std::endl;
    // save montage
    cv::imwrite(full_path + "/" + "montage.jpg", canvas);

    for (size_t i = 0; i < imgs_num.size(); i++) {
        int num = imgs_num[i];
        rows = num / cols;
        if (num % cols) {
            rows += 1;
        }
        // create a black canvas
        cv::Mat canvas_sub(sub_h * rows, sub_w * cols, CV_8UC3, cv::Scalar(0,0,0));
        int label = i;
        m = 0;
        // paste sub image on the canvas
        for (size_t j = 0; j < labels.size(); j++) {
            if (label == labels[j]) {
                path = imgs_path[j];
                cv::Mat img = cv::imread(path);
                cv::Mat img_resize;
                cv::resize(img, img_resize, cv::Size(sub_w, sub_h));
                int row = (m / cols) * sub_h;
                int col = (m % cols) * sub_w;
                cout <<"row: " << row << "col: " << col << std::endl;
                cv::Mat roi = canvas_sub(cv::Rect(col, row, sub_w, sub_h));
                img_resize.copyTo(roi);
                m++;
            }
        }
        cout << "save cluster: " << label << std::endl;
        // save canvas
        cv::imwrite(full_path + "/" + std::to_string(label) + ".jpg", canvas_sub);
    }
}

int main(int argc, char **argv){

    if (argc != 4) {
        cout <<"client facedir_list.txt root_dir output_dir" << endl;
        return 0;
    }

    string prototxt = "/home/lqy/workshop/image_utils/face_recognition/mnasnet/mnas0.5-long-softmax-retina.prototxt";
    string model = "/home/lqy/workshop/image_utils/face_recognition/mnasnet/mnas0.5-long-combine_merge_retina-153-lfw0.996233.caffemodel";
    class FeatureExtraction feature_extraction(prototxt, model, 128, 128, true);

    stdcxx::shared_ptr<TSocket> socket(new TSocket("localhost",9091));
    //TSimpleServer, client use TBufferedTransport
    //stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //TNonblockingServer, client use TFramedTransport
    stdcxx::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    ifstream facedir_list(argv[1]);
    string root(argv[2]);
    string output_dir(argv[3]);

    if(opendir(root.c_str()) == NULL) {
        cout<<"root_dir :"<< argv[2] << " doesn't exit"<<endl;
        return 0;
    }
    if (opendir(output_dir.c_str()) == NULL) {
        mkdir(output_dir.c_str(), 0766);
    }

    vector<string> imgs_path;
    int lines= 0;
    vector<float> descriptor;
    vector<double> descriptor_d;

    vector<vector<double>> face_descriptors;
    pair<unsigned long, vector<unsigned long>> labels;
    while(get_images(root, facedir_list, labels, imgs_path)) {
        #if 0
        for (size_t i = 0; i < face_descriptors.size(); i++) {
            cout << "___desc no."<< i << "___" << endl;
            for (size_t j = 0; j < face_descriptors[i].size(); j++) {
                cout<< face_descriptors[i][j] << endl;
            }
        }
        #endif
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(imgs_path.begin(), imgs_path.end(), std::default_random_engine(seed));


        face_descriptors.clear();
        cout <<"@line: "<<lines<<"______"<<endl;
        cout <<"imgs num: "<<imgs_path.size()<<endl;
        cout <<"labels num: "<<labels.second.size()<<endl;
        assert(imgs_path.size() == labels.second.size());
        for (size_t i = 0; i < imgs_path.size(); i++) {
            // cout<<"img path: "<<imgs_path[i]<<endl;
            cv::Mat img = cv::imread(imgs_path[i]);
            if (img.data != nullptr) {
                descriptor_d.clear();
                descriptor = feature_extraction.Extract(img);
                for (size_t j = 0; j < descriptor.size(); j++) {
                    descriptor_d.push_back((double)descriptor[j]);
                }
                #if 0
                for (size_t j = 0; j < descriptor_d.size(); j++) {
                    cout<<descriptor_d[j]<<endl;
                }
                #endif
                face_descriptors.emplace_back(descriptor_d);

            }
        }

        transport->open();
        FaceClusterClient client(protocol);
        ClusterInput input;
        ClusterResult result;
        input.__set_featurelist(face_descriptors);
        input.__set_uid(lines);
        input.__set_did(std::to_string(lines));
        #if 0
        for (size_t i = 0; i < input.featurelist.size(); i++) {
            cout << "___desc no."<< i << "___" << endl;
            for (size_t j = 0; j < input.featurelist[i].size(); j++) {
                cout << input.featurelist[i][j] << endl;
            }
        }
        #endif

        cout << "Feat num: " << input.featurelist.size() << std::endl;
        client.Cluster(result, input);
        transport->close();
        cout << "Cluster num: " << result.cluster_num << std::endl;
        cout << "Cluster labels: ";
        for (unsigned int i = 0; i < result.labels.size(); i++) {
            cout << result.labels[i] << " ";
            if ((i+1)%8==0) {
                cout << std::endl;
            }
        }
        cout << std::endl;
        montage(imgs_path, output_dir, result.labels, std::to_string(lines), result.cluster_num);
        lines++;

    }

    return 0;
}

