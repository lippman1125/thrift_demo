OBJECT=./gen-cpp/Photo.o \
       ./gen-cpp/send_photo_types.o \
       ./gen-cpp/send_photo_constants.o

CPP=g++ -g -std=c++11

INCLUDES=-I./gen-cpp -I/usr/local/include/thrift/
LIBS=/usr/local/lib/libthrift.a

CFLAGS=${INCLUDES}
LFLAGS=-pthread -ldl

all: client server

$(OBJECT):%.o:%.cpp
	$(CPP) ${CFLAGS} ${LFLAGS} -c $< -o $@

client.o:%.o:%.cpp
	$(CPP) ${CFLAGS} ${LFLAGS} -c $< -o $@

server.o:%.o:%.cpp
	$(CPP) ${CFLAGS} ${LFLAGS} -c $< -o $@

client:$(OBJECT) client.o
	$(CPP) -o client ${OBJECT}  client.o ${CFLAGS} ${LFLAGS} ${LIBS}

server:$(OBJECT) server.o
	$(CPP) -o server ${OBJECT}  server.o ${CFLAGS} ${LFLAGS} ${LIBS}

clean:
	$(RM) *.o ./gen-cpp/*.o  client server
