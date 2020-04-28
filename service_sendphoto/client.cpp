#include "Photo.h"
#include <transport/TSocket.h>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


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
    char *photo_name = argv[1];
    stdcxx::shared_ptr<TSocket> socket(new TSocket("localhost",9090));
    stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    transport->open();
    PhotoClient client(protocol);

    FILE * f = fopen(photo_name,"rb");
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    char * buf = (char*)malloc(size);
    if (!buf) {
        fprintf(stderr, "mem malloc fail\n");
        return 0;
    }
    memset(buf, 0, size);
    fseek(f,0,SEEK_SET);
    int rbytes = fread(buf, sizeof(char), size, f);
    assert(rbytes == size);
    fclose(f);
    bool ret = client.SendPhoto(string(buf, size), size);
    cout << ret << endl;
    transport->close();
    free(buf);

    return 0;
}

