OBJECT=./gen-cpp/FaceDetect.o \
       ./gen-cpp/face_detection_types.o \
       ./gen-cpp/face_detection_constants.o

FD_HDL=face_detection_handler.o
CXX=g++ -g -std=c++11

INCLUDES=-I./gen-cpp \
         -I/usr/local/include/thrift/

CAFFE_INSTALL_DIR=/home/lqy/workshop/caffe
CAFFE_LIBS=-L$(CAFFE_INSTALL_DIR)/build/lib/ -lcaffe \
           -lprotobuf \
           -lglog \
           -lboost_system \

LIBS=/usr/local/lib/libthrift.a
LIBS+=-lopencv_highgui   \
      -lopencv_core      \
      -lopencv_imgproc   \
      -lopencv_imgcodecs \


FD_LIBS=-L./face_detection/ -lface_detect

CXXFLAGS=${INCLUDES}
LDFLAGS=-pthread -ldl

all: client server

$(OBJECT):%.o:%.cpp
	$(CXX) ${CXXFLAGS} ${LDFLAGS} -c $< -o $@

$(FD_HDL):%.o:%.cpp
	$(CXX) ${CXXFLAGS} ${LDFLAGS} -c $< -o $@

client.o:%.o:%.cpp
	$(CXX) ${CXXFLAGS} ${LDFLAGS} -c $< -o $@

server.o:%.o:%.cpp
	$(CXX) ${CXXFLAGS} ${LDFLAGS} -c $< -o $@

client:$(OBJECT) client.o
	$(CXX) -o client ${OBJECT} client.o ${CXXFLAGS} ${LDFLAGS} ${LIBS}

server:$(OBJECT) server.o $(FD_HDL)
	$(CXX) -o server ${OBJECT} $(FD_HDL) server.o ${CXXFLAGS} ${LDFLAGS} ${LIBS} ${FD_LIBS} ${CAFFE_LIBS}

clean:
	$(RM) *.o ./gen-cpp/*.o  client  server
