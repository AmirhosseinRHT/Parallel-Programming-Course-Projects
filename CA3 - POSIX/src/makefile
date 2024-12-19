CXX = g++  
CXXFLAGS = -Wall -pthread  

TARGET = program  

SRCS = main.cpp queue.cpp customer.cpp  
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