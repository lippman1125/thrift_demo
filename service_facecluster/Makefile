THRIFT_SVR=./gen-cpp/FaceCluster.o \
           ./gen-cpp/face_cluster_types.o \
           ./gen-cpp/face_cluster_constants.o

ALGO_SRV=algo_wrapper.o \
         face_cluster_handler.o
CXX=g++ -g -std=c++11

INCLUDES=-I./gen-cpp \
         -I/usr/local/include/thrift/

CAFFE_INSTALL_DIR=/home/lqy/workshop/caffe
CAFFE_LIBS=-L$(CAFFE_INSTALL_DIR)/build/lib/ -lcaffe \
           -lprotobuf \
           -lglog \
           -lboost_system \

LIBS=/usr/local/lib/libthriftnb.a \
     /usr/local/lib/libthrift.a

OPENCV_LIBS=-lopencv_highgui   \
            -lopencv_core      \
            -lopencv_imgproc   \
            -lopencv_imgcodecs \

ALGO_LIBS=-L./face_cluster/ -lface_cluster
FEAT_LIBS=-L./face_feature/ -lface_feature

CLIENT_LIBS=${LIBS}
SERVER_LIBS=${ALGO_LIBS}  \
            ${LIBS} \
            -lgflags \
            -lglog \
            -levent \
            -ldlib

CXXFLAGS=${INCLUDES}
LDFLAGS=-pthread -ldl

all: client client_img server

${THRIFT_SVR}:%.o:%.cpp
	$(CXX) ${CXXFLAGS}  -c $< -o $@

$(ALGO_SRV):%.o:%.cpp
	$(CXX) ${CXXFLAGS}  -c $< -o $@


client.o:%.o:%.cpp
	$(CXX) ${CXXFLAGS}  -c $< -o $@

client_img.o:%.o:%.cpp
	$(CXX) ${CXXFLAGS}  -c $< -o $@

server.o:%.o:%.cpp
	$(CXX) ${CXXFLAGS}  -c $< -o $@

client:${THRIFT_SVR} client.o
	$(CXX) -o $@ $^ ${CXXFLAGS} ${LDFLAGS} ${CLIENT_LIBS}

client_img:${THRIFT_SVR} client_img.o
	$(CXX) -o $@ $^ ${CXXFLAGS} ${LDFLAGS} ${CLIENT_LIBS} ${FEAT_LIBS} ${OPENCV_LIBS} ${CAFFE_LIBS}

client_batch_parall:${THRIFT_SVR} client_batch_parall.o
	$(CXX) -o $@ $^ ${CXXFLAGS} ${LDFLAGS} ${CLIENT_LIBS}

server:${THRIFT_SVR} ${ALGO_SRV} server.o
	$(CXX) -o $@ $^ ${CXXFLAGS} ${LDFLAGS} ${SERVER_LIBS}

clean:
	$(RM) *.o ./gen-cpp/*.o  client client_img server

