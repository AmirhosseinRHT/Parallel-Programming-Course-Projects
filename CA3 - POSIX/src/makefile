CXX = g++ -std=c++17
CXXFLAGS = -Wall -pthread -g -w

TARGET = program  

SRCS = main.cpp queue.cpp customer.cpp  baker.cpp order.cpp oven.cpp
OBJS = $(SRCS:.cpp=.o)  

.PHONY: all clean  

all: $(TARGET)  
	@rm -f $(OBJS)  # Automatically remove .o files after building  

$(TARGET): $(OBJS)  
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)  

%.o: %.cpp  
	$(CXX) $(CXXFLAGS) -c $< -o $@  

clean:  
	rm -f $(OBJS) $(TARGET)