# makefile
CXX=g++
CXXFLAGS=-fPIC -g -Wall 
DEP_LDFLAGS= -L./protobuf/lib/
DEP_LDLIBS= -lprotobuf -lpthread
INCPATH= -I./protobuf/include/

all : 
	$(CXX) main.cpp Order.pb.cc -o main $(CXXFLAGS) $(INCPATH) $(DEP_LDFLAGS) $(DEP_LDLIBS)

clean:
	rm main
