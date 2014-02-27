OIIO_INCLUDE = -I../oiio-build/include
OIIO_LIB = -L../oiio-build/lib -lOpenImageIO
EMBREE_INCLUDE = -I../embree-build/include
EMBREE_LIB = -L../embree-build/lib -lembree
BOOST_INCLUDE = -L../boost-algorithm/include
BOOST_LIB = -lboost_system
PTHREAD_LIB = -lpthread
THREADPOOL_INCLUDE = -I../ThreadPool
EIGEN_INCLUDE = -I../Eigen
PUGIXML_INCLUDE = -I../pugixml/src

CC          = gcc
CXX         = g++
INCLUDES    = $(OIIO_INCLUDE) $(EMBREE_INCLUDE) $(THREADPOOL_INCLUDE) $(BOOST_INCLUDE) $(EIGEN_INCLUDE) $(PUGIXML_INCLUDE) -I.
LIBS        = $(OIIO_LIB) $(EMBREE_LIB) $(BOOST_LIB) $(PTHREAD_LIB)
CXXFLAGS    = $(INCLUDES) -std=c++11 -O3 -march=native -Wall -g
DEFS        = -DUSE_SSE -DNDEBUG
SRCS        = $(wildcard *.cpp) $(wildcard lights/*.cpp) $(wildcard shapes/*.cpp) $(wildcard samplers/*.cpp) $(wildcard bsdfs/*.cpp) $(wildcard textures/*.cpp)
PUGIXML_SRC = ../pugixml/src/pugixml.cpp
OBJS        = $(addprefix objs/, $(subst /,_,$(SRCS:.cpp=.o)))
PUGIXML_OBJ = objs/pugixml.o
HDRS        = $(wildcard *.h) $(wildcard */*.h)

default: dirs ../bin/yamcr
	
dirs:
	/bin/mkdir -p ../bin objs

objs/%.o: %.cpp $(HDRS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(DEFS)

objs/lights_%.o: lights/%.cpp $(HDRS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(DEFS)

objs/shapes_%.o: shapes/%.cpp $(HDRS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(DEFS)

objs/samplers_%.o: samplers/%.cpp $(HDRS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(DEFS)

objs/bsdfs_%.o: bsdfs/%.cpp $(HDRS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(DEFS)

objs/textures_%.o: textures/%.cpp $(HDRS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(DEFS)

objs/pugixml.o: $(wildcard ../pugixml/src/*.*)
	$(CXX) -c -o $@ $(PUGIXML_SRC) $(CXXFLAGS) -DNDEBUG

../bin/yamcr: $(OBJS) $(PUGIXML_OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	
.PHONY: clean

clean:
	rm -f objs/*.o 
