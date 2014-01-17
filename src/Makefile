OIIO_INCLUDE = -I../oiio-build/include
OIIO_LIB = -L../oiio-build/lib -lOpenImageIO
EMBREE_INCLUDE = -I../embree-build/include
EMBREE_LIB = -L../embree-build/lib -lembree
BOOST_LIB = -lboost_system
PTHREAD_LIB = -lpthread
THREADPOOL_INCLUDE = -I../ThreadPool
EIGEN_INCLUDE = -I../Eigen

CC       = gcc-4.7
CXX      = g++-4.7
INCLUDES = $(OIIO_INCLUDE) $(EMBREE_INCLUDE) $(THREADPOOL_INCLUDE) $(EIGEN_INCLUDE) -I.
LIBS     = $(OIIO_LIB) $(EMBREE_LIB) $(BOOST_LIB) $(PTHREAD_LIB)
CXXFLAGS = $(INCLUDES) -std=c++11 -O3 -march=native
DEFS     = -DUSE_SSE
SRCS     = $(wildcard *.cpp) $(wildcard lights/*.cpp) $(wildcard shapes/*.cpp) $(wildcard samplers/*.cpp) $(wildcard bsdfs/*.cpp)
OBJS     = $(addprefix objs/, $(subst /,_,$(SRCS:.cpp=.o)))
HDRS     = $(wildcard *.h) $(wildcard */*.h)

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

../bin/yamcr: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	
.PHONY: clean

clean:
	rm -f objs/*.o 
