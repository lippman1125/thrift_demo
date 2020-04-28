#include "FaceDetect.h"
#include "face_detection_types.h"

#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::stdcxx;

int main(int argc, char **argv){

    if (argc != 2) {
        fprintf(stderr, "please input filename");
        return 0;
    }

    string path = string(argv[1]);
    int pos = path.find_last_of('/');
    string basename(path.substr(pos+1));
    pos = basename.find_last_of('.');
    string prefix(basename.substr(0, pos));

    const string out_file = prefix + "_rpc_out.txt";
    // Set the output mode.
    std::streambuf* buf = std::cout.rdbuf();
    std::ofstream outfile;
    if (!out_file.empty()) {
        outfile.open(out_file.c_str());
        if (outfile.good()) {
            buf = outfile.rdbuf();
        }
    }
    std::ostream out(buf);
    ostringstream ss;

    stdcxx::shared_ptr<TSocket> socket(new TSocket("localhost",9090));
    stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    transport->open();
    FaceDetectClient client(protocol);

    FILE * f = fopen(argv[1],"rb");
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

    DetectionInput input;
    DetectionResult result;
    input.__set_img(string((const char*)img_buf, size));
    client.Detect(result, input);
    transport->close();
    free(img_buf);

    cv::Mat img = cv::imread(path);
    vector<vector<double>> detections = result.detections;
    /* Print the detection results. */
    for (unsigned int i = 0; i < detections.size(); ++i) {
        const vector<double>& d = detections[i];
        // Detection format: [image_id, label, score, xmin, ymin, xmax, ymax].
        // CHECK_EQ(d.size(), 7);
        const double score = d[2];
        out << prefix << " ";
        out << static_cast<int>(d[1]) << " ";
        out << score << " ";
        out << static_cast<int>(d[3]) << " ";
        out << static_cast<int>(d[4]) << " ";
        out << static_cast<int>(d[5]) << " ";
        out << static_cast<int>(d[6]) << std::endl;
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
    cv::imwrite(prefix + "_rpc_out.jpg", img);

    return 0;
}

