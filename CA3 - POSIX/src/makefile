# Compiler settings
CXX = g++  
CXXFLAGS = -pthread -g 

TARGET = bakery.out  

SRCS = main.cpp utility.cpp queue.cpp customer.cpp baker.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean  

all: $(TARGET)  

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:  
	rm -f $(OBJS) $(TARGET)
