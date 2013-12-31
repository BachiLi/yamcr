OIIO_INCLUDE = -I../oiio-build/include
OIIO_LIB = -L../oiio-build/lib -lOpenImageIO
EMBREE_INCLUDE = -I../embree-build/include
EMBREE_LIB = -L../embree-build/lib -lembree
BOOST_LIB = -lboost_system

CC       = gcc-4.7
CXX      = g++-4.7
INCLUDES = $(OIIO_INCLUDE) $(EMBREE_INCLUDE)
LIBS     = $(OIIO_LIB) $(EMBREE_LIB) $(BOOST_LIB)
CXXFLAGS = $(INCLUDES) -std=c++11
SRCS     = $(wildcard *.cpp)
OBJS     = $(addprefix objs/, $(subst /,_,$(SRCS:.cpp=.o)))
HDRS     = $(wildcard *.h)

default: dirs ../bin/yamcr
	
dirs:
	/bin/mkdir -p ../bin objs

objs/%.o: %.cpp $(HDRS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

../bin/yamcr: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	
.PHONY: clean

clean:
	rm -f objs/*.o 
