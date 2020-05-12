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
#include <dirent.h>



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

int get_descs(string &root, std::ifstream &facedir_file, vector<vector<double>>& face_descriptors) {
    string line;
    string line_desc;
    string delim=" ";
    vector<string> dirs;
    size_t size;
    vector<string> imgs_path;
    vector<double> face_descriptor;
    std::ifstream desc_file;

    face_descriptors.clear();
    if (std::getline(facedir_file, line)) {
        split(line, delim, dirs);
        cout <<"dirs size: "<<dirs.size()<<endl;
        for (vector<string>::iterator iter = dirs.begin(); iter != dirs.end(); iter++) {
            imgs_path.clear();
            cout<<"sub dir path: " << root + "/" + *iter << endl;
            search_files(root + "/" + *iter, imgs_path);
            for (size_t i = 0; i < imgs_path.size(); i++) {
                face_descriptor.clear();
                desc_file.open(imgs_path[i], ios::in);
                while(std::getline(desc_file, line_desc)) {
                    face_descriptor.push_back(std::stod(line_desc, &size));
                }
                face_descriptors.push_back(face_descriptor);
                desc_file.close();
            }
        }
        return true;
    } else {
        return false;
    }
}

int main(int argc, char **argv){

    if (argc != 3) {
        cout <<"client facedir_list.txt root_dir" << endl;
        return 0;
    }

    stdcxx::shared_ptr<TSocket> socket(new TSocket("localhost",9091));
    //TSimpleServer, client use TBufferedTransport
    //stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //TNonblockingServer, client use TFramedTransport
    stdcxx::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    ifstream facedir_list(argv[1]);
    string root(argv[2]);

    vector<vector<double>> face_descriptors;
    while(get_descs(root, facedir_list, face_descriptors)) {
        #if 0
        for (size_t i = 0; i < face_descriptors.size(); i++) {
            cout << "___desc no."<< i << "___" << endl;
            for (size_t j = 0; j < face_descriptors[i].size(); j++) {
                cout<< face_descriptors[i][j] << endl;
            }
        }
        #endif

        transport->open();
        FaceClusterClient client(protocol);
        ClusterInput input;
        ClusterResult result;
        input.__set_featurelist(face_descriptors);
        input.__set_uid(12345678);
        input.__set_did("87654321");
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
    }

    return 0;
}

