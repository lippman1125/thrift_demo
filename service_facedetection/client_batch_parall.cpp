#include "FaceDetect.h"
#include "face_detection_types.h"

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
#include <sys/time.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>



using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::stdcxx;

#define THREAD_NUM_LIMITED  (10)

string& replace_all(string& str, const string& old_value, const string& new_value)
{
    for(string::size_type pos(0); pos != string::npos; pos += new_value.length()){
        if((pos = str.find(old_value,pos)) != string::npos) {
            str.replace(pos, old_value.length(), new_value);
        } else {
            break;
        }
    }
    return str;
}

long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int rpc_worker(vector<string>& flist,
                    const string& input_dir,
                    const string& output_dir,
                    mutex& mtx,
                    int idx_start,
                    int idx_end)
{
    stdcxx::shared_ptr<TSocket> socket(new TSocket("localhost",9090));
    //TSimpleServer, client use TBufferedTransport
    //stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    //TNonblockingServer, client use TFramedTransport
    stdcxx::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    string filename;
    ostringstream ss;

    for (int i = idx_start; i < idx_end; i++) {
        string filename = flist[i];
        string prefix = filename;
        string image_in_file = input_dir + "/" + filename + ".jpg";
        string image_out_file = output_dir + "/" + replace_all(prefix, "/", "_") + ".jpg";
        ofstream outfile(output_dir + "/" + prefix + ".txt", ios::out);

        mtx.lock();
        cout<<"No." << i << std::endl;
        cout<<"Detect file: " << image_in_file << std::endl;
        mtx.unlock();
        FILE * f = fopen(image_in_file.c_str(),"rb");
        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        char * img_buf = (char*)malloc(size);
        if (!img_buf) {
            fprintf(stderr, "mem malloc fail\n");
            return 0;
        }
        memset(img_buf, 0, size);
        fseek(f,0,SEEK_SET);
        int rbytes = fread(img_buf, sizeof(char), size, f);
        assert(rbytes == size);
        fclose(f);

        // create client and perform RPC
        transport->open();
        FaceDetectClient client(protocol);
        DetectionInput input;
        DetectionResult result;
        input.__set_img(string((const char*)img_buf, size));
        client.Detect(result, input);
        transport->close();
        free(img_buf);

        // draw bbox on image
        cv::Mat img = cv::imread(image_in_file);
        vector<vector<double>> detections = result.detections;
        /* Print the detection results. */
        for (unsigned int i = 0; i < detections.size(); ++i) {
            const vector<double>& d = detections[i];
            // Detection format: [image_id, label, score, xmin, ymin, xmax, ymax].
            // CHECK_EQ(d.size(), 7);
            const double score = d[2];
            outfile << filename << std::endl;
            outfile << static_cast<int>(d[1]) << std::endl;
            outfile << static_cast<int>(d[3]) << " ";
            outfile << static_cast<int>(d[4]) << " ";
            outfile << static_cast<int>(d[5]) << " ";
            outfile << static_cast<int>(d[6]) << " ";
            outfile << score << std::endl;
            cv::Rect box(static_cast<int>(d[3]), static_cast<int>(d[4]),
                (static_cast<int>(d[5]) - static_cast<int>(d[3])),
                (static_cast<int>(d[6]) - static_cast<int>(d[4])));
            cv::rectangle(img, box, cv::Scalar(0, 0, 255), 2);
            ss.str("");
            ss << score;
            cv::String conf(ss.str());
            cv::String label = static_cast<int>(d[1]) + ": " + conf;
            cv::putText(img, label, cv::Point(static_cast<int>(d[3]), static_cast<int>(d[4])),
                  0, 0.5, cv::Scalar(0,0,255));

        }
        cv::imwrite(image_out_file, img);
        mtx.lock();
        cout<< "Save file: " << image_out_file << std::endl;
        cout<< std::endl;
        mtx.unlock();
    }

    return 0;

}

int main(int argc, char **argv){

    if (argc != 5) {
        fprintf(stderr, "please input images_list.txt input_dir output_dir thread_num");
        return 0;
    }

    ifstream flist(argv[1]);
    if (!flist) {
       cout<<"file:"<< argv[1] << " doesn't exit"<<endl;
       return 0;
    }
    const string input_dir(argv[2]);
    const string output_dir(argv[3]);

    int thread_num = atoi(argv[4]);
    if(thread_num < 0) {
        thread_num = 1;
    } else if (thread_num > THREAD_NUM_LIMITED) {
        thread_num = THREAD_NUM_LIMITED;
    }

    vector<thread> threads;
    mutex mtx;

    if(opendir(input_dir.c_str()) == NULL) {
        cout<<"input_dir :"<< argv[2] << " doesn't exit"<<endl;
        return 0;

    }
    if (opendir(output_dir.c_str()) == NULL) {
        mkdir(output_dir.c_str(), 0766);
    }

    vector<string> image_list;
    string filename;
    while (std::getline(flist, filename)) {
        image_list.push_back(filename);
    }
    int file_num = image_list.size();
    cout << "Total file num: "<< file_num << std::endl;

    long start_ts = getCurrentTime();
    // split file list and create thread work
    int duration = file_num / thread_num;
    int remain = file_num % thread_num;
    int idx_start;
    int idx_end;
    for (int i = 0; i < thread_num; i++) {
        idx_start = i*duration;
        if (i == thread_num -1) {
            idx_end = file_num;
        } else {
            idx_end = (i+1)*duration - 1;
        }
        threads.emplace_back(thread(rpc_worker,
                                 std::ref(image_list),
                                 std::ref(input_dir),
                                 std::ref(output_dir),
                                 std::ref(mtx),
                                 idx_start,
                                 idx_end));
    }

    // wait thread ends
    for (int i = 0; i < thread_num; i++) {
        threads[i].join();
    }
    long end_ts = getCurrentTime();
    std::cout <<"Total time cost: " << (end_ts - start_ts)/1000 <<" [s]"<< std::endl;

    return 0;
}
